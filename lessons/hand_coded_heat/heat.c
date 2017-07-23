#include <assert.h>
#include <float.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#ifdef HAVE_PAPI
#include "papi.h"
#endif

int const Nt_max = 5000;
int const Nx_max = 1000;

int savi = 0;
int save = 0;
char *alg = "ftcs";
char *prec = "double";
char *ic = "const(1)";
double alpha = 0.2;
double dt = 0.004;
double eps = 1e-6;
double dx = 0.1;
double bc0 = 0;
double bc1 = 1;

double *curr, *last, *res_history, *exact, *error_history;

int Nx = (int) (1/0.1+1.5);


float real_time, proc_time,mflops;
long long flpops;
float ireal_time, iproc_time, imflops;
long long iflpops;
int retval;

static void
initialize(void)
{
    curr = (double *) malloc(Nx_max * sizeof(double));
    last = (double *) malloc(Nx_max * sizeof(double));
    exact = (double *) malloc(Nx_max * sizeof(double));
    res_history = (double *) malloc(Nt_max * sizeof(double));
    error_history = (double *) malloc(Nt_max * sizeof(double));

#ifdef HAVE_PAPI
    if ((retval=PAPI_library_init(PAPI_VER_CURRENT)) != PAPI_VER_CURRENT)
    {   
        printf("Init failed with retval: %d\n", retval);
        exit(1);
    }
    if((retval=PAPI_flops(&ireal_time,&iproc_time,&iflpops,&imflops)) < PAPI_OK)
    {
        printf("Could not initialise PAPI_flops \n");
        printf("Your platform may not support floating point operation event.\n");
        printf("retval: %d\n", retval);
        exit(1);
    }
#endif
}

typedef struct {
    unsigned long size,resident,share,text,lib,data,dt;
} statm_t;

void read_off_memory_status(statm_t *result)
{
  unsigned long dummy;
  const char* statm_path = "/proc/self/statm";

  FILE *f = fopen(statm_path,"r");
  if(!f){
    perror(statm_path);
    abort();
  }
  if(7 != fscanf(f,"%ld %ld %ld %ld %ld %ld %ld",
    &result->size,&result->resident,&result->share,&result->text,&result->lib,&result->data,&result->dt))
  {
    perror(statm_path);
    abort();
  }
  fclose(f);
}

#define HANDLE_ARG(VAR, TYPE, STYLE, HELP) \
{ \
    void *valp = (void*) &VAR; \
    int const len = strlen(#VAR)+1; \
    for (i = 1; i < argc; i++) \
    {\
        char const *style = #STYLE; \
        int valid_style = style[1]=='d'||style[1]=='g'||style[1]=='s'; \
        if (strncmp(argv[i], #VAR"=", len)) \
            continue; \
        assert(valid_style); \
        if      (style[1] == 'd') /* int */ \
            *((int*) valp) = (int) strtol(argv[i]+len,0,10); \
        else if (style[1] == 'g') /* double */ \
            *((double*) valp) = (double) strtod(argv[i]+len,0); \
        else if (style[1] == 's') /* char* */ \
            *((char**) valp) = (char*) strdup(argv[i]+len); \
        else \
        { \
        } \
    }\
    if (help) \
        fprintf(stderr, "        %s=<%s> %s (" #STYLE ")\n", \
            #VAR, #TYPE, #HELP, VAR);\
    else \
        fprintf(stderr, "    %s="#STYLE"\n", \
            #VAR, VAR);\
}

static void
process_args(int argc, char **argv)
{
    int i;
    int help = 0;

    /* quick pass for 'help' anywhere on command line */
    for (i = 0; i < argc && !help; i++)
        help = 0!=strcasestr(argv[i], "help");
    
    if (help)
    {
        fprintf(stderr, "Usage:\n");
        fprintf(stderr, "    ./heat <arg>=<value> <arg>=<value>...\n");
    }

    HANDLE_ARG(alpha, double, %g, alpha (what is this));
    HANDLE_ARG(dx, double, %g, x-incriment (1/dx->int));
    HANDLE_ARG(dt, double, %g, t-incriment);
    HANDLE_ARG(alg, char*, %s, algorithm);
    HANDLE_ARG(eps, double, %g, convergence criterion);
    HANDLE_ARG(bc0, double, %g, bc @ x=0: u(0,t));
    HANDLE_ARG(bc1, double, %g, bc @ x=1: u(1,t));
    HANDLE_ARG(ic, char*, %s, ic @ t=0: u(x,0));
    HANDLE_ARG(savi, int, %d, save every i-th solution step);
    HANDLE_ARG(save, int, %d, compute/save error in every saved solution);
    HANDLE_ARG(prec, char*, %s, precision half|float|double);

    if (help)
    {
        fprintf(stderr, "Examples...\n");
        fprintf(stderr, "    ./heat Nx=51 dt=0.002 alg=ftcs\n");
        fprintf(stderr, "    ./heat Nx=51 bc0=5 bc1=10\n");
        exit(1);
    }

}

#define TSTART -1
#define TFINAL -2
#define RESIDUAL -3
#define ERROR -4
#define EXACT -5
static void
write_array(int t, int n, double dx, double const *a)
{
    int i;
    char fname[32];
    FILE *outf;

    if (t == TSTART)
        snprintf(fname, sizeof(fname), "heat_soln_initial.curve");
    else if (t == TFINAL)
        snprintf(fname, sizeof(fname), "heat_soln_final.curve");
    else if (t == RESIDUAL)
        snprintf(fname, sizeof(fname), "residual.curve");
    else if (t == ERROR)
        snprintf(fname, sizeof(fname), "error.curve");
    else
    {
        if (a == exact)
            snprintf(fname, sizeof(fname), "heat_exact_%03d.curve", t);
        else
            snprintf(fname, sizeof(fname), "heat_soln_%03d.curve", t);
    }
    
    outf = fopen(fname,"w");
    for (i = 0; i < n; i++)
        fprintf(outf, "%8.4g %8.4g\n", i*dx, a[i]);
    fclose(outf);
}

static void
set_initial_condition(int n, double *a, double dx, char const *ic)
{
    int i;
    double x;

    if (!strncmp(ic, "const(", 6)) /* const(val) */
    {
        double cval = strtod(ic+6, 0);
        for (i = 0; i < n; i++)
            a[i] = cval;
    }
    else if (!strncmp(ic, "step(", 5)) /* step(left,xmid,right) */
    {
        char *p;
        double left = strtod(ic+5, &p);
        double xmid = strtod(p+1, &p);
        double right = strtod(p+1, 0);
        for (i = 0, x = 0; i < n; i++, x+=dx)
        {
            if (x < xmid) a[i] = left;
            else          a[i] = right;
        }
    }
    else if (!strncmp(ic, "ramp(", 5)) /* ramp(left,right) */
    {
        char *p;
        double left = strtod(ic+5, &p);
        double right = strtod(p+1, 0);
        double dv = (right-left)/(n-1);
        for (i = 0, x = left; i < n; i++, x+=dv)
            a[i] = x;
    }
    else if (!strncmp(ic, "rand(", 5)) /* rand(seed,amp) */
    {
        char *p;
        int seed = (int) strtol(ic+5,&p,10);
        double amp = strtod(p+1, 0);
        const double maxr = ((long long)1<<31)-1;
        srandom(seed);
        for (i = 0; i < n; i++)
            a[i] = amp * random()/maxr;
    }
    else if (!strncmp(ic, "sin(Pi*x)", 9)) /* rand(seed,amp) */
    {
        for (i = 0, x = 0; i < n; i++, x+=dx)
            a[i] = sin(M_PI*x);
    }

    write_array(TSTART, Nx, dx, a);
}

static void
compute_exact_solution(int n, double *a, double dx, char const *ic,
    double alpha, double t, double bc0, double bc1)
{
    int i;
    double x;
    if (bc0 == 0 && bc1 == 0 && !strncmp(ic, "sin(Pi*x)", 9))
    {
        for (i = 0, x = 0; i < n; i++, x+=dx)
            a[i] = sin(M_PI*x)*exp(-alpha*M_PI*M_PI*t);
    }
    else if (bc0 == 0 && bc1 == 0 && !strncmp(ic, "const(", 6))
    {
        double cval = strtod(ic+6, 0);
        for (i = 0, x = 0; i < n; i++, x+=dx)
        {
            int n;
            double fsum = 0;

            /* sum first 200 terms of Fourier series */
            for (n = 1; n < 200; n++)
            {
                double coeff = 2*cval*(1-pow(-1.0,(double)n))/(n*M_PI);
                double func = sin(n*M_PI*x)*exp(-alpha*n*n*M_PI*M_PI*t);
                fsum += coeff * func;
            }
            a[i] = fsum;
        }
    }
    else /* can only compute final steady state solution */
    {
        for (i = 0, x = 0; i < n; i++, x+=dx)
            a[i] = bc0 + (bc1-bc0)*x;
    }
}

static double
l2_norm(int n, double const *a, double const *b)
{
    int i;
    double sum = 0;
    for (i = 0; i < n; i++)
    {
        double diff = a[i] - b[i];
        sum += diff * diff;
    }
    return sum;
}

static void
copy(int n, double *dst, double const *src)
{
    int i;
    for (i = 0; i < n; i++)
        dst[i] = src[i];
}

static void
solution_update_ftcs(int n, double *curr, double const *last,
    double alpha, double dx, double dt,
    double bc_0, double bc_1)
{
    int i;
    double k = alpha * alpha * dt / (dx * dx);
    curr[0  ] = bc_0;
    curr[n-1] = bc_1;
    for (i = 1; i < n-1; i++)
        curr[i] = last[i] + k * (last[i-1] - 2 * last[i] + last[i+1]);
}

static void
solution_update_upwind15(int n, double *curr, double const *last,
    double alpha, double dx, double dt,
    double bc_0, double bc_1)
{
    double const f2 = 1.0/24;
    double const f1 = 1.0/6;
    double const f0 = 1.0/4;
    double const k = alpha * alpha * dt / (dx * dx);
    double const k2 = k*k;

    int i;
    curr[0  ] = bc_0;
    curr[1  ] = last[1  ] + k * (last[0  ] - 2 * last[1  ] + last[2  ]);
    curr[n-2] = last[n-2] + k * (last[n-3] - 2 * last[n-2] + last[n-1]);
    curr[n-1] = bc_1;
    for (i = 2; i < n-2; i++)
        curr[i] =  f2*(12*k2  -2*k    )*last[i-2]
                  +f2*(12*k2  -2*k    )*last[i+2]
                  -f1*(12*k2  -8*k    )*last[i-1]
                  -f1*(12*k2  -8*k    )*last[i+1]
                  +f0*(12*k2 -10*k  +4)*last[i  ];
}

int finalize(int ti, int nt_max, double res)
{
    int retval = 0;

    if (ti >= nt_max)
    {
        printf("Did not converge: residual = %8.4g after %d iterations\n", res, ti);
        retval = 1;
    }
    else
    {
        printf("Converged to %8.4g in %d iterations\n", res, ti);
    }

#ifdef HAVE_PAPI /* [ */
    {   
        PAPI_dmem_info_t dmem;


        if((retval=PAPI_flops( &real_time, &proc_time, &flpops, &mflops))<PAPI_OK)
        {    
            printf("retval: %d\n", retval);
            retval = 1;
        }

        if ((retval=PAPI_get_dmem_info(&dmem))<PAPI_OK)
        {   
            printf("get_dmem_info failed with retval: %d\n", retval);
            retval = 1;
        }
        else
        {   
            printf("Memory Info:\n");
            printf("\tMem Size:     %lld\n",dmem.size);
            /*printf("\tMem Resident:\t\t%lld\n",dmem.resident);*/
            printf("\tMem Heap:     %lld\n",dmem.heap);
        }
        printf("Timing Info:\n");
        printf("\tReal_time:    %g\n", real_time);
        printf("\tProc_time:    %g\n", proc_time);
        printf("Flops Info:\n");
        printf("\tTotal flpops: %lld\n", flpops);
        printf("\tMFLOPS:       %g\n", mflops);
    }
#endif /* ] HAVE_PAPI */

    free(curr);
    free(last);
    free(exact);
    free(res_history);
    free(error_history);

    return retval;
}

int main(int argc, char **argv)
{
    int i, ti;
    double error;
    FILE *outf;

    initialize();

    process_args(argc, argv);

    double residual = eps;
    Nx = (int) (1/dx+1.5);
    dx = 1.0/(Nx-1);

    /* Initial condition */
    set_initial_condition(Nx, last, dx, ic);

    /* Iterate until residual is small or hit max iterations */
    for (ti = 0; residual >= dt*eps && ti < Nt_max; ti++)
    {
        if (!strcmp(alg, "ftcs"))
            solution_update_ftcs(Nx, curr, last, alpha, dx, dt, bc0, bc1);
        else if (!strcmp(alg, "upwind15"))
            solution_update_upwind15(Nx, curr, last, alpha, dx, dt, bc0, bc1);

        if (save)
        {
            compute_exact_solution(Nx, exact, dx, ic, alpha, ti*dt, bc0, bc1);
            if (savi && ti%savi==0)
                write_array(ti, Nx, dx, exact);
        }

        if (savi && ti%savi==0)
            write_array(ti, Nx, dx, curr);

        residual = l2_norm(Nx, curr, last);
        res_history[ti] = residual;
        if (save)
            error_history[ti] = l2_norm(Nx, curr, exact);

        copy(Nx, last, curr);
    }

    write_array(TFINAL, Nx, dx, curr);
    write_array(RESIDUAL, ti, dt, res_history);
    if (save)
        write_array(ERROR, ti, dt, error_history);

    return finalize(ti, Nt_max, residual);
}

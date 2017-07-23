# Hand Coded Solution of the 1D Heat Equation

In this lesson, we use a simple, hand-coded, C application to solve
a few different variations of the one dimensional heat equation as
shown here...

![](simple_1d_heat.png)

## Step 1: Compiling The Application

If the application is already compiled for you, proceed to step 2

Note, to get useful timing and flop count information, you must
compile the application with PAPI. You may compile without PAPI
but you will not be able to observe how changes in parameters
effect memory usage, timing and flop counts.

```
cc -DHAVE_PAPI heat.c -o heat -I<path/to/papi/include> -L<path/to/papi/lib> -lpapi
```

## Step 2: Getting help

At any point, you can get help regarding various options the
application supports like so...

```
./heat --help
Usage:
    ./heat <arg>=<value> <arg>=<value>...
        alpha=0.2      thermal diffusivity of material (0.2)
        dx=0.1         x-incriment (1/dx->int)
        dt=0.004       t-incriment
        alg="ftcs"     algorithm ("ftcs", "upwind15", "cn")
        eps=1e-6       convergence criterion
        bc0=0          boundary condition @ x=0: u(0,t)
        bc1=1          boundary condition @ x=1: u(1,t)
        ic="const(1)"  initial condition @ t=0: u(x,0)
        savi=0         save every i-th solution step
        save=0         compute/save error in every saved solution
        prec="double"  precision half|float|double (double)
Examples...
    ./heat Nx=51 dt=0.002 alg=ftcs
    ./heat Nx=51 bc0=5 bc1=10
```

## Step 3: Visualizing Results

The results produced by this application are simple space separated values
text files containing xy pairs of numbers. These can be visualized with
many different tools. Here, we demonstrate the use of VisIt primarily because
VisIt will be useful in visualizing more sophisticated results produced
by more complex examples in later lessons.


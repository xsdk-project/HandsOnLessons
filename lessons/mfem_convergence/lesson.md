# Finite Elements and Convergence

## At a Glance
<!-- (Expected # minutes to complete) %% temporarily omit -->

```
Questions                    |Objectives                      |Key Points
-----------------------------|--------------------------------|---------------------------
What is a finite element     |Understand basic finite element |Basis functions determine
method?                      |machinery                       |the quality of the solution
			     |                                |
What is a high order method? |Understand  how polynomial      |High order methods add more
			     |order affects simulations       |unknowns on the same mesh
			     |                                |for more precise solutions
			     |                                |
What is convergence?         |Understand how convergence and  |High order methods converge
			     |convergence rate is calculated  |faster for smooth solutions
```

## A Widely Applicable Equation

In this lesson, we demonstrate a simple Poisson problem under
uniform refinement. An example of this equation is steady-state heat conduction.

|[<img src="diffusion.png" width="400">](diffusion.png)|

### Governing Equation

This equation can be used to model steady state heat conduction, electric potential,
and gravitational fields. In mathematical terms ...

|![](http://latex.codecogs.com/gif.latex?-%5Cnabla%5E2u%20%3D%20f)|(1)|

where _u_ is the potential field and _f_ is the source function. This is known as
[_Poisson's Equation_](https://en.wikipedia.org/wiki/Poisson's_equation) and is a
generalization of the [_Laplace Equation_](https://en.wikipedia.org/wiki/Laplace%27s_equation).

### Finite element basics

We would like to solve the continuous equation above using computers, so we need to
[_discretize_](https://en.wikipedia.org/wiki/Discretization) it by introducing scalar
unknowns to compute. In the [_finite element method_](https://en.wikipedia.org/wiki/Finite_element_method),
this is done using the concept of _basis functions_.

Instead of calculating the exact analytic solution _u_, consider approximating it by

|![](http://latex.codecogs.com/gif.latex?u%20%5Capprox%20%5Csum_%7Bj%3D1%7D%5En%20c_j%20%5Cphi_j)|(2)|

where ![](http://latex.codecogs.com/gif.latex?c_j) are scalar unknown coefficients and ![](http://latex.codecogs.com/gif.latex?%5Cphi_j)
are known _basis functions_. They are typically
polynomial functions which are only non-zero on small portions of the computational mesh. To solve for
these unknown coefficients, we multiply Poisson's equation by another basis function
![](http://latex.codecogs.com/gif.latex?%5Cphi_i) and integrate
by parts to obtain

|![](http://latex.codecogs.com/gif.latex?%5Csum_%7Bj%3D1%7D%5En%5Cint_%5COmega%20c_j%20%5Cnabla%20%5Cphi_j%20%5Ccdot%20%5Cnabla%20%5Cphi_i%20dV%20%3D%20%5Cint_%5COmega%20f%20%5Cphi_i)|(3)|

for every basis function ![](http://latex.codecogs.com/gif.latex?%5Cphi_i). However since these functions are known, we can rewrite this as

|![](http://latex.codecogs.com/gif.latex?%5Cmathbf%7BAx%7D%20%3D%20%5Cmathbf%7Bb%7D)|(4)|

where

|![](http://latex.codecogs.com/gif.latex?A_%7Bij%7D%20%3D%20%5Cint_%5COmega%20%5Cnabla%20%5Cphi_i%20%5Ccdot%20%5Cnabla%20%5Cphi_j%20dV)|(5)|
|![](http://latex.codecogs.com/gif.latex?b_i%20%3D%20%5Cint_%5COmega%20f%20%5Cphi_i%20dV)|(6)|
|![](http://latex.codecogs.com/gif.latex?x_j%20%3D%20c_j)|(7)|

which is a ![](http://latex.codecogs.com/gif.latex?n%20%5Ctimes%20n) linear algebra problem that can be
solved on a computer for the unknown coefficients. Note that we
are free to choose whichever basis functions
![](http://latex.codecogs.com/gif.latex?%5Cphi_i) as we see fit.

---

## Convergence Study Source Code

To define the system we need to solve, we need three things. First, we need to define our basis
functions which live on a computational mesh.

```c++
FiniteElementCollection *fec = new H1_FECollection(order, dim);
ParFiniteElementSpace *fespace = new ParFiniteElementSpace(pmesh, fec);
```

This defines a collection of H1 functions (meaning the gradient is defined) of
a given polynomial order on a parallel computational mesh pmesh. Next, we
need to define the integrals in equations 5 and 6.

```c++
FunctionCoefficient f(f_exact);
ParLinearForm *b = new ParLinearForm(fespace);
b->AddDomainIntegrator(new DomainLFIntegrator(f));
b->Assemble();
```

```c++
ParBilinearForm *a = new ParBilinearForm(fespace);
ConstantCoefficient one(1.0);
a->AddDomainIntegrator(new DiffusionIntegrator(one));
a->Assemble();
```

This defines the matrix A and the vector b. We then solve the linear
system for our solution vector x.

```c++
HypreParMatrix A;
Vector B, X;
a->FormLinearSystem(ess_tdof_list, x, *b, A, X, B);

HypreBoomerAMG *amg = new HypreBoomerAMG(A);
amg->SetPrintLevel(0);
HyprePCG *pcg = new HyprePCG(A);
pcg->SetTol(1e-12);
pcg->SetMaxIter(200);
pcg->SetPrintLevel(0);
pcg->SetPreconditioner(*amg);
pcg->Mult(B, X);
a->RecoverFEMSolution(X, *b, x);
```

Since we know what the exact solution is, we can measure the amount of error in
our approximate solution two ways:

|![](http://latex.codecogs.com/gif.latex?%5Cleft%20%5C%7C%20u_%7B%5Cmbox%7Bexact%7D%7D%20-%20u_%7B%5Cmbox%7Bh%7D%7D%20%5Cright%20%5C%7C_%7BL_2%7D%5E2%20%3D%20%5Cint_%5COmega%20%5Cleft%7C%20u_%7B%5Cmbox%7Bexact%7D%7D%20-%20u_%7B%5Cmbox%7Bh%7D%7D%20%5Cright%20%7C%5E2)|(8)|
|![](http://latex.codecogs.com/gif.latex?%5Cleft%20%5C%7C%20u_%7B%5Cmbox%7Bexact%7D%7D%20-%20u_%7B%5Cmbox%7Bh%7D%7D%20%5Cright%20%5C%7C_%7BH%5E1%7D%5E2%20%3D%20%5Cleft%20%5C%7C%20u_%7B%5Cmbox%7Bexact%7D%7D%20-%20u_%7B%5Cmbox%7Bh%7D%7D%20%5Cright%20%5C%7C_%7BL_2%7D%5E2%20&plus;%20%5Cleft%20%5C%7C%20%5Cnabla%20u_%7B%5Cmbox%7Bexact%7D%7D%20-%20%5Cnabla%20u_%7B%5Cmbox%7Bh%7D%7D%20%5Cright%20%5C%7C_%7BL_2%7D%5E2)|(9)|

Since we expect the error to behave like

|![](http://latex.codecogs.com/gif.latex?%5Cleft%20%5C%7C%20u_%7B%5Cmbox%7Bexact%7D%7D%20-%20u_%7B%5Cmbox%7Bh%7D%7D%20%5Cright%20%5C%7C_%7BL_2%7D%5E2%20%5Cleq%20Ch%5E%7B-r%7D)|(10)|

we can estimate the [_convergence rate_](https://en.wikipedia.org/wiki/Rate_of_convergence) as

|![](http://latex.codecogs.com/gif.latex?r%20%5Capprox%20%5Cfrac%7B%5Clog%5C%20%5Cfrac%7B%20%5Cleft%20%5C%7C%20u_%7B%5Cmbox%7Bexact%7D%7D%20-%20u_%7Bh_%7B%5Cmbox%7Bnew%7D%7D%7D%20%5Cright%20%5C%7C_%7BL_2%7D%7D%7B%5Cleft%20%5C%7C%20u_%7B%5Cmbox%7Bexact%7D%7D%20-%20u_%7Bh_%7B%5Cmbox%7Bold%7D%7D%7D%20%5Cright%20%5C%7C_%7BL_2%7D%7D%7D%7B%20%5Clog%20%5Cfrac%7Bh_%7B%5Cmbox%7Bnew%7D%7D%7D%7Bh_%7B%5Cmbox%7Bold%7D%7D%7D%7D)|(11)|

where ![](http://latex.codecogs.com/gif.latex?h) is the size of an element in the mesh and
![](http://latex.codecogs.com/gif.latex?C) is a mesh independent constant when we refine the mesh and compare runs.

```c++
double l2_err = x.ComputeL2Error(u);
double h1_err = x.ComputeH1Error(&u, &u_grad, &one, 1.0, 1);
pmesh->GetCharacteristics(h_min, h_max, kappa_min, kappa_max);

l2_rate = log(l2_err/l2_err_prev) / log(h_min/h_prev);
h1_rate = log(h1_err/h1_err_prev) / log(h_min/h_prev);
```

---

## Running the Convergence Study

The convergence study has the following options

```
./convergence --help

Usage: ./convergence [options] ...
Options:
   -h, --help
	Print this help message and exit.
   -m <string>, --mesh <string>, current value: ../../../data/star.mesh
	Mesh file to use.
   -o <int>, --order <int>, current value: 1
	Finite element order (polynomial degree).
   -sc, --static-condensation, -no-sc, --no-static-condensation, current option: --no-static-condensation
	Enable static condensation.
   -r <int>, --refinements <int>, current value: 4
	Number of total uniform refinements
   -sr <int>, --serial-refinements <int>, current value: 2
	Maximum number of serial uniform refinements
   -f <double>, --frequency <double>, current value: 1
	Set the frequency for the exact solution.
```

### Run 1 (Low order)

In this run, we will examine the error after 7 uniform refinements in both the L2 and H1 norms using first order (linear) basis functions.

```
./convergence -r 7
Options used:
   --mesh ../../../data/star.mesh
   --order 1
   --no-static-condensation
   --refinements 7
   --serial-refinements 2
   --frequency 1
----------------------------------------------------------------------------------------
DOFs            h               L^2 error       L^2 rate        H^1 error       H^1 rate
----------------------------------------------------------------------------------------
31              0.4876          0.3252          0               2.631           0
101             0.2438          0.09293         1.807           1.387           0.9229
361             0.1219          0.02393         1.957           0.7017          0.9836
1361            0.06095         0.006027        1.989           0.3518          0.996
5281            0.03048         0.00151         1.997           0.176           0.999
20801           0.01524         0.0003776       1.999           0.08803         0.9997
82561           0.007619        9.441e-05       2               0.04402         0.9999
```

Note that the L2 error is converging at a rate of 2 and the H1 error is only converging at a rate of 1.

### Run 2 (High order)

Now consider the same run only we are using 3rd order (cubic) basis functions instead.

```
./convergence -r 7 -o 3
Options used:
   --mesh ../../../data/star.mesh
   --order 3
   --no-static-condensation
   --refinements 7
   --serial-refinements 2
   --frequency 1
----------------------------------------------------------------------------------------
DOFs            h               L^2 error       L^2 rate        H^1 error       H^1 rate
----------------------------------------------------------------------------------------
211             0.4876          0.004777        0               0.118           0
781             0.2438          0.0003178       3.91            0.01576         2.905
3001            0.1219          2.008e-05       3.984           0.001995        2.982
11761           0.06095         1.258e-06       3.997           0.0002501       2.996
46561           0.03048         7.864e-08       4               3.129e-05       2.999
185281          0.01524         4.915e-09       4               3.912e-06       3
739201          0.007619        3.072e-10       4               4.891e-07       3
```

The L2 error is now converging at a rate of 4 and the H1 error is converging at a rate of 3. This is
because for the method implemented with a smooth exact solution, the L2 error converges at a rate of
p+1 while the H1 error converges at a rate of p.

### Run 3 (3D example)
The previous two runs used a 2D mesh, but the same code can be used to run a 3D problem.

```
./convergence -r 4 -o 2 -m ../../../data/inline-hex.mesh
Options used:
   --mesh ../../../data/inline-hex.mesh
   --order 2
   --no-static-condensation
   --refinements 4
   --serial-refinements 2
   --frequency 1
----------------------------------------------------------------------------------------
DOFs            h               L^2 error       L^2 rate        H^1 error       H^1 rate
----------------------------------------------------------------------------------------
729             0.25            0.001386        0               0.04431         0
4913            0.125           0.0001772       2.967           0.01106         2.002
35937           0.0625          2.227e-05       2.993           0.002765        2.001
274625          0.03125         2.787e-06       2.998           0.0006911       2
```

---

## Out-Brief

We demonstrated the ease of implementing a order and dimension independent finite element
code in MFEM. We discussed the basics of the finite element method as well as demonstrated
the effect of the polynomial order of the basis functions on convergence rates.

### Further Reading

To learn more about MFEM including examples and miniapps visit [mfem.org](mfem.org).

<!-- Insert space, horizontal line, and link to HandsOnLesson table -->

&nbsp;

---

[Back to all HandsOnLessons](../lessons.md)

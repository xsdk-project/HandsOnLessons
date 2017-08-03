# Using adjoint for optimization

## At A Glance (10 mins)

```
Questions                  |Objectives                    |Key Points
---------------------------|------------------------------|-------------------------------------
How to compute gradients   |Know the techniques and       |Adjoint enables dynamic
for simulations?           |software packages             |constrained optimization
Why use an adjoint solver? |Understand ingredients needed |Jacobian is imperative
                           |for adjoint calculation       |
How difficult is it to use |Understand the concern of     |Performance may depend on
the adjoint capability?    |checkpointing                 |checkpointing at large scale
```

## Example 1: generator stability analysis:

This code demonstrates how to solve an ODE-constrained optimization problem with TAO, TSEvent, TSAdjoint and TS.
The objective is to maximize the mechanical power input subject to the generator swing equations and a constraint on the maximum rotor angle deviation, which is reformulated as a minimization problem

![equation](http://latex.codecogs.com/gif.latex?%5Cbegin%7Balign%2A%7D%0D%0A%20%20%5Cmin%20%26%20%5C%7B-P_m%20%2B%20%5Csigma%5Cdisplaystyle%20%5Cint_%7Bt_0%7D%5E%7Bt_F%7D%20%5Cmax%5Cleft%280%2C%20%5Ctheta%20-%20%5Ctheta_%7Bmax%7D%5Cright%29%5E%5Ceta%20%5C%20%5Cmathrm%7Bd%7Dt%20%5C%7D%5C%5C%0D%0A%20%20%5Cnonumber%20%7E%7E%20%5Ctext%7Bs.t.%7D%20%26%20%5Cqquad%20%5Cfrac%7Bd%20%5Ctheta%7D%7Bdt%7D%20%3D%20%5Comega_B%5Cleft%28%5Comega%20-%20%5Comega_s%5Cright%29%20%5C%5C%0D%0A%20%20%26%20%5Cqquad%20%5Cfrac%7Bd%20%5Comega%7D%7Bdt%7D%20%3D%20%5Cfrac%7B%5Comega_s%7D%7B2H%7D%5Cleft%28P_m%20-%20P_%7Bmax%7D%5Csin%28%5Ctheta%29%20-%20D%28%5Comega%20-%20%5Comega_s%29%5Cright%29%0D%0A%5Cend%7Balign%2A%7D)

Disturbance (a fault) is applied at time 0.1 and cleared at time 0.2.
The objective function contains an integral function.
The gradient is computed with the discrete adjoint of an implicit theta method,

### Compile the code
This example is in src/ts/examples/power_grid. To compile, run the following in the source folder
```
make ex3opt
```

### Command line options
You can find out the command line options that this particular example can use by doing
```
./ex3opt -help
```
and show the options related to TAO only by doing
./ex3opt -help | grep tao

### Run 1:

```
./ex3opt -tao_monitor -tao_view
iter =   0, Function value: 2.03778,  Residual: 144.125
iter =   1, Function value: -0.552947,  Residual: 43.1456
iter =   2, Function value: -0.911654,  Residual: 18.3028
iter =   3, Function value: -1.00401,  Residual: 2.48745
iter =   4, Function value: -1.00649,  Residual: 1.17916
iter =   5, Function value: -1.00732,  Residual: 0.125532
iter =   6, Function value: -1.00733,  Residual: 0.00012392
iter =   7, Function value: -1.00733,  Residual: 1.3024e-08
iter =   8, Function value: -1.00733,  Residual: 3.46501e-12
Tao Object: 1 MPI processes
type: blmvm
Gradient steps: 0
TaoLineSearch Object: 1 MPI processes
type: more-thuente
Active Set subset type: subvec
convergence tolerances: gatol=1e-08,   steptol=0.,   gttol=0.
Residual in Function/Gradient:=3.46501e-12
Objective value=-1.00733
total number of iterations=8,                          (max: 2000)
total number of function/gradient evaluations=9,      (max: 4000)
Solution converged:    ||g(X)|| <= gatol
Vec Object: 1 MPI processes
type: seq
1.00793
```

### Further information

A more complicated example for power grid application is in src/ts/examples/power_grid/stability_9bus/ex9busopt.c.


## Example 2: a hybrid dynamical system:

This code demonstrates how to compute the adjoint sensitivity for a complex dynamical system involving discontinuities with TSEvent, TSAdjoint and TS.
The ODE system alternates the right-hand side when a switching face is encountered. The switching surfaces are given by the algebraic constraints depending on the state variables, as shown below (left)


<img src="ex1.png" width="400"><img src="ex1adj.png" width="400">

* The parameter to which the sensitivities are computed is marked in red.
* It represents the slope of the switching surface.
* Intuitively the trajectory cannot be affected before it hits the surface.
* The influence of the perturbation in the slope diminishes as the trajectory approaching the equilibrium point.

### Compile the code
This example is in src/ts/examples/hybrid.

```
make ex1adj

```

### Run 1 monitor solution graphically with phase diagram

```
./ex1adj -ts_monitor_draw_solution_phase -4,-2,2,2 -draw_pause -2
```

### Run 2 monitor the timestepping process

```
./ex1adj -ts_monitor
```
Trailing (r) indicates that a rollback happens. In this example, it is triggered by TSEvent. To check the details about the event, we use the event monitor  
```
./ex1adj -ts_monitor -ts_event_monitor
```
We can also monitor the timestepping for the adjoint calculation by doing
```
./ex1adj -ts_monitor -ts_adjoint_monitor
```


## Example 3: diffusion-reaction problem

This code demonstrates parallel adjoint calculation for a system of time-dependent PDE on a 2D rectangular grid.
We only need to write the right-hand-side function and the Jacobian and compile the code once. All the tasks in the following can be accompolished just using command line options. 

### Run 1 monitor solution graphically

```
mpirun -n 4 ./ex5adj -forwardonly -implicitform 0 -ts_type rk \
                     -ts_monitor -ts_monitor_draw_solution
```

* -forwardonly perform the forward simulation without doing adjoint
* -implicitform 0 -ts_type rk changes the time stepping algorithm to a Runge-Kutta method
* -ts_monitor_draw_solution monitors the progress for the solution at each time step
* Add -draw_pause -2 if you want to pause at the end of simulation to see the plot

### Run 2 optimal checkpointing schedule
By default, the checkpoints are stored into binary files on disk. Of course, this may not be a good choice for large scale applications running on big machines where I/O cost is significant. We can make the solver use RAM for checkpointing and specify the maximum allowable checkpoints so that an optimal adjoint checkpointing schedule that minimizes the number of recomputations will be generated.

```
mpirun -n 4 ./ex5adj -implicitform 0 -ts_type rk -ts_adapt_type none \
                     -ts_max_steps 10 -ts_monitor -ts_adjoint_monitor \
                     -ts_trajectory_type memory -ts_trajectory_max_cps_ram 3 \
                     -ts_trajectory_monitor -ts_trajectory_view  
```
The output corresponds to the schedule dipicted by the following diagram.

<img src="chkpt.png" width="600">

> **What will happen if we add two slots for disk checkpoints with -ts_trajectory_max_cps_disk 2 ?**


### Run 3 implicit time stepping
Now we switch to implicit method ([Crank-Nicolson](https://en.wikipedia.org/wiki/Crank–Nicolson_method)) using fixed stepsize, which is the default setting in the code. At each time step, a nonlinear system is solved by the PETSc nonlinear solver SNES.
```
mpirun -n 32 ./ex5adj -da_grid_x 1024 -da_grid_y 1024 -ts_max_steps 10 -snes_monitor -log_view
```
* -snes_monitor shows the progress of SNES
* -log_view prints a summary of the logging

> **What is the majority of CPU time spent on?**

> **How expensive is it to do an adjoint step?**

> **How can we improve the performance?**


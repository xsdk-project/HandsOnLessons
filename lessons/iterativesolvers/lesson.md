# Iterative Solution of Linear and Nonlinear Systems

## At a Glance
<!-- (Expected # minutes to complete) %% temporarily omit -->

```
Questions                 |Objectives                     |Key Points
--------------------------|-------------------------------|-------------------------------------
How can gradients be      |Know PETSc/TAO's capability for|Adjoint enables dynamic
computed for simulations? |adjoint and optimization       |constrained optimization.
                          |                               |
How difficult is it to    |Understand ingredients needed  |Jacobian is imperative.
use the adjoint method?   |for adjoint calculation        |
                          |                               |
                          |Understand the concern of      |Performance may depend on
                          |checkpointing                  |checkpointing at large scale.
```

## Example 1: Structural Mechanics Beam Deflection:

This code uses MFEM and [PETSc/TAO](https://www.mcs.anl.gov/petsc/) to demonstrates convergence of Krylov methods.

The source code is included in [ex2p.c](./ex2p.c)

### Run 1: Run with Jacobi preconditioner

```
PETSC_OPTIONS="-pc_type jacobi -ksp_max_it 25" ./ex2p -petscopts rc_ex2p --mesh /projects/ATPESC2017/NumericalPackages/handson/mfem/data/beam-tri.mesh 
```
#### Questions
> **Is the iteration converging?**
> **Read the output at the bottom from -ksp_view, what Krylov method and preconditioner is it using??**

### Run 2: Run with the algebraic multigrid preconditioner

```
./ex2p -petscopts rc_ex2p --mesh /projects/ATPESC2017/NumericalPackages/handson/mfem/data/beam-tri.mesh 
```

## Example 2: Hybrid Dynamical System:

This code demonstrates how to compute the adjoint sensitivity for a complex dynamical system involving discontinuities with TSEvent, TSAdjoint and TS. The dynamics are described by the ODE

![equation](http://latex.codecogs.com/gif.latex?%5Cdot%7Bx%7D%20%3D%20A_i%20x)

where ![equation](http://latex.codecogs.com/gif.latex?x%20%3D%20%5Bx_1%2C%20x_2%5D%5ET) and the matrix A change from

![equation](http://latex.codecogs.com/gif.latex?A_1%20%3D%20%5Cleft%5B%20%5Cbegin%7Barray%7D%7Bc%20c%7D1%20%26-100%5C%5C%2010%20%261%20%5Cend%7Barray%7D%0D%0A%5Cright%5D%0D%0A%5Cquad%20%5Ctext%7Bto%7D%20%5Cquad%0D%0AA_2%20%3D%20%5Cleft%5B%20%5Cbegin%7Barray%7D%7Bc%20c%7D1%20%2610%5C%5C%20-100%20%261%20%5Cend%7Barray%7D%0D%0A%5Cright%5D)

 when ![equation](http://latex.codecogs.com/gif.latex?%24x_2%3D2.75%20x_1%24) and switch back  when ![equation](http://latex.codecogs.com/gif.latex?%24x_2%3D0.365%20x_1%24).

Thus the ODE system alternates the right-hand side when a switching face is encountered. The switching surfaces are given by the algebraic constraints depending on the state variables, as shown below (left)

<img src="ex1.png" width="400"><img src="ex1adj.png" width="400">

* The parameter to which the sensitivities are computed is marked in red.
* It represents the slope of the switching surface.
* Intuitively the trajectory cannot be affected before it hits the surface.
* The influence of the perturbation in the slope diminishes as the trajectory is approaching the equilibrium point.

### Compile the code
This example is in `src/ts/examples/hybrid`. The source code is included in [ex1adj.c](./ex1adj.c)

```
make ex1adj
```

### Run 1: Monitor solution graphically with phase diagram

```
./ex1adj -ts_monitor_draw_solution_phase -4,-2,2,2 -draw_pause -2
```

### Run 2: Monitor the timestepping process

```
./ex1adj -ts_monitor
```
Trailing (r) in some lines of the output indicates that a rollback happens. In this example, it is triggered by `TSEvent`. To check  details about the event, we can use the event monitor
```
./ex1adj -ts_monitor -ts_event_monitor
```
We can also monitor the timestepping for the adjoint calculation by doing
```
./ex1adj -ts_monitor -ts_adjoint_monitor
```

### Further information

The example `ex1fwd.c` in the same folder illustrates the forward sensitivity approach for the same problem.


## Example 3: Diffusion-Reaction Problem

This code demonstrates parallel adjoint calculation for a system of time-dependent PDEs on a 2D rectangular grid.
We need only to write the right-hand-side function and the Jacobian and compile the code once. All the tasks in the following can be accomplished using command line options.

### Compile the code
This example is in `src/ts/examples/advection-diffusion-reaction`. The source code is included in [ex5adj.c](./ex5adj.c)

```
make ex5adj
```

### Run 1: Monitor solution graphically

```
mpirun -n 4 ./ex5adj -forwardonly -implicitform 0 -ts_type rk \
                     -ts_monitor -ts_monitor_draw_solution
```

* `-forwardonly` perform the forward simulation without doing adjoint
* `-implicitform 0 -ts_type rk` changes the time stepping algorithm to a Runge-Kutta method
* `-ts_monitor_draw_solution` monitors the progress for the solution at each time step
* Add `-draw_pause -2` if you want to pause at the end of simulation to see the plot

### Run 2: Optimal checkpointing schedule
By default, the checkpoints are stored in binary files on disk. Of course, this may not be a good choice for large-scale applications running on high-performance machines where I/O cost is significant. We can make the solver use RAM for checkpointing and specify the maximum allowable checkpoints so that an optimal adjoint checkpointing schedule that minimizes the number of recomputations will be generated.

```
mpirun -n 4 ./ex5adj -implicitform 0 -ts_type rk -ts_adapt_type none \
                     -ts_max_steps 10 -ts_monitor -ts_adjoint_monitor \
                     -ts_trajectory_type memory -ts_trajectory_max_cps_ram 3 \
                     -ts_trajectory_monitor -ts_trajectory_view
```
The output corresponds to the schedule depicted by the following diagram:

<img src="chkpt.png" width="800">

#### Questions
> **What will happen if we add the option `-ts_trajectory_max_cps_disk 2` to specify there are two available slots for disk checkpoints?**

|<font color="white">Looking at the output, we will find that the new schedule uses both RAM and disk for checkpointing and takes two less recomputations.</font>|

### Run 3: Implicit time integration method
Now we switch to implicit method ([Crank-Nicolson](https://en.wikipedia.org/wiki/Crank–Nicolson_method)) using fixed stepsize, which is the default setting in the code. At each time step, a nonlinear system is solved by the PETSc nonlinear solver `SNES`.
```
mpirun -n 12 ./ex5adj -da_grid_x 1024 -da_grid_y 1024 -ts_max_steps 10 -snes_monitor -log_view
```
* `-snes_monitor` shows the progress of `SNES`
* `-log_view` prints a summary of the logging

A snippet of the summary:
```
...
Phase summary info:
   Count: number of times phase was executed
   Time and Flop: Max - maximum over all processors
                   Ratio - ratio of maximum to minimum over all processors
   Mess: number of messages sent
   Avg. len: average message length (bytes)
   Reduct: number of global reductions
   Global: entire computation
   Stage: stages of a computation. Set stages with PetscLogStagePush() and PetscLogStagePop().
      %T - percent time in this phase         %F - percent flop in this phase
      %M - percent messages in this phase     %L - percent message lengths in this phase
      %R - percent reductions in this phase
   Total Mflop/s: 10e-6 * (sum of flop over all processors)/(max time over all processors)
------------------------------------------------------------------------------------------------------------------------
Event                Count      Time (sec)     Flop                             --- Global ---  --- Stage ---   Total
                   Max Ratio  Max     Ratio   Max  Ratio  Mess   Avg len Reduct  %T %F %M %L %R  %T %F %M %L %R Mflop/s
------------------------------------------------------------------------------------------------------------------------

--- Event Stage 0: Main Stage

VecDot                20 1.0 2.7505e-02 1.7 7.00e+06 1.0 0.0e+00 0.0e+00 2.0e+01  0  0  0  0  2   0  0  0  0  2  3050
VecMDot              321 1.0 2.6292e+00 1.4 6.62e+08 1.0 0.0e+00 0.0e+00 3.2e+02 25 15  0  0 34  25 15  0  0 34  3017
VecNorm              401 1.0 7.1590e-01 1.9 1.40e+08 1.0 0.0e+00 0.0e+00 4.0e+02  7  3  0  0 42   7  3  0  0 42  2349
...
```

#### Questions
> **Where is the majority of CPU time spent?**

|<font color="white">Of course answer may vary depending on the settings such as number of procs, problem size, and solver options. Typically most of the time should be spent on [VecMDot](http://www.mcs.anl.gov/petsc/petsc-current/docs/manualpages/Vec/VecMDot.html) or [MatMult](http://www.mcs.anl.gov/petsc/petsc-current/docs/manualpages/Mat/MatMult.html) </font>|

> **How expensive is it to do an adjoint step?**

|<font color="white">For this particular run, an adjoint step takes about 70% of the running time of a forward step (compare the time between TSAdjointStep and TSStep). </font>|

> **How can we improve performance?**

|<font color="white">1. Use memory instead of disk for checkpointing(`-ts_trajectory_type memory -ts_trajectory_solution_only 0`); 2. Tune the time stepping solver, nonlinear solver, linear solver, preconditioner and so forth. </font>|

### Further information
Because this example uses `DMDA`, Jacobian can be efficiently approxiated using finite difference with coloring. You can use the option `-snes_fd_color` to enable this feature.

## Out-Brief

We have used [PETSc](https://www.mcs.anl.gov/petsc/) to demonstrate Krylov methods and nonlinear solver methods.

We have shown the basic usage of the adjoint solver as well as functionalities that can facilitate rapid development, diagnosis and performance profiling.

<!-- Insert space, horizontal line, and link to HandsOnLesson table -->

&nbsp;

---

[Back to all HandsOnLessons](../lessons.md)

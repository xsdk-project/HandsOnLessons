# Iterative Solution of Linear and Nonlinear Systems

## At a Glance
<!-- (Expected # minutes to complete) %% temporarily omit -->

```
Questions                 |Objectives                     |Key Points
--------------------------|-------------------------------|-------------------------------------
Does the preconditioner   |                               |
 affect the convergence   |                               |
 rate?                    |                               |
                         
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

> **Read the output at the bottom from -ksp_view, what Krylov method and preconditioner is it using?**

### Run 2: Run with the algebraic multigrid preconditioner

```
./ex2p -petscopts rc_ex2p --mesh /projects/ATPESC2017/NumericalPackages/handson/mfem/data/beam-tri.mesh 
```


## Out-Brief

We have used [PETSc](https://www.mcs.anl.gov/petsc/) to demonstrate Krylov methods and nonlinear solver methods.

We have shown the basic usage of the adjoint solver as well as functionalities that can facilitate rapid development, diagnosis and performance profiling.

<!-- Insert space, horizontal line, and link to HandsOnLesson table -->

&nbsp;

---

[Back to all HandsOnLessons](../lessons.md)

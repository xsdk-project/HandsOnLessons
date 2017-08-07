# Iterative Solution of Linear and Nonlinear Systems

## At a Glance
<!-- (Expected # minutes to complete) %% temporarily omit -->

```
Questions                 |Objectives                     |Key Points
--------------------------|-------------------------------|-------------------------------------
Does the preconditioner   | See that the preconditioner   |
 affect the convergence   |  can be crucial for           |
 rate?                    |  convergence                  |
                         
```

Before running the examples you must switch to the bash shell by using 

```
bash
```

## Example 1: Structural Mechanics Beam Deflection:

This code uses MFEM and [PETSc/TAO](https://www.mcs.anl.gov/petsc/) to demonstrates convergence of Krylov methods.

The source code is included in [ex2p.c](./ex2p.c)

Notes: Normally PETSc options can be passed in as command line arguments but MFEM turns off this capability, thus they must be passed either in a file or in the PETSC_OPTIONS environmental variable.

### Run 1: Run with Jacobi preconditioner

```
PETSC_OPTIONS="-pc_type jacobi -ksp_max_it 25" ./ex2p -petscopts rc_ex2p --mesh /projects/ATPESC2017/NumericalPackages/handson/mfem/data/beam-tri.mesh 
```

The first column of the output is the residual norm. The next two are the maximum and minimum estimated eigenvalues of the operator and the final column is the condition number.

#### Questions
> **Is the iteration converging?**

> **Read the output at the bottom from -ksp_view, what Krylov method and preconditioner is it using?**

### Run 2: Run with the algebraic multigrid preconditioner

```
./ex2p -petscopts rc_ex2p --mesh /projects/ATPESC2017/NumericalPackages/handson/mfem/data/beam-tri.mesh 
```

#### Questions
> **Is the iteration now converging?**

> **Read the output at the bottom from -ksp_view, what Krylov method and preconditioner is it using?**

### Run 3: Run with the algebraic multigrid preconditioner but no conjugate gradient method

```
PETSC_OPTIONS="-ksp_norm_type preconditioned -ksp_type richardson -ksp_max_it 25"  ./ex2p -petscopts rc_ex2p --mesh /projects/ATPESC2017/NumericalPackages/handson/mfem/data/beam-tri.mesh 
```

#### Questions
> **Is the iteration now converging?**

### Run 4: Run with the algebraic multigrid preconditioner but with GMRES and a restart of 10

```
PETSC_OPTIONS="-ksp_norm_type preconditioned -ksp_type gmres -ksp_gmres_restart 10"  ./ex2p -petscopts rc_ex2p --mesh /projects/ATPESC2017/NumericalPackages/handson/mfem/data/beam-tri.mesh 
```

Now run with a gmres restart of 30

```
PETSC_OPTIONS="-ksp_norm_type preconditioned -ksp_type gmres -ksp_gmres_restart 30"  ./ex2p -petscopts rc_ex2p --mesh /projects/ATPESC2017/NumericalPackages/handson/mfem/data/beam-tri.mesh 
```

Note the convergence is now very similar to that with CG.

Now attempt to run this parallelly and obtain solver performance data
```
PETSC_OPTIONS="-log_view -ksp_norm_type preconditioned -ksp_type gmres -ksp_gmres_restart 30" ${MPIEXEC_OMPI} -n 4 ./ex2p -petscopts rc_ex2p --mesh /projects/ATPESC2017/NumericalPackages/handson/mfem/data/beam-tri.mesh
```

## Out-Brief

We have used [PETSc](https://www.mcs.anl.gov/petsc/) to demonstrate Krylov methods and nonlinear solver methods.


<!-- Insert space, horizontal line, and link to HandsOnLesson table -->

&nbsp;

---

[Back to all HandsOnLessons](../lessons.md)

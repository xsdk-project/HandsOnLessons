# Time Integrators

## At A Glance (15 minutes)

```
Questions                     |Objectives                      |Key Points
------------------------------|--------------------------------|----------
What is the SUNDIALS Package? |Objective 3                     |Key Point 3
What is a time integrator?    |Objective 3                     |Key Point 3
Question 3?                   |Objective 3                     |Key Point 3
```

## The problem being solved

The example application here, [transient-heat.cpp](transient-heat.cpp.numbered),
uses MFEM as a vehicle to demonstrate the use of the
[SUNDIALS](https://computation.llnl.gov/projects/sundials) package
in both serial and parallel for more robust and flexible control over _time integration_
(e.g. discretization in time) of PDEs.

|[<img src="mfem_sundials_explicit0000png" width="400">](mfem_sundails_explicit0000.png)|[<img src="pyramid_animated.gif" width="400">](pyramid_animated.gif)

The application has been designed to solve a slightly more general form of the
[_Heat Equation_](https://en.wikipedia.org/wiki/Heat_equation)

|![](http://latex.codecogs.com/gif.latex?%5Cfrac%7B%5Cpartial%20u%7D%7B%5Cpartial%20t%7D%20%3D%20%28%5Calpha%20%2B%20%5Ckappa%7Bu%7D%29%5Cfrac%7B%5Cpartial%5E2%20u%7D%7B%5Cpartial%20x%5E2%7D)|(1)|

where the material thermal diffusivity is given by
![](http://latex.codecogs.com/gif.latex?%28%5Calpha%20%2B%20%5Ckappa%7Bu%7D%29)
which includes the same constant term ![](http://latex.codecogs.com/gif.latex?%5Calpha)
as in [Lesson 1](../hand_coded_heat/lesson.md) plus a term
![](http://latex.codecogs.com/gif.latex?%5Ckappa%7Bu%7D) which varies with
temperature, _u_, introducing the option of solving systems involving non-linearity.



### Getting Help 
```
$ ./transient-heat --help
Usage: ./transient-heat [options] ...
Options:
   -h, --help
        Print this help message and exit.
   -d <int>, --dim <int>, current value: 2
        Number of dimensions in the problem (1 or 2).
   -r <int>, --refine <int>, current value: 0
        Number of times to refine the mesh uniformly.
   -o <int>, --order <int>, current value: 1
        Order (degree) of the finite elements.
   -ao <int>, --arkode-order <int>, current value: 4
        Order of the time integration scheme.
   -tf <double>, --t-final <double>, current value: 0.1
        Final time; start time is 0.
   -dt <double>, --time-step <double>, current value: 0.001
        Initial time step.
   -a <double>, --alpha <double>, current value: 0.2
        Alpha coefficient for conductivity: kappa + alpha*temperature
   -k <double>, --kappa <double>, current value: 0.5
        Kappa coefficient conductivity: kappa + alpha*temperature
   -art <double>, --arkode-reltol <double>, current value: 0.0001
        Relative tolerance for ARKODE time integration.
   -aat <double>, --arkode-abstol <double>, current value: 0.0001
        Absolute tolerance for ARKODE time integration.
   -adt, --adapt-time-step, -fdt, --fixed-time-step, current option: --fixed-time-step
        Flag whether or not to adapt the time step.
   -imp, --implicit, -exp, --explicit, current option: --explicit
        Implicit or Explicit ODE solution.
   -v, --visit, -nov, --no_visit, current option: --no_visit
        Enable dumping of visit files.
```

Note the `-k` option permits the non-linear term to be disabled.

Nonetheless, the application
can be run such that ![](http://latex.codecogs.com/gif.latex?%5Ckappa)
can be set to zero, returning the problem

 



These execute  a nonlinear heat equation:
Dt/Dt - Div( (0.5 + 0.2 T)grad(T) ) = 0 with grad(T)dot(unit normal) = 0 on the boundary (no flow boundary conditions) and an initial condition of the pyramid with height 1 in the center and final time of 0.1.


Aaron set the defaults to reflect this conductivity (0.5 + 0.2 T).  If this is removed for a different demo, we will need to add the next option to the below execution lines:
--alpha 0.2

Start with explicit:

./transient-heat -dt 0.001 --arkode-order 4 --explicit Fails after a few time steps due to lack of stability

./transient-heat -dt 0.0005 --arkode-order 4 --explicit Succeeds with 200 steps and 852 function evaluations

./transient-heat -adt --arkode-order 4 --arkode-abstol 1e-6 --arkode-reltol 1e-6 --explicit Succeeds with ?? (136 for 1e-8 tol) steps and ?? (865 for 1e-8 tol) function evaluations


Go to implicit:

./transient-heat -dt 0.001 --arkode-order 4 -- implicit Succeeds with 100 steps and 513 nonlinear solver iterations

./transient-heat -dt 0.001 --arkode-order 2 -- implicit Succeeds with 100 steps and 224 nonlinear solver iterations

./transient-heat -adt --arkode-order 4 --arkode-abstol 1e-6 --arkode-reltol 1e-6  --implicit Succeeds with 26 steps and 210 nonlinear solver iterations

./transient-heat -adt --arkode-order 2 --arkode-abstol 1e-6 --arkode-reltol 1e-6  --implicit Succeeds with 127 steps and 321 nonlinear solver iterations

./transient-heat -adt --arkode-order 5 --arkode-abstol 1e-6 --arkode-reltol 1e-6  --implicit Succeeds with 15 steps and 180 nonlinear solver iterations

I will point out a number of items with each of these runs.  It will be good to see the time step profiles (may need to zoom in close to the y-axis as I expect the time steps dramatically increase at the start) and the operation counts.  Here we will have some good work measures beyond those indicated above.  However, we don't have an accuracy measure - something for next year.

If you have a bunch of time (ha!) and have a way to take a solution from a highly resolved run (./transient-heat -adt --arkode-order 2 --arkode-abstol 1e-8 --arkode-reltol 1e-8  --implicit) and show plots of eth difference between solutions from these runs over time, that would be awesome.  I don't expect any of us have time for that at this late hour though.

Let me know if your numbers differ from these when you run.


solves
Here is the sequence of runs to do with the MFEM/SUNDIALS example:

Aaron set the defaults to reflect this conductivity (0.5 + 0.2 T).  If this is removed for a different demo, we will need to add the next option to the below execution lines:
--alpha 0.2


* A 1, 2 or 3D _square_ domain
* All boundaries are held at constant temperature of 0
* Initial condition is a _hat function_

## The Example Source Code

[transient-heat.cpp](transient-heat.cpp)

## Running the Example

### Run 1 (Problem Name)

Give the command-line to run the example

#### Expected Behavior/Output

Include here what learner should expect to happen

* How long might it take to run
* How long might they have to wait for resources before it can run
* What should they seen on their terminal

#### Examining Results

Include here examples of either plots or data you expect learners to observe.

![An Image](basic0000.png)

Or, if you need to control the size, or have multiple images next to each other
use a Markdown table and raw html...

|<img src="basic0000.png" width="200">|<img src="basic0000.png" width="400">|

**Note:** You can create [gif animations](https://www.tjhsst.edu/~dhyatt/supercomp/n401a.html)
with ImageMagick tool available on most systems as `convert` command as in...

```
convert -delay 20 -loop 0 image*.<ext> animation.gif
```

![Gif Animations](animated_basic_heat.gif)

Alternatively, you can upload videos to YouTube and embed them here 

<iframe width="560" height="315" src="https://www.youtube.com/embed/bsSFYrDXK0k" frameborder="0" allowfullscreen></iframe>

#### Questions

> **Question #1?** (triple-click box below to reveal answer)

|<font color="white">Answer to Question #1</font>|

> **Question #2?** (triple-click box below to reveal answer)

|<font color="white">Answer to Question #2</font>|

---

### Run 2 (Problem Name)

#### Expected Behavior/Output

#### Examining Results

Include here examples of either plots or data you expect learners to observe.

#### Questions

> **Question #1?** (triple-click box below to reveal answer)

|<font color="white">Answer to Question #1</font>|

> **Question #2?** (triple-click box below to reveal answer)

|<font color="white">Answer to Question #2</font>|

---

### Run 3

#### Expected Behavior/Output

#### Examining Results

Include here examples of either plots or data you expect learners to observe.

#### Questions

> **Question #1?** (triple-click box below to reveal answer)

|<font color="white">Answer to Question #1</font>|

> **Question #2?** (triple-click box below to reveal answer)

|<font color="white">Answer to Question #2</font>|

---

## Out-Brief

This application is substantially more flexible than the custom-coded application,
[heat.c](../hand_coded_heat/heat.c.numbered) from [Lesson 1](../hand_coded_heat/lesson.md)
It can handle 1, 2 or 3 dimensional mesh.

Here, re-emphasize the lesson objectives and key points.

Its fine to go into greater detail about questions or objectives this lesson
did not fully cover.

### Further Reading

https://www.phy.ornl.gov/csep/pde/node2.html

Include links to other online sources you might want to include.

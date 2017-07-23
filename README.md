## Welcome to HandsOnLessons


Hosted here are a series of increasingly sophisticated _hands-on_ lessons aimed at helping
users of all experience levels learn to use a variety of scientifc software packages for
solving complex numerical problems. We begin with custom, hand-coded solutions to the
homogeneous, one dimensional heat equation demonstarting basic numerical and software engineeriing
issues such as accruacy, stability, time to solution, memory and flops required.

[Go to the Lessons](lessons/lessons.md)

We slowly build upon
these simple, early examples introducing additional complexities such as inhomogenieties,
higher-order solutions, time-variabilities, non-linearities and complex geometries in higher
dimenions. We demonstrate the use of a variety of numerical software packages to address these
issues and the advantages they offer over custom, hand-coded software.

Throughout the currently designed lesson plans, we use the [MFEM](http://mfem.org) (unstructured)
and [AMReX](https://github.com/AMReX-Codes/AMReX-Codes.github.io) (structured, adaptive)
packages as _demonstration vehicles_. Both of these packages include the basic functional pieces
necessary to start from a continuous description of a physical problem to solve, through
PDE specification, numerical analysis, discretization, algorithm development and then implementation.
In addition, they include essential abstractions to support scalable, parallel expression
of the algorithms and to orchestrate the application of various numerical packages in the
solution.

In addition, we use [PAPI](http://icl.utk.edu/papi) (performance counters) to enable users to
observe variations in performance (time and space) as algorithmic choices are varied and
[VisIt](http://visit.llnl.gov) to visualize results.

In addition, the packages demonstrated here benefit from many person-years of software development
aimed at addressing such issues as extreme scalability and multi-modal parallelism such as message-passing,
many-threads and/or GPUs. Finally, numerical results computed with the packages demonstrated here have been vetted over
many years of use in a variety of application settings. Nonetheless, because the main focus of these
lessons is in **introducing** how to use these packages, there are likely few specific hands-on lessons
here in which there are opportunities to observe these important capabilities of numerical software packages. 

See [Contributing Guide](contribution_guide.md) for instructions on contributing lessons.

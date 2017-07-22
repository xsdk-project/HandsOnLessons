## Welcome to HandsOnLessons

Hosted here are a series of increasingly sophisticated _hands-on_ lessons aimed at helping
users of all experience levels learn to use a variety of scientifc software packages for
solving complex numerical problems. We begin with custom, hand-coded solutions to the
homogeneous, one dimensional heat equation demonstarting basic numerical and software engineeriing
issues such as accruacy, stability, time to solution, memory and flops required.

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

Testing an equation...

![equation](http://latex.codecogs.com/gif.latex?%5Cfrac%7B%5Cpartial%20Q%7D%7B%5Cpartial%20t%7D%20%3D%20%5Cfrac%7B%5Cpartial%20s%7D%7B%5Cpartial%20t%7D)

You can use the [editor on GitHub](https://github.com/xsdk-project/HandsOnLessons/edit/master/README.md) to maintain and preview the content for your website in Markdown files.

Whenever you commit to this repository, GitHub Pages will run [Jekyll](https://jekyllrb.com/) to rebuild the pages in your site, from the content in your Markdown files.

### Markdown

Markdown is a lightweight and easy-to-use syntax for styling your writing. It includes conventions for

```markdown
Syntax highlighted code block

# Header 1
## Header 2
### Header 3

- Bulleted
- List

1. Numbered
2. List

**Bold** and _Italic_ and `Code` text

[Link](url) and ![Image](src)
```

For more details see [GitHub Flavored Markdown](https://guides.github.com/features/mastering-markdown/).

### Jekyll Themes

Your Pages site will use the layout and styles from the Jekyll theme you have selected in your [repository settings](https://github.com/xsdk-project/HandsOnLessons/settings). The name of this theme is saved in the Jekyll `_config.yml` configuration file.

### Support or Contact

Having trouble with Pages? Check out our [documentation](https://help.github.com/categories/github-pages-basics/) or [contact support](https://github.com/contact) and we’ll help you sort it out.

# Lesson Title

## At A Glance (Expected # minutes to complete)

**Note**: GitHub Markdown tables are very limited! To do this section of questions,
objectives and key points properly, we need to use more features of Jekyll then I
wanna worry about prior to ATPESC. We will fix this after ATPESC to use Jekyll
properly and it will improve its look substantially. Also, to avoid horizontal scroll
of this pre-formatted section, try to keep to less than 102 chars in width.

```
Questions                  |Objectives                      |Key Points
---------------------------|--------------------------------|----------
Question 1?                |Objective 1                     |Key Point 1
Question 2?                |Objective 2                     |Key Point 2
Question 3?                |Objective 3                     |Key Point 3
```

* **Questions** are the those things we want learners to know the answers to by the end of the lesson.
We don't have to list all possible questions here...only the two or three _most_ important.
* **Objectives** are those things we want learners to actually do or observe during the lesson. Again,
only list here the ones that are _most_ important.
* **Key Points** are those things we want learners to take-away from the lesson.

## The problem being solved

Describe the problem(s) that will be solved in this lesson.
If possible, include a picture or graphic here describing the physical problem setup. If the application
or tool being used can deal with a variety of input physical problems, its fine to mention
that but here just include a picture of the problem they will be running in the _runs_
below. Maybe include the equation being solved as well.

![](http://latex.codecogs.com/gif.latex?%5Cfrac%7B%5Cpartial%20u%7D%7B%5Cpartial%20t%7D%20%3D%20%5Calpha%20%5Cfrac%7B%5Cpartial%5E2%20u%7D%7B%5Cpartial%20x%5E2%7D)

## The Example Source Code

Describe the application, its command-line arguments, have a link to view the actual source code
or, if you prefer, include snipits of the source code here in a code-highlighted box as below

```c++
Geometry::~Geometry()
{
   for (int i = 0; i < NumGeom; i++)
   {
      delete PerfGeomToGeomJac[i];
      delete GeomVert[i];
   }
}
```

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

**Note:** These Questions and _Answer Boxes_ are somewhat cheesey for time being.
We can expand our use of Jekyll and improve look and feel after ATPESC. In meantime,
in order for these _Answer Boxes_ to behave as desired (e.g. hidden text which
gets revealed by user triple-clicking in box), they have to be all on a single
line with no line breaks and have to be white text on white backgroud. Yeah, its
cheesey but will work for now.

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

Here, re-emphasize the lesson objectives and key points.

Its fine to go into greater detail about questions or objectives this lesson
did not fully cover.

### Further Reading

Include links to other online sources you might want to include.

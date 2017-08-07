
# Instructions for Numerical Package Hands-on

## Basic Instructions

Hands-on exercises are primarily run on cooley cluster at ALCF. Please do the following initial setup

 - login to cooley
```
ssh username@cooley.alcf.anl.gov
```
 - setup software environment on cooley by adding the following to  _~/.soft.cooley_ (preferably before `@default` line)
```
+mvapich2
+gcc-4.8.1
@visit
PATH+=/projects/ATPESC2017/NumericalPackages/spack/bin
MPIRUN_SPACK=/projects/ATPESC2017/NumericalPackages/spack/opt/spack/linux-rhel6-x86_64/gcc-4.8.1/openmpi-2.1.1-5b4k4f3vzgwz5qmektcqja2av4c4bjrg/bin/mpiexec
MPIRUN=/usr/bin/mpirun
```
  - run the following command to make the above change to _~/.soft.cooley_ effective
```
resoft
```
  - obtain 3 compute nodes in _interactive_ (`-I`) mode to run the hands on exercises by running the following `qsub` command...
```
qsub -I -n 3 -t 720 -A ATPESC2017
```
   - copy over precompiled binaries and data files to your home dir.
```
cp -r /projects/ATPESC2017/NumericalPackages/handson ~/
```
   - Now proceed to run the Hands-On exercises as instructed. Each lesson will
     indicate the path in the `handson` directory you copied above you should
     `cd` to in order to begin the lesson.

## Optional Visualization Instructions

Some of the hands-on exercises have optional visualization instructions
and use a variety of visualization tools.

Getting a room of more than 70 people with different laptops working with
remote visualization from cooley is not something either our tight agenda or
our staff are prepared to support. In addition, where necessary, the hands-on
leader(s) will demonstrate the use of some tools and learners can follow 
along at those points.

There is a whole day of activity devoted to visualization resources and
tools in the ATPESC agenda on August 9th.

Nonetheless, we are providing here some instructions and tools for those
would like to persue getting remote visualization working for these
exercises.

### Note
  - For basic graphics exercises - you should be able to use X11 over ssh.
    i.e `ssh -C -X -Y cooley.alcf.anl.gov, ssh -X compute_node_allocated`
    and run the graphics part of the exercise. That said, many post-2014
    X servers have _INdirect GLX_ disabled in them breaking tools like
    paraview, VisIt and glvis.

We have a devloped a script (to be run on your laptop) that can help with
setting up a VNC connection to cooley. It can work on MacOS, and with some
Linux VNC clients (vinagre, vncviewer). However the script is fragile and
might not work with everyones setup - hence this is optional. This script
will do several things

 - Logs you into cooley once with your token
 - Sets up SSH Control Master so you only have to log into cooley with your token once for the whole day and password-less will work thereafter
 - Reserves 3 nodes for 12h on cooley.
 - Sets up a VNC connection to those nodes

   Mac and Linux users are welcome download and run this script to
   setup a VNC connection. Doing so will permit quick use of tools
   like VisIt, paraview and/or glvis. 

To try this setup script...

On Linux,
```
wget https://raw.githubusercontent.com/xsdk-project/HandsOnLessons/master/tools/atpesc2017_cooley_vnc_setup.sh
```

On Mac,

```
curl https://raw.githubusercontent.com/xsdk-project/HandsOnLessons/master/tools/atpesc2017_cooley_vnc_setup.sh > atpesc2017_cooley_vnc_setup.sh
```

Ensure the script has execute permissions...
```
chmod 755 atpesc2017_cooley_vnc_setup.sh
```

Now, try running the script

```
./atpesc2017_cooley_vnc_setup.sh <your_cooley_username>
```

### Notes
  - If allocating nodes via this script - please deallocate nodes that you might have previously allocated
    by simply logging out of the interactive allocation.

### Troubleshooting
  - If you have a different preferred Linux VNC client - you should be able to use it to connect to the VNC connection that is already setup by this script. For example,
```
krdc vnc://localhost:22590
```
  - When rerunning the script - if the ssh command to setup VNC tunnel fails - you
    might have to kill the _ssh control master_ process and restart again. Its easiest
    to simply find all ssh logins to cooley and kill them
```
$ ps -ef | grep cooley
3640  7348   694   0  4:58PM ttys003    0:00.01 grep cooley
3640  7345 62009   0  4:58PM ttys004    0:00.03 ssh -C -X -Y cooley.alcf.anl.gov
3640  7347 62009   0  4:58PM ttys004    0:00.03 ssh -L 22590:cc122:5900 cooley.alcf.anl.gov
$ kill -9 7345 7347
```
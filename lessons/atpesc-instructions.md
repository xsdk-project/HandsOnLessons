
# Setup Instructions for Numerical Package Hands-on session

## Basic Instructions

Hands-on exercises are primarily run on cooley cluster at ALCF. Please do the following initial setup

 - login to cooley.
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
  - run the following command to make the above change to _~/.soft.cooley_ effective.
```
resoft
```
  - obtain compute nodes in interactive mode to run the hands on exercises by running the following `qsub` command. [for ex: requesting 3 nodes for 12h]
```
qsub -I -n 3 -t 720 -A ATPESC2017
```
   - < more instructions - copy files - lable which tutorial corresponds to which file etc>     

   - Now proceed to run the Hands-On exercises as instructed at <<URL?>>


## Optional Visualization Instructions

Some of the hands-on exercises have optional visualization instructions. This requires a VNC connection from your laptop to cooley. We have a convince script (to be run on your laptop) that can help with setting up a VNC connection to cooley. It can work on MacOS, and with some Linux VNC clients (vinagre, vncviewer). However the script is fragile and might not work with everyones setup - hence this is optional. This script will do several things

 - Logs you into cooley once with your token
 - Sets up SSH Control Master so you only have to log into cooley with your token once for the whole day and password-less will work thereafter
 - Reserves 3 nodes for 12h on cooley.
 - Sets up a VNC connection to those nodes

   Please download the script from <<URL>> and run it as:
```
./morning_setup.sh your_cooley_userid
```

### Notes
  - If allocating nodes via this script - please deallocate nodes that you might have previously allocated [via basic instructions]

### Troubleshooting
  - If you have a different preferred Linux VNC client - you should be able to use it to connect to the VNC connection that is already setup by this script. For eg:
```
krdc vnc://localhost:22590
```
  - When rerunning the script - if the ssh command to setup VNC tunnel fails - you might have to kill the 'ssh control master' process and restart again. On Linux - one can discover process this with:
```
netstat -tp |grep cooley
```


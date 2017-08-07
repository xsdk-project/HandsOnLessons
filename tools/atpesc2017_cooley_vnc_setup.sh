#!/bin/sh
acct=ATPESC2017
nnodes=3
tl=720
localos=`uname`
linuxvnc=''

if [[ -z "$1" ]]; then
    echo "Usage: $0 <cooley_username> [debug]"
    echo "    if 'debug' present reduces allocation to 1 node @ 20 mins"
    exit 1
fi
cooley_username=$1

if [[ "$2" == debug ]]; then
    set -x
    nnodes=1
    tl=20
fi

if [[ "$localos" == "Linux" ]]; then
    if [  -f /usr/bin/vncviewer ]; then
        linuxvnc=vncviewer
    elif [  -f /usr/bin/vinagre ]; then
        linuxvnc=vinagre
    else
         echo "Please install vncviewer (from TigerVNC) or vinagre and rerun the script"
         exit 1
    fi
fi

# Ensure ~/.ssh/config exists and has limited permissions
if [[ ! -e ~/.ssh/config ]]; then
    if [[ ! -e ~/.ssh ]]; then
        mkdir ~/.ssh
        chmod 700 ~/.ssh ~/.ssh/config
    fi
    touch ~/.ssh/config
    chmod 700 ~/.ssh/config
fi

#
# Append stuff to ~/.ssh/config for ssh control master to cooley
#
if [[ -z "$(grep cooley-nph ~/.ssh/config)" ]]; then
cat >> ~/.ssh/config << EOF
#added by NumericalPackagesHandsOn
Host cooley-nph cooley.alcf.anl.gov
    Compression yes
    ControlMaster auto
    ControlPersist 12h
    ControlPath ~/.ssh/cm_socket/%r@cooley.alcf.anl.gov:%p
EOF
fi

#
# open login to cooley (will prompt) and put in bg and keep open all day
# This is the login that all others will use shared authentication with
#
ssh -N -f ${cooley_username}@cooley.alcf.anl.gov

#
# copy vnc dot files to cooley prompt for desired vnc password
#
ssh ${cooley_username}@cooley-nph "mkdir -p  ~/.vnc; cat > ~/.vnc/xstartup" << EOF
#!/bin/bash
#created by NumericalPackagesHandsOn
export DISPLAY=:0.0
export HANDSON=/projects/ATPESC2017/NumericalPackages/handson/
xterm -fn 10x20 &
twm
EOF
ssh ${cooley_username}@cooley-nph "chmod u+x ~/.vnc/xstartup"
#
# Update users .soft.cooley file for basic setup
#
#ssh ${cooley_username}@cooley-nph "cat >> ~/.soft.cooley" << EOF
##added by NumericalPackagesHandsOn
#+gcc-4.8.1
#@visit
#EOF
#
# Get a temporary password from user and confirm its intended
#
while true; do
    read -p "Create temporary VNC Password: " pw
    echo "You have entered \"$pw\", is this correct?"
    select yn in "Yes" "No"; do
        case $yn in
            Yes ) break 2;;
        esac
    done
done
# Push the password to cooley and vncpasswd encode it
ssh ${cooley_username}@cooley-nph "rm -f ~/.vnc/passwd; echo $pw | vncpasswd -f > ~/.vnc/passwd; chmod 600 ~/.vnc/passwd"

#
# Reserve 3 nodes for interactive use all day
#
ssh -t -t -f ${cooley_username}@cooley-nph "qsub -I -n $nnodes -t $tl -A $acct" > ./qsub-interactive.out 2>&1 &

#
# Loop watching output from above to get allocation node name
#
nodid=""
while [[ -z "$nodid" ]] ; do
    echo "Checking for allocation completion"
    nodid=$(cat ./qsub-interactive.out | tr ' ' '\n' | grep cc[0-9][0-9][0-9].cooley | cut -d'.' -f1)
    sleep 5
done
echo "Got allocation at $nodid"

#
# Startup xvncserver on the allocation
#
ssh ${cooley_username}@cooley-nph "nohup ssh $nodid x0vncserver --display=:0.0 --NeverShared=1 --geometry=2400x1500+0+0 --PasswordFile=/home/$cooley_username/.vnc/passwd --MaxProcessorUsage=100 >& /dev/null &"
sleep 5 

#
# Set up 2-hop ssh tunnel to allocation, (above) through login and run xstartup there
#
ssh -f -L 22590:$nodid:5900 ${cooley_username}@cooley-nph "nohup ssh $nodid ~/.vnc/xstartup >& /dev/null &"
sleep 5 

#
# finally, start the vnc client on local machine
#
echo "Attempting to connect VNC to localhost:22590 - If this fails you can reattempt this manually"
if [[ "$localos" == Darwin ]]; then
    open vnc://localhost:22590
elif [[ "$localos" == Linux ]]; then
    $linuxvnc localhost::22590
elif [[ "$localos" == windows ]]; then
    echo "not implemented"
fi

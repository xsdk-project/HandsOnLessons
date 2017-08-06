#!/bin/sh -x

acct=ATPESC17_Instructors
nnodes=1
tl=20
cooley_username=mcmiller
cooley_shell=tcsh
localos=osx

# Ensure ~/.ssh/config exists and has limited permissions
if [[ ! -e ~/.ssh/config ]]; then
    if [[ ! -e ~/.ssh ]]; then
        mkdir ~/.ssh
        chmod 700 ~/.ssh ~/.ssh/config
    fi
    touch ~/.ssh/config
    chmod 700 ~/.ssh/config
fi

cat >> ~/.ssh/config << EOF
Host cooley cooley.alcf.anl.gov
    User $cooley_username
    Compression yes
    CompressionLevel 9
    ControlMaster auto
    ControlPersist 12h
    ControlPath ~/.ssh/cm_socket/%r@cooley.alcf.anl.gov:%p
EOF

# open login to cooley (will prompt) and put in bg
# keep open all day
ssh -N -f cooley.alcf.anl.gov 

# copy vnc dot files to cooley
# prompt for desired vnc password
ssh cooley "cat > ~/.vnc/xstartup" << EOF
#!/bin/sh
xterm &
twm
EOF
ssh cooley "chmod u+x ~/.vnc/xstartup"

while true; do
    read -p "Create temporary VNC Password: " pw
    echo "You have entered \"$pw\", is this correct?"
    select yn in "Yes" "No"; do
        case $yn in
            Yes ) break 2;;
        esac
    done
done
ssh cooley "rm -f ~/.vnc/passwd; echo $pw | vncpasswd -f > ~/.vnc/passwd; chmod 600 ~/.vnc/passwd"

# Reserve 3 nodes for interactive use all day
# get back allocation machine 'name' and start
# xvnc server there
ssh -t -t -f cooley "qsub -I -n $nnodes -t $tl -A $acct" > ./qsub-interactive.out 2>&1 &
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
ssh cooley "nohup ssh $nodid x0vncserver --display=:0.0 --NeverShared=1 --geometry=2400x1500+0+0 --PasswordFile=/home/$cooley_username/.vnc/passwd --MaxProcessorUsage=100 >& /dev/null &"
sleep 5 

# Set up 2-hop ssh tunnel to allocation, (above) through
# login and run xstartup there
if [[ "$cooley_shell" == tcsh ]]; then
    ssh -f -L 22590:$nodid:5900 cooley "nohup ssh $nodid 'setenv DISPLAY :0.0; ~/.vnc/xstartup' >& /dev/null &"
else
    ssh -f -L 22590:$nodid:5900 cooley "nohup ssh $nodid 'export DISPLAY=:0.0; ~/.vnc/xstartup' >& /dev/null &"
fi
sleep 5 

if [[ "$localos" == osx ]]; then
    open vnc://localhost:22590
elif [[ "$localos" == linux ]]; then
    echo "not implemented"
elif [[ "$localos" == windows ]]; then
    echo "not implemented"
fi

echo "WARNING: soft add +gcc-4.8.1"
echo "WARNING: set path=($path /projects/ATPESC2017/NumericalPackages/spack/bin)"
echo "WARNING: set path to root of hands on"


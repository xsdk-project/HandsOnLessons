#!/bin/sh -x

acct=ATPESC17_Instructors
cooley_username=mcmiller
nnodes=1
tl=5
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
ssh cooley "rm -f ~/.vnc/password; cat > ~/.vnc/password" << EOF
$pw
EOF

# Reserve 3 nodes for interactive use all day
# get back allocation machine 'name' and start
# xvnc server there
qsub_job_info=$(ssh -f cooley "qsub -I -n $nnodes -t $tl -A $acct")
nodid=$(echo $qsub_job_info | tr ' ' '\n' | grep cc[0-9][0-9][0-9].cooley)

#
# Startup xvncserver on the allocation
#
ssh cooley "ssh $nodid x0vncserver --display=:0.0 --NeverShared=1 --geometry=2400x1500+0+0 --PasswordFile=/home/$cooley_username/.vnc/passwd --MaxProcessorUsage=100"

# Set up 2-hop ssh tunnel to allocation, (above) through
# login and run xstartup there
if [[ "$cooley_shell" == tcsh ]]; then
    ssh -f -L 22590:$nodid:5900 cooley "ssh $nodid 'setenv DISPLAY :0.0; ~/.vnc/xstartup'"
else
    ssh -f -L 22590:$nodid:5900 cooley "ssh $nodid 'export DISPLAY=:0.0; ~/.vnc/xstartup'"
fi

if [[ "$localos" == osx ]]; then
    open vnc://localhost:22590 &
elif [[ "$localos" == linux ]]; then
    echo "not implemented"
elif [[ "$localos" == windows ]]; then
    echo "not implemented"
fi

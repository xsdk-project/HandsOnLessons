#!/bin/sh

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
ssh cooley "chmod u+x ~/.vnc/xstartup

while true; do
    read -p "Create temporary VNC Password: " pw
    echo "You have entered \"$pw\", is this correct?"
    select yn in "Yes" "No"; do
        case $yn in
            Yes ) exit;;
            No ) break;;
        esac
    done
done
ssh cooley "cat > ~/.vnc/password" << EOF
$pw
EOF

# Reserve 3 nodes for interactive use all day
# get back allocation machine 'name' and start
# xvnc server there
ssh cooley "qsub -I -n 1 -t 5 -A ATPESC17_Instructors" 

# Set up 2-hop ssh tunnel to allocation, (above) through
# login and run xstartup there

# OSX: open vnc://localhost:22590
# Windows:
# Linux

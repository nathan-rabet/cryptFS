USER="$1" # user name
if [ -z "$USER" ]; then
    USER=$(whoami)
fi

echo "Connecting to VM as $USER"

if [ ! $(ssh $USER@localhost -p 2222) ]; then
    ssh-keygen -f "/home/$USER/.ssh/known_hosts" -R "[localhost]:2222"
    ssh $USER@localhost -p 2222
fi

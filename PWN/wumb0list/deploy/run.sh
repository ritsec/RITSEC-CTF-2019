useradd -m $CONFIG_USERNAME -k /etc/skel -s /bin/bash
mkdir -p /home/$CONFIG_USERNAME/.ssh
echo "$CONFIG_SSHKEY" > /home/$CONFIG_USERNAME/.ssh/authorized_keys
chown -R $CONFIG_USERNAME:$CONFIG_USERNAME /home/$CONFIG_USERNAME/.ssh
/usr/sbin/sshd -D

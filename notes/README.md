Quick notes from the CTF

The socat implementation of jit-calc is not the best.  To prevent excessive
resource usage I set up a cron job with kill all docker images every 10 minutes
"sudo docker kill $(sudo docker ps -q).  ~~You also have to monitor the socat 
log in another window because some attempts to exploit it will kill the listener.
To monitor and/or restart it ssh into the jit-calc server, run tmux attach, and 
run sudo ./socat.sh if it is not already running, detach with Ctrl+B then hit D.~~
knif3 made an amazing service script which will automatically restart when it 
crashes.

Potat0 has some problems with people vandalizing it, since I don't have a good
way to catch this I have a cron job set up to reset it every 10-20 minutes along
with the downDetector scans.

For URGGGGGGGG if it looks like they have 90% of the flag but the words "said"
and "networking" are mangled inform them that their script isn't parsing all
keypresses correctly and to double check it

Saturday November 16th at 7:30 rochester had a power flicker which took down
openstack.  The contestants were notified via announcement at 7:34pm.  Luckily 
about half of the boxes recovered automatically within 15m by powering on and 
downDetector running.  The second half had to be manually migrated by neon_spandex and
downDetector redeployed the challenges.  Jit-calc and the_vault had to manually 
redeployed.  All boxes were back up by 8:15 and confirmed to be up via 
downDetector, the recovery was announced to the contestants at 8:35.  Total 
downtime was under one (1) hour.


Note for infra: the challenges not covered by the nginx transparent proxy use 
iptables routing except for the uplink1/2 and the_vault which are directly
connected to the internet and accessable by the jumpbox.
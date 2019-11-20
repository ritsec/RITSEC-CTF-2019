#!/bin/bash

# RITSEC Fall 2019
# Security Risk Advisors
# Forensics: findme.pcap 
#   __ _           _                
#  / _(_)_ __   __| |_ __ ___   ___ 
# | |_| | '_ \ / _` | '_ ` _ \ / _ \
# |  _| | | | | (_| | | | | | |  __/
# |_| |_|_| |_|\__,_|_| |_| |_|\___|
#

# Run our nc command
nc -w 2 18.219.169.113 1337 > findme

# Base64 decode
base64 -d findme > data

# Uncompress using tar
tar xvf data

# Display the contents of the flag file
cat flag

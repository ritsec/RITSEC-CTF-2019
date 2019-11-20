FROM ubuntu:18.04

WORKDIR /wumb0list
ENV DEBIAN_FRONTEND noninteractive

# setup tools and packages
RUN apt-get update && apt-get upgrade -y
RUN apt-get install -y git vim python3 python3-pip gcc make openssh-server locales python python-pip
RUN python3 -m pip install virtualenv pexpect
RUN python -m pip install virtualenv pwntools

# set locale
RUN locale-gen en_US.UTF-8 
ENV LANG en_US.UTF-8
ENV LANGUAGE en_US:en
ENV LC_ALL en_US.UTF-8

# copy files
COPY wumb0list.c /wumb0list/wumb0list.c
COPY Makefile /wumb0list/Makefile
COPY flag.txt /wumb0list/flag.txt
COPY run.sh /run.sh

# build challenge
RUN make && rm wumb0list.c Makefile
RUN strip wumb0list
RUN chown root:root /wumb0list/wumb0list /wumb0list/flag.txt
RUN chmod 4755 /wumb0list/wumb0list
RUN chmod 600 /wumb0list/flag.txt

RUN mkdir /run/sshd

# run initial setup script
CMD /run.sh

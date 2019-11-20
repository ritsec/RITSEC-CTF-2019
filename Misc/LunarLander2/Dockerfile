FROM ubuntu:16.04

RUN apt update -y
RUN apt install -y git libncurses-dev liballegro-dev g++ libgtk2.0-dev libwxgtk3.0-dev

# Clone the AGC
WORKDIR /opt
RUN git clone https://github.com/virtualagc/virtualagc
WORKDIR /opt/virtualagc
RUN make

ENV PATH=$PATH:/opt/virtualagc/VirtualAGC/temp/lVirtualAGC/bin/

WORKDIR /root

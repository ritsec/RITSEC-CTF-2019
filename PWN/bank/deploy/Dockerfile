FROM ubuntu:18.04

WORKDIR /bank
ENV DEBIAN_FRONTEND noninteractive

# setup tools and packages
RUN apt-get update && apt-get upgrade -y
RUN apt-get install -y git vim python3 python3-pip make openssh-server locales python python-pip libssl-dev libc++1 sqlite3 clang-8 libsqlite3-dev libc++-dev libc++abi-dev netcat
RUN apt-get install -y 
RUN python3 -m pip install virtualenv pexpect
RUN python -m pip install virtualenv pwntools

# set locale
RUN locale-gen en_US.UTF-8 
ENV LANG en_US.UTF-8
ENV LANGUAGE en_US:en
ENV LC_ALL en_US.UTF-8

# copy files
COPY *.cpp /bank/
COPY *.h /bank/
COPY bank.sql /bank/
COPY flag2.txt /bank/flag.txt
COPY Makefile /bank/
COPY run.sh /run.sh

# build challenge
RUN make && rm *.cpp *.h Makefile *.sql
RUN chown root:root /bank/bank /bank/flag.txt /bank/bank.db
RUN chmod 4755 /bank/bank
RUN chmod 600 /bank/bank.db /bank/flag.txt

RUN mkdir /run/sshd

# run initial setup script
CMD /run.sh

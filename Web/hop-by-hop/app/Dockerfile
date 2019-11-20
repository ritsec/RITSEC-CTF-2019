FROM ubuntu:18.04
MAINTAINER Chaim Sanders chaim.sanders@gmail.com

ENV DEBIAN_FRONTEND noninteractive

RUN apt-get update && \
    apt-get -y install python3-pip \
      apache2 \
      libapache2-mod-wsgi-py3 \
      build-essential \
      vim \
      wget \
      libldap2-dev \
      libsasl2-dev


COPY ./app/requirements.txt /var/www/html/challenge/requirements.txt
RUN pip3 install -r /var/www/html/challenge/requirements.txt
COPY ./challenge.conf /etc/apache2/sites-available/challenge.conf
COPY ./challenge.wsgi /var/www/html/challenge/challenge.wsgi
RUN a2dissite 000-default.conf
RUN a2ensite challenge
COPY entrypoint.sh /opt/entrypoint.sh


ENTRYPOINT ["/bin/sh", "/opt/entrypoint.sh"]

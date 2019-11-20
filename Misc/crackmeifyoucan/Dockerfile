from debian
RUN apt-get update; apt-get install git python3 python3-pip -y --force-yes
RUN git clone https://github.com/danielmiessler/SecLists.git
COPY CrackMeIfYouCan.py / 
COPY requirements.txt /
RUN pip3 install -r requirements.txt
EXPOSE 5555
CMD python3 CrackMeIfYouCan.py

FROM python:3.7.3-slim

WORKDIR /app
ADD . /app

RUN pip3 install -r requirements.txt
EXPOSE 5000

CMD ["python3", "misdirection.py"]

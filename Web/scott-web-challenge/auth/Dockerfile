FROM node:8

WORKDIR /usr/src/app

COPY ./package.json ./

RUN npm install

COPY ./auth.js .
COPY ./keys/ ./keys/

EXPOSE 3000

CMD [ "node", "auth.js" ]

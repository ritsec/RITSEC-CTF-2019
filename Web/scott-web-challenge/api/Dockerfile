FROM node:8

WORKDIR /usr/src/app

COPY ./package.json ./

RUN npm install

COPY ./api.js .
COPY ./keys/ ./keys/
COPY ./index.html .

EXPOSE 4000

CMD [ "node", "api.js" ]

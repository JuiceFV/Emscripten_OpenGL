FROM emscripten/emsdk:latest 

RUN mkdir /app

WORKDIR /app

COPY . /app

RUN mkdir build
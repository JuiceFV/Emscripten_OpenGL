FROM emscripten/emsdk:latest 

RUN sudo apt-get update -y

RUN sudo apt-get install -y libpython3-dev

RUN mkdir /app

WORKDIR /app

COPY . /app

RUN mkdir build
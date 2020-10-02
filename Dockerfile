FROM emscripten/emsdk:latest 

RUN apt-get -q update 

RUN mkdir /app

WORKDIR /app

COPY . /app

RUN em++ application/main.cpp -o application/out/app.html

RUN pip3 install aiohttp

RUN pip3 install aiohttp_jinja2

RUN pip3 install jinja2

RUN ls application/out
FROM emscripten/emsdk:latest 

RUN mkdir /app

WORKDIR /app

COPY . /app

RUN pip3 install aiohttp

RUN pip3 install aiohttp_jinja2

RUN pip3 install jinja2

ENTRYPOINT ./execapp.sh
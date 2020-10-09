#!/bin/bash
echo "Installing required packages..."
pip3 install aiohttp
pip3 install aiohttp_jinja2
pip3 install jinja2
cd build
echo "Compiling C++ code to the WASM. Writes them over './application/out/' directory"
emcmake cmake -DFOR_EMSDK=ON ..
emmake make
echo "Runing the server. Follow the link http://localhost:8080"
python3 ./application/entry.py

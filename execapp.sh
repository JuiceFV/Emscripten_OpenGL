#!/bin/bash
# generate yaml lib
cd build
echo "Compiling C++ code to the WASM. Writes them over './application/out/' directory"
emcmake cmake -DFOR_EMSDK=ON ..
emmake make
node Emscripten_Graphics.js
#echo "Runing the server. Follow the link http://localhost:8080"
#python3 /app/application/entry.py

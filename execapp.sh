#!/bin/bash
# generate yaml lib
echo "Compiling C++ code to the WASM. Writes them over './application/out/' directory"
em++ /app/application/main.cxx -o /app/application/out/app.html
echo "Runing the server. Follow the link http://localhost:8080"
python3 /app/application/entry.py

#!/bin/bash
cd build
emcmake cmake -DFOR_EMSDK=ON -DENABLE_TESTS=ON ..
emmake make
make test
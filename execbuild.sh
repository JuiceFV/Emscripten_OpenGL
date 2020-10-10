#!/bin/bash
cd build
emcmake cmake -DFOR_EMSDK=ON ..
emmake make
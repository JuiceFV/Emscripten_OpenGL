#!/bin/bash
cd build
emcmake cmake -DFOR_EMSDK=ON -DENABLE_TESTS=ON -DTESTS_VERBOSE=ON ..
emmake make
make tests
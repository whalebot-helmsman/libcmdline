#!/usr/bin/env bash

cmake -DCMAKE_BUILD_TYPE=Release ./
make
make test

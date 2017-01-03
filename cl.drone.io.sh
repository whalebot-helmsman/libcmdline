#!/usr/bin/env bash

mkdir gtest
pushd gtest
wget https://github.com/google/googletest/archive/release-1.6.0.zip
unzip release-1.6.0.zip
pushd googletest-release-1.6.0
cmake -DCMAKE_BUILD_TYPE=Release
make
sudo cp libgtest* /usr/lib
sudo mkdir /usr/include/gtest
sudo cp -r include/gtest/* /usr/include/gtest
popd
popd

cmake -DCMAKE_BUILD_TYPE=Release ./
make
make test

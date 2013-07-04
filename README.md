libcmdline
==========
pure-c library to handle command line arguments

Usage
=====
see [example.c](https://bitbucket.org/whalebot_helmsman/libcmdline/src/ba05097855e23daad62daf08e70bf2cdca593b48/example/example.c?at=default)

Installation
============
Building, from source root

    mkdir build
    cd build
    cmake -DCMAKE_BUILD_TYPE=Release ../
    make

Requirements
============
This library is requirenments free, you only need c/c++ compiler

Testing
=======
[![Build Status](https://drone.io/bitbucket.org/whalebot_helmsman/libcmdline/status.png)](https://drone.io/bitbucket.org/whalebot_helmsman/libcmdline/latest)
For testing you need [gtest](http://code.google.com/p/googletest/)
After build you need to run

    make test



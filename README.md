Overview
========

This arduino project is used to light some led strips under my bed, on motion.

Build
=====

    # prepare submodules (first time only)
    git submodule init
    git submodule update

    # go to build dir
    cd build/

    # initialize cmake (first time only)
    cmake ..

    # build project
    make

    # upload to arduino (must be connected)
    make bedroom-lights-upload

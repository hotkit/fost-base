Fost -- Core libraries
Copyright 1995-2009 Felspar Co. Ltd.
http://fost.3.felspar.com/


This readme is for users of Ubuntu 9.10 or later.


==Step 1: Installing dependencies==

You will need a few libraries and tools.

    apt-get install g++ libbz2-dev libssl-dev python-dev uuid-dev valgrind boost-build libboost-dev


==Step 2: Checking out Fost==

You should create a new clean directory and into that dirctory you check out the Fost and Boost libraries from the Felspar repository. The directory should look like this:

parent
 |-Boost
 |-fost-base
    |-README.karmic.txt (location of this file)

Something like the below will do this for you:

    mkdir fost
    cd fost
    svn co http://svn.felspar.com/external/Boost
    svn co http://svn.felspar.com/public/fost-base/stable fost-base


==Step 3: Telling Fost which version of Boost to use==

Boost is already installed on the machine from step, but we have to configure Fost so it knows to use it.

    Boost/install karmic


==Step 4: Building fost-base==

We can now build the core library examples.

    fost-base/compile examples


==Step 5: Running the examples==

Because the example programs and libraries have not been installed to anywhere we must set the LD_LIBRARY_PATH to point between the executable and library locations.

    export LD_LIBRARY_PATH=`pwd`/dist/lib

We can now run various examples.

    dist/bin/hello-d
    dist/bin/fizzbuzz-d
    dist/bin/fprettyjson-d fost-base/Examples/fprettyjson/sample.json

Each of the programs has a -d at the end of their file name because they are debug builds. For a release build try the following:

    fost-base/compile examples release
    dist/bin/hello
    dist/bin/fizzbuzz
    dist/bin/fprettyjson fost-base/Examples/fprettyjson/sample.json


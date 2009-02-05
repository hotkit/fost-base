<<<<<<< .working
Fost -- Core libraries
Copyright 1995-2008 Felspar Co. Ltd.
http://fost.3.felspar.com/


These core libraries are experimental versions of the new Fost 4 system. Felspar does make use of them for production code, but doesn't recommend this for others :)


=Build instructions for Windows and Linux=

We have tried to make the build process as simple as possible, but due to the size and nature of the dependancies the first build is likely to take quite some time.


==Step 1: if you have Boost 1.36 already==

If you already have Boost 1.36 available on your system you should make it available at the level above this file with the installed headers and libraries in ../Boost/install and the Boost source code available at ../Boost/src. I.e. your directory structure should look like this:

parent
 |-Boost
 |  |-install (installed Boost files)
 |  |-src (Boost source code)
 |-fost
    |-readme.txt (location of this file)

If you do not have Boost 1.36 then the first stage of the build process will fetch it and perform the required build for you.


==Step 2: Other pre-requisites==

If you're on Windows then all of the pre-requisites will be fetched by the build script. For Linux there are a number of packages that you should have avaiable.
* libssl-dev (cryptographic functions)
* uuid-dev (GUID generation)
* valgrind (testing)

On Debian based systems the following command should make sure that you have everything that you need:

apt-get install g++ libssl-dev uuid-dev valgrind


==Step 2: Building Fost==

Within this directory you will find a build script ("build" for Linux, "build.cmd" for Windows). From a command prompt in this directory execute the script. On Linux use:

    ./build

And on Windows use:

    build

Once completed you will find in the directory above this a folder structure in "dist" which will contain the build library files. The layout below dist depends on the platform you're building on.


=Using the libraries=

It is recommended that you use Bjam with Boost.Build to build projects that make use of Fost, but it is not a requirement.

In the Examples directory are various example programs that make use of different facilities within the libraries.


=Bugs, issues and help=

For the moment you will get the quickest response by emailing questions to fost@felspar.com.
=======
Fost -- Core libraries
Copyright 1995-2008 Felspar Co. Ltd.
http://fost.3.felspar.com/


These core libraries are experimental versions of the new Fost 4 system. Felspar does make use of them for production code, but doesn't recommend this for others :)


=Build instructions for Windows and Linux=

We have tried to make the build process as simple as possible, but due to the size and nature of the dependancies the first build is likely to take quite some time.


==Step 1: if you have Boost 1.36 already==

If you already have Boost 1.36 available on your system you should make it available at the level above this file with the installed headers and libraries in ../Boost/install and the Boost source code available at ../Boost/src. I.e. your directory structure should look like this:

parent
 |-Boost
 |  |-install (installed Boost files)
 |  |-src (Boost source code)
 |-fost
    |-readme.txt (location of this file)

If you do not have Boost 1.36 then the first stage of the build process will fetch it and perform the required build for you.


==Step 2: Other pre-requisites==

If you're on Windows then all of the pre-requisites will be fetched by the build script. For Linux there are a number of packages that you should have avaiable.
* libssl-dev (cryptographic functions)
* uuid-dev (GUID generation)
* valgrind (testing)

On Debian based systems the following command should make sure that you have everything that you need:

apt-get install g++ libssl-dev libbz2-dev uuid-dev valgrind


==Step 2: Building Fost==

Within this directory you will find a build script ("build" for Linux, "build.cmd" for Windows). From a command prompt in this directory execute the script. On Linux use:

    ./build

And on Windows use:

    build

Once completed you will find in the directory above this a folder structure in "dist" which will contain the build library files. The layout below dist depends on the platform you're building on.


=Using the libraries=

It is recommended that you use Bjam with Boost.Build to build projects that make use of Fost, but it is not a requirement.

In the Examples directory are various example programs that make use of different facilities within the libraries.


=Bugs, issues and help=

For the moment you will get the quickest response by emailing questions to fost@felspar.com.
>>>>>>> .merge-right.r33841

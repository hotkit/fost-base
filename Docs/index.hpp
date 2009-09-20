/** \mainpage

    <h1>Fost 4</h1>

    Fost 4 is a set of libraries and applications that build on the FOST.3 web application framework. Eventually Fost 4 will feature a full web application stack, but for the time being we are releasing a set of some of the core libraries as part of our testing for the new Linux support.

    <i>Note that these documents assume familiarity with the C++ language and it's standard libraries. A knowledge of the Boost libraries will help, but is not so important.</i>

    <h2>The philosophy of Fost</h2>

    There are a few things that Fost values:

    - Efficiency of implementation is not as important as correctness and ease of use of the libraries.
    - Fost users shouldn't pay for what they don't use.
    - When given a choice between ''simplicity of implementation'' of the library and ''simplicity of use'' of the library always ''make it easy for the library user''.
    - The libraries should always make it as easy as possible to do the right thing, and hard to do the wrong thing.
    - Fost features should be useable from as a wide a range of languages as possible -- you should not need to be a C++ programmer in order to take advantage of them or integrate to systems that use them.
    - Never be dogmatic about anything (even about never being dogmatic).
    - Don't be afraid to ''undo'' bad decisions and ''do'' undo bad designs.
    - Never embark on change without a migration strategy.

    A release of only the smallest part of the foundation of the Fost libraries means that we are not yet able to fully live up to all of these values yet, but they will take more shape as more of the older FOST.3 code is ported and more of the libraries are released.

    Some of the values appear to be contradictory as well. This is part of what makes writing libraries so much fun, and also so hard to do. Fost 4 is the fourth generation web application framework sharing these ideals. We've made a lot of mistakes, learned a lot from them, and expect to continue to do both. The core web application stack that we're porting for Fost 4 has remained largely in tact since the early days of FOST 2 -- about 10 years ago now. Some of the code still in use dates back to this period, some of it is from even earlier.


    <h2>A brief history</h2>

    As the name implies Fost 4 is the fourth generation "FOST" framework. FOST 1 was written  in 1997 and built its own object system on top of an RDBMS data store. This prooved to be OK for small applicaitons, but not so good for larger ones. FOST 2 and 2.5 followed during 1998 to 2000 using an OR/M that created databases which looked far more like normal databases. The FOST.3 project was started in late 2000 in order to provide proper Unicode support and better C++ compatibility with the web servers. The O/RM was largely unaffected, but a model compiler based on UML was added. FOST.3 also added a large library of pre-built modules and classes making it very simple to build complex web applications even in C++.

    <h2>Fost 4</h2>

    All previous versions of Fost have been Windows only using primarily Microsoft's SQL Server as the persistent data store. Although still a relevant platform for customers who wish to house systems in house, more and more of the applications that wish to use Fost are internet applicaitons and more and Linux is now a much more important commercial platform than it was in 1998 when the first generation of the C++ code that forms the heart of Fost 4 was written.

    <h2>The Boost libraries</h2>

    Every generation of Fost since version 2 was built has made heavy use of the Boost libraries. Back then they consistet of little more than a few smart pointers, but that was more than useful enough to get us started with the libraries.
*/
/** \defgroup fost_base fost-base

    This package contains all of the basic Fost 4 functionality for building DLLs/shared-objects, for command line applications and for unit testing.
*/

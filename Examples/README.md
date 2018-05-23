# Fost-base examples

To build a single example change into it's project folder. On Windows type:

    ..\..\..\bjam toolset=msvc

On Linux type:

    ../../../bjam toolset=gcc

The program and it's dependancies will be installed to that folder for execution.

## 1 - Hello

Shows a simple Hello world application using fost-cli's banner display. Also shows how INI file settings can be overrriden on the command prompt.

    $ hello
    Hello world!
    $ hello -b false
    $ hello -b true
    Hello world!
    $ hello -i nobanner.ini
    $ hello -i nobanner.ini -b true
    Hello world!

## 2 - FizzBuzz

A simple version of FizzBuzz that shows:
* The settings database
* Type coercion
* How to read from the command line

    $ fizzbuzz
    FizzBuzz
    Copyright (c) 2008 Felspar Co. Ltd.
    1 2 Fizz 4 Buzz Fizz 7 8 Fizz Buzz 11 Fizz 13 14 FizzBuzz 16 17 Fizz 19 Buzz Fizz 22 23 Fizz Buzz 26 Fizz 28 29 FizzBuzz 31 32 Fizz 34 Buzz Fizz 37 38 Fizz Buzz 41 Fizz 43 44 FizzBuzz 46 47 Fizz 49 Buzz Fizz 52 53 Fizz Buzz 56 Fizz 58 59 FizzBuzz 61 62 Fizz 64 Buzz Fizz 67 68 Fizz Buzz 71 Fizz 73 74 FizzBuzz 76 77 Fizz 79 Buzz Fizz 82 83 Fizz Buzz 86 Fizz 88 89 FizzBuzz 91 92 Fizz 94 Buzz Fizz 97 98 Fizz 100
    $ fizzbuzz -b false 20
    1 2 Fizz 4 Buzz Fizz 7 8 Fizz Buzz 11 Fizz 13 14 FizzBuzz 16 17 Fizz 19 20
    $ fizzbuzz 300
    FizzBuzz
    Copyright (c) 2008 Felspar Co. Ltd.
    1 2 Fizz 4 Buzz Fizz 7 8 Fizz Buzz 11 Fizz 13 14 FizzBuzz 16 17 Fizz 19 [etc]

## 3 - fprettyjson

A simple JSON parser and pretty printer.
* JSON parsing and unparsing.
* Loading and saving files to disk.

## 4 - hash

Generate hashes for a specified file, or files
* Argument processing
* Cryptographic hash functions
* File iteration, directory recursion
* Task progress recording
* Task monitoring display


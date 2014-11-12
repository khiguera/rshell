rshell
======
This project aims to make a working terminal called rshell.
The hw0 tag has support for the following:
=========
Comments. 

When using echo with quotes the quotes will no be printed.

Comments can be printed when using quotes. 

The cd command will result in an error reported by perror

Username and hostname will be printed out with the prompt.

The hw1 tag has an included cp.cpp and ls.cpp file
==============
The ls.cpp file can run the -l -a and -R flags only as well as no flags

Some combinations of the aforementioned flags are supported. The ones that are not supported 
are listed in the bugs section.

It has some color support, hidden files are just not on a grey background

The cp.cpp can copy files just as the regular cp. It has an optional flag that will copy the file
in 3 different ways displaying the run time of each. Otherwise the default is the fastest of the
3 methods in the file with the run time still being printed.

Known bugs:
===========
If enter is hit with no input or with simply a space, it will segfault.

There is no connector support.

There is a possiblity of memory leaks. The source code has not been analyzed thoroughly enough
to account for this.

There is no support for autofill when pressing tab.

When using quotes midway through a string such as test"quotes" there will be a space where the quote
is. Unlike in bash where the quotes are simply ignored.

If there is a comment after an ending quote it will be printed out instead of ignored.

When running script, the user login will not be printed it will result in an error reported by perror

LS BUGS
When running -R with -a in any combination, all hidden files on the computer are printed not just in
the current directory.

When combining -R and -l, the -l flag is run multiple times. Mainly for as many files there are in 
the current directory being printed out by -R

Running -R and -a is not reccommended for it will cause a near infinite loop untill all files
on the machine is printed

For the same bug as above all 3 flags should not be run on the same cmd at the same time.

The Color functionality is not applied to no flags and to the -a flag due to time constraints.

CP BUGS
The cp.cpp does not allow the optional flag to come before the arguments, only after.
This is due to time constraints not allowing to be fixed before the deadline.


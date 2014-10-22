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

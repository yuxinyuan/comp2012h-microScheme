# comp2012h-microScheme
a simple scheme interpreter based on comp2012h course project
This project mostly followed the spec of http://www.cse.ust.hk/~dekai/2012H/assignments/a4/html/
It didn't stricly follow the R5RS.
I decided to rewrite it with more c++11 features (e.g. smart pointers), and change the scoping rule from dynamic scoping to lexical scoping.

To build the interpreter, just run:
$ make 
$ ./scm
in the root directory of the project

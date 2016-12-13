Daniel Thagard
COMP 5421 Assignment 2
led

This submission contains the following files:
C++ files:
  LineEditor.h
  LineEditor.cpp
  Command.h
  Command.cpp
  Driver.cpp

Other
  container_tables.xlsx
  README.txt

The container tables portion of the assignment can be found in
container_tables.xlsx.

The program can be compiled by compiling the .cpp files to create an executable.
The program has been testing using the g++ compiler, and has succeeded at all
of the tests outlined by Dr. Ghaderpanah.

The parser in this version is slightly stricter and more accurate than that described in the
assignment description. Commands that take one line address (a, i, u, d) or zero
line addresses (q, =, w) can not be given additional line addresses.
For example: '1q' or '3,4i' will not work. This change was implemented because
I felt that simply ignoring the lines provided for these command types was a
semantic issue, and that this implementation is closer to the way that ed works.

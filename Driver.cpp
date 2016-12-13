/*
* COMP 5421 Assignment 2
* @author Daniel Thagard, 40012403
* @version 1.0
* @since June 23, 2016
*/

#include <iostream>
#include <string>
#include "Command.h"
#include "LineEditor.h"
using namespace std;

/*
* Instatiate and run the line editor.
* Use a filename if one is provided in the command line arguments.
*/
int main(int argc, char const *argv[]) {
  string filename;
  switch (argc){
    case 1:
      break;
    case 2:
      filename = argv[1];
      break;
    default:
      cout << "Too many arguments - all discarded" << endl;
      filename.clear();
      break;
  }

  LineEditor led(filename);
  led.run();

  return 0;
}

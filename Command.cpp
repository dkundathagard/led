/*
* COMP 5421 Assignment 2
* @author Daniel Thagard, 40012403
* @version 1.0
* @since June 23, 2016
*
* implementation of Command.h
*/
#include <iostream>
#include <regex>
#include <string>
#include "Command.h"
using namespace std;

/*
* Constructor:
* Initialize the input_string, then remove whitespace and parse it.
* @param input: a string input by the user representing the user's command
*/
Command::Command(const std::string& input)
{
  parse(input);
}

/*
* Overload the Assignment operator
* @param rhs is another Command object
* @return the newly assigned calling command object
*/
const Command& Command::operator=(const Command& rhs){
  parse(rhs.input_string);
  return *this;
}

/*
* Overload the stream extraction operator
* @param in is an istream object
* @param command is a command object
*/
istream &operator>>(istream& in, Command& command){
  string input;
  getline(in, input);
  command.parse(input);
  return in;
}

/*
* Parse the user input into its three portions: a starting line address,
* an ending line address (range), and a one letter command type.
*/
void Command::parse(string input){
  input_string = remove_wspace(input);
  regex re_no_param ("^([qw=])$");
  regex re_one_param ("^(\\.|\\$|\\d+)?([aiud])$");
  regex re_two_param ("^(\\.|\\$|\\d+)?(,)?(\\.|\\$|\\d+)?([rpnc])?$");
  smatch re_match;
  // Pressing the enter key moves down one line in the buffer
  if (input_string.empty()){
    address = "1";
    line_range = "";
    command_type = 'd';
    is_valid = true;
    params = 1;
  }
  // Match command types that do not take any line number parameters
  else if (regex_search(input_string, re_match, re_no_param)){
    address = "";
    line_range = "";
    command_type = re_match[1].str()[0];
    is_valid = true;
    params = 0;
  }
  // Match command types with one optional line number parameter
  else if (regex_search(input_string, re_match, re_one_param)){
    address = re_match[1].str();
    line_range = address;
    command_type = re_match[2].str()[0];
    is_valid = true;
    // Default value for up and down is to move by one line.
    if (address.empty() && (command_type == 'd' || command_type == 'u')){
      address = "1";
    }
    params = 1;
  }
  // Match command types with two optional line number parameters
  else if (regex_search(input_string, re_match, re_two_param)){
    address = !re_match[1].str().empty() ? re_match[1].str() : ".";
    line_range = re_match[3].str();
    command_type = !re_match[4].str().empty() ? re_match[4].str()[0] : 'p';
    is_valid = true;
    //Allow selecting current line with a trailing comma.
    if (line_range.empty()){
      line_range = re_match[2].str() == ","? "." : address;
    }
    params = 2;
  }
  else{     //None of the regular expressions have been matched
    is_valid = false;
  }
}

/*
* Getter method for is_valid
* @return true if the command is valid
* Note: this only checks the syntactic validity of the command.
* A line editor must determine if a command is semantically valid for its
* particular buffer.
*/
bool Command::get_validity() const{
  return is_valid;
}

/*
* Getter method for address
*/
string Command::get_address() const{
  return address;
}

/*
* Getter method for line_range
*/
string Command::get_line_range() const{
  return line_range;
}

/*
* Getter method for command_type
*/
char Command::get_command_type() const{
  return command_type;
}

/*
* Getter for params
*/
int Command::get_num_params() const{
  return params;
}

/*
* removes all spaces from a string
*/
string Command::remove_wspace(string input){
  input.erase(remove(input.begin(),input.end(),' '),input.end());
  return input;
}

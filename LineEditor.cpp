/*
* COMP 5421 Assignment 2
* @author Daniel Thagard, 40012403
* @version 1.0
* @since June 23, 2016
*
* Implementation of LineEditor.h
*/

#include <iostream>
#include <iomanip>
#include <iterator>
#include <fstream>
#include <string>
#include <list>
#include "LineEditor.h"
#include "Command.h"

using namespace std;


/*
* Constructor:
* instantiate a line editor with a filename
*/
LineEditor::LineEditor(const std::string& filename)
: filename{filename},
current{0}
{
  string line;
  if (filename.empty()){
    cout << "\"?\" [New File]" << endl;
    return;
  }

  ifstream file(filename);
  // If the file cannot be found
  if (!file){
    cout << "Unable to open file " << filename << endl;
    cout << '"' << filename << '"' << " [New File]" << endl;
    return;
  }
  // If the file can be found, add lines of file to the buffer, set current to
  // the last line
  while (getline(file, line)){
    buffer.push_back(line);
  }
  current = buffer.size();
  cout << '"' << filename << "\" " << current << " lines" << endl;
  file.close();
}
/*
* Run the LineEditor: accept, parse, and execute commands from user.
*/
void LineEditor::run(){
  cout << "Entering Command Mode" << endl;
  Command command;
  while (true){
    cout << ":";
    cin >> command;
    dispatch(command);
  }
}

/*
* Print the current address.
*/
void LineEditor::print_current_address() const{
  cout << current << endl;
}

/*
* Takes a command object and calls the appropriate function to execute the command
* @param a Command object to be executed
*/
void LineEditor::dispatch(const Command& command){
  // print error if invalid command
  if (!command.get_validity()){
    cout << "error: invalid command" << endl;
    return;
  }
  char command_type = command.get_command_type();
  // Print an error message for commands that cannot operate on an empty buffer.
  if (buffer.empty() && command_type != 'q' &&
      command_type != 'a' && command_type != 'i'){
    cout << "error: file empty - enter 'q' to quit, 'a' to append, or 'i' to insert."
        << endl;
    return;
  }
  // Addresses must be converted to integers and have bound checking performed.
  int address = address_to_int(command.get_address());
  int range = address_to_int(command.get_line_range());
  // Bound checking for commands with two params
  if (command.get_num_params() == 2 &&
      (address > range || address > buffer.size() || range > buffer.size())){
    cout << "error: invalid range " << address << " through " << range << endl;
    return;
  }
  // Bound checking for commands with one param other than movement commands
  if (command_type != 'd' && command_type != 'u' && address > buffer.size()){
    cout << "EOF Reached" << endl;
    address = buffer.size();
  }
  // Call the function specified by the command type on the provided addresses.
  switch (command_type) {
    case 'a':
      append(address);
      break;
    case 'c':
      change(address, range);
      break;
    case 'i':
      insert(address);
      break;
    case 'p':
      print_lines(address, range);
      break;
    case 'n':
      print_numbered_lines(address, range);
      break;
    case 'u':
      move_up(address);
      break;
    case 'd':
      move_down(address);
      break;
    case 'r':
      remove(address, range);
      break;
    case 'w':
      write_file();
      break;
    case '=':
      print_current_address();
      break;
    case 'q':
      quit();
      break;
    default:
      cout << "ERROR" << endl;
      break;
  }
}
/*
* Prints all lines in the buffer that fall in the specified range.
* @param address is a line address indicating the first line to be printed
* @param range is a line address indicating the last line to be printed
*/
void LineEditor::print_lines(const int& address, const int& range){
  auto it = begin(buffer);
  int count;
  advance(it, address - 1);
  for (count = address; count <= range; count++, it++){
    cout << *it << endl;
  }
  current = range; //set current to the last element in printed range
}

/*
* Prints all lines in the buffer that fall in the specified range along with
* the line number.
* @param address is a line address indicating the first line to be printed
* @param range is a line address indicating the last line to be printed
*/
void LineEditor::print_numbered_lines(const int& address, const int& range){
  auto it = begin(buffer);
  int count;
  advance(it, address - 1);
  for (count = address; count <= range; count++, it++){
    cout << count << " " << *it << endl;
  }
  current = range; //set current to the last element in printed range
}

/*
* Appends user-provided line(s) after the the specified line address.
* @param address is a line address that new lines will be appended after
*/
void LineEditor::append(const int& address){
  string line;
  auto it = begin(buffer);
  current = address;

  advance(it, address);
  getline(cin, line);
  while (cin.good() && line != "."){
    buffer.insert(it, line);
    getline(cin, line);
    current++;
    modified = true;
  }
}

/*
* Inserts user-provided line(s) before the the specified line address.
* @param address is a line address that new lines will be inserted before.
*/
void LineEditor::insert(const int& address){
  string line;
  auto it = begin(buffer);
  current = address;

  advance(it, address - 1);
  getline(cin, line);
  while (cin.good() && line != "."){
    buffer.insert(it, line);
    getline(cin, line);
    current++;
    modified = true;
  }
  // point current to the last inserted value.
  if (address != 0){
    current--;
  }
}

/*
* Move the current location up by the specified numer of lines
* @param address is the offset (number of lines to move up)
*/
void LineEditor::move_up(const int& lines){
  if (buffer.empty()){
    current = 0;
  }
  else if (current - lines < 1){
    cout << "BOF reached" << endl;
    current = 1;
  }
  else{
    current = current - lines;
  }
}


/*
* Move the current location down by the specified numer of lines
* @param address is the offset (number of lines to move down)
*/
void LineEditor::move_down(const int& lines){
  int size = buffer.size();
  if (size == 0){
    current = 0;
  }
  else if (current + lines > size){
    cout << "EOF reached" << endl;
    current = size;
  }
  else{
    current = current + lines;
  }
}

/*
* Write the buffer to the file specified in this->filename
* If the filename field is blank, prompt the user for a filename
*/
void LineEditor::write_file(){
  if (filename.empty()){
    cout << "Enter a filename:" << endl;
    cin >> filename;
  }
  ofstream outfile(filename);
  for (string line : buffer){
    outfile << line << endl;
  }
  outfile.close();
  modified = false;
}

/*
* Remove all lines in the buffer that fall in the specified range.
* @param address is a line address indicating the first line to be deleted
* @param range is a line address indicating the last line to be deleted
*/
void LineEditor::remove(const int& address, const int& range){
  auto it1 = begin(buffer);
  auto it2 = begin(buffer);
  advance(it1, address - 1);
  advance(it2, range);
  buffer.erase(it1, it2);
  //Set current element to removed address or to 0 if empty.
  current = buffer.empty()? 0 : address;
  modified = true;
}

/*
* Perform a string substitution all lines in the buffer that fall in the
* specified range.
* @param address is a line address indicating the first line to be checked
* @param range is a line address indicating the last line to be checked
*/
void LineEditor::change(const int& address, const int& range){
  string change_from, change_to;
  auto it = begin(buffer);
  size_t found, length;
  // Get user input values for string substitution
  cout << "change what? ";
  getline(cin, change_from);
  cout << "to what? ";
  getline(cin, change_to);
  length = change_from.length();
  // Iterate through the specified lines, making the substitution
  advance(it, address - 1);
  for (int count = address; count <= range; count++, it++){
    found = (*it).find(change_from);
    while (found != string::npos){
      *it = (*it).replace(found, length, change_to);
      found = (*it).find(change_from, found + change_to.length());
    }
  }
  current = range;
  modified = true;
}
/*
* Quit the Line Editor
* If the buffer has been modified since the last write / since the file was
* opened, the user is prompted to save the file.
*/
void LineEditor::quit(){
  char choice;
  if (modified){
    cout << "The file has been modified. Would you like to save your changes?"
    << endl << "y: yes, n: no, any other key: cancel" <<  endl;
    cin >> choice;
    switch (choice) {
      case 'y':
        cout << "saving changes" << endl;
        write_file();
        break;
      case 'n':
        cout << "your changes have been discarded" << endl;
        break;
      default:
        while ((choice = cin.get()) != '\n' && choice != EOF); //flush stdin
        return;
    }
  }
  exit(0);
}

/*
* Convert a general string line address to an integer value
* @param address is string representing a line number, that is either a number,
* '.' (representing the current line) or '$' representing the last line in the
* buffer.
*/
int LineEditor::address_to_int(const string& address){
  if (address.empty() || address == "."){
    return current;
  }
  if (address == "$"){
    return buffer.size();
  }
  return stoi(address);
}

/*
* COMP 5421 Assignment 2
* @author Daniel Thagard, 40012403
* @version 1.0
* @since June 23, 2016
*
* The LineEditor class defines the led application.
*/
#ifndef LINE_EDITOR_H
#define LINE_EDITOR_H

#include <string>
#include <list>
#include "Command.h"
class LineEditor{
public:
  /* CONSTRUCTORS AND DESTRUCTORS */
  LineEditor() = delete;
  explicit LineEditor(const std::string&);
  LineEditor(const LineEditor&) = delete;
  virtual ~LineEditor() = default;

  /* PUBLIC MEMBER FUNCTIONS */
  void run();

private:
  /* PRIVATE ATTRIBUTES */
  std::string filename;
  std::list<std::string> buffer;
  int current;
  bool modified = false;

  /* PRIVATE MEMBER FUNCTIONS */
  void dispatch(const Command&);
  void print_current_address() const;
  void print_lines(const int&, const int&);
  void print_numbered_lines(const int&, const int&);
  void write_file();
  void append(const int&);
  void insert(const int&);
  void move_up(const int&);
  void move_down(const int&);
  void remove(const int&, const int&);
  void change(const int&, const int&);
  void quit();

  int address_to_int(const std::string&);

};

#endif

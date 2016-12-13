/*
* COMP 5421 Assignment 2
* @author Daniel Thagard, 40012403
* @version 1.0
* @since June 23, 2016
*
* The Command class represents a user produced led command, along with its
* parsed output.
*/

#ifndef COMMAND_H
#define COMMAND_H
#include <string>
#include <regex>

class Command{
public:
  /* CONSTRUCTORS AND DESTRUCTORS */
  Command() = default;
  explicit Command(const std::string&);
  Command(const Command&) = delete;
  ~Command() = default;

  /* OPERATOR OVERLOADS */
  const Command& operator=(const Command&);
  friend std::istream &operator>>(std::istream &, Command &);

  /* PUBLIC MEMBER FUNCTIONS */
  void parse(std::string);
  bool get_validity() const;
  std::string get_address() const;
  std::string get_line_range() const;
  char get_command_type() const;
  int get_num_params() const;

private:
  std::string remove_wspace(std::string);

  std::string input_string;

  // parsed data fields
  bool is_valid;
  std::string address;
  std::string line_range;
  char command_type;
  int params;   // number of line address parameters associated with the command_type


};

#endif

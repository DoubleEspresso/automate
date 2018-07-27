#include <iostream>
#include <cstring>
#include <stdio.h>
#include <algorithm>
#include <string> 

#include "command.h"


void Command::loop() {
  std::string input = "";
  while (std::getline(std::cin, input)) {
    if (!command(input)) break;
  }
}

bool Command::command(std::string input) {
  std::istringstream instream(input);
  std::string cmd;
  bool active = true;

  while (instream >> std::skipws >> cmd) {
    
    std::transform(cmd.begin(), cmd.end(), cmd.begin(), ::tolower);
    
    if (cmd == "q" || cmd == "exit" || cmd == "stop" || cmd == "quit") {
      server->stop();
      active = false;
      break;
    }
    else if (cmd == "help" || cmd == "h") {
      show_help();
      break;
    }
    else {
      std::cout << "unknown command " << cmd << std::endl;
    }
  }
  return active;
}

void Command::show_help() {
  std::cout << "Command menu " << std::endl;
  std::cout << "\tquit  :  exits application" << std::endl;
  std::cout << "\thelp  :  print this msg" << std::endl;
}

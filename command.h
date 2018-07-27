

#ifndef COMMAND_H
#define COMMAND_H

#include <string>
#include <sstream>

#include "server.h"


class Command {
 std::unique_ptr<Server> server;
  
 public:
 Command(std::unique_ptr<Server> s) : server(std::move(s)) { }
  
 void loop();
 bool command(std::string input);
 void show_help();
};

#endif

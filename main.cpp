#include <cstring>
#include <iostream>
#include <cassert>

#include "command.h"
#include "threadpool.h"
#include "client.h"


int main(int argc, char* argv[]) {
  std::unique_ptr<Server> server;
  
  // parse args..
  for (int j = 0; j<argc; ++j) {    
    if (!strcmp(argv[j], "-port"))
      server = make_unique<Server>(atoi(argv[j+1]));
  }

  Command c(std::move(server));
  c.loop();
  
  std::cout << "..exiting" << std::endl;
  return EXIT_SUCCESS;
}


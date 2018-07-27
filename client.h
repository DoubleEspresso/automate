#ifndef CLIENT_H
#define CLIENT_H

#include <iostream>
#include <future>
#include <functional>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>

#include "util.h"
#include "threadpool.h"


class Client {
  const unsigned int port;
  
 public:
  std::unique_ptr<Threadpool> tp;
  volatile bool running;
  
  Client(const unsigned int Port);  
  ~Client() { stop(); }
  
  void stop() { if (running) { running = false; } }
  unsigned int get_port() { return port; }
};

#endif

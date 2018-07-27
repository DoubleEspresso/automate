#ifndef SERVER_H
#define SERVER_H

#include <iostream>
#include <future>
#include <functional>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>
#include <netinet/tcp.h>

#include "util.h"
#include "threadpool.h"


class Server {
  const unsigned int port;
  
 public:
  std::unique_ptr<Threadpool> tp;
  volatile bool running;
  
  Server(const unsigned int Port);
  ~Server() { stop(); }
  
  const unsigned get_port() { return port; }
  void stop() { if (running) { running = false; } }
};

#endif

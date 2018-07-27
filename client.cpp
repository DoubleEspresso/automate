#include "client.h"

void comm_task(Client& C);


Client::Client(const unsigned int Port) : port(Port), tp(make_unique<Threadpool>()), running(false)
{
  std::cout << "..attempting to connect on port " << port << std::endl;
  tp->enqueue(comm_task, *this);
}


void comm_task(Client& C) {
  if (C.running) {
    std::cout << "..socket already open" << std::endl;
    return;
  }

  // hardcode address 0.0.0.0 for now..
  struct sockaddr_in s_addr;
  const char * ipstr = "0.0.0.0";
  struct in_addr ip;
  struct hostent * hp;
  int desc = -1;
  if (!inet_aton(ipstr, &ip)) {
    std::cout << "..error failed to parse IP address " << ipstr << std::endl;
  }

  if ((hp = gethostbyaddr((const void*)&ip, sizeof(ip), AF_INET)) == NULL) {
    std::cout << "..error no associated hostname with " << ipstr << std::endl;
    return;
  }

  memset((char*)&s_addr, 0, sizeof(s_addr));
  s_addr.sin_family = AF_INET;
  memcpy((char*)hp->h_addr, (char*)&s_addr.sin_addr.s_addr, hp->h_length);
  s_addr.sin_port = htons(C.get_port());

  if (connect(desc, (struct sockaddr*)&s_addr, sizeof(s_addr)) < 0) {
    std::cout << "..error connecting to server" << std::endl;
    return;
  }
  else std::cout << "..connected!" << std::endl;
  
}

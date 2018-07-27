#include "server.h"

void listen_task(Server& S);
void comm_task(int id);


Server::Server(const unsigned int Port) : port(Port), tp(make_unique<Threadpool>(128)), running(false)
{
  std::cout << "..listening on port " << port << std::endl; 
  tp->enqueue(listen_task, *this);
}

void listen_task(Server& S) {
  if (S.running) {
    std::cout << "..socket already open" << std::endl;
    return;
  }

  struct sockaddr_in s_addr;
  int opt = 1;
  int desc = socket(AF_INET, SOCK_STREAM, 0);
  int max_queue = 3;
  int client_socket = -1;
  
  if (desc <= 0) {
    std::cout << "..failed to open socket" << std::endl;
    return;
  }

  
  { // set socket options (keep alive, no-delay, blocking)
    setsockopt(desc, SOL_TCP, TCP_NODELAY, &opt, sizeof(opt));
    setsockopt(desc, SOL_SOCKET, SO_KEEPALIVE, &opt, sizeof(opt));
    setsockopt(desc, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt));
    int flags = fcntl(desc, F_GETFL, 0);
    if (flags & O_NONBLOCK) fcntl(desc, F_SETFL, flags ^ O_NONBLOCK);
  }
  
  s_addr.sin_family = AF_INET;
  s_addr.sin_addr.s_addr = INADDR_ANY;
  s_addr.sin_port = htons(S.get_port());

  if (bind(desc, (struct sockaddr*)&s_addr, sizeof(s_addr)) < 0) {
    std::cout << "..error binding socket" << std::endl;
    return;
  }
  
  if (listen(desc, max_queue) < 0) {
    std::cout << "..error listening" << std::endl;
    return;
  }
  
  S.running = true;
  int addr_len = sizeof(s_addr);
  
  while (S.running) {
    fd_set fds;
    FD_ZERO(&fds);
    FD_SET(desc, &fds);
    struct timeval tv;
    int rec = select(desc+1, &fds, NULL, NULL, &tv);
    switch (rec) {
      case(0): { break; } // timeout
      case(-1): { break; } // error 
      default:
      {
	if ((client_socket = accept(desc, (struct sockaddr*)&s_addr, (socklen_t*)&addr_len)) < 0) {
	  std::cout << "..error attempting to accept incoming socket connection" << std::endl;
	  client_socket = -1;
	}
	else {
	  std::cout << "..client: " << client_socket << " connected" << std::endl;
	  S.tp->enqueue(comm_task, client_socket);       
	}	
      }
    }
    usleep(1000); // 1 millesecond
  }
  close(desc);
}

void comm_task(int id) {  
  while (true) {    
    std::vector<char> buffer(4096);
    int bytes = recv(id, (&buffer[0]), buffer.size(), 0);
    if (bytes > 0) {
      std::string msg = std::string(begin(buffer), end(buffer));
      std::cout << " recv: " << msg << std::endl;
      // todo : parse received msg
    }
    else if (bytes == 0) {
      std::cout << "..client-id: " << id << " disconnected" << std::endl;
      break;
    }
    else {
      std::cout << "..error receiving data from client-id: " << id << std::endl;
    }
  }
  close(id);
}

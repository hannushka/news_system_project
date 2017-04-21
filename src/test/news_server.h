#ifndef NEWS_SERVER_H
#define NEWS_SERVER_H

#include "server.h"
#include "connection.h"
#include "connectionclosedexception.h"
#include "protocol.h"
#include "mem_server_controller.h"
#include "disk_server_controller.h"

#include <memory>
#include <iostream>
#include <string>
#include <stdexcept>
#include <cstdlib>

class NewsServer {
public:
  NewsServer(bool in_memory);
  void run(Server& server);
private:
  std::unique_ptr<Controller> controller;
};

#endif

#ifndef NEWS_SERVER_H
#define NEWS_SERVER_H

#include "server.h"
#include "connection.h"
#include "connectionclosedexception.h"
#include "protocol.h"
#include "server_controller.h"

#include <memory>
#include <iostream>
#include <string>
#include <stdexcept>
#include <cstdlib>

class NewsServer {
public:
  NewsServer(ServerController controller);
  void run(Server& server);
private:
  ServerController controller;
};

#endif

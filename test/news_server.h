#ifndef NEWS_SERVER_H
#define NEWS_SERVER_H

#include "server.h"
#include "connection.h"
#include "connectionclosedexception.h"
#include "protocol.h"
#include "controller.h"

#include <memory>
#include <iostream>
#include <string>
#include <stdexcept>
#include <cstdlib>

class NewsServer {
public:
  NewsServer(Controller controller);
  void run(Server& server);
private:
  Controller controller;
};

#endif

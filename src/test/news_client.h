#ifndef NEWS_CLIENT_H
#define NEWS_CLIENT_H

#include <memory>
#include <string>
#include "connection.h"
#include "client_controller.h"

class NewsClient {
public:
  NewsClient(ClientController controller);

  void run();

private:
  ClientController controller;
};

#endif

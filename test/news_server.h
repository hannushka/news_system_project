#ifndef NEWS_SERVER_H
#define NEWS_SERVER_H

#include "server.h"
#include "connection.h"
#include "connectionclosedexception.h"
#include "protocol.h"

#include <memory>
#include <iostream>
#include <string>
#include <stdexcept>
#include <cstdlib>
#include <unordered_map>

class NewsServer {
public:
		void list_newsgroups(std::shared_ptr<Connection> conn);
		void create_newsgroups(std::shared_ptr<Connection> conn, std::string name);
		void delete_newsgroups(std::shared_ptr<Connection> conn, int id);
		void list_articles(std::shared_ptr<Connection> conn, int id);
private:
};

#endif

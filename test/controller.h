#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "server.h"
#include "connection.h"
#include "connectionclosedexception.h"
#include "protocol.h"
#include "news_group.h"

#include <memory>
#include <iostream>
#include <string>
#include <stdexcept>
#include <cstdlib>
#include <unordered_map>

class Controller {
public:
		Controller();
		void list_newsgroups(std::shared_ptr<Connection> conn);
		void create_newsgroup(std::shared_ptr<Connection> conn, std::string name);
		void delete_newsgroup(std::shared_ptr<Connection> conn, int id);
		void list_articles(std::shared_ptr<Connection> conn, int id);
		void read_article(std::shared_ptr<Connection> conn, int article_id, int news_group_id);

private:
	std::unordered_map<int,NewsGroup> news_groups;
};

#endif

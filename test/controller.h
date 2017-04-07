#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "news_group.h"
#include <string>
#include <memory>
#include "protocol.h"
#include "connection.h"
#include <unordered_map>

class Controller {
public:
		Controller();
		void list_newsgroups();
		void create_newsgroup(std::string name);
		void delete_newsgroup(int id);
		void list_articles(int id);
		void read_article(int article_id, int news_group_id);
		void set_conn(std::shared_ptr<Connection> conn);
private:
	std::unordered_map<int,NewsGroup> news_groups;
	std::shared_ptr<Connection> conn;
	void send_string(std::string);
};

#endif

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
		void delete_newsgroup(unsigned int id);
		void list_articles(unsigned int id);
		void read_article(unsigned int article_id, unsigned int news_group_id);
		void set_conn(std::shared_ptr<Connection> conn);

		unsigned read_number();
		void write_number(unsigned value);
private:
	std::unordered_map<unsigned int, NewsGroup> news_groups;
	std::shared_ptr<Connection> conn;
	void send_string(std::string);
};

#endif

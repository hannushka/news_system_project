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
		void create_article(unsigned int id, std::string title,
			std::string author, std::string text);
		void read_article(unsigned int article_id, unsigned int news_group_id);
		void set_conn(std::shared_ptr<Connection> conn);

		unsigned read_number();
		void write_number(unsigned value);
private:
	std::unordered_map<unsigned int, NewsGroup> news_groups;
	std::shared_ptr<Connection> conn;
	void write_string(std::string);
	int current_art_id;
};

#endif

#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <string>
#include <memory>
#include "protocol.h"
#include "connection.h"
#include <map>

class Controller {
public:
	Controller();
	virtual ~Controller() = default;

	virtual void list_newsgroups() = 0;
	virtual void create_newsgroup(std::string name) = 0;
	virtual void delete_newsgroup(unsigned int id) = 0;
	virtual void list_articles(unsigned int id) = 0;
	virtual void create_article(unsigned int id, std::string title,
		std::string author, std::string text) = 0;
	virtual void delete_article(unsigned int news_group_id,
		unsigned int article_id) = 0;
	virtual void read_article(unsigned int news_group_id,
		unsigned int article_id) = 0;

	virtual void set_conn(std::shared_ptr<Connection> conn);
	virtual unsigned int read_number();
	virtual void write_number(unsigned int value);
	virtual void write_string(std::string msg);
protected:
	std::shared_ptr<Connection> conn;
};

#endif

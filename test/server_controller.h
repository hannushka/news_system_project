#ifndef SERVER_CONTROLLER_H
#define SERVER_CONTROLLER_H

#include "news_group.h"
#include "controller.h"
#include <set>

class ServerController : public Controller {
public:
	ServerController();
	void list_newsgroups() override;
	void create_newsgroup(std::string name) override;
	void delete_newsgroup(unsigned int id) override;
	void list_articles(unsigned int id) override;
	void create_article(unsigned int id, std::string title,
		std::string author, std::string text) override;
	void delete_article(unsigned int news_group_id,
    unsigned int article_id) override;
	void read_article(unsigned int news_group_id,
    unsigned int article_id) override;

private:
	std::map<unsigned int, NewsGroup> news_groups;
	std::set<std::string> ng_names;
	unsigned int current_ng_id;
	unsigned int current_art_id;
};

#endif

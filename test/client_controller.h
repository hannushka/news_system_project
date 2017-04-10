#ifndef CLIENT_CONTROLLER_H
#define CLIENT_CONTROLLER_H

#include "controller.h"

class ClientController : public Controller {
public:
	ClientController();
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
};

#endif

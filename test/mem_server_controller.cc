#include "mem_server_controller.h"
#include <iostream>

using namespace std;

MemServerController::MemServerController() : Controller(),
	current_ng_id(0), current_art_id(0) {
}

void MemServerController::list_newsgroups() {
	conn->write(Protocol::ANS_LIST_NG);
	conn->write(Protocol::PAR_NUM);
	write_number(news_groups.size());
	for (auto it = news_groups.begin() ; it != news_groups.end() ; ++it) {
		string name = it->second.get_name();
		conn->write(Protocol::PAR_NUM);
		write_number(it->first);
		conn->write(Protocol::PAR_STRING);
		write_number(name.size());
		write_string(name);
	}
	conn->write(Protocol::ANS_END);
}

void MemServerController::create_newsgroup(std::string name) {
	conn->write(Protocol::ANS_CREATE_NG);
	NewsGroup n(current_ng_id, name);
	bool success = false;
	auto names_succ = ng_names.emplace(name);
	if (names_succ.second) {
		auto ngs_succ = news_groups.emplace(current_ng_id, n);
		if (ngs_succ.second) {
			++current_ng_id;
			conn->write(Protocol::ANS_ACK);
			success = true;
		}
	}

	if (!success) {
		conn->write(Protocol::ANS_NAK);
		conn->write(Protocol::ERR_NG_ALREADY_EXISTS);
	}

	conn->write(Protocol::ANS_END);
}

void MemServerController::delete_newsgroup(unsigned int id) {
	conn->write(Protocol::ANS_DELETE_NG);
	auto it = news_groups.find(id);
	if (it != news_groups.end()) {
		news_groups.erase(it);
		conn->write(Protocol::ANS_ACK);
	} else {
		conn->write(Protocol::ANS_NAK);
		conn->write(Protocol::ERR_NG_DOES_NOT_EXIST);
	}
	conn->write(Protocol::ANS_END);
}

void MemServerController::list_articles(unsigned int id) {
	conn->write(Protocol::ANS_LIST_ART);
	auto it_ng = news_groups.find(id);
	if (it_ng != news_groups.end()) {
		conn->write(Protocol::ANS_ACK);
		conn->write(Protocol::PAR_NUM);
		auto articles = it_ng->second.get_articles();
		write_number(articles.size());
		for (auto it_art = articles.begin(); it_art != articles.end(); ++it_art) {
			string title = it_art->second.get_title();
			conn->write(Protocol::PAR_NUM);
			write_number(it_art->first);
			conn->write(Protocol::PAR_STRING);
			write_number(title.size());
			write_string(title);
		}
	} else {
		conn->write(Protocol::ANS_NAK);
		conn->write(Protocol::ERR_NG_DOES_NOT_EXIST);
	}
	conn->write(Protocol::ANS_END);
}

void MemServerController::create_article(unsigned int id, string title,
	string author, string text) {
	conn->write(Protocol::ANS_CREATE_ART);
	auto it_ng = news_groups.find(id);
	if (it_ng != news_groups.end()) {
		conn->write(Protocol::ANS_ACK);
		Article article(current_art_id, title, author, text);
		it_ng->second.add_article(current_art_id, article);
		++current_art_id;
	} else {
		conn->write(Protocol::ANS_NAK);
		conn->write(Protocol::ERR_NG_DOES_NOT_EXIST);
	}
	conn->write(Protocol::ANS_END);
}

void MemServerController::delete_article(unsigned int news_group_id,
	unsigned int article_id) {
	conn->write(Protocol::ANS_DELETE_ART);
	auto it_ng = news_groups.find(news_group_id);
	if (it_ng != news_groups.end()) {
		bool succ = it_ng->second.remove_article(article_id);
		if (succ) {
			conn->write(Protocol::ANS_ACK);
		} else {
			conn->write(Protocol::ANS_NAK);
			conn->write(Protocol::ERR_ART_DOES_NOT_EXIST);
		}
	} else {
		conn->write(Protocol::ANS_NAK);
		conn->write(Protocol::ERR_NG_DOES_NOT_EXIST);
	}
	conn->write(Protocol::ANS_END);
}

void MemServerController::read_article(unsigned int news_group_id,
	unsigned int article_id) {
	conn->write(Protocol::ANS_GET_ART);
	auto it_ng = news_groups.find(news_group_id);
	if (it_ng != news_groups.end()) {
		auto articles = it_ng->second.get_articles();
		auto it_art = articles.find(article_id);
		if (it_art != articles.end()) {
			conn->write(Protocol::ANS_ACK);
			string title = it_art->second.get_title();
			string author = it_art->second.get_author();
			string text = it_art->second.get_text();

			conn->write(Protocol::PAR_STRING);
			write_number(title.size());
			write_string(title);

			conn->write(Protocol::PAR_STRING);
			write_number(author.size());
			write_string(author);

			conn->write(Protocol::PAR_STRING);
			write_number(text.size());
			write_string(text);

		} else {
			conn->write(Protocol::ANS_NAK);
			conn->write(Protocol::ERR_ART_DOES_NOT_EXIST);
		}
	} else {
		conn->write(Protocol::ANS_NAK);
		conn->write(Protocol::ERR_NG_DOES_NOT_EXIST);
	}
	conn->write(Protocol::ANS_END);
}

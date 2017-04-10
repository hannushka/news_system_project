#include "controller.h"
#include <iostream>
//#include "article.h"

using namespace std;

Controller::Controller() : current_art_id(0) {
}

void Controller::set_conn(std::shared_ptr<Connection> conn) {
	this->conn = conn;
}

void Controller::list_newsgroups() {
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

void Controller::create_newsgroup(std::string name) {
	conn->write(Protocol::ANS_CREATE_NG);
  hash<string> str_hash;
	unsigned int id = str_hash(name) % 4294967295; //max uint
	id = news_groups.size();
	NewsGroup n(id, name);

	auto succ = news_groups.emplace(id, n);
	if (succ.second) {
		conn->write(Protocol::ANS_ACK);
	} else {
		conn->write(Protocol::ANS_NAK);
		conn->write(Protocol::ERR_NG_ALREADY_EXISTS);
	}
	conn->write(Protocol::ANS_END);
}

void Controller::delete_newsgroup(unsigned int id) {
	conn->write(Protocol::ANS_DELETE_NG);
	auto it = news_groups.find(id);
	if (it != news_groups.end()) {
		news_groups.erase(it);
		conn->write(Protocol::ANS_ACK);
	} else {
		conn->write(Protocol::ANS_NAK);
		conn->write(Protocol::ERR_NG_ALREADY_EXISTS);
	}
	conn->write(Protocol::ANS_END);
}

void Controller::list_articles(unsigned int id) {
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

void Controller::create_article(unsigned int id, string title,
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

void Controller::delete_article(unsigned int news_group_id,
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

void Controller::write_string(string msg) {
	for (char c : msg)
	{
		conn->write(c);
	}
}

unsigned int Controller::read_number() {
 	unsigned char byte1 = conn->read();
 	unsigned char byte2 = conn->read();
 	unsigned char byte3 = conn->read();
 	unsigned char byte4 = conn->read();
 	return (byte1 << 24) | (byte2 << 16) | (byte3 << 8) | byte4;
}

void Controller::write_number(unsigned int value) {
 	conn->write((value >> 24) & 0xFF);
 	conn->write((value >> 16) & 0xFF);
 	conn->write((value >> 8)	 & 0xFF);
 	conn->write(value & 0xFF);
}

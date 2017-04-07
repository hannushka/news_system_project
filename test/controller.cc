#include "controller.h"

using namespace std;

Controller::Controller(/*NewsServer& ns*/)/* : news_server(ns)*/ {
	NewsGroup n(1,"Sverige");
	news_groups.emplace(1, n);
	NewsGroup n2(2,"Utrikes");
	news_groups.emplace(1, n2);
}

void Controller::set_conn(std::shared_ptr<Connection> conn) {
	this->conn = conn;
}

void Controller::list_newsgroups() {
	conn->write(Protocol::ANS_LIST_NG);
	for (auto it = news_groups.begin() ; it != news_groups.end() ; ++it) {
		send_string((*it).second.get_name());
		auto tmp = it;
		if (++tmp != news_groups.end())
			conn->write('\n');
	}
	conn->write(Protocol::ANS_END);
}

void Controller::send_string(string msg) {
	for (char c : msg)
		conn->write(c);
}

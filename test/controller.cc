#include "controller.h"
#include <iostream>

using namespace std;

Controller::Controller(/*NewsServer& ns*/)/* : news_server(ns)*/ {
/*	NewsGroup n(1,"Sverige");
	news_groups.emplace(1, n);
	NewsGroup n2(2,"Utrikes");
	news_groups.emplace(1, n2);*/
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
		cout << it->first << endl;
		write_number(it->first);
		conn->write(Protocol::PAR_STRING);
		write_number(name.size());
		send_string(name);
	}
	conn->write(Protocol::ANS_END);
}

void Controller::create_newsgroup(std::string name) {
	conn->write(Protocol::ANS_CREATE_NG);
  hash<string> str_hash;
	unsigned int id = str_hash(name) % 4294967295; //max uint
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

void Controller::send_string(string msg) {
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

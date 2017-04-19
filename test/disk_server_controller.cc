#include "disk_server_controller.h"
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

using namespace std;

DiskServerController::DiskServerController() : Controller(),
	current_ng_id(0), current_art_id(0) {
}

void DiskServerController::list_newsgroups() {
	conn->write(Protocol::ANS_LIST_NG);
	ifstream file("newsgroups/info");
	if (file.good()) {
    string str;
		getline(file, str);
		int nbr_of_folders = stoi(str);
		conn->write(Protocol::PAR_NUM);
		write_number(nbr_of_folders);
    for (int i = 0 ; i != nbr_of_folders ; i++) {
			getline(file, str);
			istringstream iss;
			iss.str(str);
			string title, tmp;
			iss >> title;
			iss >> tmp;
			int id = stoi(tmp);
			conn->write(Protocol::PAR_NUM);
			write_number(id);
			conn->write(Protocol::PAR_STRING);
			write_number(title.size());
			write_string(title);
    }
	} else {
		write_number(0);
	}
	conn->write(Protocol::ANS_END);
}

void DiskServerController::create_newsgroup(std::string name) {
}

void DiskServerController::delete_newsgroup(unsigned int id) {
}

void DiskServerController::list_articles(unsigned int id) {
}

void DiskServerController::create_article(unsigned int id, string title,
	string author, string text) {
}

void DiskServerController::delete_article(unsigned int news_group_id,
	unsigned int article_id) {}

void DiskServerController::read_article(unsigned int news_group_id,
	unsigned int article_id) {}

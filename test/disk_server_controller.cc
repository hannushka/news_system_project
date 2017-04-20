#include "disk_server_controller.h"
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>

using namespace std;

DiskServerController::DiskServerController() : Controller() {
}

void DiskServerController::list_newsgroups() {
	conn->write(Protocol::ANS_LIST_NG);
	conn->write(Protocol::PAR_NUM);
	ifstream file("newsgroups/info");
	if (file.good()) {
    string line;
		getline(file, line);
		unsigned int nbr_of_folders = stoi(line);
		write_number(nbr_of_folders);
		getline(file, line); //flush current_id
	  while (getline(file, line)) {
			string title = line;
			getline(file, line);
			unsigned int id = stoi(line);
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

void DiskServerController::create_newsgroup(string name) {
	conn->write(Protocol::ANS_CREATE_NG);

	ifstream has_newsgroups("newsgroups");
	if (!has_newsgroups) {
		mkdir("newsgroups", 0700);
	}

	string path = "newsgroups/" + name;
	ifstream has_newsgroup(path);
	//DIR* dir = opendir(path.c_str());

	if (!has_newsgroup) {
		mkdir(path.c_str(), 0700);

		ifstream in_file("newsgroups/info");
		if (!in_file) {
			ofstream out_file("newsgroups/info");
			out_file << "0" << endl << "0" << endl;
			out_file.close();
			in_file.open("newsgroups/info");
		}

		string line;
		getline(in_file, line);
		unsigned int nbr_of_folders = stoi(line);
		++nbr_of_folders;
		getline(in_file, line);
		unsigned int current_id = stoi(line);
		++current_id;

		ostringstream oss;
		oss << to_string(nbr_of_folders) << endl <<
			to_string(current_id) << endl;

		while (getline(in_file, line)) {
			oss << line << endl;
		}
		oss << name << endl << current_id << endl;
		conn->write(Protocol::ANS_ACK);

		ofstream out_file("newsgroups/info");
		out_file << oss.str();

	} else {
		conn->write(Protocol::ANS_NAK);
		conn->write(Protocol::ERR_NG_ALREADY_EXISTS);
	}
	conn->write(Protocol::ANS_END);
}

void DiskServerController::delete_newsgroup(unsigned int id) {
	conn->write(Protocol::ANS_DELETE_NG);

	bool success = false;
	ifstream in_file("newsgroups/info");
	if (in_file) {
		string line;
		getline(in_file, line);
		unsigned int nbr_of_folders = stoi(line);
		getline(in_file, line);
		unsigned int current_id = stoi(line);

		ostringstream oss;
		oss << current_id << endl;

		while (getline(in_file, line)) {
			string name = line;
			getline(in_file, line);
			unsigned int ng_id = stoi(line);
			if (ng_id != id) {
				oss << name << endl << ng_id << endl;
			} else {
				--nbr_of_folders;
				name = "\"" + name + "\""; //system does not allow spaces...
				string path = "rm -r newsgroups/" + name;
				cout << path << endl;
				int error = system(path.c_str());
				if (error) {
					cerr << "could not delete folder " + name << endl;
					exit(1);
				}
				success = true;
			}
		}

		ofstream out_file("newsgroups/info");
		out_file << nbr_of_folders << endl << oss.str();
	}

	if (success) {
		conn->write(Protocol::ANS_ACK);
	} else {
		conn->write(Protocol::ANS_NAK);
		conn->write(Protocol::ERR_NG_DOES_NOT_EXIST);
	}
	conn->write(Protocol::ANS_END);
}

void DiskServerController::list_articles(unsigned int id) {
	conn->write(Protocol::ANS_LIST_ART);
	bool success = false;
	ifstream newsgroup_info("newsgroups/info");
	if (newsgroup_info) {
		string line;
		getline(newsgroup_info, line); //flush size
		getline(newsgroup_info, line); //flush current_id
		while (getline(newsgroup_info, line)) {
			string name = line;
			getline(newsgroup_info, line);
			unsigned int ng_id = stoi(line);
			if (ng_id == id) {
				success = true;
				conn->write(Protocol::ANS_ACK);
				conn->write(Protocol::PAR_NUM);
				ifstream article_info("newsgroups/" + name + "/info");
				if (article_info) {
					getline(article_info, line);
					unsigned int nbr_of_articles = stoi(line);
					getline(article_info, line);
					//unsigned int current_id = stoi(line);
					write_number(nbr_of_articles);
					while (getline(article_info, line)) {
						string title = line;
						getline(article_info, line);
						unsigned int art_id = stoi(line);
						conn->write(Protocol::PAR_NUM);
						write_number(art_id);
						conn->write(Protocol::PAR_STRING);
						write_number(title.size());
						write_string(title);
					}
				} else {
					write_number(0);
				}
			}
		}
	}

	if (!success) {
		conn->write(Protocol::ANS_NAK);
		conn->write(Protocol::ERR_NG_DOES_NOT_EXIST);
	}
	conn->write(Protocol::ANS_END);
}

void DiskServerController::create_article(unsigned int id, string title,
	string author, string text) {
		conn->write(Protocol::ANS_CREATE_ART);
		bool success = false;
		ifstream newsgroup_info("newsgroups/info");
		if (newsgroup_info) {
			string line;
			getline(newsgroup_info, line); //flush size
			getline(newsgroup_info, line); //flush current_id

			while (getline(newsgroup_info, line)) {
				string name = line;
				getline(newsgroup_info, line);
				unsigned int ng_id = stoi(line);
				if (ng_id == id) {
					conn->write(Protocol::ANS_ACK);
					success = true;
					ifstream in_file("newsgroups/" + name + "/info");
					if (!in_file) {
						ofstream out_file("newsgroups/" + name + "/info");
						out_file << "0" << endl << "0" << endl;
						out_file.close();
						in_file.open("newsgroups/" + name + "/info");
					}
					getline(in_file, line);
					unsigned int nbr_of_articles = stoi(line);
					++nbr_of_articles;
					getline(in_file, line);
					unsigned int current_id = stoi(line);
					++current_id;

					ostringstream oss;
					while (getline(in_file, line)) {
						string temp_title = line;
						getline(in_file, line);
						unsigned int art_id = stoi(line);
						oss << temp_title << endl << art_id << endl;
					}
					oss << title << endl << current_id << endl;
					ofstream out_file("newsgroups/" + name + "/info");
					out_file << nbr_of_articles << endl <<
						current_id << endl << oss.str();

					ofstream article("newsgroups/" + name + "/" + title +
						"_" + to_string(current_id));
					article << title << endl << author << endl <<
						text;
				}
			}
		}
		if (!success) {
			conn->write(Protocol::ANS_NAK);
			conn->write(Protocol::ERR_NG_DOES_NOT_EXIST);
		}
		conn->write(Protocol::ANS_END);
}

void DiskServerController::delete_article(unsigned int news_group_id,
	unsigned int article_id) {
		conn->write(Protocol::ANS_DELETE_ART);
		bool art_found = false;
		bool ng_found = false;
		ifstream newsgroup_info("newsgroups/info");
		if (newsgroup_info) {
			string line;
			getline(newsgroup_info, line); //flush size
			getline(newsgroup_info, line); //flush current_id

			while (getline(newsgroup_info, line)) {
				string name = line;
				getline(newsgroup_info, line);
				unsigned int currrent_ng_id = stoi(line);
				if (currrent_ng_id == news_group_id) {
					ng_found = true;
					ifstream in_file("newsgroups/" + name + "/info");
					if (in_file) {
						getline(in_file, line);
						unsigned int nbr_of_articles = stoi(line);
						getline(in_file, line);
						unsigned int current_id = stoi(line);

						ostringstream oss;
						oss << current_id << endl;
						while (getline(in_file, line)) {
							string temp_title = line;
							getline(in_file, line);
							unsigned int current_art_id = stoi(line);
							if (current_art_id != article_id) {
								oss << temp_title << endl << current_art_id << endl;
							} else {
								conn->write(Protocol::ANS_ACK);
								--nbr_of_articles;
								art_found = true;
							}
						}
						ofstream out_file("newsgroups/" + name + "/info");
						out_file << nbr_of_articles << endl << oss.str();
					}
				}
			}
		}

		if (!ng_found) {
			conn->write(Protocol::ANS_NAK);
			conn->write(Protocol::ERR_NG_DOES_NOT_EXIST);
		} else if (!art_found) {
			conn->write(Protocol::ANS_NAK);
			conn->write(Protocol::ERR_ART_DOES_NOT_EXIST);
		}
		conn->write(Protocol::ANS_END);
}

void DiskServerController::read_article(unsigned int news_group_id,
	unsigned int article_id) {

	bool art_found = false;
	bool ng_found = false;
	conn->write(Protocol::ANS_GET_ART);
	ifstream newsgroup_info("newsgroups/info");
	if (newsgroup_info) {
		string line;
		getline(newsgroup_info, line); //flush size
		getline(newsgroup_info, line); //flush current_id

		while (getline(newsgroup_info, line)) {
			string name = line;
			getline(newsgroup_info, line);
			unsigned int currrent_ng_id = stoi(line);
			if (currrent_ng_id == news_group_id) {
				ng_found = true;
				ifstream in_file("newsgroups/" + name + "/info");
				if (in_file) {
					getline(in_file, line); //flush size
					getline(in_file, line); //flush current_id

					while (getline(in_file, line)) {
						string title = line;
						getline(in_file, line);
						unsigned int current_art_id = stoi(line);
						if (current_art_id == article_id) {
							art_found = true;
							conn->write(Protocol::ANS_ACK);
							ifstream article("newsgroups/" + name + "/" +
								title + "_" + to_string(article_id));

							string title, author, text;
							getline(article, line);
							title = line;
							getline(article, line);
							author = line;

							int count = 0;
							ostringstream oss;
							while (getline(article, line)) {
								count++;
								oss << line << endl;
							}

							text = oss.str();
							if (count == 1) {
								//remove last endl when text contains only one line
								text = text.substr(0, text.size() - 1);
							}

							conn->write(Protocol::PAR_STRING);
							write_number(title.size());
							write_string(title);

							conn->write(Protocol::PAR_STRING);
							write_number(author.size());
							write_string(author);

							conn->write(Protocol::PAR_STRING);
							write_number(text.size());
							write_string(text);
						}
					}
				}
			}
		}
	}
	if (!ng_found) {
		conn->write(Protocol::ANS_NAK);
		conn->write(Protocol::ERR_NG_DOES_NOT_EXIST);
	} else if (!art_found) {
		conn->write(Protocol::ANS_NAK);
		conn->write(Protocol::ERR_ART_DOES_NOT_EXIST);
	}
	conn->write(Protocol::ANS_END);
}

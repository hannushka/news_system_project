#include "connection.h"
#include "connectionclosedexception.h"

#include <iostream>
#include <string>
#include <stdexcept>
#include <cstdlib>
#include <iterator>
#include "protocol.h"
#include "news_client.h"
#include <sstream>

using namespace std;

NewsClient::NewsClient(ClientController controller) :
  controller(controller) {

}

void NewsClient::run() {
	while (true) {
		try {
			cout << "Type a command: ";
			string line;
			getline(cin, line);

			auto space = line.find(' ');
			string first_word = line.substr(0, space);
			if (first_word == "ls") {
				controller.list_newsgroups();
			} else if (first_word == "cn") {
				if (space == string::npos) {
					cout << "Must enter a name. Exiting..." << endl;
					exit(1);
				}
				string name = line.substr(space + 1, string::npos);
				controller.create_newsgroup(name);
			} else if (first_word == "dn") {
				if (space == string::npos) {
					cout << "Must enter an id. Exiting..." << endl;
					exit(1);
				}
				string id_str = line.substr(space + 1, string::npos);
				unsigned int id = stoul(id_str);
				controller.delete_newsgroup(id);
			}	else if (first_word == "la") {
				if (space == string::npos) {
					cout << "Must enter an id. Exiting..." << endl;
					exit(1);
				}
				string id_str = line.substr(space + 1, string::npos);
				unsigned int id = stoul(id_str);
				controller.list_articles(id);
			} else if (first_word == "ca") {
				if (space == string::npos) {
					cout << "Must enter more than ca. Exiting..." << endl;
					exit(1);
				}

				string input = line.substr(space + 1, string::npos);
				istringstream iss;
				iss.str(input);

				string id_str;
				iss >> id_str;
				unsigned int id = stoul(id_str);

				string title;
				iss >> title;
				if (title == "") {
					cout << "Must input title. Exiting..." << endl;
					exit(1);
				}

				string author;
				iss >> author;
				if (author == "") {
					cout << "Must input author. Exiting..." << endl;
					exit(1);
				}

				string word;
				string text;
				while (iss >> word) {
					text += word +" ";
				}

				controller.create_article(id, title, author, text);
			} else if (first_word == "da" || first_word == "ra") {
				if (space == string::npos) {
					cout << "Must enter more than da. Exiting..." << endl;
					exit(1);
				}

				string input = line.substr(space + 1, string::npos);
				istringstream iss;
				iss.str(input);

				string ng_id_str;
				iss >> ng_id_str;
				if (ng_id_str == "") {
					cout << "Must input newsgroup ID. Exiting..." << endl;
					exit(1);
				}
				unsigned int ng_id = stoul(ng_id_str);

				string art_id_str;
				iss >> art_id_str;
				if (art_id_str == "") {
					cout << "Must input article ID. Exiting..." << endl;
					exit(1);
				}
				unsigned int art_id = stoul(art_id_str);

				if (first_word == "da")
					controller.delete_article(ng_id, art_id);
				if (first_word == "ra")
					controller.read_article(ng_id, art_id);
			} else {
				cout << "Wrong response" << endl;
			}
		} catch (ConnectionClosedException&) {
			cout << " no reply from server. Exiting..." << endl;
			exit(1);
		}
	}
}

int main(int argc, char* argv[]) {
	if (argc != 3) {
		cerr << "Usage: myclient host-name port-number" << endl;
		exit(1);
	}

	int port = -1;
	try {
		port = stoi(argv[2]);
	} catch (exception& e) {
		cerr << "Wrong port number. " << e.what() << endl;
		exit(1);
	}

	shared_ptr<Connection> conn(make_shared<Connection>(argv[1], port));
	if (!conn->isConnected()) {
		cerr << "Connection attempt failed" << endl;
		exit(1);
	}

	ClientController controller;
	controller.set_conn(conn);
	NewsClient news_client(controller);
	news_client.run();
}

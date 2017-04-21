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
  cout << "Client is running. Type help for all commands." << endl << endl;
	while (true) {
		try {
			cout << "Type a command: ";
			string line;
			getline(cin, line);
      istringstream iss(line);

      string command;
      iss >> command;
			if (command == "ls") {
        if (iss >> command) {
          cout << command + " takes no arguments" << endl;
        } else {
	         controller.list_newsgroups();
        }
			} else if (command == "cn") {
        string name;
        iss >> name;
        if (iss >> name) {
          cout << command + " takes only one argument" << endl;
        } else {
          controller.create_newsgroup(name);
        }
			} else if (command == "dn") {
        string id_str;
        if (iss >> id_str) {
          try {
            if (iss >> id_str) {
              cout << command + " takes only one argument" << endl;
            } else {
              unsigned int id = stoul(id_str);
              controller.delete_newsgroup(id);
            }
          } catch (std::invalid_argument) {
              cout << "id must be an integer" << endl;
          }
        } else {
          cout << command + " needs at least one argument" <<endl;
        }

			}	else if (command == "la") {
        string id_str;
        if (iss >> id_str) {
          try {
            if (iss >> id_str) {
              cout << command + " takes only one argument" << endl;
            } else {
              unsigned int id = stoul(id_str);
              controller.list_articles(id);
            }
          } catch (std::invalid_argument) {
              cout << "id must be an integer" << endl;
          }
        } else {
            cout << command + " needs at least one argument" <<endl;
        }

			} else if (command == "ca") {
				string id_str;
				if (iss >> id_str) {
          try {
            unsigned int id = stoul(id_str);
            string title;
            iss >> title;
            if (title == "") {
              cout << "must input title" << endl;
            } else {
              string author;
              iss >> author;
              if (author == "") {
                cout << "must input author" << endl;
              } else {
                  string word;
                  string text;
                  while (iss >> word) {
                    text += word + " ";
                  }
                  controller.create_article(id, title, author, text);
              }
            }
          } catch (std::invalid_argument) {
              cout << "id must be an integer" << endl;
          }
        } else {
          cout << command + " needs four arguments" << endl;
        }

			} else if (command == "da" || command == "ra") {
				string ng_id_str;
        string art_id_str;
				if (iss >> ng_id_str) {
          try {
  				  unsigned int ng_id = stoul(ng_id_str);
            if (iss >> art_id_str) {
              try {
      				  unsigned int art_id = stoul(art_id_str);
                if (command == "da")
                  controller.delete_article(ng_id, art_id);
                if (command == "ra")
                  controller.read_article(ng_id, art_id);
              } catch (std::invalid_argument) {
                  cout << "article id must be an integer" << endl;
              }
            } else {
                cout << command + " needs two arguments" << endl;
            }
          } catch (std::invalid_argument) {
              cout << "newsgroup id must be an integer" << endl;
          }
        } else {
            cout << command + " needs two arguments" << endl;
        }
			} else if (command == "help") {
        string dummy;
        if (iss >> dummy) {
          cout << command + " takes no arguments" << endl;
        } else {
          cout << endl;
          cout << "cn <name>\t\t\t\tcreates a newsgroup with the typed " <<
            "name" << endl << endl;
          cout << "ls\t\t\t\t\tlists all newsgroups" << endl << endl;
          cout << "ca <ng_id> <title> <author> <text>\t" <<
            "creates an article in newsgroup with" << endl <<
            "\t\t\t\t\tid: ng_id, title, author and text" << endl << endl;
          cout << "da <ng_id> <art_id>\t\t\tdeletes article with " <<
            "id: art_id" << endl << "\t\t\t\t\tfrom newsgroup with " <<
            "id: ng_id" << endl << endl;
          cout << "dn <ng_id>\t\t\t\tdeletes newsgroup with id: " <<
            "ng_id" << endl << endl;
          cout << "la <ng_id>\t\t\t\tlists all articles in newsgroup with" <<
            endl << "\t\t\t\t\tid: ng_id" << endl << endl;
          cout << "ra <ng_id> <art_id>\t\t\treads the title, author " <<
            "and text from" << endl << "\t\t\t\t\tarticle with id: art_id "<<
            "and from" << endl << "\t\t\t\t\tnewsgroup with id: ng_id" <<
            endl << endl;
        }
      } else {
				cout << "command does not exist, type help for all commands" << endl;
			}
		} catch (ConnectionClosedException&) {
			cout << "no reply from server. Exiting..." << endl;
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

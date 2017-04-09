#include "connection.h"
#include "connectionclosedexception.h"

#include <iostream>
#include <string>
#include <stdexcept>
#include <cstdlib>
#include <iterator>
#include "protocol.h"
#include "news_client.h"

using namespace std;

NewsClient::NewsClient() {

}

void NewsClient::list_newsgroups() {
	conn->write(Protocol::COM_LIST_NG);
	conn->write(Protocol::COM_END);

//debugging
	/*char c = conn->read();
	while (c != Protocol::ANS_END) {
		int s = c;
		cout << s << endl;
		c = conn->read();
	}*/

	char c = conn->read();
	if (c == Protocol::ANS_LIST_NG) {
		string msg;
		c = conn->read();
		if (c == Protocol::PAR_NUM) {
			int nbr_ng = read_number();
			for (int i = 0; i != nbr_ng; ++i) {
				if (conn->read() == Protocol::PAR_NUM) {
					if (conn->read() == Protocol::PAR_STRING) {
						int str_len = read_number();
						for (int j = 0; j != str_len; ++j)
							msg += conn->read();
						msg += '\n';
					}
				}
			}
		}
		cout << msg;
		if (conn->read() != Protocol::ANS_END)
			cerr << "did not reach end" << endl;
	}
}

void NewsClient::create_newsgroup(std::string name) {
	conn->write(Protocol::COM_CREATE_NG);
	conn->write(Protocol::PAR_STRING);
	write_number(name.size());
	for (char c : name)
		conn->write(c);
	conn->write(Protocol::COM_END);

	char c = conn->read();
	if (c == Protocol::ANS_CREATE_NG) {
		c = conn->read();
		if (c == Protocol::ANS_ACK) {
			cout << name << " was added successfully" << endl;
		} else if (c == Protocol::ANS_NAK) {
			c = conn->read();
			if (c == Protocol::ERR_NG_ALREADY_EXISTS) {
				cout << name << " already exists" << endl;
			}
		}
		if (conn->read() != Protocol::ANS_END)
			cerr << "did not reach end" << endl;
	}
}

void NewsClient::set_conn(shared_ptr<Connection> conn) {
	this->conn = conn;
}

int NewsClient::read_number() {
 	unsigned char byte1 = conn->read();
 	unsigned char byte2 = conn->read();
 	unsigned char byte3 = conn->read();
 	unsigned char byte4 = conn->read();
 	return (byte1 << 24) | (byte2 << 16) | (byte3 << 8) | byte4;
}

void NewsClient::write_number(int value) {
 	conn->write((value >> 24) & 0xFF);
 	conn->write((value >> 16) & 0xFF);
 	conn->write((value >> 8)	 & 0xFF);
 	conn->write(value & 0xFF);
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

	NewsClient news_client;
	news_client.set_conn(conn);

	while (true) {
		try {
			cout << "Type a command: ";
			string line;
			getline(cin, line);
			auto space = line.find(' ');
			string first_word = line.substr(0, space);
			if (first_word == "ls") {
				news_client.list_newsgroups();
			} else if (first_word == "create") {
				if (space == string::npos) {
					cout << "Must enter a name. Exiting" << endl;
					exit(1);
				}
				string name = line.substr(space + 1, string::npos);
				news_client.create_newsgroup(name);
	/*		} else if (first_word == "delete") {
				news_client.delete_newsgroup();
			}	else if (first_word == "create") {
				news_client.read_articles();
			}	else if (first_word == "la") {
				news_client.list_newsgroups();*/
			} else {
				cout << "Wrong response" << endl;
			}
		} catch (ConnectionClosedException&) {
			cout << " no reply from server. Exiting." << endl;
			exit(1);
		}
	}
}

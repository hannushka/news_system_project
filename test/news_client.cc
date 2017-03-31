/* myclient.cc: sample client program */
#include "connection.h"
#include "connectionclosedexception.h"

#include <iostream>
#include <string>
#include <stdexcept>
#include <cstdlib>
#include <iterator>
#include "protocol.h"

using namespace std;

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

	Connection conn(argv[1], port);
	if (!conn.isConnected()) {
		cerr << "Connection attempt failed" << endl;
		exit(1);
	}

	while (true) {
		try {
			cout << "Type a command: ";
			string line;
			getline(cin, line);
		//	auto space = line.find(' ');
		//	string first_word = line.substr(0, space);
			if (line == "ls") {
				conn.write(Protocol::COM_LIST_NG);
				conn.write(Protocol::COM_END);
				char c = conn.read();
				if (c == Protocol::ANS_LIST_NG) {
					c = conn.read();
					string msg;
					while (c != Protocol::ANS_END) {
						msg += c;
						c = conn.read();
					}
					cout << msg << endl;
				}
			} else {
				cout << "Wrong response" << endl;
			}
		} catch (ConnectionClosedException&) {
			cout << " no reply from server. Exiting." << endl;
			exit(1);
		}
	}
}

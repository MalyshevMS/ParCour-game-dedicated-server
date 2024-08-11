#pragma comment(lib, "ws2_32.lib")
#include <winsock2.h>
#include <iostream>
#include <vector>
#pragma warning(disable: 4996)

using namespace std;

SOCKET Connections[2];
int Counter = 0;

void ClientHandler(int index) {
	int msg_size;
	while (true) {
		recv(Connections[index], (char*)&msg_size, sizeof(int), NULL);
		char* msg = new char[msg_size + 1];
		msg[msg_size] = '\0';
		recv(Connections[index], msg, msg_size, NULL);
		for (int i = 0; i < Counter; i++) {
			if (i == index) {
				continue;
			}

			send(Connections[i], (char*)&msg_size, sizeof(int), NULL);
			send(Connections[i], msg, msg_size, NULL);
		}
		delete[] msg;
	}
}

int main(int argc, char* argv[]) {
	WSAData wsaData;
	WORD DLLVersion = MAKEWORD(2, 1);
	if (WSAStartup(DLLVersion, &wsaData) != 0) {
		cout << "Error: can't initialize WSA" << endl;
		return -1;
	}

	string ip = "127.0.0.1";
	u_short port = 1111;
	
	if (argc >= 2) {
		ip = argv[1];
		port = atoi(argv[2]);
	}

	SOCKADDR_IN addr;
	int sizeofaddr = sizeof(addr);
	addr.sin_addr.s_addr = inet_addr(ip.c_str());
	addr.sin_port = htons(port);
	addr.sin_family = AF_INET;

	SOCKET sListen = socket(AF_INET, SOCK_STREAM, NULL);
	bind(sListen, (SOCKADDR*)&addr, sizeof(addr));
	listen(sListen, SOMAXCONN);

	cout << "Server started at " << ip << ":" << port << endl << "Waiting for clients..." << endl;

	SOCKET newConnection;
	for (int i = 0; i < 2; i++) {
		newConnection = accept(sListen, (SOCKADDR*)&addr, &sizeofaddr);

		if (newConnection == 0) {
			cout << "Error: can't connect client" << endl;
		}
		else {
			cout << "Player " << i+1 << " Connected!" << endl;

			Connections[i] = newConnection;
			Counter++;
			CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)ClientHandler, (LPVOID)(i), NULL, NULL);
		}
	}

	cout << "All players connected. Starting game..." << endl;

	string com;
	while (true) {
		cout << ">> ";
		cin >> com;

		if (com == "stop" || com == "exit") {
			return 0;
		} else if (com == "ip") {
			cout << "Server ip is " << ip << endl;
		} else if (com == "port") {
			cout << "Server port is " << port << endl;
		} else {
			cout << "Incorrect or unexisting command!" << endl;
		}
	}
	return 0;
}
//AUTHOR : SRIPAD
//Client2 code

#include <iostream>
#include <winSock2.h>
#include <ws2tcpip.h>
#include <thread>
#include <string>

using namespace std;
#pragma comment (lib,"ws2_32.lib")


bool initialize() {
	WSAData data;
	return WSAStartup(MAKEWORD(2, 2), &data) == 0;
}

void sendmsg(SOCKET clientsocket) {
	cout << "Enter the chat name : " << endl;
	string name;
	getline(cin, name);
	string message;
	while (1) {
		getline(cin, message);
		string msg = name + ":" + message;

		int bytesent = send(clientsocket, msg.c_str(), msg.length(), 0);
		if (bytesent == SOCKET_ERROR) {
			cout << " message not sent" << endl;
			break;
		}

		if (message == "quit") {
			cout << "stop application" << endl;
			break;
		}
	}
	closesocket(clientsocket);
	WSACleanup();
}

void recvMsg(SOCKET clientsocket) {
	char buffer[4096];

	while (1) {
		int byterecv = recv(clientsocket, buffer, sizeof(buffer), 0);
		if (byterecv <= 0) {
			cout << "msg not received" << endl;
			break;
		}
		string message(buffer, byterecv);
		cout  << message << endl;
	}
	closesocket(clientsocket);
	WSACleanup();
}

int main() {

	//initialize socket
	//create socket
	//connect to socket
	//send/recv
	//close socket

	if (!initialize()) {
		cout << "Initialization failed" << endl;
		//return 1;
	}

	SOCKET clientsocket = socket(AF_INET, SOCK_STREAM, 0);
	if (clientsocket == INVALID_SOCKET) {
		cout << "Client socket creation failed" << endl;
		return 1;
	}

	int port = 12345;
	string serveraddress = "127.0.0.1";
	sockaddr_in clientaddr;
	clientaddr.sin_family = AF_INET;
	clientaddr.sin_port = htons(port);

	inet_pton(AF_INET, serveraddress.c_str(), &(clientaddr.sin_addr));

	if (connect(clientsocket, reinterpret_cast<sockaddr*>(&clientaddr), sizeof(clientaddr)) == SOCKET_ERROR) {
		cout << "not able to connect to server" << endl;
		cout << "." << WSAGetLastError();
		closesocket(clientsocket);
		WSACleanup();
		return 1;

	}

	cout << "successfully connected to server " << endl;

	thread senderthread(sendmsg, clientsocket);
	thread receiverthread(recvMsg, clientsocket);

	senderthread.join();
	receiverthread.join();

	WSACleanup();
	return 0;
}

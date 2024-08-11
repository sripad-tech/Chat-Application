//AUTHOR : SRIPAD

#include <iostream>
#include <WinSock2.h>
#include <ws2tcpip.h>
#include <tchar.h>
#include <thread >
#include <vector>

using namespace std;
#pragma comment (lib,"ws2_32.lib")

bool Initialize() {
	WSADATA data;
	return WSAStartup(MAKEWORD(2, 2), &data) == 0;
}

void InteractwithClient(SOCKET clientsocket, vector<SOCKET>& clients) {
	cout << "client connected" << endl;
	char buffer[4096];

	while (1) {
		int bytesrecvd = recv(clientsocket, buffer, sizeof(buffer), 0);

		if (bytesrecvd<=0) {
			cout << "disconnected client" << endl;
			break;
		}

		string message(buffer, bytesrecvd);
		std::cout << "message from client : " << message << endl;

		for (auto client : clients) {
			if (client != clientsocket) {
				send(client,message.c_str(),message.length(),0);

			}

		}
	}
	auto it = find(clients.begin(), clients.end(), clientsocket);
	if (it != clients.end()) {
		clients.erase(it);
	}

	closesocket(clientsocket);

}

int main(){
	if (!Initialize()) {
		std::cout << "Initialization failed" << endl;
	}
	//initialize winsock library
	// create socket
    //get ip and port
	//bind io port with socket
	//listen on the socket
	//accept
	//recv/send
	//clean socket
	std::cout << "Welcome to Sripad Chat Application" << endl;
	SOCKET listensocket = socket(AF_INET, SOCK_STREAM, 0);
	if (listensocket == INVALID_SOCKET) {
		std::cout << "socket creation failed" << endl;
		return 1;
	}

	int port = 12345;
	sockaddr_in serveraddr;
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_port = htons(port);
	
	//convert ip put it inside sin_family in binary format
	if (InetPton(AF_INET, _T("0.0.0.0"), &serveraddr.sin_addr) != 1){
		std::cout << "setting address structure failed" << endl;
		closesocket(listensocket);
		WSACleanup();
		return 1;
	}

	//bind
	if (bind(listensocket, reinterpret_cast<sockaddr*>(&serveraddr), sizeof(serveraddr)) == SOCKET_ERROR) {
		std::cout << "bindfailed" << endl;
		closesocket(listensocket);
		WSACleanup();
		return 1;
	}

	//listen
	if (listen(listensocket, SOMAXCONN) == SOCKET_ERROR) {
		std::cout << "listen error" << endl;
		closesocket(listensocket);
		WSACleanup();
		return 1;
	}

	std::cout << "server has started listening on port  : "<< port << endl;
	vector<SOCKET> clients;
	while (1) {
		//accept
		SOCKET clientsocket = accept(listensocket, nullptr, nullptr);
		if (clientsocket == INVALID_SOCKET) {
			std::cout << "INvalid client socket" << endl;
			//closesocket(listensocket);
			//WSACleanup();
			return 1;
		}

		clients.push_back(clientsocket);

		thread t1(InteractwithClient, clientsocket, std::ref(clients));
		t1.detach();
	}

	closesocket(listensocket);

	WSACleanup();
	return 0;
}

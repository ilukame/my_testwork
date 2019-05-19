#pragma once
//#pragma comment(lib, "wsock32.lib")
#pragma comment(lib, "ws2_32.lib")


#include <winsock2.h>
#include <iostream>
#include <string>
#include <windows.h>
#include <process.h>
#include <ws2tcpip.h>
#include <atlstr.h>

using namespace std;

class Udp
{
private:
	WSADATA wsaData;
	SOCKET sock;
	struct sockaddr_in addr;

	
public:
	Udp(string ip, int port);
	~Udp();
	void Send(string message) {
		sendto(sock, message.c_str(), message.length(), 0, (struct sockaddr *)&addr, sizeof(addr));
	}
	void Bind() {
		if (bind(sock, (sockaddr*)&addr, sizeof(addr)) == SOCKET_ERROR) {
			printf("%d\n", WSAGetLastError());
			printf("ERROR: bind() failed. (%s, %d)\n", __FILE__, __LINE__);
		}
	}
	string Receive() {
		char buf[2048];
		memset(buf, 0, sizeof(buf));
		recv(sock, buf, sizeof(buf), 0);
		return string(buf);
	}
};


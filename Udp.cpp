#include "Udp.h"

Udp::Udp(string ip, int port){
	WSAStartup(MAKEWORD(2, 0), &wsaData);
	sock = socket(AF_INET, SOCK_DGRAM, 0);
	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);
	//addr.sin_addr.S_un.S_addr = inet_addr(ip);
	CString str = ip.c_str();
	InetPton(addr.sin_family, str, &addr.sin_addr.S_un.S_addr);
}


Udp::~Udp(){
	//int reuse = 1;
	//setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, (const char*)&reuse, sizeof(reuse));
	closesocket(sock);
	WSACleanup();
}

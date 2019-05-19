#include "Udp.h"
#define IP "192.168.10.1"
#define PORT 8889

void ReceiveThread(void *pParam) {
	Udp *udp = (Udp*) pParam;
	cout << udp->Receive() << endl;
}


int main() {
	HANDLE thread;
	Udp *udp = new Udp(IP, PORT);
	udp->Bind();
	thread = (HANDLE)_beginthread(ReceiveThread, 0, udp);

	delete udp;
	return 0;
}
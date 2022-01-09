#pragma comment(lib, "ws2_32.lib")
#include <stdio.h>
#include <string.h>
#include <winsock2.h>
#include <windows.h>
#include <iostream>
#pragma warning(disable:4996) 

using namespace std;

int main()
{
	char buf[100], b[100];
	WORD wVersionRequested;
	WSADATA wsaData;
	wVersionRequested = MAKEWORD(2, 2);
	if (FAILED(WSAStartup(wVersionRequested, &wsaData)))
	{
		cout << "Error: " << WSAGetLastError() << '\n';
	}

	struct sockaddr_in add;
	add.sin_family = AF_INET;
	add.sin_port = htons(1024);
	add.sin_addr.s_addr = inet_addr("127.0.0.1");

	SOCKET s = socket(AF_INET, SOCK_DGRAM, 0);

	int t, l = 1;
	t = sizeof(add);
	cout << "Enter the string:" << endl;
	cin.getline(buf, 225, '\n');

	sendto(s, buf, strlen(buf), 0, (struct sockaddr*)&add, t);
	int rv=recvfrom(s, b, strlen(b), 0, (struct sockaddr*)&add, &t);

	b[rv] = '\0';

	cout <<"Number of brackets in a line: "<< b << '\n';

	closesocket(s);
	WSACleanup();
	system("pause");
	return 0;
}


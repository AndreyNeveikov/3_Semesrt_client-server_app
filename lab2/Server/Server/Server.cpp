//Клиент вводит с клавиатуры строку символов и посылает ее серверу.
//Признак окончания ввода строки – нажатие  клавиши «Ввод».
//Сервер, получив эту строку, должен определить длину введенной строки,
//и, если эта длина кратна 5, то подсчитывается количество скобок всех видов.
//Их количество посылается клиенту.

#define _WINSOCK_DEPRECATED_NO_WARNINGS
#pragma comment (lib, "ws2_32.lib")
#include <winsock2.h>
#include <iostream>
#include <stdlib.h>

using namespace std;

int main()
{
    WORD wVersionRequested;
    WSADATA wsaData;
    wVersionRequested = MAKEWORD(2, 2);

    if (FAILED(WSAStartup(wVersionRequested, &wsaData)))
    {
        cout << "Error: " << WSAGetLastError() << '\n';
    }
    SOCKET s = socket(AF_INET, SOCK_DGRAM, 0);

    struct sockaddr_in ad;
    ad.sin_port = htons(1024);
    ad.sin_family = AF_INET;
    ad.sin_addr.s_addr = 0;

    bind(s, (struct sockaddr*)&ad, sizeof(ad));

	char b[200], brackets[] = "[{()}]",res[10];
	int l;
	l = sizeof(ad);
	while (true)
	{
		int rv=recvfrom(s, b, strlen(b), 0, (struct sockaddr*)&ad, &l);
		b[rv] = '\0';

		int count = 0;
		if (strlen(b) % 5 == 0)
		{
			for (int i = 0; i < strlen(b); i++)
			{
				if (strchr(brackets,b[i]))
				{ 
					count++;
				}
			}
		}
		else
		{
			cout << "String is not multiple of 5";
		}
		_itoa_s(count, res, 10);
		sendto(s, res, strlen(b), 0, (struct sockaddr*)&ad, l);

	}

	closesocket(s);
	WSACleanup();
	return 0;
}


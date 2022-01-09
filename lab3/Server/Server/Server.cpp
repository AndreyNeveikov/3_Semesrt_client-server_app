//На сервере хранится список сотрудников фирмы.
//Каждая запись списка содержит следующую информацию о сотрудниках :
// - Ф.И.О.сотрудника;
// - табельный номер;
// - количество отработанных часов за месяц;
// - почасовой тариф.
//Таких записей должно быть не менее пяти.
//(пятый параметр зарплата)

//Клиент посылает на сервер величину заработной платы, 
//по которой он хочет получить информацию о сотрудниках.
//Назад он получает список только тех сотрудников, 
//заработная плата которых меньше указанной.

#pragma comment(lib, "ws2_32.lib")
#include <winsock2.h>
#include <iostream>
#include <string>
#include <cstdlib>
#include <stdlib.h>
#pragma warning(disable: 4996)
using namespace std;

struct help
{
	int check;
};

struct Worker
{
	char name[40];
	int personnel_number;  //табельный номер
	int work_time;	//время работы
	double hourly_rate;	//почасовой тариф
	int salary; //зарплата
};


int numcl = 0;
int number = 0;
const int SIZE_ARR_OF_WORKERS = 5;
DWORD  WINAPI
ThreadFunc(LPVOID client_socket);
void print();
Worker workers[SIZE_ARR_OF_WORKERS];
struct help h;
void main()
{
	strcpy(workers[0].name, "Vasiliev Vasilii Vasilievich \t");
	workers[0].personnel_number = 987654;
	workers[0].work_time = 155;
	workers[0].hourly_rate = 12.5;
	workers[0].salary = 2000;
	strcpy(workers[1].name, "Maximov Maxim Maximovich \t");
	workers[1].personnel_number = 321098;
	workers[1].work_time = 130;
	workers[1].hourly_rate = 15.0;
	workers[1].salary = 1500;
	strcpy(workers[2].name, "Artemov Artem Artemovich \t");
	workers[2].personnel_number = 765432;
	workers[2].work_time = 160;
	workers[2].hourly_rate = 11.0;
	workers[2].salary = 2700;
	strcpy(workers[3].name, "Dmitryev Dmitry Dmitryvich \t");
	workers[3].personnel_number = 109876;
	workers[3].work_time = 100;
	workers[3].hourly_rate = 9.5;
	workers[3].salary = 1000;
	strcpy(workers[4].name, "Andreev Andrey Andreevich \t");
	workers[4].personnel_number = 543210;
	workers[4].work_time = 125;
	workers[4].hourly_rate = 14.5;
	workers[4].salary = 1200;

	cout << "Workers list:\t" << "\t\t\tPersonnel number \t" << "Work time (hours)\t" << "Hourly rate(byn/hour)\t" << "Salary\t\n";
	cout << endl;
	int j = 0;
	while (j < SIZE_ARR_OF_WORKERS)
	{
		cout << workers[j].name << "\t" << workers[j].personnel_number << "\t\t\t" << workers[j].work_time << "\t\t\t" << workers[j].hourly_rate << "\t\t\t" << workers[j].salary << endl;
		j++;
	}
	cout << "---------------------------------------------------------------------------------------------------\n";
	WORD wVersionRequested;
	WSADATA wsaData;
	int err;
	wVersionRequested = MAKEWORD(2, 2);
	err = WSAStartup(wVersionRequested, &wsaData);
	if (err != 0) { return; }

	SOCKET s = socket(AF_INET, SOCK_STREAM, 0);
	if (s == INVALID_SOCKET) { return; }
	sockaddr_in local_addr;
	local_addr.sin_family = AF_INET;
	local_addr.sin_port = htons(1280);
	local_addr.sin_addr.s_addr = 0;
	bind(s, (sockaddr*)&local_addr, sizeof(local_addr));
	int c = listen(s, 5);
	
	// извлекаем сообщение из очереди
	SOCKET client_socket;    // сокет для клиента
	sockaddr_in client_addr; // адрес клиента (заполняется системой)
	int client_addr_size = sizeof(client_addr);
	// цикл извлечения запросов на подключение из очереди

	while ((client_socket = accept(s, (sockaddr*)&client_addr, &client_addr_size)))
	{
		numcl++;
		number++;
		print();
		char* ip = inet_ntoa(client_addr.sin_addr);
		cout << "ip - |" << ip << "|" << "  ";
		cout << "descriptor - |" << client_socket << "| " << "port - |" << ntohs(local_addr.sin_port) << "|" << endl;
		DWORD thID;  //thID идентификатор типа DWORD
		CreateThread(NULL, NULL, ThreadFunc, &client_socket, NULL, &thID);
	}
}

DWORD WINAPI  ThreadFunc(LPVOID client_socket)
{
	int i;
	int q;

	SOCKET s2 = ((SOCKET*)client_socket)[0];

	char buf[200];
	while (true)
	{
		ZeroMemory(buf, sizeof(buf));

		if (recv(s2, (char*)&h, sizeof(h), 0) == SOCKET_ERROR)
		{
			numcl--;
			cout << "Client disconnected. " << "Number of active clients " << "| " << numcl << endl;
			break;
		}
		q = 0;
		cout << "Number of active clients " << "|" << numcl << "|" << " descriptor: " << "|" << s2 << "|" << endl << "The request:  " << h.check << endl;
		cout << "Sent to the client: " << endl;
		for (i = 0; i < SIZE_ARR_OF_WORKERS; i++)
		{
			if (workers[i].salary <= h.check)
			{
				strcat(buf, workers[i].name);
			}
			else
			{
				q++;
			}

			if (q == SIZE_ARR_OF_WORKERS)
			{
				strcat(buf, "No more workers");
			}
		}
		cout << buf << endl;
		send(s2, buf, 200, 0);

	}
	closesocket(s2);
	return 0;
}

void print()
{
	if (number)
		printf("%d client connected  ", number);
	else
		printf("No clients connected\n");
}

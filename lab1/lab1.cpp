#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#include <winsock2.h>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#pragma comment(lib, "ws2_32.lib")

using namespace std;

int main()
{

	WORD wVersionRequested;
	WSADATA wsaData;
	wVersionRequested = MAKEWORD(2, 2);
	WSAStartup(wVersionRequested, &wsaData);
	/*
	1-й параметр функции WSAStartup() — значение типа word, 
	определяет максимальный номер версии WinSock
	2-й параметр – структура wsaData – содержит номер используемой версии (поле wVersion),
	максимальный номер версии, поддер-живаемый данной библиотекой (поле wHighVersion),
	текстовые строки с описа-нием реализации WinSock, максимальное число сокетов, 
	доступных процессу и максимально допустимый размер дейтаграмм
	*/

	SOCKET  s = socket(AF_INET, SOCK_STREAM, 0); //Вызов функции socket()
	/*
		Первый параметр: этим сокетом будут использоваться ад-реса Интернет;
		Следующие два аргумента задают тип создаваемого сокета и протокол обмена данными через него.
		В приве¬денном примере создается сокет потока, использую¬щий протокол TCP
	*/

	struct sockaddr_in local; //физический IP-адрес компьютера в формате структуры in_addr
	local.sin_family = AF_INET;
	local.sin_port = htons(1280);
	local.sin_addr.s_addr = htonl(INADDR_ANY);
	int c = bind(s, (struct sockaddr*)&local, sizeof(local)); 
	int r = listen(s, 5); //Помещает сокет  в состояние прослушивания
	/*
	 Функция bind (s, addr, addrlen): связывание конкретного адреса с сокетом.
	 В нее передается дескриптор сокета, указатель на структуру ад-реса и  длина этой структуры.
	*/

	while (true) {
		char buf[255], res[100], inputing_string[255], * Res;
		//структура определяет удаленный адрес,
		//с которым  соединяется сокет
		sockaddr_in remote_addr;
		int size = sizeof(remote_addr);
		SOCKET	s2 = accept(s, (struct sockaddr*)&remote_addr, &size); //Открытие соединения


		int element_index;
		while (recv(s2, inputing_string, sizeof(inputing_string), 0) != 0) { //Прием данных через сокет потока

			int Number_of_digits = 0, string_len = strlen(inputing_string);
			for (element_index = 0; element_index <= string_len; element_index++)
			{
				while (element_index < string_len && inputing_string[element_index] == ' ')
				{
					element_index++;
				}
				if (element_index < string_len)
				{
					Number_of_digits++;
				}
				while (element_index < string_len && inputing_string[element_index] != ' ')
				{
					element_index++;
				}
			}

		int* array_of_numbers = new int[Number_of_digits];
			int number, B_element_index = 0, E_element_index = 0, index_of_numbers = 0;
			char temporary_string[10];

			for (element_index = 0; element_index < string_len; element_index++)
			{
				while (element_index < string_len && inputing_string[element_index] == ' ')
				{
					element_index++;
				}
				B_element_index = element_index; //Индекс элемента с которого начинается число
				while (element_index < string_len && inputing_string[element_index] != ' ')
				{
					element_index++;
				}
				E_element_index = element_index; //Индекс элемента которым число заканчивается
				strncpy_s(temporary_string, inputing_string + B_element_index, E_element_index - B_element_index);
				number = atoi(temporary_string);
				array_of_numbers[index_of_numbers] = number;
				index_of_numbers++;
			}

			number = 0; //Используется для подсчета чисел кратных трем

			for (element_index = 0; element_index < Number_of_digits - 1; element_index++)
			{
				if (array_of_numbers[element_index] % 3 == 0)
				{
					number++;
				}
		
			}

			delete[]array_of_numbers;

			_itoa_s(number, res, 10);
	
			Res = new char[strlen(res) + 1];
			strcpy(Res, res);
			Res[strlen(res)] = '\0';


			//Посылает данные на соединенный сокет
			send(s2, Res, sizeof(Res) - 2, 0);
		}

	closesocket(s2); //Закрытие сокета
	}

	WSACleanup(); //Прекращение работы с WinSock DLL

}

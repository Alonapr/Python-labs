// test client.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//
#include <iostream>
#include <fstream>
#include <string>
#include <WS2tcpip.h>
#include <ctime>
#include <chrono>

using namespace std;
using namespace chrono;

void main(){
	string ipAddress = "127.0.0.1";	//IP Address of the server
	int port = 1033;	//54000			// Listening port on the server

	// Initialize winsock
	WSADATA data;
	WORD ver = MAKEWORD(2, 2);
	int wsResult = WSAStartup(ver, &data);
	if (wsResult != 0) {
		cerr << "Can't start winsock, Err # " << wsResult << endl;
		return;
	}

	// Create a socket
	SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock == INVALID_SOCKET) {
		cerr << "Can't create a socket, Err # " << WSAGetLastError() << endl;
		return;
	}

	// Fill in a hint structure
	sockaddr_in hint;
	hint.sin_family = AF_INET;
	hint.sin_port = htons(port);
	inet_pton(AF_INET, ipAddress.c_str(), &hint.sin_addr);
	
	// Connect to server
	int connResult = connect(sock, (sockaddr*)&hint, sizeof(hint));
	if (connResult == SOCKET_ERROR) {
		cerr << "Can't connect to server, Err # " << WSAGetLastError() << endl;
		closesocket(sock);
		WSACleanup();
		return;
	}

	// Do-while loop to send and receive a data
	char buf[4096];
	string userInput;
	do {
		// Prompt the user for some text
		cout << "> ";
		getline(cin, userInput);

		if (userInput.size() > 0) { //Make sure the user has typed in something
			// Send the text
			int sendResult = send(sock, userInput.c_str(), userInput.size() + 1, 0);
			if (sendResult != SOCKET_ERROR) {
				// Wait for response
				ZeroMemory(buf, 4096);
				int bytesReceived = recv(sock, buf, 4096, 0);
				if (bytesReceived > 0) {
					//calculate time for journal
					auto start = system_clock::now();
					char tmBuff[255];
					string(buf, 0, bytesReceived);
					auto end = system_clock::now();
					duration<double> elapsed_seconds = end - start;
					auto end_time = system_clock::to_time_t(end);
					ctime_s(tmBuff, sizeof(tmBuff), &end_time);
								
					// write inforomation to journal
					ofstream fout;
					fout.open("journal.txt"); //("journal.txt", ofstream::app)
					if (!fout.is_open()) {
						cout << "Error in opening txt file" << endl;
					}
					else {
						fout << "Command		" << "Data" << endl;
						fout << string(buf, 0, bytesReceived) << "		" << tmBuff << endl;
					}
					fout.close();
				}
			}
		}
	} while (userInput.size() > 0);

	// Gracefully close down everything
	closesocket(sock);
	WSACleanup();
}
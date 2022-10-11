// test server.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//
#include <iostream>
#include <WS2tcpip.h>
#include <string>

using namespace std;

void main() {
	// Initialize winsock
	WSADATA wsData;
	WORD ver = MAKEWORD(2, 2);

	int wsok = WSAStartup(ver, &wsData);
	if (wsok != 0) {
		cerr <<  "Can't initialize winsock!" << endl;
		return;
	}

	// Create a socket
	SOCKET listening = socket(AF_INET, SOCK_STREAM, 0);
	if (listening == INVALID_SOCKET) {
		cerr <<  "Can't create a socket!" << endl;
		return;
	}

	// Bind the ip address and port to a socket
	sockaddr_in hint;
	hint.sin_family = AF_INET;
	hint.sin_port = htons(1033);//54000
	hint.sin_addr.S_un.S_addr = INADDR_ANY; // could also use inet_pton ...

	bind(listening, (sockaddr*)&hint, sizeof(hint));

	// Tell winsock the socket is for listening
	listen(listening, SOMAXCONN);

	// Wait for a connection
	sockaddr_in client;
	int clientSize = sizeof(client);

	SOCKET ClientSocket = accept(listening, (sockaddr*)&client, &clientSize);

	char host[NI_MAXHOST];		// Client's remote name
	char service[NI_MAXHOST];	// Service (i.e port) the client is connect on

	ZeroMemory(host, NI_MAXHOST);
	ZeroMemory(service, NI_MAXHOST);

	if (getnameinfo((sockaddr*)&client, sizeof(client), host, NI_MAXHOST, service, NI_MAXSERV, 0) == 0) {
		cout << host << " connected on port " << service << endl;
	}
	else {
		inet_ntop(AF_INET, &client.sin_addr, host, NI_MAXHOST);
		cout << host << " connected on port " << ntohs(client.sin_port) << endl;
	}

	// Close listening socket
	closesocket(listening);

	// While loop: accept and echo message back to client
	char buf[4096];
	while (true) {
		ZeroMemory(buf, 4096);
		//Wait for client to send data
		int bytesReceived = recv(ClientSocket, buf, 4096, 0);
		if (bytesReceived == SOCKET_ERROR) {
			cerr << "Error in recv()." << endl;
			break;
		}
		if (bytesReceived == 0) {
			cout << "Client disconnected " << endl;
			break;
		}
		
		// words and links
		string word1 = "assignment";
		string word2 = "variable";
		string word3 = "bug";
		string word4 = "callback";
		string word5 = "crash";
		string word6 = "declaration";
		string word7 = "implementation";
		string word8 = "instance";
		string word9 = "invoke";
		string word10 = "paranthesis";
		string link1 = "https://en.wikipedia.org/wiki/Assignment_(computer_science)";
		string link2 = "https://launchschool.com/books/ruby/read/variables";
		string link3 = "https://searchsoftwarequality.techtarget.com/definition/bug";
		string link4 = "https://en.wikipedia.org/wiki/Callback_(computer_programming)#:~:text=In%20computer%20programming%2C%20a%20callback,argument%20at%20a%20given%20time.";
		string link5 = "https://en.wikipedia.org/wiki/Crash_(computing)#:~:text=In%20computing%2C%20a%20crash%2C%20or,stops%20functioning%20properly%20and%20exits.";
		string link6 = "https://en.wikipedia.org/wiki/Declaration_(computer_programming)#:~:text=In%20computer%20programming%2C%20a%20declaration,(identifier)%20%22means%22.";
		string link7 = "https://en.wikipedia.org/wiki/Implementation";
		string link8 = "https://www.codemotion.com/magazine/Glossary/instance-computer-science/#:~:text=In%20object%2Doriented%20programming%20(OOP,runtime%20of%20a%20computer%20program.&text=An%20object%20is%20an%20instance,then%20also%20known%20as%20construction.";
		string link9 = "https://www.webopedia.com/definitions/invoke/";
		string link10 = "https://www.computerhope.com/jargon/p/parenthe.htm#:~:text=A%20parenthesis%20is%20a%20punctuation,used%20to%20begin%20parenthetical%20text.&text=Parentheses%20in%20computer%20programming.,Parentheses%20in%20spreadsheet%20formulas.";
		
		//Return information in response on client
		if (string(buf, 0, bytesReceived) == "Who") {
			cout << "\n" << "Executor is Alona Predkova." << endl;
			cout << "Groupe: K-23.2(1)." << endl;
			cout << "Variant 8: Translator." << endl;
			cout << "Condition: The client sends the word to the server, which is entered by the user. The server returns either a dictionary entry or a denial." << endl;
		}
		else if (string(buf, 0, bytesReceived) == "Commands") {
			cout << "Information about next words is available: assignment, variable, bug, callback, crash, declaration, implementation, instance, invoke, paranthesis." << endl;
		}
		else if (string(buf, 0, bytesReceived) == word1) {
			cout << word1 << ": " << link1 << endl;
		}
		else if (string(buf, 0, bytesReceived) == word2) {
			cout << word2 << ": " << link2 << endl;
		}
		else if (string(buf, 0, bytesReceived) == word3) {
			cout << word3 << ": " << link3 << endl;
		}
		else if (string(buf, 0, bytesReceived) == word4) {
			cout << word4 << ": " << link4 << endl;
		}
		else if (string(buf, 0, bytesReceived) == word5) {
			cout << word5 << ": " << link5 << endl;
		}
		else if (string(buf, 0, bytesReceived) == word6) {
			cout << word6 << ": " << link6 << endl;
		}
		else if (string(buf, 0, bytesReceived) == word7) {
			cout << word7 << ": " << link7 << endl;
		}
		else if (string(buf, 0, bytesReceived) == word8) {
			cout << word8 << ": " << link8 << endl;
		}
		else if (string(buf, 0, bytesReceived) == word9) {
			cout << word9 << ": " << link9 << endl;
		}
		else if (string(buf, 0, bytesReceived) == word10) {
			cout << word10 << ": " << link10 << endl;
		}
		else
			cout << "Wrong input!" << endl;

		// Echo message back to client
		send(ClientSocket, buf, bytesReceived + 1, 0);
	}
	// Close the socket
	closesocket(ClientSocket);

	// Cleanup winsock;
	WSACleanup();

	system("pause");
	return;
}
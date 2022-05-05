/*
 * main.cpp
 *
 *  Created on: 29 abr 2022
 *      Author: Iker
 */

// IMPORTANT: Winsock Library ("ws2_32") should be linked
#include "functions/functions.h"
#include "handler/lib/sqlite3.h"
#include <stdio.h>
#include <stdlib.h>
#include <winsock2.h>

#define MAX_LINE 40
#define SERVER_IP "127.0.0.1" // Fichero de configuración
#define SERVER_PORT 6000 // Fichero de configuración

int main(int argc, char *argv[]) {
	WSADATA wsaData;
	SOCKET conn_socket;
	SOCKET comm_socket;
	struct sockaddr_in server;
	struct sockaddr_in client;
	char sendBuff[512], recvBuff[512];

	printf("\nInitialising Winsock...\n");
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
		printf("Failed. Error Code : %d", WSAGetLastError());
		return -1;
	}

	printf("Initialised.\n");

	//SOCKET creation
	if ((conn_socket = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET) {
		printf("Could not create socket : %d", WSAGetLastError());
		WSACleanup();
		return -1;
	}

	printf("Socket created.\n");

	server.sin_addr.s_addr = inet_addr(SERVER_IP);
	server.sin_family = AF_INET;
	server.sin_port = htons(SERVER_PORT);

	//BIND (the IP/port with socket)
	if (bind(conn_socket, (struct sockaddr*) &server,
			sizeof(server)) == SOCKET_ERROR) {
		printf("Bind failed with error code: %d", WSAGetLastError());
		closesocket(conn_socket);
		WSACleanup();
		return -1;
	}

	printf("Bind done.\n");

	//LISTEN to incoming connections (socket server moves to listening mode)
	if (listen(conn_socket, 1) == SOCKET_ERROR) {
		printf("Listen failed with error code: %d", WSAGetLastError());
		closesocket(conn_socket);
		WSACleanup();
		return -1;
	}

	//ACCEPT incoming connections (server keeps waiting for them)
	printf("Waiting for incoming connections...\n");
	int stsize = sizeof(struct sockaddr);
	comm_socket = accept(conn_socket, (struct sockaddr*) &client, &stsize);
	// Using comm_socket is able to send/receive data to/from connected client
	if (comm_socket == INVALID_SOCKET) {
		printf("accept failed with error code : %d", WSAGetLastError());
		closesocket(conn_socket);
		WSACleanup();
		return -1;
	}
	printf("Incomming connection from: %s (%d)\n", inet_ntoa(client.sin_addr),
			ntohs(client.sin_port));

	// Closing the listening sockets (is not going to be used anymore)
	closesocket(conn_socket);

	//SEND and RECEIVE data (CLIENT/SERVER PROTOCOL)
	printf("Waiting for incoming commands from client... \n");
	do {
		recv(comm_socket, recvBuff, sizeof(recvBuff), 0);

		printf("Command received: %s \n", recvBuff);

		// SHOWSTATS -------------------------------------------------- showStatistics();
		if (strcmp(recvBuff, "SHOWSTATS") == 0) {

			// Código aquí
			Data *dataArray = showStatistics();
			sqlite3_step(dataArray[0].stmt);
			sqlite3_step(dataArray[1].stmt);

			double stat1 = sqlite3_column_double(dataArray[0].stmt, 0);

			sprintf(sendBuff, "%lf", stat1);
			send(comm_socket, sendBuff, sizeof(sendBuff), 0);

			double stat2 = sqlite3_column_double(dataArray[1].stmt, 0);

			sprintf(sendBuff, "%lf", stat2);
			send(comm_socket, sendBuff, sizeof(sendBuff), 0);

			printf("Response sent: %s \n", sendBuff);
		}

		// SHOWSMKTS -------------------------------------------------- showSupermarkets(true);
		if (strcmp(recvBuff, "SHOWSMKTS") == 0) {

			// Código aquí
			showSupermarkets(true);

			send(comm_socket, sendBuff, sizeof(sendBuff), 0);
			printf("Response sent: %s \n", sendBuff);
		}

		// SHOWSMKTSPK -------------------------------------------------- showSupermarketPK();
		if (strcmp(recvBuff, "SHOWSMKTSPK") == 0) {

			// Código aquí
			showSupermarketPK();

			send(comm_socket, sendBuff, sizeof(sendBuff), 0);
			printf("Response sent: %s \n", sendBuff);
		}

		// SHOWPRODS -------------------------------------------------- showProducts(true);
		if (strcmp(recvBuff, "SHOWPRODS") == 0) {

			// Código aquí
			showProducts(true);

			send(comm_socket, sendBuff, sizeof(sendBuff), 0);
			printf("Response sent: %s \n", sendBuff);
		}

		// SHOWPRODSPK -------------------------------------------------- showProductPK();
		if (strcmp(recvBuff, "SHOWPRODSPK") == 0) {

			// Código aquí
			showProductPK();

			send(comm_socket, sendBuff, sizeof(sendBuff), 0);
			printf("Response sent: %s \n", sendBuff);
		}

		// ADDSMKTDB -------------------------------------------------- addSupermarketDB();
		if (strcmp(recvBuff, "ADDSMKTDB") == 0) {
			// Código aquí
			recv(comm_socket, recvBuff, sizeof(recvBuff), 0);
			while (strcmp(recvBuff, "ADDSMKTDB-END") != 0) {
				int n = atoi(recvBuff);
				// Código aquí
				recv(comm_socket, recvBuff, sizeof(recvBuff), 0);
			}
			//			sprintf(sendBuff, "%d", suma); // Cambiar
			send(comm_socket, sendBuff, sizeof(sendBuff), 0);
			printf("Response sent: %s \n", sendBuff);
		}

		// DELSMKTDB -------------------------------------------------- deleteSupermarketDB();
		if (strcmp(recvBuff, "DELSMKTDB") == 0) {
			// Código aquí
			recv(comm_socket, recvBuff, sizeof(recvBuff), 0);
			while (strcmp(recvBuff, "DELSMKTDB-END") != 0) {
				int n = atoi(recvBuff);
				// Código aquí
				recv(comm_socket, recvBuff, sizeof(recvBuff), 0);
			}
			//			sprintf(sendBuff, "%d", suma); // Cambiar
			send(comm_socket, sendBuff, sizeof(sendBuff), 0);
			printf("Response sent: %s \n", sendBuff);
		}

		// UDSMKTDB -------------------------------------------------- updateSupermarketDB();
		if (strcmp(recvBuff, "UDSMKTDB") == 0) {
			// Código aquí
			recv(comm_socket, recvBuff, sizeof(recvBuff), 0);
			while (strcmp(recvBuff, "UDSMKTDB-END") != 0) {
				int n = atoi(recvBuff);
				// Código aquí
				recv(comm_socket, recvBuff, sizeof(recvBuff), 0);
			}
			//			sprintf(sendBuff, "%d", suma); // Cambiar
			send(comm_socket, sendBuff, sizeof(sendBuff), 0);
			printf("Response sent: %s \n", sendBuff);
		}

		// ADDPRODDB -------------------------------------------------- addProductDB();
		if (strcmp(recvBuff, "ADDPRODDB") == 0) {
			// Código aquí
			recv(comm_socket, recvBuff, sizeof(recvBuff), 0);
			while (strcmp(recvBuff, "ADDPRODDB-END") != 0) {
				int n = atoi(recvBuff);
				// Código aquí
				recv(comm_socket, recvBuff, sizeof(recvBuff), 0);
			}
			//			sprintf(sendBuff, "%d", suma); // Cambiar
			send(comm_socket, sendBuff, sizeof(sendBuff), 0);
			printf("Response sent: %s \n", sendBuff);
		}

		// DELPRODDB -------------------------------------------------- deleteProductDB();
		if (strcmp(recvBuff, "DELPRODDB") == 0) {
			// Código aquí
			recv(comm_socket, recvBuff, sizeof(recvBuff), 0);
			while (strcmp(recvBuff, "DELPRODDB-END") != 0) {
				int n = atoi(recvBuff);
				// Código aquí
				recv(comm_socket, recvBuff, sizeof(recvBuff), 0);
			}
			//			sprintf(sendBuff, "%d", suma); // Cambiar
			send(comm_socket, sendBuff, sizeof(sendBuff), 0);
			printf("Response sent: %s \n", sendBuff);
		}

		// UDPRODDB -------------------------------------------------- updateProductDB();
		if (strcmp(recvBuff, "UDPRODDB") == 0) {
			// Código aquí
			recv(comm_socket, recvBuff, sizeof(recvBuff), 0);
			while (strcmp(recvBuff, "UDPRODDB-END") != 0) {
				int n = atoi(recvBuff);
				// Código aquí
				recv(comm_socket, recvBuff, sizeof(recvBuff), 0);
			}
			//			sprintf(sendBuff, "%d", suma); // Cambiar
			send(comm_socket, sendBuff, sizeof(sendBuff), 0);
			printf("Response sent: %s \n", sendBuff);
		}

		// Código de muestra
//		if (strcmp(recvBuff, "RAIZ") == 0) {
//			recv(comm_socket, recvBuff, sizeof(recvBuff), 0);
//			int n = atoi(recvBuff);
//			float raiz = sqrt(n);
//
//			recv(comm_socket, recvBuff, sizeof(recvBuff), 0);
//			if (strcmp(recvBuff, "RAIZ-END") == 0)
//				; // Nada que hacer
//
//			sprintf(sendBuff, "%f", raiz);
//			send(comm_socket, sendBuff, sizeof(sendBuff), 0);
//			printf("Response sent: %s \n", sendBuff);
//		}
//
//		if (strcmp(recvBuff, "IP") == 0) {
//			recv(comm_socket, recvBuff, sizeof(recvBuff), 0);
//			if (strcmp(recvBuff, "IP-END") == 0)
//				; // Nada que hacer
//
//			strcpy(sendBuff, inet_ntoa(server.sin_addr));
//			send(comm_socket, sendBuff, sizeof(sendBuff), 0);
//			printf("Response sent: %s \n", sendBuff);
//		}

// ¿Haría falta un comando EXIT?

//		if (strcmp(recvBuff, "EXIT") == 0)
//			break;

	} while (1);

	// CLOSING the sockets and cleaning Winsock...
	closesocket(comm_socket);
	WSACleanup();

	return 0;
}


/*
 * main.cpp
 *
 *  Created on: 29 abr 2022
 *      Author: Iker
 */

// IMPORTANT: Winsock Library ("ws2_32") should be linked
extern "C" {
#include "functions/functions.h"
#include "handler/lib/sqlite3.h"
#include "handler/logger/logger.h"
#include <stdio.h>
#include <stdlib.h>
#include <winsock2.h>
}

#define MAX_LINE 40
#define SERVER_IP "127.0.0.1" // Fichero de configuración
#define SERVER_PORT 6000 // Fichero de configuración

int main(int argc, char *argv[]) {
	openLogger("server.log");

	Properties prop = prepareDB();

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
			Data *dataArray = showStatistics();

			sqlite3_step(dataArray[0].stmt);
			sqlite3_step(dataArray[1].stmt);

			// Enviar stat1
			double stat1 = sqlite3_column_double(dataArray[0].stmt, 0);
			sprintf(sendBuff, "%lf", stat1);
			send(comm_socket, sendBuff, sizeof(sendBuff), 0);
			printf("\nSent: %s\n", sendBuff); // Comp.

			// Enviar stat2
			double stat2 = sqlite3_column_double(dataArray[1].stmt, 0);
			sprintf(sendBuff, "%lf", stat2);
			send(comm_socket, sendBuff, sizeof(sendBuff), 0);
			printf("Sent: %s\n", sendBuff); // Comp.
		}

		// SHOWSMKTS -------------------------------------------------- showSupermarkets();
		if (strcmp(recvBuff, "SHOWSMKTS") == 0) {
			sqlite3_stmt *stmt = showSupermarkets(true);

			while (sqlite3_step(stmt) == SQLITE_ROW) {

				// Enviar cod_s
				int cod_s = sqlite3_column_int(stmt, 0);
				sprintf(sendBuff, "%i", cod_s);
				send(comm_socket, sendBuff, sizeof(sendBuff), 0);
				printf("\nSent: %s ", sendBuff); // Comp.

				// Enviar nom_s
				char *nom_s = (char*) sqlite3_column_text(stmt, 1);
				sprintf(sendBuff, "%s", nom_s);
				send(comm_socket, sendBuff, sizeof(sendBuff), 0);
				printf("%s ", sendBuff); // Comp.

				// Enviar dir_s
				char *dir_s = (char*) sqlite3_column_text(stmt, 2);
				sprintf(sendBuff, "%s", dir_s);
				send(comm_socket, sendBuff, sizeof(sendBuff), 0);
				printf("%s ", sendBuff); // Comp.

				// Enviar tlf_s
				int tlf_s = sqlite3_column_int(stmt, 3);
				sprintf(sendBuff, "%i", tlf_s);
				send(comm_socket, sendBuff, sizeof(sendBuff), 0);
				printf("%s ", sendBuff); // Comp.

				// Enviar metros_cuad_s
				double metros_cuad_s = sqlite3_column_double(stmt, 4);
				sprintf(sendBuff, "%lf", metros_cuad_s);
				send(comm_socket, sendBuff, sizeof(sendBuff), 0);
				printf("%s ", sendBuff); // Comp.

				// Enviar cod_ciu
				int cod_ciu = sqlite3_column_int(stmt, 5);
				sprintf(sendBuff, "%i", cod_ciu);
				send(comm_socket, sendBuff, sizeof(sendBuff), 0);
				printf("%s\n", sendBuff); // Comp.
			}

			sqlite3_finalize(stmt);

			send(comm_socket, "END", sizeof("END"), 0);
		}

		// SHOWSMKTSPK -------------------------------------------------- showSupermarketPK();
		if (strcmp(recvBuff, "SHOWSMKTSPK") == 0) {
			sqlite3_stmt *stmt = showSupermarketPK();

			while (sqlite3_step(stmt) == SQLITE_ROW) {
				// Enviar cod_s
				int cod_s = sqlite3_column_int(stmt, 0);
				sprintf(sendBuff, "%i", cod_s);
				send(comm_socket, sendBuff, sizeof(sendBuff), 0);
				printf("\nSent: %s\n", sendBuff); // Comp.
			}

			sqlite3_finalize(stmt);

			send(comm_socket, "END", sizeof("END"), 0);
		}

		// SHOWPRODS -------------------------------------------------- showProducts();
		if (strcmp(recvBuff, "SHOWPRODS") == 0) {
			sqlite3_stmt *stmt = showProducts(true);

			while (sqlite3_step(stmt) == SQLITE_ROW) {

				// Enviar id_prod
				int id_prod = sqlite3_column_int(stmt, 0);
				sprintf(sendBuff, "%i", id_prod);
				send(comm_socket, sendBuff, sizeof(sendBuff), 0);
				printf("\nSent: %s ", sendBuff); // Comp.

				// Enviar nom_prod
				char *nom_prod = (char*) sqlite3_column_text(stmt, 1);
				sprintf(sendBuff, "%s", nom_prod);
				send(comm_socket, sendBuff, sizeof(sendBuff), 0);
				printf("%s ", sendBuff); // Comp.

				// Enviar precio_prod
				double precio_prod = sqlite3_column_double(stmt, 2);
				sprintf(sendBuff, "%lf", precio_prod);
				send(comm_socket, sendBuff, sizeof(sendBuff), 0);
				printf("%s ", sendBuff); // Comp.

				// Enviar desc_prod
				char *desc_prod = (char*) sqlite3_column_text(stmt, 3);
				sprintf(sendBuff, "%s", desc_prod);
				send(comm_socket, sendBuff, sizeof(sendBuff), 0);
				printf("%s\n", sendBuff); // Comp.
			}

			sqlite3_finalize(stmt);

			send(comm_socket, "END", sizeof("END"), 0);
		}

		// SHOWPRODSPK -------------------------------------------------- showProductPK();
		if (strcmp(recvBuff, "SHOWPRODSPK") == 0) {
			sqlite3_stmt *stmt = showProductPK();

			while (sqlite3_step(stmt) == SQLITE_ROW) {
				// Enviar id_prod
				int id_prod = sqlite3_column_int(stmt, 0);
				sprintf(sendBuff, "%i", id_prod);
				send(comm_socket, sendBuff, sizeof(sendBuff), 0);
				printf("\nSent: %s\n", sendBuff); // Comp.
			}

			sqlite3_finalize(stmt);

			send(comm_socket, "END", sizeof("END"), 0);
		}

		// ADDSMKTDB -------------------------------------------------- addSupermarketDB(sql, s);
		if (strcmp(recvBuff, "ADDSMKTDB") == 0) {
			char *sql;
			Supermercado s;

			// Recibir sql
			recv(comm_socket, recvBuff, sizeof(recvBuff), 0);
			//sscanf(recvBuff, "%s", sql);
			strcpy(sql, recvBuff);
			printf("\n%s\n", sql); // Comp.

			// Recibir cod_s
			recv(comm_socket, recvBuff, sizeof(recvBuff), 0);
			s.cod_s = atoi(recvBuff);
			//sscanf(recvBuff, "%i", &s.cod_s);
			printf("%i ", s.cod_s); // Comp.

			// Recibir nom_s
			recv(comm_socket, recvBuff, sizeof(recvBuff), 0);
			//sscanf(recvBuff, "%s", s.nom_s);
			strcpy(s.nom_s, recvBuff);
			printf("%s ", s.nom_s); // Comp.

			// Recibir dir_s
			recv(comm_socket, recvBuff, sizeof(recvBuff), 0);
			//sscanf(recvBuff, "%s", s.dir_s);
			strcpy(s.nom_s, recvBuff);
			printf("%s ", s.dir_s); // Comp.

			// Recibir tlf_s
			recv(comm_socket, recvBuff, sizeof(recvBuff), 0);
			s.tlf_s = atoi(recvBuff);
			//sscanf(recvBuff, "%i", &s.tlf_s);
			printf("%i ", s.tlf_s); // Comp.

			// Recibir metros_cuad_s
			recv(comm_socket, recvBuff, sizeof(recvBuff), 0);
			s.metros_cuad_s = atof(recvBuff);
			//sscanf(recvBuff, "%lf", &s.metros_cuad_s);
			printf("%lf ", s.metros_cuad_s); // Comp.

			// Recibir cod_ciu
			recv(comm_socket, recvBuff, sizeof(recvBuff), 0);
			s.cod_ciu = atoi(recvBuff);
			//sscanf(recvBuff, "%i", &s.cod_ciu);
			printf("%i", s.cod_ciu); // Comp.

			addSupermarketDB(sql, s);

//			while (strcmp(recvBuff, "ADDSMKTDB-END") != 0) {
//				int n = atoi(recvBuff);
//				// Código aquí
//				recv(comm_socket, recvBuff, sizeof(recvBuff), 0);
//			}

//			printf("Response sent: %s \n", sendBuff);
		}

		// DELSMKTDB -------------------------------------------------- deleteSupermarketDB(sql, cod_s);
		if (strcmp(recvBuff, "DELSMKTDB") == 0) {
			char *sql;
			int cod_s;

			// Recibir sql
			recv(comm_socket, recvBuff, sizeof(recvBuff), 0);
			sscanf(recvBuff, "%s", sql);

			// Recibir cod_s
			recv(comm_socket, recvBuff, sizeof(recvBuff), 0);
			sscanf(recvBuff, "%i", &cod_s);

			deleteSupermarketDB(sql, cod_s);

//			while (strcmp(recvBuff, "DELSMKTDB-END") != 0) {
//				int n = atoi(recvBuff);
//				// Código aquí
//				recv(comm_socket, recvBuff, sizeof(recvBuff), 0);
//			}

//			printf("Response sent: %s \n", sendBuff);
		}

		// UDSMKTDB -------------------------------------------------- updateSupermarketDB(sql, s);
		if (strcmp(recvBuff, "UDSMKTDB") == 0) {
			char *sql;
			Supermercado s;

			// Recibir sql
			recv(comm_socket, recvBuff, sizeof(recvBuff), 0);
			sscanf(recvBuff, "%s", sql);

			// Recibir cod_s
			recv(comm_socket, recvBuff, sizeof(recvBuff), 0);
			sscanf(recvBuff, "%i", &s.cod_s);

			// Recibir nom_s
			recv(comm_socket, recvBuff, sizeof(recvBuff), 0);
			sscanf(recvBuff, "%s", s.nom_s);

			// Recibir dir_s
			recv(comm_socket, recvBuff, sizeof(recvBuff), 0);
			sscanf(recvBuff, "%s", s.dir_s);

			// Recibir tlf_s
			recv(comm_socket, recvBuff, sizeof(recvBuff), 0);
			sscanf(recvBuff, "%i", &s.tlf_s);

			// Recibir metros_cuad_s
			recv(comm_socket, recvBuff, sizeof(recvBuff), 0);
			sscanf(recvBuff, "%lf", &s.metros_cuad_s);

			// Recibir cod_ciu
			recv(comm_socket, recvBuff, sizeof(recvBuff), 0);
			sscanf(recvBuff, "%i", &s.cod_ciu);

			updateSupermarketDB(sql, s);

//			while (strcmp(recvBuff, "UDSMKTDB-END") != 0) {
//				int n = atoi(recvBuff);
//				// Código aquí
//				recv(comm_socket, recvBuff, sizeof(recvBuff), 0);
//			}

//			printf("Response sent: %s \n", sendBuff);
		}

		// ADDPRODDB -------------------------------------------------- addProductDB(sql, p);
		if (strcmp(recvBuff, "ADDPRODDB") == 0) {
			char *sql;
			Producto p;

			// Recibir sql
			recv(comm_socket, recvBuff, sizeof(recvBuff), 0);
			sscanf(recvBuff, "%s", sql);

			// Recibir id_prod
			recv(comm_socket, recvBuff, sizeof(recvBuff), 0);
			sscanf(recvBuff, "%i", &p.id_prod);

			// Recibir nom_prod
			recv(comm_socket, recvBuff, sizeof(recvBuff), 0);
			sscanf(recvBuff, "%s", p.nom_prod);

			// Recibir precio_prod
			recv(comm_socket, recvBuff, sizeof(recvBuff), 0);
			sscanf(recvBuff, "%lf", &p.precio_prod);

			// Recibir desc_prod
			recv(comm_socket, recvBuff, sizeof(recvBuff), 0);
			sscanf(recvBuff, "%s", p.desc_prod);

			addProductDB(sql, p);

//			while (strcmp(recvBuff, "ADDPRODDB-END") != 0) {
//				int n = atoi(recvBuff);
//				// Código aquí
//				recv(comm_socket, recvBuff, sizeof(recvBuff), 0);
//			}

//			printf("Response sent: %s \n", sendBuff);
		}

		// DELPRODDB -------------------------------------------------- deleteProductDB(sql, id_prod);
		if (strcmp(recvBuff, "DELPRODDB") == 0) {
			char *sql;
			int id_prod;

			// Recibir sql
			recv(comm_socket, recvBuff, sizeof(recvBuff), 0);
			sscanf(recvBuff, "%s", sql);

			// Recibir id_prod
			recv(comm_socket, recvBuff, sizeof(recvBuff), 0);
			sscanf(recvBuff, "%i", &id_prod);

			deleteProductDB(sql, id_prod);

//			while (strcmp(recvBuff, "DELPRODDB-END") != 0) {
//				int n = atoi(recvBuff);
//				// Código aquí
//				recv(comm_socket, recvBuff, sizeof(recvBuff), 0);
//			}

//			printf("Response sent: %s \n", sendBuff);
		}

		// UDPRODDB -------------------------------------------------- updateProductDB(sql, p);
		if (strcmp(recvBuff, "UDPRODDB") == 0) {
			char *sql;
			Producto p;

			// Recibir sql
			recv(comm_socket, recvBuff, sizeof(recvBuff), 0);
			sscanf(recvBuff, "%s", sql);

			// Recibir id_prod
			recv(comm_socket, recvBuff, sizeof(recvBuff), 0);
			sscanf(recvBuff, "%i", &p.id_prod);

			// Recibir nom_prod
			recv(comm_socket, recvBuff, sizeof(recvBuff), 0);
			sscanf(recvBuff, "%s", p.nom_prod);

			// Recibir precio_prod
			recv(comm_socket, recvBuff, sizeof(recvBuff), 0);
			sscanf(recvBuff, "%lf", &p.precio_prod);

			// Recibir desc_prod
			recv(comm_socket, recvBuff, sizeof(recvBuff), 0);
			sscanf(recvBuff, "%s", p.desc_prod);

			updateProductDB(sql, p);

//			while (strcmp(recvBuff, "UDPRODDB-END") != 0) {
//				int n = atoi(recvBuff);
//				// Código aquí
//				recv(comm_socket, recvBuff, sizeof(recvBuff), 0);
//			}

//			printf("Response sent: %s \n", sendBuff);
		}

// ¿Haría falta un comando EXIT?

//		if (strcmp(recvBuff, "EXIT") == 0)
//			break;

	} while (1);

	// CLOSING the sockets and cleaning Winsock...
	closesocket(comm_socket);
	WSACleanup();
	closeDB();
	closeLogger();
	return 0;
}


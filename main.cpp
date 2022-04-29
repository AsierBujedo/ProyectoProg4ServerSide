/*
 * main.cpp
 *
 *  Created on: 29 abr 2022
 *      Author: Iker
 */

// IMPORTANT: Winsock Library ("ws2_32") should be linked
#include <stdio.h>
#include <stdlib.h>
#include <winsock2.h>

#define SERVER_IP "127.0.0.1" // Coger del properties
#define SERVER_PORT 6000 // Coger del properties

// NIVEL DE MENÚ: 5 (administrador)
void manageProdMenu() {
	int opt;
	char str[10];

	printf("\n-------------------\n");
	printf("GESTIONAR PRODUCTOS\n");
	printf("-------------------\n\n");
	printf("1. Añadir producto\n");
	printf("2. Eliminar producto\n");
	printf("3. Actualizar información de un producto\n");
	printf("4. Volver\n");
	printf("Introduzca una opción: ");
	fflush(stdout);
	fgets(str, 2, stdin);
	fflush(stdin);
	sscanf(str, "%i", &opt);

	switch (opt) {
	case 1:

		break;

	case 2:

		break;

	case 3:

		manageProdMenu();
		break;

	case 4:

		manageProdMenu();
		break;
	}
}

// NIVEL DE MENÚ: 5 (administrador)
void manageSuperMenu() {
	int opt;
	char str[10];

	printf("\n-----------------------\n");
	printf("GESTIONAR SUPERMERCADOS\n");
	printf("-----------------------\n\n");
	printf("1. Añadir supermercado\n");
	printf("2. Eliminar supermercado\n");
	printf("3. Actualizar información de un supermercado\n");
	printf("4. Volver\n");
	printf("Introduzca una opción: ");
	fflush(stdout);
	fgets(str, 2, stdin);
	fflush(stdin);
	sscanf(str, "%i", &opt);

	switch (opt) {
	case 1:

		manageSuperMenu();
		break;

	case 2:

		manageSuperMenu();
		break;

	case 3:

		manageSuperMenu();
		break;

	case 4:

		break;
	}
}

// NIVEL DE MENÚ: 4
void updateBDMenu() {
	int opt;
	char str[10];

	printf("\n-------------\n");
	printf("ACTUALIZAR BD\n");
	printf("-------------\n\n");
	printf("1. Gestionar supermercados\n");
	printf("2. Gestionar productos\n");
	printf("3. Volver\n");
	printf("Introduzca una opción: ");
	fflush(stdout);
	fgets(str, 2, stdin);
	fflush(stdin);
	sscanf(str, "%i", &opt);

	switch (opt) {
	case 1:

		break;

	case 2:

		break;

	case 3:

		break;
	}
}

// NIVEL DE MENÚ: 3 (usuario) y 4 (administrador)
void queryBDMenu() {
	int opt;
	char str[10];

	printf("\n-----------------\n");
	printf("CONSULTAS A LA BD\n");
	printf("-----------------\n\n");
	printf("1. Visualizar supermercados\n");
	printf("2. Visualizar productos\n");
	printf("3. Volver a menú principal\n");
	printf("Introduzca una opción: ");
	fflush(stdout);
	fgets(str, 2, stdin);
	fflush(stdin);
	sscanf(str, "%i", &opt);

	switch (opt) {
	case 1:

		break;

	case 2:

		break;

	case 3:

		break;
	}
}

// NIVEL DE MENÚ: 3
void adminMenu() {
	int opt;
	char str[10];

	printf("\n------------------\n");
	printf("MENÚ ADMINISTRADOR\n");
	printf("------------------\n\n");
	printf("1. Actualizar BD\n");
	printf("2. Consultas a la BD\n");
	printf("3. Estadísticas\n");
	printf("4. Volver al menú principal\n");
	printf("Introduzca una opción: ");
	fflush(stdout);
	fgets(str, 2, stdin);
	fflush(stdin);
	sscanf(str, "%i", &opt);

	switch (opt) {
	case 1:

		break;

	case 2:

		break;

	case 3:

		break;

	case 4:

		break;
	}
}

// NIVEL DE MENÚ: 2
void userMenu() {
	int opt;
	char str[10];

	printf("\n------------\n");
	printf("MENÚ USUARIO\n");
	printf("------------\n\n");
	printf("1. Consultas a la BD\n");
	printf("2. Estadísticas\n");
	printf("3. Volver al menú principal\n");
	printf("Introduzca una opción: ");
	fflush(stdout);
	fgets(str, 2, stdin);
	fflush(stdin);
	sscanf(str, "%i", &opt);

	switch (opt) {
	case 1:

		break;

	case 2:

		break;

	case 3:

		break;
	}
}

// NIVEL DE MENÚ: 2
void adminAccessMenu() {
	int pass;
	char str[10];

	printf("\n--------------------\n");
	printf("ACCESO ADMINISTRADOR\n");
	printf("--------------------\n\n");
	printf(
			"Introduzca la clave (si quiere volver al menú principal pulse 'q' y para continuar pulse ENTER): ");
	fflush(stdout);
	fgets(str, 6, stdin);
	fflush(stdin);
	sscanf(str, "%i", &pass);

	int propPass;

	if (pass == propPass) {

	} else if (str[0] == 'q') {

	} else {

	}

}

// NIVEL DE MENÚ: 1
void mainMenu(bool b) {
	int opt;
	char str[10];

	if (b) {
		printf("\n------------\n");
		printf("DEUSTOMARKET\n");
		printf("------------\n\n");
	} else {
		printf("------------\n");
		printf("DEUSTOMARKET\n");
		printf("------------\n\n");
	}

	printf("1. Entrar como usuario\n");
	printf("2. Entrar como administrador\n");
	printf("3. Salir\n");
	printf("Introduzca una opción: ");
	fflush(stdout);
	fgets(str, 2, stdin);
	fflush(stdin);
	sscanf(str, "%i", &opt);

	switch (opt) {
	case 1:

		break;

	case 2:

		break;

	case 3:

		exit(0);
	}

}

int main(int argc, char *argv[])
{

	WSADATA wsaData;
	SOCKET s;
	struct sockaddr_in server;
	char sendBuff[512], recvBuff[512];

	printf("\nInitialising Winsock...\n");
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
	{
		printf("Failed. Error Code : %d", WSAGetLastError());
		return -1;
	}

	printf("Initialised.\n");

	//SOCKET creation
	if ((s = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET)
	{
		printf("Could not create socket : %d", WSAGetLastError());
		WSACleanup();
		return -1;
	}

	printf("Socket created.\n");

	server.sin_addr.s_addr = inet_addr(SERVER_IP);
	server.sin_family = AF_INET;
	server.sin_port = htons(SERVER_PORT);

	//CONNECT to remote server
	if (connect(s, (struct sockaddr*) &server, sizeof(server)) == SOCKET_ERROR)
	{
		printf("Connection error: %d", WSAGetLastError());
		closesocket(s);
		WSACleanup();
		return -1;
	}

	printf("Connection stablished with: %s (%d)\n", inet_ntoa(server.sin_addr),
			ntohs(server.sin_port));

	//SEND and RECEIVE data (CLIENT/SERVER PROTOCOL)
	char c;
	do
	{
		if (c == '1')
		{
			// SENDING command SUMAR and parameters to the server
			strcpy(sendBuff, "SUMAR");
			send(s, sendBuff, sizeof(sendBuff), 0);
			strcpy(sendBuff, "2");
			send(s, sendBuff, sizeof(sendBuff), 0);
			strcpy(sendBuff, "3");
			send(s, sendBuff, sizeof(sendBuff), 0);
			strcpy(sendBuff, "5");
			send(s, sendBuff, sizeof(sendBuff), 0);
			strcpy(sendBuff, "SUMAR-END");
			send(s, sendBuff, sizeof(sendBuff), 0);

			// RECEIVING response to command SUMAR from the server
			recv(s, recvBuff, sizeof(recvBuff), 0);
			printf("Suma = %s \n", recvBuff);
		}

		if (c == '2')
		{
			// SENDING command RAIZ and parameter to the server
			strcpy(sendBuff, "RAIZ");
			send(s, sendBuff, sizeof(sendBuff), 0);
			strcpy(sendBuff, "9");
			send(s, sendBuff, sizeof(sendBuff), 0);
			strcpy(sendBuff, "RAIZ-END");
			send(s, sendBuff, sizeof(sendBuff), 0);

			// RECEIVING response to command RAIZ from the server
			recv(s, recvBuff, sizeof(recvBuff), 0);
			printf("Raiz cuadrada = %s \n", recvBuff);
		}

		if (c == '3')
		{
			// SENDING command IP
			strcpy(sendBuff, "IP");
			send(s, sendBuff, sizeof(sendBuff), 0);
			strcpy(sendBuff, "IP-END");
			send(s, sendBuff, sizeof(sendBuff), 0);

			// RECEIVING response to command IP from the server
			recv(s, recvBuff, sizeof(recvBuff), 0);
			printf("IP del servidor = %s \n", recvBuff);
		}

		if (c == '4')
		{
			// SENDING command EXIT
			strcpy(sendBuff, "EXIT");
			send(s, sendBuff, sizeof(sendBuff), 0);
		}
	}while(c != '4');

	// CLOSING the socket and cleaning Winsock...
	closesocket(s);
	WSACleanup();

	return 0;
}



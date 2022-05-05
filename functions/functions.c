/*
 * functions.c
 *
 *  Created on: 2 abr. 2022
 *      Author: Iker López
 */

#include "functions.h"
#include "../handler/DBH.h"
#include "../handler/logger/logger.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE 40

Data* showStatistics() {
	Data datos1;
	Data datos2;

	Data *dataArray = malloc(sizeof(Data) * 2);

	datos1 = executeQuery("SELECT AVG(PRECIO_PROD) FROM PRODUCTO");
	datos2 = executeQuery("SELECT AVG(METROS_CUAD_S) FROM SUPERMERCADO");

	dataArray[0] = datos1;
	dataArray[1] = datos2;

	return dataArray;
}

sqlite3_stmt* showSupermarkets(bool b) {
	char *sql = "SELECT * FROM SUPERMERCADO;";
	char strAux[2];

	if (b) {
		printf("\n-------------------------------\n");
		printf("LISTA COMPLETA DE SUPERMERCADOS\n");
		printf("-------------------------------\n\n");
	}

	Data datos = executeQuery(sql);
	printf(
			"CODIGO || NOMBRE || DIRECCION || TELEFONO || METROS_CUADRADOS || CODIGO_CIUDAD\n\n");
	while (sqlite3_step(datos.stmt) == SQLITE_ROW) {
		printf("%i || ", sqlite3_column_int(datos.stmt, 0));
		printf("%s || ", (char*) sqlite3_column_text(datos.stmt, 1));
		printf("%s || ", (char*) sqlite3_column_text(datos.stmt, 2));
		printf("%i || ", sqlite3_column_int(datos.stmt, 3));
		printf("%.2lf || ", sqlite3_column_double(datos.stmt, 4));
		printf("%i\n", sqlite3_column_int(datos.stmt, 5));
	}

	sqlite3_finalize(datos.stmt);

	logFile(INFO, "Lista completa de supermercados mostrada");

	if (b) {
		printf(
				"\n¡Lista completa de supermercados mostrada! Pulse ENTER para volver al menú principal: ");
		fflush(stdout);
		fgets(strAux, 2, stdin);
		fflush(stdin);
	}

	return NULL;
}

sqlite3_stmt* showSupermarketPK() {
	char *sql = "SELECT COD_S FROM SUPERMERCADO;";

	Data datos = executeQuery(sql);
	printf("CODIGOS DE SUPERMERCADO ACTUALES\n");
	while (sqlite3_step(datos.stmt) == SQLITE_ROW) {
		printf("%i\n", sqlite3_column_int(datos.stmt, 0));
	}

	sqlite3_finalize(datos.stmt);

	logFile(INFO, "Códigos de supermercados mostrados");

	return NULL;
}

sqlite3_stmt* showProducts(bool b) {
	char *sql = "SELECT * FROM PRODUCTO;";
	char strAux[2];

	if (b) {
		printf("\n---------------------------\n");
		printf("LISTA COMPLETA DE PRODUCTOS\n");
		printf("---------------------------\n\n");
	}

	Data datos = executeQuery(sql);
	printf("CODIGO || NOMBRE || PRECIO || DESCRIPCION\n\n");
	while (sqlite3_step(datos.stmt) == SQLITE_ROW) {
		printf("%i || ", sqlite3_column_int(datos.stmt, 0));
		printf("%s || ", (char*) sqlite3_column_text(datos.stmt, 1));
		printf("%.2lf || ", sqlite3_column_double(datos.stmt, 2));
		printf("%s\n", (char*) sqlite3_column_text(datos.stmt, 3));
	}

	sqlite3_finalize(datos.stmt);

	logFile(INFO, "Lista completa de productos mostrada");

	if (b) {
		printf(
				"\n¡Lista completa de productos mostrada! Pulse ENTER para volver al menú principal: ");
		fflush(stdout);
		fgets(strAux, 2, stdin);
		fflush(stdin);
	}

	return NULL;
}

sqlite3_stmt* showProductPK() {
	char *sql = "SELECT ID_PROD FROM PRODUCTO;";

	Data datos = executeQuery(sql);
	printf("CODIGOS DE PRODUCTO ACTUALES\n");
	while (sqlite3_step(datos.stmt) == SQLITE_ROW) {
		printf("%i\n", sqlite3_column_int(datos.stmt, 0));
	}

	sqlite3_finalize(datos.stmt);

	logFile(INFO, "Códigos de productos mostrados");

	return NULL;
}

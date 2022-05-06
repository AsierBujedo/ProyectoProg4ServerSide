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

Data* showStatistics() { // HECHO
	Data datos1;
	Data datos2;

	Data *dataArray = malloc(sizeof(Data) * 2);

	datos1 = executeQuery("SELECT AVG(PRECIO_PROD) FROM PRODUCTO");
	datos2 = executeQuery("SELECT AVG(METROS_CUAD_S) FROM SUPERMERCADO");

	dataArray[0] = datos1;
	dataArray[1] = datos2;

	return dataArray;
}

sqlite3_stmt* showSupermarkets(bool b) { // HECHO
	char *sql = "SELECT * FROM SUPERMERCADO;";

	Data datos = executeQuery(sql);

	return datos.stmt;
}

sqlite3_stmt* showSupermarketPK() {
	char *sql = "SELECT COD_S FROM SUPERMERCADO;";

	Data datos = executeQuery(sql);

	return datos.stmt;
}

sqlite3_stmt* showProducts(bool b) {
	char *sql = "SELECT * FROM PRODUCTO;";

	Data datos = executeQuery(sql);

	return datos.stmt;
}

sqlite3_stmt* showProductPK() {
	char *sql = "SELECT ID_PROD FROM PRODUCTO;";

	Data datos = executeQuery(sql);

	return datos.stmt;
}

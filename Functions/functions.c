/*
 * functions.c
 *
 *  Created on: 2 abr. 2022
 *      Author: Iker L?pez
 */

#include "functions.h"
#include "../Handler/DBH.h"
#include "../Handler/Logger/logger.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE 40

Properties prepareDB() {

	Properties prop;
	FILE *file;
	if ((file = fopen("config.prop", "r"))) {
		fclose(file);
		loadProperties(&prop, "config.prop");
	} else {
		prop.numProp = 12;
		char **propName = malloc(sizeof(char*) * prop.numProp);
		propName[0] = "IP";
		propName[1] = "PORT";
		propName[2] = "ADMIN_PASS";
		propName[3] = "DEBUG";
		propName[4] = "DATABASE_DIR";
		propName[5] = "CITY_DIR";
		propName[6] = "EMPLOYEE_DIR";
		propName[7] = "PRODUCT_DIR";
		propName[8] = "PROVINCE_DIR";
		propName[9] = "SUPERMARKET_DIR";
		propName[10] = "WORKS_DIR";
		propName[11] = "SELLS_DIR";
		prop.propName = propName;

		char **propValues = malloc(sizeof(char*) * prop.numProp);
		propValues[0] = "127.0.0.1";
		propValues[1] = "1024";
		propValues[2] = "11111";
		propValues[3] = "1";
		propValues[4] = "DeustoMarket.db";
		propValues[5] = "dataSource/ciudad.csv";
		propValues[6] = "dataSource/empleado.csv";
		propValues[7] = "dataSource/producto.csv";
		propValues[8] = "dataSource/provincia.csv";
		propValues[9] = "dataSource/supermercado.csv";
		propValues[10] = "dataSource/trabaja.csv";
		propValues[11] = "dataSource/vende.csv";
		prop.propValue = propValues;

		createProperties(&prop, "config.prop");
	}

	char *sql2 = "DROP TABLE IF EXISTS PROVINCIA;"
			"CREATE TABLE PROVINCIA ("
			"COD_PROV INT PRIMARY KEY NOT NULL,"
			"NOM_PROV VARCHAR(25));";

	char *sql3 =
			"DROP TABLE IF EXISTS CIUDAD;"
					"CREATE TABLE CIUDAD ("
					"COD_CIU INT PRIMARY KEY NOT NULL,"
					"NOM_CIU VARCHAR(25),"
					"COD_PROV INT, FOREIGN KEY (COD_PROV) REFERENCES PROVINCIA (COD_PROV));";

	char *sql1 = "DROP TABLE IF EXISTS SUPERMERCADO;"
			"CREATE TABLE SUPERMERCADO ("
			"COD_S INT PRIMARY KEY NOT NULL,"
			"NOM_S VARCHAR(25),"
			"DIR_S VARCHAR(35),"
			"TLF_S INT,"
			"METROS_CUAD_S DOUBLE,"
			"COD_CIU INT, FOREIGN KEY (COD_CIU) REFERENCES CIUDAD (COD_CIU));";

	char *sql4 =
			"DROP TABLE IF EXISTS EMPLEADO;"
					"CREATE TABLE EMPLEADO ("
					"DNI_EMP VARCHAR(9) PRIMARY KEY NOT NULL,"
					"NOM_EMP VARCHAR(25),"
					"SALARIO_EMP DOUBLE,"
					"FECHA_NCTO_EMP DATE,"
					"DIR_EMP VARCHAR(35),"
					"DNI_JEFE VARCHAR(9), FOREIGN KEY (DNI_JEFE) REFERENCES EMPLEADO (DNI_EMP));";

	char *sql5 = "DROP TABLE IF EXISTS PRODUCTO;"
			"CREATE TABLE PRODUCTO ("
			"ID_PROD INT PRIMARY KEY NOT NULL,"
			"NOM_PROD VARCHAR(25),"
			"PRECIO_PROD DOUBLE,"
			"DESC_PROD VARVHAR(35));";

	char *sql6 = "DROP TABLE IF EXISTS VENDE;"
			"CREATE TABLE VENDE ("
			"COD_S INT NOT NULL, "
			"ID_PROD INT NOT NULL, "
			"PRIMARY KEY(COD_S, ID_PROD),"
			"FOREIGN KEY (COD_S) REFERENCES SUPERMERCADO (COD_S),"
			"FOREIGN KEY (ID_PROD) REFERENCES PRODUCTO (ID_PROD));";

	char *sql7 = "DROP TABLE IF EXISTS TRABAJA;"
			"CREATE TABLE TRABAJA("
			"COD_S	INT NOT NULL,"
			"DNI_EMP VARCHAR(9) NOT NULL, "
			"PRIMARY KEY (COD_S, DNI_EMP) "
			"FOREIGN KEY (COD_S) REFERENCES SUPERMERCADO (COD_S),"
			"FOREIGN KEY (DNI_EMP) REFERENCES EMPLEADO (DNI_EMP));";

	initDB(prop.propValue[4]);

	executeStatement(sql1);
	executeStatement(sql2);
	executeStatement(sql3);
	executeStatement(sql4);
	executeStatement(sql5);
	executeStatement(sql6);
	executeStatement(sql7);

	csvCityLoader(prop.propValue[5]);
	csvEmployeeLoader(prop.propValue[6]);
	csvProductLoader(prop.propValue[7]);
	csvProvinceLoader(prop.propValue[8]);
	csvSupermarketLoader(prop.propValue[9]);
	csvWorksLoader(prop.propValue[10]);
	csvSellsLoader(prop.propValue[11]);

	return prop;
}

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

	printf("%i", sqlite3_column_int(datos.stmt, 0));

	return datos.stmt;
}

sqlite3_stmt* showProvinces(bool b) {
	char *sql = "SELECT * FROM PROVINCIA;";

	Data datos = executeQuery(sql);

	return datos.stmt;
}

sqlite3_stmt* showCities(bool b) {
	char *sql = "SELECT * FROM CIUDAD;";

	Data datos = executeQuery(sql);

	return datos.stmt;
}

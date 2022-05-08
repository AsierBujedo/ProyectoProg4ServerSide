/*
 * functions.h
 *
 *  Created on: 2 abr. 2022
 *      Author: Iker L�pez
 */

#ifdef __cplusplus
extern "C" {
#endif

#include "../handler/DBH.h"
#include "../handler/lib/sqlite3.h"
#include "../handler/properties/properties.h"
#include <stdbool.h>

#ifndef FUNCTIONS_H_
#define FUNCTIONS_H_

Properties prepareDB();
int prepareSocket();
Data* showStatistics();
sqlite3_stmt* showSupermarkets(bool b);
sqlite3_stmt* showSupermarketPK();
sqlite3_stmt* showProducts(bool b);
sqlite3_stmt* showProductPK();
void addSupermarket();
void deleteSupermarket();
void updateSupermarket();
void addProduct();
void deleteProduct();
void updateProduct();

#endif /* FUNCTIONS_H_ */

#ifdef __cplusplus
}
#endif

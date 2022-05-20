/*
 * functions.h
 *
 *  Created on: 2 abr. 2022
 *      Author: Iker López
 */

#ifdef __cplusplus
extern "C" {
#endif

#include "../Handler/DBH.h"
#include "../Handler/Lib/sqlite3.h"
#include "../Handler/Properties/properties.h"
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

/*
 * functions.h
 *
 *  Created on: 2 abr. 2022
 *      Author: Iker López
 */

#ifdef __cplusplus
extern "C" {
#endif

#ifndef FUNCTIONS_FUNCTIONS_H_
#define FUNCTIONS_FUNCTIONS_H_
#include "../handler/lib/sqlite3.h"
#include "../handler/DBH.h"
#include <stdbool.h>

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

#endif /* FUNCTIONS_FUNCTIONS_H_ */

#ifdef __cplusplus
}
#endif

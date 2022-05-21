/*
 * properties.h
 *
 *  Created on: 1 abr. 2022
 *      Author: asier
 */

#ifndef PROPERTIES_H_
#define PROPERTIES_H_

typedef struct {
	int numProp;
	char **propName;
	char **propValue;
} Properties;

void loadProperties(Properties *properties, char name[]);
void createProperties(Properties *properties, char name[]);

#endif /* PROPERTIES_H_ */

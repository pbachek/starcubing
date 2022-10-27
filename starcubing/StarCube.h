/* 
 * File:   StarCube.h
 * Author: Paul
 *
 * Created on October 14, 2019, 1:32 PM
 */

#ifndef STARCUBE_H
#define	STARCUBE_H

#include <vector>
#include <map>
#include <string>
#include <fstream>

typedef std::string attribute_t; // attributes are represented as strings
typedef std::vector<attribute_t> tuple_t; // Single tuple of data
typedef std::vector<tuple_t> dataSet_t; // Array of tuples
typedef std::map<tuple_t, int> baseTable_t; // Base table
typedef std::vector<std::map<attribute_t, int> > starTable_t; // Star table

std::ostream& operator<<(std::ostream& os, const tuple_t& t);

void starCubeMain(dataSet_t dataSet, int icebergVal);

#endif	/* STARCUBE_H */


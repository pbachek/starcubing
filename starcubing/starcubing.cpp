/*
* File:   main.cpp
* Author: Paul
*
* Created on October 14, 2019, 9:58 AM
*/

#include <cstdlib>
#include <fstream>
#include <sstream>
#include <iostream>
#include "StarCube.h"

using namespace std;

/*
*
*/
int main(int argc, char** argv) {
	// check command line arguments
	if (argc != 3)
		return 2;
	string filename = argv[1];
	int iceberg = stoi(argv[2]);
	
	cout << "Parsing data..." << endl;
	// Parse data
	dataSet_t dataSet(0, tuple_t(0, ""));
	ifstream ifp;
	ifp.open(filename);
	if (!ifp.is_open()) return 1;
	while (!ifp.eof()) {
		tuple_t tuple(0, "");
		attribute_t attribute;
		string line;
		getline(ifp, line);
		stringstream linestream(line);
		while(getline(linestream, attribute, ',')){
		    if (!attribute.empty()) tuple.push_back(attribute);
		}
		if (tuple.size()) {
			dataSet.push_back(tuple);
		}
	}
	ifp.close();

	cout << "Partitioning data..." << endl;
	// Partition data if it is continuous
	for (int i = 0; i < dataSet[1].size(); i++) {
		int cardinality = stoi(dataSet[1][i]);
		if (cardinality) {
			float max, min;
			bool init = false;
			for (int j = 2; j < dataSet.size(); j++) {
				float val;
				try {
					val = stof(dataSet[j][i]);
				}
				catch (const exception& e) {
					continue;
				}
				if (!init) {
					max = min = val;
					init = true;
				}
				max = val > max ? val : max;
				min = val < min ? val : min;
			}
			for (int j = 2; j < dataSet.size(); j++) {
				float thresh, val;
				try {
					val = stof(dataSet[j][i]);
				}
				catch (const exception& e) {
					continue;
				}
				thresh = min + (max - min) / cardinality;
				while (val > thresh) {
					thresh += (max - min) / cardinality;
				}
				stringstream range;
				range << thresh - (max - min) / cardinality << "-" << thresh;
				dataSet[j][i] = range.str();
			}
		}
	}
	dataSet.erase(dataSet.begin() + 1);

	cout << "Computing iceberg cubes..." << endl;
	// run star cubing algorithm
	starCubeMain(dataSet, iceberg);

	return 0;
}

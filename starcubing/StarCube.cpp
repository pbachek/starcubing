/*
* File:   StarCube.cpp
* Author: Paul
*/

#include "StarCube.h"
#include "StarTree.h"
#include "StarTreeNode.h"

std::ofstream ofp;
int icebergParam;

// Override operator to print out a tuple

std::ostream& operator<<(std::ostream& os, const tuple_t& t) {
    std::string s = "{";
    for (int i = 0; i < t.size(); i++) {
        if (i) s += ',';
        s += t[i];
    }
    s += "}";
    return os << s;
}

// Star cube recursive algorithm

void starCube(StarTree *tree, StarTreeNode *node) {
    for (int i = 0; i < tree->getNumChildren(); i++) { // loop over child trees
        StarTreeNode *childRoot = tree->getChild(i)->getRoot(); // get child tree root
        if (node->getDimension() > childRoot->getDimension()) { // if this node should be added to child tree
            tuple_t tuple = node->getTuple(childRoot->getDimension()); // get the tuple of this node to dimension of child tree
            StarTreeNode *cnode = childRoot; // set pointer to child tree root
            for (int n, d = 0; d < tuple.size(); d++) { // loop over all values in the tuple
                for (n = 0; n < cnode->getNumChildren() && tuple[d] != cnode->getChild(n)->getAttribute(); n++); // find node with this attribute from the tuple
                if (d == (node->getDimension() - childRoot->getDimension() - 1)) { // only insert or aggregate node on the level of this node
                    if (n == cnode->getNumChildren()) { // if node is not already present
                        cnode->addChild(new StarTreeNode(node->getCount(), node->getDimension(), node->getAttribute())); // insert new node
                    } else { // if node already exists
                        cnode->getChild(n)->incrementCount(node->getCount()); // aggregate count
                    }
                }
                cnode = cnode->getChild(n); // go to next child
            }
        }
    }
    StarTree *newStarTree = NULL; // initialize new star tree to null
    if (node->getCount() >= icebergParam) { // see if node meets iceberg condition
		tuple_t tuple(tree->getTuple()), t2(node->getTuple()); // get root tree and current node tuples
		tuple.insert(tuple.end(), t2.begin(), t2.end()); // concatenate tuples
        if (node->getParent()) { // if node is not a root
			int i;
			for (i = 0; i < tuple.size() && tuple[i].find('*') == std::string::npos; i++); // if any attribute in tuple is *, do not print
			if (i == tuple.size()) ofp << node->getCount() << "\t: " << tuple << std::endl; // print out cuboid
        }
		if (node->getNumChildren() && node->hasNonStarChild()) { // if node is not a leaf
			StarTreeNode *newStarTreeNode = new StarTreeNode(node->getCount(), node->getDimension() + 1); // create a new star tree node
			newStarTree = new StarTree(newStarTreeNode); // create a new star tree with new node as root
			for (int i = 0; i < tuple.size(); i++) { // loop over all attributes
				newStarTree->addAttribute(tuple[i]); // add all attributes to new tree
			}
			tree->addChild(newStarTree); // add new tree as child of current tree
		}
    }
    if (node->getNumChildren()) { // if not a leaf
        starCube(tree, node->getChild(0)); // continue algorithm at next node
    }
    if (newStarTree) { // if new star tree was created
        starCube(newStarTree, newStarTree->getRoot()); // continue algorithm at next tree
		tree->removeChild(newStarTree); // remove star tree from list of children
    }
    if (node->getSibling()) { // if node has sibling
        starCube(tree, node->getSibling()); // continue algorithm at sibling
    }
}

// Star cubing algorithm

void starCubeMain(dataSet_t dataSet, int icebergVal) {
	// assign global iceberg value
	icebergParam = icebergVal;

	// extract dimension names
	std::vector<std::string>names(dataSet[0]);
	dataSet.erase(dataSet.begin());

    // create star table
    starTable_t starTable(dataSet[0].size(), std::map<attribute_t, int>());
    for (int i = 0; i < dataSet.size(); i++) {
        for (int d = 0; d < dataSet[0].size(); d++) {
            starTable[d][dataSet[i][d]]++;
        }
    }

    // overwrite data that doesn't meet iceberg 
    for (int i = 0; i < dataSet.size(); i++) {
        for (int d = 0; d < dataSet[0].size(); d++) {
            if (starTable[d][dataSet[i][d]] < icebergParam) {
                dataSet[i][d] = "*";
            }
			dataSet[i][d].insert(0, names[d] + "=");
        }
    }

    // create reduced base table
    baseTable_t baseTable;
    for (int i = 0; i < dataSet.size(); i++) {
        baseTable[dataSet[i]]++;
    }

    // Create base star tree
    StarTree *base = new StarTree(new StarTreeNode(0, 0));
    for (std::map<tuple_t, int>::iterator it = baseTable.begin(); it != baseTable.end(); it++) {
        StarTreeNode *cnode = base->getRoot();
        cnode->incrementCount(it->second);
        for (int n, d = 0; d < it->first.size(); d++) {
            for (n = 0; n < cnode->getNumChildren() && it->first[d] != cnode->getChild(n)->getAttribute(); n++);
            if (n == cnode->getNumChildren()) {
                cnode->addChild(new StarTreeNode(it->second, d + 1, it->first[d]));
            } else {
                cnode->getChild(n)->incrementCount(it->second);
            }
            cnode = cnode->getChild(n);
        }
    }

    // Compute cuboids
	ofp.open("cuboids.txt", std::ofstream::out | std::ofstream::trunc);
	ofp << base->getRoot()->getCount() << "\t: " << base->getRoot()->getTuple() << std::endl; // print out apex cuboid
    starCube(base, base->getRoot());
	ofp.close();
}

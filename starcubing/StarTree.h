/* 
 * File:   StarTree.h
 * Author: Paul
 *
 * Created on October 14, 2019, 1:31 PM
 */

#ifndef STARTREE_H
#define	STARTREE_H

#include "StarCube.h"
#include "StarTreeNode.h"

class StarTree {
private:
    std::vector<StarTree *> children;
    StarTreeNode *root;
    tuple_t tuple;
public:
    StarTree(StarTreeNode *root);
	~StarTree();

    StarTreeNode *getRoot();
    StarTree *getChild(int i);
    int getNumChildren();
    tuple_t getTuple();

    void addChild(StarTree *child);
	void removeChild(StarTree *child);
    void addAttribute(attribute_t attribute);
};

#endif	/* STARTREE_H */


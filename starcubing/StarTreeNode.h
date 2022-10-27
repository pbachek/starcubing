/* 
 * File:   StarTreeNode.h
 * Author: Paul
 *
 * Created on October 14, 2019, 1:31 PM
 */

#ifndef STARTREENODE_H
#define	STARTREENODE_H

#include "StarCube.h"

class StarTreeNode {
private:
    StarTreeNode *parent;
    StarTreeNode *sibling;
    std::vector<StarTreeNode *> children;
    attribute_t attribute;
    int count;
    int dimension;
public:
    StarTreeNode(int count, int dimension);
    StarTreeNode(int count, int dimension, attribute_t attribute);
	~StarTreeNode();

    StarTreeNode *getParent();
    StarTreeNode *getSibling();
    StarTreeNode *getChild(int i);
    int getNumChildren();
    attribute_t getAttribute();
    int getCount();
    tuple_t getTuple(int dimension = 0);
    int getDimension();

    void setParent(StarTreeNode *parent);
    void setSibling(StarTreeNode *sibling);
    void addChild(StarTreeNode *child);
    void setAttribute(attribute_t attribute);
    void incrementCount(int inc);
    void setDimension(int dimension);

	bool hasNonStarChild();
};

#endif	/* STARTREENODE_H */


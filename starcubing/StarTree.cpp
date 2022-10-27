#include "StarTree.h"

StarTree::StarTree(StarTreeNode *root) :
root(root),
children(0, NULL),
tuple(0, "") {
}

StarTree::~StarTree() {
	for (int i = 0; i < this->getNumChildren(); i++) {
		delete this->children[i];
	}
	delete root;
}

StarTreeNode *StarTree::getRoot() {
    return this->root;
}

StarTree *StarTree::getChild(int i) {
    if (i < this->children.size())
        return this->children[i];
    else
        return NULL;
}

int StarTree::getNumChildren() {
    return this->children.size();
}

tuple_t StarTree::getTuple() {
    return this->tuple;
}

void StarTree::addChild(StarTree *child) {
    this->children.push_back(child);
}

void StarTree::removeChild(StarTree *child) {
	for (int i = 0; i < this->getNumChildren(); i++) {
		if (this->getChild(i) == child) {
			this->children.erase(this->children.begin() + i);
			delete child;
		}
	}
}

void StarTree::addAttribute(attribute_t attribute) {
    this->tuple.push_back(attribute);
}

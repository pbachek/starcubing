#include "StarTreeNode.h"

StarTreeNode::StarTreeNode(int count, int dimension) : StarTreeNode(count, dimension, "") {
}

StarTreeNode::StarTreeNode(int count, int dimension, attribute_t attribute) :
count(count),
dimension(dimension),
attribute(attribute),
children(0, NULL),
parent(NULL),
sibling(NULL) {
}

StarTreeNode::~StarTreeNode() {
	for (int i = 0; i < this->getNumChildren(); i++) {
		delete this->children[i];
	}
}

StarTreeNode *StarTreeNode::getParent() {
    return this->parent;
}

StarTreeNode *StarTreeNode::getSibling() {
    return this->sibling;
}

StarTreeNode *StarTreeNode::getChild(int i) {
    if (i < children.size())
        return this->children[i];
    else
        return NULL;
}

int StarTreeNode::getNumChildren() {
    return this->children.size();
}

attribute_t StarTreeNode::getAttribute() {
    return this->attribute;
}

int StarTreeNode::getCount() {
    return this->count;
}

int StarTreeNode::getDimension() {
    return this->dimension;
}

void StarTreeNode::setParent(StarTreeNode *parent) {
    this->parent = parent;
}

void StarTreeNode::setSibling(StarTreeNode *sibling) {
    this->sibling = sibling;
}

void StarTreeNode::addChild(StarTreeNode *child) {
    int n = this->children.size();
    child->setParent(this);
    this->children.push_back(child);
    if (n) this->getChild(n - 1)->setSibling(child);
}

void StarTreeNode::setAttribute(attribute_t attribute) {
    this->attribute = attribute;
}

void StarTreeNode::incrementCount(int count) {
    this->count += count;
}

tuple_t StarTreeNode::getTuple(int dimension) {
    tuple_t tuple(0, "");
    StarTreeNode *cnode = this;
    do {
        if (!cnode->getAttribute().empty()) tuple.push_back(cnode->getAttribute());
    } while ((cnode = cnode->getParent()) && cnode->getDimension() > dimension);
    std::reverse(tuple.begin(), tuple.end());
    return tuple;
}

void StarTreeNode::setDimension(int dimension) {
    this->dimension = dimension;
}

bool StarTreeNode::hasNonStarChild() {
	for (int i = 0; i < this->getNumChildren(); i++) {
		if (this->getChild(i)->getAttribute().find('*') == std::string::npos) {
			return true;
		}
		else if (this->getChild(i)->hasNonStarChild()) {
			return true;
		}
	}
	return false;
}

#pragma once
#include <algorithm>
#include <vector>
#include "Node.h"

class SelfSortingQueue
{
public:
	void push(Node* node);
	Node* pop();
	bool empty();

	std::vector<Node*> getVector();

	~SelfSortingQueue();

private:
	std::vector<Node*> _nodes;

};


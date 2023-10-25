#include "SelfSortingQueue.h"

void SelfSortingQueue::push(Node* node)
{
	this->_nodes.push_back(node);
	std::sort(this->_nodes.begin(), this->_nodes.end(), [](Node* a, Node* b) {
		return a->f > b->f;
	});
}

Node* SelfSortingQueue::pop()
{
	if (this->_nodes.empty()) {
		return nullptr;
	}

	auto node = this->_nodes.back();
	this->_nodes.pop_back();
	return node;
}

bool SelfSortingQueue::empty()
{
	return this->_nodes.empty();
}

std::vector<Node*> SelfSortingQueue::getVector()
{
	return this->_nodes;
}

SelfSortingQueue::~SelfSortingQueue()
{
	for (auto& node : this->_nodes) {
		delete node;
	}
}

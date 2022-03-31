#include "Node.h"

Node::Node()
{
	_parent = nullptr;
	_state = NodeState::FAILURE;
}

Node::~Node()
{
	for (Node* node : children)
		delete node;
	children.clear();
}

void Node::AddChild(Node* node)
{
	node->_parent = this;
	children.push_back(node);
}

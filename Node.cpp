#include "Node.h"

Node::Node(std::vector<Node*> children)
{
	_parent = nullptr;
	_state = NodeState::FAILURE;
	for (int i = 0; i < children.size(); i++)
		AddChild(children[i]);
}

Node::Node()
{
}

Node::~Node()
{
	for (Node* node : _children)
		delete node;
	_children.clear();
	_parent = nullptr;
}

void Node::AddChild(Node* node)
{
	node->_parent = this;
	_children.push_back(node);
}

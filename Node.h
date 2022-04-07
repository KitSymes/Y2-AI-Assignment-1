#pragma once
#ifndef _NODE_H
#define _NODE_H

#include <iostream>
#include <vector>
#include <iostream>
#include <sstream>
#include <string>

enum class NodeState
{
	RUNNING,
	SUCCESS,
	FAILURE
};

class Node
{
protected:
	Node* _parent;
	std::vector<Node*> _children;
	NodeState _state;
public:
	Node(std::vector<Node*> children);
	Node();
	~Node();

	void AddChild(Node* node);

	virtual NodeState Evauluate(float dt) { return NodeState::FAILURE; };
};
#endif

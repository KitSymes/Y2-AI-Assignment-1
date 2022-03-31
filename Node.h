#pragma once
#ifndef _NODE_H
#define _NODE_H

#include <iostream>
#include <vector>

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
	std::vector<Node*> children;
	NodeState _state;
public:
	Node();
	~Node();

	void AddChild(Node* node);

	virtual NodeState Evauluate() { return NodeState::FAILURE; };
};
#endif

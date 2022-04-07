#include "Selector.h"

Selector::Selector(std::vector<Node*> children) : Node(children)
{
}

NodeState Selector::Evauluate(float dt)
{
	for (Node* node : _children)
		switch (node->Evauluate(dt))
		{
		case NodeState::RUNNING:
			_state = NodeState::RUNNING;
			return _state;
		case NodeState::SUCCESS:
			_state = NodeState::SUCCESS;
			return _state;
		case NodeState::FAILURE:
		default:
			continue;
		}

	_state = NodeState::FAILURE;
	return _state;
}

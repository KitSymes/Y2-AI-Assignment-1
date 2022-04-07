#include "Sequence.h"

Sequence::Sequence(std::vector<Node*> children) : Node(children)
{
}

NodeState Sequence::Evauluate(float dt)
{
	for (Node* node : _children)
		switch (node->Evauluate(dt))
		{
		case NodeState::RUNNING:
			_state = NodeState::RUNNING;
			return _state;
		case NodeState::FAILURE:
			_state = NodeState::FAILURE;
			return _state;
		case NodeState::SUCCESS:
		default:
			continue;
		}

	_state = NodeState::SUCCESS;
	return _state;
}

#include "Selector.h"

NodeState Selector::Evauluate()
{
	for (Node* node : children)
		switch (node->Evauluate())
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

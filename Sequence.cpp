#include "Sequence.h"

NodeState Sequence::Evauluate()
{
	bool isChildRunning = false;

	for (Node* node : children)
		switch (node->Evauluate())
		{
		case NodeState::RUNNING:
			isChildRunning = true;
			continue;
		case NodeState::FAILURE:
			_state = NodeState::FAILURE;
			return _state;
		case NodeState::SUCCESS:
		default:
			continue;
		}

	_state = isChildRunning ? NodeState::RUNNING : NodeState::SUCCESS;
}

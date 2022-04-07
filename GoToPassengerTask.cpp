#include "GoToPassengerTask.h"
#include "Vehicle.h"

GoToPassengerTask::GoToPassengerTask(BehaviourTree* tree)
{
	_tree = tree;
}

NodeState GoToPassengerTask::Evauluate(float dt)
{
	if (_tree->passengerCollected)
	{
		_state = NodeState::SUCCESS;
		return _state;
	}

	_tree->SetTargetPosition(_tree->GetPassenger()->getPosition());

	_state = NodeState::RUNNING;
	return _state;
}

#include "GoToFuelTask.h"
#include "Vehicle.h"

GoToFuelTask::GoToFuelTask(BehaviourTree* tree)
{
	_tree = tree;
}

NodeState GoToFuelTask::Evauluate(float dt)
{
	if (_tree->fuelCollected)
	{
		_state = NodeState::SUCCESS;
		return _state;
	}

	_tree->SetTargetPosition(_tree->GetFuel()->getPosition());

	_state = NodeState::RUNNING;
	return _state;
}

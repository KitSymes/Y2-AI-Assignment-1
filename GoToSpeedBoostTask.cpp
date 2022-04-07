#include "GoToSpeedBoostTask.h"
#include "Vehicle.h"

GoToSpeedBoostTask::GoToSpeedBoostTask(BehaviourTree* tree)
{
	_tree = tree;
}

NodeState GoToSpeedBoostTask::Evauluate(float dt)
{
	if (_tree->speedboostCollected)
	{
		_state = NodeState::SUCCESS;
		return _state;
	}

	_tree->SetTargetPosition(_tree->GetSpeedboost()->getPosition());

	_state = NodeState::RUNNING;
	return _state;
}

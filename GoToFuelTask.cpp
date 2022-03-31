#include "GoToFuelTask.h"

GoToFuelTask::GoToFuelTask(BehaviourTree* tree)
{
	_tree = tree;
}

NodeState GoToFuelTask::Evauluate()
{

	return NodeState::FAILURE;
}

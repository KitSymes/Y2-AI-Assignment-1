#pragma once
#ifndef _GOTOSPEEDBOOSTTASK_H
#define _GOTOSPEEDBOOSTTASK_H

#include "Node.h"
#include "BehaviourTree.h"

class GoToSpeedBoostTask :
	public Node
{
private:
	BehaviourTree* _tree;
public:
	GoToSpeedBoostTask(BehaviourTree* tree);

	NodeState Evauluate(float dt) override;
};
#endif

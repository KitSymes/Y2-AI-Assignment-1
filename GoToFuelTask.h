#pragma once
#ifndef _GOTOFUELTASK_H
#define _GOTOFUELTASK_H

#include "Node.h"
#include "BehaviourTree.h"

class GoToFuelTask :
	public Node
{
private:
	BehaviourTree* _tree;
public:
	GoToFuelTask(BehaviourTree* tree);

	NodeState Evauluate() override;
};
#endif

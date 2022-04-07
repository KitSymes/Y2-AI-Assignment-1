#pragma once
#ifndef _GOTOPASSENGERTASK_H
#define _GOTOPASSENGERTASK_H

#include "Node.h"
#include "BehaviourTree.h"

class GoToPassengerTask :
	public Node
{
private:
	BehaviourTree* _tree;
public:
	GoToPassengerTask(BehaviourTree* tree);

	NodeState Evauluate(float dt) override;
};
#endif

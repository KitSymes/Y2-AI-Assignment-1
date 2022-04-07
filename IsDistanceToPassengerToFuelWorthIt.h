#pragma once
#ifndef _ISDISTANCETOPASSENGERTOFUELWORTHIT_H
#define _ISDISTANCETOPASSENGERTOFUELWORTHIT_H

#include "Node.h"
#include "BehaviourTree.h"

class IsDistanceToPassengerToFuelWorthIt :
	public Node
{
private:
	BehaviourTree* _tree;
public:
	IsDistanceToPassengerToFuelWorthIt(BehaviourTree* tree);

	NodeState Evauluate(float dt) override;
};
#endif

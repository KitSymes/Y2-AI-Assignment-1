#pragma once
#ifndef _ISDISTANCETOSPEEDBOOSTTOPASSENGERTOFUELWORTHIT_H
#define _ISDISTANCETOSPEEDBOOSTTOPASSENGERTOFUELWORTHIT_H

#include "Node.h"
#include "BehaviourTree.h"

class IsDistanceToSpeedBoostToPassengerToFuelWorthIt :
	public Node
{
private:
	BehaviourTree* _tree;
public:
	IsDistanceToSpeedBoostToPassengerToFuelWorthIt(BehaviourTree* tree);

	NodeState Evauluate(float dt) override;
};
#endif

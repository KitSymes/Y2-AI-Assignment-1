#pragma once
#ifndef _ISDISTANCETOSPEEDBOOSTTOFUELWORTHIT_H
#define _ISDISTANCETOSPEEDBOOSTTOFUELWORTHIT_H

#include "Node.h"
#include "BehaviourTree.h"

class IsDistanceToSpeedBoostToFuelWorthIt :
	public Node
{
private:
	BehaviourTree* _tree;
public:
	IsDistanceToSpeedBoostToFuelWorthIt(BehaviourTree* tree);

	NodeState Evauluate(float dt) override;
};
#endif

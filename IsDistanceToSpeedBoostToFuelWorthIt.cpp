#include "IsDistanceToSpeedBoostToFuelWorthIt.h"
#include "Vehicle.h"

IsDistanceToSpeedBoostToFuelWorthIt::IsDistanceToSpeedBoostToFuelWorthIt(BehaviourTree* tree)
{
	_tree = tree;
}

NodeState IsDistanceToSpeedBoostToFuelWorthIt::Evauluate(float dt)
{
	float speedBoostTemp = _tree->GetVehicle()->GetSpeedBoostDistance();
	float distance = 0.0f;
	// Self -> Speed Boost
	float selfToSB = _tree->GetVehicle()->getPosition().Distance(_tree->GetSpeedboost()->getPosition());
	distance += max(selfToSB / 2.0f, selfToSB - (speedBoostTemp / 2.0f));
	speedBoostTemp -= max(selfToSB, 0.0f);
	// Speed Boost -> Fuel
	float sbToF = _tree->GetSpeedboost()->getPosition().Distance(_tree->GetFuel()->getPosition());
	distance += max(sbToF / 2.0f, sbToF - (speedBoostTemp / 2.0f));

	std::stringstream out;
	out << "->SB->F " << distance << endl;
	OutputDebugStringA(out.str().c_str());

	if (distance <= _tree->GetVehicle()->GetFuelDistance())
		_state = NodeState::SUCCESS;
	else
		_state = NodeState::FAILURE;

	return _state;
}

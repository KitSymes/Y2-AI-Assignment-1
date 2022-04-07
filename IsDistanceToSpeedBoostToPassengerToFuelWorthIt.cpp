#include "IsDistanceToSpeedBoostToPassengerToFuelWorthIt.h"
#include "Vehicle.h"

IsDistanceToSpeedBoostToPassengerToFuelWorthIt::IsDistanceToSpeedBoostToPassengerToFuelWorthIt(BehaviourTree* tree)
{
	_tree = tree;
}

NodeState IsDistanceToSpeedBoostToPassengerToFuelWorthIt::Evauluate(float dt)
{
	float speedBoostTemp = _tree->GetVehicle()->GetSpeedBoostDistance();
	float distance = 0.0f;
	// Self -> Speed Boost
	float selfToSB = _tree->GetVehicle()->getPosition().Distance(_tree->GetSpeedboost()->getPosition());
	distance += max(selfToSB / 2.0f, selfToSB - (speedBoostTemp / 2.0f));
	speedBoostTemp = SPEEDBOOSTMAX;
	// Speed Boost -> Passenger
	float sbToP = _tree->GetSpeedboost()->getPosition().Distance(_tree->GetPassenger()->getPosition());
	distance += max(sbToP / 2.0f, sbToP - (speedBoostTemp / 2.0f));
	speedBoostTemp -= max(sbToP, 0.0f);
	// Passenger -> Fuel
	float pToF = _tree->GetSpeedboost()->getPosition().Distance(_tree->GetPassenger()->getPosition());
	distance += max(pToF / 2.0f, pToF - (speedBoostTemp / 2.0f));

	std::stringstream out;
	out << "->SB->P->F " << distance << endl;
	OutputDebugStringA(out.str().c_str());

	if (distance <= _tree->GetVehicle()->GetFuelDistance())
		_state = NodeState::SUCCESS;
	else
		_state = NodeState::FAILURE;

	return _state;
}

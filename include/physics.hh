#ifndef PHYSICS_HH
#define PHYSINCS_HH

#include "G4VModularPhysicsList.hh"
#include "G4EmStandardPhysics.hh"
#include "G4OpticalPhysics.hh"
#include "G4DecayPhysics.hh"
#include "G4RadioactiveDecayPhysics.hh"

class MyPhysicsList : public G4VModularPhysicsList {
public:
	MyPhysicsList();
	~MyPhysicsList();
};

#endif

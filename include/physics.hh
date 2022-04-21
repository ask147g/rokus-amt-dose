#ifndef physics_h
#define physics_h 1

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

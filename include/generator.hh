#ifndef generator_h
#define generator_h 1

#include <rapidxml/rapidxml.hpp>

#include "G4VUserPrimaryGeneratorAction.hh"
#include "G4ParticleTable.hh"
#include "G4IonTable.hh"
#include "G4Geantino.hh"
#include "G4SingleParticleSource.hh"

class MyPrimaryGenerator : public G4VUserPrimaryGeneratorAction {
public:
	MyPrimaryGenerator();
	~MyPrimaryGenerator();
	
	virtual void GeneratePrimaries(G4Event*);
private:
	void ReadSize();
	G4SingleParticleSource *sourceCo;
	G4double biasingHead;
	G4double source_active_diameter = 0 * CLHEP::mm;
	G4double source_active_height = 0 * CLHEP::mm;
};

#endif

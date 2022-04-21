#ifndef generator_h
#define generator_h 1

#include "G4VUserPrimaryGeneratorAction.hh"
#include "G4ParticleTable.hh"
#include "G4IonTable.hh"
#include "G4Geantino.hh"
#include "G4SingleParticleSource.hh"

class MyPrimaryGenerator : public G4VUserPrimaryGeneratorAction {
public:
	MyPrimaryGenerator(G4double bias);
	~MyPrimaryGenerator();
	
	virtual void GeneratePrimaries(G4Event*);
private:
	G4SingleParticleSource *sourceCo;
	G4double biasingHead;
};

#endif

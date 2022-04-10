#ifndef ACTION_HH
#define ACTION_HH

#include "G4VUserActionInitialization.hh"
#include "G4Run.hh"

#include "generator.hh"
#include "run.hh"
#include "construction.hh"

class MyActionInitialization : public G4VUserActionInitialization {
public:
	MyActionInitialization(G4double bias, SimpleRunAction *arun);
	~MyActionInitialization();
	
	virtual void Build() const;
	virtual void BuildForMaster() const;

private:
	G4double biasingHead;
	SimpleRunAction *run;
};

#endif

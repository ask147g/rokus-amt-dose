#ifndef action_h
#define action_h 1

#include "G4VUserActionInitialization.hh"
#include "G4Run.hh"

#include "generator.hh"
#include "run.hh"
#include "construction.hh"

class MyActionInitialization : public G4VUserActionInitialization {
public:
	MyActionInitialization(SimpleRunAction *arun);
	~MyActionInitialization();
	
	virtual void Build() const;
	virtual void BuildForMaster() const;

private:
	G4double biasingHead;
	SimpleRunAction *run;
};

#endif

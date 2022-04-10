#include "action.hh"

MyActionInitialization::MyActionInitialization(G4double bias, SimpleRunAction *arun) : biasingHead(bias), run(arun) {}

MyActionInitialization::~MyActionInitialization() {}

void MyActionInitialization::BuildForMaster() const {
	SetUserAction(run);
}

void MyActionInitialization::Build() const {
	MyPrimaryGenerator *generator = new MyPrimaryGenerator(biasingHead);
	SetUserAction(generator);
	SetUserAction(new SimpleRunAction);
}

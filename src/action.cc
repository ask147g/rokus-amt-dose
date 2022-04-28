#include "action.hh"

MyActionInitialization::MyActionInitialization(SimpleRunAction *arun) : run(arun) {}

MyActionInitialization::~MyActionInitialization() {}

void MyActionInitialization::BuildForMaster() const {
	SetUserAction(run);
}

void MyActionInitialization::Build() const {
	MyPrimaryGenerator *generator = new MyPrimaryGenerator();
	SetUserAction(generator);
	SetUserAction(new SimpleRunAction);
}

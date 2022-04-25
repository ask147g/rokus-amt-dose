#ifndef physics_h
#define physics_h 1

#include "G4VUserPhysicsList.hh"

#include "G4ParticleTypes.hh"
#include "G4IonConstructor.hh"
#include "G4PhysicsListHelper.hh"
#include "G4RadioactiveDecay.hh"
#include "G4UAtomicDeexcitation.hh"
#include "G4LossTableManager.hh"
#include "G4PhotoElectricEffect.hh"
#include "G4ComptonScattering.hh"
#include "G4ionIonisation.hh"
#include "G4CoulombScattering.hh"
#include "G4GammaConversion.hh"
#include "G4eBremsstrahlung.hh"
#include "G4eMultipleScattering.hh"
#include "G4LivermoreIonisationModel.hh"
#include "G4UniversalFluctuation.hh"
#include "G4eIonisation.hh"

#include "G4GoudsmitSaundersonMscModel.hh"
#include "G4WentzelVIModel.hh"
#include "G4eCoulombScatteringModel.hh"
#include "G4SeltzerBergerModel.hh"
#include "G4Generator2BS.hh"
#include "G4ePairProduction.hh"
#include "G4PEEffectFluoModel.hh"
#include "G4KleinNishinaCompton.hh"
#include"G4BetheHeitlerModel.hh"

#include "G4PenelopePhotoElectricModel.hh"
#include "G4PenelopeComptonModel.hh"
#include "G4PenelopeGammaConversionModel.hh"
#include "G4PenelopeIonisationModel.hh"
#include "G4PenelopeBremsstrahlungModel.hh"

#include "G4LivermoreComptonModel.hh"

class MyPhysicsList : public G4VUserPhysicsList {
public:
	MyPhysicsList();
	~MyPhysicsList();
protected:
	virtual void ConstructParticle();
	virtual void ConstructProcess();
	virtual void SetCuts();
};

#endif

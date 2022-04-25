#include "physics.hh"

MyPhysicsList::MyPhysicsList() : G4VUserPhysicsList() {}

MyPhysicsList::~MyPhysicsList() {}

void MyPhysicsList::ConstructParticle() {
	// pseudo-particles
  G4Geantino::GeantinoDefinition();
  
  // gamma
  G4Gamma::GammaDefinition();

  // leptons
  G4Electron::ElectronDefinition();
  G4Positron::PositronDefinition();

  G4NeutrinoE::NeutrinoEDefinition();
  G4AntiNeutrinoE::AntiNeutrinoEDefinition();
  
  // baryons
  G4Proton::ProtonDefinition();
  G4Neutron::NeutronDefinition();  

  // ions
  G4IonConstructor iConstructor;
  iConstructor.ConstructParticle();  
}

void MyPhysicsList::ConstructProcess() {
	AddTransportation();
	
	G4PhysicsListHelper* ph = G4PhysicsListHelper::GetPhysicsListHelper();  

	G4RadioactiveDecay* radioactiveDecay = new G4RadioactiveDecay();	
	radioactiveDecay->SetICM(true);                //Internal Conversion
	radioactiveDecay->SetARM(true);               //Atomic Rearangement
	ph->RegisterProcess(radioactiveDecay, G4GenericIon::GenericIon());
	
	// Deexcitation (in case of Atomic Rearangement)
	G4UAtomicDeexcitation* de = new G4UAtomicDeexcitation();
	de->SetFluo(true);
	de->SetAuger(true);   
	de->SetPIXE(true);  
	G4LossTableManager::Instance()->SetAtomDeexcitation(de);

	// gamma	
	G4PhotoElectricEffect* photoeff = new G4PhotoElectricEffect();
	photoeff->SetEmModel(new G4PenelopePhotoElectricModel());
	ph->RegisterProcess(photoeff, G4Gamma::Gamma());
	G4ComptonScattering* compton = new G4ComptonScattering();
	compton->SetEmModel(new G4PenelopeComptonModel());
	//ph->RegisterProcess(compton, G4Gamma::Gamma());
	G4GammaConversion *gam_conv = new G4GammaConversion();
	gam_conv->SetEmModel(new G4PenelopeGammaConversionModel());
	ph->RegisterProcess(gam_conv, G4Gamma::Gamma());

	// electron
	G4CoulombScattering* ss = new G4CoulombScattering();
	ss->SetEmModel(new G4eCoulombScatteringModel());
	ph->RegisterProcess(ss, G4Electron::Electron());

	G4eIonisation* eioni = new G4eIonisation();
	eioni->SetEmModel(new G4PenelopeIonisationModel());
	ph->RegisterProcess(eioni, G4Electron::Electron());

	G4eBremsstrahlung* brem = new G4eBremsstrahlung();
	brem->SetEmModel(new G4PenelopeBremsstrahlungModel());
	ph->RegisterProcess(brem, G4Electron::Electron());

	G4ePairProduction* ee = new G4ePairProduction();
	ph->RegisterProcess(ee, G4Electron::Electron());

	G4eMultipleScattering* msc = new G4eMultipleScattering();
	msc->SetEmModel(new G4GoudsmitSaundersonMscModel);
	ph->RegisterProcess(msc, G4Electron::Electron());

}

void MyPhysicsList::SetCuts() {
	SetParticleCuts(0.1*CLHEP::mm, "gamma");
	SetParticleCuts(0.01*CLHEP::mm, "e-");
	SetParticleCuts(0.1*CLHEP::mm, "e+");
	SetParticleCuts(0.1*CLHEP::mm, "proton");
}
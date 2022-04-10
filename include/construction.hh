#ifndef CONSTRUCTION.HH
#define CONSTRUCTION.HH

#include <utility>

#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4Orb.hh"
#include "G4Cons.hh"
#include "G4Sphere.hh"
#include "G4SystemOfUnits.hh"
#include "G4VUserDetectorConstruction.hh"
#include "G4VPhysicalVolume.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4NistManager.hh"
#include "G4SDParticleFilter.hh"
#include "G4SubtractionSolid.hh"
#include "G4VSensitiveDetector.hh"
#include "G4SDManager.hh"

#include "detector.hh"
#include "detectorRadDecay.hh"

class MyDetectorConstruction : public G4VUserDetectorConstruction {
public:
	MyDetectorConstruction(SimpleRunAction *arun);
	~MyDetectorConstruction();

	virtual G4VPhysicalVolume *Construct();

	G4double GetBiasingHead() {return biasingHead;}
private:
	G4SDManager* SDman = G4SDManager::GetSDMpointer();
	G4double biasingHead = 500*cm;
	SimpleRunAction *run;
	
	std::pair<G4LogicalVolume*, G4VPhysicalVolume*> BuildWorld();
	void BuildRadioaciveHead(G4LogicalVolume *logicWorld, G4SDManager *SDman, G4bool isDet);
	void BuildWaterFantom(G4LogicalVolume *logicWorld, G4SDManager *SDman);
	virtual void ConstructSDandField();
};

#endif
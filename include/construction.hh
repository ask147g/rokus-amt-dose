#ifndef construction_h
#define construction_h 1

#include <utility>
#include <map>

#include <rapidxml/rapidxml.hpp>

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
	SimpleRunAction *run;
	
	std::pair<G4LogicalVolume*, G4VPhysicalVolume*> BuildWorld();
	void BuildRadioaciveHead(G4LogicalVolume *logicWorld);
	void BuildWaterFantom(G4LogicalVolume *logicWorld);
	void BuildContainer(G4LogicalVolume *logicWorld);

	void ReadSizes();
	void ReadMaterials();
	virtual void ConstructSDandField();
	// world
	G4double world_sizeA;
	G4double world_sizeB;
	G4double world_sizeC;
	// radioactive head
	G4double biasingHead = 0*cm;
	G4bool radioactiveheadPlaced;
	G4bool sourceSens;
		// frame
		G4double Rsphere = 0 * cm;

		// source
		G4double source_active_diameter = 0 * mm;
		G4double source_active_height = 0 * mm;
		G4double source_diameter = 0 * mm;
		G4double source_height = 0 * mm;

	// container
	G4bool containerPlaced;
	G4double thikness_container = 0 * mm;
	G4double edge_container = 0 * mm;
	G4double placement_container = 0 * mm;

	// fantom
	G4bool fantomPlaced;
	G4bool fantomSensitive;
	G4double fantom_sizeA;
	G4double fantom_sizeB;
	G4double fantom_sizeC;

};

#endif
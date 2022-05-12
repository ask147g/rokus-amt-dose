#ifndef construction_h
#define construction_h 1

#include <utility>
#include <map>
#include <fstream>

#include <rapidxml/rapidxml.hpp>

#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4Orb.hh"
#include "G4Cons.hh"
#include "G4Sphere.hh"
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
#include "detectorPlane.hh"
#include "typeCalc.hh"

class MyDetectorConstruction : public G4VUserDetectorConstruction {
public:
	MyDetectorConstruction(SimpleRunAction *arun);
	~MyDetectorConstruction();

	virtual G4VPhysicalVolume *Construct();

private:
	G4SDManager* SDman = G4SDManager::GetSDMpointer();
	SimpleRunAction *run;
	
	std::pair<G4LogicalVolume*, G4VPhysicalVolume*> BuildWorld();
	void BuildRadioaciveHead(G4LogicalVolume *logicWorld);
	void BuildFantom(G4LogicalVolume *logicWorld, G4double x, G4double y, G4double z, G4int copy, G4RotationMatrix* rotation);
	void BuildContainer(G4LogicalVolume *logicWorld, G4double x, G4double y, G4double z, G4int copy, G4RotationMatrix* rotation);
	void BuildDiaphragm(G4LogicalVolume *logicWorld);
	void BuildMaterials();

	void BuildWalls(G4LogicalVolume* logicWorld);

	void BuildFantomPlane(G4LogicalVolume *logicWorld);
	void BuildContainerPlane(G4LogicalVolume *logicWorld);
	void ReCalculatePlaneBiasing();

	void ReadCloset();
	void BuildFantomCloset(G4LogicalVolume *logicWorld);
	void BuildContainerCloset(G4LogicalVolume *logicWorld);

	void ReadSizes();
	void ReadMaterials();
	G4double SetDistance();
	virtual void ConstructSDandField();
	// SIZES
	// world
	G4double world_sizeA;
	G4double world_sizeB;
	G4double world_sizeC;
	// radioactive head
	G4double biasingHead = 0*CLHEP::cm;
	G4bool radioactiveheadPlaced;
	G4bool sourceSens;
		// frame
		G4double Rsphere = 0 * CLHEP::cm;
		G4double biasEdge_frame;
		G4double biasing_frame;
		G4double edge_frame;

		// source
		G4bool isCap1;
		G4bool isCap2;
		G4double source_active_diameter = 0 * CLHEP::mm;
		G4double source_active_height = 0 * CLHEP::mm;
		G4double source_diameter = 0 * CLHEP::mm;
		G4double source_height = 0 * CLHEP::mm;

	// container
	G4bool containerPlaced;
	G4double thikness_container = 0 * CLHEP::mm;
	G4double edge_container = 0 * CLHEP::mm;
	G4double placement_container = 0 * CLHEP::mm;

	// fantom
	G4bool fantomPlaced;
	G4bool fantomSensitive;
	G4double fantom_sizeA;
	G4double fantom_sizeB;
	G4double fantom_sizeC;

	// diaphragm
	G4bool diaphragmPlaced;

	// plane
	G4bool planeFantomPlaced;
	G4bool planeContainerPlaced;
	G4double planeSize;
	G4double planeBiasing;
	G4double planeStep;

	// closet
	G4int closetAmount = 0;
	G4double closetBiasing;
	std::vector<G4bool> closetTable;
	G4double dxBiasing = 0;
	G4double dyBiasing = 0;
	G4double angleBiasing = 0;
	G4RotationMatrix *closetRot;

	// walls
	G4bool isWalls = true;

	// general
	G4double biasRocus;
	G4double height_cap;
	G4double diameter_half;
	G4int amountPlane;
	G4RotationMatrix* rotAll;
	// MATERIALS
	// uran
	G4Material *Uran;
	G4double ro_uran;
	std::vector<G4int> Z_uran;
	std::vector<G4int> A_uran;
	std::vector<G4double> mass_uran;
	std::vector<G4double> perCent_uran;

	// steel
	// steel12
	G4Material *steel12;
	G4double ro_steel12;
	std::map<std::string, G4double> mat_steel12;
	// steel02
	G4Material *steel02;
	G4double ro_steel02;
	std::map<std::string, G4double> mat_steel02;

};

#endif
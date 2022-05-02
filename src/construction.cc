#include "construction.hh"

MyDetectorConstruction::MyDetectorConstruction(SimpleRunAction *arun): G4VUserDetectorConstruction(), run(arun) {
	ReadSizes();
	ReadMaterials();
	if (TypeCalculations::GetTypeCalc() == 2) placement_container = SetDistance();
	std::cout << placement_container << std::endl;
	if (TypeCalculations::GetTypeCalc() == 5) {
		planeBiasing = SetDistance();
		ReCalculatePlaneBiasing();
	}
	BuildMaterials();

	// general
	diameter_half = (source_diameter - source_active_diameter)/4.;
	height_cap = (source_height-source_active_height-2*diameter_half)/4.;
	biasRocus = -biasingHead+Rsphere/2.+edge_container/2.+placement_container;
	amountPlane = ceil(planeSize/planeStep);
	if (amountPlane % 2 == 0) ++amountPlane;

	if (TypeCalculations::GetTypeCalc() >= 3 && TypeCalculations::GetTypeCalc() <= 5)
		run->SetParametersPlane(amountPlane, planeStep);
}


MyDetectorConstruction::~MyDetectorConstruction() {}


G4VPhysicalVolume *MyDetectorConstruction::Construct() {
	std::pair<G4LogicalVolume*, G4VPhysicalVolume*> world;
	world = BuildWorld();
	
	if (radioactiveheadPlaced)
		BuildRadioaciveHead(world.first);
	if (diaphragmPlaced)
		BuildDiaphragm(world.first);

	// only 1 container
	if ((TypeCalculations::GetTypeCalc()) >= 0 && TypeCalculations::GetTypeCalc() <= 2) {
		if (containerPlaced)
			BuildContainer(world.first, 0, 0, -biasRocus-placement_container, 0);
		if (fantomPlaced)
			BuildFantom(world.first, 0, 0, -biasRocus-placement_container, 0);
	}
	
	// plane
	if ((TypeCalculations::GetTypeCalc()) >= 3 && TypeCalculations::GetTypeCalc() <= 5) {
		if (planeContainerPlaced)
			BuildContainerPlane(world.first); 
		if (planeFantomPlaced)
			BuildFantomPlane(world.first);
	}

	// closet
	if ((TypeCalculations::GetTypeCalc()) >= 6 && TypeCalculations::GetTypeCalc() <= 8) {
		if (planeContainerPlaced)
			BuildContainerPlane(world.first);
		if (planeFantomPlaced)
			BuildFantomPlane(world.first);
	}

	return world.second;
}


void MyDetectorConstruction::ReCalculatePlaneBiasing() {
	planeSize = 50;
	if (planeBiasing > 100*CLHEP::cm) planeSize = 100;
	if (planeBiasing > 150*CLHEP::cm) planeSize = 150;
	if (planeBiasing > 200*CLHEP::cm) planeSize = 200;
	if (planeBiasing > 450*CLHEP::cm) planeSize = 250;
	if (planeBiasing > 650*CLHEP::cm) planeSize = 300;
	if (planeBiasing > 750*CLHEP::cm) planeSize = 350;
	if (planeBiasing > 850*CLHEP::cm) planeSize = 400;
	planeSize *= CLHEP::cm;
}

void MyDetectorConstruction::ReadSizes() {
	rapidxml::xml_document<> doc;
	rapidxml::xml_node<> *root_node;

    std::ifstream theFile ("data/constructData.xml");
    std::vector<char> buffer((std::istreambuf_iterator<char>(theFile)), std::istreambuf_iterator<char>());
    buffer.push_back('\0');
    doc.parse<0>(&buffer[0]);
	
	std::string string;
	// world
	root_node = doc.first_node("world");
	world_sizeA = std::stof(root_node->first_node("sizeA")->value()); world_sizeA *= CLHEP::m;
	world_sizeB = std::stof(root_node->first_node("sizeB")->value()); world_sizeB *= CLHEP::m;
	world_sizeC = std::stof(root_node->first_node("sizeC")->value()); world_sizeC *= CLHEP::m;
	
	// radioactive head
    root_node = doc.first_node("radioactivehead");
	string = root_node->first_attribute("placed")->value();
	std::istringstream(string) >> std::boolalpha >> radioactiveheadPlaced;
	biasingHead = std::stof(root_node->first_node("biasingHead")->value()); biasingHead *= CLHEP::cm;
		// frame
		Rsphere = std::stof(root_node->first_node("frame")->first_node("sphere")->value()); Rsphere *= CLHEP::mm;
		edge_frame = std::stof(root_node->first_node("frame")->first_node("cons")->first_node("edge")->value()); edge_frame *= CLHEP::cm;
		biasEdge_frame = std::stof(root_node->first_node("frame")->first_node("cons")->first_node("biasEdge")->value()); biasEdge_frame *= CLHEP::cm;
		biasing_frame = std::stof(root_node->first_node("frame")->first_node("cons")->first_node("biasing")->value()); biasing_frame *= CLHEP::mm;

		// source
		string = root_node->first_node("source")->first_attribute("sensitive")->value();
		std::istringstream(string) >> std::boolalpha >> sourceSens;
		string = root_node->first_node("source")->first_attribute("cap1")->value();
		std::istringstream(string) >> std::boolalpha >> isCap1;
		string = root_node->first_node("source")->first_attribute("cap2")->value();
		std::istringstream(string) >> std::boolalpha >> isCap2;
		source_active_diameter = std::stof(root_node->first_node("source")->first_node("source_active_diameter")->value()); source_active_diameter *= CLHEP::mm;
		source_active_height = std::stof(root_node->first_node("source")->first_node("source_active_height")->value()); source_active_height *= CLHEP::mm;
		source_diameter = std::stof(root_node->first_node("source")->first_node("source_diameter")->value()); source_diameter *= CLHEP::mm;
		source_height = std::stof(root_node->first_node("source")->first_node("source_height")->value()); source_height *= CLHEP::mm;

	// container
	root_node = doc.first_node("container");
	string = root_node->first_attribute("placed")->value();
	std::istringstream(string) >> std::boolalpha >> containerPlaced;
	thikness_container = std::stof(root_node->first_node("thikness")->value()); thikness_container *= CLHEP::mm;	
	edge_container = std::stof(root_node->first_node("edge")->value()); edge_container *= CLHEP::cm;
	placement_container = std::stof(root_node->first_node("placement")->value()); placement_container *= CLHEP::cm;

	// fantom
	root_node = doc.first_node("fantom");
	string = root_node->first_attribute("placed")->value();
	std::istringstream(string) >> std::boolalpha >> fantomPlaced;
	string = root_node->first_attribute("sensitive")->value();
	std::istringstream(string) >> std::boolalpha >> fantomSensitive;
	fantom_sizeA = std::stof(root_node->first_node("sizeA")->value()); fantom_sizeA *= CLHEP::cm;
	fantom_sizeB = std::stof(root_node->first_node("sizeB")->value()); fantom_sizeB *= CLHEP::cm;
	fantom_sizeC = std::stof(root_node->first_node("sizeC")->value()); fantom_sizeC *= CLHEP::cm;
	
	// plane
	root_node = doc.first_node("plane");
	string = root_node->first_attribute("fantomPlaced")->value();
	std::istringstream(string) >> std::boolalpha >> planeFantomPlaced;
	string = root_node->first_attribute("containerPlaced")->value();
	std::istringstream(string) >> std::boolalpha >> planeContainerPlaced;
	planeSize = std::stof(root_node->first_node("planeSize")->value()); planeSize *= CLHEP::cm;
	planeBiasing = std::stof(root_node->first_node("planeBiasing")->value()); planeBiasing *= CLHEP::cm;
	planeStep = std::stof(root_node->first_node("step")->value()); planeStep *= CLHEP::cm;
}


void MyDetectorConstruction::ReadMaterials() {
	rapidxml::xml_document<> doc;
	rapidxml::xml_node<> *root_node;

    std::ifstream theFile ("data/constructMaterial.xml");
    std::vector<char> buffer((std::istreambuf_iterator<char>(theFile)), std::istreambuf_iterator<char>());
    buffer.push_back('\0');
    doc.parse<0>(&buffer[0]);

	// Uran
	root_node = doc.first_node("uran");
	ro_uran = std::stof(root_node->first_attribute("ro")->value()); ro_uran *= CLHEP::g/CLHEP::cm3;
	for (rapidxml::xml_node<> *root = root_node->first_node("material"); root; root = root->next_sibling()) {
		Z_uran.push_back(std::stof(root->first_node("Z")->value()));
		A_uran.push_back(std::stof(root->first_node("A")->value()));
		mass_uran.push_back(std::stof(root->first_node("mass")->value()));
		perCent_uran.push_back(std::stof(root->first_node("perCent")->value()));
	}

	// Steel 12
	root_node = doc.first_node("steel12");
	ro_steel12 = std::stof(root_node->first_attribute("ro")->value()); ro_steel12 *= CLHEP::g/CLHEP::cm3;
	for (rapidxml::xml_node<> *root = root_node->first_node("material"); root; root = root->next_sibling()) {
		mat_steel12.insert({root->first_node("name")->value(), std::stof(root->first_node("perCent")->value())});
	}
	std::map<std::string, G4double>::iterator it = mat_steel12.begin();
	G4double purities = 100;
	while (it != mat_steel12.end()) {
		purities -= (*it).second;
		++it;
	}
	mat_steel12.insert({"Fe", purities});
	
	// Steel 02
	root_node = doc.first_node("steel02");
	ro_steel02 = std::stof(root_node->first_attribute("ro")->value()); ro_steel02 *= CLHEP::g/CLHEP::cm3;
	for (rapidxml::xml_node<> *root = root_node->first_node("material"); root; root = root->next_sibling()) {
		mat_steel02.insert({root->first_node("name")->value(), std::stof(root->first_node("perCent")->value())});
	}
	it = mat_steel02.begin();
	purities = 100;
	while (it != mat_steel02.end()) {
		purities -= (*it).second;
		++it;
	}
	mat_steel02.insert({"Fe", purities});
}


void MyDetectorConstruction::BuildMaterials() {
	G4NistManager *nist = G4NistManager::Instance();
	// Uran
	Uran = new G4Material("Uran", ro_uran, 1);
	G4Element* elU = new G4Element("Uranium", "U", Z_uran.size());
	for (int i = 0; i < Z_uran.size(); i++) {
		std::string name = "U" + std::to_string(A_uran[i]);
		elU->AddIsotope(new G4Isotope(name, Z_uran[i], A_uran[i], mass_uran[i]*CLHEP::g/CLHEP::mole), perCent_uran[i]*CLHEP::perCent);
	}
	Uran->AddElement(elU, 100.*CLHEP::perCent);

	// steels
	steel12 = new G4Material("steel12", ro_steel12, mat_steel12.size());
	std::map<std::string, G4double>::iterator it = mat_steel12.begin();
	while (it != mat_steel12.end()) {
		steel12->AddElement(nist->FindOrBuildElement(it->first), (it->second)*CLHEP::perCent);
		++it;
	}
	
	steel02 = new G4Material("steel02", ro_steel02, mat_steel02.size());
	it = mat_steel02.begin();
	while (it != mat_steel02.end()) {
		steel02->AddElement(nist->FindOrBuildElement(it->first), (it->second)*CLHEP::perCent);
		++it;
	}
}


std::pair<G4LogicalVolume*, G4VPhysicalVolume*> MyDetectorConstruction::BuildWorld() {
	G4NistManager *nist = G4NistManager::Instance();
	G4Material *worldMat = nist->FindOrBuildMaterial("G4_AIR");

	G4Box *solidWorld = new G4Box("solidWorld", world_sizeA, world_sizeB, world_sizeC);
	
	G4LogicalVolume *logicWorld = new G4LogicalVolume(solidWorld, worldMat, "logicWorld");
	
	G4VPhysicalVolume *physWorld = new G4PVPlacement(0,
								G4ThreeVector(0, 0, 0),
								logicWorld,
								"physWorld",
								0,
								false,
								0,
								true);

	return std::make_pair(logicWorld, physWorld);
}


void MyDetectorConstruction::BuildRadioaciveHead(G4LogicalVolume *logicWorld) {
	G4NistManager *nist = G4NistManager::Instance();
	G4Material *galacyMat = nist->FindOrBuildMaterial("G4_Galactic");
	G4Material *CoMat = nist->FindOrBuildMaterial("G4_Co");

	G4VSolid *innerTubSolid = new G4Tubs("innerTubSolid",
									0,
									(source_active_diameter+2*diameter_half)/2.,
									(source_active_height+diameter_half+2*height_cap)/2.,
									0,
									360);

	G4VSolid *outerTubSolid = new G4Tubs("outerTubSolid",
									0,
									(source_active_diameter+4*diameter_half)/2.,
									(source_active_height+2*diameter_half+4*height_cap)/2.,
									0,
									360);

	// source
	G4Tubs *sourceSolid = new G4Tubs("sourceSolid",
									0,
									source_active_diameter/2.,
									source_active_height/2.,
									0,
									360);
	
	outerTubSolid = new G4SubtractionSolid("outerTubSolid", outerTubSolid, innerTubSolid, 0, G4ThreeVector(0,0,(source_active_height+2*height_cap+diameter_half)/2.-source_active_height/2.-diameter_half));
	innerTubSolid = new G4SubtractionSolid("innerTubSolid", innerTubSolid, sourceSolid, 0, G4ThreeVector(0,0,(source_active_height+2*height_cap+diameter_half)/2.-source_active_height/2.-diameter_half));
	
	G4VSolid *deleteTubSolid = new G4Tubs("deleteTubSolid",
									0,
									source_active_diameter/2.,
									source_active_height/2.,
									0,
									360);

	innerTubSolid = new G4SubtractionSolid("innerTubSolid", innerTubSolid, deleteTubSolid, 0, G4ThreeVector(0,0,-4*height_cap));
	
	deleteTubSolid = new G4Tubs("deleteTubSolid",
									0,
									(source_active_diameter+2*diameter_half)/2.,
									(source_active_height+diameter_half+2*height_cap)/2.,
									0,
									360);

	outerTubSolid = new G4SubtractionSolid("outerTubSolid", outerTubSolid, deleteTubSolid, 0, G4ThreeVector(0,0,-4*height_cap));

	G4LogicalVolume *innerTubLogic = new G4LogicalVolume(innerTubSolid, steel12, "innerTubLogic");
	G4LogicalVolume *outerTubLogic = new G4LogicalVolume(outerTubSolid, steel02, "outerTubLogic");

	new G4PVPlacement(0,
					G4ThreeVector(0, 0, biasingHead+source_active_height/2.-((source_active_height+2*height_cap+diameter_half)/2.-source_active_height/2.-diameter_half)),
					innerTubLogic,
					"innerTubPhys",
					logicWorld,
					true, 	
					0, 
					false);

	new G4PVPlacement(0,
					G4ThreeVector(0, 0, biasingHead+source_active_height/2.-((source_active_height+4*height_cap+2*diameter_half)/2.-source_active_height/2.-2*diameter_half)),
					outerTubLogic,
					"outerTubPhys",
					logicWorld,
					true, 	
					0, 
					false);

	G4LogicalVolume *sourceLogic = new G4LogicalVolume(sourceSolid, CoMat, "sourceLogic");
	
	new G4PVPlacement(0,
					G4ThreeVector(0, 0, biasingHead+source_active_height/2.),
					sourceLogic,
					"sourcePhys",
					logicWorld,
					true, 	
					0, 
					false);

	if (isCap1) {
		G4Tubs *cap1Solid = new G4Tubs("cap1Solid",
									0,
									source_active_diameter/2.,
									height_cap/2.,
									0,
									360);

		G4LogicalVolume *cap1Logic = new G4LogicalVolume(cap1Solid, steel12, "cap1Logic");

		new G4PVPlacement(0,
					G4ThreeVector(0, 0, biasingHead-height_cap/2.),
					cap1Logic,
					"cap1Phys",
					logicWorld,
					true, 	
					0, 
					false);
	}

	if (isCap2) {
		G4Tubs *cap2Solid = new G4Tubs("cap2Solid",
									0,
									(source_active_diameter+2*diameter_half)/2.,
									height_cap/2.,
									0,
									360);

		G4LogicalVolume *cap2Logic = new G4LogicalVolume(cap2Solid, steel12, "cap2Logic");

		new G4PVPlacement(0,
					G4ThreeVector(0, 0, biasingHead-5*height_cap/2.),
					cap2Logic,
					"cap2Phys",
					logicWorld,
					true, 	
					0, 
					false);
	}

	// head
	G4VSolid *headSolid = new G4Orb("headSolid", Rsphere/2.);

	G4VSolid *headSolidMinus = new G4Cons("headSolidMinus",
											0,
											edge_frame/2.,
											0,
											source_active_diameter/2.+2*diameter_half,
											biasing_frame/2.+biasEdge_frame/2.-4*height_cap,
											0,
											360);

	G4VSolid *headSolidMinus2 = new G4Tubs("headSolidMinus2",
									0,
									(source_active_diameter+4*diameter_half)/2.,
									(source_active_height+2*diameter_half+4*height_cap)/2.,
									0,
									360);
	
	headSolid = new G4SubtractionSolid("headSolid", headSolid, headSolidMinus, 0, G4ThreeVector(0,0,-(biasing_frame/2.*CLHEP::mm+biasEdge_frame/2.)));
	headSolid = new G4SubtractionSolid("headSolid", headSolid, headSolidMinus2, 0, G4ThreeVector(0,0,source_active_height/2.-2*height_cap));

	G4LogicalVolume *headLogic = new G4LogicalVolume(headSolid, Uran, "headLogic");

	new G4PVPlacement(0,
					G4ThreeVector(0, 0, biasingHead), 
					headLogic, 
					"headPhys",
					logicWorld, 
					true,
					0, 
					false);
}


void MyDetectorConstruction::BuildContainer(G4LogicalVolume *logicWorld, G4double x, G4double y, G4double z, G4int copy) {
	G4NistManager *nist = G4NistManager::Instance();
	G4Material *PbMat = nist->FindOrBuildMaterial("G4_Pb");
	G4Material *AlMat = nist->FindOrBuildMaterial("G4_Al");

	G4Box *fantomWorld = new G4Box("fantomWorld",
					(edge_container-4*thikness_container)/2.,
					(edge_container-4*thikness_container)/2.,
					(edge_container-4*thikness_container)/2.);

	G4VSolid *innerWorld = new G4Box("innerWorld",
					(edge_container-2*thikness_container)/2.,
					(edge_container-2*thikness_container)/2.,
					(edge_container-2*thikness_container)/2.);
	
	G4VSolid *outerWorld = new G4Box("outerWorld",
									edge_container/2.,
									edge_container/2.,
									edge_container/2.);

	outerWorld = new G4SubtractionSolid("outerWorld", outerWorld, innerWorld, 0, G4ThreeVector(0,0,0));
	innerWorld = new G4SubtractionSolid("innerWorld", innerWorld, fantomWorld, 0, G4ThreeVector(0,0,0));

	G4LogicalVolume *innerLogic = new G4LogicalVolume(innerWorld, AlMat, "innerLogic");

	new G4PVPlacement(0,
					G4ThreeVector(x, y, z), 
					innerLogic, 
					"innerPhys",
					logicWorld, 
					true,
					0, 
					false);

	G4LogicalVolume *outerLogic = new G4LogicalVolume(outerWorld, PbMat, "outerLogic");

	new G4PVPlacement(0,
					G4ThreeVector(x, y, z), 
					outerLogic, 
					"outerPhys",
					logicWorld, 
					true,
					0, 
					false);
}


void MyDetectorConstruction::BuildFantom(G4LogicalVolume *logicWorld, G4double x, G4double y, G4double z, G4int copy) {
	G4NistManager *nist = G4NistManager::Instance();
	G4Material *SiMat = nist->FindOrBuildMaterial("G4_Si");

	G4Box *fantomWorld = new G4Box("fantomWorld",
					(fantom_sizeA/2),
					(fantom_sizeB/2),
					(fantom_sizeC/2));
	
	G4LogicalVolume *fantomLogic = new G4LogicalVolume(fantomWorld, SiMat, "fantomLogic");
	
	new G4PVPlacement(0,
					G4ThreeVector(x, y, z), 
					fantomLogic, 
					"fantomPhys",
					logicWorld, 
					true,
					copy, 
					false);
}


void MyDetectorConstruction::BuildDiaphragm(G4LogicalVolume *logicWorld) {

}


void MyDetectorConstruction::BuildFantomPlane(G4LogicalVolume *logicWorld) {
	int copy = 0;
	for (int x = -amountPlane/2.; x < amountPlane/2.; ++x) {
		for (int y = -amountPlane/2.; y < amountPlane/2.; ++y) {
			BuildFantom(logicWorld, x*planeStep, y*planeStep, -biasRocus-planeBiasing, copy);
			++copy;
		}
	}
}


void MyDetectorConstruction::BuildContainerPlane(G4LogicalVolume *logicWorld) {
	int copy = 0;
	for (int x = -amountPlane/2.; x < amountPlane/2.; ++x) {
		for (int y = -amountPlane/2.; y < amountPlane/2.; ++y) {
			BuildContainer(logicWorld, x*planeStep, y*planeStep, -biasRocus-planeBiasing, copy);
			++copy;
		}
	}
}


// sets sensitive volume for calculations
void MyDetectorConstruction::ConstructSDandField() {
	const std::vector<G4String> part = {"e-", "gamma"};
	G4SDParticleFilter* Filter = new G4SDParticleFilter("Filter",part);
	
	if (TypeCalculations::GetTypeCalc() >= 0 && TypeCalculations::GetTypeCalc() <= 2) {
		if (fantomPlaced) {
			G4VSensitiveDetector* sd = new MySensitiveDetector("detectorField", run);
			SetSensitiveDetector("fantomLogic", sd, true);
			sd->SetFilter(Filter);
			SDman->AddNewDetector(sd);
		}

		if (fantomPlaced && fantomSensitive) {
			G4VSensitiveDetector* sdRadDecay = new DetectorRadDecay("fantomRadDecay");
			SetSensitiveDetector("fantomLogic", sdRadDecay, true);
			sdRadDecay->SetFilter(Filter);
			SDman->AddNewDetector(sdRadDecay);
		}

		if (radioactiveheadPlaced && sourceSens) {
			G4VSensitiveDetector* sdRadDecay2 = new DetectorRadDecay("sourceRadDecay");
			SetSensitiveDetector("sourceLogic", sdRadDecay2, true);
			sdRadDecay2->SetFilter(Filter);
			SDman->AddNewDetector(sdRadDecay2);
		}
	}

	if (TypeCalculations::GetTypeCalc() >= 3 && TypeCalculations::GetTypeCalc() <= 5) {
		if (fantomPlaced) {
			G4VSensitiveDetector* sd = new PlaneDetector("planeDet", run);
			SetSensitiveDetector("fantomLogic", sd, true);
			sd->SetFilter(Filter);
			SDman->AddNewDetector(sd);
		}
	}
}


// gets distance from diaphragm to container
G4double MyDetectorConstruction::SetDistance() {
	std::ifstream dist;
	dist.open("scripts/distance.txt", std::ios::in);
	G4double aDist;
	if (dist) {
		while (1) {
			if (dist.eof()) break;
			dist >> aDist;
		}
		aDist *= CLHEP::cm;
	}
	return aDist;
}

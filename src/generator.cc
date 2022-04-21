#include "generator.hh"

MyPrimaryGenerator::MyPrimaryGenerator() {
	ReadSize();
	sourceCo = new G4SingleParticleSource();
	G4SPSEneDistribution* energy = sourceCo->GetEneDist();
	energy->SetEnergyDisType("Mono");
	energy->SetMonoEnergy(0*CLHEP::keV);

	G4SPSPosDistribution* position = sourceCo->GetPosDist();
	position->SetPosDisType("Volume");
	position->SetPosDisShape("Cylinder");
	position->SetHalfZ(source_active_height/2.*CLHEP::mm);
	position->SetRadius(source_active_diameter/2.*CLHEP::mm);
	position->SetCentreCoords(G4ThreeVector(0.0*CLHEP::cm, 0.0*CLHEP::cm, biasingHead+source_active_height/2*CLHEP::mm));
	G4SPSAngDistribution* angular = sourceCo->GetAngDist();
	angular->SetParticleMomentumDirection(G4ThreeVector(0.0,0.0,1.0));
	
	sourceCo->SetNumberOfParticles(1);
}

MyPrimaryGenerator::~MyPrimaryGenerator() {delete sourceCo;}

void MyPrimaryGenerator::ReadSize() {
	rapidxml::xml_document<> doc;
	rapidxml::xml_node<> *root_node;

    std::ifstream theFile ("data/constructData.xml");
    std::vector<char> buffer((std::istreambuf_iterator<char>(theFile)), std::istreambuf_iterator<char>());
    buffer.push_back('\0');
    doc.parse<0>(&buffer[0]);

	// Uran
	root_node = doc.first_node("radioactivehead");
	biasingHead = std::stof(root_node->first_node("biasingHead")->value()); biasingHead *= CLHEP::cm;
	source_active_diameter = std::stof(root_node->first_node("source")->first_node("source_active_diameter")->value()); source_active_diameter *= CLHEP::mm;
	source_active_height = std::stof(root_node->first_node("source")->first_node("source_active_height")->value()); source_active_height *= CLHEP::mm;

}

void MyPrimaryGenerator::GeneratePrimaries(G4Event *anEvent) {
    G4ParticleDefinition *particle = G4ParticleTable::GetParticleTable()->FindParticle("geantino");
	sourceCo->SetParticleDefinition(particle);
    
    if(particle == G4Geantino::Geantino()) {
        G4int Z = 27;
		G4int A = 60;
        
        G4double charge = 0*CLHEP::eplus;
        G4double energy = 0*CLHEP::keV;
        
        G4ParticleDefinition* ion = G4IonTable::GetIonTable()->GetIon(Z,A,energy);
        sourceCo->SetParticleDefinition(ion);
        sourceCo->SetParticleCharge(charge);
    }
    	sourceCo->GeneratePrimaryVertex(anEvent);
}
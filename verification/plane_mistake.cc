#include <iostream>
#include <fstream>
#include <cmath>
#include <map>
#include <vector>
#include <sstream>

void plane_mistake()
{
	gStyle->SetTitleOffset(1.5,"x");
	gStyle->SetTitleOffset(1.5,"y");
	gStyle->SetTitleOffset(1.5,"z");
	gROOT->ForceStyle();

	std::ifstream exp, geant;
	exp.open("plane/planeExp.csv", std::ios::in);
	geant.open("plane/164.csv", std::ios::in);

	std::vector<double> expDose, geantDose;
	std::pair<std::vector<int>, std::vector<int> > expXY, geantXY;

	TGraph2D *output = new TGraph2D();
	if (exp && geant) {
		while (1) {
			double x1,y1,D1, x2,y2,D2;
			if (exp.eof()) break;
			if (geant.eof()) break;
			exp >> x1 >> y1 >> D1;
			geant >> x2 >> y2 >> D2;
			expDose.push_back(D1);
			geantDose.push_back(D2);
			expXY.first.push_back(x1);
			expXY.second.push_back(y1);
			geantXY.first.push_back(x2);
			geantXY.second.push_back(y2);
		}
	}

	std::vector<int>::iterator x1 = expXY.first.begin(), y1 = expXY.second.begin(),
		x2 = geantXY.first.begin(), y2 = geantXY.second.begin();

	std::vector<double>::iterator D1 = expDose.begin(), D2 = geantDose.begin();

	for (x2; x2 != geantXY.first.end(); ++x2) {
		if ((*x2 == *x1) && (*y2 == *y1)) {
			output->SetPoint(output->GetN(), *x2, *y2, abs(*D1-*D2)/(*D1)*100.);
			++x1;
			++y1;
			++D1;
		}
		++y2;
		++D2;
	}

	TCanvas *c1 = new TCanvas("c1","Plane",0,0,600,600);
	c1->cd(1);
	c1->cd(1)->SetGrid();
	output->SetTitle("; x, cm; y, cm; #delta, %");
	output->Draw("colz");

	c1->SaveAs("figure/plane_mistake.png");
}
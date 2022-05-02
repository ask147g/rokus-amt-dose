#include <iostream>
#include <fstream>
#include <cmath>
#include <map>
#include <vector>
#include <sstream>

void plane_distance()
{
	gStyle->SetTitleOffset(1.5,"x");
	gStyle->SetTitleOffset(1.5,"y");
	gStyle->SetTitleOffset(1.5,"z");
	gROOT->ForceStyle();
	std::ifstream input;
	input.open("plane/planeResult.csv", std::ios::in);

	TGraph2D *output = new TGraph2D();
	if (input) {
		while (1) {
			double x,y,D;
			input >> x >> y >> D;
			if (input.eof()) break;
			output->SetPoint(output->GetN(), x, y, D);
		}
	}

	TCanvas *c1 = new TCanvas("c1","Plane",0,0,600,600);
	c1->cd(1);
	c1->cd(1)->SetGrid();
	output->SetTitle("; x, cm; y, cm; D, rad/s");
	output->Draw("surf1");

	TCanvas *c2 = new TCanvas("c2","Plane",0,0,600,600);
	c2->cd(1);
	c2->cd(1)->SetGrid();
	output->SetTitle("; x, cm; y, cm; D, rad/s");
	output->Draw("colz");

	c1->SaveAs("figure/plane3D.png");
	c2->SaveAs("figure/plane2D.png");
}
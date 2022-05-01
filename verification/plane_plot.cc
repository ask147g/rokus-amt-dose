#include <iostream>
#include <fstream>
#include <cmath>
#include <map>
#include <vector>
#include <sstream>

void plane_plot()
{
	std::ifstream gamma_data;
	gamma_data.open("plane/planeResult.csv", std::ios::in);


	TGraph2D *gamma_spectrum = new TGraph2D();
	if (gamma_data) {
		while (1) {
			double x,y,D;
			gamma_data >> x >> y >> D;
			if (gamma_data.eof()) break;
			gamma_spectrum->SetPoint(gamma_spectrum->GetN(), x, y, D);
		}
	}

	TCanvas *c1 = new TCanvas("c1","Spectrum",800,1000);
	c1->cd(1);
	c1->cd(1)->SetGrid();
	gamma_spectrum->GetXaxis()->SetTitle("x");
	gamma_spectrum->GetYaxis()->SetTitle("y");
	gamma_spectrum->Draw("TRI2 Z”");

	c1->SaveAs("plane/fantom_model7.png");
}
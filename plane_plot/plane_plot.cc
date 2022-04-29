#include <iostream>
#include <fstream>
#include <cmath>
#include <map>
#include <vector>
#include <sstream>

void plane_plot()
{
	std::ifstream gamma_data;
	gamma_data.open("planeResult.csv", std::ios::in);


	TH2F *gamma_spectrum = new TH2F("gamma_spectrum","Gamma spectrum",5,0,100, 5, 0, 100);
	if (gamma_data) {
		while (1) {
			double x,y,D;
			gamma_data >> x >> y >> D;
			if (gamma_data.eof()) break;
			gamma_spectrum->Fill(x, y, D);
		}
	}

	TCanvas *c1 = new TCanvas("c1","Spectrum",800,1000);
	c1->cd(1);
	c1->cd(1)->SetGrid();
	gamma_spectrum->Draw("CONTZ");

	c1->SaveAs("fantom_model7.png");
}
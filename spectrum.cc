#include <iostream>
#include <fstream>

void spectrum()
{
	//gROOT->SetStyle("BELLE2");
	std::ifstream gamma_data;
	gamma_data.open("sourceRadDecay_gamma.csv", std::ios::in);

	std::ifstream electron_data;
	electron_data.open("sourceRadDecay_electron.csv", std::ios::in);

	TH1F *gamma_spectrum = new TH1F("gamma_spectrum","Gamma spectrum",500,0,1500);
	gamma_spectrum->SetLineColor(1);
	gamma_spectrum->SetFillColor(1);
	gamma_spectrum->GetXaxis()->SetTitle("E, keV");
	gamma_spectrum->GetYaxis()->SetTitle("N, amount");
	if (gamma_data) {
		while (1) {
			double kine;
			gamma_data >> kine;
			if (gamma_data.eof()) break;
			gamma_spectrum->Fill(kine);
		}
	}
	TH1F *electron_spectrum = new TH1F("electron_spectrum","Electron spectrum",500,0,1500);
	electron_spectrum->SetLineColor(1);
	electron_spectrum->SetFillColor(1);
	electron_spectrum->GetXaxis()->SetTitle("E, keV");
	electron_spectrum->GetYaxis()->SetTitle("N, amount");
	if (electron_data) {
		while (1) {
			double kine;
			electron_data >> kine;
			if (electron_data.eof()) break;
			electron_spectrum->Fill(kine);
		}
	}

	TCanvas *c1 = new TCanvas("c1","Spectrum",800,1000);
	c1->Divide(1,2);
	c1->cd(1);
	c1->cd(1)->SetGrid();
	gamma_spectrum->Draw();
	c1->cd(2);
	c1->cd(2)->SetGrid();
	electron_spectrum->Draw();

	c1->SaveAs("source_model1.png");
}
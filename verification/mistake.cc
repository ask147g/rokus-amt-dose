#include <iostream>
#include <fstream>
#include <cmath>
#include <map>
#include <vector>
#include <sstream>

void mistake()
{
	double exp_data = 0.0132;
	const int amount_models = 1;
	const int first_model = 1;
	const int mult_amount = 10;

	auto c1 = new TCanvas("c1","dose",700,500);
   	c1->SetGrid();
	gStyle->SetOptFit();

	auto leg = new TLegend(0.1,0.15,0.5,0.55);
	auto mg = new TMultiGraph();
	auto mg_err = new TMultiGraph();
	TGraphErrors *gr;
	TGraphErrors *gr_err;

	TGraph* gr1 = new TGraph();
	gr1->SetFillColor(1);
	gr1->SetFillStyle(3002);
   	gr1->SetMarkerStyle(19+1);
   	gr1->SetMarkerSize(1);
   	gr1->SetMarkerColor(1);
   	gr1->SetLineColor(1);
	gr1->SetTitle("experimental");


	std::ifstream model;
		std::map<int, std::vector<float> > input_data;

		gr = new TGraphErrors();
		gr->SetFillColor(4);
		gr->SetFillStyle(3002);
   		gr->SetMarkerStyle(21);
   		gr->SetMarkerSize(1);
   		gr->SetMarkerColor(4);
   		gr->SetLineColor(4);

		gr_err = new TGraphErrors();
		gr_err->SetFillColor(4);
		gr_err->SetFillStyle(3002);
   		gr_err->SetMarkerStyle(22);
   		gr_err->SetMarkerSize(1);
   		gr_err->SetMarkerColor(4);
   		gr_err->SetLineColor(4);

	std::string name = "mistake/630.csv";
	model.open(name, std::ios::in);


	if (model) {
		std::vector<float> mean_dose_dist;
		while (1) {
			std::vector<float> dose_all_dist;
			int distance;
			float mean = 0;
			for (int sko_i = 0; sko_i < mult_amount; sko_i++) {
				int distan;
				float dose;
				model >> dose >> distan;
				mean += dose;
				dose_all_dist.push_back(dose);
				distance = distan;
			}
			input_data.insert({distance, dose_all_dist});
			mean_dose_dist.push_back(mean/(mult_amount+0.));
			if (model.eof()) break;
		}
		model.close();

		std::vector<float> mist_sko;
		std::vector<float>::iterator mean_it = mean_dose_dist.begin();
		std::vector<int> distance;
		std::vector<float> sko;
		std::vector<float> mis;
		int amount = 0;
		for (std::map<int, std::vector<float> >::iterator it = input_data.begin(); it != input_data.end(); it++) {
			float sko_step = 0;
			for (std::vector<float>::iterator sko_i = (it->second).begin(); sko_i < (it->second).end(); sko_i++) {
				sko_step += std::pow((*mean_it - *sko_i), 2);
			}
			distance.push_back(it->first);
			sko.push_back(std::sqrt(sko_step/(mult_amount*(mult_amount-1))));
			mis.push_back(std::abs(*mean_it - exp_data)/(exp_data)*100);
			mist_sko.push_back(1./(*mean_it)*std::sqrt(std::pow(std::sqrt(sko_step/(mult_amount*(mult_amount-1))), 2)));
			++mean_it;
			++amount;
		}

		std::vector<float>::iterator it_dose = mean_dose_dist.begin();
		std::vector<float>::iterator it_sko = sko.begin();
		std::vector<float>::iterator it_mis = mis.begin();
		std::vector<float>::iterator it_mis_sko = mist_sko.begin();
		for (std::vector<int>::iterator it_distance = distance.begin(); it_distance != distance.end(); ++it_distance) {
			int point = gr->GetN();
			gr_err->SetPoint(gr_err->GetN(), (*it_distance), (*it_mis));
			if (*it_dose == 0) continue;
			gr_err->SetPointError(point, 0, (*it_mis_sko)*100);
			gr->SetPoint(point, (*it_distance), std::log((*it_dose)*1000));
			gr1->SetPoint(point, (*it_distance), std::log((exp_data)*1000));			
			gr->SetPointError(point, 0, (*it_sko)/((*it_dose)));
			++it_dose;
			++it_sko;
			++it_mis;
			++it_mis_sko;
		}
		mg->Add(gr);
		gr->SetTitle("GEANT4");
	}
	mg->Add(gr1);

	mg->Draw("apl3");
	mg->GetXaxis()->SetTitle("Particles, amount");
	mg->GetYaxis()->SetTitle("ln(D), mrad/sec");
	c1->BuildLegend();
	auto c2 = new TCanvas("c2","error",700,500);
   	c2->SetGrid();
	gStyle->SetOptFit();

	gr_err->Draw("APL3");
	c1->SetLogx();
	c2->SetLogx();
	gr_err->GetXaxis()->SetTitle("Particles, amount");
	gr_err->GetYaxis()->SetTitle("mistake, %");

	c1->SaveAs("figure/dose_mc.png");
	c2->SaveAs("figure/error_mc.png");	
}
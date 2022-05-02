#include <iostream>
#include <fstream>
#include <cmath>
#include <map>
#include <vector>
#include <sstream>

void distance_plot()
{
	const int mult_amount = 10;
	std::ifstream model;
	std::map<int, std::vector<float> > input_data;

	auto c1 = new TCanvas("c1","dose",700,500);
   	c1->SetGrid();
	gStyle->SetOptFit();
	
	TGraphErrors *gr = new TGraphErrors();
	gr->SetFillColor(1);
	gr->SetFillStyle(3002);
   	gr->SetMarkerStyle(20);
   	gr->SetMarkerSize(1);
   	gr->SetMarkerColor(1);
   	gr->SetLineColor(1);
	
	std::string name = "distance/distanceResult.csv";
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
			++mean_it;
			++amount;
		}

		std::vector<float>::iterator it_dose = mean_dose_dist.begin();
		std::vector<float>::iterator it_sko = sko.begin();
		for (std::vector<int>::iterator it_distance = distance.begin(); it_distance != distance.end(); ++it_distance) {
			int point = gr->GetN();
			if (*it_dose == 0) continue;
			gr->SetPoint(point, (*it_distance), std::log((*it_dose)*1000));
			gr->SetPointError(point, 0, (*it_sko)/((*it_dose)));
			++it_dose;
			++it_sko;
		}
	}

	gr->Draw("apl3");
	gr->GetXaxis()->SetTitle("r, cm");
	gr->GetYaxis()->SetTitle("ln(D), mrad/sec");

	c1->SaveAs("figure/dose.png");
}
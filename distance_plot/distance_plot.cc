#include <iostream>
#include <fstream>
#include <cmath>
#include <map>

void distance_plot()
{
	std::vector<float> exp_data = {4.38224094, 0.506902748, 0.185518641, 0.100267959, 0.060958821, 0.040324669, 0.028542712, 0.021523865, 0.016913534, 0.013201903, 0.010712504, 0.00952622, 0.008654481, 0.007468197, 0.00674025};
	std::vector<int> exp_dist = {0, 70, 140, 210, 280, 350, 420, 490, 560, 630, 700, 770, 840, 910, 980};
	std::vector<float> exp_mist = {0.0133973736787141, 0.00161481160892966, 0.000831052142980693, 0.000311694762051112, 0.000197734423945098, 0.000103487168332118, 6.79692935581946E-05, 6.48061786251899E-05, 6.73725366733366E-05, 3.97871450647065E-05, 2.98065069909239E-05, 3.88629304131845E-05, 2.49379129299148E-05, 2.29124441843292E-05, 2.30880210368927E-05};
	
	const int amount_models = 11;
	const int first_model = 11;
	const int mult_amount = 10;
	std::vector<std::map<int, std::vector<float> > > all_models_data;

	auto c1 = new TCanvas("c1","dose",700,500);
   	c1->SetGrid();
	gStyle->SetOptFit();

	auto leg = new TLegend(0.1,0.15,0.5,0.55);
	auto mg = new TMultiGraph();
	auto mg_err = new TMultiGraph();
	TGraphErrors *gr;
	TGraphErrors *gr_err;

	gr = new TGraphErrors();
	gr->SetFillColor(1);
	gr->SetFillStyle(3002);
   	gr->SetMarkerStyle(19+1);
   	gr->SetMarkerSize(1);
   	gr->SetMarkerColor(1);
   	gr->SetLineColor(1);
	gr->SetTitle("experimental");

	std::vector<float>::iterator it_dose1 = exp_data.begin();
	std::vector<float>::iterator it_sko1 = exp_mist.begin();			
	for (std::vector<int>::iterator it_distance = exp_dist.begin(); it_distance != exp_dist.end(); ++it_distance) {
		int point = gr->GetN();
		if (*it_dose1 == 0) continue;
		gr->SetPoint(point, (*it_distance), std::log((*it_dose1)*1000));
		gr->SetPointError(point, 0, (*it_sko1)/((*it_dose1)));
		++it_dose1;
		++it_sko1;
	}
	mg->Add(gr);

	std::ifstream model;
	for (int i = first_model; i < amount_models+1; i++) {
		std::map<int, std::vector<float> > input_data;

		int color = i;
		if (i > 8) color = i + 1;
		gr = new TGraphErrors();
		gr->SetFillColor(color+1);
		gr->SetFillStyle(3002);
   		gr->SetMarkerStyle(19+color+1);
   		gr->SetMarkerSize(1);
   		gr->SetMarkerColor(color+1);
   		gr->SetLineColor(color+1);

		gr_err = new TGraphErrors();
		gr_err->SetFillColor(color+1);
		gr_err->SetFillStyle(3002);
   		gr_err->SetMarkerStyle(19+color+1);
   		gr_err->SetMarkerSize(1);
   		gr_err->SetMarkerColor(color+1);
   		gr_err->SetLineColor(color+1);

		std::string name = "model" + std::to_string(i) + ".csv";
		model.open(name, std::ios::in);
		auto leg = new TLegend(0.1,0.15,0.5,0.55);
   		leg->SetHeader("Legend");


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
				all_models_data.push_back(input_data);
				mean_dose_dist.push_back(mean/(mult_amount+0.));
				if (model.eof()) break;
			}
			model.close();

			std::vector<float>::iterator it_Dexp_sko = exp_mist.begin();
			std::vector<float> mist_sko;
			std::vector<float>::iterator mean_it = mean_dose_dist.begin();
			std::vector<float>::iterator exp_it = exp_data.begin();
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
				mis.push_back(std::abs(*mean_it - *exp_it)/(*exp_it)*100);
				mist_sko.push_back(1./(*mean_it)*std::sqrt(std::pow(std::sqrt(sko_step/(mult_amount*(mult_amount-1))), 2)+std::pow((*mean_it)*(*it_Dexp_sko)/(*exp_it), 2)));
				++mean_it;
				++amount;
				++exp_it;
				++it_Dexp_sko;
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
				gr->SetPointError(point, 0, (*it_sko)/((*it_dose)));
				++it_dose;
				++it_sko;
				++it_mis;
				++it_mis_sko;
			}
			std::stringstream tmp;
			tmp << "model ";
			tmp << i;
			char const *ch = tmp.str().c_str();
			gr->SetTitle(ch);
			gr_err->SetTitle(ch);
			mg->Add(gr);
			mg_err->Add(gr_err);
		}

		mg->Draw("apl3");
		mg->GetXaxis()->SetTitle("r, cm");
		mg->GetYaxis()->SetTitle("ln(D), mrad/sec");
		c1->BuildLegend();
	}
	auto c2 = new TCanvas("c2","error",700,500);
   	c2->SetGrid();
	gStyle->SetOptFit();

	mg_err->Draw("APL3");
	mg_err->GetXaxis()->SetTitle("r, cm");
	mg_err->GetYaxis()->SetTitle("mistake, %");
	c2->BuildLegend();

	c1->SaveAs("dose.png");
	c2->SaveAs("error.png");	
}
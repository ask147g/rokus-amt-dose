#include <iostream>
#include <fstream>

void distance_plot()
{
	std::ifstream model;
	model.open("model1.txt", std::ios::in);
	
	gStyle->SetOptFit();
   	auto c1 = new TCanvas("c1","result",700,500);
   	c1->SetGrid();
   	
   	auto mg = new TMultiGraph();
   	mg->SetTitle("Dependence neutrons from material");
   		
	auto gr1 = new TGraph();
   	gr1 -> SetLineWidth(1);
   	gr1 -> SetMarkerStyle(20);
   	gr1 -> SetMarkerSize(1);
   	gr1 -> SetMarkerColor(1);
   	gr1->SetLineColor(1);
   	

	if (model)
	{
		const int count = 150;
		int i = 0;
		float dose_all[count];
		float model_amount[count];
		while (1)
		{
			model >> dose_all[i] >> model_amount[i];
			if (model.eof()) break;
			gr1->SetPoint(gr1->GetN(), model_amount[i], dose_all[i]);
			i++;
		}
		model.close();
		

		mg->Add(gr1);
		
		mg->GetXaxis()->SetTitle("Material");
		mg->GetYaxis()->SetTitle("Amount, particle");
		
		mg->Draw("APL");
		
		auto leg = new TLegend(0.1,0.15,0.5,0.55);
   		leg->SetHeader("Legend");
   		leg->AddEntry(gr1,"QBBC","lp");
   		leg->Draw();
		

	}
	else
	{
		std::cout << "Data not found" << std::endl;
	}
	
}
#include <cmath>
#include <vector>
#include <fstream>
#include <iostream>
 
void closet_mistake(){

	std::ifstream exp, geant;
	exp.open("closet/exp.csv", std::ios::in);
	geant.open("closet/closetResult.csv", std::ios::in);

	std::vector<double> data, data2;
	int amount;

	TGraph2D *output = new TGraph2D();
	if (exp) {
		while (1) {
			int i;
			double time, time2;
			if (exp.eof()) break;
			if (geant.eof()) break;
			exp >> i >> time;
			geant >> i >> time2;
			data.push_back(time);
			data2.push_back(time2);
		}
		amount = sqrt(data.size());
	}

	double size = 1./(amount+2);
	std::cout << amount << std::endl;

   TCanvas *c1 = new TCanvas("c1","Closet",200,10,600,600);
 
   gBenchmark->Start("closet_plot");
 
   TPad *pad1 = new TPad("pad1","pad1",0.05,1,1,1);
   pad1->SetFillColor(0);
   pad1->Draw();
 
   pad1->cd();
   float yt1 = 0.1;
   
   TLine *line1;
   for (int i = 1; i <= amount; ++i) {
	   for (int k = 1; k <= amount+1; ++k) {
		   	line1 = new TLine(i*size,1-k*size,1-size,1-k*size);
   			line1->SetLineWidth(2);
   			line1->SetLineColor(1);
   			line1->Draw();

			line1 = new TLine(1-k*size,i*size,1-k*size,1-size);
   			line1->SetLineWidth(2);
   			line1->SetLineColor(1);
   			line1->Draw();
	   }
   }

	TText *t1;
	std::vector<double>::iterator it = data.begin();
	std::vector<double>::iterator it2 = data2.begin();
   for (int i = 1; i <= amount; ++i) {
	   for (int k = 2; k <= amount+1; ++k) {
		   		std::stringstream tmp;
				double mis = abs((*it-*it2)/(*it)*100);
				tmp << std::setprecision(4) << mis;
				tmp << " %";
				char const *text = tmp.str().c_str();
		   		t1 = new TText(k*size-size/2.,1-i*size-size/2.,text);
				if (*it == 0) t1 = new TText(k*size-size/2.,1-i*size-size/2.,"-");
   				t1->SetTextAlign(22);
   				t1->SetTextSize(0.03);
				   t1->Draw();
			   ++it;
			   ++it2;
	   }
   }
 
  gBenchmark->Show("closet_plot");
  c1->SaveAs("figure/closet.png");
}
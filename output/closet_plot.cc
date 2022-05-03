#include <cmath>
#include <vector>
#include <fstream>
 
void closet_plot(){

	std::ifstream input;
	input.open("closet/closetResult.csv", std::ios::in);

	std::vector<double> data;
	int amount;

	TGraph2D *output = new TGraph2D();
	if (input) {
		while (1) {
			int i;
			double time;
			input >> i >> time;
			if (input.eof()) break;
			data.push_back(time);
		}
		amount = sqrt(data.size());
	}

	double size = 1./(amount+2);
	std::cout << amount << std::endl;

   TCanvas *c1 = new TCanvas("c1","Canvas Example",200,10,600,600);
 
   gBenchmark->Start("closet_plot");
 
   TPad *pad1 = new TPad("pad1","This is pad1",0.05,1,1,1);
   pad1->SetFillColor(0);
   pad1->Draw();
 
   pad1->cd();
   float yt1 = 0.1;
   //t1->Draw();
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
   for (int i = 1; i <= amount; ++i) {
	   for (int k = 2; k <= amount+1; ++k) {
		   		std::stringstream tmp;
				tmp << std::setprecision(4) << (*it);
				char const *text = tmp.str().c_str();
		   		t1 = new TText(k*size-size/2.,1-i*size-size/2.,text);
   				t1->SetTextAlign(22);
   				t1->SetTextSize(0.03);
				   t1->Draw();
			   ++it;
	   }
   }
 
  gBenchmark->Show("closet_plot");
  c1->SaveAs("figure/closet.png");
}
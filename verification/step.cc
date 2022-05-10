void step()
{
	auto c1 = new TCanvas("c1","dose",700,500);
   	c1->SetGrid();
	gStyle->SetOptFit();

	int planeSize[16] = {50, 50, 100, 100, 150, 150, 200, 200, 250, 250, 300, 300, 350,  350, 400, 400};
	int planeBiasing[16] = {0, 100, 100, 150, 150, 200, 200, 450, 450, 650, 650, 750, 750, 850, 850, 1000};

	TGraph* gr = new TGraph(16, planeBiasing, planeSize);
	gr->SetFillColor(1);
	gr->SetFillStyle(3002);
   	gr->SetMarkerStyle(19+1);
   	gr->SetLineWidth(3);
   	gr->SetMarkerColor(1);
   	gr->SetLineColor(1);
	gr->SetTitle("");

	gr->Draw("al3");
	gr->GetXaxis()->SetTitle("x, cm");
	gr->GetYaxis()->SetTitle("a, cm");

	c1->SaveAs("figure/step.png");
}
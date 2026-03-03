//Analysis Code Root Macro 
//Importing TTrees

using namespace std;

////////////////////////////////////////////////
// User Inputs in this section
////////////////////////////////////////////////

//Lambda and Kaon Extraction May require slightly different parameters
//Convecntion here: element[0] = Lambda ; element[1] = Kaon

vector<int> trainnum = { 100,100 }; //number of time NN is trained
vector<string> network = { "Mass:350:Count", "Mass:350:Count" }; //defining NN
//int varidx = 0;

//Vectors of fit subtraction values for lambda=0 and k0=1 particles, values for mlp
//vector<double> leftfitbound1 = { 1.082, 0.36 };
//vector<double> leftfitbound2 = { 1.1, 0.425 };
//vector<double> rightfitbound1 = { 1.132, 0.57 };
//vector<double> rightfitbound2 = { 1.16, 0.66 };
//vector<double> leftsubbound = { 1.1, 0.43 };
//vector<double> rightsubbound = { 1.132, 0.565 };

//Fit ranges defined for line BG fits
vector<double> leftfitbound1 = { 1.093, 0.4 };
vector<double> leftfitbound2 = { 1.106, 0.42 };
//vector<double> rightfitbound1 = { 1.134, 0.58 };
vector<double> rightfitbound1 = { 1.13, 0.58 };
vector<double> rightfitbound2 = { 1.14, 0.60 };
vector<double> leftsubbound = { 1.092, 0.42 };
vector<double> rightsubbound = { 1.134, 0.58 };
//defining vectors of the binning definitions for each of the kinematic variables we are looking at
//when playing with binning, have to adjust these vectors and hand specify the number of bins for defining Hist arrays
const int q2entries = 9;


//Branching Ratios
double BRlambda = 0.639;
double BRkaon = 0.692;


//output files
string outputfilename = "/data/dust/group/h1/gtustin/h1oo/H1LambdaC/AnalysisResultFiles/AnalysisOutput.root";
string dummyfile = "/data/dust/group/h1/gtustin/h1oo/H1LambdaC/dummyfile.root";

// Histograms Binning
Double_t lambdabins = 1000;
Double_t lambdaupperbinrange = 1.5;
Double_t lambdalowerbinrange = 1.0;
Double_t kaonbins = 500;
Double_t kaonupperbinrange = 1.0;
Double_t kaonlowerbinrange = 0.0;

// Get pion mass
double mPion = TDatabasePDG::Instance()->GetParticle(211)->Mass();
// Get proton mass
double mProton = TDatabasePDG::Instance()->GetParticle(2212)->Mass();
// Get Lambda mass
double mLambda = TDatabasePDG::Instance()->GetParticle(3122)->Mass();


/////////////////////////////////////////////////
// End of Input Section
/////////////////////////////////////////////////

//Defining functions used for calculations
double Square(double value) {
	return value * value;
}

double GetRatioError(double ratio, double numerator, double errnumerator, double denominator, double errdenominator) {
	return abs(ratio) * sqrt(Square(errnumerator / numerator) + Square(errdenominator / denominator));
}


//creating file to output histograms to
TFile outputfile(outputfilename.c_str(), "RECREATE");
TFile* f = new TFile(dummyfile.c_str(),"RECREATE");

//defining function and reject bool for the function fit of our background
bool reject;
double flineLambda(double* x, double* par)
{
	if (reject && x[0] > leftfitbound2[0] && x[0] < rightfitbound1[0]) {
		TF1::RejectPoint();
		return 0;
	}
	return par[0] + par[1] * x[0] + par[2] * x[0] * x[0];
}

//RooGenericPdf bg("bg", "pow(max(mLam-01.0782,0.),a1)*exp(a2*(mLam-1.0782))", RooArgSet(mLam, a1, a2));
//double bgfun(double mLam, double a1, double a2)
//{
//	return pow(mLam - 01.0782, a1) * exp(a2 * (mLam - 1.0782));
//}

//double fPolyLambda(double* x, double* par)
//{
//	if (reject && x[0] > leftfitbound2[0] && x[0] < rightfitbound1[0]) {
//		TF1::RejectPoint();
//		return 0;
//	}
//
//	if (x[0] < mProton + mPion) {
//		return 0;
//	}
//
//	if (x[0]>mProton + mPion) {
//		return par[0] * pow(x[0] - mPion - mProton, par[1])*exp(par[2]+x[0]*par[3]+x[0]*x[0]*par[4]);
//	}
//
//}

double fPolyLambda(double* x, double* par)
{
	if (reject && x[0] > leftfitbound2[0] && x[0] < rightfitbound1[0]) {
		TF1::RejectPoint();
		return 0;
	}

	if (reject && x[0] > 1.140 && x[0] < 1.145) {
		TF1::RejectPoint();
		return 0;
	}

	if (x[0] < mProton + mPion) {
		return 0;
	}

	if (x[0] > mProton + mPion) {
		//RooGenericPdf bg("bg", "Power(Max(mLam-01.0782,0.),a1)*Exp(a2*(mLam-1.0782))", RooArgSet(mLam, a1, a2));
		return par[0] * pow(x[0] - mPion - mProton, par[1]);
	}

}


//double fPolyLambdaVal(double x, double* par)
//{
//	if (reject && x > leftfitbound2[0] && x < rightfitbound1[0]) {
//		TF1::RejectPoint();
//		return 0;
//	}
//
//	if (x < mProton + mPion) {
//		return 0;
//	}
//
//	if (x > mProton + mPion) {
//		return par[0] * pow(x - mPion - mProton, par[1]) * exp(par[2] + x * par[3] + x * x * par[4]);
//	}
//
//}

double fPolyLambdaVal(double x, double* par)
{
	if (reject && x > leftfitbound2[0] && x < rightfitbound1[0]) {
		TF1::RejectPoint();
		return 0;
	}

	if (reject && x > 1.140 && x < 1.145) {
		TF1::RejectPoint();
		return 0;
	}

	if (x < mProton + mPion) {
		return 0;
	}

	if (x > mProton + mPion) {
		return par[0] * pow(x - mPion - mProton, par[1]);
	}

}

double flineK0(double* x, double* par)
{
	if (reject && x[0] > leftfitbound2[1] && x[0] < rightfitbound1[1]) {
		TF1::RejectPoint();
		return 0;
	}
	return par[0] + par[1] * x[0];
}

//MLP function, this function does the mlp fit and peak extraction, output is a 3 component vecotr {total particle peak count after BG subtraction, error in the peak count, total BG count}
vector<double> linearfunction(TH1F* inHist, string plotname, int particlecode, TCanvas* c3, TCanvas* c1) { //when calling function, particle code deictates 0=lambda or 1=kaon MLP parameters

	//vector<double> leftfitbound1 = { 1.09, 0.4 };
	//vector<double> leftfitbound2 = { 1.1, 0.43 };
	//vector<double> rightfitbound1 = { 1.135, 0.57 };
	//vector<double> rightfitbound2 = { 1.145, 0.60 };
	//vector<double> leftsubbound = { 1.1, 0.43 };
	//vector<double> rightsubbound = { 1.132, 0.565 };
	//Define a fit function to fit to BG, BG is just flat so will use a line
	//TF1* f1 = new TF1("f1", "[0]*x + [1]", xmin, xmax, );
	//double x1 = mProton+mPion;
	double x1 = leftfitbound1[particlecode];
	double x2 = leftfitbound2[particlecode];
	double x3 = rightfitbound1[particlecode];
	double x4 = rightfitbound2[particlecode];

	TF1* f1 = new TF1("f1", fPolyLambda, x1, x4, 2);

	f1->SetParameter(0, 1.0);
	f1->SetParameter(1, 0.8);
	//f1->SetParLimits(1, 0.1, 1.0);


	//f1->SetParameter(2, 1.0);
	//f1->SetParameter(3, 1.0);
	//f1->SetParameter(4, 1.0);
	//f1->SetParameter(2, -1.0);

	//Observable
	RooRealVar mLam("mLam", "mLam", x1, x4);
	//Parameters
	RooRealVar a0("a0", "a0", 10., .0, 1.0e6);
	RooRealVar a1("a1", "a1", 0.5, 0., 5.);
	RooRealVar a2("a2", "a2", 0.0, -100., 100.);
	//Background PDF
	//RooGenericPdf bg("bg", "Power(max(mLam-01.0782,0.),a1)*exp(a2*(mLam-1.0782))", RooArgSet(mLam, a1, a2));
	RooGenericPdf bg("bg", "Power(max(mLam-01.0782,0.),a1)", RooArgSet(mLam, a1));


	//Clone a hist from the input histogram then reset its data to clear
	TH2F* BGhist = (TH2F*)inHist->Clone();
	BGhist->Reset("ICESM");
	//MLP funciton takes a tuple input, will copy this tuple input into the histogram to use for functional fit
	TNtuple* ntp_bg = new TNtuple("datatree", "datatree", "Mass:Count");


	//Read in the bin center and bin content from the histogram and prepare the text file
	for (int i = 1; i < (int)inHist->GetNbinsX() + 1; i++) {

		double bcx = inHist->GetBinCenter(i);

		if (isinf(inHist->GetBinContent(i))) { inHist->SetBinContent(i, 0); }

		if ((bcx > leftfitbound1[particlecode] && bcx < leftfitbound2[particlecode]) || (bcx > rightfitbound1[particlecode] && bcx < rightfitbound2[particlecode])) {
			//ofl << bcx << "\t" << inHist->GetBinContent(i) << endl;
			ntp_bg->Fill(bcx, inHist->GetBinContent(i));
			BGhist->SetBinContent(i, inHist->GetBinContent(i));
		}
	}

	//Tuple is now filled with BG data, can copy it to BGhist, I tried this but it does not work
	//ntp_bg->Draw("variable>>BGhist");
	//Now fit the hist to previously defined function
	//Fit only the BG regions
	reject = true;
	inHist->Fit("f1", "R0", "0", x1, x4);
	reject = false;

	//TF1* f2 = new TF1("f2", flineLambda, 0, 2.5, 2);

	cout << "Param1: " << f1->GetParameter(0) << endl;
	cout << "Param2: " << f1->GetParameter(1) << endl;

	//c1->cd();
	//TMultiLayerPerceptron* mlp = new TMultiLayerPerceptron(network[particlecode].c_str(), ntp_bg); //Defining the architecture - 1 hidden layer with 100 nodes
	//mlp->Train(trainnum[particlecode], "graph, current, update=1000");

	//outputfile.cd();
	//c1->Write();
	//outputfile.Write();
	f->cd();




	TH1F* fgHist = (TH1F*)inHist->Clone();


	Double_t inputs[1];

	TH2F* fgData = (TH2F*)fgHist->Clone();
	fgData->Reset("ICESM");

	TH2F* signalHist = (TH2F*)fgHist->Clone();
	signalHist->Reset("ICESM");

	TH2F* fitHistsubrange = (TH2F*)fgHist->Clone();
	fitHistsubrange->Reset("ICESM");

	TH2F* xaxis = (TH2F*)fgHist->Clone();
	xaxis->Reset("ICESM");
	xaxis->SetLineColor(kBlack);


	TH2F* fitHist = (TH2F*)fgHist->Clone();
	fitHist->Reset("ICESM");

	fgHist->SetLineColor(kBlack);
	signalHist->SetLineColor(kRed);
	fitHist->SetLineColor(kMagenta);
	fgData->SetLineColor(kBlack);
	fitHistsubrange->SetLineColor(kBlue);

	double kcount = 0;
	//double kcounterror = 0;
	double bgcount = 0;
	double totalcountwithbg = 0;

	for (int i = 1; i < fgHist->GetNbinsX() + 1; i++) {

		inputs[0] = ((TAxis*)fgHist->GetXaxis())->GetBinCenter(i);
		fgData->SetBinContent(i, fgHist->GetBinContent(i));
		fgData->SetBinError(i, fgHist->GetBinError(i));

		if ((inputs[0] >= leftfitbound1[particlecode] && inputs[0] < leftfitbound2[particlecode]) || (inputs[0] >= rightfitbound1[particlecode] && inputs[0] < rightfitbound2[particlecode])) {
			//fitHist->SetBinContent(i, mlp->Evaluate(0, inputs));
			//fitHist->SetBinContent(i, (f1->GetParameter(0)+(f1->GetParameter(1))*(inputs[0])+(f1->GetParameter(2))* (f1->GetParameter(2))*(inputs[0])));
			fitHist->SetBinContent(i, (fPolyLambdaVal(inputs[0], f1->GetParameters())));
		}

		if (inputs[0] >= leftsubbound[particlecode] && inputs[0] < rightsubbound[particlecode]) {
			//double temp_content = fgHist->GetBinContent(i) - mlp->Evaluate(0, inputs);
			double temp_content = fgHist->GetBinContent(i) - (fPolyLambdaVal(inputs[0], f1->GetParameters()));
			signalHist->SetBinContent(i, temp_content);
			//fitHist->SetBinContent(i, mlp->Evaluate(0, inputs));
			signalHist->SetBinError(i, fgHist->GetBinError(i));
			//fitHistsubrange->SetBinContent(i, mlp->Evaluate(0, inputs));
			fitHistsubrange->SetBinContent(i, (fPolyLambdaVal(inputs[0], f1->GetParameters())));
			totalcountwithbg = totalcountwithbg + fgHist->GetBinContent(i);
			//kcounterror = kcounterror + fgHist->GetBinError(i);
			//if (temp_content > 0) {
			kcount = kcount + temp_content;
			//bgcount = bgcount + mlp->Evaluate(0, inputs);
			bgcount = bgcount + (fPolyLambdaVal(inputs[0], f1->GetParameters()));
			//}

		}

	}

	double kcounterror = sqrt(totalcountwithbg);

	//Draw MLP Fit
	c3->cd();
	fgHist->GetXaxis()->SetTitle("Mass [Gev]");
	fgHist->GetYaxis()->SetTitle("Count");
	fgHist->SetNameTitle(plotname.c_str(), plotname.c_str());
	fgHist->Draw();

	f1->Draw("same");
	signalHist->Draw("same");
	fitHist->Draw("same");
	fitHistsubrange->Draw("same");
	xaxis->Draw("same");

	TLegend* legend = new TLegend(0.5513, 0.4144, 0.888, 0.8823);
	//legend->SetHeader(Form("#splitline{Count- %.2f #pm %.2f (stat.)}{Stochastic MLP- %s Network Trained %ix}", kcount, kcounterror, network[particlecode].c_str(), trainnum[particlecode])); // option "C" allows to center the header
	if (particlecode == 0) {
		//f1->SetFunction(flineLambda);
		legend->SetHeader(Form("#splitline{Count: %.2f #pm %.2f (stat.)}{Linear Fit: Ax^2+Bx+C, A = %.2f #pm %.2f, B = %.2f #pm %.2f, C = %.2f #pm %.2f}", kcount, kcounterror, f1->GetParameter(2), f1->GetParError(2), f1->GetParameter(1), f1->GetParError(1), f1->GetParameter(0), f1->GetParError(0))); // option "C" allows to center the header
	}
	if (particlecode == 1) {
		//f1->SetFunction(flineK0);
		legend->SetHeader(Form("#splitline{Count: %.2f #pm %.2f (stat.)}{Linear Fit: Ax+B, A = %.2f #pm %.2f, B = %.2f #pm %.2f}", kcount, kcounterror, f1->GetParameter(1), f1->GetParError(1), f1->GetParameter(0), f1->GetParError(0))); // option "C" allows to center the header
	}
	//legend->SetHeader(Form("#splitline{Count: %.2f #pm %.2f (stat.)}{Linear Fit: Ax+B, A = %.2f #pm %.2f, B = %.2f #pm %.2f}", kcount, kcounterror, f1->GetParameter(0), f1->GetParError(0), f1->GetParameter(1), f1->GetParError(1))); // option "C" allows to center the header
	legend->AddEntry(fgHist, "Data", "l");
	legend->AddEntry(fitHist, "Fit Accross Data BG", "l");
	legend->AddEntry(fitHistsubrange, "Fit Extended under Peak", "l");
	legend->AddEntry(signalHist, "Subtracted Peak", "lep");
	legend->Draw("same");
	f1->Draw("same");

	outputfile.cd();
	c3->Write();

	//Draw Line Fit
	//c3->cd();
	//fgHist->Draw();
	//BGhist->Draw("same");
	//fgHist->Draw("same");
	//f1->Draw("same");
	//outputfile.cd();
	//c3->Write();

	outputfile.Write();
	f->cd();



	//delete training data after each iteration
	ntp_bg->Reset();
	return { kcount,kcounterror, bgcount };
};

//MLP function, this function does the mlp fit and peak extraction, output is a 3 component vecotr {total particle peak count after BG subtraction, error in the peak count, total BG count}
vector<double> kY2(TH1F* inHist, string plotname, int particlecode, TCanvas* c3, TCanvas* c1, vector<double> fitparaminit, vector<double> rangedefs) { //when calling function, particle code deictates 0=lambda or 1=kaon MLP parameters
	
	//vector<double> leftfitbound1 = { 1.09, 0.4 };
	//vector<double> leftfitbound2 = { 1.1, 0.43 };
	//vector<double> rightfitbound1 = { 1.135, 0.57 };
	//vector<double> rightfitbound2 = { 1.145, 0.60 };
	//vector<double> leftsubbound = { 1.1, 0.43 };
	//vector<double> rightsubbound = { 1.132, 0.565 };
	//Define a fit function to fit to BG, BG is just flat so will use a line
	//TF1* f1 = new TF1("f1", "[0]*x + [1]", xmin, xmax, );
	//double x1 = mProton+mPion;
	double x1 = leftfitbound1[particlecode];
	double x2 = leftfitbound2[particlecode];
	double x3 = rightfitbound1[particlecode];
	double x4 = rightfitbound2[particlecode];

	//TF1* f1 = new TF1("f1", fPolyLambda, x1, x4, 2);

	//f1->SetParameter(0, 1.0);
	//f1->SetParameter(1, 0.8);
	

	//Observable
	//RooRealVar mLam("mLam", "mLam", 1.1, 1.13);
	RooRealVar mLam("mLam", "mLam", x1, x4);
	RooRealVar mLamDummy("mLamDummy", "mLamDummy", x1, x4);
	//Parameters
	RooRealVar a0("a0", "a0", 10., .0, 1.0e6);
	RooRealVar a1("a1", "a1", 0.5, 0., 5.);
	RooRealVar a2("a2", "a2", 0.0, -100., 100.);
	RooRealVar a1Dummy("a1Dummy", "a1Dummy", 0.5, 0., 5.);
	RooRealVar a2Dummy("a2Dummy", "a2Dummy", 0.0, -100., 100.);
	RooRealVar n("n", "# background events", 10., 0., 1.0e9);
	//Background PDF
	//RooGenericPdf bg("bg", "Power(Max(mLam-01.0782,0.),a1)*Exp(a2*(mLam-1.0782))", RooArgSet(mLam, a1, a2));
	//RooGenericPdf bg("bg", "pow(max(mLam-01.0782,0.),a1)*exp(a2*(mLam-1.0782))", RooArgSet(mLam, a1, a2));
	RooGenericPdf bg("bg", "pow(max(mLam-01.0782,0.),a1)*exp(a2*(mLam-1.0782))", RooArgSet(mLam, a1, a2));
	//RooGenericPdf bg("bg", "pow(max(mLam-01.0782,0.),a1)", RooArgSet(mLam, a1));
	mLam.setRange("full", x1, x4);
	mLam.setRange("left", x1, x2);
	mLam.setRange("right", x3, x4);
	mLam.setRange("signal", x2, x3);
	RooExtendPdf bgnorm("bgnorm", "bgnorm", bg, n);
	///////////////////////////////////////////////////////////////////////////////
	cout << "RAW Val(): " << bgnorm.getVal() << endl;
	RooArgSet nset(mLam);
	cout << "Norm to mLam Val(): " << bgnorm.getVal(&nset) << endl;
	std::unique_ptr<RooAbsReal> igx{ bgnorm.createIntegral(mLam) };
	cout << "Int[mLam] = " << igx->getVal() << endl;
	std::unique_ptr<RooAbsReal> igx_sig{ bgnorm.createIntegral(mLam, RooFit::NormSet(mLam), RooFit::Range("signal")) };
	cout << "gx_Int[x|signal]_Norm[x] = " << igx_sig->getVal() << endl;
	//////////////////////////////////////////////////////////////////////////////
	//RooAddPdf sum("sum", "signal+bg", RooArgList(sigf, bg), RooArgList(nsig, nbkg));
	RooGenericPdf bgDummy("bgDummy", "pow(max(mLamDummy-01.0782,0.),a1Dummy)*exp(a2Dummy*(mLamDummy-1.0782))", RooArgSet(mLamDummy, a1Dummy, a2Dummy));


	//Create two Gaussian PDFs g1(x,mean1,sigma) anf g2(x,mean2,sigma) and their parameters
	RooRealVar mu("mu", "mu", 1.1156);
	RooRealVar sigma1("sigma1", "sigma1", 0.002, 1.e-6,0.1);
	RooRealVar sigma2("sigma2", "sigma2", 0.001, 1.e-6, 0.1);
	RooRealVar alpha("alpha", "alpha", 0.9, 0,1.0);
	RooRealVar Stot("Stot", "Stot", 100, 10, 1.0e8);
	//RooGenericPdf sigf("sigf", "signal", "TMath::Gamma((p3+1)/2)/TMath::Gamma(p3/2)/TMath::Sqrt(TMath::Pi()*p3)*TMath::Power(1.+((mLam-mu)/sig)*((mLam-mu)/sig)/p3,-(p3+1)/2)", RooArgSet(mLam, mu, p3, sig));
	//RooGenericPdf sigf("sigf", "signal", "alpha* (1 / Stot)* (1 / (sigma1 * TMath::Sqrt(2 * TMath::Pi()))) * TMath::Exp(-(1 / 2) * pow(mLam - mu, 2) / (pow(sigma1, 2))) + (1 - alpha) * (1 / Stot) * (1 / (sigma2 * TMath::Sqrt(2 * TMath::Pi()))) * TMath::Exp(-(1 / 2) * pow(mLam - mu, 2) / (pow(sigma2, 2)))", RooArgSet(mLam, alpha, sigma1, sigma2, mu, Stot));
	RooGenericPdf sigf("sigf", "signal", "(Stot)* (1 / (sigma1 * TMath::Sqrt(2 * TMath::Pi()))) * TMath::Exp(-(1 / 2) * pow(mLam - mu, 2) / (pow(sigma1, 2)))", RooArgSet(mLam, sigma1, mu, Stot));

	//Signal+Bg PDF
	RooRealVar nsig("nsig", "# signal events", 10., 0, 1000000.);
	RooRealVar nbkg("nbkg", "# background events", 10., 0., 1e10);
	RooAddPdf sum("sum", "signal+bg", RooArgList(sigf, bg), RooArgList(nsig, nbkg));

	//Clone a hist from the input histogram then reset its data to clear
	TH2F* BGhist = (TH2F*)inHist->Clone();
	BGhist->Reset("ICESM");
	//MLP funciton takes a tuple input, will copy this tuple input into the histogram to use for functional fit
	TNtuple* ntp_bg = new TNtuple("datatree", "datatree", "Mass:Count");


	//Read in the bin center and bin content from the histogram and prepare the text file
	for (int i = 1; i < (int)inHist->GetNbinsX() + 1; i++) {

		double bcx = inHist->GetBinCenter(i);

		if (isinf(inHist->GetBinContent(i))) { inHist->SetBinContent(i, 0); }

		if ((bcx > leftfitbound1[particlecode] && bcx < leftfitbound2[particlecode]) || (bcx > rightfitbound1[particlecode] && bcx < rightfitbound2[particlecode])) {
			//ofl << bcx << "\t" << inHist->GetBinContent(i) << endl;
			ntp_bg->Fill(bcx, inHist->GetBinContent(i));
			BGhist->SetBinContent(i, inHist->GetBinContent(i));
		}
	}

	//Tuple is now filled with BG data, can copy it to BGhist, I tried this but it does not work
	//ntp_bg->Draw("variable>>BGhist");
	//Now fit the hist to previously defined function
	//Fit only the BG regions
	// Define the sidebands (e.g. background regions
	reject = true;
	//inHist->Fit("f1", "R0", "0", x1, x4);
	//Create binned dataset that imports TH1 contents and associates its contents to mLam variable of RootFit
	RooRealVar mLam2("mLam2", "mLam2", x1, x2);
	RooRealVar mLam3("mLam3", "mLam3", x3, x4);
	RooDataHist RooFitDataHist("RooFitDataHist", "RooFitDataHist", mLam, inHist);
	RooDataHist RooFitDataHistDummy("RooFitDataHist", "RooFitDataHist", mLamDummy, inHist);
	//std::unique_ptr<RooDataSet> data{ expo.generate(x, 1000) };
	//std::unique_ptr<RooAbsData> blindedData{ data->reduce(CutRange("left,right")) };
	//RooDataHist blindedDataHist(RooFitDataHist.reduce(RooFit::CutRange("left,right")));
	RooDataHist RooFitDataHist2("RooFitDataHist", "RooFitDataHist", mLam2, inHist);
	RooDataHist RooFitDataHist3("RooFitDataHist", "RooFitDataHist", mLam3, inHist);
	//Fit BG function to RooFit data hist
	//bg.fitTo(RooFitDataHist);
	//bg.fitTo(RooFitDataHist, "left,right");
	RooArgSet xvals(mLam);
	RooArgSet xvalsleft(mLam2);
	RooArgSet xvalsright(mLam3);
	//RooFitDataHist.reduce(RooFit::CutRange("signal"));
	RooFitResult* result = bgnorm.fitTo(RooFitDataHist,RooFit::Range("left,right"), RooFit::Save());

	///////////////////////////////////////////////////////////////////////////////
	cout << "RAW Val(): " << bgnorm.getVal() << endl;
	cout << "Norm to mLam Val(): " << bgnorm.getVal(mLam) << endl;
	cout << "Int[mLam] = " << igx->getVal() << endl;
	cout << "gx_Int[x|signal]_Norm[x] = " << igx_sig->getVal() << endl;
	//////////////////////////////////////////////////////////////////////////////
	//TF1* bgfunc = (TF1*)bg.asTF(mLam, RooArgList(a1, a2), mLam);
	cout << "GET NORM: " << n.getVal() << endl;
	
	//bg.fitTo(RooFitDataHist);
	bgDummy.fitTo(RooFitDataHistDummy);
	//RooFitDataHist.reduce(RooFit::CutRange("left,right"));
	cout << "Norm 1: " << bg.getNorm(RooArgSet(mLam)) << endl;
	cout << "Norm 2: " << bg.getNorm(RooArgSet()) << endl;
	double norm = bg.getNorm(RooArgSet(mLam));
	reject = false;
	//RooFitDataHist.reduce(RooFit::CutRange("full"));
	//TF1* bgfunc = (TF1*)bg.asTF(RooArgList(mLam), RooArgList(a1, a2), RooArgSet());
	
	//TF1* bgfunc = (TF1*)bg.asTF(mLam, RooArgList(a1, a2));
	//double norm = (1.0 / (bg.getNorm(xvalsleft) + bg.getNorm(xvalsright)));
	//double norm = (1.0 / (bg.getNorm(xvals)));
	//double norm = 1.0/bgDummy.getNorm(RooArgSet(mLamDummy));
	
	//double norm = 1;

	RooPlot* RooFitPlot = mLam.frame();
	RooFitDataHist.plotOn(RooFitPlot);
	bgnorm.plotOn(RooFitPlot, RooFit::Range("full"));
	//bgnorm.plotOn(RooFitPlot);
	//bgnorm.plotOn(RooFitPlot, RooFit::Range("full"));
	RooFitPlot->Write();
	RooCurve* FitCurve = RooFitPlot->getCurve("bgnorm");

	TF1* bgfunc = (TF1*)bgnorm.asTF(mLam, RooArgList(n, a1, a2),mLam);
	//TF1* bgfunc = (TF1*)bgnorm.asTF(mLam, RooArgList(n, a1), mLam);
	RooRealVar* var = (RooRealVar*)bgnorm.getObservables(RooArgSet(mLam))->first();
	
	cout << "BGNORM NORM: " << norm << endl; 
	cout << "FRBinWidth: " << RooFitPlot->getFitRangeBinW() << endl;
	cout << "FRNEvt: " << RooFitPlot->getFitRangeNEvt() << endl;
	cout << "FRNEvt Plot Range: " << RooFitPlot->getFitRangeNEvt(x1,x4) << endl;
	cout << "FRNEvt Left + RIGHT: " << RooFitPlot->getFitRangeNEvt(x1, x2) + RooFitPlot->getFitRangeNEvt(x3, x4) << endl;
	cout << "inHist NEvt: " << RooFitDataHist.sum(kFALSE) << endl;
	cout << "RAW Val(): " << bgnorm.getVal() << endl;
	cout << "Norm to mLam Val(): " << bgnorm.getVal(mLam) << endl;
	cout << "Int[mLam] = " << igx->getVal() << endl;
	cout << "gx_Int[x|signal]_Norm[x] = " << igx_sig->getVal() << endl;


	//cout << "Norm Variable: " << n << endl;
	//bg.plotOn(RooFitPlot, RooFit::Range("full"), RooFit::NormRange("left,right")); //This is equivalent to the line above, Norm range defaults to the range used for fitting
	//bg.plotOn(RooFitPlot, RooFit::Range("full"), RooFit::NormRange("full"));
	//cout << "a1: " << a1.getValV() << endl;
	double factor1 = RooFitPlot->getFitRangeNEvt();
	double BW = RooFitPlot->getFitRangeBinW();
	//TF1* f2 = new TF1("f2", flineLambda, 0, 2.5, 2);
	
	//cout << "Param1: " << f1->GetParameter(0) << endl;
	//cout << "Param2: " << f1->GetParameter(1) << endl;

	//c1->cd();
	//TMultiLayerPerceptron* mlp = new TMultiLayerPerceptron(network[particlecode].c_str(), ntp_bg); //Defining the architecture - 1 hidden layer with 100 nodes
	//mlp->Train(trainnum[particlecode], "graph, current, update=1000");

	//outputfile.cd();
	//c1->Write();
	//outputfile.Write();
	f->cd();

	TH1F* fgHist = (TH1F*)inHist->Clone();

	Double_t inputs[1];

	TH2F* fgData = (TH2F*)fgHist->Clone();
	fgData->Reset("ICESM");

	TH2F* signalHist = (TH2F*)fgHist->Clone();
	signalHist->Reset("ICESM");

	TH2F* fitHistsubrange = (TH2F*)fgHist->Clone();
	fitHistsubrange->Reset("ICESM");

	TH2F* checkHist = (TH2F*)inHist->Clone();
	checkHist->Reset("ICESM");

	TH2F* xaxis = (TH2F*)fgHist->Clone();
	xaxis->Reset("ICESM");
	xaxis->SetLineColor(kBlack);


	TH2F* fitHist = (TH2F*)fgHist->Clone();
	fitHist->Reset("ICESM");

	fgHist->SetLineColor(kBlack);
	signalHist->SetLineColor(kRed);
	fitHist->SetLineColor(kMagenta);
	fgData->SetLineColor(kBlack);
	fitHistsubrange->SetLineColor(kBlue);

	double kcount = 0;
	//double kcounterror = 0;
	double bgcount = 0;
	double totalcountwithbg = 0;
	double datacheck = 0;

	//std::unique_ptr<RooAbsReal> bgundersig{ bgnorm.createIntegral(mLam, RooFit::NormSet(mLam), RooFit::Range("signal")) }
	//RooAbsReal ig = bgnorm.createIntegral(mLam, mLam, "signal");
	std::unique_ptr<RooAbsReal> ig{ bgnorm.createIntegral(mLam, RooFit::Range("signal")) };
	double ntot = bgnorm.expectedEvents(RooArgSet(mLam));  //this should be nsig + nbkg
	cout << ig->getVal() * ntot << endl;
	//std::unique_ptr<RooAbsReal> bgundersig{ bgnorm.createIntegral(mLam, RooFit::Range("signal")) };
	double BGVALINT = ig->getVal() * ntot;
	cout << "BGVALINT: " << BGVALINT << endl;

	for (int i = 1; i < fgHist->GetNbinsX() + 1; i++) {

		inputs[0] = ((TAxis*)fgHist->GetXaxis())->GetBinCenter(i);
		fgData->SetBinContent(i, fgHist->GetBinContent(i));
		fgData->SetBinError(i, fgHist->GetBinError(i)); 

		if ((inputs[0] >= x1 && inputs[0] < x2 )|| (inputs[0] >= x3 && inputs[0] < x4)) {
			//HERE//fitHist->SetBinContent(i, (fPolyLambdaVal(inputs[0],f1->GetParameters())));
			datacheck = datacheck+inHist->GetBinContent(i);
			checkHist->SetBinContent(i, inHist->GetBinContent(i));
		}

		if (inputs[0] >= leftsubbound[particlecode] && inputs[0] < rightsubbound[particlecode]) {
			//mLam.setRange("bin", inputs[0]-BW/2, inputs[0] + BW/2);
			mLam.setVal(inputs[0]);//test
			//double temp_content = fgHist->GetBinContent(i) - bg.getVal(mLam);
			//double BGVAL = bgnorm.getValV();
			double BGVAL = bgfunc->Eval(inputs[0]);
			//double BGVAL = bgnorm.getVal(mLam) * (inputs[0]) * BW*n.getVal();
			std::unique_ptr<RooAbsReal> binintegral{ bgnorm.createIntegral(mLam, RooFit::NormSet(mLam), RooFit::Range("bin")) };
			//double BGVAL = binintegral->getVal();
			double temp_content = fgHist->GetBinContent(i) - BGVAL;// *norm;
			cout << "Data: " << fgHist->GetBinContent(i) << endl;
			cout << "mLam: " << inputs[0] << endl;
			cout << "a1: " << a1.getValV() << endl;
			cout << "a2: " << a2.getValV() << endl;
			cout << "BG: " << bgfunc->Eval(inputs[0]) << endl;
			cout << "Norm: " << norm << endl;
			cout << "Signal: " << temp_content << endl;
			signalHist->SetBinContent(i, temp_content);
			
			//fitHist->SetBinContent(i, mlp->Evaluate(0, inputs));
			signalHist->SetBinError(i, fgHist->GetBinError(i));
			//fitHistsubrange->SetBinContent(i, mlp->Evaluate(0, inputs));
			fitHistsubrange->SetBinContent(i, BGVAL);// *(norm);
			//fitHistsubrange->SetBinContent(i, bg.getVal(mLam));
			totalcountwithbg = totalcountwithbg + fgHist->GetBinContent(i);
			//kcounterror = kcounterror + fgHist->GetBinError(i);
			//if (temp_content > 0) {
				kcount = kcount + temp_content;
				//bgcount = bgcount + mlp->Evaluate(0, inputs);
				//bgcount = bgcount + (bgfunc->Eval(inputs[0]));// *(norm);
				bgcount = bgcount + BGVAL;
			//}

		}

	}
	cout << "DATA CHECK" << datacheck << endl;
	cout << "Range CHECK" << (x2-x1) + (x4-x3) << endl;
	double kcounterror = sqrt(totalcountwithbg);

	//Draw MLP Fit
	c3->cd();
	fgHist->GetXaxis()->SetTitle("Mass [Gev]");
	fgHist->GetYaxis()->SetTitle("Count");
	fgHist->SetNameTitle(plotname.c_str(), plotname.c_str());
	fgHist->Draw();

	//HERE//f1->Draw("same");
	RooFitPlot->Draw("same");
	signalHist->Draw("same");
	fitHist->Draw("same");
	checkHist->Draw("same");
	fitHistsubrange->Draw("same");
	xaxis->Draw("same");

	TLegend* legend = new TLegend(0.5513, 0.4144, 0.888, 0.8823);
	//legend->SetHeader(Form("#splitline{Count- %.2f #pm %.2f (stat.)}{Stochastic MLP- %s Network Trained %ix}", kcount, kcounterror, network[particlecode].c_str(), trainnum[particlecode])); // option "C" allows to center the header
	if (particlecode == 0) {
		//f1->SetFunction(flineLambda);
		//HERE//legend->SetHeader(Form("#splitline{Count: %.2f #pm %.2f (stat.)}{Linear Fit: Ax^2+Bx+C, A = %.2f #pm %.2f, B = %.2f #pm %.2f, C = %.2f #pm %.2f}", kcount, kcounterror, f1->GetParameter(2), f1->GetParError(2), f1->GetParameter(1), f1->GetParError(1), f1->GetParameter(0), f1->GetParError(0))); // option "C" allows to center the header
	}
	if (particlecode == 1) {
		//f1->SetFunction(flineK0);
		//HERE//legend->SetHeader(Form("#splitline{Count: %.2f #pm %.2f (stat.)}{Linear Fit: Ax+B, A = %.2f #pm %.2f, B = %.2f #pm %.2f}", kcount, kcounterror, f1->GetParameter(1), f1->GetParError(1), f1->GetParameter(0), f1->GetParError(0))); // option "C" allows to center the header
	}
	//legend->SetHeader(Form("#splitline{Count: %.2f #pm %.2f (stat.)}{Linear Fit: Ax+B, A = %.2f #pm %.2f, B = %.2f #pm %.2f}", kcount, kcounterror, f1->GetParameter(0), f1->GetParError(0), f1->GetParameter(1), f1->GetParError(1))); // option "C" allows to center the header
	legend->AddEntry(fgHist, "Data", "l");
	legend->AddEntry(fitHist, "Fit Accross Data BG", "l");
	legend->AddEntry(fitHistsubrange, "Fit Extended under Peak", "l");
	legend->AddEntry(signalHist, "Subtracted Peak", "lep");
	legend->Draw("same");
	//f1->Draw("same");

	outputfile.cd();
	c3->Write();
	RooFitPlot->Write();
	
	//Draw Line Fit
	//c3->cd();
	//fgHist->Draw();
	//BGhist->Draw("same");
	//fgHist->Draw("same");
	//f1->Draw("same");
	//outputfile.cd();
	//c3->Write();

	outputfile.Write();
	f->cd();



	//delete training data after each iteration
	ntp_bg->Reset();
	//return { kcount,kcounterror, bgcount };
	
	return { totalcountwithbg - BGVALINT ,kcounterror, bgcount };
};


//MLP function, this function does the mlp fit and peak extraction, output is a 3 component vecotr {total particle peak count after BG subtraction, error in the peak count, total BG count}
vector<double> mlpfunction(TH1F* inHist, string plotname, int particlecode, TCanvas* c3, TCanvas* c1, vector<double> fitparaminit, vector<double> rangedefs) { //when calling function, particle code deictates 0=lambda or 1=kaon MLP parameters

	//vector<double> leftfitbound1 = { 1.09, 0.4 };
	//vector<double> leftfitbound2 = { 1.1, 0.43 };
	//vector<double> rightfitbound1 = { 1.135, 0.57 };
	//vector<double> rightfitbound2 = { 1.145, 0.60 };
	//vector<double> leftsubbound = { 1.1, 0.43 };
	//vector<double> rightsubbound = { 1.132, 0.565 };
	//Define a fit function to fit to BG, BG is just flat so will use a line
	//TF1* f1 = new TF1("f1", "[0]*x + [1]", xmin, xmax, );
	//double x1 = mProton+mPion;
	/*double x1 = leftfitbound1[particlecode];
	double x2 = leftfitbound2[particlecode];
	double x3 = rightfitbound1[particlecode];
	double x4 = rightfitbound2[particlecode];
	double s1 = x2;
	double s2 = x3;*/
	double x1 = rangedefs[0];
	double x2 = rangedefs[1];
	double x3 = rangedefs[2];
	double x4 = rangedefs[3];
	double s1 = rangedefs[4];
	double s2 = rangedefs[5];

	TLine leftfitbound1line = TLine(x1, 0, x1, 200);
	TLine leftfitbound2line = TLine(x2, 0, x2, 200);
	TLine rightfitbound1line = TLine(x3, 0, x3, 200);
	TLine rightfitbound2line = TLine(x4, 0, x4, 200);
	TLine extractionbound1line = TLine(s1, 0, s1, 300);
	TLine extractionbound2line = TLine(s2, 0, s2, 300);

	leftfitbound1line.SetLineColor(kMagenta);
	leftfitbound2line.SetLineColor(kMagenta);;
	rightfitbound1line.SetLineColor(kMagenta);;
	rightfitbound2line.SetLineColor(kMagenta);
	extractionbound1line.SetLineColor(kRed);
	extractionbound2line.SetLineColor(kRed);

	//TF1* f1 = new TF1("f1", fPolyLambda, x1, x4, 2);

	//f1->SetParameter(0, 1.0);
	//f1->SetParameter(1, 0.8);


	//Observable
	//RooRealVar mLam("mLam", "mLam", 1.1, 1.13);
	RooRealVar mLam("mLam", "mLam", x1, x4);
	mLam.setRange("full", x1, x4);
	mLam.setRange("left", x1, x2);
	mLam.setRange("right", x3, x4);
	mLam.setRange("signal", s1, s2);
	mLam.setRange("compositefitrange", x2, x3);
	//Composite
	// Background PDF

	RooRealVar a0("a0", "a0", 10., .0, 1.0e6);
	RooRealVar a1("a1", "a1", 0.5, 0., 5.);
	RooRealVar a2("a2", "a2", 0.0, -100., 100.);
	RooGenericPdf bg("bg", "TMath::Power(TMath::Max(mLam-1.0782,0.),a1)*TMath::Exp(a2*(mLam-1.0782))", RooArgSet(mLam, a1, a2));

	// Signal PDF
	//Students T
	//RooRealVar mu("mu", "mu", 1.1156);
	RooRealVar mu("mu", "mu", fitparaminit[2]);
	RooRealVar an("an", "an", 10., 0., 1.0e6);
	RooRealVar sigma("sigma", "sigma", fitparaminit[0]);
	RooRealVar p3("p3", "p3", fitparaminit[1]);
	RooGenericPdf sigf("sigf", "signal", "TMath::Gamma((p3+1)/2)/TMath::Gamma(p3/2)/TMath::Sqrt(TMath::Pi()*p3)*TMath::Power(1.+((mLam-mu)/sigma)*((mLam-mu)/sigma)/p3,-(p3+1)/2)", RooArgSet(mLam, mu, p3, sigma));
	
	//
	// Doubel Guass
	// Create two Gaussian PDFs g1(x,mean1,sigma) anf g2(x,mean2,sigma) and their parameters
	RooRealVar mean("mean", "mean of gaussians", 1.1156);
	RooRealVar sigma1("sigma1", "width of gaussians", 0.0018);
	RooRealVar sigma2("sigma2", "width of gaussians", 0.0078);

	RooGaussian sig1("sig1", "Signal component 1", mLam, mean, sigma1);
	RooGaussian sig2("sig2", "Signal component 2", mLam, mean, sigma2);

	// Sum the signal components into a composite signal pdf
	RooRealVar sig1frac("sig1frac", "fraction of component 1 in signal", 0.8, 0., 1.);
	RooAddPdf sig("sig", "Signal", RooArgList(sig1, sig2), sig1frac);
	// 
	// Signal+Bg PDF
	////////////////////////////////////////////////////////////////////////////////////
	RooRealVar nsig("nsig", "# signal events", 1., 0, 100000.);
	RooRealVar nbkg("nbkg", "# background events", 1., 0., 100000.);
	RooAddPdf sum("sum", "signal+bg", RooArgList(sigf, bg), RooArgList(nsig, nbkg));
	//RooAddPdf sum("sum", "signal+bg", RooArgList(sigf, bg));
	
	/////////////////////////////////////////////////////////////////////////////////////
	/*RooRealVar bkgfrac("bkgfrac", "fraction of background", 0.5, 0., 1.);
	RooAddPdf sum("sum", "g1+g2+bg", RooArgList(bg, sig), bkgfrac);*/
	//////////////////////////////////////////////////////////////////////////////////////

	//Clone a hist from the input histogram then reset its data to clear
	TH2F* BGhist = (TH2F*)inHist->Clone();
	BGhist->Reset("ICESM");
	//MLP funciton takes a tuple input, will copy this tuple input into the histogram to use for functional fit
	TNtuple* ntp_bg = new TNtuple("datatree", "datatree", "Mass:Count");


	//Read in the bin center and bin content from the histogram and prepare the text file
	for (int i = 1; i < (int)inHist->GetNbinsX() + 1; i++) {

		double bcx = inHist->GetBinCenter(i);

		if (isinf(inHist->GetBinContent(i))) { inHist->SetBinContent(i, 0); }

		if ((bcx > leftfitbound1[particlecode] && bcx < leftfitbound2[particlecode]) || (bcx > rightfitbound1[particlecode] && bcx < rightfitbound2[particlecode])) {
			//ofl << bcx << "\t" << inHist->GetBinContent(i) << endl;
			ntp_bg->Fill(bcx, inHist->GetBinContent(i));
			BGhist->SetBinContent(i, inHist->GetBinContent(i));
		}
	}

	RooDataHist RooFitDataHist("RooFitDataHist", "RooFitDataHist", mLam, inHist);
	
	//RooFitDataHist.reduce(RooFit::CutRange("signal"));
	bg.fitTo(RooFitDataHist, RooFit::Range("left,right"), RooFit::Save());
	RooPlot* RooFitPlot = mLam.frame();
	RooFitDataHist.plotOn(RooFitPlot);
	
	//bg.plotOn(RooFitPlot, RooFit::Range("full"), RooFit::LineColor(kRed));
	
	//lock in the BG fit parameters now for the composite signal fit
	//a1.setConstant(kTRUE);
	//a2.setConstant(kTRUE);
	//nbkg.setConstant(kTRUE);
	
	//sigf.fitTo(RooFitDataHist, RooFit::Range("compositefitrange"), RooFit::Save());
	//sigf.plotOn(RooFitPlot, RooFit::Range("full"));
	//RooFitPlot->Write();
	//RooFitResult* result = sum.fitTo(RooFitDataHist, RooFit::Range("compositefitrange"), RooFit::Save());
	//RooFitResult* result = sum.fitTo(RooFitDataHist, RooFit::Range("compositefitrange"), RooFit::Save());
	

	//sum.fitTo(RooFitDataHist, RooFit::Range("compositefitrange"), RooFit::Save());
	//sum.fitTo(RooFitDataHist, RooFit::Range("full"), RooFit::Save());
	RooFitResult* result = sum.fitTo(RooFitDataHist, RooFit::Range("full"), RooFit::Save());
	
	double norm = sum.getNorm(RooArgSet(mLam));
	reject = false;
	//RooFitDataHist.reduce(RooFit::CutRange("full"));
	//TF1* bgfunc = (TF1*)bg.asTF(RooArgList(mLam), RooArgList(a1, a2), RooArgSet());

	//TF1* bgfunc = (TF1*)bg.asTF(mLam, RooArgList(a1, a2));
	//double norm = (1.0 / (bg.getNorm(xvalsleft) + bg.getNorm(xvalsright)));
	//double norm = (1.0 / (bg.getNorm(xvals)));
	//double norm = 1.0/bgDummy.getNorm(RooArgSet(mLamDummy));

	//double norm = 1;

	//sum.plotOn(RooFitPlot, RooFit::Range("full"));
	
	// Overlay the background component of model with a dashed line
	//sum.plotOn(RooFitPlot, RooFit::Components(bg), RooFit::Range("full"), RooFit::LineStyle(kDashed));

	//
	//bg.fitTo(RooFitDataHist, RooFit::Range("left,right"), RooFit::Save());
	//bg.plotOn(RooFitPlot, RooFit::Range("full"), RooFit::LineColor(kGreen));
	//
	sum.plotOn(RooFitPlot, RooFit::Components(bg), RooFit::Range("full"), RooFit::LineStyle(kDashed), RooFit::LineColor(kBlue));
	//bg.plotOn(RooFitPlot, RooFit::Range("full"), RooFit::LineStyle(kDashed), RooFit::LineColor(kYellow));
	sum.plotOn(RooFitPlot, RooFit::Range("full"), RooFit::LineColor(kBlue));
	//

	RooCurve* FitCurve = RooFitPlot->getCurve("sum");

	RooRealVar* var = (RooRealVar*)sum.getObservables(RooArgSet(mLam))->first();

	//cout << "Norm Variable: " << n << endl;
	//bg.plotOn(RooFitPlot, RooFit::Range("full"), RooFit::NormRange("left,right")); //This is equivalent to the line above, Norm range defaults to the range used for fitting
	//bg.plotOn(RooFitPlot, RooFit::Range("full"), RooFit::NormRange("full"));
	//cout << "a1: " << a1.getValV() << endl;
	double factor1 = RooFitPlot->getFitRangeNEvt();
	double BW = RooFitPlot->getFitRangeBinW();
	//TF1* f2 = new TF1("f2", flineLambda, 0, 2.5, 2);

	//cout << "Param1: " << f1->GetParameter(0) << endl;
	//cout << "Param2: " << f1->GetParameter(1) << endl;

	//c1->cd();
	//TMultiLayerPerceptron* mlp = new TMultiLayerPerceptron(network[particlecode].c_str(), ntp_bg); //Defining the architecture - 1 hidden layer with 100 nodes
	//mlp->Train(trainnum[particlecode], "graph, current, update=1000");

	//outputfile.cd();
	//c1->Write();
	//outputfile.Write();
	f->cd();

	TH1F* fgHist = (TH1F*)inHist->Clone();

	Double_t inputs[1];

	TH2F* fgData = (TH2F*)fgHist->Clone();
	fgData->Reset("ICESM");

	TH2F* signalHist = (TH2F*)fgHist->Clone();
	signalHist->Reset("ICESM");

	TH2F* fitHistsubrange = (TH2F*)fgHist->Clone();
	fitHistsubrange->Reset("ICESM");

	TH2F* checkHist = (TH2F*)inHist->Clone();
	checkHist->Reset("ICESM");
	checkHist->SetLineColor(kRed);

	TH2F* xaxis = (TH2F*)fgHist->Clone();
	xaxis->Reset("ICESM");
	xaxis->SetLineColor(kBlack);


	TH2F* fitHist = (TH2F*)fgHist->Clone();
	fitHist->Reset("ICESM");

	fgHist->SetLineColor(kBlack);
	signalHist->SetLineColor(kRed);
	fitHist->SetLineColor(kMagenta);
	fgData->SetLineColor(kBlack);
	fitHistsubrange->SetLineColor(kRed);

	double kcount = 0;
	//double kcounterror = 0;
	double bgcount = 0;
	double totalcountwithbg = 0;
	double datacheck = 0;

	///////////////////////HHHHEEEERRREE
	mLam.setRange("bin", 1.1070, 1.1075);
	//std::unique_ptr<RooAbsReal> bgundersig{ bgnorm.createIntegral(mLam, RooFit::NormSet(mLam), RooFit::Range("signal")) }
	//RooAbsReal ig = signal.createIntegral(mLam, mLam, "signal");
	//std::unique_ptr<RooAbsReal> ig{ sigf.createIntegral(mLam, RooFit::NormSet(mLam), RooFit::Range("signal")) };
	//std::unique_ptr<RooAbsReal> ig2{ bg.createIntegral(mLam, RooFit::NormSet(mLam), RooFit::Range("signal")) };
	//std::unique_ptr<RooAbsReal> ig3{ sum.createIntegral(mLam, RooFit::NormSet(mLam), RooFit::Range("signal")) };
	std::unique_ptr<RooAbsReal> ig{ sigf.createIntegral(mLam, RooFit::NormSet(mLam), RooFit::Range("bin")) };
	std::unique_ptr<RooAbsReal> ig2{ bg.createIntegral(mLam, RooFit::NormSet(mLam), RooFit::Range("bin")) };
	std::unique_ptr<RooAbsReal> ig3{ sum.createIntegral(mLam, RooFit::NormSet(mLam), RooFit::Range("bin")) };
	//double ntot = sum.expectedEvents(RooArgSet(mLam));  //this should be nsig + nbkg
	//double ntot = RooFitDataHist.sumEntries(0,"signal");
	double ntot = RooFitDataHist.sumEntries(0, "bin");//number of events in this bin
	cout << "ntot: " << ntot << endl;
	cout << "Nsig: " << nsig.getVal() << endl;
	cout << "Nbkg: " << nbkg.getVal() << endl;
	cout << "Nsig + Nbkg: " << nsig.getVal() + nbkg.getVal() << endl;
	cout << "SigF RAW: " << ig->getVal() << endl;
	cout << "BG RAW: " << ig2->getVal() << endl;
	cout << "Sum RAW: " << ig3->getVal() << endl;
	cout << "SigF Count: " << ig->getVal() * nsig.getVal() << endl;
	cout << "BG Count: " << ig2->getVal() * nbkg.getVal() << endl;
	cout << "Sum Count: " << ig3->getVal() * (nbkg.getVal() + nsig.getVal()) << endl;
	//cout << "Sum Count: " << ig3->getVal() * (nbkg.getVal() + nsig.getVal()) << " +/- " << ig->getPropagatedError(sigf, mLam) * nsig.getVal() << endl;
	//std::unique_ptr<RooAbsReal> bgundersig{ bgnorm.createIntegral(mLam, RooFit::Range("signal")) };
	//double BGVALINT = ig->getVal() * ntot;
	RooAbsReal* icheck = bg.createIntegral(mLam, RooFit::NormSet(mLam), RooFit::Range("signal"));

	double SIGNALINT = ig->getVal() * ntot;
	double lowerbound = 0;
	double upperbound = 0;
	double pdfbinintval = 0;
	for (int i = 1; i < fgHist->GetNbinsX() + 1; i++) {

		inputs[0] = ((TAxis*)fgHist->GetXaxis())->GetBinCenter(i);
		fgData->SetBinContent(i, fgHist->GetBinContent(i));
		fgData->SetBinError(i, fgHist->GetBinError(i));
		//Integrating the PDF fits bin by bin and checking
		lowerbound = inputs[0] - (fgHist->GetBinWidth(i)) / 2;
		upperbound = inputs[0] + (fgHist->GetBinWidth(i)) / 2;
		//std::unique_ptr<RooAbsReal> igCHECK{ sum.createIntegral(mLam, RooFit::NormSet(mLam), RooFit::Range(lowerbound,upperbound)) };
		//pdfbinintval = igCHECK->getVal() * nbkg.getVal();
		//checkHist->SetBinContent(i, pdfbinintval);
		//checkHist->SetBinContent(i, 100);

		//If within signal extraction range defined by s1 and s2 then copy data to this hist
		if ((inputs[0] >= s1 && inputs[0] < s2)) {
			fitHistsubrange->SetBinContent(i, fgHist->GetBinContent(i));
			//Integrating the PDF fits bin by bin and checking
			lowerbound = checkHist->GetBinLowEdge(i);
			upperbound = lowerbound + checkHist->GetBinWidth(i);
			//icheck->Set
			//pdfbinintval = igCHECK->getVal() * nbkg.getVal();
			checkHist->SetBinContent(i, pdfbinintval);
			cout << "Lower bound: " << lowerbound << endl;
			cout << "Upper bound: " << upperbound << endl;
			//cout << "IG BIN VAL: " << igCHECK->getVal() << endl;
			cout << "pdfbinintval: " << pdfbinintval << endl;
			//delete igCHECK;
			//checkHist->SetBinContent(i, 100);
		}

		if ((inputs[0] >= x1 && inputs[0] < x2) || (inputs[0] >= x3 && inputs[0] < x4)) {
			//HERE//fitHist->SetBinContent(i, (fPolyLambdaVal(inputs[0],f1->GetParameters())));
			datacheck = datacheck + inHist->GetBinContent(i);
			//checkHist->SetBinContent(i, inHist->GetBinContent(i));
			
		}

		if (inputs[0] >= leftsubbound[particlecode] && inputs[0] < rightsubbound[particlecode]) {
			//mLam.setRange("bin", inputs[0]-BW/2, inputs[0] + BW/2);
			mLam.setVal(inputs[0]);//test
			//double temp_content = fgHist->GetBinContent(i) - bg.getVal(mLam);
			double temp_content = 0;
			double BGVAL = 0;
			//double BGVAL = bgfunc->Eval(inputs[0]);
			//double BGVAL = sum.getVal(mLam) * (inputs[0]) * BW*n.getVal();
			std::unique_ptr<RooAbsReal> binintegral{ sum.createIntegral(mLam, RooFit::NormSet(mLam), RooFit::Range("bin")) };
			//double BGVAL = binintegral->getVal();
			//double temp_content = fgHist->GetBinContent(i) - BGVAL;// *norm;
			//cout << "Data: " << fgHist->GetBinContent(i) << endl;
			//cout << "mLam: " << inputs[0] << endl;
			//cout << "a1: " << a1.getValV() << endl;
			//cout << "a2: " << a2.getValV() << endl;
			//cout << "BG: " << bgfunc->Eval(inputs[0]) << endl;
			//cout << "Norm: " << norm << endl;
			//cout << "Signal: " << temp_content << endl;
			signalHist->SetBinContent(i, temp_content);

			//fitHist->SetBinContent(i, mlp->Evaluate(0, inputs));
			signalHist->SetBinError(i, fgHist->GetBinError(i));
			//fitHistsubrange->SetBinContent(i, mlp->Evaluate(0, inputs));
			//fitHistsubrange->SetBinContent(i, BGVAL);// *(norm);
			//fitHistsubrange->SetBinContent(i, bg.getVal(mLam));
			totalcountwithbg = totalcountwithbg + fgHist->GetBinContent(i);
			//kcounterror = kcounterror + fgHist->GetBinError(i);
			//if (temp_content > 0) {
			kcount = kcount + temp_content;
			//bgcount = bgcount + mlp->Evaluate(0, inputs);
			//bgcount = bgcount + (bgfunc->Eval(inputs[0]));// *(norm);
			bgcount = bgcount + BGVAL;
			//}

		}

	}
	cout << "DATA CHECK" << datacheck << endl;
	cout << "Range CHECK" << (x2 - x1) + (x4 - x3) << endl;
	double kcounterror = sqrt(totalcountwithbg);

	//Draw MLP Fit
	c3->cd();
	fgHist->GetXaxis()->SetTitle("Mass [Gev]");
	fgHist->GetYaxis()->SetTitle("Count");
	fgHist->SetNameTitle(plotname.c_str(), plotname.c_str());
	fgHist->Draw();

	//HERE//f1->Draw("same");
	RooFitPlot->Draw("same");
	//signalHist->Draw("same");
	fitHist->Draw("same");
	checkHist->Draw("same");
	//fitHistsubrange->Draw("same");
	xaxis->Draw("same");
	extractionbound1line.Draw("same");
	extractionbound2line.Draw("same");
	leftfitbound1line.Draw("same");
	leftfitbound2line.Draw("same");
	rightfitbound1line.Draw("same");
	rightfitbound2line.Draw("same");

	TLegend* legend = new TLegend(0.5513, 0.4144, 0.888, 0.8823);
	//legend->SetHeader(Form("#splitline{Count- %.2f #pm %.2f (stat.)}{Stochastic MLP- %s Network Trained %ix}", kcount, kcounterror, network[particlecode].c_str(), trainnum[particlecode])); // option "C" allows to center the header
	if (particlecode == 0) {
		//f1->SetFunction(flineLambda);
		//HERE//legend->SetHeader(Form("#splitline{Count: %.2f #pm %.2f (stat.)}{Linear Fit: Ax^2+Bx+C, A = %.2f #pm %.2f, B = %.2f #pm %.2f, C = %.2f #pm %.2f}", kcount, kcounterror, f1->GetParameter(2), f1->GetParError(2), f1->GetParameter(1), f1->GetParError(1), f1->GetParameter(0), f1->GetParError(0))); // option "C" allows to center the header
	}
	if (particlecode == 1) {
		//f1->SetFunction(flineK0);
		//HERE//legend->SetHeader(Form("#splitline{Count: %.2f #pm %.2f (stat.)}{Linear Fit: Ax+B, A = %.2f #pm %.2f, B = %.2f #pm %.2f}", kcount, kcounterror, f1->GetParameter(1), f1->GetParError(1), f1->GetParameter(0), f1->GetParError(0))); // option "C" allows to center the header
	}
	//legend->SetHeader(Form("#splitline{Count: %.2f #pm %.2f (stat.)}{Linear Fit: Ax+B, A = %.2f #pm %.2f, B = %.2f #pm %.2f}", kcount, kcounterror, f1->GetParameter(0), f1->GetParError(0), f1->GetParameter(1), f1->GetParError(1))); // option "C" allows to center the header
	legend->AddEntry("RooFitDataHist", "Data");
	//legend->AddEntry(fitHist, "Fit Accross Data BG", "l");
	legend->AddEntry("sum", "Signal+BG Fit");
	legend->AddEntry("extractionbound1line", "Signal Extraction Range", "l");
	legend->AddEntry("rightfitbound1line", "Left and Right BG Fit Ranges", "l");
	//legend->Draw("same");
	//f1->Draw("same");

	outputfile.cd();
	c3->Write();
	//RooFitPlot->Write();

	//Draw Line Fit
	//c3->cd();
	//fgHist->Draw();
	//BGhist->Draw("same");
	//fgHist->Draw("same");
	//f1->Draw("same");
	//outputfile.cd();
	//c3->Write();

	outputfile.Write();
	f->cd();



	//delete training data after each iteration
	ntp_bg->Reset();
	//return { kcount,kcounterror, bgcount };
	//return { totalcountwithbg - BGVALINT ,kcounterror, bgcount };
	return { SIGNALINT ,kcounterror, bgcount };
};


//This function takes in raw tree from TTree input files and creates histograms with appropriate binning for peak extractions
//after it has all histrograms for appropriate bins, it extracts counts with mlpfunciton, output is a vector of mlpfunciton vector outputs
vector<vector<double>> AnalyzeTree(int distvariableindex, TFile* Data, string TreeName, int binentries, int particlecode, vector<TH1F*> HistArray, vector<double> binsvector, string extractionReadback, TCanvas* c3, TCanvas* c1, vector<vector<double>> binparaminits, vector<vector<double>> binrangedefs) { //when calling function, particle code deictates 0=lambda or 1=kaon MLP parameters
	vector<vector<double>> ExtractionDataVector = {};

	//have files for lambdaTree, kaonTree, lambdaTreeMC, kaonTreeMC, genMCTree
	//TFile* Data = new TFile(filelambdaTree.c_str(), "read");
	//grab Tree from file above
	TTree* tree = (TTree*)Data->Get(TreeName.c_str());

	int entries;

	entries = tree->GetEntries();

	//Defining the variables that are stored in the Tree branches
	//double invMasslambda, invMasskaon, pT, Eta, x, Q2s, Ys;
	double invMass, pT, Eta, x, Q2s, Ys, Angle, weight1, weight2, Phi, SVx, SVy, SVz;
	int s61;
	string invMassName[2] = {"invMasslambda","invMasskaon"};
	vector<double> distvariable = { 0,0,0,0,0,0,0 };
	//Defining the branch addresses for the Trees to access these stored variables
	tree->SetBranchAddress(invMassName[particlecode].c_str(), &invMass);
	tree->SetBranchAddress("pT", &pT);
	tree->SetBranchAddress("Eta", &Eta);
	tree->SetBranchAddress("x", &x);
	tree->SetBranchAddress("Q2s", &Q2s);
	tree->SetBranchAddress("Ys", &Ys);
	//tree->SetBranchAddress("Angle", &Angle);
	tree->SetBranchAddress("weight1", &weight1);
	tree->SetBranchAddress("weight2", &weight2);
	tree->SetBranchAddress("s61", &s61);
	tree->SetBranchAddress("Phi", &Phi);
	tree->SetBranchAddress("SVx", &SVx);
	tree->SetBranchAddress("SVy", &SVy);
	tree->SetBranchAddress("SVz", &SVz);
	int m;
	int j;
	//Have to enforce pT and Eta bounds here
	for (m = 0; m < entries; m++) {
		tree->GetEntry(m);
		for (j = 0; j < binentries; j++) {
			if(s61>0){
				if (pT > 0.5 && pT < 3.5) {
					if (abs(Eta) < 1.3) {
						if (Ys > 0.1 && Ys < 0.6) {
							if (Q2s > 5 && Q2s < 100) {
								distvariable = { Q2s,Q2s / (Ys * 101568),pT,Eta, Phi,sqrt((SVx)*(SVx) + (SVy)*(SVy)), SVz };
								if (abs(SVz) <= 30.0) {
									if (distvariable[distvariableindex] > binsvector[j] && distvariable[distvariableindex] < binsvector[j + 1]) {
										HistArray[j]->Fill(invMass, weight1 * weight2);
									}
								}
							}
						}
					}
				}
			}
		}
	}

	for (int j = 0; j < binentries; j++) {
		//Read in histogram of interest
		//TH1F* inHist = (TH1F*)f3->Get(Peaks2[j].c_str());
		//mlpfunction(inHist, PlotNames[j]);
		//mlpfunction writes training plot and extraction plot to external root file, then outputs count and statistical count error values
		cout << extractionReadback << j + 1 << " of " << binentries << endl;
		string trainingnamestr = "Training " + extractionReadback + to_string(j + 1);
		string extractionnamestr = extractionReadback + to_string(j + 1);
		c3->SetName(extractionnamestr.c_str());
		c1->SetName(trainingnamestr.c_str());
		ExtractionDataVector.push_back(mlpfunction(HistArray[j], extractionReadback.c_str() + to_string(j + 1), particlecode, c3, c1, binparaminits[j], binrangedefs[j]));
	}
	return ExtractionDataVector;

};

//This takes the raw gen MC trees and gets MC total counts for efficiency calculation in the appropraiate bin defined by inputs, function output is the count
vector<double> AnalyzeGenTree(int distvariableindex, TFile* Data, string TreeName, int binentries, vector<double> binsvector) { //when calling function, particle code deictates 0=lambda or 1=kaon MLP parameters

	vector<double> GenCounts = {};
	//initialize all GenCounts for each bin to zero in the array
	int p;
	for (p = 0; p < binentries; p++) {
		GenCounts.push_back(0);
	}

	//have files for lambdaTree, kaonTree, lambdaTreeMC, kaonTreeMC, genMCTree
	//TFile* Data = new TFile(filelambdaTree.c_str(), "read");
	//grab Tree from file above
	TTree* tree = (TTree*)Data->Get(TreeName.c_str());
	int entries;

	entries = tree->GetEntries();

	//Defining the variables that are stored in the Tree branches
	//double invMasslambda, invMasskaon, pT, Eta, x, Q2s, Ys;
	double pT, Eta, x, Q2s, Ys, weight1, weight2, Empz, GenElecEn, VtxZgen, genPhi, DecayZVtx, DecayXVtx, DecayYVtx;

	//Defining the branch addresses for the Trees to access these stored variables
	//tree->SetBranchAddress("invMass", &invMass);
	tree->SetBranchAddress("pT", &pT);
	tree->SetBranchAddress("Eta", &Eta);
	tree->SetBranchAddress("x", &x);
	tree->SetBranchAddress("Q2s", &Q2s);
	tree->SetBranchAddress("Ys", &Ys);
	tree->SetBranchAddress("weight1", &weight1);
	tree->SetBranchAddress("weight2", &weight2);
	tree->SetBranchAddress("Empz", &Empz);
	tree->SetBranchAddress("GenElecEn", &GenElecEn);
	//tree->SetBranchAddress("H1GenBool", &H1GenBool);
	tree->SetBranchAddress("VtxZgen", &VtxZgen);
	tree->SetBranchAddress("genPhi", &genPhi);
	tree->SetBranchAddress("DecayZVtx", &DecayZVtx);
	tree->SetBranchAddress("DecayXVtx", &DecayXVtx);
	tree->SetBranchAddress("DecayYVtx", &DecayYVtx);

	vector<double> distvariable = { 0,0,0,0,0,0,0 };
	int m;
	int j;
	for (m = 0; m < entries; m++) {
		tree->GetEntry(m);
		for (j = 0; j < binentries; j++) {
			if (pT > 0.5 && pT < 3.5) {
				if (abs(Eta) < 1.3) {
					if (Ys > 0.1 && Ys < 0.6) {
						if (Q2s > 5 && Q2s < 100) {
							//if (GenElecEn>11) {
								if (Empz>35 && Empz<70) {
									if (abs(VtxZgen)<30) {
										if (abs(DecayZVtx) < 30.0) {
											distvariable = { Q2s, Q2s / (Ys * 101568), pT, Eta, genPhi, sqrt((DecayXVtx) * (DecayXVtx)+(DecayYVtx) * (DecayYVtx)), DecayZVtx };
											if (distvariable[distvariableindex] > binsvector[j] && distvariable[distvariableindex] < binsvector[j + 1]) {
												GenCounts[j] = GenCounts[j] + 1 * weight1 * weight2;
											}
										}
									}
								}
							//}
						}
					}
				}
			}
		}
	}

	return GenCounts;

};

vector<double> AnalyzeGenTreeRadCorr(int distvariableindex, TFile* Data, string TreeName, int binentries, vector<double> binsvector) { //when calling function, particle code deictates 0=lambda or 1=kaon MLP parameters

	vector<double> GenCounts = {};
	//initialize all GenCounts for each bin to zero in the array
	int p;
	for (p = 0; p < binentries; p++) {
		GenCounts.push_back(0);
	}

	//have files for lambdaTree, kaonTree, lambdaTreeMC, kaonTreeMC, genMCTree
	//TFile* Data = new TFile(filelambdaTree.c_str(), "read");
	//grab Tree from file above
	TTree* tree = (TTree*)Data->Get(TreeName.c_str());
	int entries;

	entries = tree->GetEntries();

	//Defining the variables that are stored in the Tree branches
	//double invMasslambda, invMasskaon, pT, Eta, x, Q2s, Ys;
	double pT, Eta, x, Q2s, Ys, weight1, weight2, Empz, GenElecEn, H1GenBool, VtxZgen, genPhi, DecayZVtx, DecayXVtx, DecayYVtx;

	//Defining the branch addresses for the Trees to access these stored variables
	//tree->SetBranchAddress("invMass", &invMass);
	tree->SetBranchAddress("pT", &pT);
	tree->SetBranchAddress("Eta", &Eta);
	tree->SetBranchAddress("x", &x);
	tree->SetBranchAddress("Q2s", &Q2s);
	tree->SetBranchAddress("Ys", &Ys);
	tree->SetBranchAddress("weight1", &weight1);
	tree->SetBranchAddress("weight2", &weight2);
	tree->SetBranchAddress("Empz", &Empz);
	tree->SetBranchAddress("GenElecEn", &GenElecEn);
	tree->SetBranchAddress("H1GenBool", &H1GenBool);
	tree->SetBranchAddress("VtxZgen", &VtxZgen);
	tree->SetBranchAddress("genPhi", &genPhi);
	tree->SetBranchAddress("DecayZVtx", &DecayZVtx);
	tree->SetBranchAddress("DecayXVtx", &DecayXVtx);
	tree->SetBranchAddress("DecayYVtx", &DecayYVtx);
	vector<double> distvariable = { 0,0,0,0,0,0,0 };
	int m;
	int j;
	for (m = 0; m < entries; m++) {
		tree->GetEntry(m);
		for (j = 0; j < binentries; j++) {
			if (pT > 0.5 && pT < 3.5) {
				if (abs(Eta) < 1.3) {
					if (Ys > 0.1 && Ys < 0.6) {
						if (Q2s > 5 && Q2s < 100) {
							if (H1GenBool == 1) {
								if (abs(VtxZgen)<30) {
									if(GenElecEn>11){
										if (Empz > 35 && Empz < 70) {
											distvariable = {Q2s,Q2s / (Ys * 101568),pT,Eta, genPhi, sqrt((DecayXVtx)*(DecayXVtx)+(DecayYVtx)*(DecayYVtx)), DecayZVtx};
											if (distvariable[distvariableindex] > binsvector[j] && distvariable[distvariableindex] < binsvector[j + 1]) {
												GenCounts[j] = GenCounts[j] + 1 * weight1 * weight2;
											}
										}
								    }
								}
							}
						}
					}
				}
			}
		}
	}

	return GenCounts;

};

//Function plots data
int PlotData(string plotcanvasname, string plottitle, string xaxistitle, string yaxistitle, double ylowbound, double yupbound, TGraphErrors* cs, int binentries, double X[binentries], double y[binentries], double xerr[binentries], double y1err[binentries], double dylow[binentries], double dyup[binentries], double dylow2[binentries], double dyup2[binentries]) { //when calling function, particle code deictates 0=lambda or 1=kaon MLP parameters

	TCanvas* c5 = new TCanvas(plotcanvasname.c_str(), plotcanvasname.c_str(), 10, 10, 800, 400);
	TGraphAsymmErrors* xyscan = new TGraphAsymmErrors(binentries, X, y, xerr, xerr, dylow, dyup);
	TGraphAsymmErrors* xyscan2 = new TGraphAsymmErrors(binentries, X, y, xerr, xerr, dylow2, dyup2);
	xyscan->SetMarkerColor(kBlack);
	xyscan->SetLineColor(kBlack);
	xyscan2->SetMarkerColor(kBlack);
	xyscan2->SetLineColor(kBlack);
	//auto cs = new TGraphErrors(pTentries, X, y1, xerr, y1err);
	//auto cs = new TGraphErrors(binentries, X, y1, xerr, y1err);
	cs->SetMarkerSize(0.01);
	xyscan->SetMarkerSize(0.01);
	xyscan->SetMarkerStyle(21);
	xyscan2->SetMarkerSize(0.01);
	xyscan2->SetMarkerStyle(21);
	cs->SetMarkerStyle(21);
	cs->SetMarkerColor(kRed);
	cs->SetLineColor(kRed);
	TMultiGraph* mgcs = new TMultiGraph();

	mgcs->Add(cs, "AP");
	//mgcs->Add(xyscan, "AP");
	//mgcs->Add(xyscan2, "AP");
	cs->Draw("AP");
	cs->GetYaxis()->SetRange(ylowbound, yupbound);
	//MCcrosssection->Draw("AP");
	mgcs->Draw("SAME");
	mgcs->SetTitle(plottitle.c_str());
	mgcs->GetXaxis()->SetTitle(xaxistitle.c_str());
	mgcs->GetYaxis()->SetTitle(yaxistitle.c_str());
	//MCcrosssection->Draw("SAME");
	TLegend* legend2 = new TLegend();
	//legend2->AddEntry(MCcrosssection, "RAPGAP31", "l");
	legend2->AddEntry(cs, "2005-2007 HERAII Data", "l");
	//legend2->AddEntry(cs, "2007 HERAII Data", "l");
	//legend2->AddEntry(xyscan, "1999-2000 H1 Analysis", "l");
	legend2->Draw("SAME");
	c5->SetGridy();
	outputfile.cd();
	cs->SetName((plotcanvasname + " TGraph").c_str());
	cs->Write();
	c5->Write();
	f->cd();

	//c5->Clear();
	//c5->SetName("RAPGAP Lambda Cross Section");
	//MCcrosssection->Draw();
	//c5->Write();

	
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	return 0;

};


//End of MLP function and inputs, actual analysis code follows
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



void Asym(int varidx, vector<double> q2bins, double DataIntLumi06, double MCDjangoh14radgen, double MCDjangoh14nonradgen, string filekaonTree, string filelambdaTree, string filekaonTreeMC, string filelambdaTreeMCRAPGAP, string filelambdaTreeMCDJANGOH, string filegenMCTreeRAPGAP, string filegenMCTreeDJANGOH, string fileNonRadgenMCTree, string fileRadgenMCTree, string xaxistitle, vector<vector<double>> binparaminits, vector<vector<double>> binrangedefs, vector<vector<double>> binparaminitsMC, vector<vector<double>> binrangedefsMC) {

	cout << "Proton Mass: " << mProton << endl;
	cout << "Pion Mass: " << mPion << endl;

	double sigmaData = 0.0018;
	double sigmaMC = 0.001;
	double paramData = 5;
	double paramMC = 8;

	//defining arrays of histograms corresponding to particles within different kinematic bins
	//vector<TH1F*> LambdaTotDataQ2;
	vector<TH1F*> LambdaRegDataQ2;
	vector<TH1F*> LambdaBarDataQ2;
	//vector<TH1F*> LambdaTotMCRAPGAPQ2;
	vector<TH1F*> LambdaRegMCRAPGAPQ2;
	vector<TH1F*> LambdaBarMCRAPGAPQ2;
	//vector<TH1F*> LambdaTotMCDJANGOHQ2;
	vector<TH1F*> LambdaRegMCDJANGOHQ2;
	vector<TH1F*> LambdaBarMCDJANGOHQ2;
	//vector<TH1F*> LambdaDJANGOHBG;
	//vector<TH1F*> LambdaRAPGAPBG;

	//I don't have Histograms for particle level generator data because counts for these
	//will be done by just going through the generate TTree and adding events up that are within our conditions
	for (int a = 0; a < q2entries; a++) {
		//string namestr1 = "Lambda Tot Data q2 Bin " + to_string(a + 1);
		string namestr2 = "Lambda Reg Data q2 Bin " + to_string(a + 1);
		string namestr3 = "Lambda Bar Data q2 Bin " + to_string(a + 1);
		//string namestr4 = "Lambda Tot MC RAPGAP q2 Bin " + to_string(a + 1);
		string namestr5 = "Lambda Reg MC RAPGAP q2 Bin " + to_string(a + 1);
		string namestr6 = "Lambda Bar MC RAPGAP q2 Bin " + to_string(a + 1);
		//string namestr7 = "Lambda Tot MC DJANGOH q2 Bin " + to_string(a + 1);
		string namestr8 = "Lambda Reg MC DJANGOH q2 Bin " + to_string(a + 1);
		string namestr9 = "Lambda Bar MC DJANGOH q2 Bin " + to_string(a + 1);
		//string namestr10 = "Lambda DJANGOH BG q2 Bin " + to_string(a + 1);
		//string namestr11 = "Lambda RAPGAP BG q2 Bin " + to_string(a + 1);
		
		
		//LambdaTotDatapT[a] = new TH1F(namestr1.c_str(), namestr1.c_str(), lambdabins, lambdalowerbinrange, lambdaupperbinrange);
		//LambdaTotDataQ2.push_back(new TH1F(namestr1.c_str(), namestr1.c_str(), lambdabins, lambdalowerbinrange, lambdaupperbinrange));
		LambdaRegDataQ2.push_back(new TH1F(namestr2.c_str(), namestr2.c_str(), lambdabins, lambdalowerbinrange, lambdaupperbinrange));
		LambdaBarDataQ2.push_back(new TH1F(namestr3.c_str(), namestr3.c_str(), lambdabins, lambdalowerbinrange, lambdaupperbinrange));
		//LambdaTotMCRAPGAPQ2.push_back(new TH1F(namestr4.c_str(), namestr4.c_str(), lambdabins, lambdalowerbinrange, lambdaupperbinrange));
		LambdaRegMCRAPGAPQ2.push_back(new TH1F(namestr5.c_str(), namestr5.c_str(), lambdabins, lambdalowerbinrange, lambdaupperbinrange));
		LambdaBarMCRAPGAPQ2.push_back(new TH1F(namestr6.c_str(), namestr6.c_str(), lambdabins, lambdalowerbinrange, lambdaupperbinrange));
		//LambdaTotMCDJANGOHQ2.push_back(new TH1F(namestr7.c_str(), namestr7.c_str(), lambdabins, lambdalowerbinrange, lambdaupperbinrange));
		LambdaRegMCDJANGOHQ2.push_back(new TH1F(namestr8.c_str(), namestr8.c_str(), lambdabins, lambdalowerbinrange, lambdaupperbinrange));
		LambdaBarMCDJANGOHQ2.push_back(new TH1F(namestr9.c_str(), namestr9.c_str(), lambdabins, lambdalowerbinrange, lambdaupperbinrange));
		//LambdaDJANGOHBG.push_back(new TH1F(namestr10.c_str(), namestr10.c_str(), lambdabins, lambdalowerbinrange, lambdaupperbinrange));
		//LambdaRAPGAPBG.push_back(new TH1F(namestr11.c_str(), namestr11.c_str(), lambdabins, lambdalowerbinrange, lambdaupperbinrange));
	}


	//
	//Space here for doing similar code as above for the other three variables used for binning
	//


	//do this after creating histograms
	//Creating the canvases used for storing MLP fit plots, good for troubleshooting
	TCanvas* c3 = new TCanvas("Lambda Extraction", "Lambda Extraction", 10, 10, 800, 400);
	TCanvas* c1 = new TCanvas("Training", "Training", 400, 400);
	
	//Defining vectors now that will contain the count and count error data for each paticle at MC, Data, and gen level (gen level has no count error, just count)
	//These vectors will be for all counting binning for all variables, can just clear them inbetween uses
	//vector<vector<double>> LambdaTotDataCount;
	vector<vector<double>> LambdaRegDataCount;
	vector<vector<double>> LambdaBarDataCount;
	//vector<vector<double>> LambdaTotMCRAPGAPCount;
	vector<vector<double>> LambdaRegMCRAPGAPCount;
	vector<vector<double>> LambdaBarMCRAPGAPCount;
	//vector<vector<double>> LambdaTotMCDJANGOHCount;
	vector<vector<double>> LambdaRegMCDJANGOHCount;
	vector<vector<double>> LambdaBarMCDJANGOHCount;
	
	//Gen particle count
	//vector<double> LambdaTotGenCountRAPGAP;
	vector<double> LambdaRegGenCountRAPGAP;
	vector<double> LambdaBarGenCountRAPGAP;
	//vector<double> LambdaTotGenCountDJANGOH;
	vector<double> LambdaRegGenCountDJANGOH;
	vector<double> LambdaBarGenCountDJANGOH;

	vector<double> LambdaRegBGGenCountDJANGOH;
	vector<double> LambdaRegBGGenCountRAPGAP;
	vector<double> LambdaBarBGGenCountDJANGOH;
	vector<double> LambdaBarBGGenCountRAPGAP;
	//Gen particle counting for Rad Corrections
	//vector<double> LambdaGenCountRad;
	//vector<double> LambdaGenCountNonRad;

	//Analyzing Tot Lambda Counts for q2 bins for MC and Data
	//LambdaTotDataCount = AnalyzeTree(varidx, new TFile(filelambdaTree.c_str(), "read"), "Lambda Tot", q2entries, 0, LambdaTotDataQ2, q2bins, "Lambda Tot Data q2 Extraction ", c3, c1, sigmaData, paramData);
	//LambdaTotMCRAPGAPCount = AnalyzeTree(varidx, new TFile(filelambdaTreeMCRAPGAP.c_str(), "read"), "Lambda Tot", q2entries, 0, LambdaTotMCRAPGAPQ2, q2bins, "Lambda Tot MC RAPGAP q2 Extraction ", c3, c1, sigmaMC, paramMC);
	//LambdaTotMCDJANGOHCount = AnalyzeTree(varidx, new TFile(filelambdaTreeMCDJANGOH.c_str(), "read"), "Lambda Tot", q2entries, 0, LambdaTotMCDJANGOHQ2, q2bins, "Lambda Tot MC DJANGOH q2 Extraction ", c3, c1, sigmaMC, paramMC);

	LambdaRegDataCount = AnalyzeTree(varidx, new TFile(filelambdaTree.c_str(), "read"), "Lambda Reg", q2entries, 0, LambdaRegDataQ2, q2bins, "Lambda Reg Data q2 Extraction ", c3, c1, binparaminits, binrangedefs);
	LambdaRegMCRAPGAPCount = AnalyzeTree(varidx, new TFile(filelambdaTreeMCRAPGAP.c_str(), "read"), "Lambda Reg", q2entries, 0, LambdaRegMCRAPGAPQ2, q2bins, "Lambda Reg MC RAPGAP q2 Extraction ", c3, c1, binparaminitsMC, binrangedefsMC);
	LambdaRegMCDJANGOHCount = AnalyzeTree(varidx, new TFile(filelambdaTreeMCDJANGOH.c_str(), "read"), "Lambda Reg", q2entries, 0, LambdaRegMCDJANGOHQ2, q2bins, "Lambda Reg MC DJANGOH q2 Extraction ", c3, c1, binparaminitsMC, binrangedefsMC);

	LambdaBarDataCount = AnalyzeTree(varidx, new TFile(filelambdaTree.c_str(), "read"), "Lambda Bar", q2entries, 0, LambdaBarDataQ2, q2bins, "Lambda Bar Data q2 Extraction ", c3, c1, binparaminits, binrangedefs);
	LambdaBarMCRAPGAPCount = AnalyzeTree(varidx, new TFile(filelambdaTreeMCRAPGAP.c_str(), "read"), "Lambda Bar", q2entries, 0, LambdaBarMCRAPGAPQ2, q2bins, "Lambda Bar MC RAPGAP q2 Extraction ", c3, c1, binparaminitsMC, binrangedefsMC);
	LambdaBarMCDJANGOHCount = AnalyzeTree(varidx, new TFile(filelambdaTreeMCDJANGOH.c_str(), "read"), "Lambda Bar", q2entries, 0, LambdaBarMCDJANGOHQ2, q2bins, "Lambda Bar MC DJANGOH q2 Extraction ", c3, c1, binparaminitsMC, binrangedefsMC);


	//Analyzing Gen Counts
	//LambdaTotGenCountRAPGAP = AnalyzeGenTree(varidx, new TFile(filegenMCTreeRAPGAP.c_str(), "read"), "Lambda Tot Channel", q2entries, q2bins);
	LambdaRegGenCountRAPGAP = AnalyzeGenTree(varidx, new TFile(filegenMCTreeRAPGAP.c_str(), "read"), "Lambda Reg Channel", q2entries, q2bins);
	LambdaBarGenCountRAPGAP = AnalyzeGenTree(varidx, new TFile(filegenMCTreeRAPGAP.c_str(), "read"), "Lambda Bar Channel", q2entries, q2bins);
	//LambdaTotGenCountDJANGOH = AnalyzeGenTree(varidx, new TFile(filegenMCTreeDJANGOH.c_str(), "read"), "Lambda Tot Channel", q2entries, q2bins);
	LambdaRegGenCountDJANGOH = AnalyzeGenTree(varidx, new TFile(filegenMCTreeDJANGOH.c_str(), "read"), "Lambda Reg Channel", q2entries, q2bins);
	LambdaBarGenCountDJANGOH = AnalyzeGenTree(varidx, new TFile(filegenMCTreeDJANGOH.c_str(), "read"), "Lambda Bar Channel", q2entries, q2bins);

	LambdaRegBGGenCountDJANGOH = AnalyzeGenTree(varidx, new TFile(filegenMCTreeDJANGOH.c_str(), "read"), "Lambda Reg BG", q2entries, q2bins);
	LambdaRegBGGenCountRAPGAP = AnalyzeGenTree(varidx, new TFile(filegenMCTreeRAPGAP.c_str(), "read"), "Lambda Reg BG", q2entries, q2bins);
	LambdaBarBGGenCountDJANGOH = AnalyzeGenTree(varidx, new TFile(filegenMCTreeDJANGOH.c_str(), "read"), "Lambda Bar BG", q2entries, q2bins);
	LambdaBarBGGenCountRAPGAP = AnalyzeGenTree(varidx, new TFile(filegenMCTreeRAPGAP.c_str(), "read"), "Lambda Bar BG", q2entries, q2bins);
	
	//LambdaGenCountRad = AnalyzeGenTreeRadCorr(varidx, new TFile(fileRadgenMCTree.c_str(), "read"), "Lambda Tot", q2entries, q2bins);
	//LambdaGenCountNonRad = AnalyzeGenTreeRadCorr(varidx, new TFile(fileNonRadgenMCTree.c_str(), "read"), "Lambda Tot", q2entries, q2bins);

	//cout << "LambdaGenCountRad: " << LambdaGenCountRad[0] << endl;
	//cout << "LambdaGenCountNonRad: " << LambdaGenCountNonRad[0] << endl;

	double y1[q2entries] = {};
	double y2[q2entries] = {};
	double y3[q2entries] = {};
	//vector<double> q2bins = { 5.0, 7.0, 10.0, 15.0, 25.0, 100.0 };
	//double xerr[q2entries] = { 1,1.5,2.5,5,37.5 };
	//double xvals[q2entries] = { 6,8.5,12.5,20,62.5 };
	double xerr[q2entries] = {};
	double xvals[q2entries] = {};
	double y1err[q2entries] = {};
	double y2err[q2entries] = {};
	double y3err[q2entries] = {};
	double binarray[q2entries+1] = {};
	for (int d = 0; d < q2entries; d++) {
		xerr[d] = abs(q2bins[d+1]-q2bins[d])/2.0;
		cout << "Xerr: " << xerr[d] << endl;
		xvals[d] = q2bins[d]+xerr[d];
		cout << "Xvals: " << xvals[d] << endl;
	}
	for (int d = 0; d <= q2entries; d++) {
		binarray[d] = q2bins[d];
		cout << "Bin Array: " << binarray[d] << endl;
	}
	//double q2[q2entries] = { 0.55,0.65,0.75,0.85,1.0,1.2,1.45,1.9,2.85 };
	//double dx[q2entries] = { 0.1,0.1,0.1,0.1,0.2,0.2,0.3,0.6,1.3 };

	double effregplot[q2entries] = {};
	double effbarplot[q2entries] = {};
	double erreffregplot[q2entries] = {};
	double erreffbarplot[q2entries] = {};
	double lambdaradcorrplot[q2entries] = {};
	double lambdaradcorrploterrors[q2entries] = {};

	
	double X[5];
	double y[5];
	double dxlow[5];
	double dxup[5];
	double dylow[5];
	double dyup[5];
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//Lambda Asymm Data from 2009 paper obtained through xyscan, no syst error published for this, so just stat. error
	
	int n = 0;

	X[n] = xvals[n];	y[n] = -0.0160338;	dxlow[n] = 0;	dxup[n] = 0;	dylow[n] = 0.0388186;	dyup[n] = 0.0388186;	n++;
	X[n] = xvals[n];	y[n] = 0.0177215;	dxlow[n] = 0;	dxup[n] = 0;	dylow[n] = 0.0438819;	dyup[n] = 0.0438819;	n++;
	X[n] = xvals[n];	y[n] = -0.0683544;	dxlow[n] = 0;	dxup[n] = 0;	dylow[n] = 0.0337553;	dyup[n] = 0.0337553;	n++;
	X[n] = xvals[n];	y[n] = -0.0160338;	dxlow[n] = 0;	dxup[n] = 0;	dylow[n] = 0.0472574;	dyup[n] = 0.0472574;	n++;
	X[n] = xvals[n];	y[n] = 0.0953586;	dxlow[n] = 0;	dxup[n] = 0;	dylow[n] = 0.0320675;	dyup[n] = 0.0320675;	n++;
	
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// PLOTTING MEASURED LAMBDA ASYMMETRY
	for (int k = 0; k < q2entries; k++) {

		double templamregeffRAPGAP = (LambdaRegMCRAPGAPCount[k][0]) / (LambdaRegGenCountRAPGAP[k]);
		double templambareffRAPGAP = (LambdaBarMCRAPGAPCount[k][0]) / (LambdaBarGenCountRAPGAP[k]);
		double templamregefferrRAPGAP = GetRatioError(templamregeffRAPGAP, LambdaRegMCRAPGAPCount[k][0], LambdaRegMCRAPGAPCount[k][1], LambdaRegGenCountRAPGAP[k], 0);
		double templambarefferrRAPGAP = GetRatioError(templambareffRAPGAP, LambdaBarMCRAPGAPCount[k][0], LambdaBarMCRAPGAPCount[k][1], LambdaBarGenCountRAPGAP[k], 0);
		double templamregRAPGAP = (LambdaRegDataCount[k][0]) / (templamregeffRAPGAP);
		double templambarRAPGAP = (LambdaBarDataCount[k][0]) / (templambareffRAPGAP);
		double templamregerrRAPGAP = GetRatioError((LambdaRegDataCount[k][0]) / (templamregeffRAPGAP), (LambdaRegDataCount[k][0]), (LambdaRegDataCount[k][1]), (templamregeffRAPGAP), templamregefferrRAPGAP);
		double templambarerrRAPGAP = GetRatioError((LambdaBarDataCount[k][0]) / (templambareffRAPGAP), (LambdaBarDataCount[k][0]), (LambdaBarDataCount[k][1]), (templambareffRAPGAP), templambarefferrRAPGAP);
		y1[k] = (templamregRAPGAP - templambarRAPGAP) / ((templamregRAPGAP + templambarRAPGAP));
		//y1[k] = (LambdaRegDataCount[k][0] - LambdaBarDataCount[k][0])/(LambdaRegDataCount[k][0] + LambdaBarDataCount[k][0]);
		//y1[k] = (LambdaRegGenCount[k] - LambdaBarGenCount[k]) / (LambdaRegGenCount[k] + LambdaBarGenCount[k]);

		y1err[k] = GetRatioError(((templamregRAPGAP - templambarRAPGAP) / ((templamregRAPGAP + templambarRAPGAP))), (templamregRAPGAP - templambarRAPGAP), (templamregerrRAPGAP + templambarerrRAPGAP), ((templamregRAPGAP + templambarRAPGAP)), ((templamregerrRAPGAP + templambarerrRAPGAP)));
		//y1err[k] = GetRatioError(y1[k], (LambdaRegDataCount[k][0] - LambdaBarDataCount[k][0]), (LambdaRegDataCount[k][1] + LambdaBarDataCount[k][1]), ((LambdaRegDataCount[k][0] - LambdaBarDataCount[k][0])), ((LambdaRegDataCount[k][1] + LambdaBarDataCount[k][1])));
		//y1err[k] = 0;

		double templamregeffDJANGOH = (LambdaRegMCDJANGOHCount[k][0]) / (LambdaRegGenCountDJANGOH[k]);
		double templambareffDJANGOH = (LambdaBarMCDJANGOHCount[k][0]) / (LambdaBarGenCountDJANGOH[k]);
		double templamregefferrDJANGOH = GetRatioError(templamregeffDJANGOH, LambdaRegMCDJANGOHCount[k][0], LambdaRegMCDJANGOHCount[k][1], LambdaRegGenCountDJANGOH[k], 0);
		double templambarefferrDJANGOH = GetRatioError(templambareffDJANGOH, LambdaBarMCDJANGOHCount[k][0], LambdaBarMCDJANGOHCount[k][1], LambdaBarGenCountDJANGOH[k], 0);
		double templamregDJANGOH = (LambdaRegDataCount[k][0]) / (templamregeffDJANGOH);
		double templambarDJANGOH = (LambdaBarDataCount[k][0]) / (templambareffDJANGOH);
		double templamregerrDJANGOH = GetRatioError((LambdaRegDataCount[k][0]) / (templamregeffDJANGOH), (LambdaRegDataCount[k][0]), (LambdaRegDataCount[k][1]), (templamregeffDJANGOH), templamregefferrDJANGOH);
		double templambarerrDJANGOH = GetRatioError((LambdaBarDataCount[k][0]) / (templambareffDJANGOH), (LambdaBarDataCount[k][0]), (LambdaBarDataCount[k][1]), (templambareffDJANGOH), templambarefferrDJANGOH);
		y2[k] = (templamregDJANGOH - templambarDJANGOH) / ((templamregDJANGOH + templambarDJANGOH));
		//y1[k] = (LambdaRegDataCount[k][0] - LambdaBarDataCount[k][0])/(LambdaRegDataCount[k][0] + LambdaBarDataCount[k][0]);
		//y1[k] = (LambdaRegGenCount[k] - LambdaBarGenCount[k]) / (LambdaRegGenCount[k] + LambdaBarGenCount[k]);

		y2err[k] = GetRatioError(((templamregDJANGOH - templambarDJANGOH) / ((templamregDJANGOH + templambarDJANGOH))), (templamregDJANGOH - templambarDJANGOH), (templamregerrDJANGOH + templambarerrDJANGOH), ((templamregDJANGOH + templambarDJANGOH)), ((templamregerrDJANGOH + templambarerrDJANGOH)));
		//y1err[k] = GetRatioError(y1[k], (LambdaRegDataCount[k][0] - LambdaBarDataCount[k][0]), (LambdaRegDataCount[k][1] + LambdaBarDataCount[k][1]), ((LambdaRegDataCount[k][0] - LambdaBarDataCount[k][0])), ((LambdaRegDataCount[k][1] + LambdaBarDataCount[k][1])));
		//y1err[k] = 0;

		effregplot[k] = templamregeffRAPGAP;
		effbarplot[k] = templambareffRAPGAP;
		erreffregplot[k] = templamregefferrRAPGAP;
		erreffbarplot[k] = templambarefferrRAPGAP;
	}

	
	TMultiGraph* mg = new TMultiGraph();

	c3->SetName("Lambda Asymmetry Measured in Data");
	TGraphAsymmErrors* LambdaAsymRAPGAP = new TGraphAsymmErrors(q2entries, xvals, y1, xerr, xerr, y1err, y1err);
	TGraphAsymmErrors* LambdaAsymDJANGOH = new TGraphAsymmErrors(q2entries, xvals, y2, xerr, xerr, y2err, y2err);
	LambdaAsymRAPGAP->SetLineColor(kBlack);
	LambdaAsymDJANGOH->SetLineColor(kRed);
	mg->Add(LambdaAsymRAPGAP,"AP");
	mg->Add(LambdaAsymDJANGOH, "AP");
	TLegend* legend = new TLegend(0.5513, 0.4144, 0.888, 0.8823);
	legend->AddEntry(LambdaAsymRAPGAP, "#epsilon_{#Lambda} with RAPGAP", "l");
	legend->AddEntry(LambdaAsymDJANGOH, "#epsilon_{#Lambda} with DJANGOH", "l");
	//LambdaAsymRAPGAP->Draw("ap");
	//LambdaAsymDJANGOH->SetOption("same");
	//LambdaAsymDJANGOH->Draw();
	mg->Draw("AP");
	legend->Draw("same");
	mg->SetTitle("#frac{#sigma(ep #rightarrow e #Lambda X) - #sigma(ep #rightarrow e #bar{ #Lambda } X)}{#sigma(ep #rightarrow e #Lambda X) + #sigma(ep #rightarrow e #bar{ #Lambda } X)} [2005-2007 HERA II DST7]");
	mg->GetXaxis()->SetTitle(xaxistitle.c_str());
	mg->GetYaxis()->SetTitle("A_{#Lambda}");
	outputfile.cd();
	c3->Write();
	f->cd();
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// PLOTTING RAPGAP GENERATOR ASSYMETRY HISTOGRAM
	// THEN CALCULATING MEASURED RAPGAP ASYMMETRY USING DJANGOH FOR THE EFFICIENCY
	TH1F* GenHist = new TH1F("Gen Hist", "GenHist", q2entries, binarray);
	TH1F* GenHistWithBG = new TH1F("Gen Hist BG Included", "GenHist BG Included", q2entries, binarray);
	//Getting data for MC Gen Asymmetry
	cout << "q2entries: " << q2entries << endl;
	for (int k = 0; k < q2entries; k++) {
		
		y1[k] = (LambdaRegGenCountRAPGAP[k] - LambdaBarGenCountRAPGAP[k]) / (LambdaRegGenCountRAPGAP[k] + LambdaBarGenCountRAPGAP[k]);
		y2[k] = ((LambdaRegGenCountRAPGAP[k] + LambdaRegBGGenCountRAPGAP[k]) - (LambdaBarGenCountRAPGAP[k]+ LambdaBarBGGenCountRAPGAP[k])) / ((LambdaRegGenCountRAPGAP[k]+ LambdaRegBGGenCountRAPGAP[k]) + (LambdaBarGenCountRAPGAP[k]+ LambdaBarBGGenCountRAPGAP[k]));
		GenHist->SetBinContent(k+1, y1[k]);
		GenHistWithBG->SetBinContent(k + 1, y2[k]);
		cout << "Bin Width: " << GenHist->GetBinWidth(k) << endl;
		cout << "Gen Value: " << y2[k] << endl;
		
		//double templamregeffDJANGOH = (LambdaRegMCDJANGOHCount[k][0]) / (LambdaRegGenCountDJANGOH[k] + LambdaRegBGGenCountRAPGAP[k]);
		double templamregeffDJANGOH = (LambdaRegMCDJANGOHCount[k][0]) / (LambdaRegGenCountDJANGOH[k]);
		double templambareffDJANGOH = (LambdaBarMCDJANGOHCount[k][0]) / (LambdaBarGenCountDJANGOH[k] + LambdaBarBGGenCountRAPGAP[k]);
		double templamregefferrDJANGOH = GetRatioError(templamregeffDJANGOH, LambdaRegMCDJANGOHCount[k][0], LambdaRegMCDJANGOHCount[k][1], LambdaRegGenCountDJANGOH[k], 0);
		double templambarefferrDJANGOH = GetRatioError(templambareffDJANGOH, LambdaBarMCDJANGOHCount[k][0], LambdaBarMCDJANGOHCount[k][1], LambdaBarGenCountDJANGOH[k], 0);
		double templamregDJANGOH = (LambdaRegMCRAPGAPCount[k][0]) / (templamregeffDJANGOH);
		double templambarDJANGOH = (LambdaBarMCRAPGAPCount[k][0]) / (templambareffDJANGOH);
		double templamregerrDJANGOH = GetRatioError((LambdaRegMCRAPGAPCount[k][0]) / (templamregeffDJANGOH), (LambdaRegMCRAPGAPCount[k][0]), (LambdaRegMCRAPGAPCount[k][1]), (templamregeffDJANGOH), templamregefferrDJANGOH);
		double templambarerrDJANGOH = GetRatioError((LambdaBarMCRAPGAPCount[k][0]) / (templambareffDJANGOH), (LambdaBarMCRAPGAPCount[k][0]), (LambdaBarMCRAPGAPCount[k][1]), (templambareffDJANGOH), templambarefferrDJANGOH);
		y2[k] = (templamregDJANGOH - templambarDJANGOH) / ((templamregDJANGOH + templambarDJANGOH));


		/*double templamregeffRAPGAP = (LambdaRegMCRAPGAPCount[k][0]) / (LambdaRegGenCountRAPGAP[k]);
		double templambareffRAPGAP = (LambdaBarMCRAPGAPCount[k][0]) / (LambdaBarGenCountRAPGAP[k]);
		double templamregefferrRAPGAP = GetRatioError(templamregeffRAPGAP, LambdaRegMCRAPGAPCount[k][0], LambdaRegMCRAPGAPCount[k][1], LambdaRegGenCountRAPGAP[k], 0);
		double templambarefferrRAPGAP = GetRatioError(templambareffRAPGAP, LambdaBarMCRAPGAPCount[k][0], LambdaBarMCRAPGAPCount[k][1], LambdaBarGenCountRAPGAP[k], 0);
		double templamregRAPGAP = (LambdaRegMCRAPGAPCount[k][0]) / (templamregeffRAPGAP);
		double templambarRAPGAP = (LambdaBarMCRAPGAPCount[k][0]) / (templambareffRAPGAP);
		double templamregerrRAPGAP = GetRatioError((LambdaRegMCRAPGAPCount[k][0]) / (templamregeffRAPGAP), (LambdaRegMCRAPGAPCount[k][0]), (LambdaRegMCRAPGAPCount[k][1]), (templamregeffRAPGAP), templamregefferrRAPGAP);
		double templambarerrRAPGAP = GetRatioError((LambdaBarMCRAPGAPCount[k][0]) / (templambareffRAPGAP), (LambdaBarMCRAPGAPCount[k][0]), (LambdaBarMCRAPGAPCount[k][1]), (templambareffRAPGAP), templambarefferrRAPGAP);
		y2[k] = (templamregRAPGAP - templambarRAPGAP) / ((templamregRAPGAP + templambarRAPGAP));*/


		
		//y2err[k] = GetRatioError(((templamregRAPGAP - templambarRAPGAP) / ((templamregRAPGAP + templambarRAPGAP))), (templamregRAPGAP - templambarRAPGAP), (templamregerrRAPGAP + templambarerrRAPGAP), ((templamregRAPGAP + templambarRAPGAP)), ((templamregerrRAPGAP + templambarerrRAPGAP)));

		y2err[k] = GetRatioError(((templamregDJANGOH - templambarDJANGOH) / ((templamregDJANGOH + templambarDJANGOH))), (templamregDJANGOH - templambarDJANGOH), (templamregerrDJANGOH + templambarerrDJANGOH), ((templamregDJANGOH + templambarDJANGOH)), ((templamregerrDJANGOH + templambarerrDJANGOH)));
		
	}

	c3->Clear();
	mg->Clear();
	legend->Clear();
	c3->SetName("RAPGAP Cross Check with DJANGOH Efficiency");
	TGraphAsymmErrors* RAPGAPmeasuredAsym = new TGraphAsymmErrors(q2entries, xvals, y2, xerr, xerr, y2err, y2err);
	GenHist->SetLineColor(kBlack);
	GenHistWithBG->SetLineColor(kBlue);
	RAPGAPmeasuredAsym->SetLineColor(kRed);
	legend->AddEntry(GenHist, "Generator Asymmetry in RAPGAP", "l");
	legend->AddEntry(GenHistWithBG, "Generator Asymmetry in RAPGAP + BG", "l");
	legend->AddEntry(RAPGAPmeasuredAsym, "Measured A_{#Lambda} in RAPGAP", "l");
	//GenAsym->Draw("APF");
	GenHist->SetStats(0);
	RAPGAPmeasuredAsym->Draw("AP");
	GenHist->Draw("same");
	GenHistWithBG->Draw("same");
	legend->Draw("same");
	RAPGAPmeasuredAsym->SetTitle("RAPGAP Cross Check with DJANGOH Efficiency");
	RAPGAPmeasuredAsym->GetXaxis()->SetTitle(xaxistitle.c_str());
	RAPGAPmeasuredAsym->GetYaxis()->SetTitle("A_{#Lambda}");
	outputfile.cd();
	c3->Write();
	f->cd();

	
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// PLOTTING DJANGOH GENERATOR ASSYMETRY HISTOGRAM
	// THEN CALCULATING MEASURED DJANGOH ASYMMETRY USING RAPGAP FOR THE EFFICIENCY
	GenHist->Clear();
	GenHistWithBG->Clear();
	//Getting data for MC Gen Asymmetry
	cout << "q2entries: " << q2entries << endl;
	for (int k = 0; k < q2entries; k++) {

		y1[k] = (LambdaRegGenCountDJANGOH[k] - LambdaBarGenCountDJANGOH[k]) / (LambdaRegGenCountDJANGOH[k] + LambdaBarGenCountDJANGOH[k]);
		y2[k] = ((LambdaRegGenCountDJANGOH[k] + LambdaRegBGGenCountDJANGOH[k]) - (LambdaBarGenCountDJANGOH[k] + LambdaBarBGGenCountDJANGOH[k])) / ((LambdaRegGenCountDJANGOH[k] + LambdaRegBGGenCountDJANGOH[k]) + (LambdaBarGenCountDJANGOH[k] + LambdaBarBGGenCountDJANGOH[k]));
		GenHist->SetBinContent(k + 1, y1[k]);
		GenHistWithBG->SetBinContent(k + 1, y2[k]);
		cout << "Bin Width: " << GenHist->GetBinWidth(k) << endl;

		//double templamregeffRAPGAP = (LambdaRegMCRAPGAPCount[k][0]) / (LambdaRegGenCountRAPGAP[k]+ LambdaRegBGGenCountDJANGOH[k]);
		double templamregeffRAPGAP = (LambdaRegMCRAPGAPCount[k][0]) / (LambdaRegGenCountRAPGAP[k]);
		double templambareffRAPGAP = (LambdaBarMCRAPGAPCount[k][0]) / (LambdaBarGenCountRAPGAP[k]+ LambdaBarBGGenCountDJANGOH[k]);
		double templamregefferrRAPGAP = GetRatioError(templamregeffRAPGAP, LambdaRegMCRAPGAPCount[k][0], LambdaRegMCRAPGAPCount[k][1], LambdaRegGenCountRAPGAP[k], 0);
		double templambarefferrRAPGAP = GetRatioError(templambareffRAPGAP, LambdaBarMCRAPGAPCount[k][0], LambdaBarMCRAPGAPCount[k][1], LambdaBarGenCountRAPGAP[k], 0);
		double templamregRAPGAP = (LambdaRegMCDJANGOHCount[k][0]) / (templamregeffRAPGAP);
		double templambarRAPGAP = (LambdaBarMCDJANGOHCount[k][0]) / (templambareffRAPGAP);
		double templamregerrRAPGAP = GetRatioError((LambdaRegMCDJANGOHCount[k][0]) / (templamregeffRAPGAP), (LambdaRegMCDJANGOHCount[k][0]), (LambdaRegMCDJANGOHCount[k][1]), (templamregeffRAPGAP), templamregefferrRAPGAP);
		double templambarerrRAPGAP = GetRatioError((LambdaBarMCDJANGOHCount[k][0]) / (templambareffRAPGAP), (LambdaBarMCDJANGOHCount[k][0]), (LambdaBarMCDJANGOHCount[k][1]), (templambareffRAPGAP), templambarefferrRAPGAP);
		y2[k] = (templamregRAPGAP - templambarRAPGAP) / ((templamregRAPGAP + templambarRAPGAP));


		y2err[k] = GetRatioError(((templamregRAPGAP - templambarRAPGAP) / ((templamregRAPGAP + templambarRAPGAP))), (templamregRAPGAP - templambarRAPGAP), (templamregerrRAPGAP + templambarerrRAPGAP), ((templamregRAPGAP + templambarRAPGAP)), ((templamregerrRAPGAP + templambarerrRAPGAP)));

	}

	c3->Clear();
	mg->Clear();
	legend->Clear();
	c3->SetName("DJANGOH Cross Check with RAPGAP Efficiency");
	TGraphAsymmErrors* DJANGOHmeasuredAsym = new TGraphAsymmErrors(q2entries, xvals, y2, xerr, xerr, y2err, y2err);
	GenHist->SetLineColor(kBlack);
	GenHistWithBG->SetLineColor(kBlue);
	DJANGOHmeasuredAsym->SetLineColor(kRed);
	legend->AddEntry(GenHist, "Generator Asymmetry in DJANGOH", "l");
	legend->AddEntry(GenHistWithBG, "Generator Asymmetry in DJANGOH + BG", "l");
	legend->AddEntry(DJANGOHmeasuredAsym, "Measured A_{#Lambda} in DJANGOH", "l");
	//GenAsym->Draw("APF");
	GenHist->SetStats(0);
	DJANGOHmeasuredAsym->Draw("AP");
	GenHist->Draw("same");
	GenHistWithBG->Draw("same");
	legend->Draw("same");
	DJANGOHmeasuredAsym->SetTitle("DJANGOH Cross Check with RAPGAP Efficiency");
	DJANGOHmeasuredAsym->GetXaxis()->SetTitle(xaxistitle.c_str());
	DJANGOHmeasuredAsym->GetYaxis()->SetTitle("A_{#Lambda}");
	outputfile.cd();
	c3->Write();
	f->cd();

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	
	
	
	
	
	
	
	
	//Now doing peak extractions and analysis for inclusive measurements
	//vector<TH1F*> LambdaTotDataxincl;
	vector<TH1F*> LambdaRegDataxincl;
	vector<TH1F*> LambdaBarDataxincl;
	//vector<TH1F*> LambdaTotMCRAPGAPxincl;
	vector<TH1F*> LambdaRegMCRAPGAPxincl;
	vector<TH1F*> LambdaBarMCRAPGAPxincl;
	//vector<TH1F*> LambdaTotMCDJANGOHxincl;
	vector<TH1F*> LambdaRegMCDJANGOHxincl;
	vector<TH1F*> LambdaBarMCDJANGOHxincl;

	//vector<TH1F*> LambdaTotMCRAPGAPgenassigned;
	//vector<TH1F*> LambdaTotMCDJANGOHgenassigned;
	//vector<TH1F*> LambdaTotMCRAPGAPgenassignedBG;
	//vector<TH1F*> LambdaTotMCDJANGOHgenassignedBG;
	
	//I don't have Histograms for particle level generator data because counts for these
	//will be done by just going through the generate TTree and adding events up that are within our conditions
	for (int a = 0; a < 1; a++) {
		//string namestr1 = "Lambda Tot Data";
		string namestr2 = "Lambda Reg Data q2 Bin";
		string namestr3 = "Lambda Bar Data q2 Bin";
		//string namestr4 = "Lambda Tot MC RAPGAP q2 Bin";
		string namestr5 = "Lambda Reg MC RAPGAP q2 Bin";
		string namestr6 = "Lambda Bar MC RAPGAP q2 Bin";
		//string namestr7 = "Lambda Tot MC DJANGOH q2 Bin";
		string namestr8 = "Lambda Reg MC DJANGOH q2 Bin";
		string namestr9 = "Lambda Bar MC DJANGOH q2 Bin";

		string namestr10 = "Lambda Reg MC RAPGAP q2 Bin";
		string namestr11 = "Lambda Reg MC DJANGOH q2 Bin";
		string namestr12 = "Lambda Reg MC RAPGAP BG q2 Bin";
		string namestr13 = "Lambda Reg MC DJANGOH BG q2 Bin";
		

		//LambdaTotDataQ2[a] = new TH1F(namestr1.c_str(), namestr1.c_str(), lambdabins, lambdalowerbinrange, lambdaupperbinrange);
		//LambdaTotDataxincl.push_back(new TH1F(namestr1.c_str(), namestr1.c_str(), lambdabins, lambdalowerbinrange, lambdaupperbinrange));
		LambdaRegDataxincl.push_back(new TH1F(namestr2.c_str(), namestr2.c_str(), lambdabins, lambdalowerbinrange, lambdaupperbinrange));
		LambdaBarDataxincl.push_back(new TH1F(namestr3.c_str(), namestr3.c_str(), lambdabins, lambdalowerbinrange, lambdaupperbinrange));
		//LambdaTotMCRAPGAPxincl.push_back(new TH1F(namestr4.c_str(), namestr4.c_str(), lambdabins, lambdalowerbinrange, lambdaupperbinrange));
		LambdaRegMCRAPGAPxincl.push_back(new TH1F(namestr5.c_str(), namestr5.c_str(), lambdabins, lambdalowerbinrange, lambdaupperbinrange));
		LambdaBarMCRAPGAPxincl.push_back(new TH1F(namestr6.c_str(), namestr6.c_str(), lambdabins, lambdalowerbinrange, lambdaupperbinrange));
		//LambdaTotMCDJANGOHxincl.push_back(new TH1F(namestr7.c_str(), namestr7.c_str(), lambdabins, lambdalowerbinrange, lambdaupperbinrange));
		LambdaRegMCDJANGOHxincl.push_back(new TH1F(namestr8.c_str(), namestr8.c_str(), lambdabins, lambdalowerbinrange, lambdaupperbinrange));
		LambdaBarMCDJANGOHxincl.push_back(new TH1F(namestr9.c_str(), namestr9.c_str(), lambdabins, lambdalowerbinrange, lambdaupperbinrange));
		
		//LambdaTotMCRAPGAPgenassigned.push_back(new TH1F(namestr10.c_str(), namestr10.c_str(), lambdabins, lambdalowerbinrange, lambdaupperbinrange));
		//LambdaTotMCDJANGOHgenassigned.push_back(new TH1F(namestr11.c_str(), namestr11.c_str(), lambdabins, lambdalowerbinrange, lambdaupperbinrange));
		//LambdaTotMCRAPGAPgenassignedBG.push_back(new TH1F(namestr12.c_str(), namestr12.c_str(), lambdabins, lambdalowerbinrange, lambdaupperbinrange));
		//LambdaTotMCDJANGOHgenassignedBG.push_back(new TH1F(namestr13.c_str(), namestr13.c_str(), lambdabins, lambdalowerbinrange, lambdaupperbinrange));

	}



	//Defining vectors now that will contain the count and count error data for each paticle at MC, Data, and gen level (gen level has no count error, just count)
	//These vectors will be for all counting binning for all variables, can just clear them inbetween uses
	//vector<vector<double>> LambdaTotDataCountincl;
	vector<vector<double>> LambdaRegDataCountincl;
	vector<vector<double>> LambdaBarDataCountincl;
	//vector<vector<double>> LambdaTotMCRAPGAPCountincl;
	vector<vector<double>> LambdaRegMCRAPGAPCountincl;
	vector<vector<double>> LambdaBarMCRAPGAPCountincl;
	//vector<vector<double>> LambdaTotMCDJANGOHCountincl;
	vector<vector<double>> LambdaRegMCDJANGOHCountincl;
	vector<vector<double>> LambdaBarMCDJANGOHCountincl;
	
	//vector<vector<double>> LambdaTotMCRAPGAPgenassignedCountincl;
	//vector<vector<double>> LambdaTotMCDJANGOHgenassignedCountincl;
	//vector<vector<double>> LambdaTotMCRAPGAPgenassignedBGCountincl;
	//vector<vector<double>> LambdaTotMCDJANGOHgenassignedBGCountincl;
	//Gen particle count
	//vector<double> LambdaTotGenCountinclRAPGAP;
	vector<double> LambdaRegGenCountinclRAPGAP;
	vector<double> LambdaBarGenCountinclRAPGAP;
	//vector<double> LambdaTotGenCountinclDJANGOH;
	vector<double> LambdaRegGenCountinclDJANGOH;
	vector<double> LambdaBarGenCountinclDJANGOH;
	
	//Analyzing Tot Lambda Counts for q2 bins for MC and Data
	//vector<vector<double>> AnalyzeTree(TFile * Data, string TreeName, int binentries, int particlecode, vector<TH1F*> HistArray, vector<double> binsvector, string extractionReadback, TCanvas * c3, TCanvas * c1) { //when calling function, particle code deictates 0=lambda or 1=kaon MLP parameters

	//LambdaTotDataCountincl = AnalyzeTree(varidx, new TFile(filelambdaTree.c_str(), "read"), "Lambda Tot", 1, 0, LambdaTotDataxincl, { q2bins[0],q2bins[q2entries] }, "Lambda Tot Incl Data q2 Extraction ", c3, c1, sigmaData, paramData);
	//LambdaTotMCRAPGAPCountincl = AnalyzeTree(varidx, new TFile(filelambdaTreeMCRAPGAP.c_str(), "read"), "Lambda Tot", 1, 0, LambdaTotMCRAPGAPxincl, { q2bins[0],q2bins[q2entries] }, "Lambda Tot Incl MC RAPGAP q2 Extraction ", c3, c1, sigmaMC, paramMC);
	//LambdaTotMCDJANGOHCountincl = AnalyzeTree(varidx, new TFile(filelambdaTreeMCDJANGOH.c_str(), "read"), "Lambda Tot", 1, 0, LambdaTotMCDJANGOHxincl, { q2bins[0],q2bins[q2entries] }, "Lambda Tot Incl MC DJANGOH q2 Extraction ", c3, c1, sigmaMC, paramMC);

	
	//LambdaTotMCRAPGAPgenassignedCountincl = AnalyzeTree(varidx, new TFile(filelambdaTreeMCRAPGAP.c_str(), "read"), "Lambda Reg Gen", 1, 0, LambdaTotMCRAPGAPgenassigned, { q2bins[0],q2bins[q2entries] }, "Lambda Reg Gen Incl MC RAPGAP q2 Extraction ", c3, c1, sigmaMC, paramMC);
	//LambdaTotMCDJANGOHgenassignedCountincl = AnalyzeTree(varidx, new TFile(filelambdaTreeMCDJANGOH.c_str(), "read"), "Lambda Reg Gen", 1, 0, LambdaTotMCDJANGOHgenassigned, { q2bins[0],q2bins[q2entries] }, "Lambda Reg Gen Incl MC DJANGOH q2 Extraction ", c3, c1, sigmaMC, paramMC);
	
	//LambdaTotMCRAPGAPgenassignedBGCountincl = AnalyzeTree(varidx, new TFile(filelambdaTreeMCRAPGAP.c_str(), "read"), "Lambda Reg BG", 1, 0, LambdaTotMCRAPGAPgenassignedBG, { q2bins[0],q2bins[q2entries] }, "Lambda Reg BG Incl MC RAPGAP q2 Extraction ", c3, c1, sigmaMC, paramMC);
	//LambdaTotMCDJANGOHgenassignedBGCountincl = AnalyzeTree(varidx, new TFile(filelambdaTreeMCDJANGOH.c_str(), "read"), "Lambda Reg BG", 1, 0, LambdaTotMCDJANGOHgenassignedBG, { q2bins[0],q2bins[q2entries] }, "Lambda Reg BG Incl MC DJANGOH q2 Extraction ", c3, c1, sigmaMC, paramMC);

	LambdaRegDataCountincl = AnalyzeTree(varidx, new TFile(filelambdaTree.c_str(), "read"), "Lambda Reg", 1, 0, LambdaRegDataxincl, { q2bins[0],q2bins[q2entries] }, "Lambda Reg Incl Data q2 Extraction ", c3, c1, binparaminits, binrangedefs);
	LambdaRegMCRAPGAPCountincl = AnalyzeTree(varidx, new TFile(filelambdaTreeMCRAPGAP.c_str(), "read"), "Lambda Reg", 1, 0, LambdaRegMCRAPGAPxincl, { q2bins[0],q2bins[q2entries] }, "Lambda Reg Incl MC RAPGAP q2 Extraction ", c3, c1, binparaminitsMC, binrangedefsMC);
	LambdaRegMCDJANGOHCountincl = AnalyzeTree(varidx, new TFile(filelambdaTreeMCDJANGOH.c_str(), "read"), "Lambda Reg", 1, 0, LambdaRegMCDJANGOHxincl, { q2bins[0],q2bins[q2entries] }, "Lambda Reg Incl MC DJANGOH q2 Extraction ", c3, c1, binparaminitsMC, binrangedefsMC);

	LambdaBarDataCountincl = AnalyzeTree(varidx, new TFile(filelambdaTree.c_str(), "read"), "Lambda Bar", 1, 0, LambdaBarDataxincl, { q2bins[0],q2bins[q2entries] }, "Lambda Bar Incl Data q2 Extraction ", c3, c1, binparaminits, binrangedefs);
	LambdaBarMCRAPGAPCountincl = AnalyzeTree(varidx, new TFile(filelambdaTreeMCRAPGAP.c_str(), "read"), "Lambda Bar", 1, 0, LambdaBarMCRAPGAPxincl, { q2bins[0],q2bins[q2entries] }, "Lambda Bar Incl MC RAPGAP q2 Extraction ", c3, c1, binparaminitsMC, binrangedefsMC);
	LambdaBarMCDJANGOHCountincl = AnalyzeTree(varidx, new TFile(filelambdaTreeMCDJANGOH.c_str(), "read"), "Lambda Bar", 1, 0, LambdaBarMCDJANGOHxincl, { q2bins[0],q2bins[q2entries] }, "Lambda Bar Incl MC DJANGOH q2 Extraction ", c3, c1, binparaminitsMC, binrangedefsMC);



	//LambdaTotGenCountinclRAPGAP = AnalyzeGenTree(varidx, new TFile(filegenMCTreeRAPGAP.c_str(), "read"), "Lambda Tot Channel", 1, { q2bins[0],q2bins[q2entries] });
	LambdaRegGenCountinclRAPGAP = AnalyzeGenTree(varidx, new TFile(filegenMCTreeRAPGAP.c_str(), "read"), "Lambda Reg Channel", 1, { q2bins[0],q2bins[q2entries] });
	LambdaBarGenCountinclRAPGAP = AnalyzeGenTree(varidx, new TFile(filegenMCTreeRAPGAP.c_str(), "read"), "Lambda Bar Channel", 1, { q2bins[0],q2bins[q2entries] });
	
	//LambdaTotGenCountinclDJANGOH = AnalyzeGenTree(varidx, new TFile(filegenMCTreeDJANGOH.c_str(), "read"), "Lambda Tot Channel", 1, { q2bins[0],q2bins[q2entries] });
	LambdaRegGenCountinclDJANGOH = AnalyzeGenTree(varidx, new TFile(filegenMCTreeDJANGOH.c_str(), "read"), "Lambda Reg Channel", 1, { q2bins[0],q2bins[q2entries] });
	LambdaBarGenCountinclDJANGOH = AnalyzeGenTree(varidx, new TFile(filegenMCTreeDJANGOH.c_str(), "read"), "Lambda Bar Channel", 1, { q2bins[0],q2bins[q2entries] });

	
	outputfile.Write();
	outputfile.Close();

	cout << "Done" << endl;

};
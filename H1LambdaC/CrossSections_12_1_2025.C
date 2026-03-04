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
vector<double> leftfitbound1 = { 1.088, 0.4 };
vector<double> leftfitbound2 = { 1.095, 0.42 };
vector<double> rightfitbound1 = { 1.14, 0.58 };
vector<double> rightfitbound2 = { 1.15, 0.60 };
vector<double> leftsubbound = { 1.095, 0.42 };
vector<double> rightsubbound = { 1.14, 0.58 };

//defining vectors of the binning definitions for each of the kinematic variables we are looking at
//when playing with binning, have to adjust these vectors and hand specify the number of bins for defining Hist arrays
const int q2entries = 9;
vector<double> Etabins = {-1.3, -1.0, -0.75, -0.5, -0.25, 0, 0.25, 0.5, 0.75, 1.0, 1.3};
vector<double> xbins = {0.0001, 0.0002, 0.0004, 0.001, 0.01};

double lowPtbound = 0.5;
//double lowerYsbound = 0.0375;
double lowerYsbound = 0.1;
double upperYsbound = 0.6;
//Branching Ratios
double BRlambda = 0.639;
double BRkaon = 0.692;
//vector<double> q2bins = { 5.0, 7.0, 10.0, 15.0, 25.0, 100.0 };
//const int pTentries = 9;
//const int Etaentries = 10;
//const int xentries = 4;
//const int q2entries = 5;
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//LUMI DATA and cross seciton values

//Defining integrated Lumi values for 05,06,07 data set obtained separately from oolumi

//double DataIntLumi06 = 106823.1; //2005 DATA nb-1
//double DataIntLumi06 = 144634.0; //2006 DATA nb-1
//double DataIntLumi06 = 46200.7; //2007 DATA nb-1
//double DataIntLumi06 = 297657.8; //ALL DATA nb-1

//double MCIntLumi06 = 126880.0; //nb-1
double MCIntLumi06 = 126880.0; //RAPGAP nb-1
//double MCIntLumi06 = 2918092.00; //DJANGOH nb-1


//double MCDjangoh14radgen = 1087523.00; //2005 Rad DJANGOH14 nb-1
//double MCDjangoh14nonradgen = 1133362.00; //2005 NonRad DJANGOH14 nb-1
//double MCDjangoh14radgen = 2268210.00; //2006 Rad DJANGOH14 nb-1
//double MCDjangoh14nonradgen = 1944680.00; //2006 NonRad DJANGOH14 nb-1
//double MCDjangoh14radgen = 753224.00; //2007 Rad DJANGOH14 nb-1
//double MCDjangoh14nonradgen = 482191.00; //2007 NonRad DJANGOH14 nb-1
//double MCDjangoh14radgen = 4108957.0; //ALL Rad DJANGOH14 nb-1
//double MCDjangoh14nonradgen = 3560233.0; //ALL NonRad DJANGOH14 nb-1



//string names for file paths of 5 Trees we need to access
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// DATA
//string filekaonTree = "/data/dust/group/h1/klesthen/BaryonAsym/H1LambdaC/KaonTree/2007/merged.root";
//string filekaonTree = "/data/dust/group/h1/klesthen/BaryonAsym/H1LambdaC/KaonTree/merged.root";

//string filelambdaTree = "/data/dust/group/h1/klesthen/BaryonAsym/H1LambdaC/LambdaTree/2007/merged.root";
//string filelambdaTree = "/data/dust/group/h1/klesthen/BaryonAsym/H1LambdaC/LambdaTree/merged.root";
//END DATA
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//MC
//string filekaonTreeMC = "/data/dust/group/h1/klesthen/BaryonAsym/H1LambdaC_MC/KaonTreeMC/RAPGAP31/2007/merged.root";
//string filekaonTreeMC = "/data/dust/group/h1/klesthen/BaryonAsym/H1LambdaC_MC/KaonTreeMC/RAPGAP31/merged.root";


//string filelambdaTreeMC = "/data/dust/group/h1/klesthen/BaryonAsym/H1LambdaC_MC/LambdaTreeMC/RAPGAP31/2007/merged.root";
//string filelambdaTreeMC = "/data/dust/group/h1/klesthen/BaryonAsym/H1LambdaC_MC/LambdaTreeMC/RAPGAP31/merged.root";


//string filegenMCTree = "/data/dust/group/h1/klesthen/BaryonAsym/H1LambdaC_MC/genMCTree/RAPGAP31/Rad/2007/merged.root"; //rad RAPGAP
//string filegenMCTree = "/data/dust/group/h1/klesthen/BaryonAsym/H1LambdaC_MC/genMCTree/RAPGAP31/Rad/merged.root"; //rad RAPGAP



//string fileNonRadgenMCTree = "/data/dust/group/h1/klesthen/BaryonAsym/H1LambdaC_MC/genMCTree/DJANGOH14/NonRad/2007/merged.root"; //non rad DJANGOH
//string fileRadgenMCTree = "/data/dust/group/h1/klesthen/BaryonAsym/H1LambdaC_MC/genMCTree/DJANGOH14/Rad/2007/merged.root";

//string fileNonRadgenMCTree = "/data/dust/group/h1/klesthen/BaryonAsym/H1LambdaC_MC/genMCTree/DJANGOH14/NonRad/merged.root"; //non rad DJANGOH
//string fileRadgenMCTree = "/data/dust/group/h1/klesthen/BaryonAsym/H1LambdaC_MC/genMCTree/DJANGOH14/Rad/merged.root";

//END MC
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//output files
string outputfilename = "/data/dust/group/h1/klesthen/BaryonAsym/H1LambdaC/AnalysisResultFiles/AnalysisOutput.root";
string dummyfile = "/data/dust/group/h1/klesthen/BaryonAsym/H1LambdaC/dummyfile.root";

// Histograms Binning
Double_t lambdabins = 1000;
Double_t lambdaupperbinrange = 1.5;
Double_t lambdalowerbinrange = 1.0;
Double_t kaonbins = 500;
Double_t kaonupperbinrange = 1.0;
Double_t kaonlowerbinrange = 0.0;


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

double flineK0(double* x, double* par)
{
	if (reject && x[0] > leftfitbound2[1] && x[0] < rightfitbound1[1]) {
		TF1::RejectPoint();
		return 0;
	}
	return par[0] + par[1] * x[0];
}



//Main peak extraction, composite fit with RooFit
vector<double> mlpfunctionLambda(TH1F* inHist, string plotname, int particlecode, TCanvas* c3, vector<double> fitparaminit, vector<double> rangedefs) { //when calling function, particle code deictates 0=lambda or 1=kaon MLP parameters

	//outputfile.cd();
	//Define the fit and signal ranges with script inputs
	double x1 = rangedefs[0];
	double x2 = rangedefs[1];
	double x3 = rangedefs[2];
	double x4 = rangedefs[3];
	//double x4 = 1.165;
	//double s1 = rangedefs[4];
	//double s2 = rangedefs[5];

	//define line to visualize the fitting and signal limits on the end plot
	TLine leftfitbound1line = TLine(x1, 0, x1, 200);
	TLine leftfitbound2line = TLine(x2, 0, x2, 200);
	TLine rightfitbound1line = TLine(x3, 0, x3, 200);
	TLine rightfitbound2line = TLine(x4, 0, x4, 200);
	//TLine extractionbound1line = TLine(s1, 0, s1, 300);
	//TLine extractionbound2line = TLine(s2, 0, s2, 300);

	//setting line colors of sitting and signal limits
	leftfitbound1line.SetLineColor(kMagenta);
	leftfitbound2line.SetLineColor(kMagenta);;
	rightfitbound1line.SetLineColor(kMagenta);;
	rightfitbound2line.SetLineColor(kMagenta);
	//extractionbound1line.SetLineColor(kRed);
	//extractionbound2line.SetLineColor(kRed);

	//Defining invariant mass as the observable for RooFit
	RooRealVar mLam("mLam", "mLam", x1, x4);
	//Defining different observable ranges for fitting and signal extractions
	mLam.setRange("full", x1, x4);
	//mLam.setRange("fullalt", , x4);
	mLam.setRange("left", x1, x2);
	mLam.setRange("right", x3, x4);
	//mLam.setRange("signal", s1, s2);
	mLam.setRange("compositefitrange", x2, x3);

	//Composite Fit
	// Background PDF
	//RooRealVar a0("a0", "a0", 10., .0, 1.0e6);
	RooRealVar b1("b1", "b1", -1.0, -1000, 1000);
	RooRealVar b2("b2", "b2", 1.0, -1000, 1000);
	RooRealVar b3("b3", "b3", 1.0, -1000, 1000);
	RooRealVar b4("b4", "b4", 1.0, -1000, 1000);
	RooRealVar b5("b5", "b5", 1.0, -1000, 1000);
	RooRealVar nsigalt("nsigalt", "alt # signal events", 100, 1, 1000000000);
	RooRealVar nbkgalt("nbkgalt", "alt # background events", 10, 1, 1000000000);

	RooRealVar a1("a1", "a1", 1.0, 0.05, 5.);
	RooRealVar a2("a2", "a2", -5.0, -100., -0.1);
	//RooRealVar mu("mu", "mu", fitparaminit[2], fitparaminit[2] - 0.001, fitparaminit[2] + 0.001);
	RooRealVar mu("mu", "mu", 1.1156, 1.1150, 1.1162);
	RooRealVar sigma("sigma", "sigma", fitparaminit[0], 0.0010, 0.005);
	//RooRealVar sigma("sigma", "sigma", 0.001, 0.001, 0.005 );
	RooRealVar p3("p3", "p3", fitparaminit[1], 2.0, 10.0);
	//RooRealVar p3("p3", "p3", 5.0, 2.0, 10);
	//RooRealVar mudouble("mudouble", "mudouble", fitparaminit[2], fitparaminit[2] - 0.001, fitparaminit[2] + 0.001);
	RooRealVar mudouble("mudouble", "mudouble", 1.1156, 1.1152, 1.1161);
	RooRealVar sigmadouble("sigmadouble", "sigmadouble", fitparaminit[0], 0.0010, 0.005);
	//RooRealVar sigmadouble("sigmadouble", "sigmadouble", 0.001, 0.001, 0.005);
	RooRealVar p3double("p3double", "p3double", fitparaminit[1], 2.0, 8.0);
	//RooRealVar p3double("p3double", "p3double", 5.0, 2.0, 10);
	RooRealVar nsig("nsig", "# signal events", 100, 1, 1000000000);
	RooRealVar nbkg("nbkg", "# background events", 10, 1, 1000000000);
	RooGenericPdf bg("bg", "TMath::Power(TMath::Max(mLam-1.0782,0.),a1)*TMath::Exp(a2*(mLam-1.0782))", RooArgSet(mLam, a1, a2));
	//if (particlecode == 1) { RooGenericPdf bg("bg", "TMath::Power(TMath::Max(mLam-0.2792,0.),a1)*TMath::Exp(a2*(mLam-0.2792))", RooArgSet(mLam, a1, a2)); }
	RooGenericPdf bgalt("bgalt", "b1 + mLam*b2 + mLam*mLam*b3 + mLam*mLam*mLam*b4 + mLam*mLam*mLam*mLam*b5", RooArgSet(mLam, b1, b2, b3, b4, b5));
	//RooGenericPdf bgalt("bgalt", "b1 + mLam*b2 + mLam*mLam*b3", RooArgSet(mLam, b1, b2, b3));
	RooGenericPdf sigf("sigf", "sigf", "TMath::Gamma((p3+1)/2)/TMath::Gamma(p3/2)/TMath::Sqrt(TMath::Pi()*p3)*TMath::Power(1.+((mLam-mu)/sigma)*((mLam-mu)/sigma)/p3,-(p3+1)/2)", RooArgSet(mLam, mu, p3, sigma));
	RooGenericPdf sigfdouble("sigfdouble", "sigfdouble", "TMath::Gamma((p3double+1)/2)/TMath::Gamma(p3double/2)/TMath::Sqrt(TMath::Pi()*p3double)*TMath::Power(1.+((mLam-mudouble)/sigmadouble)*((mLam-mudouble)/sigmadouble)/p3double,-(p3double+1)/2)", RooArgSet(mLam, mudouble, p3double, sigmadouble));
	RooAddPdf sum("sum", "sigf+bg", RooArgList(sigf, bg), RooArgList(nsig, nbkg));
	RooAddPdf sumalt("sumalt", "sigfdouble+bgalt", RooArgList(sigfdouble, bgalt), RooArgList(nsigalt, nbkgalt));


	RooDataHist RooFitDataHist("RooFitDataHist", "RooFitDataHist", mLam, inHist);

	bg.fitTo(RooFitDataHist, RooFit::Extended(kTRUE), RooFit::Range("left,right"));
	bgalt.fitTo(RooFitDataHist, RooFit::Extended(kTRUE), RooFit::Range("left,right"));
	//bg.fitTo(RooFitDataHist, RooFit::Range("left,right"), RooFit::Save());
	RooPlot* RooFitPlot = mLam.frame();
	RooFitDataHist.plotOn(RooFitPlot);

	//sum.fitTo(RooFitDataHist, RooFit::Range("full"));
	//bg.fitTo(RooFitDataHist, RooFit::Range("left,right"));
	RooFitResult* result = sum.fitTo(RooFitDataHist, RooFit::Range("full"), RooFit::Save());
	double FWHM = 2 * sigma.getVal() * sqrt(2 * (pow(2, 1 / p3.getVal()) - 1));
	double s1 = mu.getVal() - 6 * FWHM;
	double s2 = mu.getVal() + 6 * FWHM;
	//mLam.setRange("altfitrange", s1-0.01, s2+0.02);
	mLam.setRange("altfitrange", x1, x4);
	mudouble.setVal(mu.getVal());
	sigmadouble.setVal(sigma.getVal());
	p3double.setVal(p3.getVal());
	nbkgalt.setVal(nbkg.getVal());
	nsigalt.setVal(nsig.getVal());
	RooFitResult* altresult = sumalt.fitTo(RooFitDataHist, RooFit::Range("altfitrange"), RooFit::Save());
	//sum.fitTo(RooFitDataHist, RooFit::Range("full"), RooFit::Save());
	//RooFitResult* result = sum.fitTo(RooFitDataHist, RooFit::Range("compositefitrange"), RooFit::Save());
	double FWHMalt = 2 * sigmadouble.getVal() * sqrt(2 * (pow(2, 1 / p3double.getVal()) - 1));
	double s1alt = mudouble.getVal() - 6 * FWHMalt;
	double s2alt = mudouble.getVal() + 6 * FWHMalt;
	//Now that the fit is complete, we can define the signal extraction range based on the FWHM of the Students T signal fit
	//double FWHM = 2 * sigma.getVal() * sqrt(2 * (pow(2, 1 / p3.getVal()) - 1));
	//double s1 = mu.getVal() - 6*FWHM;
	//double s2 = mu.getVal() + 6*FWHM;
	double s1extended = mu.getVal() - 8 * FWHM;
	double s2extended = mu.getVal() + 8 * FWHM;
	mLam.setRange("signal", s1, s2);
	mLam.setRange("signalalt", s1alt, s2alt);
	mLam.setRange("signalextended", s1extended, s2extended);
	TLine extractionbound1line = TLine(s1, 0, s1, 300);
	TLine extractionbound2line = TLine(s2, 0, s2, 300);
	extractionbound1line.SetLineColor(kRed);
	extractionbound2line.SetLineColor(kRed);

	cout << "b1: " << b1.getVal() << endl;
	cout << "b2: " << b2.getVal() << endl;
	cout << "b3: " << b3.getVal() << endl;
	cout << "b4: " << b4.getVal() << endl;
	cout << "b5: " << b5.getVal() << endl;

	cout << "CONVERGENCE STATUS: " << result->covQual() << endl;

	sumalt.plotOn(RooFitPlot, RooFit::Components(bgalt), RooFit::Range("full"), RooFit::LineStyle(kDashed), RooFit::LineColor(kGreen));
	sumalt.plotOn(RooFitPlot, RooFit::Range("full"), RooFit::LineColor(kGreen));

	sum.plotOn(RooFitPlot, RooFit::Components(bg), RooFit::Range("full"), RooFit::LineStyle(kDashed), RooFit::LineColor(kBlue));
	sum.plotOn(RooFitPlot, RooFit::Range("full"), RooFit::LineColor(kBlue));




	/////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////

	////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////

	f->cd();

	//Forground hist, just a copy of the input histogram
	TH1F* fgHist = (TH1F*)inHist->Clone();

	Double_t inputs[1];

	//Check Hist
	//I integrate the fit pdf in each bin and add information to the checkhist
	TH1F* checkHist = (TH1F*)inHist->Clone();
	checkHist->Reset("ICESM");
	checkHist->SetLineColor(kBlue);

	TH1F* bgcheckHist = (TH1F*)inHist->Clone();
	bgcheckHist->Reset("ICESM");
	bgcheckHist->SetLineColor(kGreen);

	TH1F* sigcheckHist = (TH1F*)inHist->Clone();
	sigcheckHist->Reset("ICESM");
	sigcheckHist->SetLineColor(kBlue);

	TH1F* altsigcheckHist = (TH1F*)inHist->Clone();
	altsigcheckHist->Reset("ICESM");
	altsigcheckHist->SetLineColor(kGreen);

	TH1F* sumcheckHist = (TH1F*)inHist->Clone();
	sumcheckHist->Reset("ICESM");
	sumcheckHist->SetLineColor(kBlue);

	TH1F* TaylorcheckHist = (TH1F*)inHist->Clone();
	TaylorcheckHist->Reset("ICESM");
	TaylorcheckHist->SetLineColor(kRed);

	//TH1F* MLPcheckHist = (TH1F*)inHist->Clone();
	//MLPcheckHist->Reset("ICESM");
	//MLPcheckHist->SetLineColor(kGreen);

	TH1F* xaxis = (TH1F*)inHist->Clone();
	xaxis->Reset("ICESM");
	xaxis->SetLineColor(kBlack);

	double kcount = 0;
	//double kcounterror = 0;
	double bgcount = 0;
	double totalcountwithbg = 0;
	double datacheck = 0;

	///////////////////////HHHHEEEERRREE
	mLam.setRange("signal2", s1, s2);
	RooFormulaVar nsum("nsum", "nsig + nbkg", RooArgList(nsig, nbkg));
	//RooAbsReal* integral_bkg = hist_pdf_bkg.createIntegral(x, NormSet(x), Range("signal"));
	//RooProduct backround_yield{ "backround_yield", "backround_yield", {*integral_bkg, N_bkg} };
	//Double_t integral_bkg_value = backround_yield.getVal();
	//Double_t integral_bkg_value_error = backround_yield.getPropagatedError(*fit_result_data, x);
	//cout << " Background Integral: " << integral_bkg_value << " +/- " << integral_bkg_value_error << endl;

	//std::unique_ptr<RooAbsReal> bgundersig{ bgnorm.createIntegral(mLam, RooFit::NormSet(mLam), RooFit::Range("signal")) }
	//RooAbsReal ig = signal.createIntegral(mLam, mLam, "signal");
	//std::unique_ptr<RooAbsReal> ig{ sigf.createIntegral(mLam, RooFit::NormSet(mLam), RooFit::Range("signal")) };
	//std::unique_ptr<RooAbsReal> ig2{ bg.createIntegral(mLam, RooFit::NormSet(mLam), RooFit::Range("signal")) };
	//std::unique_ptr<RooAbsReal> ig3{ sum.createIntegral(mLam, RooFit::NormSet(mLam), RooFit::Range("signal")) };
	std::unique_ptr<RooAbsReal> ig{ sigf.createIntegral(mLam, RooFit::NormSet(mLam), RooFit::Range("signal")) };
	std::unique_ptr<RooAbsReal> igalt{ sigfdouble.createIntegral(mLam, RooFit::NormSet(mLam), RooFit::Range("signalalt")) };
	std::unique_ptr<RooAbsReal> igextended{ sigf.createIntegral(mLam, RooFit::NormSet(mLam), RooFit::Range("signalextended")) };
	std::unique_ptr<RooAbsReal> ig2{ bg.createIntegral(mLam, RooFit::NormSet(mLam), RooFit::Range("signal")) };
	std::unique_ptr<RooAbsReal> ig3{ sum.createIntegral(mLam, RooFit::NormSet(mLam), RooFit::Range("signal")) };
	RooProduct signalyield("signalyield", "signalyield", RooArgSet(*ig, nsig));
	RooProduct altsignalyield("altsignalyield", "altsignalyield", RooArgSet(*igalt, nsigalt));
	RooProduct signalyieldextended("signalyieldextended", "signalyieldextended", RooArgSet(*igextended, nsig));
	RooProduct bgyield("bgyield", "bgyield", RooArgSet(*ig2, nbkg));
	double signalyieldvalue = signalyield.getVal();
	double altsignalyieldvalue = altsignalyield.getVal();
	double signalyieldvalueerror = signalyield.getPropagatedError(*result, mLam);
	double signalyieldvalueextended = signalyieldextended.getVal();
	double signalyieldvalueextendederror = signalyieldextended.getPropagatedError(*result, mLam);
	double bgyielddvalue = bgyield.getVal();
	cout << "Error Check: " << signalyieldvalue << " +/- " << signalyieldvalueerror << endl;
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

	//double SIGNALINT = ig->getVal() * ntot;
	double SIGNALINT = ig->getVal() * nsig.getVal();
	//double SIGNALINT = (nsig.getVal());
	//double SIGNALINT = ig->getVal() * (nsig.getVal() + nbkg.getVal());
	//double SIGNALINT = nsig.getVal();
	//double SIGNALINT = nbkg.getVal();
	double BGINT = ig2->getVal() * (nbkg.getVal());
	double lowerbound = 0;
	double upperbound = 0;
	double pdfbinintval = 0;
	double tempsignalyieldvalueerror = 0;
	double runningsignalyield = 0;
	double totalbgcount = 0;
	//
	double runningbg = 0;
	double runningTaylorfityield = 0;
	//double runningMLPfityield = 0;
	double runningrawsignalsubtraction = 0;

	for (int i = 1; i < fgHist->GetNbinsX() + 1; i++) {

		inputs[0] = ((TAxis*)fgHist->GetXaxis())->GetBinCenter(i);

		//If within signal extraction range defined by s1 and s2 then copy data to this hist
		if ((inputs[0] >= s1 && inputs[0] < s2)) {
			//Integrating the PDF fits bin by bin and checking
			lowerbound = checkHist->GetBinLowEdge(i);
			upperbound = lowerbound + checkHist->GetBinWidth(i);
			mLam.setRange("bin", lowerbound, upperbound);
			std::unique_ptr<RooAbsReal> sigig{ sigf.createIntegral(mLam, RooFit::NormSet(mLam), RooFit::Range("bin")) };
			std::unique_ptr<RooAbsReal> altsigig{ sigfdouble.createIntegral(mLam, RooFit::NormSet(mLam), RooFit::Range("bin")) };
			std::unique_ptr<RooAbsReal> sumig{ sum.createIntegral(mLam, RooFit::NormSet(mLam), RooFit::Range("bin")) };
			std::unique_ptr<RooAbsReal> bgig{ bg.createIntegral(mLam, RooFit::NormSet(mLam), RooFit::Range("bin")) };

			RooProduct tempsignalyield("tempsignalyield", "tempsignalyield", RooArgSet(*sigig, nsig));
			RooProduct alttempsignalyield("alttempsignalyield", "alttempsignalyield", RooArgSet(*altsigig, nsigalt));
			RooProduct tempsumyield("tempsumyield", "tempsumyield", RooArgSet(*sumig, nsum));
			RooProduct tempbgyield("tempbgyield", "tempbgyield", RooArgSet(*bgig, nbkg));

			double tempsig = tempsignalyield.getVal();
			double tempsigerror = tempsignalyield.getPropagatedError(*result, mLam);

			double alttempsig = alttempsignalyield.getVal();
			double alttempsigerror = alttempsignalyield.getPropagatedError(*altresult, mLam);

			double tempsumyieldvalue = tempsumyield.getVal();
			double tempsumyieldvalueerror = tempsumyield.getPropagatedError(*result, mLam);

			double tempbgyieldvalue = tempbgyield.getVal();
			double tempbgyieldvalueerror = tempbgyield.getPropagatedError(*result, mLam);

			//tempsumyieldvalue = sumig->getVal() * (ntot);
			//tempbgyieldvalue = bgig->getVal() * (nbkg.getVal());
			//tempsig = sigig->getVal() * (nsig.getVal());

			//tempbgyieldvalue = bgig->getVal() * (ntot);

			//tempsig = sigig->getVal() * (ntot);


			checkHist->SetBinContent(i, tempbgyieldvalue);
			checkHist->SetBinError(i, tempbgyieldvalueerror);

			sumcheckHist->SetBinContent(i, tempsumyieldvalue);
			sumcheckHist->SetBinError(i, tempsumyieldvalueerror);

			sigcheckHist->SetBinContent(i, tempsig);
			sigcheckHist->SetBinError(i, tempsigerror);

			altsigcheckHist->SetBinContent(i, alttempsig);
			altsigcheckHist->SetBinError(i, alttempsigerror);

			//double binTaylorfitvalue = f1->GetParameter(0) + f1->GetParameter(1) * inputs[0] + f1->GetParameter(2) * inputs[0] * inputs[0];
			double binTaylorfitvalue = 0;
			TaylorcheckHist->SetBinContent(i, binTaylorfitvalue);
			//MLPcheckHist->SetBinContent(i, mlp->Evaluate(0, inputs));
			runningTaylorfityield = runningTaylorfityield + (fgHist->GetBinContent(i) - binTaylorfitvalue);
			//runningMLPfityield = runningTaylorfityield + (fgHist->GetBinContent(i) - mlp->Evaluate(0, inputs));

			runningsignalyield = runningsignalyield + tempsig;
			//runningsignalyield = runningsignalyield + tempsumyieldvalue - tempbgyieldvalue;
			runningbg = runningbg + tempbgyieldvalue;
			runningrawsignalsubtraction = runningrawsignalsubtraction + (fgHist->GetBinContent(i) - tempbgyieldvalue);

			cout << "Lower bound: " << lowerbound << endl;
			cout << "Upper bound: " << upperbound << endl;
			//cout << "IG BIN VAL: " << igCHECK->getVal() << endl;
			cout << "pdfbinintval: " << pdfbinintval << endl;
			//delete igCHECK;
			//checkHist->SetBinContent(i, 100);
			totalcountwithbg = totalcountwithbg + fgHist->GetBinContent(i);
			totalbgcount = totalbgcount + fgHist->GetBinContent(i) - tempbgyieldvalue;
			cout << "FGHist: " << fgHist->GetBinContent(i) << endl;
			cout << "BGHist: " << checkHist->GetBinContent(i) << endl;
			cout << "FG - BGHist: " << fgHist->GetBinContent(i) - checkHist->GetBinContent(i) << endl;
			cout << "Signal: " << tempsig << endl;

		}
	}

	cout << "Particle Count 1: " << runningsignalyield << endl;
	cout << "Particle Count 2: " << runningrawsignalsubtraction << endl;
	cout << "Particle Count 3: " << signalyieldvalue << endl;
	cout << "Percent Diff: " << abs(runningsignalyield - runningrawsignalsubtraction) * 100 / runningsignalyield << endl;
	double countstaterror = sqrt(totalcountwithbg);

	//Draw MLP Fit
	//TCanvas* canvas = new TCanvas("Lambda Extraction", "Lambda Extraction", 10, 10, 800, 400);
	//c3->cd();
	//Creating Canvas and pads
	TCanvas* canvas = new TCanvas("c1", "multipads", 800, 800);
	canvas->SetName(plotname.c_str());
	TPad* pad1 = new TPad("pad1", "pad1", 0, 0.3, 1, 1.0);
	pad1->SetBottomMargin(0.01);
	//pad1->SetGridx();
	pad1->Draw();
	pad1->cd();

	TAxis* axis = fgHist->GetYaxis();
	axis->ChangeLabel(1, -1, -1, -1, -1, -1, " ");

	sumcheckHist->GetXaxis()->SetTitle("");
	sumcheckHist->GetYaxis()->SetTitle("Count");
	sumcheckHist->SetNameTitle(plotname.c_str(), plotname.c_str());
	sumcheckHist->GetXaxis()->SetRangeUser(std::min({ s1, x1, x2, x3, x4, s2 }), std::max({ s1, x1, x2, x3, x4, s2 }));
	//fgHist->Draw();
	sumcheckHist->Draw();
	sumcheckHist->SetStats(0);

	RooFitPlot->Draw("same");
	checkHist->Draw("same");
	//sumcheckHist->Draw("same");
	sigcheckHist->Draw("same");
	altsigcheckHist->Draw("same");
	xaxis->Draw("same");
	extractionbound1line.Draw("same");
	extractionbound2line.Draw("same");
	leftfitbound1line.Draw("same");
	leftfitbound2line.Draw("same");
	rightfitbound1line.Draw("same");
	rightfitbound2line.Draw("same");
	TaylorcheckHist->Draw("same");
	//MLPcheckHist->Draw("same");
	//f1->Draw("same");

	//LEGEND
	TLegend* legend = new TLegend(0.5513, 0.4144, 0.888, 0.8823);
	//yScatter3[k] = 100 * abs(LambdaRegDataCount[k][0] - LambdaRegDataCount[k][6]) / LambdaRegDataCount[k][0]; //Lambda
	legend->SetHeader(Form("#splitline{Count: %.2f #pm %.2f (stat.)}{Percent Diff from Alt Fit: %.2f}", signalyieldvalue, signalyieldvalueerror, 100 * abs(signalyieldvalue - altsignalyieldvalue) / signalyieldvalue)); // option "C" allows to center the header
	legend->AddEntry((TObject*)0, Form("#mu: %.6f #pm %.6f", mu.getVal(), mu.getError()), "");
	legend->AddEntry((TObject*)0, Form("#sigma: %.6f #pm %.6f", sigma.getVal(), sigma.getError()), "");
	legend->AddEntry((TObject*)0, Form("#nu: %.6f #pm %.6f", p3.getVal(), p3.getError()), "");
	legend->AddEntry("RooFitDataHist", "Data");
	//legend->AddEntry(fitHist, "Fit Accross Data BG", "l");
	legend->AddEntry(sigcheckHist, "Primary Fit");
	//legend->AddEntry(extractionbound1line, "Signal Extraction Range", "l");
	//legend->AddEntry(rightfitbound1line, "Left and Right BG Fit Ranges", "l");
	legend->AddEntry(altsigcheckHist, "Alt Fit");
	legend->Draw("same");
	//END LEGEND

	//Pad 2 for Ratio
	TPad* pad2 = new TPad("pad2", "pad2", 0, 0.05, 1, 0.3);
	canvas->cd();
	pad2->SetTopMargin(0.03);
	pad2->SetBottomMargin(0.4);
	//pad2->SetGridx();
	pad2->Draw();
	pad2->cd();

	TH1F* RatioHist = new TH1F();
	*RatioHist = (*inHist) / (*sumcheckHist);
	RatioHist->SetLineColor(kBlack);
	RatioHist->SetStats(0);
	RatioHist->SetTitle("");
	RatioHist->GetXaxis()->SetTitle("Mass [GeV]");
	RatioHist->GetXaxis()->SetTitleOffset(2.0);
	RatioHist->GetYaxis()->SetTitle("Data / Fit");
	RatioHist->GetXaxis()->SetRangeUser(std::min({ s1, x1, x2, x3, x4, s2 }), std::max({ s1, x1, x2, x3, x4, s2 }));

	//RatioHist->GetXaxis()->SetLabelOffset(0.015);
	//RatioHist->GetYaxis()->SetTitleSize(0.35);
	//RatioHist->GetXaxis()->SetTitleSize(0.35);
	RatioHist->GetYaxis()->SetRangeUser(0.5, 1.5);
	RatioHist->GetXaxis()->SetLabelFont(43); // Absolute font size in pixel (precision 3)
	RatioHist->GetXaxis()->SetLabelSize(15);
	RatioHist->GetYaxis()->SetLabelFont(43); // Absolute font size in pixel (precision 3)
	RatioHist->GetYaxis()->SetLabelSize(15);

	RatioHist->GetXaxis()->SetTitleFont(43);
	RatioHist->GetXaxis()->SetTitleSize(20);
	RatioHist->GetYaxis()->SetTitleFont(43);
	RatioHist->GetYaxis()->SetTitleSize(20);
	RatioHist->GetXaxis()->SetTitleOffset(4.55);
	RatioHist->GetYaxis()->SetTitleOffset(1.55);

	//TAxis* axis2 = RatioHist->GetYaxis();
	//axis2->ChangeLabel(1, -1, -1, -1, -1, -1, " ");

	RatioHist->Draw();
	//extractionbound1line.Draw("same");
	//extractionbound2line.Draw("same");

	TLine onerefline = TLine(x1, 1, x4, 1);
	//TLine(s1, 0, s1, 300);
	TLine vertrefline1 = TLine(s1, 0.5, s1, 1.5);
	TLine vertrefline2 = TLine(s2, 0.5, s2, 1.5);
	onerefline.SetLineColor(kBlack);
	vertrefline1.SetLineColor(kRed);
	vertrefline2.SetLineColor(kRed);
	onerefline.Draw("same");
	vertrefline1.Draw("same");
	vertrefline2.Draw("same");
	outputfile.cd();
	//c3->Write();
	canvas->Write();
	fgHist->Write();
	//canvas->Write();
	outputfile.Write();
	if (canvas) canvas->Close();
	f->cd();

	cout << "Stupid Check - Fit Vals: " << (ig3->getVal()) * (nsig.getVal() + nbkg.getVal()) << " Data: " << totalcountwithbg << endl;
	cout << "Yield Comp - Signal: " << (ig->getVal()) * (nsig.getVal()) << " Raw - BG Fit: " << (ig2->getVal()) * (nbkg.getVal()) - totalcountwithbg << endl;
	cout << (ig3->getVal()) * (nsig.getVal() + nbkg.getVal()) << " = " << (ig2->getVal()) * (nbkg.getVal()) << " + " << (ig->getVal()) * (nsig.getVal()) << endl;

	//return { signalyieldvalue ,countstaterror, bgyielddvalue, signalyieldvalueerror, totalcountwithbg - bgyielddvalue };
	//return { SIGNALINT ,countstaterror, bgyielddvalue, signalyieldvalueerror, totalcountwithbg - bgyielddvalue };
	//return { signalyieldvalue ,countstaterror, signalyieldvalueerror, bgyielddvalue, signalyieldvalueextended, runningrawsignalsubtraction, runningTaylorfityield, runningMLPfityield};
	return { signalyieldvalue ,countstaterror, signalyieldvalueerror, bgyielddvalue, signalyieldvalueextended, totalcountwithbg - bgyielddvalue, altsignalyieldvalue, 0, s1,s2 };

};


//Main peak extraction, composite fit with RooFit
vector<double> mlpfunctionKaon(TH1F* inHist, string plotname, int particlecode, TCanvas* c3, vector<double> fitparaminit, vector<double> rangedefs) { //when calling function, particle code deictates 0=lambda or 1=kaon MLP parameters

	//outputfile.cd();
	//Define the fit and signal ranges with script inputs
	double x1 = rangedefs[0];
	double x2 = rangedefs[1];
	double x3 = rangedefs[2];
	double x4 = rangedefs[3];
	//double x4 = 1.165;
	//double s1 = rangedefs[4];
	//double s2 = rangedefs[5];

	//define line to visualize the fitting and signal limits on the end plot
	TLine leftfitbound1line = TLine(x1, 0, x1, 200);
	TLine leftfitbound2line = TLine(x2, 0, x2, 200);
	TLine rightfitbound1line = TLine(x3, 0, x3, 200);
	TLine rightfitbound2line = TLine(x4, 0, x4, 200);
	//TLine extractionbound1line = TLine(s1, 0, s1, 300);
	//TLine extractionbound2line = TLine(s2, 0, s2, 300);

	//setting line colors of sitting and signal limits
	leftfitbound1line.SetLineColor(kMagenta);
	leftfitbound2line.SetLineColor(kMagenta);;
	rightfitbound1line.SetLineColor(kMagenta);;
	rightfitbound2line.SetLineColor(kMagenta);
	//extractionbound1line.SetLineColor(kRed);
	//extractionbound2line.SetLineColor(kRed);

	//Defining invariant mass as the observable for RooFit
	RooRealVar mLam("mLam", "mLam", x1, x4);
	//Defining different observable ranges for fitting and signal extractions
	mLam.setRange("full", x1, x4);
	//mLam.setRange("fullalt", , x4);
	mLam.setRange("left", x1, x2);
	mLam.setRange("right", x3, x4);
	//mLam.setRange("signal", s1, s2);
	mLam.setRange("compositefitrange", x2, x3);

	//Composite Fit
	// Background PDF
	//RooRealVar a0("a0", "a0", 10., .0, 1.0e6);
	RooRealVar b1("b1", "b1", 10.0, -1000, 1000);
	RooRealVar b2("b2", "b2", 1.0, -1000, 1000);
	RooRealVar b3("b3", "b3", 1.0, -1000, 1000);
	RooRealVar b4("b4", "b4", 1.0, -1000, 1000);
	RooRealVar b5("b5", "b5", 1.0, -1000, 1000);
	RooRealVar nsigalt("nsigalt", "alt # signal events", 100, 1, 1000000000);
	RooRealVar nbkgalt("nbkgalt", "alt # background events", 10, 1, 1000000000);

	RooRealVar a1("a1", "a1", 1.0, 0.05, 5.);
	RooRealVar a2("a2", "a2", -5.0, -100., -0.1);
	//RooRealVar mu("mu", "mu", fitparaminit[2], fitparaminit[2] - 0.001, fitparaminit[2] + 0.001);
	RooRealVar mu("mu", "mu", 0.49761, 0.495, 0.4985);
	RooRealVar sigma("sigma", "sigma", fitparaminit[0], 0.0040, 0.008);
	//RooRealVar sigma("sigma", "sigma", 0.001, 0.001, 0.005 );
	RooRealVar p3("p3", "p3", fitparaminit[1], 1.0, 10.0);
	//RooRealVar p3("p3", "p3", 5.0, 2.0, 10);
	//RooRealVar mudouble("mudouble", "mudouble", fitparaminit[2], fitparaminit[2] - 0.001, fitparaminit[2] + 0.001);
	RooRealVar mudouble("mudouble", "mudouble", 0.49761, 0.495, 0.4985);
	RooRealVar sigmadouble("sigmadouble", "sigmadouble", fitparaminit[0], 0.0040, 0.008);
	//RooRealVar sigmadouble("sigmadouble", "sigmadouble", 0.001, 0.001, 0.005);
	RooRealVar p3double("p3double", "p3double", fitparaminit[1], 1.0, 10.0);
	//RooRealVar p3double("p3double", "p3double", 5.0, 2.0, 10);
	RooRealVar nsig("nsig", "# signal events", 100, 1, 1000000000);
	RooRealVar nbkg("nbkg", "# background events", 10, 1, 1000000000);
	//if (particlecode == 0) { RooGenericPdf bg("bg", "TMath::Power(TMath::Max(mLam-1.0782,0.),a1)*TMath::Exp(a2*(mLam-1.0782))", RooArgSet(mLam, a1, a2)); }
	RooGenericPdf bg("bg", "TMath::Power(TMath::Max(mLam-0.2792,0.),a1)*TMath::Exp(a2*(mLam-0.2792))", RooArgSet(mLam, a1, a2));
	RooGenericPdf bgalt("bgalt", "b1 + mLam*b2 + mLam*mLam*b3 + mLam*mLam*mLam*b4 + mLam*mLam*mLam*mLam*b5", RooArgSet(mLam, b1, b2, b3, b4, b5));
	//RooGenericPdf bgalt("bgalt", "b1 + mLam*b2 + mLam*mLam*b3", RooArgSet(mLam, b1, b2, b3));
	RooGenericPdf sigf("sigf", "sigf", "TMath::Gamma((p3+1)/2)/TMath::Gamma(p3/2)/TMath::Sqrt(TMath::Pi()*p3)*TMath::Power(1.+((mLam-mu)/sigma)*((mLam-mu)/sigma)/p3,-(p3+1)/2)", RooArgSet(mLam, mu, p3, sigma));
	RooGenericPdf sigfdouble("sigfdouble", "sigfdouble", "TMath::Gamma((p3double+1)/2)/TMath::Gamma(p3double/2)/TMath::Sqrt(TMath::Pi()*p3double)*TMath::Power(1.+((mLam-mudouble)/sigmadouble)*((mLam-mudouble)/sigmadouble)/p3double,-(p3double+1)/2)", RooArgSet(mLam, mudouble, p3double, sigmadouble));
	RooAddPdf sum("sum", "sigf+bg", RooArgList(sigf, bg), RooArgList(nsig, nbkg));
	RooAddPdf sumalt("sumalt", "sigfdouble+bgalt", RooArgList(sigfdouble, bgalt), RooArgList(nsigalt, nbkgalt));


	RooDataHist RooFitDataHist("RooFitDataHist", "RooFitDataHist", mLam, inHist);

	bg.fitTo(RooFitDataHist, RooFit::Extended(kTRUE), RooFit::Range("left,right"));
	bgalt.fitTo(RooFitDataHist, RooFit::Extended(kTRUE), RooFit::Range("left,right"));
	//bg.fitTo(RooFitDataHist, RooFit::Range("left,right"), RooFit::Save());
	RooPlot* RooFitPlot = mLam.frame();
	RooFitDataHist.plotOn(RooFitPlot);

	//sum.fitTo(RooFitDataHist, RooFit::Range("full"));
	//bg.fitTo(RooFitDataHist, RooFit::Range("left,right"));
	RooFitResult* result = sum.fitTo(RooFitDataHist, RooFit::Range("full"), RooFit::Save());
	double FWHM = 2 * sigma.getVal() * sqrt(2 * (pow(2, 1 / p3.getVal()) - 1));
	double s1 = mu.getVal() - 6 * FWHM;
	double s2 = mu.getVal() + 6 * FWHM;
	//mLam.setRange("altfitrange", s1-0.01, s2+0.02);
	mLam.setRange("altfitrange", x1, x4);
	mudouble.setVal(mu.getVal());
	sigmadouble.setVal(sigma.getVal());
	p3double.setVal(p3.getVal());
	nbkgalt.setVal(nbkg.getVal());
	nsigalt.setVal(nsig.getVal());
	RooFitResult* altresult = sumalt.fitTo(RooFitDataHist, RooFit::Range("altfitrange"), RooFit::Save());
	//sum.fitTo(RooFitDataHist, RooFit::Range("full"), RooFit::Save());
	//RooFitResult* result = sum.fitTo(RooFitDataHist, RooFit::Range("compositefitrange"), RooFit::Save());
	double FWHMalt = 2 * sigmadouble.getVal() * sqrt(2 * (pow(2, 1 / p3double.getVal()) - 1));
	double s1alt = mudouble.getVal() - 6 * FWHMalt;
	double s2alt = mudouble.getVal() + 6 * FWHMalt;
	//Now that the fit is complete, we can define the signal extraction range based on the FWHM of the Students T signal fit
	//double FWHM = 2 * sigma.getVal() * sqrt(2 * (pow(2, 1 / p3.getVal()) - 1));
	//double s1 = mu.getVal() - 6*FWHM;
	//double s2 = mu.getVal() + 6*FWHM;
	double s1extended = mu.getVal() - 8 * FWHM;
	double s2extended = mu.getVal() + 8 * FWHM;
	mLam.setRange("signal", s1, s2);
	mLam.setRange("signalalt", s1alt, s2alt);
	mLam.setRange("signalextended", s1extended, s2extended);
	TLine extractionbound1line = TLine(s1, 0, s1, 300);
	TLine extractionbound2line = TLine(s2, 0, s2, 300);
	extractionbound1line.SetLineColor(kRed);
	extractionbound2line.SetLineColor(kRed);

	cout << "b1: " << b1.getVal() << endl;
	cout << "b2: " << b2.getVal() << endl;
	cout << "b3: " << b3.getVal() << endl;
	cout << "b4: " << b4.getVal() << endl;
	cout << "b5: " << b5.getVal() << endl;

	cout << "CONVERGENCE STATUS: " << result->covQual() << endl;

	sumalt.plotOn(RooFitPlot, RooFit::Components(bgalt), RooFit::Range("full"), RooFit::LineStyle(kDashed), RooFit::LineColor(kGreen));
	sumalt.plotOn(RooFitPlot, RooFit::Range("full"), RooFit::LineColor(kGreen));

	sum.plotOn(RooFitPlot, RooFit::Components(bg), RooFit::Range("full"), RooFit::LineStyle(kDashed), RooFit::LineColor(kBlue));
	sum.plotOn(RooFitPlot, RooFit::Range("full"), RooFit::LineColor(kBlue));




	/////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////

	////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////

	f->cd();

	//Forground hist, just a copy of the input histogram
	TH1F* fgHist = (TH1F*)inHist->Clone();

	Double_t inputs[1];

	//Check Hist
	//I integrate the fit pdf in each bin and add information to the checkhist
	TH1F* checkHist = (TH1F*)inHist->Clone();
	checkHist->Reset("ICESM");
	checkHist->SetLineColor(kBlue);

	TH1F* bgcheckHist = (TH1F*)inHist->Clone();
	bgcheckHist->Reset("ICESM");
	bgcheckHist->SetLineColor(kGreen);

	TH1F* sigcheckHist = (TH1F*)inHist->Clone();
	sigcheckHist->Reset("ICESM");
	sigcheckHist->SetLineColor(kBlue);

	TH1F* altsigcheckHist = (TH1F*)inHist->Clone();
	altsigcheckHist->Reset("ICESM");
	altsigcheckHist->SetLineColor(kGreen);

	TH1F* sumcheckHist = (TH1F*)inHist->Clone();
	sumcheckHist->Reset("ICESM");
	sumcheckHist->SetLineColor(kBlue);

	TH1F* TaylorcheckHist = (TH1F*)inHist->Clone();
	TaylorcheckHist->Reset("ICESM");
	TaylorcheckHist->SetLineColor(kRed);

	//TH1F* MLPcheckHist = (TH1F*)inHist->Clone();
	//MLPcheckHist->Reset("ICESM");
	//MLPcheckHist->SetLineColor(kGreen);

	TH1F* xaxis = (TH1F*)inHist->Clone();
	xaxis->Reset("ICESM");
	xaxis->SetLineColor(kBlack);

	double kcount = 0;
	//double kcounterror = 0;
	double bgcount = 0;
	double totalcountwithbg = 0;
	double datacheck = 0;

	///////////////////////HHHHEEEERRREE
	mLam.setRange("signal2", s1, s2);
	RooFormulaVar nsum("nsum", "nsig + nbkg", RooArgList(nsig, nbkg));
	//RooAbsReal* integral_bkg = hist_pdf_bkg.createIntegral(x, NormSet(x), Range("signal"));
	//RooProduct backround_yield{ "backround_yield", "backround_yield", {*integral_bkg, N_bkg} };
	//Double_t integral_bkg_value = backround_yield.getVal();
	//Double_t integral_bkg_value_error = backround_yield.getPropagatedError(*fit_result_data, x);
	//cout << " Background Integral: " << integral_bkg_value << " +/- " << integral_bkg_value_error << endl;

	//std::unique_ptr<RooAbsReal> bgundersig{ bgnorm.createIntegral(mLam, RooFit::NormSet(mLam), RooFit::Range("signal")) }
	//RooAbsReal ig = signal.createIntegral(mLam, mLam, "signal");
	//std::unique_ptr<RooAbsReal> ig{ sigf.createIntegral(mLam, RooFit::NormSet(mLam), RooFit::Range("signal")) };
	//std::unique_ptr<RooAbsReal> ig2{ bg.createIntegral(mLam, RooFit::NormSet(mLam), RooFit::Range("signal")) };
	//std::unique_ptr<RooAbsReal> ig3{ sum.createIntegral(mLam, RooFit::NormSet(mLam), RooFit::Range("signal")) };
	std::unique_ptr<RooAbsReal> ig{ sigf.createIntegral(mLam, RooFit::NormSet(mLam), RooFit::Range("signal")) };
	std::unique_ptr<RooAbsReal> igalt{ sigfdouble.createIntegral(mLam, RooFit::NormSet(mLam), RooFit::Range("signalalt")) };
	std::unique_ptr<RooAbsReal> igextended{ sigf.createIntegral(mLam, RooFit::NormSet(mLam), RooFit::Range("signalextended")) };
	std::unique_ptr<RooAbsReal> ig2{ bg.createIntegral(mLam, RooFit::NormSet(mLam), RooFit::Range("signal")) };
	std::unique_ptr<RooAbsReal> ig3{ sum.createIntegral(mLam, RooFit::NormSet(mLam), RooFit::Range("signal")) };
	RooProduct signalyield("signalyield", "signalyield", RooArgSet(*ig, nsig));
	RooProduct altsignalyield("altsignalyield", "altsignalyield", RooArgSet(*igalt, nsigalt));
	RooProduct signalyieldextended("signalyieldextended", "signalyieldextended", RooArgSet(*igextended, nsig));
	RooProduct bgyield("bgyield", "bgyield", RooArgSet(*ig2, nbkg));
	double signalyieldvalue = signalyield.getVal();
	double altsignalyieldvalue = altsignalyield.getVal();
	double signalyieldvalueerror = signalyield.getPropagatedError(*result, mLam);
	double signalyieldvalueextended = signalyieldextended.getVal();
	double signalyieldvalueextendederror = signalyieldextended.getPropagatedError(*result, mLam);
	double bgyielddvalue = bgyield.getVal();
	cout << "Error Check: " << signalyieldvalue << " +/- " << signalyieldvalueerror << endl;
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

	//double SIGNALINT = ig->getVal() * ntot;
	double SIGNALINT = ig->getVal() * nsig.getVal();
	//double SIGNALINT = (nsig.getVal());
	//double SIGNALINT = ig->getVal() * (nsig.getVal() + nbkg.getVal());
	//double SIGNALINT = nsig.getVal();
	//double SIGNALINT = nbkg.getVal();
	double BGINT = ig2->getVal() * (nbkg.getVal());
	double lowerbound = 0;
	double upperbound = 0;
	double pdfbinintval = 0;
	double tempsignalyieldvalueerror = 0;
	double runningsignalyield = 0;
	double totalbgcount = 0;
	//
	double runningbg = 0;
	double runningTaylorfityield = 0;
	//double runningMLPfityield = 0;
	double runningrawsignalsubtraction = 0;

	for (int i = 1; i < fgHist->GetNbinsX() + 1; i++) {

		inputs[0] = ((TAxis*)fgHist->GetXaxis())->GetBinCenter(i);

		//If within signal extraction range defined by s1 and s2 then copy data to this hist
		if ((inputs[0] >= s1 && inputs[0] < s2)) {
			//Integrating the PDF fits bin by bin and checking
			lowerbound = checkHist->GetBinLowEdge(i);
			upperbound = lowerbound + checkHist->GetBinWidth(i);
			mLam.setRange("bin", lowerbound, upperbound);
			std::unique_ptr<RooAbsReal> sigig{ sigf.createIntegral(mLam, RooFit::NormSet(mLam), RooFit::Range("bin")) };
			std::unique_ptr<RooAbsReal> altsigig{ sigfdouble.createIntegral(mLam, RooFit::NormSet(mLam), RooFit::Range("bin")) };
			std::unique_ptr<RooAbsReal> sumig{ sum.createIntegral(mLam, RooFit::NormSet(mLam), RooFit::Range("bin")) };
			std::unique_ptr<RooAbsReal> bgig{ bg.createIntegral(mLam, RooFit::NormSet(mLam), RooFit::Range("bin")) };

			RooProduct tempsignalyield("tempsignalyield", "tempsignalyield", RooArgSet(*sigig, nsig));
			RooProduct alttempsignalyield("alttempsignalyield", "alttempsignalyield", RooArgSet(*altsigig, nsigalt));
			RooProduct tempsumyield("tempsumyield", "tempsumyield", RooArgSet(*sumig, nsum));
			RooProduct tempbgyield("tempbgyield", "tempbgyield", RooArgSet(*bgig, nbkg));

			double tempsig = tempsignalyield.getVal();
			double tempsigerror = tempsignalyield.getPropagatedError(*result, mLam);

			double alttempsig = alttempsignalyield.getVal();
			double alttempsigerror = alttempsignalyield.getPropagatedError(*altresult, mLam);

			double tempsumyieldvalue = tempsumyield.getVal();
			double tempsumyieldvalueerror = tempsumyield.getPropagatedError(*result, mLam);

			double tempbgyieldvalue = tempbgyield.getVal();
			double tempbgyieldvalueerror = tempbgyield.getPropagatedError(*result, mLam);

			//tempsumyieldvalue = sumig->getVal() * (ntot);
			//tempbgyieldvalue = bgig->getVal() * (nbkg.getVal());
			//tempsig = sigig->getVal() * (nsig.getVal());

			//tempbgyieldvalue = bgig->getVal() * (ntot);

			//tempsig = sigig->getVal() * (ntot);


			checkHist->SetBinContent(i, tempbgyieldvalue);
			checkHist->SetBinError(i, tempbgyieldvalueerror);

			sumcheckHist->SetBinContent(i, tempsumyieldvalue);
			sumcheckHist->SetBinError(i, tempsumyieldvalueerror);

			sigcheckHist->SetBinContent(i, tempsig);
			sigcheckHist->SetBinError(i, tempsigerror);

			altsigcheckHist->SetBinContent(i, alttempsig);
			altsigcheckHist->SetBinError(i, alttempsigerror);

			//double binTaylorfitvalue = f1->GetParameter(0) + f1->GetParameter(1) * inputs[0] + f1->GetParameter(2) * inputs[0] * inputs[0];
			double binTaylorfitvalue = 0;
			TaylorcheckHist->SetBinContent(i, binTaylorfitvalue);
			//MLPcheckHist->SetBinContent(i, mlp->Evaluate(0, inputs));
			runningTaylorfityield = runningTaylorfityield + (fgHist->GetBinContent(i) - binTaylorfitvalue);
			//runningMLPfityield = runningTaylorfityield + (fgHist->GetBinContent(i) - mlp->Evaluate(0, inputs));

			runningsignalyield = runningsignalyield + tempsig;
			//runningsignalyield = runningsignalyield + tempsumyieldvalue - tempbgyieldvalue;
			runningbg = runningbg + tempbgyieldvalue;
			runningrawsignalsubtraction = runningrawsignalsubtraction + (fgHist->GetBinContent(i) - tempbgyieldvalue);

			cout << "Lower bound: " << lowerbound << endl;
			cout << "Upper bound: " << upperbound << endl;
			//cout << "IG BIN VAL: " << igCHECK->getVal() << endl;
			cout << "pdfbinintval: " << pdfbinintval << endl;
			//delete igCHECK;
			//checkHist->SetBinContent(i, 100);
			totalcountwithbg = totalcountwithbg + fgHist->GetBinContent(i);
			totalbgcount = totalbgcount + fgHist->GetBinContent(i) - tempbgyieldvalue;
			cout << "FGHist: " << fgHist->GetBinContent(i) << endl;
			cout << "BGHist: " << checkHist->GetBinContent(i) << endl;
			cout << "FG - BGHist: " << fgHist->GetBinContent(i) - checkHist->GetBinContent(i) << endl;
			cout << "Signal: " << tempsig << endl;

		}
	}

	cout << "Particle Count 1: " << runningsignalyield << endl;
	cout << "Particle Count 2: " << runningrawsignalsubtraction << endl;
	cout << "Particle Count 3: " << signalyieldvalue << endl;
	cout << "Percent Diff: " << abs(runningsignalyield - runningrawsignalsubtraction) * 100 / runningsignalyield << endl;
	double countstaterror = sqrt(totalcountwithbg);

	//Draw MLP Fit
	//TCanvas* canvas = new TCanvas("Lambda Extraction", "Lambda Extraction", 10, 10, 800, 400);
	//c3->cd();
	//Creating Canvas and pads
	TCanvas* canvas = new TCanvas("c1", "multipads", 800, 800);
	canvas->SetName(plotname.c_str());
	TPad* pad1 = new TPad("pad1", "pad1", 0, 0.3, 1, 1.0);
	pad1->SetBottomMargin(0.01);
	//pad1->SetGridx();
	pad1->Draw();
	pad1->cd();

	TAxis* axis = fgHist->GetYaxis();
	axis->ChangeLabel(1, -1, -1, -1, -1, -1, " ");

	sumcheckHist->GetXaxis()->SetTitle("");
	sumcheckHist->GetYaxis()->SetTitle("Count");
	sumcheckHist->SetNameTitle(plotname.c_str(), plotname.c_str());
	sumcheckHist->GetXaxis()->SetRangeUser(std::min({ s1, x1, x2, x3, x4, s2 }), std::max({ s1, x1, x2, x3, x4, s2 }));
	//fgHist->Draw();
	sumcheckHist->Draw();
	sumcheckHist->SetStats(0);

	RooFitPlot->Draw("same");
	checkHist->Draw("same");
	//sumcheckHist->Draw("same");
	sigcheckHist->Draw("same");
	altsigcheckHist->Draw("same");
	xaxis->Draw("same");
	extractionbound1line.Draw("same");
	extractionbound2line.Draw("same");
	leftfitbound1line.Draw("same");
	leftfitbound2line.Draw("same");
	rightfitbound1line.Draw("same");
	rightfitbound2line.Draw("same");
	TaylorcheckHist->Draw("same");
	//MLPcheckHist->Draw("same");
	//f1->Draw("same");

	//LEGEND
	TLegend* legend = new TLegend(0.5513, 0.4144, 0.888, 0.8823);
	//yScatter3[k] = 100 * abs(LambdaRegDataCount[k][0] - LambdaRegDataCount[k][6]) / LambdaRegDataCount[k][0]; //Lambda
	legend->SetHeader(Form("#splitline{Count: %.2f #pm %.2f (stat.)}{Percent Diff from Alt Fit: %.2f}", signalyieldvalue, signalyieldvalueerror, 100 * abs(signalyieldvalue - altsignalyieldvalue) / signalyieldvalue)); // option "C" allows to center the header
	//legend->AddEntry((TObject*)0, Form("#mu: %.6f #pm %.6f", mu.getVal(), mu.getError()), "");
	//legend->AddEntry((TObject*)0, Form("#sigma: %.6f #pm %.6f", sigma.getVal(), sigma.getError()), "");
	//legend->AddEntry((TObject*)0, Form("#nu: %.6f #pm %.6f", p3.getVal(), p3.getError()), "");
	legend->AddEntry((TObject*)0, Form("b1: %.6f #pm %.6f", b1.getVal(), b1.getError()), "");
	legend->AddEntry((TObject*)0, Form("b2: %.6f #pm %.6f", b2.getVal(), b2.getError()), "");
	legend->AddEntry((TObject*)0, Form("b3: %.6f #pm %.6f", b3.getVal(), b3.getError()), "");
	legend->AddEntry((TObject*)0, Form("b4: %.6f #pm %.6f", b4.getVal(), b4.getError()), "");
	legend->AddEntry((TObject*)0, Form("b5: %.6f #pm %.6f", b5.getVal(), b5.getError()), "");
	legend->AddEntry("RooFitDataHist", "Data");
	//legend->AddEntry(fitHist, "Fit Accross Data BG", "l");
	legend->AddEntry(sigcheckHist, "Primary Fit");
	//legend->AddEntry(extractionbound1line, "Signal Extraction Range", "l");
	//legend->AddEntry(rightfitbound1line, "Left and Right BG Fit Ranges", "l");
	legend->AddEntry(altsigcheckHist, "Alt Fit");
	legend->Draw("same");
	//END LEGEND

	//Pad 2 for Ratio
	TPad* pad2 = new TPad("pad2", "pad2", 0, 0.05, 1, 0.3);
	canvas->cd();
	pad2->SetTopMargin(0.03);
	pad2->SetBottomMargin(0.4);
	//pad2->SetGridx();
	pad2->Draw();
	pad2->cd();

	TH1F* RatioHist = new TH1F();
	*RatioHist = (*inHist) / (*sumcheckHist);
	RatioHist->SetLineColor(kBlack);
	RatioHist->SetStats(0);
	RatioHist->SetTitle("");
	RatioHist->GetXaxis()->SetTitle("Mass [GeV]");
	RatioHist->GetXaxis()->SetTitleOffset(2.0);
	RatioHist->GetYaxis()->SetTitle("Data / Fit");
	RatioHist->GetXaxis()->SetRangeUser(std::min({ s1, x1, x2, x3, x4, s2 }), std::max({ s1, x1, x2, x3, x4, s2 }));

	//RatioHist->GetXaxis()->SetLabelOffset(0.015);
	//RatioHist->GetYaxis()->SetTitleSize(0.35);
	//RatioHist->GetXaxis()->SetTitleSize(0.35);
	RatioHist->GetYaxis()->SetRangeUser(0.5, 1.5);
	RatioHist->GetXaxis()->SetLabelFont(43); // Absolute font size in pixel (precision 3)
	RatioHist->GetXaxis()->SetLabelSize(15);
	RatioHist->GetYaxis()->SetLabelFont(43); // Absolute font size in pixel (precision 3)
	RatioHist->GetYaxis()->SetLabelSize(15);

	RatioHist->GetXaxis()->SetTitleFont(43);
	RatioHist->GetXaxis()->SetTitleSize(20);
	RatioHist->GetYaxis()->SetTitleFont(43);
	RatioHist->GetYaxis()->SetTitleSize(20);
	RatioHist->GetXaxis()->SetTitleOffset(4.55);
	RatioHist->GetYaxis()->SetTitleOffset(1.55);

	//TAxis* axis2 = RatioHist->GetYaxis();
	//axis2->ChangeLabel(1, -1, -1, -1, -1, -1, " ");

	RatioHist->Draw();
	//extractionbound1line.Draw("same");
	//extractionbound2line.Draw("same");

	TLine onerefline = TLine(x1, 1, x4, 1);
	//TLine(s1, 0, s1, 300);
	TLine vertrefline1 = TLine(s1, 0.5, s1, 1.5);
	TLine vertrefline2 = TLine(s2, 0.5, s2, 1.5);
	onerefline.SetLineColor(kBlack);
	vertrefline1.SetLineColor(kRed);
	vertrefline2.SetLineColor(kRed);
	onerefline.Draw("same");
	vertrefline1.Draw("same");
	vertrefline2.Draw("same");
	outputfile.cd();
	//c3->Write();
	canvas->Write();
	fgHist->Write();
	//canvas->Write();
	outputfile.Write();
	if (canvas) canvas->Close();
	f->cd();

	cout << "Stupid Check - Fit Vals: " << (ig3->getVal()) * (nsig.getVal() + nbkg.getVal()) << " Data: " << totalcountwithbg << endl;
	cout << "Yield Comp - Signal: " << (ig->getVal()) * (nsig.getVal()) << " Raw - BG Fit: " << (ig2->getVal()) * (nbkg.getVal()) - totalcountwithbg << endl;
	cout << (ig3->getVal()) * (nsig.getVal() + nbkg.getVal()) << " = " << (ig2->getVal()) * (nbkg.getVal()) << " + " << (ig->getVal()) * (nsig.getVal()) << endl;

	//return { signalyieldvalue ,countstaterror, bgyielddvalue, signalyieldvalueerror, totalcountwithbg - bgyielddvalue };
	//return { SIGNALINT ,countstaterror, bgyielddvalue, signalyieldvalueerror, totalcountwithbg - bgyielddvalue };
	//return { signalyieldvalue ,countstaterror, signalyieldvalueerror, bgyielddvalue, signalyieldvalueextended, runningrawsignalsubtraction, runningTaylorfityield, runningMLPfityield};
	return { signalyieldvalue ,countstaterror, signalyieldvalueerror, bgyielddvalue, signalyieldvalueextended, totalcountwithbg - bgyielddvalue, altsignalyieldvalue, 0, s1,s2 };

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
	double PionNuclCorrWeight;
	int s61;
	int fRunType;
	string invMassName[2] = { "invMasslambda","invMasskaon" };
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
	//tree->SetBranchAddress("PionNuclCorrWeight", &PionNuclCorrWeight);
	tree->SetBranchAddress("fRunType", &fRunType);
	//tree->SetBranchAddress("SVz", &SVz);
	PionNuclCorrWeight = 0;
	int m;
	int j;
	//Have to enforce pT and Eta bounds here
	for (m = 0; m < entries; m++) {
		tree->GetEntry(m);
		for (j = 0; j < binentries; j++) {
			if (s61 > 0) {
				if (pT > lowPtbound && pT < 3.5) {
					//if (abs(Eta) < 1.3) {
					if (Eta < 1.3 && Eta> -1.3) {
						if (Ys > lowerYsbound && Ys < upperYsbound) {
							if (Q2s > 5 && Q2s < 100) {
								distvariable = { Q2s,Q2s / (Ys * 101568),pT,Eta, Phi,sqrt((SVx) * (SVx)+(SVy) * (SVy)), SVz };
								if (abs(SVz) <= 30.0) {
									//cout << "Nuclear Correction Weight: " << PionNuclCorrWeight << endl;
									//cout << "Run Type: " << fRunType << endl;
									if (distvariable[distvariableindex] > binsvector[j] && distvariable[distvariableindex] < binsvector[j + 1]) {
										
										HistArray[j]->Fill(invMass, weight1 * weight2);
										//if (PionNuclCorrWeight < 0.015) { //If this is Data
										//	HistArray[j]->Fill(invMass, weight1 * weight2);
										//}
										//if (PionNuclCorrWeight > 0.015) { //If this is MC
										//	HistArray[j]->Fill(invMass, weight1 * weight2 * PionNuclCorrWeight);
										//}

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
		//ExtractionDataVector.push_back(mlpfunction(HistArray[j], extractionReadback.c_str(), particlecode, c3, binparaminits[j], binrangedefs[j]));
		if (particlecode == 0) { ExtractionDataVector.push_back(mlpfunctionLambda(HistArray[j], extractionReadback.c_str(), particlecode, c3, binparaminits[j], binrangedefs[j])); }
		if (particlecode == 1) { ExtractionDataVector.push_back(mlpfunctionKaon(HistArray[j], extractionReadback.c_str(), particlecode, c3, binparaminits[j], binrangedefs[j])); }
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
			if (pT > lowPtbound && pT < 3.5) {
				if (abs(Eta) < 1.3) {
					if (Ys > lowerYsbound && Ys < upperYsbound) {
						if (Q2s > 5 && Q2s < 100) {
							//if (GenElecEn>11) {
								//if (Empz>35 && Empz<70) {
							if (abs(VtxZgen) < 30) {
								if (abs(DecayZVtx) < 30.0) {
									distvariable = { Q2s, Q2s / (Ys * 101568), pT, Eta, genPhi, sqrt((DecayXVtx) * (DecayXVtx)+(DecayYVtx) * (DecayYVtx)), DecayZVtx };
									if (distvariable[distvariableindex] > binsvector[j] && distvariable[distvariableindex] < binsvector[j + 1]) {
										GenCounts[j] = GenCounts[j] + 1 * weight1 * weight2;
										//GenCounts[j] = GenCounts[j] + 1;
									}
								}
							}
							//}
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

//Function plotting cross section data
int PlotK0CSData(string plotcanvasname, string plottitle, string xaxistitle, string yaxistitle, double ylowbound, double yupbound, TGraphErrors* cs, int binentries, double X[binentries], double y[binentries], double xerr[binentries], double y1err[binentries], double dylow[binentries], double dyup[binentries], double dylow2[binentries], double dyup2[binentries]) { //when calling function, particle code deictates 0=lambda or 1=kaon MLP parameters

	//double xerr[q2entries] = { 1,1.5,2.5,5,37.5 };
	//double xvals[q2entries] = { 6,8.5,12.5,20,62.5 };
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//K0s Cross section results from HERAII Preliminary
	double X2[4];
	double y2[4];
	double dxlow2[4];
	double dxup2[4];
	double dylow3[4];
	double dyup3[4];
	int n = 0;
	X2[n] = 8.5;	y2[n] = 0.94825;	dxlow2[n] = 1.5;	dxup2[n] = 1.5;	dylow3[n] = 0;	dyup3[n] = 0;	n++;
	X2[n] = 12.5;	y2[n] = 0.514681;	dxlow2[n] = 2.5;	dxup2[n] = 2.5;	dylow3[n] = 0;	dyup3[n] = 0;	n++;
	X2[n] = 20;	y2[n] = 0.240309;	dxlow2[n] = 5;	dxup2[n] = 5;	dylow3[n] = 0;	dyup3[n] = 0;	n++;
	X2[n] = 62.5;	y2[n] = 0.0408826;	dxlow2[n] = 37.5;	dxup2[n] = 37.5;	dylow3[n] = 0;	dyup3[n] = 0;	n++;
	////
	gPad->SetLogx(1);
	gPad->SetLogy(1);
	TCanvas* c5 = new TCanvas(plotcanvasname.c_str(), plotcanvasname.c_str(), 10, 10, 800, 400);
	TGraphAsymmErrors* xyscan = new TGraphAsymmErrors(binentries, X, y, xerr, xerr, dylow, dyup);
	TGraphAsymmErrors* xyscan2 = new TGraphAsymmErrors(binentries, X, y, xerr, xerr, dylow2, dyup2);
	TGraphAsymmErrors* H1prelim = new TGraphAsymmErrors(binentries-1, X2, y2, dxup2, dxup2, dylow3, dyup3);
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
	H1prelim->SetMarkerSize(0.01);
	H1prelim->SetMarkerStyle(21);
	H1prelim->SetMarkerColor(kBlue);
	H1prelim->SetLineColor(kBlue);
	cs->SetMarkerStyle(21);
	cs->SetMarkerColor(kRed);
	cs->SetLineColor(kRed);
	TMultiGraph* mgcs = new TMultiGraph();

	mgcs->Add(cs, "AP");
	//mgcs->Add(xyscan, "AP");
	//mgcs->Add(xyscan2, "AP");
	//mgcs->Add(H1prelim, "AP");
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
	legend2->AddEntry(cs, "HERAII Analysis in Progress (2006 DST7)", "l");
	//legend2->AddEntry(xyscan, "HERAI H1 Publication (1999-2000 DST5)", "l");
	//legend2->AddEntry(H1prelim, "HERAII H1 Prelim (2006 DST7)", "l");
	legend2->Draw("SAME");
	c5->SetGridy();
	outputfile.cd();
	cs->SetName((plotcanvasname + " TGraph").c_str());
	cs->Write();
	c5->Write();
	f->cd();
	gPad->SetLogx(0);
	gPad->SetLogy(0);
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


void CrossSections(int varidx, vector<double> q2bins, double DataIntLumi06, double MCDjangoh14radgen, double MCDjangoh14nonradgen, string filekaonTree, string filelambdaTree, string filekaonTreeMC, string filelambdaTreeMCRAPGAP, string filelambdaTreeMCDJANGOH, string filegenMCTreeRAPGAP, string filegenMCTreeDJANGOH, string fileNonRadgenMCTree, string fileRadgenMCTree, string xaxistitle, vector<vector<double>> binparaminitsLambda, vector<vector<double>> binrangedefsLambda, vector<vector<double>> binparaminitsKaon, vector<vector<double>> binrangedefsKaon, vector<vector<double>> binparaminitsMCLambda, vector<vector<double>> binrangedefsMCLambda, vector<vector<double>> binparaminitsMCKaon, vector<vector<double>> binrangedefsMCKaon) {
	//defining arrays of histograms corresponding to particles within kinematic bins
	//must be done for each particle, for data
	//Q2 data
	vector<TH1F*> LambdaTotDataQ2;
	//vector<TH1F*> LambdaRegDataQ2;
	//vector<TH1F*> LambdaBarDataQ2;
	vector<TH1F*> LambdaTotMCQ2;
	//vector<TH1F*> LambdaRegMCQ2;
	//vector<TH1F*> LambdaBarMCQ2;
	vector<TH1F*> KaonDataQ2;
	vector<TH1F*> KaonMCQ2;


	//I don't have Histograms for particle level generator data because counts for these
	//will be done by just going through the generate TTree and adding events up that are within our conditions
	for (int a = 0; a < q2entries; a++) {
		string namestr1 = "Lambda Tot Data q2 Bin " + to_string(a + 1);
		//string namestr2 = "Lambda Reg Data q2 Bin " + to_string(a + 1);
		//string namestr3 = "Lambda Bar Data q2 Bin " + to_string(a + 1);
		string namestr4 = "Lambda Tot MC q2 Bin " + to_string(a + 1);
		//string namestr5 = "Lambda Reg MC q2 Bin " + to_string(a + 1);
		//string namestr6 = "Lambda Bar MC q2 Bin " + to_string(a + 1);
		string namestr7 = "Kaon Data q2 Bin " + to_string(a + 1);
		string namestr8 = "Kaon MC q2 Bin " + to_string(a + 1);

		
		//LambdaTotDatapT[a] = new TH1F(namestr1.c_str(), namestr1.c_str(), lambdabins, lambdalowerbinrange, lambdaupperbinrange);
		LambdaTotDataQ2.push_back(new TH1F(namestr1.c_str(), namestr1.c_str(), lambdabins, lambdalowerbinrange, lambdaupperbinrange));
		//LambdaRegDataQ2.push_back(new TH1F(namestr2.c_str(), namestr2.c_str(), lambdabins, lambdalowerbinrange, lambdaupperbinrange));
		//LambdaBarDataQ2.push_back(new TH1F(namestr3.c_str(), namestr3.c_str(), lambdabins, lambdalowerbinrange, lambdaupperbinrange));
		LambdaTotMCQ2.push_back(new TH1F(namestr4.c_str(), namestr4.c_str(), lambdabins, lambdalowerbinrange, lambdaupperbinrange));
		//LambdaRegMCQ2.push_back(new TH1F(namestr5.c_str(), namestr5.c_str(), lambdabins, lambdalowerbinrange, lambdaupperbinrange));
		//LambdaBarMCQ2.push_back(new TH1F(namestr6.c_str(), namestr6.c_str(), lambdabins, lambdalowerbinrange, lambdaupperbinrange));
		KaonDataQ2.push_back(new TH1F(namestr7.c_str(), namestr7.c_str(), kaonbins, kaonlowerbinrange, kaonupperbinrange));
		KaonMCQ2.push_back(new TH1F(namestr8.c_str(), namestr8.c_str(), kaonbins, kaonlowerbinrange, kaonupperbinrange));
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
	vector<vector<double>> LambdaTotDataCount;
	//vector<vector<double>> LambdaRegDataCount;
	//vector<vector<double>> LambdaBarDataCount;
	vector<vector<double>> LambdaTotMCCount;
	//vector<vector<double>> LambdaRegMCCount;
	//vector<vector<double>> LambdaBarMCCount;
	vector<vector<double>> KaonDataCount;
	vector<vector<double>> KaonMCCount;
	
	//Gen particle count
	vector<double> LambdaTotGenCount;
	//vector<double> LambdaRegGenCount;
	//vector<double> LambdaBarGenCount;
	vector<double> KaonGenCount;
	//Gen particle counting for Rad Corrections
	vector<double> LambdaGenCountRad;
	vector<double> KaonGenCountRad;
	vector<double> LambdaGenCountNonRad;
	vector<double> KaonGenCountNonRad;

	//Analyzing Tot Lambda Counts for q2 bins for MC and Data
	LambdaTotDataCount = AnalyzeTree(varidx, new TFile(filelambdaTree.c_str(), "read"), "Lambda Tot", q2entries, 0, LambdaTotDataQ2, q2bins, "Lambda Tot Data q2 Extraction ", c3, c1, binparaminitsLambda, binrangedefsLambda);
	LambdaTotMCCount = AnalyzeTree(varidx, new TFile(filelambdaTreeMCRAPGAP.c_str(), "read"), "Lambda Tot", q2entries, 0, LambdaTotMCQ2, q2bins, "Lambda Tot MC q2 Extraction ", c3, c1, binparaminitsMCLambda, binrangedefsMCLambda);

	//LambdaRegDataCount = AnalyzeTree(varidx, new TFile(filelambdaTree.c_str(), "read"), "Lambda Reg", q2entries, 0, LambdaRegDataQ2, q2bins, "Lambda Reg Data q2 Extraction ", c3, c1);
	//LambdaRegMCCount = AnalyzeTree(varidx, new TFile(filelambdaTreeMC.c_str(), "read"), "Lambda Reg", q2entries, 0, LambdaRegMCQ2, q2bins, "Lambda Reg MC q2 Extraction ", c3, c1);

	//LambdaBarDataCount = AnalyzeTree(varidx, new TFile(filelambdaTree.c_str(), "read"), "Lambda Bar", q2entries, 0, LambdaBarDataQ2, q2bins, "Lambda Bar Data q2 Extraction ", c3, c1);
	//LambdaBarMCCount = AnalyzeTree(varidx, new TFile(filelambdaTreeMC.c_str(), "read"), "Lambda Bar", q2entries, 0, LambdaBarMCQ2, q2bins, "Lambda Bar MC q2 Extraction ", c3, c1);

	KaonDataCount = AnalyzeTree(varidx, new TFile(filekaonTree.c_str(), "read"), "K0", q2entries, 1, KaonDataQ2, q2bins, "Kaon Tot Data q2 Extraction ", c3, c1, binparaminitsKaon, binrangedefsKaon);
	KaonMCCount = AnalyzeTree(varidx, new TFile(filekaonTreeMC.c_str(), "read"), "K0", q2entries, 1, KaonMCQ2, q2bins, "Kaon Tot MC q2 Extraction ", c3, c1, binparaminitsMCKaon, binrangedefsMCKaon);

	//Analyzing Gen Counts
	LambdaTotGenCount = AnalyzeGenTree(varidx, new TFile(filegenMCTreeRAPGAP.c_str(), "read"), "Lambda Tot Channel", q2entries, q2bins);
	//LambdaRegGenCount = AnalyzeGenTree(varidx, new TFile(filegenMCTree.c_str(), "read"), "Lambda Reg Channel", q2entries, q2bins);
	//LambdaBarGenCount = AnalyzeGenTree(varidx, new TFile(filegenMCTree.c_str(), "read"), "Lambda Bar Channel", q2entries, q2bins);
	KaonGenCount = AnalyzeGenTree(varidx, new TFile(filegenMCTreeRAPGAP.c_str(), "read"), "K0s Channel", q2entries, q2bins);
	
	//Analyzing Gen counts for radiative and non Radiative MC
	//Non Radiative Gen Counting for Radiative Corrections
	//LambdaTotGenCountinclNonRad = AnalyzeGenTreeRadCorr(new TFile(fileNonRadgenMCTree.c_str(), "read"), "Lambda Tot", 1, { q2bins[0],q2bins[q2entries] });
	//LambdaTotGenCountinclRad = AnalyzeGenTreeRadCorr(new TFile(filegenMCTree.c_str(), "read"), "Lambda Tot", 1, { q2bins[0],q2bins[q2entries] });
	//KaonGenCountinclNonRad = AnalyzeGenTreeRadCorr(new TFile(fileNonRadgenMCTree.c_str(), "read"), "K0s", 1, { q2bins[0],q2bins[q2entries] });
	//KaonGenCountinclRad = AnalyzeGenTreeRadCorr(new TFile(filegenMCTree.c_str(), "read"), "K0s", 1, { q2bins[0],q2bins[q2entries] });
	KaonGenCountRad = AnalyzeGenTreeRadCorr(varidx, new TFile(fileRadgenMCTree.c_str(), "read"), "K0s", q2entries, q2bins);
	KaonGenCountNonRad = AnalyzeGenTreeRadCorr(varidx, new TFile(fileNonRadgenMCTree.c_str(), "read"), "K0s", q2entries, q2bins);
	LambdaGenCountRad = AnalyzeGenTreeRadCorr(varidx, new TFile(fileRadgenMCTree.c_str(), "read"), "Lambda Tot", q2entries, q2bins);
	LambdaGenCountNonRad = AnalyzeGenTreeRadCorr(varidx, new TFile(fileNonRadgenMCTree.c_str(), "read"), "Lambda Tot", q2entries, q2bins);

	cout << "KaonGenCountRad: " << KaonGenCountRad[0] << endl;
	cout << "KaonGenCountNonRad: " << KaonGenCountNonRad[0] << endl;
	cout << "LambdaGenCountRad: " << LambdaGenCountRad[0] << endl;
	cout << "LambdaGenCountNonRad: " << LambdaGenCountNonRad[0] << endl;

	double y1[q2entries] = {};
	double ourresultsoverpaper[q2entries] = {};
	double ourresultsoverpaperk0[q2entries] = {};
	double ourresultsoverprelimk0[q2entries-1] = {};
	//vector<double> q2bins = { 5.0, 7.0, 10.0, 15.0, 25.0, 100.0 };
	//double xerr[q2entries] = { 1,1.5,2.5,5,37.5 };
	//double xvals[q2entries] = { 6,8.5,12.5,20,62.5 };
	double xerr[q2entries] = {};
	double xvals[q2entries] = {};
	double y1err[q2entries] = {};
	for (int d = 0; d < q2entries; d++) {
		xerr[d] = abs(q2bins[d+1]-q2bins[d])/2.0;
		cout << "Xerr: " << xerr[d] << endl;
		xvals[d] = q2bins[d]+xerr[d];
		cout << "Xvals: " << xvals[d] << endl;
	}
	//double q2[q2entries] = { 0.55,0.65,0.75,0.85,1.0,1.2,1.45,1.9,2.85 };
	//double dx[q2entries] = { 0.1,0.1,0.1,0.1,0.2,0.2,0.3,0.6,1.3 };

	double effregplot[q2entries] = {};
	double effbarplot[q2entries] = {};
	double erreffregplot[q2entries] = {};
	double erreffbarplot[q2entries] = {};
	double lambdaradcorrplot[q2entries] = {};
	double k0radcorrplot[q2entries] = {};
	double lambdaradcorrploterrors[q2entries] = {};

	//Lambda Cross section results from 2009 H1 Paper
	double X[5];
	double y[5];
	double dxlow[5];
	double dxup[5];
	double dylow[5];
	double dyup[5];
	int n = 0;
	X[n] = xvals[n];	y[n] = 0.462;	dxlow[n] = 0;	dxup[n] = 0;	dylow[n] = 0.009;	dyup[n] = 0.009;	n++;
	X[n] = xvals[n];	y[n] = 0.282;	dxlow[n] = 0;	dxup[n] = 0;	dylow[n] = 0.006;	dyup[n] = 0.006;	n++;
	X[n] = xvals[n];	y[n] = 0.153;	dxlow[n] = 0;	dxup[n] = 0;	dylow[n] = 0.003;	dyup[n] = 0.003;	n++;
	X[n] = xvals[n];	y[n] = 0.071;	dxlow[n] = 0;	dxup[n] = 0;	dylow[n] = 0.0015;	dyup[n] = 0.0015;	n++;
	X[n] = xvals[n];	y[n] = 0.0120;	dxlow[n] = 0;	dxup[n] = 0;	dylow[n] = 0.0002;	dyup[n] = 0.0002;	n++;
	////
	

	for (int k = 0; k < q2entries; k++) {
		double templameff = (LambdaTotMCCount[k][0]) / (LambdaTotGenCount[k]);
		double templamefferr = GetRatioError(templameff, LambdaTotMCCount[k][0], LambdaTotMCCount[k][1], LambdaTotGenCount[k], 0);
		
		//cout << "Lambda Rad Correction: " << (LambdaTotGenCountinclRad[0] / LambdaTotGenCountinclNonRad[0]) * (MCDjangoh14nonradgen / MCDjangoh14radgen) << endl;
		double templambdaradcorr = (LambdaGenCountRad[k] / LambdaGenCountNonRad[k]) * (MCDjangoh14nonradgen / MCDjangoh14radgen);
		lambdaradcorrplot[k] = (templambdaradcorr-1)*100;
		lambdaradcorrploterrors[k] = 0;
		cout << "Lambda Rad Cor Q2 Bin " << k + 1 << ": " << templambdaradcorr << endl;

		double templamcrosssection = (LambdaTotDataCount[k][0]) / (templameff * BRlambda * DataIntLumi06 * templambdaradcorr * (q2bins[k+1]-q2bins[k]));
		//cout << "Lambda Tot Data Count: " << LambdaTotDataCount[k][0] << endl;
		//cout << "Lambda Bin Cross Section: " << templamcrosssection << endl;
		//cout << "Lambda BR: " << BRlambda << endl;
		//cout << "Lambda Bin Eff: " << templameff << endl;
		//cout << "Lambda Tot Data Lumi: " << DataIntLumi06 << endl;
		//cout << "Bin Width: " << q2bins[k + 1] - q2bins[k] << endl;
		cout << "Our Results Over H1 Results in Bin " << k+1 << " : " << templamcrosssection/y[k] << endl;
		ourresultsoverpaper[k] = templamcrosssection / y[k];

		double templamcrosssectionerr = GetRatioError(templamcrosssection, (LambdaTotDataCount[k][0]), (LambdaTotDataCount[k][1]), (templameff * BRlambda * DataIntLumi06* templambdaradcorr * (q2bins[k + 1] - q2bins[k])), templamefferr);
		
		y1[k] = templamcrosssection;
		y1err[k] = templamcrosssectionerr;
		effregplot[k] = templameff;
		//effbarplot[k] = templambareff;
		erreffregplot[k] = templamefferr;
		//erreffbarplot[k] = templambarefferr;

		//y2[k - 1] = 100 * (1-(countdata[k][0]) / (bgref));
		//y1err[k - 1] = (100*(countdata[k][0]) / (countref))*sqrt((Square(countdata[k][1])/(Square(countdata[k][0])))+Square(errref)/(Square(countref)));
		//y2err[k - 1] = (100 * (countdata[k][2]) / (bgref))*sqrt((Square(countdata[k][1]) / (Square(countdata[k][2]))) + Square(errref) / (Square(bgref)));
	}

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//Lambda Cross section results from 2009 H1 Paper
	/*double X[5];
	double y[5];
	double dxlow[5];
	double dxup[5];
	double dylow[5];
	double dyup[5];
	int n = 0;
	X[n] = xvals[n];	y[n] = 0.462;	dxlow[n] = 0;	dxup[n] = 0;	dylow[n] = 0.009;	dyup[n] = 0.009;	n++;
	X[n] = xvals[n];	y[n] = 0.282;	dxlow[n] = 0;	dxup[n] = 0;	dylow[n] = 0.006;	dyup[n] = 0.006;	n++;
	X[n] = xvals[n];	y[n] = 0.153;	dxlow[n] = 0;	dxup[n] = 0;	dylow[n] = 0.003;	dyup[n] = 0.003;	n++;
	X[n] = xvals[n];	y[n] = 0.071;	dxlow[n] = 0;	dxup[n] = 0;	dylow[n] = 0.0015;	dyup[n] = 0.0015;	n++;
	X[n] = xvals[n];	y[n] = 0.0120;	dxlow[n] = 0;	dxup[n] = 0;	dylow[n] = 0.0002;	dyup[n] = 0.0002;	n++;*/
	////

	double dylow2[5];
	double dyup2[5];
	int m = 0;
	dylow2[m] = 0.03;	dyup2[m] = 0.028;	m++;
	dylow2[m] = 0.02;	dyup2[m] = 0.019;	m++;
	dylow2[m] = 0.009;	dyup2[m] = 0.009;	m++;
	dylow2[m] = 0.004;	dyup2[m] = 0.004;	m++;
	dylow2[m] = 0.0006;	dyup2[m] = 0.0006;	m++;
	
	
	//Plotting Lambda Efficiency
	TCanvas* ceff = new TCanvas("Lambda Cross Section vs q2", "Lambda Cross Section vs q2", 10, 10, 800, 400);
	ceff->SetName("Lambda Reconstruction Eff vs q2");

	auto effreg = new TGraphErrors(q2entries, xvals, effregplot, xerr, erreffregplot);
	//auto effbar = new TGraphErrors(arraysize, X, effbarplot, xerr, erreffbarplot);
	effreg->SetMarkerColor(kRed);
	effreg->SetLineColor(kRed);
	effreg->SetMarkerSize(0.01);
	//effbar->SetMarkerColor(kBlue);
	//effbar->SetLineColor(kBlue);
	//effbar->SetMarkerSize(0.5);

	effreg->SetTitle("#Lambda Reconstruction Eff vs q2");
	effreg->GetXaxis()->SetTitle("q2");
	effreg->GetYaxis()->SetTitle("Efficiency");
	effreg->Draw();
	//effbar->Draw("SAME");
	TLegend* legend3 = new TLegend();
	legend3->AddEntry(effreg, "#Lambda", "l");
	//legend3->AddEntry(effbar, "#bar{#Lambda}", "l");
	legend3->Draw("SAME");
	outputfile.cd();
	ceff->Write();
	f->cd();
	//End Plotting Lambda Efficiency

	auto cs = new TGraphErrors(q2entries, xvals, y1, xerr, y1err);
	PlotData("Lambda Cross Section vs q2", "d#sigma(ep #rightarrow e [#Lambda + #bar{#Lambda}] X)/dx", "q2", "d#sigma/dx",0,4.2, cs, q2entries, xvals, y, xerr, y1err, dylow, dyup, dylow2, dyup2);
	for (int k = 0; k < q2entries; k++) {
		cout << "Lambda Q2 Cross Section Entry " << k + 1 << " : " << y1[k] << " X: " << xvals[k] << "p/m: " << y1err[k] << endl;
	}
	///*auto g = new TGraph(5, X, ourresultsoverpaper);
	//g->SetTitle("Our Results Over Paper's;Q^{2};Cross Section Ratio");
	//g->Draw("AC*");
	//outputfile.cd();
	//g->Write();
	//f->cd();*/
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	////Lambda Asymm Data from 2009 paper obtained through xyscan, no syst error published for this, so just stat. error
	//
	//n = 0;

	//X[n] = xvals[n];	y[n] = -0.0160338;	dxlow[n] = 0;	dxup[n] = 0;	dylow[n] = 0.0388186;	dyup[n] = 0.0388186;	n++;
	//X[n] = xvals[n];	y[n] = 0.0177215;	dxlow[n] = 0;	dxup[n] = 0;	dylow[n] = 0.0438819;	dyup[n] = 0.0438819;	n++;
	//X[n] = xvals[n];	y[n] = -0.0683544;	dxlow[n] = 0;	dxup[n] = 0;	dylow[n] = 0.0337553;	dyup[n] = 0.0337553;	n++;
	//X[n] = xvals[n];	y[n] = -0.0160338;	dxlow[n] = 0;	dxup[n] = 0;	dylow[n] = 0.0472574;	dyup[n] = 0.0472574;	n++;
	//X[n] = xvals[n];	y[n] = 0.0953586;	dxlow[n] = 0;	dxup[n] = 0;	dylow[n] = 0.0320675;	dyup[n] = 0.0320675;	n++;
	//
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//
	//for (int k = 0; k < q2entries; k++) {
	//	double templamregeff = (LambdaRegMCCount[k][0]) / (LambdaRegGenCount[k]);
	//	double templambareff = (LambdaBarMCCount[k][0]) / (LambdaBarGenCount[k]);
	//	double templamregefferr = GetRatioError(templamregeff, LambdaRegMCCount[k][0], LambdaRegMCCount[k][1], LambdaRegGenCount[k], 0);
	//	double templambarefferr = GetRatioError(templambareff, LambdaBarMCCount[k][0], LambdaBarMCCount[k][1], LambdaBarGenCount[k], 0);
	//	double templamreg = (LambdaRegDataCount[k][0]) / (templamregeff);
	//	double templambar = (LambdaBarDataCount[k][0]) / (templambareff);
	//	double templamregerr = GetRatioError((LambdaRegDataCount[k][0]) / (templamregeff), (LambdaRegDataCount[k][0]), (LambdaRegDataCount[k][1]), (templamregeff), templamregefferr);
	//	double templambarerr = GetRatioError((LambdaBarDataCount[k][0]) / (templambareff), (LambdaBarDataCount[k][0]), (LambdaBarDataCount[k][1]), (templambareff), templambarefferr);
	//	y1[k] = (templamreg - templambar) / ((templamreg + templambar));
	//	y1err[k] = GetRatioError(((templamreg - templambar) / ((templamreg + templambar))), (templamreg - templambar), (templamregerr + templambarerr), ((templamreg + templambar)), ((templamregerr + templambarerr)));
	//	
	//	//cout << k+1 << " Lambda reg eff err: " << templamregefferr << endl;
	//	//cout << k+1 << " Lambda bar eff err: " << templambarefferr << endl;
	//	//cout << k+1 << " Lambda Reg err: " << templamregerr << endl;
	//	//cout << k+1 << " Lambda Bar err: " << templambarerr << endl;
	//	//cout << k+1 << " Asymm err: " << y1err[k] << endl;

	//	effregplot[k] = templamregeff;
	//	effbarplot[k] = templambareff;
	//	erreffregplot[k] = templamregefferr;
	//	erreffbarplot[k] = templambarefferr;
	//}

	//auto cs2 = new TGraphErrors(q2entries, xvals, y1, xerr, y1err);
	//PlotData("Lambda Asymm vs q2", "[#sigma(ep #rightarrow e #Lambda X) - #sigma(ep #rightarrow e #bar{#Lambda} X)] / [#sigma(ep #rightarrow e #Lambda X) + #sigma(ep #rightarrow e #bar{#Lambda} X)]", "q2", "A_{#Lambda}",-0.15,0.15, cs2, q2entries, xvals, y, xerr, y1err, dylow, dyup, dylow, dyup);
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//K0 Cross section results from 2009 H1 Paper
	n = 0;
	X[n] = xvals[n];	y[n] = 1.332;	dxlow[n] = 0;	dxup[n] = 0;	dylow[n] = 0.009;	dyup[n] = 0.009;	n++;
	X[n] = xvals[n];	y[n] = 0.764;	dxlow[n] = 0;	dxup[n] = 0;	dylow[n] = 0.0055;	dyup[n] = 0.0055;	n++;
	X[n] = xvals[n];	y[n] = 0.417;	dxlow[n] = 0;	dxup[n] = 0;	dylow[n] = 0.003;	dyup[n] = 0.003;	n++;
	X[n] = xvals[n];	y[n] = 0.197;	dxlow[n] = 0;	dxup[n] = 0;	dylow[n] = 0.0015;	dyup[n] = 0.0015;	n++;
	X[n] = xvals[n];	y[n] = 0.0340;	dxlow[n] = 0;	dxup[n] = 0;	dylow[n] = 0.0002;	dyup[n] = 0.0002;	n++;
	
	m = 0;
	dylow2[m] = 0.082;	dyup2[m] = 0.082;	m++;
	dylow2[m] = 0.047;	dyup2[m] = 0.045;	m++;
	dylow2[m] = 0.024;	dyup2[m] = 0.023;	m++;
	dylow2[m] = 0.012;	dyup2[m] = 0.012;	m++;
	dylow2[m] = 0.0021;	dyup2[m] = 0.002;	m++;
	//K0s Cross section results from HERAII Preliminary
	double X2[4];
	double y2[4];
	double dxlow2[4];
	double dxup2[4];
	double dylow3[4];
	double dyup3[4];
	n = 0;
	X2[n] = 8.5;	y2[n] = 0.94825;	dxlow2[n] = 1.5;	dxup2[n] = 1.5;	dylow3[n] = 0;	dyup3[n] = 0;	n++;
	X2[n] = 12.5;	y2[n] = 0.514681;	dxlow2[n] = 2.5;	dxup2[n] = 2.5;	dylow3[n] = 0;	dyup3[n] = 0;	n++;
	X2[n] = 20;	y2[n] = 0.240309;	dxlow2[n] = 5;	dxup2[n] = 5;	dylow3[n] = 0;	dyup3[n] = 0;	n++;
	X2[n] = 62.5;	y2[n] = 0.0408826;	dxlow2[n] = 37.5;	dxup2[n] = 37.5;	dylow3[n] = 0;	dyup3[n] = 0;	n++;
	////
	
	for (int k = 0; k < q2entries; k++) {
		double tempk0eff = (KaonMCCount[k][0]) / (KaonGenCount[k]);
		double tempk0efferr = GetRatioError(tempk0eff, KaonMCCount[k][0], KaonMCCount[k][1], KaonGenCount[k], 0);

		//cout << "K0s Rad Correction: " << (KaonGenCountinclRad[0] / KaonGenCountinclNonRad[0]) * (MCDjangoh14nonradgen / MCDjangoh14radgen) << endl;
		double tempk0radcorr = (KaonGenCountRad[k] / KaonGenCountNonRad[k]) * (MCDjangoh14nonradgen / MCDjangoh14radgen);
		k0radcorrplot[k] = (tempk0radcorr-1)*100;
		cout << "K0 Rad Cor Q2 Bin " << k + 1 << ": " << tempk0radcorr << endl;

		double tempk0crosssection = (KaonDataCount[k][0]) / (tempk0eff * BRkaon * DataIntLumi06 * tempk0radcorr * (q2bins[k + 1] - q2bins[k]));

		double tempk0crosssectionerr = GetRatioError(tempk0crosssection, (KaonDataCount[k][0]), (KaonDataCount[k][1]), (tempk0eff * BRkaon * DataIntLumi06 * (q2bins[k + 1] - q2bins[k])), tempk0efferr);
		
		cout << "stat err: " << tempk0crosssectionerr << endl;

		y1[k] = tempk0crosssection;
		y1err[k] = tempk0crosssectionerr;
		effregplot[k] = tempk0eff;
		erreffregplot[k] = tempk0efferr;

		ourresultsoverpaperk0[k] = tempk0crosssection / y[k];
		if (k>0) {
			ourresultsoverprelimk0[k-1] = tempk0crosssection / y2[k-1];
		}
	}

	//auto gk0 = new TGraph(5, xvals, ourresultsoverpaperk0);
	//auto gk0prelim = new TGraph(4, X2, ourresultsoverprelimk0);
	//TMultiGraph* mgratios = new TMultiGraph();
	//TCanvas* cgk = new TCanvas("Results Ratio", "Results Ratio", 10, 10, 800, 400);
	//cgk->SetName("Results Ratio");
	//gk0->SetTitle("Our Results Over Paper's;Q^{2};Cross Section Ratio");
	////mgratios->Add(gk0, "AP");
	//gk0->GetXaxis()->SetRange(0, 105);
	//gk0->Draw();
	//gk0->SetLineColor(kBlack);
	//gk0prelim->SetTitle("Our Results Over Prelim;Q^{2};Cross Section Ratio");
	//gk0prelim->Draw("same");
	////mgratios->Add(gk0prelim, "AP");
	//gk0prelim->SetLineColor(kBlue);
	//g->SetLineColor(kRed);
	//g->Draw("same");
	////mgratios->Add(g, "AP");
	//mgratios->Draw();
	//g->SetLineStyle("AC*")
	TLegend* legendgk = new TLegend();
	//legendgk->AddEntry(g, "#Lambda HERAI H1 Publication (1999-2000 DST5):Our HERAII Results (2006 DST7)", "l");
	//legendgk->AddEntry(gk0, "K^{0}_{s} HERAI H1 Publication (1999-2000 DST5):Our HERAII Results (2006 DST7)", "l");
	//legendgk->AddEntry(gk0prelim, "K^{0}_{s} HERAII H1 Prelim (2006 DST7):Our HERAII Results (2006 DST7)", "l");
	//legend3->AddEntry(effbar, "#bar{#Lambda}", "l");
	//legendgk->Draw("same");
	outputfile.cd();
	//cgk->SetGridy();
	//cgk->Write();
	//gk0->Write();
	//gk0prelim->Write();
	f->cd();

	//Plotting K0 Efficiency
	TCanvas* ckeff = new TCanvas("K0 Cross Section vs q2", "K0 Cross Section vs q2", 10, 10, 800, 400);
	ckeff->SetName("K0 Reconstruction Eff vs q2");

	auto effreg2 = new TGraphErrors(q2entries, xvals, effregplot, xerr, erreffregplot);
	//auto effbar = new TGraphErrors(arraysize, X, effbarplot, xerr, erreffbarplot);
	effreg2->SetMarkerColor(kRed);
	effreg2->SetLineColor(kRed);
	effreg2->SetMarkerSize(0.01);
	//effbar->SetMarkerColor(kBlue);
	//effbar->SetLineColor(kBlue);
	//effbar->SetMarkerSize(0.5);

	effreg2->SetTitle("K^{0}_{S} Reconstruction Eff vs q2");
	effreg2->GetXaxis()->SetTitle("q2");
	effreg2->GetYaxis()->SetTitle("Efficiency");
	effreg2->Draw();
	//effbar->Draw("SAME");
	TLegend* legend4 = new TLegend();
	legend4->AddEntry(effreg2, "K^{0}_{S}", "l");
	//legend3->AddEntry(effbar, "#bar{#Lambda}", "l");
	legend4->Draw("SAME");
	outputfile.cd();
	ckeff->Write();
	//End Plotting K0 Efficiency

	//Plotting radiative Corrections
	TCanvas* cradcorr = new TCanvas("Radiative Corrections", "Radiative Corrections", 10, 10, 800, 400);
	cradcorr->SetName("Radiaitve Corrections vs q2");

	auto radcorrlambda = new TGraphErrors(q2entries, xvals, lambdaradcorrplot, xerr, lambdaradcorrploterrors);
	auto radcorrk0 = new TGraphErrors(q2entries, xvals, k0radcorrplot, xerr, lambdaradcorrploterrors);
	//auto effbar = new TGraphErrors(arraysize, X, effbarplot, xerr, erreffbarplot);
	radcorrlambda->SetMarkerColor(kRed);
	radcorrlambda->SetLineColor(kRed);
	radcorrlambda->SetMarkerSize(0.01);
	radcorrk0->SetMarkerColor(kBlue);
	radcorrk0->SetLineColor(kBlue);
	radcorrk0->SetMarkerSize(0.01);
	//effbar->SetMarkerColor(kBlue);
	//effbar->SetLineColor(kBlue);
	//effbar->SetMarkerSize(0.5);

	radcorrlambda->SetTitle("Radiative Corrections vs Q^{2}");
	radcorrlambda->GetXaxis()->SetTitle("Q^{2}");
	radcorrlambda->GetYaxis()->SetTitle("#delta_{QED} [%]");
	radcorrlambda->Draw();
	radcorrk0->Draw("SAME");
	//effbar->Draw("SAME");
	TLegend* legendradcorr = new TLegend();
	legendradcorr->AddEntry(radcorrlambda, "#Lambda", "l");
	legendradcorr->AddEntry(radcorrk0, "K^{0}_{s}", "l");
	//legend3->AddEntry(effbar, "#bar{#Lambda}", "l");
	legendradcorr->Draw("SAME");
	outputfile.cd();
	cradcorr->Write();
	//End plotting radiative corrections

	f->cd();

	auto cs3 = new TGraphErrors(q2entries, xvals, y1, xerr, y1err);
	PlotK0CSData("K0 Cross Section vs q2", "d#sigma(ep #rightarrow e K^{0}_{S} X)/dx", "q2", "d#sigma/dx [nb]",0,10.5, cs3, q2entries, xvals, y, xerr, y1err, dylow, dyup, dylow2, dyup2);
	for (int k = 0; k < q2entries; k++) {
		cout << "K0 Q2 Cross Section Entry " << k + 1 << " : " << y1[k] << " X: " << xvals[k] << "p/m: " << y1err[k] << endl;
	}

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//K0/Lambda Ratio Cross section results from 2009 H1 Paper
	n = 0;
	X[n] = xvals[n];	y[n] = 0.347;	dxlow[n] = 0;	dxup[n] = 0;	dylow[n] = 0.007;	dyup[n] = 0.007;	n++;
	X[n] = xvals[n];	y[n] = 0.369;	dxlow[n] = 0;	dxup[n] = 0;	dylow[n] = 0.008;	dyup[n] = 0.008;	n++;
	X[n] = xvals[n];	y[n] = 0.367;	dxlow[n] = 0;	dxup[n] = 0;	dylow[n] = 0.0075;	dyup[n] = 0.0075;	n++;
	X[n] = xvals[n];	y[n] = 0.360;	dxlow[n] = 0;	dxup[n] = 0;	dylow[n] = 0.008;	dyup[n] = 0.008;	n++;
	X[n] = xvals[n];	y[n] = 0.353;	dxlow[n] = 0;	dxup[n] = 0;	dylow[n] = 0.006;	dyup[n] = 0.006;	n++;
	
	m = 0;
	dylow2[m] = 0.014;	dyup2[m] = 0.014;	m++;
	dylow2[m] = 0.019;	dyup2[m] = 0.018;	m++;
	dylow2[m] = 0.014;	dyup2[m] = 0.013;	m++;
	dylow2[m] = 0.017;	dyup2[m] = 0.017;	m++;
	dylow2[m] = 0.018;	dyup2[m] = 0.017;	m++;



	for (int k = 0; k < q2entries; k++) {
		double tempk0eff = (KaonMCCount[k][0]) / (KaonGenCount[k]);

		double tempk0efferr = GetRatioError(tempk0eff, KaonMCCount[k][0], KaonMCCount[k][1], KaonGenCount[k], 0);
		double tempk0crosssection = (KaonDataCount[k][0]) / (tempk0eff * BRkaon * DataIntLumi06 * (q2bins[k + 1] - q2bins[k]));

		double tempk0crosssectionerr = GetRatioError(tempk0crosssection, (KaonDataCount[k][0]), (KaonDataCount[k][1]), (tempk0eff * BRkaon * DataIntLumi06 * (q2bins[k + 1] - q2bins[k])), tempk0efferr);

		//y1[k] = tempk0crosssection;
		//y1err[k] = tempk0crosssectionerr;
		//effregplot[k] = tempk0eff;
		//erreffregplot[k] = tempk0efferr;
		//
		double templameff = (LambdaTotMCCount[k][0]) / (LambdaTotGenCount[k]);

		double templamefferr = GetRatioError(templameff, LambdaTotMCCount[k][0], LambdaTotMCCount[k][1], LambdaTotGenCount[k], 0);
		double templamcrosssection = (LambdaTotDataCount[k][0]) / (templameff * BRlambda * DataIntLumi06 * (q2bins[k + 1] - q2bins[k]));

		double templamcrosssectionerr = GetRatioError(templamcrosssection, (LambdaTotDataCount[k][0]), (LambdaTotDataCount[k][1]), (templameff * BRlambda * DataIntLumi06 * (q2bins[k + 1] - q2bins[k])), templamefferr);
		//y1[k] = templamcrosssection;
		//y1err[k] = templamcrosssectionerr;
		//effregplot[k] = templameff;
		//erreffregplot[k] = templamefferr;
		//

		double tempratio = (templamcrosssection) / (tempk0crosssection);
		double tempratioerr = GetRatioError(tempratio, templamcrosssection, templamcrosssectionerr, tempk0crosssection, tempk0crosssectionerr);
		y1[k] = tempratio;
		y1err[k] = tempratioerr;

	}

	auto cs4 = new TGraphErrors(q2entries, xvals, y1, xerr, y1err);
	PlotData("Lambda Over K0 Cross Section vs q2", "[d#sigma(ep #rightarrow e #Lambda X)]/[d#sigma(ep #rightarrow e K^{0}_{S} X)]", "q2", "#Lambda/K^{0}_{S}",0,0.51, cs4, q2entries, xvals, y, xerr, y1err, dylow, dyup, dylow2, dyup2);


	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//Now doing peak extractions and analysis for inclusive measurements
	vector<TH1F*> LambdaTotDataxincl;
	//vector<TH1F*> LambdaRegDataxincl;
	//vector<TH1F*> LambdaBarDataxincl;
	vector<TH1F*> LambdaTotMCxincl;
	//vector<TH1F*> LambdaRegMCxincl;
	//vector<TH1F*> LambdaBarMCxincl;
	vector<TH1F*> KaonDataxincl;
	vector<TH1F*> KaonMCxincl;
	//I don't have Histograms for particle level generator data because counts for these
	//will be done by just going through the generate TTree and adding events up that are within our conditions
	for (int a = 0; a < 1; a++) {
		string namestr1 = "Lambda Tot Data";
		//string namestr2 = "Lambda Reg Data q2 Bin";
		//string namestr3 = "Lambda Bar Data q2 Bin";
		string namestr4 = "Lambda Tot MC q2 Bin";
		//string namestr5 = "Lambda Reg MC q2 Bin";
		//string namestr6 = "Lambda Bar MC q2 Bin";
		string namestr7 = "Kaon Data q2 Bin";
		string namestr8 = "Kaon MC q2 Bin";


		//LambdaTotDataQ2[a] = new TH1F(namestr1.c_str(), namestr1.c_str(), lambdabins, lambdalowerbinrange, lambdaupperbinrange);
		LambdaTotDataxincl.push_back(new TH1F(namestr1.c_str(), namestr1.c_str(), lambdabins, lambdalowerbinrange, lambdaupperbinrange));
		//LambdaRegDataxincl.push_back(new TH1F(namestr2.c_str(), namestr2.c_str(), lambdabins, lambdalowerbinrange, lambdaupperbinrange));
		//LambdaBarDataxincl.push_back(new TH1F(namestr3.c_str(), namestr3.c_str(), lambdabins, lambdalowerbinrange, lambdaupperbinrange));
		LambdaTotMCxincl.push_back(new TH1F(namestr4.c_str(), namestr4.c_str(), lambdabins, lambdalowerbinrange, lambdaupperbinrange));
		//LambdaRegMCxincl.push_back(new TH1F(namestr5.c_str(), namestr5.c_str(), lambdabins, lambdalowerbinrange, lambdaupperbinrange));
		//LambdaBarMCxincl.push_back(new TH1F(namestr6.c_str(), namestr6.c_str(), lambdabins, lambdalowerbinrange, lambdaupperbinrange));
		KaonDataxincl.push_back(new TH1F(namestr7.c_str(), namestr7.c_str(), kaonbins, kaonlowerbinrange, kaonupperbinrange));
		KaonMCxincl.push_back(new TH1F(namestr8.c_str(), namestr8.c_str(), kaonbins, kaonlowerbinrange, kaonupperbinrange));
	}



	//Defining vectors now that will contain the count and count error data for each paticle at MC, Data, and gen level (gen level has no count error, just count)
	//These vectors will be for all counting binning for all variables, can just clear them inbetween uses
	vector<vector<double>> LambdaTotDataCountincl;
	//vector<vector<double>> LambdaRegDataCountincl;
	//vector<vector<double>> LambdaBarDataCountincl;
	vector<vector<double>> LambdaTotMCCountincl;
	//vector<vector<double>> LambdaRegMCCountincl;
	//vector<vector<double>> LambdaBarMCCountincl;
	vector<vector<double>> KaonDataCountincl;
	vector<vector<double>> KaonMCCountincl;

	//Gen particle count
	vector<double> LambdaTotGenCountincl;
	//vector<double> LambdaRegGenCountincl;
	//vector<double> LambdaBarGenCountincl;
	vector<double> KaonGenCountincl;
	vector<double> LambdaTotGenCountinclNonRad;
	vector<double> LambdaTotGenCountinclRad;
	vector<double> KaonGenCountinclNonRad;
	vector<double> KaonGenCountinclRad;

	//Analyzing Tot Lambda Counts for q2 bins for MC and Data
	//vector<vector<double>> AnalyzeTree(TFile * Data, string TreeName, int binentries, int particlecode, vector<TH1F*> HistArray, vector<double> binsvector, string extractionReadback, TCanvas * c3, TCanvas * c1) { //when calling function, particle code deictates 0=lambda or 1=kaon MLP parameters

	
	LambdaTotDataCountincl = AnalyzeTree(varidx, new TFile(filelambdaTree.c_str(), "read"), "Lambda Tot", 1, 0, LambdaTotDataxincl, { q2bins[0],q2bins[q2entries] }, "Lambda Tot Incl Data q2 Extraction ", c3, c1, { {0.00205,3.0,1.1156} }, { {1.083,1.1,1.137,1.16} });
	LambdaTotMCCountincl = AnalyzeTree(varidx, new TFile(filelambdaTreeMCRAPGAP.c_str(), "read"), "Lambda Tot", 1, 0, LambdaTotMCxincl, { q2bins[0],q2bins[q2entries] }, "Lambda Tot Incl MC q2 Extraction ", c3, c1, { { {0.00168, 2.5,1.1156} } }, { {1.083,1.1,1.137,1.16} });

	//LambdaRegDataCountincl = AnalyzeTree(varidx, new TFile(filelambdaTree.c_str(), "read"), "Lambda Reg", 1, 0, LambdaRegDataxincl, { q2bins[0],q2bins[q2entries] }, "Lambda Reg Incl Data q2 Extraction ", c3, c1);
	//LambdaRegMCCountincl = AnalyzeTree(varidx, new TFile(filelambdaTreeMC.c_str(), "read"), "Lambda Reg", 1, 0, LambdaRegMCxincl, { q2bins[0],q2bins[q2entries] }, "Lambda Reg Incl MC q2 Extraction ", c3, c1);

	//LambdaBarDataCountincl = AnalyzeTree(varidx, new TFile(filelambdaTree.c_str(), "read"), "Lambda Bar", 1, 0, LambdaBarDataxincl, { q2bins[0],q2bins[q2entries] }, "Lambda Bar Incl Data q2 Extraction ", c3, c1);
	//LambdaBarMCCountincl = AnalyzeTree(varidx, new TFile(filelambdaTreeMC.c_str(), "read"), "Lambda Bar", 1, 0, LambdaBarMCxincl, { q2bins[0],q2bins[q2entries] }, "Lambda Bar Incl MC q2 Extraction ", c3, c1);

	KaonDataCountincl = AnalyzeTree(varidx, new TFile(filekaonTree.c_str(), "read"), "K0", 1, 1, KaonDataxincl, { q2bins[0],q2bins[q2entries] }, "Kaon Incl Data q2 Extraction ", c3, c1, { {0.006,3.0,0.49761} }, { {0.35,0.4,0.6,0.69} });
	KaonMCCountincl = AnalyzeTree(varidx, new TFile(filekaonTreeMC.c_str(), "read"), "K0", 1, 1, KaonMCxincl, { q2bins[0],q2bins[q2entries] }, "Kaon Incl MC q2 Extraction ", c3, c1, { {0.006,3.0,0.49761} }, { {0.35,0.4,0.6,0.69} });

	
	//LambdaTotDataCountincl = AnalyzeTree(new TFile(filelambdaTree.c_str(), "read"), "Lambda Tot", 1, 0, LambdaTotDataxincl, { 10,q2bins[q2entries] }, "Lambda Tot Incl Data q2 Extraction ", c3, c1);
	//LambdaTotMCCountincl = AnalyzeTree(new TFile(filelambdaTreeMC.c_str(), "read"), "Lambda Tot", 1, 0, LambdaTotMCxincl, { 10,q2bins[q2entries] }, "Lambda Tot Incl MC q2 Extraction ", c3, c1);

	//LambdaRegDataCountincl = AnalyzeTree(new TFile(filelambdaTree.c_str(), "read"), "Lambda Reg", 1, 0, LambdaRegDataxincl, { 10,q2bins[q2entries] }, "Lambda Reg Incl Data q2 Extraction ", c3, c1);
	//LambdaRegMCCountincl = AnalyzeTree(new TFile(filelambdaTreeMC.c_str(), "read"), "Lambda Reg", 1, 0, LambdaRegMCxincl, { 10,q2bins[q2entries] }, "Lambda Reg Incl MC q2 Extraction ", c3, c1);

	//LambdaBarDataCountincl = AnalyzeTree(new TFile(filelambdaTree.c_str(), "read"), "Lambda Bar", 1, 0, LambdaBarDataxincl, { 10,q2bins[q2entries] }, "Lambda Bar Incl Data q2 Extraction ", c3, c1);
	//LambdaBarMCCountincl = AnalyzeTree(new TFile(filelambdaTreeMC.c_str(), "read"), "Lambda Bar", 1, 0, LambdaBarMCxincl, { 10,q2bins[q2entries] }, "Lambda Bar Incl MC q2 Extraction ", c3, c1);

	//KaonDataCountincl = AnalyzeTree(new TFile(filekaonTree.c_str(), "read"), "K0", 1, 1, KaonDataxincl, { 10,q2bins[q2entries] }, "Kaon Incl Data q2 Extraction ", c3, c1);
	//KaonMCCountincl = AnalyzeTree(new TFile(filekaonTreeMC.c_str(), "read"), "K0", 1, 1, KaonMCxincl, { 10,q2bins[q2entries] }, "Kaon Incl MC q2 Extraction ", c3, c1);

	//Analyzing Gen Counts
	//vector<double> AnalyzeGenTree(TFile* Data, string TreeName, int binentries, vector<double> binsvector) { //when calling function, particle code deictates 0=lambda or 1=kaon MLP parameters

	LambdaTotGenCountincl = AnalyzeGenTree(varidx, new TFile(filegenMCTreeRAPGAP.c_str(), "read"), "Lambda Tot Channel", 1, { q2bins[0],q2bins[q2entries] });
	//LambdaRegGenCountincl = AnalyzeGenTree(varidx, new TFile(filegenMCTree.c_str(), "read"), "Lambda Reg Channel", 1, { q2bins[0],q2bins[q2entries] });
	//LambdaBarGenCountincl = AnalyzeGenTree(varidx, new TFile(filegenMCTree.c_str(), "read"), "Lambda Bar Channel", 1, { q2bins[0],q2bins[q2entries] });
	KaonGenCountincl = AnalyzeGenTree(varidx, new TFile(filegenMCTreeRAPGAP.c_str(), "read"), "K0s Channel", 1, { q2bins[0],q2bins[q2entries] });

	//Non Radiative Gen Counting for Radiative Corrections
	LambdaTotGenCountinclNonRad = AnalyzeGenTreeRadCorr(varidx, new TFile(fileNonRadgenMCTree.c_str(), "read"), "Lambda Tot", 1, { q2bins[0],q2bins[q2entries] });
	LambdaTotGenCountinclRad = AnalyzeGenTreeRadCorr(varidx, new TFile(fileRadgenMCTree.c_str(), "read"), "Lambda Tot", 1, { q2bins[0],q2bins[q2entries] });
	KaonGenCountinclNonRad = AnalyzeGenTreeRadCorr(varidx, new TFile(fileNonRadgenMCTree.c_str(), "read"), "K0s", 1, { q2bins[0],q2bins[q2entries] });
	KaonGenCountinclRad = AnalyzeGenTreeRadCorr(varidx, new TFile(fileRadgenMCTree.c_str(), "read"), "K0s", 1, { q2bins[0],q2bins[q2entries] });

	//double MCDjangoh14radgen = 2331962.00;//radiative gen level DJANGOH nb-1
	//double MCDjangoh14nonradgen = 3560342.00;//non-radiatice gen level DJANGOH nb-1
	cout << "Lambda Tot Gen Count Rad Channel: " << LambdaTotGenCountincl[0] << endl;
	cout << "Lambda Tot Gen Count Rad Tot: " << LambdaTotGenCountinclRad[0] << endl;
	cout << "Lambda Tot Gen Count Non Rad Tot: " << LambdaTotGenCountinclNonRad[0] << endl;
	double lambdatotincradcorr = (LambdaTotGenCountinclRad[0] / LambdaTotGenCountinclNonRad[0]) * (MCDjangoh14nonradgen / MCDjangoh14radgen);
	cout << "Lambda Rad Correction: " << (LambdaTotGenCountinclRad[0] / LambdaTotGenCountinclNonRad[0]) * (MCDjangoh14nonradgen / MCDjangoh14radgen) << endl;
	//cout << "Lambda Rad Correction: " << (LambdaTotGenCountinclRad[0] / LambdaTotGenCountinclNonRad[0]) * (MCDjangoh14nonradgen / MCIntLumi06) << endl;
	
	double k0totincradcorr = (KaonGenCountinclRad[0] / KaonGenCountinclNonRad[0]) * (MCDjangoh14nonradgen / MCDjangoh14radgen);
	cout << "K0s Rad Correction: " << (KaonGenCountinclRad[0] / KaonGenCountinclNonRad[0]) * (MCDjangoh14nonradgen / MCDjangoh14radgen) << endl;
	//cout << "K0s Rad Correction: " << (KaonGenCountinclRad[0] / KaonGenCountinclNonRad[0]) * (MCDjangoh14nonradgen / MCIntLumi06) << endl;

	//LambdaTotGenCountincl = AnalyzeGenTree(new TFile(filegenMCTree.c_str(), "read"), "Lambda Tot", 1, { 10,q2bins[q2entries] });
	//LambdaRegGenCountincl = AnalyzeGenTree(new TFile(filegenMCTree.c_str(), "read"), "Lambda Reg", 1, { 10,q2bins[q2entries] });
	//LambdaBarGenCountincl = AnalyzeGenTree(new TFile(filegenMCTree.c_str(), "read"), "Lambda Bar", 1, { 10,q2bins[q2entries] });
	//KaonGenCountincl = AnalyzeGenTree(new TFile(filegenMCTree.c_str(), "read"), "K0s", 1, { 10,q2bins[q2entries] });

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//lambda reg + lambda bar cross section
	
	double incllameff = (LambdaTotMCCountincl[0][0]) / (LambdaTotGenCountincl[0]);

	double incllamefferr = GetRatioError(incllameff, LambdaTotMCCountincl[0][0], LambdaTotMCCountincl[0][1], LambdaTotGenCountincl[0], 0);
	double incllamcrosssection = (LambdaTotDataCountincl[0][0]) / (incllameff * BRlambda * DataIntLumi06 * lambdatotincradcorr);
	double incllamcrosssectionerr = GetRatioError(incllamcrosssection, (LambdaTotDataCountincl[0][0]), (LambdaTotDataCountincl[0][1]), (incllameff * BRlambda * DataIntLumi06), incllamefferr);

	//MC Lumi Calc
	double MCincllamcrosssection = (LambdaTotGenCountincl[0]) / (MCIntLumi06);

	//lambda reg cross section
	//double incllamregeff = (LambdaRegMCCountincl[0][0]) / (LambdaRegGenCountincl[0]);

	//double incllamregefferr = GetRatioError(incllamregeff, LambdaRegMCCountincl[0][0], LambdaRegMCCountincl[0][1], LambdaRegGenCountincl[0], 0);
	//double incllamregcrosssection = (LambdaRegDataCountincl[0][0]) / (incllamregeff * BRlambda * DataIntLumi06);
	//double incllamregcrosssectionerr = GetRatioError(incllamregcrosssection, (LambdaRegDataCountincl[0][0]), (LambdaRegDataCountincl[0][1]), (incllamregeff * BRlambda * DataIntLumi06), incllamregefferr);

	//lambda bar cross section
	//double incllambareff = (LambdaBarMCCountincl[0][0]) / (LambdaBarGenCountincl[0]);

	//double incllambarefferr = GetRatioError(incllambareff, LambdaBarMCCountincl[0][0], LambdaBarMCCountincl[0][1], LambdaBarGenCountincl[0], 0);
	//double incllambarcrosssection = (LambdaBarDataCountincl[0][0]) / (incllambareff * BRlambda * DataIntLumi06);
	//double incllambarcrosssectionerr = GetRatioError(incllambarcrosssection, (LambdaBarDataCountincl[0][0]), (LambdaBarDataCountincl[0][1]), (incllambareff * BRlambda * DataIntLumi06), incllambarefferr);

	//lambda asymmetry
	//double incllamasymm = (incllamregcrosssection - incllambarcrosssection) / (incllamregcrosssection + incllambarcrosssection);
	//double incllamasymmerr = GetRatioError(((incllamregcrosssection - incllambarcrosssection) / (incllamregcrosssection + incllambarcrosssection)), (incllamregcrosssection - incllambarcrosssection), (incllamregcrosssectionerr + incllambarcrosssectionerr), (incllamregcrosssection + incllambarcrosssection), (incllamregcrosssectionerr + incllambarcrosssectionerr));

	//K0 cross section

	double inclK0eff = (KaonMCCountincl[0][0]) / (KaonGenCountincl[0]);

	double inclK0efferr = GetRatioError(inclK0eff, KaonMCCountincl[0][0], KaonMCCountincl[0][1], KaonGenCountincl[0], 0);
	double inclK0crosssection = (KaonDataCountincl[0][0]) / (inclK0eff * BRkaon * DataIntLumi06* k0totincradcorr);
	double inclK0crosssectionerr = GetRatioError(inclK0crosssection, (KaonDataCountincl[0][0]), (KaonDataCountincl[0][1]), (inclK0eff * BRkaon * DataIntLumi06), inclK0efferr);

	double MCinclK0crosssection = (KaonGenCountincl[0]) / (MCIntLumi06);

	//Lambda:K0 Ratio

	double inclratio = incllamcrosssection/ inclK0crosssection;
	double inclratioerr = GetRatioError(incllamcrosssection / inclK0crosssection, incllamcrosssection, incllamcrosssectionerr, inclK0crosssection, inclK0crosssectionerr);


	/////////////////////////////////////////////////////////////////////////////////
	//Creating Histograms from Trees I want to see


	/////////////////////////////////////////////////////////////////////////////////



	cout << "Lambda Total Gen Count: " << LambdaTotGenCountincl[0] << endl;
	cout << "K0 Total Gen Count: " << KaonGenCountincl[0] << endl;
	cout << "Lambda+Lambda Bar Inclusive Gen Cross Section: " << MCincllamcrosssection << endl;
	cout << "Lambda+Lambda Bar Inclusive Gen Cross Section: " << MCinclK0crosssection << endl;
	cout << "Lambda+Lambda Bar Inclusive Reconstruction Eff: " << incllameff << " pm " << incllamefferr << endl;
	cout << "Lambda+Lambda Bar Inclusive Cross Section: " << incllamcrosssection << " pm " << incllamcrosssectionerr << endl;
	//cout << "Lambda Reg Inclusive Reconstruction Eff: " << incllamregeff << " pm " << incllamregefferr << endl;
	//cout << "Lambda Reg Inclusive Cross Section: " << incllamregcrosssection << " pm " << incllamregcrosssectionerr << endl;
	//cout << "Lambda Bar Inclusive Reconstruction Eff: " << incllambareff << " pm " << incllambarefferr << endl;
	//cout << "Lambda Bar Inclusive Cross Section: " << incllambarcrosssection << " pm " << incllambarcrosssectionerr << endl;
	//cout << "Lambda Asymm Inclusive Measurement: " << incllamasymm << " pm " << incllamasymmerr << endl;
	cout << "Kaon Inclusive Reconstruction Eff: " << inclK0eff << " pm " << inclK0efferr << endl;
	cout << "Kaon Inclusive Cross Section: " << inclK0crosssection << " pm " << inclK0crosssectionerr << endl;
	cout << "Kaon Inclusive Gen Cross Section: " << MCinclK0crosssection << endl;
	cout << "Lambda:Kaon Inclusive Ratio: " << inclratio << " pm " << inclratioerr << endl;
		
	
	outputfile.Write();
	outputfile.Close();

	cout << "Done" << endl;

};
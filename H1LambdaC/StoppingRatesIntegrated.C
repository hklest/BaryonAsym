//Analysis Code Root Macro 
//Importing TTrees

using namespace std;

////////////////////////////////////////////////
// User Inputs in this section
////////////////////////////////////////////////

//Lambda and Kaon Extraction May require slightly different parameters
//Convecntion here: element[0] = Lambda ; element[1] = Kaon

vector<int> trainnum = { 50,50 }; //number of time NN is trained
vector<string> network = { "Mass:100:Count", "Mass:100:Count" }; //defining NN
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

double lowPtbound = 0.5;
double lowerYsbound = 0.1;
double upperYsbound = 0.6;
//Branching Ratios
double BRlambda = 0.639;
double BRkaon = 0.692;


//output files
string outputfilename = "/data/dust/group/h1/klesthen/BaryonAsym/H1LambdaC/AnalysisResultFiles/AnalysisOutput.root";
string dummyfile = "/data/dust/group/h1/klesthen/BaryonAsym/H1LambdaC/dummyfile.root";

// Histograms Binning
Double_t lambdabins = 1200;
Double_t lambdaupperbinrange = 1.50014;
Double_t lambdalowerbinrange = 1.00014;
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

double GetAsymError(double NPlus, double errNPlus, double NMinus, double errNMinus) {
	double dAdNPlus = abs(2*NMinus/Square(NPlus + NMinus));
	double dAdNMinus = abs(2*NPlus/Square(NPlus + NMinus));
	return sqrt(Square(dAdNPlus*errNPlus)+Square(dAdNMinus*errNMinus));
}

//creating file to output histograms to
TFile outputfile(outputfilename.c_str(), "RECREATE");
TFile* f = new TFile(dummyfile.c_str(),"RECREATE");





//Main peak extraction, composite fit with RooFit
vector<double> mlpfunction(TH1F* inHist, string plotname, int particlecode, TCanvas* c3, vector<double> fitparaminit, vector<double> rangedefs) { //when calling function, particle code deictates 0=lambda or 1=kaon MLP parameters

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
	RooRealVar b1("b1", "b1", -1.0,-1000,1000);
	RooRealVar b2("b2", "b2", 1.0, -1000, 1000);
	RooRealVar b3("b3", "b3", 1.0, -1000, 1000);
	RooRealVar b4("b4", "b4", 1.0, -1000, 1000);
	RooRealVar b5("b5", "b5", 1.0, -1000, 1000);
	RooRealVar nsigalt("nsigalt", "alt # signal events", 100, 1, 1000000000);
	RooRealVar nbkgalt("nbkgalt", "alt # background events", 10, 1, 1000000000);

	RooRealVar a1("a1", "a1", 0.5, 0.01, 1.);
	RooRealVar a2("a2", "a2", -10.0, -100., -0.1);
	//RooRealVar mu("mu", "mu", fitparaminit[2], fitparaminit[2] - 0.001, fitparaminit[2] + 0.001);
	RooRealVar mu("mu", "mu", 1.1156, 1.1150, 1.1162);
	RooRealVar sigma("sigma", "sigma", fitparaminit[0], 0.0008, 0.005);
	//RooRealVar sigma("sigma", "sigma", 0.001, 0.001, 0.005 );
	RooRealVar p3("p3", "p3", fitparaminit[1],1.0,100.0);
	//RooRealVar p3("p3", "p3", 5.0, 2.0, 10);
	//RooRealVar mudouble("mudouble", "mudouble", fitparaminit[2], fitparaminit[2] - 0.001, fitparaminit[2] + 0.001);
	RooRealVar mudouble("mudouble", "mudouble", 1.1156, 1.1152, 1.1161);
	RooRealVar sigmadouble("sigmadouble", "sigmadouble", fitparaminit[0], 0.0010, 0.005);
	//RooRealVar sigmadouble("sigmadouble", "sigmadouble", 0.001, 0.001, 0.005);
	RooRealVar p3double("p3double", "p3double", fitparaminit[1], 1.0, 100.0);
	//RooRealVar p3double("p3double", "p3double", 5.0, 2.0, 10);
	RooRealVar nsig("nsig", "# signal events", 100, 1, 1000000000);
	RooRealVar nbkg("nbkg", "# background events", 10, 1, 1000000000);
	RooGenericPdf bg("bg", "TMath::Power(TMath::Max(mLam-1.0782,0.),a1)*TMath::Exp(a2*(mLam-1.0782))", RooArgSet(mLam, a1, a2));
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

		if ((inputs[0] >= x1 && inputs[0] < s1) || (inputs[0] > s2 && inputs[0] <= x4)) {
			//If outside of signal range but within fit range want to still see the ratio hist
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

			sumcheckHist->SetBinContent(i, tempsumyieldvalue);
			sumcheckHist->SetBinError(i, tempsumyieldvalueerror);
		}

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
	cout << "Percent Diff: " << abs(runningsignalyield- runningrawsignalsubtraction)*100/ runningsignalyield << endl;
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
	legend->SetHeader(Form("#splitline{Count: %.2f #pm %.2f (stat.)}{Percent Diff from Alt Fit: %.2f}", signalyieldvalue, signalyieldvalueerror, 100*abs(signalyieldvalue- altsignalyieldvalue)/ signalyieldvalue)); // option "C" allows to center the header
	//legend->AddEntry("Signal Fit Parameters");
	legend->AddEntry((TObject*)0, Form("#mu: %.6f #pm %.6f", mu.getVal(), mu.getError()), "");
	legend->AddEntry((TObject*)0, Form("#sigma: %.6f #pm %.6f", sigma.getVal(), sigma.getError()), "");
	legend->AddEntry((TObject*)0, Form("#nu: %.6f #pm %.6f", p3.getVal(), p3.getError()), "");
	//legend->AddEntry("Primary Background Fit Parameters");
	legend->AddEntry((TObject*)0, Form("a_{0}: %.6f #pm %.6f", a1.getVal(), a1.getError()), "");
	legend->AddEntry((TObject*)0, Form("a_{1}: %.6f #pm %.6f", a2.getVal(), a2.getError()), "");
	//legend->AddEntry("Alternate Background Fit Parameters");
	legend->AddEntry((TObject*)0, Form("b_{0}: %.6f #pm %.6f", b1.getVal(), b1.getError()), "");
	legend->AddEntry((TObject*)0, Form("b_{1}: %.6f #pm %.6f", b2.getVal(), b2.getError()), "");
	legend->AddEntry((TObject*)0, Form("b_{2}: %.6f #pm %.6f", b3.getVal(), b3.getError()), "");
	legend->AddEntry((TObject*)0, Form("b_{3}: %.6f #pm %.6f", b4.getVal(), b4.getError()), "");
	legend->AddEntry((TObject*)0, Form("b_{4}: %.6f #pm %.6f", b5.getVal(), b5.getError()), "");
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
	//fgHist->Write();
	//canvas->Write();
	outputfile.Write();
	if (canvas) canvas->Close();
	f->cd();

	cout << "Stupid Check - Fit Vals: " << (ig3->getVal())*(nsig.getVal() + nbkg.getVal()) << " Data: " << totalcountwithbg << endl;
	cout << "Yield Comp - Signal: " << (ig->getVal()) * (nsig.getVal()) << " Raw - BG Fit: " << (ig2->getVal()) * (nbkg.getVal()) - totalcountwithbg << endl;
	cout << (ig3->getVal()) * (nsig.getVal() + nbkg.getVal()) << " = " << (ig2->getVal()) * (nbkg.getVal()) << " + " << (ig->getVal()) * (nsig.getVal()) << endl;

	//return { signalyieldvalue ,countstaterror, bgyielddvalue, signalyieldvalueerror, totalcountwithbg - bgyielddvalue };
	//return { SIGNALINT ,countstaterror, bgyielddvalue, signalyieldvalueerror, totalcountwithbg - bgyielddvalue };
	//return { signalyieldvalue ,countstaterror, signalyieldvalueerror, bgyielddvalue, signalyieldvalueextended, runningrawsignalsubtraction, runningTaylorfityield, runningMLPfityield};
	return { signalyieldvalue ,countstaterror, signalyieldvalueerror, bgyielddvalue, signalyieldvalueextended, totalcountwithbg - bgyielddvalue, altsignalyieldvalue, 0, s1,s2 };

};





//This function takes in raw tree from TTree input files and creates histograms with appropriate binning for peak extractions
//after it has all histrograms for appropriate bins, it extracts counts with mlpfunciton, output is a vector of mlpfunciton vector outputs
vector<vector<double>> AnalyzeTreeRef(int distvariableindex, TFile* Data, string TreeName, int binentries, int particlecode, vector<TH1F*> HistArray, vector<double> binsvector, string extractionReadback, TCanvas* c3, TCanvas* c1, vector<vector<double>> binparaminits, vector<vector<double>> binrangedefs) { //when calling function, particle code deictates 0=lambda or 1=kaon MLP parameters
	vector<vector<double>> ExtractionDataVector = {};

	//have files for lambdaTree, kaonTree, lambdaTreeMC, kaonTreeMC, genMCTree
	//TFile* Data = new TFile(filelambdaTree.c_str(), "read");
	//grab Tree from file above
	TTree* tree = (TTree*)Data->Get(TreeName.c_str());

	int entries;

	entries = tree->GetEntries();

	//Defining the variables that are stored in the Tree branches
	//double invMasslambda, invMasskaon, pT, Eta, x, Q2s, Ys;
	double invMass, pT, Eta, x, Q2s, Ys, Angle, weight1, weight2, Phi, SVx, SVy, SVz, DecayLength, DeltaDCA, PionDCA, ProtonDCA, PionSDCA, ProtonSDCA;
	double PionNuclCorrWeight;
	double PionL, ProtonL, MassElecElec, PionPt, PionNHitsdEdx, ProtonNHitsdEdx, PionTrackStartRadius, ProtonTrackStartRadius;
	int s61;
	int fRunType;
	TVector3* ProtonTrackV0Momentum = nullptr;
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
	tree->SetBranchAddress("PionNuclCorrWeight", &PionNuclCorrWeight);
	tree->SetBranchAddress("fRunType", &fRunType);
	tree->SetBranchAddress("ProtonTrackV0Momentum", &ProtonTrackV0Momentum);
	tree->SetBranchAddress("DecayLength", &DecayLength);
	tree->SetBranchAddress("DeltaDCA", &DeltaDCA);
	tree->SetBranchAddress("PionDCA", &PionDCA);
	tree->SetBranchAddress("PionSDCA", &PionSDCA);
	tree->SetBranchAddress("ProtonDCA", &ProtonDCA);
	tree->SetBranchAddress("ProtonSDCA", &ProtonSDCA);
	tree->SetBranchAddress("PionL", &PionL);
	tree->SetBranchAddress("ProtonL", &ProtonL);
	tree->SetBranchAddress("MassElecElec", &MassElecElec);
	tree->SetBranchAddress("PionPt", &PionPt);
	tree->SetBranchAddress("PionNHitsdEdx", &PionNHitsdEdx);
	tree->SetBranchAddress("ProtonNHitsdEdx", &ProtonNHitsdEdx);
	tree->SetBranchAddress("PionTrackStartRadius", &PionTrackStartRadius);
	tree->SetBranchAddress("ProtonTrackStartRadius", &ProtonTrackStartRadius);


	//tree->SetBranchAddress("SVz", &SVz);
	PionNuclCorrWeight = 0;
	int m;
	int j;
	//Have to enforce pT and Eta bounds here
	for (m = 0; m < entries; m++) {
		tree->GetEntry(m);
		for (j = 0; j < binentries; j++) {
			if (s61 > 0 && PionL > 0.05 && ProtonL > 0.05) {
				if (pT > lowPtbound && pT < 3.5) {
					if (abs(Eta) < 1.3) {
						if (Ys > lowerYsbound && Ys < upperYsbound) {
							if (Q2s > 5 && Q2s < 100) {
								distvariable = { Q2s,Q2s / (Ys * 101568),pT,Eta, Phi,sqrt((SVx) * (SVx)+(SVy) * (SVy)), SVz };
								//if (PionL > 0.001 && ProtonL > 0.001 && DecayLength > 2 && DeltaDCA > 0.5 && abs(PionSDCA) > 1.0 && abs(ProtonSDCA) > 1.0 && ProtonDCA * PionDCA < 0.1 && MassElecElec > 0.05 && PionPt > 0.12 && PionNHitsdEdx>=10 && ProtonNHitsdEdx>=10 && PionSDCA * ProtonSDCA > 50) {
									//if (abs(ProtonTrackV0Momentum->Eta()) < 1.0 && PionTrackStartRadius<25 && ProtonTrackStartRadius < 25) {
									if (abs(ProtonTrackV0Momentum->Eta()) < 1.0) {
										
										
												
													if (ProtonSDCA * PionSDCA > 50) {
														//cout << "Nuclear Correction Weight: " << PionNuclCorrWeight << endl;
														//cout << "Run Type: " << fRunType << endl;
														if (distvariable[distvariableindex] > binsvector[j] && distvariable[distvariableindex] < binsvector[j + 1]) {
															//HistArray[j]->Fill(invMass, weight1 * weight2);
															//if (PionNuclCorrWeight < 0.015) { //If this is Data
															HistArray[j]->Fill(invMass, weight1 * weight2);
															//}
															//if (PionNuclCorrWeight > 0.015) { //If this is MC
															//	HistArray[j]->Fill(invMass, weight1 * weight2 * PionNuclCorrWeight);
															//}

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
		ExtractionDataVector.push_back(mlpfunction(HistArray[j], extractionReadback.c_str(), particlecode, c3, binparaminits[j], binrangedefs[j]));
	}


	return ExtractionDataVector;

};




//This function takes in raw tree from TTree input files and creates histograms with appropriate binning for peak extractions
//after it has all histrograms for appropriate bins, it extracts counts with mlpfunciton, output is a vector of mlpfunciton vector outputs
vector<vector<double>> AnalyzeTreeInt(int distvariableindex, TFile* Data, string TreeName, int binentries, int particlecode, vector<TH1F*> HistArray, vector<double> binsvector, string extractionReadback, TCanvas* c3, TCanvas* c1, vector<vector<double>> binparaminits, vector<vector<double>> binrangedefs) { //when calling function, particle code deictates 0=lambda or 1=kaon MLP parameters
	vector<vector<double>> ExtractionDataVector = {};

	//have files for lambdaTree, kaonTree, lambdaTreeMC, kaonTreeMC, genMCTree
	//TFile* Data = new TFile(filelambdaTree.c_str(), "read");
	//grab Tree from file above
	TTree* tree = (TTree*)Data->Get(TreeName.c_str());

	int entries;

	entries = tree->GetEntries();

	//Defining the variables that are stored in the Tree branches
	//double invMasslambda, invMasskaon, pT, Eta, x, Q2s, Ys;
	double invMass, pT, Eta, x, Q2s, Ys, Angle, weight1, weight2, Phi, SVx, SVy, SVz, DecayLength, DeltaDCA, PionDCA, PionSDCA, ProtonDCA, ProtonSDCA;
	double PionNuclCorrWeight;
	double ProtonTrackEndRadius;
	double PionL, ProtonL, MassElecElec, PionPt, PionNHitsdEdx, ProtonNHitsdEdx, PionTrackStartRadius, ProtonTrackStartRadius;
	int s61;
	int fRunType;
	TVector3* ProtonTrackV0Momentum = nullptr;
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
	tree->SetBranchAddress("PionNuclCorrWeight", &PionNuclCorrWeight);
	tree->SetBranchAddress("fRunType", &fRunType);
	tree->SetBranchAddress("ProtonTrackV0Momentum", &ProtonTrackV0Momentum);
	tree->SetBranchAddress("ProtonTrackEndRadius", &ProtonTrackEndRadius);
	tree->SetBranchAddress("DecayLength", &DecayLength);
	tree->SetBranchAddress("DeltaDCA", &DeltaDCA);
	tree->SetBranchAddress("PionDCA", &PionDCA);
	tree->SetBranchAddress("PionSDCA", &PionSDCA);
	tree->SetBranchAddress("ProtonDCA", &ProtonDCA);
	tree->SetBranchAddress("ProtonSDCA", &ProtonSDCA);
	tree->SetBranchAddress("PionL", &PionL);
	tree->SetBranchAddress("ProtonL", &ProtonL);
	tree->SetBranchAddress("MassElecElec", &MassElecElec);
	tree->SetBranchAddress("PionPt", &PionPt);
	tree->SetBranchAddress("PionNHitsdEdx", &PionNHitsdEdx);
	tree->SetBranchAddress("ProtonNHitsdEdx", &ProtonNHitsdEdx);
	tree->SetBranchAddress("PionTrackStartRadius", &PionTrackStartRadius);
	tree->SetBranchAddress("ProtonTrackStartRadius", &ProtonTrackStartRadius);

	//tree->SetBranchAddress("SVz", &SVz);
	PionNuclCorrWeight = 0;
	int m;
	int j;
	//Have to enforce pT and Eta bounds here
	for (m = 0; m < entries; m++) {
		tree->GetEntry(m);
		for (j = 0; j < binentries; j++) {
			if (s61 > 0 && PionL>0.05 && ProtonL>0.05) {
				if (pT > lowPtbound && pT < 3.5) {
					if (abs(Eta) < 1.3) {
						if (Ys > lowerYsbound && Ys < upperYsbound) {
							if (Q2s > 5 && Q2s < 100) {
								distvariable = { Q2s,Q2s / (Ys * 101568),pT,Eta, Phi,sqrt((SVx) * (SVx)+(SVy) * (SVy)), SVz };
								//if (PionL > 0.001 && ProtonL > 0.001 && DecayLength > 2 && DeltaDCA > 0.5 && abs(PionSDCA) > 1.0 && abs(ProtonSDCA) > 1.0 && ProtonDCA * PionDCA < 0.1 && MassElecElec > 0.05 && PionPt > 0.12 && PionNHitsdEdx >= 10 && ProtonNHitsdEdx >= 10 && PionSDCA*ProtonSDCA>20) {
									
										
											
												if (ProtonSDCA * PionSDCA > 50) {
													//if (abs(ProtonTrackV0Momentum->Eta()) < 1.0 && PionTrackStartRadius < 25 && ProtonTrackStartRadius < 25) {
													if (abs(ProtonTrackV0Momentum->Eta()) < 1.0) {
														if (ProtonTrackEndRadius > 40 && ProtonTrackEndRadius < 55) {
															//cout << "Nuclear Correction Weight: " << PionNuclCorrWeight << endl;
															//cout << "Run Type: " << fRunType << endl;
															if (distvariable[distvariableindex] > binsvector[j] && distvariable[distvariableindex] < binsvector[j + 1]) {
																//HistArray[j]->Fill(invMass, weight1 * weight2);
																//if (PionNuclCorrWeight < 0.015) { //If this is Data
																HistArray[j]->Fill(invMass, weight1 * weight2);
																//}
																//if (PionNuclCorrWeight > 0.015) { //If this is MC
																//	HistArray[j]->Fill(invMass, weight1 * weight2 * PionNuclCorrWeight);
																//}

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
		ExtractionDataVector.push_back(mlpfunction(HistArray[j], extractionReadback.c_str(), particlecode, c3, binparaminits[j], binrangedefs[j]));
	}


	return ExtractionDataVector;

};




//This function takes in raw tree from TTree input files and creates histograms with appropriate binning for peak extractions
//after it has all histrograms for appropriate bins, it extracts counts with mlpfunciton, output is a vector of mlpfunciton vector outputs
vector<vector<double>> AnalyzeTreeMCOLD(int distvariableindex, TFile* Data, string TreeName, int binentries, int particlecode, vector<TH1F*> HistArray, vector<double> binsvector, string extractionReadback, TCanvas* c3, TCanvas* c1, vector<vector<double>> binparaminits, vector<vector<double>> binrangedefs) { //when calling function, particle code deictates 0=lambda or 1=kaon MLP parameters
	vector<vector<double>> ExtractionDataVector = {};

	//have files for lambdaTree, kaonTree, lambdaTreeMC, kaonTreeMC, genMCTree
	//TFile* Data = new TFile(filelambdaTree.c_str(), "read");
	//grab Tree from file above
	TTree* tree = (TTree*)Data->Get(TreeName.c_str());

	int entries;

	entries = tree->GetEntries();

	//Defining the variables that are stored in the Tree branches
	//double invMasslambda, invMasskaon, pT, Eta, x, Q2s, Ys;
	double invMass, pT, Eta, x, Q2s, Ys, Angle, weight1, weight2, Phi, SVx, SVy, SVz, Phigen, genrecAngle;
	double PionNuclCorrWeight;
	//double pTgen, Etagen, xgen, Q2sgen, Ysgen, SVxgen, SVygen, SVzgen, PVzgen, Phigen, genrecAngle;
	int s61;
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
	//tree->SetBranchAddress("s61", &s61);
	tree->SetBranchAddress("Phi", &Phi);
	tree->SetBranchAddress("SVx", &SVx);
	tree->SetBranchAddress("SVy", &SVy);
	tree->SetBranchAddress("SVz", &SVz);
	tree->SetBranchAddress("genrecAngle", &genrecAngle);
	tree->SetBranchAddress("PionNuclCorrWeight", &PionNuclCorrWeight);

	double roughcount;

	int m;
	int j;
	//Have to enforce pT and Eta bounds here
	for (m = 0; m < entries; m++) {
		tree->GetEntry(m);
		for (j = 0; j < binentries; j++) {
			//if (s61 > 0) {
				if (pT > lowPtbound && pT < 3.5) {
					if (abs(Eta) < 1.3) {
						if (Ys > lowerYsbound && Ys < upperYsbound) {
							if (Q2s > 5 && Q2s < 100) {
								distvariable = { Q2s,Q2s / (Ys * 101568),pT,Eta, Phi,sqrt((SVx) * (SVx)+(SVy) * (SVy)), SVz };
								if (abs(SVz) <= 30.0) {
										if (genrecAngle < 3) {
											if (distvariable[distvariableindex] > binsvector[j] && distvariable[distvariableindex] < binsvector[j + 1]) {
												HistArray[j]->Fill(invMass, weight1 * weight2);
												if (invMass > 1.09 && invMass < 1.15) {
													roughcount = roughcount + weight1 * weight2;
												
											}
										}
									}
								}
							}
						}
					}
				}
			//}
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
		ExtractionDataVector.push_back(mlpfunction(HistArray[j], extractionReadback.c_str(), particlecode, c3, binparaminits[j], binrangedefs[j]));
	}
	ExtractionDataVector[0][0] = roughcount;
	cout << "Rough Count: " << roughcount << endl;
	cout << "Fit Count: " << ExtractionDataVector[0][0] << endl;

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
	double pT, Eta, x, Q2s, Ys, weight1, weight2, Empz, GenElecEn, VtxZgen, genPhi, DecayZVtx, DecayXVtx, DecayYVtx, H1GenBool;

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
			if (pT > lowPtbound && pT < 3.5) {
				if (abs(Eta) < 1.3) {
					if (Ys > lowerYsbound && Ys < upperYsbound) {
						if (Q2s > 5 && Q2s < 100) {
							//if (GenElecEn>11) {
								//if (Empz>35 && Empz<70) {
							//if (H1GenBool == 1) {
								if (abs(VtxZgen) < 30) {
									if (abs(DecayZVtx) < 30.0) {
										distvariable = { Q2s, Q2s / (Ys * 101568), pT, Eta, genPhi, sqrt((DecayXVtx) * (DecayXVtx)+(DecayYVtx) * (DecayYVtx)), DecayZVtx };
										if (distvariable[distvariableindex] > binsvector[j] && distvariable[distvariableindex] < binsvector[j + 1]) {
											GenCounts[j] = GenCounts[j] + 1 * weight1 * weight2;
										}
									}
								}
							//}
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
			if (pT > lowPtbound && pT < 3.5) {
				if (abs(Eta) < 1.3) {
					if (Ys > lowerYsbound && Ys < upperYsbound) {
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

}

//This takes the raw gen MC trees and gets MC total counts for efficiency calculation in the appropraiate bin defined by inputs, function output is the count
//vector<double> AnalyzeBGTree(int distvariableindex, TFile* Data, string TreeName, int binentries, vector<double> binsvector, double s1, double s2) { //when calling function, particle code deictates 0=lambda or 1=kaon MLP parameters
double AnalyzeBGTree(int distvariableindex, TFile * Data, string TreeName, double lowerbinvalue, double upperbinvalue, double s1, double s2) {
	double GenCounts = 0;
	//initialize all GenCounts for each bin to zero in the array
	/*int p;
	for (p = 0; p < binentries; p++) {
		GenCounts.push_back(0);
	}*/
	//cout << "s1: " << s1 << endl;
	//cout << "s2: " << s2 << endl;
	//cout << "lowerbinvalue: " << lowerbinvalue << endl;
	//cout << "upperbinvalue: " << upperbinvalue << endl;
	//have files for lambdaTree, kaonTree, lambdaTreeMC, kaonTreeMC, genMCTree
	//TFile* Data = new TFile(filelambdaTree.c_str(), "read");
	//grab Tree from file above
	TTree* tree = (TTree*)Data->Get(TreeName.c_str());
	int entries;

	entries = tree->GetEntries();

	//Defining the variables that are stored in the Tree branches
	//double invMasslambda, invMasskaon, pT, Eta, x, Q2s, Ys;
	double pT, Eta, x, Q2s, Ys, weight1, weight2, Empz, GenElecEn, H1GenBool, VtxZgen, genPhi, DecayZVtx, DecayXVtx, DecayYVtx, invMasslambda;

	//Defining the branch addresses for the Trees to access these stored variables
	//tree->SetBranchAddress("invMass", &invMass);
	tree->SetBranchAddress("pT", &pT);
	tree->SetBranchAddress("invMasslambda", &invMasslambda);
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
		//for (j = 0; j < binentries; j++) {
			distvariable = { Q2s, Q2s / (Ys * 101568), pT, Eta, genPhi, sqrt((DecayXVtx) * (DecayXVtx)+(DecayYVtx) * (DecayYVtx)), DecayZVtx };
			if(pT > lowPtbound && pT < 3.5){
				if (abs(Eta) < 1.3) {
					if (Ys > lowerYsbound && Ys < upperYsbound) {
						if (Q2s > 5 && Q2s < 100) {
							if (invMasslambda > s1 && invMasslambda < s2) {
								if (distvariable[distvariableindex] > lowerbinvalue && distvariable[distvariableindex] < upperbinvalue) {
									GenCounts = GenCounts + 1 * weight1 * weight2;
								}
							}
						}
					}
				}
			}
		//}
	}

	return GenCounts;

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



void StoppingRatesIntegrated(int varidx, vector<double> q2bins, double DataIntLumi06, double MCDjangoh14radgen, double MCDjangoh14nonradgen, string filekaonTree, string filelambdaTree, string filekaonTreeMC, string filelambdaTreeMCRAPGAP, string filelambdaTreeMCDJANGOH, string filegenMCTreeRAPGAP, string filegenMCTreeDJANGOH, string fileNonRadgenMCTree, string fileRadgenMCTree, string xaxistitle, vector<vector<double>> binparaminitsLambda, vector<vector<double>> binrangedefsLambda, vector<vector<double>> binparaminitsLambdaBar, vector<vector<double>> binrangedefsLambdaBar, vector<vector<double>> binparaminitsMCLambda, vector<vector<double>> binrangedefsMCLambda, vector<vector<double>> binparaminitsMCLambdaBar, vector<vector<double>> binrangedefsMCLambdaBar) {

	cout << "Proton Mass: " << mProton << endl;
	cout << "Pion Mass: " << mPion << endl;

	double sigmaData = 0.0018;
	double sigmaMC = 0.001;
	double paramData = 5;
	double paramMC = 8;

	
	

	//do this after creating histograms
	//Creating the canvases used for storing MLP fit plots, good for troubleshooting
	TCanvas* c3 = new TCanvas("Lambda Extraction", "Lambda Extraction", 10, 10, 800, 400);
	TCanvas* cSquare = new TCanvas("Square", "Square", 10, 10, 800, 800);
	TCanvas* c1 = new TCanvas("Training", "Training", 400, 400);
	


	
	//Now doing peak extractions and analysis for inclusive measurements
	
	vector<TH1F*> LambdaRegDataxinclRef;
	vector<TH1F*> LambdaBarDataxinclRef;
	
	vector<TH1F*> LambdaRegMCRAPGAPxinclRef;
	vector<TH1F*> LambdaBarMCRAPGAPxinclRef;
	
	vector<TH1F*> LambdaRegMCDJANGOHxinclRef;
	vector<TH1F*> LambdaBarMCDJANGOHxinclRef;

	vector<TH1F*> LambdaRegDataxinclInteracting;
	vector<TH1F*> LambdaBarDataxinclInteracting;

	vector<TH1F*> LambdaRegMCRAPGAPxinclInteracting;
	vector<TH1F*> LambdaBarMCRAPGAPxinclInteracting;

	vector<TH1F*> LambdaRegMCDJANGOHxinclInteracting;
	vector<TH1F*> LambdaBarMCDJANGOHxinclInteracting;

	
	
	//I don't have Histograms for particle level generator data because counts for these
	//will be done by just going through the generate TTree and adding events up that are within our conditions
	for (int a = 0; a < 1; a++) {
		
		string namestr2 = "Lambda Reg Ref Data Bin";
		string namestr3 = "Lambda Bar Ref Data Bin";
		
		string namestr5 = "Lambda Reg Ref MC RAPGAP Bin";
		string namestr6 = "Lambda Bar Ref MC RAPGAP Bin";
		
		string namestr8 = "Lambda Reg Ref MC DJANGOH Bin";
		string namestr9 = "Lambda Bar Ref MC DJANGOH Bin";

		


		string namestr14 = "Lambda Reg Interacting Data Bin";
		string namestr15 = "Lambda Bar Interacting Data Bin";

		string namestr16 = "Lambda Reg Interacting MC RAPGAP Bin";
		string namestr17 = "Lambda Bar Interacting MC RAPGAP Bin";

		string namestr18 = "Lambda Reg Interacting MC DJANGOH Bin";
		string namestr19 = "Lambda Bar Interacting MC DJANGOH Bin";

		
		

		
		LambdaRegDataxinclRef.push_back(new TH1F(namestr2.c_str(), namestr2.c_str(), lambdabins, lambdalowerbinrange, lambdaupperbinrange));
		LambdaBarDataxinclRef.push_back(new TH1F(namestr3.c_str(), namestr3.c_str(), lambdabins, lambdalowerbinrange, lambdaupperbinrange));
		
		LambdaRegMCRAPGAPxinclRef.push_back(new TH1F(namestr5.c_str(), namestr5.c_str(), lambdabins, lambdalowerbinrange, lambdaupperbinrange));
		LambdaBarMCRAPGAPxinclRef.push_back(new TH1F(namestr6.c_str(), namestr6.c_str(), lambdabins, lambdalowerbinrange, lambdaupperbinrange));
		
		LambdaRegMCDJANGOHxinclRef.push_back(new TH1F(namestr8.c_str(), namestr8.c_str(), lambdabins, lambdalowerbinrange, lambdaupperbinrange));
		LambdaBarMCDJANGOHxinclRef.push_back(new TH1F(namestr9.c_str(), namestr9.c_str(), lambdabins, lambdalowerbinrange, lambdaupperbinrange));


		LambdaRegDataxinclInteracting.push_back(new TH1F(namestr14.c_str(), namestr14.c_str(), lambdabins, lambdalowerbinrange, lambdaupperbinrange));
		LambdaBarDataxinclInteracting.push_back(new TH1F(namestr15.c_str(), namestr15.c_str(), lambdabins, lambdalowerbinrange, lambdaupperbinrange));

		LambdaRegMCRAPGAPxinclInteracting.push_back(new TH1F(namestr16.c_str(), namestr16.c_str(), lambdabins, lambdalowerbinrange, lambdaupperbinrange));
		LambdaBarMCRAPGAPxinclInteracting.push_back(new TH1F(namestr17.c_str(), namestr17.c_str(), lambdabins, lambdalowerbinrange, lambdaupperbinrange));

		LambdaRegMCDJANGOHxinclInteracting.push_back(new TH1F(namestr18.c_str(), namestr18.c_str(), lambdabins, lambdalowerbinrange, lambdaupperbinrange));
		LambdaBarMCDJANGOHxinclInteracting.push_back(new TH1F(namestr19.c_str(), namestr19.c_str(), lambdabins, lambdalowerbinrange, lambdaupperbinrange));
		

	}



	//Defining vectors now that will contain the count and count error data for each paticle at MC, Data, and gen level (gen level has no count error, just count)
	//These vectors will be for all counting binning for all variables, can just clear them inbetween uses
	
	vector<vector<double>> LambdaRegDataCountinclRef;
	vector<vector<double>> LambdaBarDataCountinclRef;
	
	vector<vector<double>> LambdaRegMCRAPGAPCountinclRef;
	vector<vector<double>> LambdaBarMCRAPGAPCountinclRef;
	
	vector<vector<double>> LambdaRegMCDJANGOHCountinclRef;
	vector<vector<double>> LambdaBarMCDJANGOHCountinclRef;


	vector<vector<double>> LambdaRegDataCountinclInteracting;
	vector<vector<double>> LambdaBarDataCountinclInteracting;

	vector<vector<double>> LambdaRegMCRAPGAPCountinclInteracting;
	vector<vector<double>> LambdaBarMCRAPGAPCountinclInteracting;

	vector<vector<double>> LambdaRegMCDJANGOHCountinclInteracting;
	vector<vector<double>> LambdaBarMCDJANGOHCountinclInteracting;


	
	
	
	//Stopping Rates
	/////////////////////////////////////

	LambdaRegDataCountinclRef = AnalyzeTreeRef(varidx, new TFile(filelambdaTree.c_str(), "read"), "Lambda Reg", 1, 0, LambdaRegDataxinclRef, { q2bins[0],q2bins[q2entries] }, "#Lambda Data Extraction Reference Sample", c3, c1, { {0.0017,4.0,1.1156} }, { {1.0782,1.095,1.135,1.155} });
	LambdaRegMCRAPGAPCountinclRef = AnalyzeTreeRef(varidx, new TFile(filelambdaTreeMCRAPGAP.c_str(), "read"), "Lambda Reg", 1, 0, LambdaRegMCRAPGAPxinclRef, { q2bins[0],q2bins[q2entries] }, "#Lambda RAPGAP Extraction Reference Sample", c3, c1, { {0.002,4.0,1.1156} }, { {1.0782,1.095,1.135,1.155} });
	LambdaRegMCDJANGOHCountinclRef = AnalyzeTreeRef(varidx, new TFile(filelambdaTreeMCDJANGOH.c_str(), "read"), "Lambda Reg", 1, 0, LambdaRegMCDJANGOHxinclRef, { q2bins[0],q2bins[q2entries] }, "#Lambda DJANGOH Extraction Reference Sample", c3, c1, { {0.002, 4.0,1.1156} }, { {1.0782,1.095,1.135,1.155} });

	LambdaBarDataCountinclRef = AnalyzeTreeRef(varidx, new TFile(filelambdaTree.c_str(), "read"), "Lambda Bar", 1, 0, LambdaBarDataxinclRef, { q2bins[0],q2bins[q2entries] }, "#bar{#Lambda} Data Extraction Reference Sample", c3, c1, { {0.0017,4.0,1.1156} }, { {1.0782,1.095,1.135,1.155} });
	LambdaBarMCRAPGAPCountinclRef = AnalyzeTreeRef(varidx, new TFile(filelambdaTreeMCRAPGAP.c_str(), "read"), "Lambda Bar", 1, 0, LambdaBarMCRAPGAPxinclRef, { q2bins[0],q2bins[q2entries] }, "#bar{#Lambda} RAPGAP Extraction Reference Sample", c3, c1, { {0.0025,4.0,1.1156} }, { {1.0782,1.095,1.132,1.155} });
	LambdaBarMCDJANGOHCountinclRef = AnalyzeTreeRef(varidx, new TFile(filelambdaTreeMCDJANGOH.c_str(), "read"), "Lambda Bar", 1, 0, LambdaBarMCDJANGOHxinclRef, { q2bins[0],q2bins[q2entries] }, "#bar{#Lambda} DJANGOH Extraction Reference Sample", c3, c1, { {0.002,4.0,1.1156} }, { {1.0782,1.095,1.135,1.155} });



	LambdaRegDataCountinclInteracting = AnalyzeTreeInt(varidx, new TFile(filelambdaTree.c_str(), "read"), "Lambda Reg", 1, 0, LambdaRegDataxinclInteracting, { q2bins[0],q2bins[q2entries] }, "#Lambda Data Extraction Interacting Sample", c3, c1, { {0.002,4.0,1.1156} }, { {1.0782,1.095,1.135,1.155} });
	LambdaRegMCRAPGAPCountinclInteracting = AnalyzeTreeInt(varidx, new TFile(filelambdaTreeMCRAPGAP.c_str(), "read"), "Lambda Reg", 1, 0, LambdaRegMCRAPGAPxinclInteracting, { q2bins[0],q2bins[q2entries] }, "#Lambda RAPGAP Extraction Interacting Sample", c3, c1, { {0.0025, 4.0,1.1158} }, { {1.0782,1.095,1.135,1.155} });
	LambdaRegMCDJANGOHCountinclInteracting = AnalyzeTreeInt(varidx, new TFile(filelambdaTreeMCDJANGOH.c_str(), "read"), "Lambda Reg", 1, 0, LambdaRegMCDJANGOHxinclInteracting, { q2bins[0],q2bins[q2entries] }, "#Lambda DJANGOH Extraction Interacting Sample", c3, c1, { {0.002, 4.0,1.1156} }, { {1.0782,1.095,1.135,1.155} });

	LambdaBarDataCountinclInteracting = AnalyzeTreeInt(varidx, new TFile(filelambdaTree.c_str(), "read"), "Lambda Bar", 1, 0, LambdaBarDataxinclInteracting, { q2bins[0],q2bins[q2entries] }, "#bar{#Lambda} Data Extraction Interacting Sample", c3, c1, { {0.002,4.0,1.1156} }, { {1.0782,1.095,1.135,1.155} });
	LambdaBarMCRAPGAPCountinclInteracting = AnalyzeTreeInt(varidx, new TFile(filelambdaTreeMCRAPGAP.c_str(), "read"), "Lambda Bar", 1, 0, LambdaBarMCRAPGAPxinclInteracting, { q2bins[0],q2bins[q2entries] }, "#bar{#Lambda} RAPGAP Extraction Interacting Sample", c3, c1, { {0.002,4.0,1.1156} }, { {1.0782,1.095,1.132,1.155} });
	LambdaBarMCDJANGOHCountinclInteracting = AnalyzeTreeInt(varidx, new TFile(filelambdaTreeMCDJANGOH.c_str(), "read"), "Lambda Bar", 1, 0, LambdaBarMCDJANGOHxinclInteracting, { q2bins[0],q2bins[q2entries] }, "#bar{#Lambda} DJANGOH Extraction Interacting Sample", c3, c1, { {0.0025,4.0,1.1156} }, { {1.0782,1.095,1.135,1.155} });



	
	////////////////////////////////////////////////////////////////////////////////////////////////
	//Calculating Stopping Rates

	double DataLambdaRegStoppingRate = LambdaRegDataCountinclInteracting[0][0] / LambdaRegDataCountinclRef[0][0];
	double DataLambdaBarStoppingRate = LambdaBarDataCountinclInteracting[0][0] / LambdaBarDataCountinclRef[0][0];

	double RAPGAPLambdaRegStoppingRate = LambdaRegMCRAPGAPCountinclInteracting[0][0] / LambdaRegMCRAPGAPCountinclRef[0][0];
	double RAPGAPLambdaBarStoppingRate = LambdaBarMCRAPGAPCountinclInteracting[0][0] / LambdaBarMCRAPGAPCountinclRef[0][0];

	double DJANGOHLambdaRegStoppingRate = LambdaRegMCDJANGOHCountinclInteracting[0][0] / LambdaRegMCDJANGOHCountinclRef[0][0];
	double DJANGOHLambdaBarStoppingRate = LambdaBarMCDJANGOHCountinclInteracting[0][0] / LambdaBarMCDJANGOHCountinclRef[0][0];


	cout << "Data Lambda Reg Stopping Rate: " << DataLambdaRegStoppingRate << endl;
	cout << "RAPGAP Lambda Reg Stopping Rate: " << RAPGAPLambdaRegStoppingRate << endl;
	cout << "DJANGOH Lambda Reg Stopping Rate: " << DJANGOHLambdaRegStoppingRate << endl;

	cout << "Data Lambda Bar Stopping Rate: " << DataLambdaBarStoppingRate << endl;
	cout << "RAPGAP Lambda Bar Stopping Rate: " << RAPGAPLambdaBarStoppingRate << endl;
	cout << "DJANGOH Lambda Bar Stopping Rate: " << DJANGOHLambdaBarStoppingRate << endl;

	outputfile.Write();
	outputfile.Close();

	cout << "Done" << endl;

};
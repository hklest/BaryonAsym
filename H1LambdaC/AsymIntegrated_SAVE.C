//Analysis Code Root Macro 
//Importing TTrees

using namespace std;

////////////////////////////////////////////////Why960204
// 
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
const int q2entries = 9;

double lowPtbound = 1.0;
//double lowerYsbound = 0.1;
double lowerYsbound = 0.0375;
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
	return { signalyieldvalue ,countstaterror, signalyieldvalueerror, bgyielddvalue, signalyieldvalueextended, totalcountwithbg - bgyielddvalue, altsignalyieldvalue, 0, s1,s2 , sigma.getVal() ,sigma.getError() };

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
	//tree->SetBranchAddress("SVz", &SVz);
	PionNuclCorrWeight = 0;
	int m;
	int j;
	//Have to enforce pT and Eta bounds here
	for (m = 0; m < entries; m++) {
		tree->GetEntry(m);
		for (j = 0; j < binentries; j++) {
			if(s61>0){
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
										//HistArray[j]->Fill(invMass, weight1 * weight2 * PionNuclCorrWeight);
										if (PionNuclCorrWeight<0.015) { //If this is Data
											HistArray[j]->Fill(invMass, weight1 * weight2);
										}
										if (PionNuclCorrWeight > 0.015) { //If this is MC
											HistArray[j]->Fill(invMass, weight1 * weight2 * PionNuclCorrWeight);
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
					//if (abs(Eta) < 1.3) {
					if (Eta < 1.3 && Eta> -1.3) {
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
				//if (abs(Eta) < 1.3) {
				if (Eta < 1.3 && Eta> -1.3) {
					if (Ys > lowerYsbound && Ys < upperYsbound) {
						if (Q2s > 5 && Q2s < 100) {
							//if (GenElecEn>11) {
								//if (Empz>35 && Empz<70) {
									if (abs(VtxZgen)<30) {
										//if (H1GenBool == 1) {
											//if (abs(DecayZVtx) < 30.0) {
											distvariable = { Q2s, Q2s / (Ys * 101568), pT, Eta, genPhi, sqrt((DecayXVtx) * (DecayXVtx)+(DecayYVtx) * (DecayYVtx)), DecayZVtx };
											if (distvariable[distvariableindex] > binsvector[j] && distvariable[distvariableindex] < binsvector[j + 1]) {
												GenCounts[j] = GenCounts[j] + 1 * weight1 * weight2;
											}
										//}
										//}
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
			if (pT > lowPtbound && pT < 3.5) {
				//if (abs(Eta) < 1.3) {
				if (Eta < 1.3 && Eta> -1.3) {
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
				//if (abs(Eta) < 1.3) {
				if (Eta < 1.3 && Eta> -1.3) {
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



void AsymIntegrated(int varidx, vector<double> q2bins, double DataIntLumi06, double MCDjangoh14radgen, double MCDjangoh14nonradgen, string filekaonTree, string filelambdaTree, string filekaonTreeMC, string filelambdaTreeMCRAPGAP, string filelambdaTreeMCDJANGOH, string filegenMCTreeRAPGAP, string filegenMCTreeDJANGOH, string fileNonRadgenMCTree, string fileRadgenMCTree, string xaxistitle, vector<vector<double>> binparaminitsLambda, vector<vector<double>> binrangedefsLambda, vector<vector<double>> binparaminitsLambdaBar, vector<vector<double>> binrangedefsLambdaBar, vector<vector<double>> binparaminitsMCLambda, vector<vector<double>> binrangedefsMCLambda, vector<vector<double>> binparaminitsMCLambdaBar, vector<vector<double>> binrangedefsMCLambdaBar) {

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
	TCanvas* cSquare = new TCanvas("Square", "Square", 10, 10, 800, 800);
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

	vector<double> LambdaRegBGCountDJANGOH;
	vector<double> LambdaRegBGCountRAPGAP;
	vector<double> LambdaBarBGCountDJANGOH;
	vector<double> LambdaBarBGCountRAPGAP;

	vector<double> LambdaRegTotCountforBGDJANGOH;
	vector<double> LambdaRegTotCountforBGRAPGAP;
	vector<double> LambdaBarTotCountforBGDJANGOH;
	vector<double> LambdaBarTotCountforBGRAPGAP;
	//Gen particle counting for Rad Corrections
	//vector<double> LambdaGenCountRad;
	//vector<double> LambdaGenCountNonRad;

	
	/*LambdaRegDataCount = AnalyzeTree(varidx, new TFile(filelambdaTree.c_str(), "read"), "Lambda Reg", q2entries, 0, LambdaRegDataQ2, q2bins, "#Lambda Data Bin ", c3, c1, binparaminitsLambda, binrangedefsLambda);
	LambdaRegMCRAPGAPCount = AnalyzeTree(varidx, new TFile(filelambdaTreeMCRAPGAP.c_str(), "read"), "Lambda Reg", q2entries, 0, LambdaRegMCRAPGAPQ2, q2bins, "Lambda Reg MC RAPGAP q2 Extraction ", c3, c1, binparaminitsMCLambda, binrangedefsMCLambda);
	LambdaRegMCDJANGOHCount = AnalyzeTree(varidx, new TFile(filelambdaTreeMCDJANGOH.c_str(), "read"), "Lambda Reg", q2entries, 0, LambdaRegMCDJANGOHQ2, q2bins, "Lambda Reg MC DJANGOH q2 Extraction ", c3, c1, binparaminitsMCLambda, binrangedefsMCLambda);

	LambdaBarDataCount = AnalyzeTree(varidx, new TFile(filelambdaTree.c_str(), "read"), "Lambda Bar", q2entries, 0, LambdaBarDataQ2, q2bins, "Lambda Bar Data q2 Extraction ", c3, c1, binparaminitsLambdaBar, binrangedefsLambdaBar);
	LambdaBarMCRAPGAPCount = AnalyzeTree(varidx, new TFile(filelambdaTreeMCRAPGAP.c_str(), "read"), "Lambda Bar", q2entries, 0, LambdaBarMCRAPGAPQ2, q2bins, "Lambda Bar MC RAPGAP q2 Extraction ", c3, c1, binparaminitsMCLambdaBar, binrangedefsMCLambdaBar);
	LambdaBarMCDJANGOHCount = AnalyzeTree(varidx, new TFile(filelambdaTreeMCDJANGOH.c_str(), "read"), "Lambda Bar", q2entries, 0, LambdaBarMCDJANGOHQ2, q2bins, "Lambda Bar MC DJANGOH q2 Extraction ", c3, c1, binparaminitsMCLambdaBar, binrangedefsMCLambdaBar);
	*/

	//Analyzing Gen Counts
	//LambdaTotGenCountRAPGAP = AnalyzeGenTree(varidx, new TFile(filegenMCTreeRAPGAP.c_str(), "read"), "Lambda Tot Channel", q2entries, q2bins);
	//LambdaRegGenCountRAPGAP = AnalyzeGenTree(varidx, new TFile(filegenMCTreeRAPGAP.c_str(), "read"), "Lambda Reg Channel", q2entries, q2bins);
	//LambdaBarGenCountRAPGAP = AnalyzeGenTree(varidx, new TFile(filegenMCTreeRAPGAP.c_str(), "read"), "Lambda Bar Channel", q2entries, q2bins);
	//LambdaTotGenCountDJANGOH = AnalyzeGenTree(varidx, new TFile(filegenMCTreeDJANGOH.c_str(), "read"), "Lambda Tot Channel", q2entries, q2bins);
	//LambdaRegGenCountDJANGOH = AnalyzeGenTree(varidx, new TFile(filegenMCTreeDJANGOH.c_str(), "read"), "Lambda Reg Channel", q2entries, q2bins);
	//LambdaBarGenCountDJANGOH = AnalyzeGenTree(varidx, new TFile(filegenMCTreeDJANGOH.c_str(), "read"), "Lambda Bar Channel", q2entries, q2bins);

	//LambdaRegBGGenCountDJANGOH = AnalyzeGenTree(varidx, new TFile(filegenMCTreeDJANGOH.c_str(), "read"), "Lambda Reg BG", q2entries, q2bins);
	//LambdaRegBGGenCountRAPGAP = AnalyzeGenTree(varidx, new TFile(filegenMCTreeRAPGAP.c_str(), "read"), "Lambda Reg BG", q2entries, q2bins);
	//LambdaBarBGGenCountDJANGOH = AnalyzeGenTree(varidx, new TFile(filegenMCTreeDJANGOH.c_str(), "read"), "Lambda Bar BG", q2entries, q2bins);
	//LambdaBarBGGenCountRAPGAP = AnalyzeGenTree(varidx, new TFile(filegenMCTreeRAPGAP.c_str(), "read"), "Lambda Bar BG", q2entries, q2bins);

	//LambdaRegBGCountDJANGOH = AnalyzeBGTree(varidx, new TFile(filelambdaTreeMCDJANGOH.c_str(), "read"), "Lambda Reg BG", q2entries, q2bins);
	//LambdaRegBGCountRAPGAP = AnalyzeBGTree(varidx, new TFile(filelambdaTreeMCRAPGAP.c_str(), "read"), "Lambda Reg BG", q2entries, q2bins);
	//LambdaBarBGCountDJANGOH = AnalyzeBGTree(varidx, new TFile(filelambdaTreeMCDJANGOH.c_str(), "read"), "Lambda Bar BG", q2entries, q2bins);
	//LambdaBarBGCountRAPGAP = AnalyzeBGTree(varidx, new TFile(filelambdaTreeMCRAPGAP.c_str(), "read"), "Lambda Bar BG", q2entries, q2bins);

	//LambdaRegTotCountforBGDJANGOH = ;
	//LambdaRegTotCountforBGRAPGAP;
	//LambdaBarTotCountforBGDJANGOH;
	//LambdaBarTotCountforBGRAPGAP;
	
	//LambdaGenCountRad = AnalyzeGenTreeRadCorr(varidx, new TFile(fileRadgenMCTree.c_str(), "read"), "Lambda Tot", q2entries, q2bins);
	//LambdaGenCountNonRad = AnalyzeGenTreeRadCorr(varidx, new TFile(fileNonRadgenMCTree.c_str(), "read"), "Lambda Tot", q2entries, q2bins);

	//cout << "LambdaGenCountRad: " << LambdaGenCountRad[0] << endl;
	//cout << "LambdaGenCountNonRad: " << LambdaGenCountNonRad[0] << endl;
	//double yAsymm[q2entries] = {};
	//double y1[q2entries] = {};
	//double y2[q2entries] = {};
	//double y3[q2entries] = {};
	////vector<double> q2bins = { 5.0, 7.0, 10.0, 15.0, 25.0, 100.0 };
	////double xerr[q2entries] = { 1,1.5,2.5,5,37.5 };
	////double xvals[q2entries] = { 6,8.5,12.5,20,62.5 };
	//double xerr[q2entries] = {};
	//double xvals[q2entries] = {};
	//double yAsymmerr[q2entries] = {};
	//double ySys1[q2entries] = {};
	//double ySys2[q2entries] = {};
	//double ySys3[q2entries] = {};
	//double ySys4[q2entries] = {};
	//double ySys5[q2entries] = {};
	//double ySys6[q2entries] = {};
	//double yScatter1[q2entries] = {};
	//double yScatter2[q2entries] = {};
	//double yScatter3[q2entries] = {};
	//double xScatter1[q2entries] = {};
	//double xScatter2[q2entries] = {};
	//double xScatter3[q2entries] = {};
	//double yScatterMC1[q2entries] = {};
	//double yScatterMC2[q2entries] = {};
	//double yScatterMC3[q2entries] = {};
	//double xScatterMC1[q2entries] = {};
	//double xScatterMC2[q2entries] = {};
	//double xScatterMC3[q2entries] = {};
	//double yAsymmerrSYS[q2entries] = {};
	//double yAsymmTOTerr[q2entries] = {};
	//double y1err[q2entries] = {};
	//double y1errSYS[q2entries] = {};
	//double y2err[q2entries] = {};
	//double y2errSYS[q2entries] = {};
	//double y3err[q2entries] = {};
	//double binarray[q2entries+1] = {};

	////
	//double LamRegDataStatOverRoo[q2entries] = {};
	//double LamBarDataStatOverRoo[q2entries] = {};
	//double LamRegRAPGAPStatOverRoo[q2entries] = {};
	//double LamBarRAPGAPStatOverRoo[q2entries] = {};
	//double LamRegDJANGOHStatOverRoo[q2entries] = {};
	//double LamBarDJANGOHStatOverRoo[q2entries] = {};

	//for (int d = 0; d < q2entries; d++) {
	//	xerr[d] = abs(q2bins[d+1]-q2bins[d])/2.0;
	//	cout << "Xerr: " << xerr[d] << endl;
	//	xvals[d] = q2bins[d]+xerr[d];
	//	cout << "Xvals: " << xvals[d] << endl;
	//}
	//for (int d = 0; d <= q2entries; d++) {
	//	binarray[d] = q2bins[d];
	//	cout << "Bin Array: " << binarray[d] << endl;
	//}
	////double q2[q2entries] = { 0.55,0.65,0.75,0.85,1.0,1.2,1.45,1.9,2.85 };
	////double dx[q2entries] = { 0.1,0.1,0.1,0.1,0.2,0.2,0.3,0.6,1.3 };

	//double effregplotRAPGAP[q2entries] = {};
	//double effbarplotRAPGAP[q2entries] = {};
	//double erreffregplotRAPGAP[q2entries] = {};
	//double erreffbarplotRAPGAP[q2entries] = {};

	//double effregplotDJANGOH[q2entries] = {};
	//double effbarplotDJANGOH[q2entries] = {};
	//double erreffregplotDJANGOH[q2entries] = {};
	//double erreffbarplotDJANGOH[q2entries] = {};
	//
	//double lambdaradcorrplot[q2entries] = {};
	//double lambdaradcorrploterrors[q2entries] = {};

	//
	//double X[5];
	//double y[5];
	//double dxlow[5];
	//double dxup[5];
	//double dylow[5];
	//double dyup[5];
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	////Lambda Asymm Data from 2009 paper obtained through xyscan, no syst error published for this, so just stat. error
	//
	//int n = 0;

	//X[n] = xvals[n];	y[n] = -0.0160338;	dxlow[n] = 0;	dxup[n] = 0;	dylow[n] = 0.0388186;	dyup[n] = 0.0388186;	n++;
	//X[n] = xvals[n];	y[n] = 0.0177215;	dxlow[n] = 0;	dxup[n] = 0;	dylow[n] = 0.0438819;	dyup[n] = 0.0438819;	n++;
	//X[n] = xvals[n];	y[n] = -0.0683544;	dxlow[n] = 0;	dxup[n] = 0;	dylow[n] = 0.0337553;	dyup[n] = 0.0337553;	n++;
	//X[n] = xvals[n];	y[n] = -0.0160338;	dxlow[n] = 0;	dxup[n] = 0;	dylow[n] = 0.0472574;	dyup[n] = 0.0472574;	n++;
	//X[n] = xvals[n];	y[n] = 0.0953586;	dxlow[n] = 0;	dxup[n] = 0;	dylow[n] = 0.0320675;	dyup[n] = 0.0320675;	n++;
	//
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//// PLOTTING MEASURED LAMBDA ASYMMETRY
	//for (int k = 0; k < q2entries; k++) {

	//	//Caclulating the detector efficiencies for lambda and lambda bar from RAPGAP
	//	double templamregeffRAPGAP = (LambdaRegMCRAPGAPCount[k][0]) / (LambdaRegGenCountRAPGAP[k]);
	//	double templambareffRAPGAP = (LambdaBarMCRAPGAPCount[k][0]) / (LambdaBarGenCountRAPGAP[k]);
	//	//Calculating detector efficiency statistical from binomial process standard deviation
	//	double templamregefferrRAPGAP = sqrt(templamregeffRAPGAP*(1- templamregeffRAPGAP)/ (LambdaRegGenCountRAPGAP[k]));
	//	double templambarefferrRAPGAP = sqrt(templambareffRAPGAP*(1- templambareffRAPGAP)/ (LambdaBarGenCountRAPGAP[k]));
	//	//double templamregefferrRAPGAP = GetRatioError(templamregeffRAPGAP, LambdaRegMCRAPGAPCount[k][0], LambdaRegMCRAPGAPCount[k][1], LambdaRegGenCountRAPGAP[k], 0);
	//	//double templambarefferrRAPGAP = GetRatioError(templambareffRAPGAP, LambdaBarMCRAPGAPCount[k][0], LambdaBarMCRAPGAPCount[k][1], LambdaBarGenCountRAPGAP[k], 0);
	//	
	//	//Caclulating the lambda and lambda bar counts from RAPGAP efficiencies
	//	double templamregRAPGAP = (LambdaRegDataCount[k][0]) / (templamregeffRAPGAP);
	//	double templambarRAPGAP = (LambdaBarDataCount[k][0]) / (templambareffRAPGAP);

	//	//Propagating error through lambda and lambda bar calculation
	//	double templamregerrRAPGAP = GetRatioError((LambdaRegDataCount[k][0]) / (templamregeffRAPGAP), (LambdaRegDataCount[k][0]), (LambdaRegDataCount[k][1]), (templamregeffRAPGAP), templamregefferrRAPGAP);
	//	double templambarerrRAPGAP = GetRatioError((LambdaBarDataCount[k][0]) / (templambareffRAPGAP), (LambdaBarDataCount[k][0]), (LambdaBarDataCount[k][1]), (templambareffRAPGAP), templambarefferrRAPGAP);
	//	
	//	//Calculating the lambda Asymmetry
	//	y1[k] = (templamregRAPGAP - templambarRAPGAP) / ((templamregRAPGAP + templambarRAPGAP));
	//	yAsymm[k] = (templamregRAPGAP - templambarRAPGAP) / ((templamregRAPGAP + templambarRAPGAP));
	//	
	//	//Calculating the error of the asymmetry
	//	y1err[k] = GetAsymError(templamregRAPGAP, templamregerrRAPGAP, templambarRAPGAP, templambarerrRAPGAP);
	//	yAsymmerr[k] = y1err[k];
	//	
	//	//y1err[k] = GetRatioError(((templamregRAPGAP - templambarRAPGAP) / ((templamregRAPGAP + templambarRAPGAP))), (templamregRAPGAP - templambarRAPGAP), (templamregerrRAPGAP + templambarerrRAPGAP), ((templamregRAPGAP + templambarRAPGAP)), ((templamregerrRAPGAP + templambarerrRAPGAP)));
	//	//y1err[k] = GetRatioError(y1[k], (LambdaRegDataCount[k][0] - LambdaBarDataCount[k][0]), (LambdaRegDataCount[k][1] + LambdaBarDataCount[k][1]), ((LambdaRegDataCount[k][0] - LambdaBarDataCount[k][0])), ((LambdaRegDataCount[k][1] + LambdaBarDataCount[k][1])));
	//	//y1err[k] = 0;

	//	//Caclulating the detector efficiencies for lambda and lambda bar from DJANGOH
	//	double templamregeffDJANGOH = (LambdaRegMCDJANGOHCount[k][0]) / (LambdaRegGenCountDJANGOH[k]);
	//	double templambareffDJANGOH = (LambdaBarMCDJANGOHCount[k][0]) / (LambdaBarGenCountDJANGOH[k]);
	//	//Calculating detector efficiency statistical from binomial process standard deviation
	//	double templamregefferrDJANGOH = sqrt(templamregeffDJANGOH * (1 - templamregeffDJANGOH) / (LambdaRegGenCountDJANGOH[k]));
	//	double templambarefferrDJANGOH = sqrt(templambareffDJANGOH * (1 - templambareffDJANGOH) / (LambdaBarGenCountDJANGOH[k]));
	//	//double templamregefferrDJANGOH = GetRatioError(templamregeffDJANGOH, LambdaRegMCDJANGOHCount[k][0], LambdaRegMCDJANGOHCount[k][1], LambdaRegGenCountDJANGOH[k], 0);
	//	//double templambarefferrDJANGOH = GetRatioError(templambareffDJANGOH, LambdaBarMCDJANGOHCount[k][0], LambdaBarMCDJANGOHCount[k][1], LambdaBarGenCountDJANGOH[k], 0);

	//	//Caclulating the lambda and lambda bar counts from DJANGOH efficiencies
	//	double templamregDJANGOH = (LambdaRegDataCount[k][0]) / (templamregeffDJANGOH);
	//	double templambarDJANGOH = (LambdaBarDataCount[k][0]) / (templambareffDJANGOH);

	//	//Propagating error through lambda and lambda bar calculation
	//	double templamregerrDJANGOH = GetRatioError((LambdaRegDataCount[k][0]) / (templamregeffDJANGOH), (LambdaRegDataCount[k][0]), (LambdaRegDataCount[k][1]), (templamregeffDJANGOH), templamregefferrDJANGOH);
	//	double templambarerrDJANGOH = GetRatioError((LambdaBarDataCount[k][0]) / (templambareffDJANGOH), (LambdaBarDataCount[k][0]), (LambdaBarDataCount[k][1]), (templambareffDJANGOH), templambarefferrDJANGOH);

	//	//Calculating the lambda Asymmetry
	//	y2[k] = (templamregDJANGOH - templambarDJANGOH) / ((templamregDJANGOH + templambarDJANGOH));

	//	//Calculating the error of the asymmetry
	//	y2err[k] = GetAsymError(templamregDJANGOH, templamregerrDJANGOH, templambarDJANGOH, templambarerrDJANGOH);
	//	
	//	//Detector Efficiency Plots
	//	effregplotRAPGAP[k] = templamregeffRAPGAP;
	//	effbarplotRAPGAP[k] = templambareffRAPGAP;
	//	erreffregplotRAPGAP[k] = templamregefferrRAPGAP;
	//	erreffbarplotRAPGAP[k] = templambarefferrRAPGAP;

	//	effregplotDJANGOH[k] = templamregeffDJANGOH;
	//	effbarplotDJANGOH[k] = templambareffDJANGOH;
	//	erreffregplotDJANGOH[k] = templamregefferrDJANGOH;
	//	erreffbarplotDJANGOH[k] = templambarefferrDJANGOH;
	//	
	//	//Calculating all of the systematic errors for y1AsymmSYS now
	//	//double MCerr = GetAsymError(templamregRAPGAP, abs(templamregRAPGAP-templamregDJANGOH), templambarRAPGAP, abs(templambarRAPGAP- templambarDJANGOH));
	//	//double MCerr = GetAsymError(templamregRAPGAP, GetRatioError((templamregRAPGAP), LambdaRegDataCount[0][0], 0.0, templamregeffRAPGAP, 0.5 * abs(templamregeffDJANGOH - templamregeffRAPGAP) / (templamregeffRAPGAP)), templambarRAPGAP, GetRatioError((templambarRAPGAP), LambdaBarDataCount[0][0], 0.0, templambareffRAPGAP, 0.5 * abs(templambareffDJANGOH - templambareffRAPGAP) / (templambareffRAPGAP)));
	//	double MCerr = sqrt(Square((2 * LambdaBarMCRAPGAPCount[0][0] * templambareffRAPGAP * LambdaRegMCRAPGAPCount[0][0] / Square(LambdaBarMCRAPGAPCount[0][0] * templamregeffRAPGAP + templambareffRAPGAP * LambdaRegMCRAPGAPCount[0][0])) * (0.5 * abs(templamregeffDJANGOH - templamregeffRAPGAP))) + Square((2 * LambdaBarMCRAPGAPCount[0][0] * templambareffRAPGAP * LambdaRegMCRAPGAPCount[0][0] / Square(LambdaBarMCRAPGAPCount[0][0] * templamregeffRAPGAP + templambareffRAPGAP * LambdaRegMCRAPGAPCount[0][0])) * (0.5 * abs(templambareffDJANGOH - templambareffRAPGAP))));
	//	ySys1[k] = MCerr;
	//	double MCSignalRooFitYieldError = GetAsymError(templamregRAPGAP, GetRatioError(templamregRAPGAP, LambdaRegDataCount[k][0], LambdaRegDataCount[k][2], (LambdaRegMCRAPGAPCount[k][0]) / (LambdaRegGenCountRAPGAP[k]), (LambdaRegMCRAPGAPCount[k][2]) / (LambdaRegGenCountRAPGAP[k])), templambarRAPGAP, GetRatioError(templambarRAPGAP, LambdaBarDataCount[k][0], LambdaBarDataCount[k][2], (LambdaBarMCRAPGAPCount[k][0]) / (LambdaBarGenCountRAPGAP[k]), (LambdaBarMCRAPGAPCount[k][2]) / (LambdaBarGenCountRAPGAP[k])));
	//	ySys2[k] = MCSignalRooFitYieldError;
	//	double extractionrange = GetAsymError(templamregRAPGAP, abs(templamregRAPGAP - (LambdaRegDataCount[k][4]) / ((LambdaRegMCRAPGAPCount[k][4]) / (LambdaRegGenCountRAPGAP[k]))), templambarRAPGAP, abs(templambarRAPGAP - (LambdaBarDataCount[k][4]) / ((LambdaBarMCRAPGAPCount[k][4]) / (LambdaBarGenCountRAPGAP[k]))));
	//	ySys3[k] = extractionrange;
	//	yScatter1[k] = 100*abs(LambdaRegDataCount[k][0] - LambdaRegDataCount[k][4])/ LambdaRegDataCount[k][0]; //Lambda
	//	xScatter1[k] = 100*abs(LambdaBarDataCount[k][0] - LambdaBarDataCount[k][4]) / LambdaBarDataCount[k][0]; //Lambda Bar
	//	yScatterMC1[k] = 100*abs(LambdaRegMCRAPGAPCount[k][0] - LambdaRegMCRAPGAPCount[k][4]) / LambdaRegMCRAPGAPCount[k][0]; //Lambda
	//	xScatterMC1[k] = 100*abs(LambdaBarMCRAPGAPCount[k][0] - LambdaBarMCRAPGAPCount[k][4]) / LambdaBarMCRAPGAPCount[k][0]; //Lambda Bar
	//	double rawextraction = GetAsymError(templamregRAPGAP, abs(templamregRAPGAP - (LambdaRegDataCount[k][5]) / ((LambdaRegMCRAPGAPCount[k][5]) / (LambdaRegGenCountRAPGAP[k]))), templambarRAPGAP, abs(templambarRAPGAP - (LambdaBarDataCount[k][5]) / ((LambdaBarMCRAPGAPCount[k][5]) / (LambdaBarGenCountRAPGAP[k]))));
	//	ySys4[k] = rawextraction;
	//	yScatter2[k] = 100*abs(LambdaRegDataCount[k][0] - LambdaRegDataCount[k][5]) / LambdaRegDataCount[k][0]; //Lambda
	//	xScatter2[k] = 100*abs(LambdaBarDataCount[k][0] - LambdaBarDataCount[k][5]) / LambdaBarDataCount[k][0]; //Lambda Bar
	//	yScatterMC2[k] = 100*abs(LambdaRegMCRAPGAPCount[k][0] - LambdaRegMCRAPGAPCount[k][5]) / LambdaRegMCRAPGAPCount[k][0]; //Lambda
	//	xScatterMC2[k] = 100*abs(LambdaBarMCRAPGAPCount[k][0] - LambdaBarMCRAPGAPCount[k][5]) / LambdaBarMCRAPGAPCount[k][0]; //Lambda Bar
	//	double alternativeBGfit = GetAsymError(templamregRAPGAP, abs(templamregRAPGAP - (LambdaRegDataCount[k][6]) / ((LambdaRegMCRAPGAPCount[k][6]) / (LambdaRegGenCountRAPGAP[k]))), templambarRAPGAP, abs(templambarRAPGAP - (LambdaBarDataCount[k][6]) / ((LambdaBarMCRAPGAPCount[k][6]) / (LambdaBarGenCountRAPGAP[k]))));
	//	ySys5[k] = alternativeBGfit;
	//	yScatter3[k] = 100*abs(LambdaRegDataCount[k][0] - LambdaRegDataCount[k][6]) / LambdaRegDataCount[k][0]; //Lambda
	//	xScatter3[k] = 100*abs(LambdaBarDataCount[k][0] - LambdaBarDataCount[k][6]) / LambdaBarDataCount[k][0]; //Lambda Bar
	//	yScatterMC3[k] = 100*abs(LambdaRegMCRAPGAPCount[k][0] - LambdaRegMCRAPGAPCount[k][6]) / LambdaRegMCRAPGAPCount[k][0]; //Lambda
	//	xScatterMC3[k] = 100*abs(LambdaBarMCRAPGAPCount[k][0] - LambdaBarMCRAPGAPCount[k][6]) / LambdaBarMCRAPGAPCount[k][0]; //Lambda Bar
	//	//Calculating systematic error associated with MC detector interaction contaminations
	//	//LambdaRegBGCountDJANGOH = AnalyzeBGTree(varidx, new TFile(filelambdaTreeMCDJANGOH.c_str(), "read"), "Lambda Reg BG", q2entries, q2bins);
	//	//LambdaRegBGCountRAPGAP = AnalyzeBGTree(varidx, new TFile(filelambdaTreeMCRAPGAP.c_str(), "read"), "Lambda Reg BG", q2entries, q2bins);
	//	//LambdaBarBGCountDJANGOH = AnalyzeBGTree(varidx, new TFile(filelambdaTreeMCDJANGOH.c_str(), "read"), "Lambda Bar BG", q2entries, q2bins);
	//	//LambdaBarBGCountRAPGAP = AnalyzeBGTree(varidx, new TFile(filelambdaTreeMCRAPGAP.c_str(), "read"), "Lambda Bar BG", q2entries, q2bins);
	//	//double percentRAPGAPcontam = LambdaRegBGCountRAPGAP[k]/ LambdaRegMCRAPGAPCount[k][0];
	//	//double percentDJANGOHcontam = LambdaRegBGCountDJANGOH[k]/ LambdaRegMCDJANGOHCount[k][0];
	//	//double averagepercentBGcontam = (percentRAPGAPcontam + percentDJANGOHcontam) / 2;
	//	double percentRAPGAPcontam = AnalyzeBGTree(varidx, new TFile(filelambdaTreeMCRAPGAP.c_str(), "read"), "Lambda Reg BG", q2bins[k], q2bins[k+1], LambdaRegMCRAPGAPCount[k][8], LambdaRegMCRAPGAPCount[k][9]) / AnalyzeBGTree(varidx, new TFile(filelambdaTreeMCRAPGAP.c_str(), "read"), "Lambda Reg Gen", q2bins[k], q2bins[k + 1], LambdaRegMCRAPGAPCount[k][8], LambdaRegMCRAPGAPCount[k][9]);
	//	double percentDJANGOHcontam = AnalyzeBGTree(varidx, new TFile(filelambdaTreeMCDJANGOH.c_str(), "read"), "Lambda Reg BG", q2bins[k], q2bins[k + 1], LambdaRegMCDJANGOHCount[k][8], LambdaRegMCDJANGOHCount[k][9]) / AnalyzeBGTree(varidx, new TFile(filelambdaTreeMCDJANGOH.c_str(), "read"), "Lambda Reg Gen", q2bins[k], q2bins[k + 1], LambdaRegMCDJANGOHCount[k][8], LambdaRegMCDJANGOHCount[k][9]);
	//	//cout << "Percent Num Check: " << AnalyzeBGTree(varidx, new TFile(filelambdaTreeMCRAPGAP.c_str(), "read"), "Lambda Reg BG", q2bins[k], q2bins[k + 1], LambdaRegMCRAPGAPCount[k][8], LambdaRegMCRAPGAPCount[k][9]) << endl;
	//	//cout << "Percent Denom Check: " << AnalyzeBGTree(varidx, new TFile(filelambdaTreeMCRAPGAP.c_str(), "read"), "Lambda Reg Gen", q2bins[k], q2bins[k + 1], LambdaRegMCRAPGAPCount[k][8], LambdaRegMCRAPGAPCount[k][9]) << endl;
	//	//cout << "s1: " << LambdaRegMCRAPGAPCount[k][8] << endl;
	//	//cout << "s2: " << LambdaRegMCRAPGAPCount[k][9] << endl;
	//	cout << "Asym in Bin " << k + 1 << " : " << yAsymm[k] << " +/- " << yAsymmerr[k] << " (stat.)" << endl;
	//	double averagepercentBGcontam = (percentRAPGAPcontam + percentDJANGOHcontam) / 2;
	//	//double templamregeffRAPGAP = (LambdaRegMCRAPGAPCount[k][0]) / (LambdaRegGenCountRAPGAP[k]);
	//	//double templamregRAPGAP = (LambdaRegDataCount[k][0]) / (templamregeffRAPGAP);
	//	double detectorinterror = GetAsymError(templamregRAPGAP, abs(templamregRAPGAP - (LambdaRegDataCount[k][0]+ LambdaRegDataCount[k][0]*averagepercentBGcontam)* (LambdaRegGenCountRAPGAP[k])/ LambdaRegMCRAPGAPCount[k][0]), templambarRAPGAP, 0.0);
	//	ySys6[k] = detectorinterror;
	//	//yAsymmerrSYS[k] = sqrt(Square(MCerr) + Square(MCSignalRooFitYieldError) + Square(extractionrange) + Square(rawextraction) + Square(alternativeBGfit) + Square(detectorinterror));
	//	yAsymmerrSYS[k] = sqrt(Square(MCerr) + Square(extractionrange) + Square(rawextraction) + Square(alternativeBGfit) + Square(detectorinterror));
	//	//yAsymmerrSYS[k] = 0.01;
	//	yAsymmTOTerr[k] = sqrt(Square(yAsymmerrSYS[k]) + Square(yAsymmerr[k]));

	//	LamRegDataStatOverRoo[k] = LambdaRegDataCount[k][2]/ LambdaRegDataCount[k][1];
	//	LamBarDataStatOverRoo[k] = LambdaBarDataCount[k][2] / LambdaBarDataCount[k][1];
	//	LamRegRAPGAPStatOverRoo[k] = LambdaRegMCRAPGAPCount[k][2] / LambdaRegMCRAPGAPCount[k][1];
	//	LamBarRAPGAPStatOverRoo[k] = LambdaBarMCRAPGAPCount[k][2] / LambdaBarMCRAPGAPCount[k][1];
	//	LamRegDJANGOHStatOverRoo[k] = LambdaRegMCDJANGOHCount[k][2] / LambdaRegMCDJANGOHCount[k][1];
	//	LamBarDJANGOHStatOverRoo[k] = LambdaBarMCDJANGOHCount[k][2] / LambdaBarMCDJANGOHCount[k][1];
	//	cout << "Total Error in Bin " << k + 1 << " : " << yAsymmTOTerr[k] << endl;
	//}


	////Plotting RAPGAP and DJANGOH efficiencies
	//TMultiGraph* mgeff = new TMultiGraph();

	//c3->SetName("Detector Efficiencies");
	//TGraphAsymmErrors* LambdaRegRAPGAPeff = new TGraphAsymmErrors(q2entries, xvals, effregplotRAPGAP, xerr, xerr, erreffregplotRAPGAP, erreffregplotRAPGAP);
	//TGraphAsymmErrors* LambdaBarRAPGAPeff = new TGraphAsymmErrors(q2entries, xvals, effbarplotRAPGAP, xerr, xerr, erreffbarplotRAPGAP, erreffbarplotRAPGAP);
	//TGraphAsymmErrors* LambdaRegDJANGOHeff = new TGraphAsymmErrors(q2entries, xvals, effregplotDJANGOH, xerr, xerr, erreffregplotDJANGOH, erreffregplotDJANGOH);
	//TGraphAsymmErrors* LambdaBarDJANGOHeff = new TGraphAsymmErrors(q2entries, xvals, effbarplotDJANGOH, xerr, xerr, erreffbarplotDJANGOH, erreffbarplotDJANGOH);
	//LambdaRegRAPGAPeff->SetLineColor(kBlack);
	//LambdaBarRAPGAPeff->SetLineColor(kBlue);
	//LambdaRegDJANGOHeff->SetLineColor(kRed);
	//LambdaBarDJANGOHeff->SetLineColor(kGreen);

	//mgeff->Add(LambdaRegRAPGAPeff, "AP");
	//mgeff->Add(LambdaBarRAPGAPeff, "AP");
	//mgeff->Add(LambdaRegDJANGOHeff, "AP");
	//mgeff->Add(LambdaBarDJANGOHeff, "AP");

	//TLegend* legend = new TLegend(0.5513, 0.4144, 0.888, 0.8823);
	//legend->AddEntry(LambdaRegRAPGAPeff, "RAPGAP #epsilon_{#Lambda}", "l");
	//legend->AddEntry(LambdaBarRAPGAPeff, "RAPGAP #epsilon_{#bar{#Lambda}}", "l");
	//legend->AddEntry(LambdaRegDJANGOHeff, "DJANGOH #epsilon_{#Lambda}", "l");
	//legend->AddEntry(LambdaBarDJANGOHeff, "DJANGOH #epsilon_{#bar{#Lambda}}", "l");
	////LambdaAsymRAPGAP->Draw("ap");
	////LambdaAsymDJANGOH->SetOption("same");
	////LambdaAsymDJANGOH->Draw();
	//mgeff->Draw("AP");
	//legend->Draw("same");
	//mgeff->SetTitle("Detector Efficiencies");
	//mgeff->GetXaxis()->SetTitle(xaxistitle.c_str());
	//mgeff->GetYaxis()->SetTitle("N_{MC,rec}/N_{MC,gen}");
	//outputfile.cd();
	//c3->SetGrid();
	//c3->Write();
	//f->cd();

	//
	//TMultiGraph* mg = new TMultiGraph();
	//c3->Clear();
	//legend->Clear();
	//c3->SetName("Lambda Asymmetry Measured in Data");
	//TGraphAsymmErrors* LambdaAsymRAPGAP = new TGraphAsymmErrors(q2entries, xvals, y1, xerr, xerr, y1err, y1err);
	//TGraphAsymmErrors* LambdaAsymDJANGOH = new TGraphAsymmErrors(q2entries, xvals, y2, xerr, xerr, y2err, y2err);
	//LambdaAsymRAPGAP->SetLineColor(kBlack);
	//LambdaAsymDJANGOH->SetLineColor(kRed);
	//mg->Add(LambdaAsymRAPGAP,"AP");
	//mg->Add(LambdaAsymDJANGOH, "AP");
	////TLegend* legend = new TLegend(0.5513, 0.4144, 0.888, 0.8823);
	//legend->AddEntry(LambdaAsymRAPGAP, "#epsilon_{#Lambda} with RAPGAP", "l");
	//legend->AddEntry(LambdaAsymDJANGOH, "#epsilon_{#Lambda} with DJANGOH", "l");
	////LambdaAsymRAPGAP->Draw("ap");
	////LambdaAsymDJANGOH->SetOption("same");
	////LambdaAsymDJANGOH->Draw();
	//mg->Draw("AP");
	//legend->Draw("same");
	//mg->SetTitle("#frac{#sigma(ep #rightarrow e #Lambda X) - #sigma(ep #rightarrow e #bar{ #Lambda } X)}{#sigma(ep #rightarrow e #Lambda X) + #sigma(ep #rightarrow e #bar{ #Lambda } X)} [2005-2007 HERA II DST7]");
	//mg->GetXaxis()->SetTitle(xaxistitle.c_str());
	//mg->GetYaxis()->SetTitle("A_{#Lambda}");
	//outputfile.cd();
	//c3->SetGrid();
	//c3->Write();
	//f->cd();

	//TMultiGraph* mgAsym = new TMultiGraph();
	//c3->Clear();
	//legend->Clear();
	//c3->SetName("Asymmetry");
	//TGraphAsymmErrors* LambdaAsym = new TGraphAsymmErrors(q2entries, xvals, yAsymm, xerr, xerr, yAsymmerr, yAsymmerr);
	//TGraphAsymmErrors* LambdaAsymSYS = new TGraphAsymmErrors(q2entries, xvals, yAsymm, xerr, xerr, yAsymmerrSYS, yAsymmerrSYS);
	//TGraphAsymmErrors* LambdaAsymTOT = new TGraphAsymmErrors(q2entries, xvals, yAsymm, xerr, xerr, yAsymmTOTerr, yAsymmTOTerr);
	//LambdaAsym->SetLineColor(kBlue);
	//LambdaAsymSYS->SetLineColor(kRed);
	//LambdaAsymTOT->SetLineColor(kBlack);
	//legend->AddEntry(LambdaAsym, "Statistical", "l");
	//legend->AddEntry(LambdaAsymSYS, "Systematic", "l");
	//legend->AddEntry(LambdaAsymTOT, "Total", "l");
	//mgAsym->Add(LambdaAsym, "AP");
	//mgAsym->Add(LambdaAsymSYS, "AP");
	//mgAsym->Add(LambdaAsymTOT, "AP");
	//mgAsym->Draw("AP");
	//legend->Draw("same");
	//mgAsym->SetTitle("#frac{#sigma(ep #rightarrow e #Lambda X) - #sigma(ep #rightarrow e #bar{ #Lambda } X)}{#sigma(ep #rightarrow e #Lambda X) + #sigma(ep #rightarrow e #bar{ #Lambda } X)} [2005-2007 HERA II DST7]");
	//mgAsym->GetXaxis()->SetTitle(xaxistitle.c_str());
	//mgAsym->GetYaxis()->SetTitle("A_{#Lambda}");
	//outputfile.cd();
	//c3->SetGrid();
	//c3->Write();
	//f->cd();

	////Checking RooFit error with Statistical, they should be very similar if RooFits are working well
	//TMultiGraph* mgRooErrCheck = new TMultiGraph();
	//c3->Clear();
	//legend->Clear();
	//c3->SetName("Statistical Over RooFit");
	//TGraphAsymmErrors* Roo1 = new TGraphAsymmErrors(q2entries, xvals, LamRegDataStatOverRoo, xerr, xerr, 0, 0);
	//TGraphAsymmErrors* Roo2 = new TGraphAsymmErrors(q2entries, xvals, LamBarDataStatOverRoo, xerr, xerr, 0, 0);
	//TGraphAsymmErrors* Roo3 = new TGraphAsymmErrors(q2entries, xvals, LamRegRAPGAPStatOverRoo, xerr, xerr, 0, 0);
	//TGraphAsymmErrors* Roo4 = new TGraphAsymmErrors(q2entries, xvals, LamBarRAPGAPStatOverRoo, xerr, xerr, 0, 0);
	//TGraphAsymmErrors* Roo5 = new TGraphAsymmErrors(q2entries, xvals, LamRegDJANGOHStatOverRoo, xerr, xerr, 0, 0);
	//TGraphAsymmErrors* Roo6 = new TGraphAsymmErrors(q2entries, xvals, LamBarDJANGOHStatOverRoo, xerr, xerr, 0, 0);
	//
	//Roo1->SetLineColor(kBlack);
	//Roo2->SetLineColor(kRed);
	//Roo3->SetLineColor(kBlue);
	//Roo4->SetLineColor(kGreen);
	//Roo5->SetLineColor(kMagenta);
	//Roo6->SetLineColor(kOrange);
	//
	//legend->AddEntry(Roo1, "#Lambda Data", "l");
	//legend->AddEntry(Roo2, "#bar{#Lambda} Data", "l");
	//legend->AddEntry(Roo3, "#Lambda RAPGAP", "l");
	//legend->AddEntry(Roo4, "#bar{#Lambda} RAPGAP", "l");
	//legend->AddEntry(Roo5, "#Lambda DJANGOH", "l");
	//legend->AddEntry(Roo6, "#bar{#Lambda} DJANGOH", "l");
	//
	//mgRooErrCheck->Add(Roo1, "AP");
	//mgRooErrCheck->Add(Roo2, "AP");
	//mgRooErrCheck->Add(Roo3, "AP");
	//mgRooErrCheck->Add(Roo4, "AP");
	//mgRooErrCheck->Add(Roo5, "AP");
	//mgRooErrCheck->Add(Roo6, "AP");
	//
	///*Sys1->Draw("hist");
	//Sys2->Draw("histsame");
	//Sys3->Draw("histsame");
	//Sys4->Draw("histsame");
	//Sys5->Draw("histsame");
	//Sys6->Draw("histsame");*/
	//mgRooErrCheck->Draw("AP");
	//legend->Draw("same");
	//mgRooErrCheck->SetTitle("#delta#Lambda_{RooFit} / #delta#Lambda_{Stat}");
	//mgRooErrCheck->GetXaxis()->SetTitle(xaxistitle.c_str());
	//mgRooErrCheck->GetYaxis()->SetTitle("Ratio");
	//outputfile.cd();
	//c3->Update();
	//c3->SetGrid();
	//c3->Write();
	//f->cd();

	////Plotting Individual Systematic Error Contributions
	//TMultiGraph* mgSys = new TMultiGraph();
	//c3->Clear();
	//legend->Clear();
	//c3->SetName("Error Contributions");
	//TGraphAsymmErrors* Sys1 = new TGraphAsymmErrors(q2entries, xvals, ySys1, xerr, xerr, 0, 0);
	//TGraphAsymmErrors* Sys2 = new TGraphAsymmErrors(q2entries, xvals, ySys2, xerr, xerr, 0, 0);
	//TGraphAsymmErrors* Sys3 = new TGraphAsymmErrors(q2entries, xvals, ySys3, xerr, xerr, 0, 0);
	//TGraphAsymmErrors* Sys4 = new TGraphAsymmErrors(q2entries, xvals, ySys4, xerr, xerr, 0, 0);
	//TGraphAsymmErrors* Sys5 = new TGraphAsymmErrors(q2entries, xvals, ySys5, xerr, xerr, 0, 0);
	//TGraphAsymmErrors* Sys6 = new TGraphAsymmErrors(q2entries, xvals, ySys6, xerr, xerr, 0, 0);
	//TGraphAsymmErrors* STATerr = new TGraphAsymmErrors(q2entries, xvals, yAsymmerr, xerr, xerr, 0, 0);
	//Sys1->SetLineColor(kBlack);
	//Sys2->SetLineColor(kRed);
	//Sys3->SetLineColor(kBlue);
	//Sys4->SetLineColor(kGreen);
	//Sys5->SetLineColor(kMagenta);
	//Sys6->SetLineColor(kOrange);
	//STATerr->SetLineColor(kCyan);
	//legend->AddEntry(Sys1, "DJANGOH vs. RAPGAP", "l");
	//legend->AddEntry(Sys2, "RooFit Error", "l");
	//legend->AddEntry(Sys3, "Extraction Range", "l");
	//legend->AddEntry(Sys4, "Raw Signal Extraction", "l");
	//legend->AddEntry(Sys5, "Alternative BG Fit", "l");
	//legend->AddEntry(Sys6, "Detector Interactions", "l");
	//legend->AddEntry(STATerr, "Statistical", "l");
	//mgSys->Add(Sys1, "AP");
	//mgSys->Add(Sys2,"AP");
	//mgSys->Add(Sys3,"AP");
	//mgSys->Add(Sys4,"AP");
	//mgSys->Add(Sys5,"AP");
	//mgSys->Add(Sys6,"AP");
	//mgSys->Add(STATerr, "AP");
	///*Sys1->Draw("hist");
	//Sys2->Draw("histsame");
	//Sys3->Draw("histsame");
	//Sys4->Draw("histsame");
	//Sys5->Draw("histsame");
	//Sys6->Draw("histsame");*/
	//mgSys->Draw("AP");
	//legend->Draw("same");
	//mgSys->SetTitle("Uncertainty Contributions");
	//mgSys->GetXaxis()->SetTitle(xaxistitle.c_str());
	//mgSys->GetYaxis()->SetTitle("Uncertainty #deltaA_{#Lambda}");
	//outputfile.cd();
	//c3->Update();
	//c3->SetGrid();
	//c3->Write();
	//f->cd();

	////Plotting Individual Systematic Error Contributions
	//TMultiGraph* mgScatter = new TMultiGraph();
	//cSquare->Clear();
	//legend->Clear();
	//cSquare->SetName("Yield Variation Scatter Plot");
	//TGraph* Scatter1 = new TGraphAsymmErrors(q2entries, xScatter1, yScatter1);
	//TGraph* ScatterMC1 = new TGraphAsymmErrors(q2entries, xScatterMC1, yScatterMC1);
	//TGraph* Scatter2 = new TGraphAsymmErrors(q2entries, xScatter2, yScatter2);
	//TGraph* ScatterMC2 = new TGraphAsymmErrors(q2entries, xScatterMC2, yScatterMC2);
	//TGraph* Scatter3 = new TGraphAsymmErrors(q2entries, xScatter3, yScatter3);
	//TGraph* ScatterMC3 = new TGraphAsymmErrors(q2entries, xScatterMC3, yScatterMC3);
	//TLine xequalsyline = TLine(0, 0, 6, 6);
	//xequalsyline.SetLineColor(kBlack);
	//Scatter1->SetMarkerColor(kGreen);
	//ScatterMC1->SetMarkerColor(kGreen);
	//Scatter2->SetMarkerColor(kRed);
	//ScatterMC2->SetMarkerColor(kRed);
	//Scatter3->SetMarkerColor(kBlue);
	//ScatterMC3->SetMarkerColor(kBlue);
	//double markersize = 1.0;
	//Scatter1->SetMarkerSize(markersize);
	//ScatterMC1->SetMarkerSize(markersize);
	//Scatter2->SetMarkerSize(markersize);
	//ScatterMC2->SetMarkerSize(markersize);
	//Scatter3->SetMarkerSize(markersize);
	//ScatterMC3->SetMarkerSize(markersize);
	//Scatter1->SetMarkerStyle(20);
	//ScatterMC1->SetMarkerStyle(4);
	//Scatter2->SetMarkerStyle(20);
	//ScatterMC2->SetMarkerStyle(4);
	//Scatter3->SetMarkerStyle(20);
	//ScatterMC3->SetMarkerStyle(4);
	//legend->AddEntry(Scatter1, "Extended Extraction Range", "p");
	//legend->AddEntry(Scatter2, "Raw Signal Extraction", "p");
	//legend->AddEntry(Scatter3, "Different BG Function", "p");
	//legend->SetHeader("Filled = Data   Open = MC");
	//mgScatter->Add(Scatter1,"AP");
	//mgScatter->Add(ScatterMC1, "AP");
	//mgScatter->Add(Scatter2,"AP");
	//mgScatter->Add(ScatterMC2,"AP");
	//mgScatter->Add(Scatter3,"AP");
	//mgScatter->Add(ScatterMC3,"AP");
	//mgScatter->Draw("AP");
	//mgScatter->GetXaxis()->SetLimits(0., 6.);
	//mgScatter->GetYaxis()->SetLimits(0., 6.);
	//legend->Draw("same");
	//xequalsyline.Draw("same");
	//mgScatter->SetTitle("Yield Variations");
	//mgScatter->GetYaxis()->SetTitle("#delta#Lambda (%)");
	//mgScatter->GetXaxis()->SetTitle("#delta#bar{#Lambda} (%)");
	//outputfile.cd();
	//cSquare->Modified();
	//cSquare->SetGrid();
	//cSquare->Write();
	//f->cd();
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// PLOTTING RAPGAP GENERATOR ASSYMETRY HISTOGRAM
	// THEN CALCULATING MEASURED RAPGAP ASYMMETRY USING DJANGOH FOR THE EFFICIENCY
	//TH1F* GenHist = new TH1F("Gen Hist", "GenHist", q2entries, binarray);
	//TH1F* GenHistWithBG = new TH1F("Gen Hist BG Included", "GenHist BG Included", q2entries, binarray);
	////Getting data for MC Gen Asymmetry
	//cout << "q2entries: " << q2entries << endl;
	//for (int k = 0; k < q2entries; k++) {
	//	
	//	y1[k] = (LambdaRegGenCountRAPGAP[k] - LambdaBarGenCountRAPGAP[k]) / (LambdaRegGenCountRAPGAP[k] + LambdaBarGenCountRAPGAP[k]);
	//	y2[k] = ((LambdaRegGenCountRAPGAP[k] + LambdaRegBGGenCountRAPGAP[k]) - (LambdaBarGenCountRAPGAP[k]+ LambdaBarBGGenCountRAPGAP[k])) / ((LambdaRegGenCountRAPGAP[k]+ LambdaRegBGGenCountRAPGAP[k]) + (LambdaBarGenCountRAPGAP[k]+ LambdaBarBGGenCountRAPGAP[k]));
	//	GenHist->SetBinContent(k+1, y1[k]);
	//	GenHistWithBG->SetBinContent(k + 1, y2[k]);
	//	cout << "Bin Width: " << GenHist->GetBinWidth(k) << endl;
	//	cout << "Gen Value: " << y2[k] << endl;
	//	
	//	//double templamregeffDJANGOH = (LambdaRegMCDJANGOHCount[k][0]) / (LambdaRegGenCountDJANGOH[k] + LambdaRegBGGenCountRAPGAP[k]);
	//	double templamregeffDJANGOH = (LambdaRegMCDJANGOHCount[k][0]) / (LambdaRegGenCountDJANGOH[k]);
	//	double templambareffDJANGOH = (LambdaBarMCDJANGOHCount[k][0]) / (LambdaBarGenCountDJANGOH[k] + LambdaBarBGGenCountRAPGAP[k]);
	//	double templamregefferrDJANGOH = GetRatioError(templamregeffDJANGOH, LambdaRegMCDJANGOHCount[k][0], LambdaRegMCDJANGOHCount[k][1], LambdaRegGenCountDJANGOH[k], 0);
	//	double templambarefferrDJANGOH = GetRatioError(templambareffDJANGOH, LambdaBarMCDJANGOHCount[k][0], LambdaBarMCDJANGOHCount[k][1], LambdaBarGenCountDJANGOH[k], 0);
	//	double templamregDJANGOH = (LambdaRegMCRAPGAPCount[k][0]) / (templamregeffDJANGOH);
	//	double templambarDJANGOH = (LambdaBarMCRAPGAPCount[k][0]) / (templambareffDJANGOH);
	//	double templamregerrDJANGOH = GetRatioError((LambdaRegMCRAPGAPCount[k][0]) / (templamregeffDJANGOH), (LambdaRegMCRAPGAPCount[k][0]), (LambdaRegMCRAPGAPCount[k][1]), (templamregeffDJANGOH), templamregefferrDJANGOH);
	//	double templambarerrDJANGOH = GetRatioError((LambdaBarMCRAPGAPCount[k][0]) / (templambareffDJANGOH), (LambdaBarMCRAPGAPCount[k][0]), (LambdaBarMCRAPGAPCount[k][1]), (templambareffDJANGOH), templambarefferrDJANGOH);
	//	y2[k] = (templamregDJANGOH - templambarDJANGOH) / ((templamregDJANGOH + templambarDJANGOH));


	//	/*double templamregeffRAPGAP = (LambdaRegMCRAPGAPCount[k][0]) / (LambdaRegGenCountRAPGAP[k]);
	//	double templambareffRAPGAP = (LambdaBarMCRAPGAPCount[k][0]) / (LambdaBarGenCountRAPGAP[k]);
	//	double templamregefferrRAPGAP = GetRatioError(templamregeffRAPGAP, LambdaRegMCRAPGAPCount[k][0], LambdaRegMCRAPGAPCount[k][1], LambdaRegGenCountRAPGAP[k], 0);
	//	double templambarefferrRAPGAP = GetRatioError(templambareffRAPGAP, LambdaBarMCRAPGAPCount[k][0], LambdaBarMCRAPGAPCount[k][1], LambdaBarGenCountRAPGAP[k], 0);
	//	double templamregRAPGAP = (LambdaRegMCRAPGAPCount[k][0]) / (templamregeffRAPGAP);
	//	double templambarRAPGAP = (LambdaBarMCRAPGAPCount[k][0]) / (templambareffRAPGAP);
	//	double templamregerrRAPGAP = GetRatioError((LambdaRegMCRAPGAPCount[k][0]) / (templamregeffRAPGAP), (LambdaRegMCRAPGAPCount[k][0]), (LambdaRegMCRAPGAPCount[k][1]), (templamregeffRAPGAP), templamregefferrRAPGAP);
	//	double templambarerrRAPGAP = GetRatioError((LambdaBarMCRAPGAPCount[k][0]) / (templambareffRAPGAP), (LambdaBarMCRAPGAPCount[k][0]), (LambdaBarMCRAPGAPCount[k][1]), (templambareffRAPGAP), templambarefferrRAPGAP);
	//	y2[k] = (templamregRAPGAP - templambarRAPGAP) / ((templamregRAPGAP + templambarRAPGAP));*/


	//	
	//	//y2err[k] = GetRatioError(((templamregRAPGAP - templambarRAPGAP) / ((templamregRAPGAP + templambarRAPGAP))), (templamregRAPGAP - templambarRAPGAP), (templamregerrRAPGAP + templambarerrRAPGAP), ((templamregRAPGAP + templambarRAPGAP)), ((templamregerrRAPGAP + templambarerrRAPGAP)));

	//	y2err[k] = GetRatioError(((templamregDJANGOH - templambarDJANGOH) / ((templamregDJANGOH + templambarDJANGOH))), (templamregDJANGOH - templambarDJANGOH), (templamregerrDJANGOH + templambarerrDJANGOH), ((templamregDJANGOH + templambarDJANGOH)), ((templamregerrDJANGOH + templambarerrDJANGOH)));
	//	
	//}

	//c3->Clear();
	//mg->Clear();
	//legend->Clear();
	//c3->SetName("RAPGAP Cross Check with DJANGOH Efficiency");
	//TGraphAsymmErrors* RAPGAPmeasuredAsym = new TGraphAsymmErrors(q2entries, xvals, y2, xerr, xerr, y2err, y2err);
	//GenHist->SetLineColor(kBlack);
	//GenHistWithBG->SetLineColor(kBlue);
	//RAPGAPmeasuredAsym->SetLineColor(kRed);
	//legend->AddEntry(GenHist, "Generator Asymmetry in RAPGAP", "l");
	//legend->AddEntry(GenHistWithBG, "Generator Asymmetry in RAPGAP + BG", "l");
	//legend->AddEntry(RAPGAPmeasuredAsym, "Measured A_{#Lambda} in RAPGAP", "l");
	////GenAsym->Draw("APF");
	//GenHist->SetStats(0);
	//RAPGAPmeasuredAsym->Draw("AP");
	//GenHist->Draw("same");
	//GenHistWithBG->Draw("same");
	//legend->Draw("same");
	//RAPGAPmeasuredAsym->SetTitle("RAPGAP Cross Check with DJANGOH Efficiency");
	//RAPGAPmeasuredAsym->GetXaxis()->SetTitle(xaxistitle.c_str());
	//RAPGAPmeasuredAsym->GetYaxis()->SetTitle("A_{#Lambda}");
	//outputfile.cd();
	//c3->Write();
	//f->cd();

	//
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//// PLOTTING DJANGOH GENERATOR ASSYMETRY HISTOGRAM
	//// THEN CALCULATING MEASURED DJANGOH ASYMMETRY USING RAPGAP FOR THE EFFICIENCY
	//GenHist->Clear();
	//GenHistWithBG->Clear();
	////Getting data for MC Gen Asymmetry
	//cout << "q2entries: " << q2entries << endl;
	//for (int k = 0; k < q2entries; k++) {

	//	y1[k] = (LambdaRegGenCountDJANGOH[k] - LambdaBarGenCountDJANGOH[k]) / (LambdaRegGenCountDJANGOH[k] + LambdaBarGenCountDJANGOH[k]);
	//	y2[k] = ((LambdaRegGenCountDJANGOH[k] + LambdaRegBGGenCountDJANGOH[k]) - (LambdaBarGenCountDJANGOH[k] + LambdaBarBGGenCountDJANGOH[k])) / ((LambdaRegGenCountDJANGOH[k] + LambdaRegBGGenCountDJANGOH[k]) + (LambdaBarGenCountDJANGOH[k] + LambdaBarBGGenCountDJANGOH[k]));
	//	GenHist->SetBinContent(k + 1, y1[k]);
	//	GenHistWithBG->SetBinContent(k + 1, y2[k]);
	//	cout << "Bin Width: " << GenHist->GetBinWidth(k) << endl;

	//	//double templamregeffRAPGAP = (LambdaRegMCRAPGAPCount[k][0]) / (LambdaRegGenCountRAPGAP[k]+ LambdaRegBGGenCountDJANGOH[k]);
	//	double templamregeffRAPGAP = (LambdaRegMCRAPGAPCount[k][0]) / (LambdaRegGenCountRAPGAP[k]);
	//	double templambareffRAPGAP = (LambdaBarMCRAPGAPCount[k][0]) / (LambdaBarGenCountRAPGAP[k]+ LambdaBarBGGenCountDJANGOH[k]);
	//	double templamregefferrRAPGAP = GetRatioError(templamregeffRAPGAP, LambdaRegMCRAPGAPCount[k][0], LambdaRegMCRAPGAPCount[k][1], LambdaRegGenCountRAPGAP[k], 0);
	//	double templambarefferrRAPGAP = GetRatioError(templambareffRAPGAP, LambdaBarMCRAPGAPCount[k][0], LambdaBarMCRAPGAPCount[k][1], LambdaBarGenCountRAPGAP[k], 0);
	//	double templamregRAPGAP = (LambdaRegMCDJANGOHCount[k][0]) / (templamregeffRAPGAP);
	//	double templambarRAPGAP = (LambdaBarMCDJANGOHCount[k][0]) / (templambareffRAPGAP);
	//	double templamregerrRAPGAP = GetRatioError((LambdaRegMCDJANGOHCount[k][0]) / (templamregeffRAPGAP), (LambdaRegMCDJANGOHCount[k][0]), (LambdaRegMCDJANGOHCount[k][1]), (templamregeffRAPGAP), templamregefferrRAPGAP);
	//	double templambarerrRAPGAP = GetRatioError((LambdaBarMCDJANGOHCount[k][0]) / (templambareffRAPGAP), (LambdaBarMCDJANGOHCount[k][0]), (LambdaBarMCDJANGOHCount[k][1]), (templambareffRAPGAP), templambarefferrRAPGAP);
	//	y2[k] = (templamregRAPGAP - templambarRAPGAP) / ((templamregRAPGAP + templambarRAPGAP));


	//	y2err[k] = GetRatioError(((templamregRAPGAP - templambarRAPGAP) / ((templamregRAPGAP + templambarRAPGAP))), (templamregRAPGAP - templambarRAPGAP), (templamregerrRAPGAP + templambarerrRAPGAP), ((templamregRAPGAP + templambarRAPGAP)), ((templamregerrRAPGAP + templambarerrRAPGAP)));

	//}

	//c3->Clear();
	//mg->Clear();
	//legend->Clear();
	//c3->SetName("DJANGOH Cross Check with RAPGAP Efficiency");
	//TGraphAsymmErrors* DJANGOHmeasuredAsym = new TGraphAsymmErrors(q2entries, xvals, y2, xerr, xerr, y2err, y2err);
	//GenHist->SetLineColor(kBlack);
	//GenHistWithBG->SetLineColor(kBlue);
	//DJANGOHmeasuredAsym->SetLineColor(kRed);
	//legend->AddEntry(GenHist, "Generator Asymmetry in DJANGOH", "l");
	//legend->AddEntry(GenHistWithBG, "Generator Asymmetry in DJANGOH + BG", "l");
	//legend->AddEntry(DJANGOHmeasuredAsym, "Measured A_{#Lambda} in DJANGOH", "l");
	////GenAsym->Draw("APF");
	//GenHist->SetStats(0);
	//DJANGOHmeasuredAsym->Draw("AP");
	//GenHist->Draw("same");
	//GenHistWithBG->Draw("same");
	//legend->Draw("same");
	//DJANGOHmeasuredAsym->SetTitle("DJANGOH Cross Check with RAPGAP Efficiency");
	//DJANGOHmeasuredAsym->GetXaxis()->SetTitle(xaxistitle.c_str());
	//DJANGOHmeasuredAsym->GetYaxis()->SetTitle("A_{#Lambda}");
	//outputfile.cd();
	//c3->Write();
	//f->cd();

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
	
	
	//Inclusive Asymmetry Measurement
	/////////////////////////////////////

	LambdaRegDataCountincl = AnalyzeTree(varidx, new TFile(filelambdaTree.c_str(), "read"), "Lambda Reg", 1, 0, LambdaRegDataxincl, { q2bins[0],q2bins[q2entries] }, "#Lambda Data Extraction", c3, c1, { {0.00205,3.0,1.1156} }, { {1.083,1.1,1.137,1.16} });
	LambdaRegMCRAPGAPCountincl = AnalyzeTree(varidx, new TFile(filelambdaTreeMCRAPGAP.c_str(), "read"), "Lambda Reg", 1, 0, LambdaRegMCRAPGAPxincl, { q2bins[0],q2bins[q2entries] }, "#Lambda RAPGAP Extraction", c3, c1, { {0.00168, 2.5,1.1156} }, { {1.082,1.1,1.137,1.16} });
	LambdaRegMCDJANGOHCountincl = AnalyzeTree(varidx, new TFile(filelambdaTreeMCDJANGOH.c_str(), "read"), "Lambda Reg", 1, 0, LambdaRegMCDJANGOHxincl, { q2bins[0],q2bins[q2entries] }, "#Lambda DJANGOH Extraction", c3, c1, { {0.00168, 2.5,1.1156} }, { {1.082,1.1,1.137,1.16} });

	LambdaBarDataCountincl = AnalyzeTree(varidx, new TFile(filelambdaTree.c_str(), "read"), "Lambda Bar", 1, 0, LambdaBarDataxincl, { q2bins[0],q2bins[q2entries] }, "#bar{#Lambda} Data Extraction", c3, c1, { {0.00209,3.0,1.1156} }, { {1.09,1.098,1.137,1.155} });
	LambdaBarMCRAPGAPCountincl = AnalyzeTree(varidx, new TFile(filelambdaTreeMCRAPGAP.c_str(), "read"), "Lambda Bar", 1, 0, LambdaBarMCRAPGAPxincl, { q2bins[0],q2bins[q2entries] }, "#bar{#Lambda} RAPGAP Extraction", c3, c1, { {0.0017,2.5,1.1156} }, { {1.081,1.096,1.137,1.155} });
	LambdaBarMCDJANGOHCountincl = AnalyzeTree(varidx, new TFile(filelambdaTreeMCDJANGOH.c_str(), "read"), "Lambda Bar", 1, 0, LambdaBarMCDJANGOHxincl, { q2bins[0],q2bins[q2entries] }, "#bar{#Lambda} DJANGOH Extraction", c3, c1, { {0.00168,2.5,1.1156} }, { {1.081,1.096,1.137,1.155} });



	//LambdaTotGenCountinclRAPGAP = AnalyzeGenTree(varidx, new TFile(filegenMCTreeRAPGAP.c_str(), "read"), "Lambda Tot Channel", 1, { q2bins[0],q2bins[q2entries] });
	LambdaRegGenCountinclRAPGAP = AnalyzeGenTree(varidx, new TFile(filegenMCTreeRAPGAP.c_str(), "read"), "Lambda Reg Channel", 1, { q2bins[0],q2bins[q2entries] });
	LambdaBarGenCountinclRAPGAP = AnalyzeGenTree(varidx, new TFile(filegenMCTreeRAPGAP.c_str(), "read"), "Lambda Bar Channel", 1, { q2bins[0],q2bins[q2entries] });
	
	//LambdaTotGenCountinclDJANGOH = AnalyzeGenTree(varidx, new TFile(filegenMCTreeDJANGOH.c_str(), "read"), "Lambda Tot Channel", 1, { q2bins[0],q2bins[q2entries] });
	LambdaRegGenCountinclDJANGOH = AnalyzeGenTree(varidx, new TFile(filegenMCTreeDJANGOH.c_str(), "read"), "Lambda Reg Channel", 1, { q2bins[0],q2bins[q2entries] });
	LambdaBarGenCountinclDJANGOH = AnalyzeGenTree(varidx, new TFile(filegenMCTreeDJANGOH.c_str(), "read"), "Lambda Bar Channel", 1, { q2bins[0],q2bins[q2entries] });

	////////////////////////////////////////////////////////////////////////////////////////////////
	//Calculating Inclusive Asymmetry
	
	//RAPGAP
	//efficiencies
	double tempINClamregeffRAPGAP = (LambdaRegMCRAPGAPCountincl[0][0]) / (LambdaRegGenCountinclRAPGAP[0]);
	double tempINClambareffRAPGAP = (LambdaBarMCRAPGAPCountincl[0][0]) / (LambdaBarGenCountinclRAPGAP[0]);
	//Calculating detector efficiency statistical from binomial process standard deviation
	double tempINClamregefferrRAPGAP = sqrt(tempINClamregeffRAPGAP * (1 - tempINClamregeffRAPGAP) / (LambdaRegGenCountinclRAPGAP[0]));
	double tempINClambarefferrRAPGAP = sqrt(tempINClambareffRAPGAP * (1 - tempINClambareffRAPGAP) / (LambdaBarGenCountinclRAPGAP[0]));
	//Caclulating the lambda and lambda bar counts from RAPGAP efficiencies
	double tempINClamregRAPGAP = (LambdaRegDataCountincl[0][0]) / (tempINClamregeffRAPGAP);
	double tempINClambarRAPGAP = (LambdaBarDataCountincl[0][0]) / (tempINClambareffRAPGAP);
	//Propagating error through lambda and lambda bar calculation
	double tempINClamregerrRAPGAP = GetRatioError((LambdaRegDataCountincl[0][0]) / (tempINClamregeffRAPGAP), (LambdaRegDataCountincl[0][0]), (LambdaRegDataCountincl[0][1]), (tempINClamregeffRAPGAP), tempINClamregefferrRAPGAP);
	double tempINClambarerrRAPGAP = GetRatioError((LambdaBarDataCountincl[0][0]) / (tempINClambareffRAPGAP), (LambdaBarDataCountincl[0][0]), (LambdaBarDataCountincl[0][1]), (tempINClambareffRAPGAP), tempINClambarefferrRAPGAP);
	//Calculating the lambda Asymmetry
	double INCASYMRAPGAP = (tempINClamregRAPGAP - tempINClambarRAPGAP) / ((tempINClamregRAPGAP + tempINClambarRAPGAP));
	//Calculating the statistical error of the asymmetry
	double INCASYMstaterrRAPGAP = GetAsymError(tempINClamregRAPGAP, tempINClamregerrRAPGAP, tempINClambarRAPGAP, tempINClambarerrRAPGAP);

	//DJANGOH
	//efficiencies
	double tempINClamregeffDJANGOH = (LambdaRegMCDJANGOHCountincl[0][0]) / (LambdaRegGenCountinclDJANGOH[0]);
	double tempINClambareffDJANGOH = (LambdaBarMCDJANGOHCountincl[0][0]) / (LambdaBarGenCountinclDJANGOH[0]);
	//Calculating detector efficiency statistical from binomial process standard deviation
	double tempINClamregefferrDJANGOH = sqrt(tempINClamregeffDJANGOH * (1 - tempINClamregeffDJANGOH) / (LambdaRegGenCountinclDJANGOH[0]));
	double tempINClambarefferrDJANGOH = sqrt(tempINClambareffDJANGOH * (1 - tempINClambareffDJANGOH) / (LambdaBarGenCountinclDJANGOH[0]));
	//Caclulating the lambda and lambda bar counts from DJANGOH efficiencies
	double tempINClamregDJANGOH = (LambdaRegDataCountincl[0][0]) / (tempINClamregeffDJANGOH);
	double tempINClambarDJANGOH = (LambdaBarDataCountincl[0][0]) / (tempINClambareffDJANGOH);
	//Propagating error through lambda and lambda bar calculation
	double tempINClamregerrDJANGOH = GetRatioError((LambdaRegDataCountincl[0][0]) / (tempINClamregeffDJANGOH), (LambdaRegDataCountincl[0][0]), (LambdaRegDataCountincl[0][1]), (tempINClamregeffDJANGOH), tempINClamregefferrDJANGOH);
	double tempINClambarerrDJANGOH = GetRatioError((LambdaBarDataCountincl[0][0]) / (tempINClambareffDJANGOH), (LambdaBarDataCountincl[0][0]), (LambdaBarDataCountincl[0][1]), (tempINClambareffDJANGOH), tempINClambarefferrDJANGOH);
	//Calculating the lambda Asymmetry
	double INCASYMDJANGOH = (tempINClamregDJANGOH - tempINClambarDJANGOH) / ((tempINClamregDJANGOH + tempINClambarDJANGOH));
	//Calculating the statistical error of the asymmetry
	double INCASYMstaterrDJANGOH = GetAsymError(tempINClamregDJANGOH, tempINClamregerrDJANGOH, tempINClambarDJANGOH, tempINClambarerrDJANGOH);
	
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//Systematic Uncertainties
	//Will calculate these by observing direct differences to the Asymmetry results for various methods of peak extractions and detector efficiency calculations
	//return { signalyieldvalue ,countstaterror, signalyieldvalueerror, bgyielddvalue, signalyieldvalueextended, totalcountwithbg - bgyielddvalue, altsignalyieldvalue, 0, s1,s2 };
	//ALT BG FIT CALCULATION
	double tempINClamregeffRAPGAPaltBGfit = (LambdaRegMCRAPGAPCountincl[0][6]) / (LambdaRegGenCountinclRAPGAP[0]);
	double tempINClambareffRAPGAPaltBGfit = (LambdaBarMCRAPGAPCountincl[0][6]) / (LambdaBarGenCountinclRAPGAP[0]);
	double tempINClamregRAPGAPaltBGfit = (LambdaRegDataCountincl[0][6]) / (tempINClamregeffRAPGAPaltBGfit);
	double tempINClambarRAPGAPaltBGfit = (LambdaBarDataCountincl[0][6]) / (tempINClambareffRAPGAPaltBGfit);
	double INCASYMRAPGAPaltBGfit = (tempINClamregRAPGAPaltBGfit - tempINClambarRAPGAPaltBGfit) / ((tempINClamregRAPGAPaltBGfit + tempINClambarRAPGAPaltBGfit));
	double INCalternativeBGfitDirectDiff = abs(INCASYMRAPGAPaltBGfit - INCASYMRAPGAP);
	cout << " " << endl;
	cout << "Alternate BG Fit" << endl;
	cout << "Asymmetry: " << INCASYMRAPGAPaltBGfit << endl;
	cout << "Difference in Asymmetry: " << INCalternativeBGfitDirectDiff << endl;
	cout << " " << endl;

	//EXTENDED EXTRACTION RANGE CALCULATION
	double tempINClamregeffRAPGAPExtendedRange = (LambdaRegMCRAPGAPCountincl[0][4]) / (LambdaRegGenCountinclRAPGAP[0]);
	double tempINClambareffRAPGAPExtendedRange = (LambdaBarMCRAPGAPCountincl[0][4]) / (LambdaBarGenCountinclRAPGAP[0]);
	double tempINClamregRAPGAPExtendedRange = (LambdaRegDataCountincl[0][4]) / (tempINClamregeffRAPGAPExtendedRange);
	double tempINClambarRAPGAPExtendedRange = (LambdaBarDataCountincl[0][4]) / (tempINClambareffRAPGAPExtendedRange);
	double INCASYMRAPGAPExtendedRange = (tempINClamregRAPGAPExtendedRange - tempINClambarRAPGAPExtendedRange) / ((tempINClamregRAPGAPExtendedRange + tempINClambarRAPGAPExtendedRange));
	double INCextractionrangeDirectDiff = abs(INCASYMRAPGAPExtendedRange - INCASYMRAPGAP);
	cout << "Extended Siganl Extraction Range" << endl;
	cout << "Asymmetry: " << INCASYMRAPGAPExtendedRange << endl;
	cout << "Difference in Asymmetry: " << INCextractionrangeDirectDiff << endl;
	cout << " " << endl;

	//RAW EXTRACTION CALCULATION
	double tempINClamregeffRAPGAPRawExtraction = (LambdaRegMCRAPGAPCountincl[0][5]) / (LambdaRegGenCountinclRAPGAP[0]);
	double tempINClambareffRAPGAPRawExtraction = (LambdaBarMCRAPGAPCountincl[0][5]) / (LambdaBarGenCountinclRAPGAP[0]);
	double tempINClamregRAPGAPRawExtraction = (LambdaRegDataCountincl[0][5]) / (tempINClamregeffRAPGAPRawExtraction);
	double tempINClambarRAPGAPRawExtraction = (LambdaBarDataCountincl[0][5]) / (tempINClambareffRAPGAPRawExtraction);
	double INCASYMRAPGAPRawExtraction = (tempINClamregRAPGAPRawExtraction - tempINClambarRAPGAPRawExtraction) / ((tempINClamregRAPGAPRawExtraction + tempINClambarRAPGAPRawExtraction));
	double INCrawextractionDirectDiff = abs(INCASYMRAPGAPRawExtraction - INCASYMRAPGAP);
	cout << "Raw Extraction" << endl;
	cout << "Asymmetry: " << INCASYMRAPGAPRawExtraction << endl;
	cout << "Difference in Asymmetry: " << INCrawextractionDirectDiff << endl;
	cout << " " << endl;

	//DJANGOH vs RAPGAP
	double INCMCerrorDirectDiff = abs(INCASYMDJANGOH - INCASYMRAPGAP);
	cout << "RAPGAP vs DJANGOH for Detector Efficiencies" << endl;
	cout << "Asymmetry: " << INCASYMDJANGOH << endl;
	cout << "Difference in Asymmetry: " << INCMCerrorDirectDiff << endl;
	cout << " " << endl;

	//MC NUCLEAR INTERACTION ESTIMATION
	//double AnalyzeBGTree(int distvariableindex, TFile* Data, string TreeName, double lowerbinvalue, double upperbinvalue, double s1, double s2) {
	double INCpercentRAPGAPcontam = AnalyzeBGTree(varidx, new TFile(filelambdaTreeMCRAPGAP.c_str(), "read"), "Lambda Reg BG", q2bins[0], q2bins[q2entries], LambdaRegMCRAPGAPCountincl[0][8], LambdaRegMCRAPGAPCountincl[0][9]) / AnalyzeBGTree(varidx, new TFile(filelambdaTreeMCRAPGAP.c_str(), "read"), "Lambda Reg Gen", q2bins[0], q2bins[q2entries], LambdaRegMCRAPGAPCountincl[0][8], LambdaRegMCRAPGAPCountincl[0][9]);
	double INCpercentDJANGOHcontam = AnalyzeBGTree(varidx, new TFile(filelambdaTreeMCDJANGOH.c_str(), "read"), "Lambda Reg BG", q2bins[0], q2bins[q2entries], LambdaRegMCDJANGOHCountincl[0][8], LambdaRegMCDJANGOHCountincl[0][9]) / AnalyzeBGTree(varidx, new TFile(filelambdaTreeMCDJANGOH.c_str(), "read"), "Lambda Reg Gen", q2bins[0], q2bins[q2entries], LambdaRegMCDJANGOHCountincl[0][8], LambdaRegMCDJANGOHCountincl[0][9]);
	double INCaveragepercentBGcontam = (INCpercentRAPGAPcontam + INCpercentDJANGOHcontam) / 2;
	//double INCdetectorinterror = GetAsymError(tempINClamregRAPGAP, abs(tempINClamregRAPGAP - (LambdaRegDataCountincl[0][0] + LambdaRegDataCountincl[0][0] * INCaveragepercentBGcontam) * (LambdaRegGenCountinclRAPGAP[0]) / LambdaRegMCRAPGAPCountincl[0][0]), tempINClambarRAPGAP, 0.0);
	//double INCAsymmerrSYS = sqrt(Square(INCMCerr) + Square(INCMCSignalRooFitYieldError) + Square(INCextractionrange) + Square(INCrawextraction) + Square(INCalternativeBGfit) + Square(INCdetectorinterror));
	double tempINClamregeffRAPGAPNuclearInt = (LambdaRegMCRAPGAPCountincl[0][0]) / (LambdaRegGenCountinclRAPGAP[0]);
	double tempINClambareffRAPGAPNuclearInt = (LambdaBarMCRAPGAPCountincl[0][0]) / (LambdaBarGenCountinclRAPGAP[0]);
	double tempINClamregRAPGAPNuclearInt = (LambdaRegDataCountincl[0][0]+ LambdaRegDataCountincl[0][0]*(INCaveragepercentBGcontam)) / (tempINClamregeffRAPGAPNuclearInt);
	double tempINClambarRAPGAPNuclearInt = (LambdaBarDataCountincl[0][0]) / (tempINClambareffRAPGAPNuclearInt);
	double INCASYMRAPGAPNuclearInt = (tempINClamregRAPGAPNuclearInt - tempINClambarRAPGAPNuclearInt) / ((tempINClamregRAPGAPNuclearInt + tempINClambarRAPGAPNuclearInt));
	double INCdetectorinterrorDirectDiff = abs(INCASYMRAPGAPNuclearInt - INCASYMRAPGAP);
	cout << "MC Estimation of Nuclear Interactions" << endl;
	cout << "Percent of Nuclear Interaction Contamination (Average of RAPGAP and DJANGOH): " << INCaveragepercentBGcontam << endl;
	cout << "Asymmetry: " << INCASYMRAPGAPNuclearInt << endl;
	cout << "Difference in Asymmetry: " << INCdetectorinterrorDirectDiff << endl;
	cout << " " << endl;

	//Systematic Error Contributions
	cout << "Systematic Error Contributions: " << endl;
	cout << "RAPGAP vs DJANGOH for Detector Efficiencies: " << INCMCerrorDirectDiff << endl;
	cout << "MC Estimation of Nuclear Interactions: " << INCdetectorinterrorDirectDiff << endl;
	cout << "Alternate BG Fit: " << INCalternativeBGfitDirectDiff << endl;
	cout << "Raw Extraction: " << INCextractionrangeDirectDiff << endl;
	cout << "Extended Siganl Extraction Range: " << INCextractionrangeDirectDiff << endl;
	cout << " " << endl;
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


	//double INCMCerr = GetAsymError(tempINClamregRAPGAP, abs(tempINClamregRAPGAP - tempINClamregDJANGOH), tempINClambarRAPGAP, abs(tempINClambarRAPGAP - tempINClambarDJANGOH));
	//double err1 = 2 * LambdaBarDataCountincl[0][0] * tempINClambareffRAPGAP * LambdaRegDataCountincl[0][0] / (Square(LambdaBarDataCountincl[0][0] * tempINClamregeffRAPGAP + tempINClambareffRAPGAP * LambdaRegDataCountincl[0][0]));
	//double err2 = 2 * LambdaBarDataCountincl[0][0] * tempINClamregeffRAPGAP * LambdaRegDataCountincl[0][0] / (Square(LambdaBarDataCountincl[0][0] * tempINClamregeffRAPGAP + tempINClambareffRAPGAP * LambdaRegDataCountincl[0][0]));
	//double INCMCerr = sqrt(Square((2* LambdaBarDataCountincl[0][0]* tempINClambareffRAPGAP* LambdaRegDataCountincl[0][0]/Square(LambdaBarDataCountincl[0][0]* tempINClamregeffRAPGAP + tempINClambareffRAPGAP* LambdaRegDataCountincl[0][0]))*(0.5*abs(tempINClamregeffDJANGOH- tempINClamregeffRAPGAP)))+Square((2 * LambdaBarDataCountincl[0][0] * tempINClambareffRAPGAP * LambdaRegDataCountincl[0][0] / Square(LambdaBarDataCountincl[0][0] * tempINClamregeffRAPGAP + tempINClambareffRAPGAP * LambdaRegDataCountincl[0][0])) *(0.5*abs(tempINClambareffDJANGOH- tempINClambareffRAPGAP))));
	//double INCMCerr = GetAsymError(tempINClamregRAPGAP, GetRatioError((tempINClamregRAPGAP),LambdaRegDataCountincl[0][0], 0.0, tempINClamregeffRAPGAP, 0.5*abs(tempINClamregeffDJANGOH- tempINClamregeffRAPGAP)/(tempINClamregeffRAPGAP)), tempINClambarRAPGAP, GetRatioError((tempINClambarRAPGAP), LambdaBarDataCountincl[0][0], 0.0, tempINClambareffRAPGAP, 0.5*abs(tempINClambareffDJANGOH - tempINClambareffRAPGAP) / (tempINClambareffRAPGAP)));
	//double INCMCerr = sqrt(Square(err1 * 0.5 * abs(tempINClamregeffDJANGOH - tempINClamregeffRAPGAP)) + Square(err2 * 0.5 * abs(tempINClambareffDJANGOH - tempINClambareffRAPGAP)));
	//double INCMCerr2 = GetAsymError((LambdaRegDataCountincl[0][0]) / (tempINClamregeffRAPGAP), 0.5*abs((LambdaRegDataCountincl[0][0]) / (tempINClamregeffRAPGAP)-(LambdaRegDataCountincl[0][0]) / (tempINClamregeffDJANGOH)), (LambdaBarDataCountincl[0][0]) / (tempINClambareffRAPGAP),0.5*abs((LambdaBarDataCountincl[0][0]) / (tempINClambareffRAPGAP)-(LambdaBarDataCountincl[0][0]) / (tempINClambareffDJANGOH)));
	double INCMCSignalRooFitYieldError = GetAsymError(tempINClamregRAPGAP, GetRatioError(tempINClamregRAPGAP, LambdaRegDataCountincl[0][0], LambdaRegDataCountincl[0][2], (LambdaRegMCRAPGAPCountincl[0][0]) / (LambdaRegGenCountinclRAPGAP[0]), (LambdaRegMCRAPGAPCountincl[0][2]) / (LambdaRegGenCountinclRAPGAP[0])), tempINClambarRAPGAP, GetRatioError(tempINClambarRAPGAP, LambdaBarDataCountincl[0][0], LambdaBarDataCountincl[0][2], (LambdaBarMCRAPGAPCountincl[0][0]) / (LambdaBarGenCountinclRAPGAP[0]), (LambdaBarMCRAPGAPCountincl[0][2]) / (LambdaBarGenCountinclRAPGAP[0])));
	//double INCextractionrange = GetAsymError(tempINClamregRAPGAP, abs(tempINClamregRAPGAP - (LambdaRegDataCountincl[0][4]) / ((LambdaRegMCRAPGAPCountincl[0][4]) / (LambdaRegGenCountinclRAPGAP[0]))), tempINClambarRAPGAP, abs(tempINClambarRAPGAP - (LambdaBarDataCountincl[0][4]) / ((LambdaBarMCRAPGAPCountincl[0][4]) / (LambdaBarGenCountinclRAPGAP[0]))));
	//double INCrawextraction = GetAsymError(tempINClamregRAPGAP, abs(tempINClamregRAPGAP - (LambdaRegDataCountincl[0][5]) / ((LambdaRegMCRAPGAPCountincl[0][5]) / (LambdaRegGenCountinclRAPGAP[0]))), tempINClambarRAPGAP, abs(tempINClambarRAPGAP - (LambdaBarDataCountincl[0][5]) / ((LambdaBarMCRAPGAPCountincl[0][5]) / (LambdaBarGenCountinclRAPGAP[0]))));
	//double INCalternativeBGfit = GetAsymError(tempINClamregRAPGAP, abs(tempINClamregRAPGAP - (LambdaRegDataCountincl[0][6]) / ((LambdaRegMCRAPGAPCountincl[0][6]) / (LambdaRegGenCountinclRAPGAP[0]))), tempINClambarRAPGAP, abs(tempINClambarRAPGAP - (LambdaBarDataCountincl[0][6]) / ((LambdaBarMCRAPGAPCountincl[0][6]) / (LambdaBarGenCountinclRAPGAP[0]))));
	
	double INCAsymmerrSYS = sqrt(Square(INCMCerrorDirectDiff) + Square(INCextractionrangeDirectDiff) + Square(INCrawextractionDirectDiff) + Square(INCalternativeBGfitDirectDiff) + Square(INCdetectorinterrorDirectDiff));

	double IntTotErr = sqrt(Square(INCAsymmerrSYS) + Square(INCASYMstaterrRAPGAP));

	cout << "DJANGOH Asymmetry: " << INCASYMDJANGOH << " +/- " << INCASYMstaterrDJANGOH << " (stat.)" << endl;
	cout << "RAPGAP Asymmetry: " << INCASYMRAPGAP << " +/- " << INCMCSignalRooFitYieldError << " (stat.)" << endl;
	cout << "RAPGAP Total Lambda Reg Efficiency: " << tempINClamregeffRAPGAP << endl;
	cout << "RAPGAP Total Lambda Bar Efficiency: " << tempINClambareffRAPGAP << endl;
	cout << "DJANGOH Total Lambda Reg Efficiency: " << tempINClamregeffDJANGOH << endl;
	cout << "DJANGOH Total Lambda Bar Efficiency: " << tempINClambareffDJANGOH << endl;
	cout << "Integrated Asymmetry: " << INCASYMRAPGAP << " +/- " << INCASYMstaterrRAPGAP << "(stat.) +/- " << INCAsymmerrSYS << " (sys.), +/- " << IntTotErr << " (tot.)" << endl;
	cout << "Systematic Error Contributions: " << endl;
	//cout << "RAPGAP vs DJANGOH: " << INCMCerr << endl;
	//cout << "RAPGAP vs DJANGOH (Method 2): " << INCMCerr2 << endl;
	cout << "Propogated RooFit Parameter Error: " << INCMCSignalRooFitYieldError << endl;
	//cout << "Extended Signal Extraction Range: " << INCextractionrange << endl;
	//cout << "Raw Signal Extraction: " << INCrawextraction << endl;
	//cout << "Alternative BG Fit: " << INCalternativeBGfit << endl;
	//cout << "MC Nuclear Interactions: " << INCdetectorinterror << endl;
	cout << "Standard Statistical Error: " << INCASYMstaterrRAPGAP << endl;

	outputfile.Write();
	outputfile.Close();

	cout << "Done" << endl;

};
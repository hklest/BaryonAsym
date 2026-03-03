
class TDataPreparation{

	private:

		TTree* fDataTree;

	public:

		TDataPreparation(){

			fDataTree = new TTree("datatree", "datatree");

			Int_t nlines = fDataTree->ReadFile("pi0_bkg.dat","mass:counts");
		}

		TTree* getDataTree(){return fDataTree;}
};


void mlp4Gage(){


	bool check_mlp_fit = false;

	TFile* f = new TFile("pi0_pp200.root"); 				//read in your data file
	TH1F* inHist = (TH1F*) f->Get("fg2d_ggmass_sameArm");   //read in the histogram of interest

	inHist->AddDirectory(0); 

	TH1F* checkHist = (TH1F*) inHist->Clone();	

	ofstream ofl("pi0_bkg.dat");							//define your output text file; make sure its the same name as the one used for data preparation above


	// Read in the bin center and bin content from the histogram and prepare the text file

	for(int i = 1; i < (int)inHist->GetNbinsX()+1; i++){
		
		double bcx = inHist->GetBinCenter(i);

   		if(isinf(inHist->GetBinContent(i))) inHist->SetBinContent(i,0);

   		if((bcx > 1.08 && bcx < 1.095) || (bcx > 1.135 && bcx < 1.16))  
   			ofl << bcx << "\t" << inHist->GetBinContent(i) << endl;
		
	}

	f->Close();


	// Data preparation for MLP input

	TDataPreparation myFeedingData;
	TTree* T = myFeedingData.getDataTree();


	TMultiLayerPerceptron* mlp = new TMultiLayerPerceptron("mass:100:counts", T); //Defining the architecture - 1 hidden layer with 100 nodes
	mlp->Train(1000,"text, graph, update=100");


	if(check_mlp_fit){

		Int_t nEvent = checkHist->GetEntries();

		Double_t* mass = new Double_t [nEvent];
		Double_t* counts = new Double_t [nEvent];
		Double_t* counts_cal = new Double_t [nEvent];

		fstream readIn;
		readIn.open("pi0_bkg.dat", ios::in);

		checkHist->Reset("ICESM");

		for(int i = 0; i < nEvent; i++){
			readIn >> mass[i] >> counts[i];
			checkHist->SetBinContent(checkHist->GetXaxis()->FindBin(mass[i]), counts[i]);

		}

		readIn.close();



		TH2F* outHist = (TH2F*) checkHist->Clone();
		outHist->Reset("ICESM");

		Double_t inputs[1];
		for(int i = 0; i < nEvent; i++){

			inputs[0] = mass[i];

			counts_cal[i] = mlp->Evaluate(0, inputs);

			outHist->SetBinContent(outHist->GetXaxis()->FindBin(mass[i]), counts_cal[i]);

		}


		TCanvas* c1 = new TCanvas("c1", "c1", 500, 400);
		c1->Divide(2,1);

		c1->cd(1);
		inHist->Draw("AP");

		c1->cd(2);
		outHist->Draw("AP");


	}

	
	//Extracting the signal

	cout << "Now analyzing" << endl;

	TFile* f1 = new TFile("pi0_pp200.root");
	TH2F* fgHist = (TH2F*) f1->Get("fg2d_ggmass_sameArm");

	fgHist->AddDirectory(0);

	Double_t inputs[1];

	TH2F* signalHist = (TH2F*) fgHist->Clone();
	signalHist->Reset("ICESM");

	cout << "Getting to counting" << endl;
	for(int i = 1; i < fgHist->GetNbinsX()+1; i++){

		inputs[0] = ((TAxis*)fgHist->GetXaxis())->GetBinCenter(i);

		if(inputs[0] < 1.08 || inputs[0] > 1.16 ) continue;

		double temp_content = fgHist->GetBinContent(i) - mlp->Evaluate(0, inputs);
		signalHist->SetBinContent(i, temp_content);
		signalHist->SetBinError(i, fgHist->GetBinError(i));
   		
   			
	}

	TCanvas* c2 = new TCanvas("c2", "c2", 400, 400);
	c2->cd();
	signalHist->Draw("COLZ");



}
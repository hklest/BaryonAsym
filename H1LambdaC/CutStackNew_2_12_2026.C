//Analysis Code Root Macro 
//Importing TTrees
#include <iostream>
#include <stdlib.h>
#include <cmath>
using namespace std;

////////////////////////////////////////////////
// User Inputs in this section
////////////////////////////////////////////////

string filelambdaTree = "/data/dust/group/h1/klesthen/BaryonAsym/H1LambdaC/LambdaTree/merged.root";
string filelambdaRAPGAPTree = "/data/dust/group/h1/klesthen/BaryonAsym/H1LambdaC_MC/LambdaTreeMC/RAPGAP31/merged.root";
string filelambdaDJANGOHTree = "/data/dust/group/h1/klesthen/BaryonAsym/H1LambdaC_MC/LambdaTreeMC/DJANGOH14/merged.root";

string filelambdaTreeOutputFileName = "/data/dust/group/h1/klesthen/BaryonAsym/H1LambdaC/LambdaTree/CutsApplied/merged.root";
string filelambdaRAPGAPTreeOutputFileName = "/data/dust/group/h1/klesthen/BaryonAsym/H1LambdaC_MC/LambdaTreeMC/RAPGAP31/CutsApplied/merged.root";
string filelambdaDJANGOHTreeOutputOutputFileName = "/data/dust/group/h1/klesthen/BaryonAsym/H1LambdaC_MC/LambdaTreeMC/DJANGOH14/CutsApplied/merged.root";

string outputfilename = "/data/dust/group/h1/klesthen/BaryonAsym/H1LambdaC/CutStackNew.root";


// Histograms Binning
Double_t lambdabins = 1000;
Double_t lambdaupperbinrange = 1.5;
Double_t lambdalowerbinrange = 1.0;

Double_t lambdaCbins = 50;
Double_t lambdaCupperbinrange = 3.317;
Double_t lambdaClowerbinrange = 2.017;

Double_t kaonbins = 500;
Double_t kaonupperbinrange = 1.0;
Double_t kaonlowerbinrange = 0.0;






void CutStackNew() {

	//Canvas
	TCanvas* canvas = new TCanvas("ODS", "Plot", 10, 10, 800, 800);
	canvas->SetLeftMargin(0.15);
	
	//Files
	TFile* DataLambda = new TFile(filelambdaTree.c_str());
	TFile* RAPGAPLambda = new TFile(filelambdaRAPGAPTree.c_str());
	TFile* DJANGOHLambda = new TFile(filelambdaDJANGOHTree.c_str());
	//Getting Trees from Files
	TTree* LambdaRegDataTree = (TTree*)DataLambda->Get("Lambda Reg");
	TTree* LambdaBarDataTree = (TTree*)DataLambda->Get("Lambda Bar");
	TTree* LambdaRegRAPGAPTree = (TTree*)RAPGAPLambda->Get("Lambda Reg");
	TTree* LambdaBarRAPGAPTree = (TTree*)RAPGAPLambda->Get("Lambda Bar");
	TTree* LambdaRegDJANGOHTree = (TTree*)DJANGOHLambda->Get("Lambda Reg");
	TTree* LambdaBarDJANGOHTree = (TTree*)DJANGOHLambda->Get("Lambda Bar");

	//Creating Histograms
	TH1F* LambdaRegDataCutProgression[16];
	TH1F* LambdaBarDataCutProgression[16];
	TH1F* LambdaRegRAPGAPCutProgression[16];
	TH1F* LambdaBarRAPGAPCutProgression[16];
	TH1F* LambdaRegDJANGOHCutProgression[16];
	TH1F* LambdaBarDJANGOHCutProgression[16];
	int lw = 2; //Line width for hist
	for (int a = 0; a < 16; a++) {
		string lambdaREGDatacutprogstring = "Lambda Reg Data Cut " + to_string(a);
		string lambdaBARDatacutprogstring = "Lambda Bar Data Cut " + to_string(a);
		string lambdaREGRAPGAPcutprogstring = "Lambda Reg RAPGAP Cut " + to_string(a);
		string lambdaBARRAPGAPcutprogstring = "Lambda Bar RAPGAP Cut " + to_string(a);
		string lambdaREGDJANGOHcutprogstring = "Lambda Reg DJANGOH Cut " + to_string(a);
		string lambdaBARDJANGOHcutprogstring = "Lambda Bar DJANGOH Cut " + to_string(a);
		LambdaRegDataCutProgression[a] = new TH1F(lambdaREGDatacutprogstring.c_str(), lambdaREGDatacutprogstring.c_str(), lambdabins, lambdalowerbinrange, lambdaupperbinrange);
		LambdaBarDataCutProgression[a] = new TH1F(lambdaBARDatacutprogstring.c_str(), lambdaBARDatacutprogstring.c_str(), lambdabins, lambdalowerbinrange, lambdaupperbinrange);
		LambdaRegRAPGAPCutProgression[a] = new TH1F(lambdaREGRAPGAPcutprogstring.c_str(), lambdaREGRAPGAPcutprogstring.c_str(), lambdabins, lambdalowerbinrange, lambdaupperbinrange);
		LambdaBarRAPGAPCutProgression[a] = new TH1F(lambdaBARRAPGAPcutprogstring.c_str(), lambdaBARRAPGAPcutprogstring.c_str(), lambdabins, lambdalowerbinrange, lambdaupperbinrange);
		LambdaRegDJANGOHCutProgression[a] = new TH1F(lambdaREGDJANGOHcutprogstring.c_str(), lambdaREGDJANGOHcutprogstring.c_str(), lambdabins, lambdalowerbinrange, lambdaupperbinrange);
		LambdaBarDJANGOHCutProgression[a] = new TH1F(lambdaBARDJANGOHcutprogstring.c_str(), lambdaBARDJANGOHcutprogstring.c_str(), lambdabins, lambdalowerbinrange, lambdaupperbinrange);
		
		LambdaRegDataCutProgression[a]->SetLineWidth(lw);
		LambdaBarDataCutProgression[a]->SetLineWidth(lw);
		LambdaRegRAPGAPCutProgression[a]->SetLineWidth(lw);
		LambdaBarRAPGAPCutProgression[a]->SetLineWidth(lw);
		LambdaRegDJANGOHCutProgression[a]->SetLineWidth(lw);
		LambdaBarDJANGOHCutProgression[a]->SetLineWidth(lw);

	}
	
	
	//creating file to output histograms to
	//TFile outputfile(outputfilename.c_str(), "RECREATE");
	//TFile outputfileLambdaCutsAppliedTree(filelambdaTreeOutputFileName.c_str(), "RECREATE");
	//TFile dummyfile("/data/dust/group/h1/klesthen/BaryonAsym/H1LambdaC/dummyfile.root", "RECREATE");

	//Creating Tree to save Lambda Candidates to that pass all of our cuts
	//TTree* lambdaRegTree = new TTree("Lambda Reg", "Lambda Reg");

	//Defining Tree Variables and Branches
	//defining the variables I want to store in Lambda Trees, will save the TTrees and access branches later in a separate analysis code
	double pT, Eta, x, invMasslambda, Q2s, Ys, Angle, v0chi2, weight1, weight2, SVx, SVy, SVz, PVx, PVy, PVz, LambdaPX, LambdaPY, LambdaPZ, ystar, ylab, scatEX, scatEY, scatEZ;
	double PionPt, ProtonPt, PionTrackStartRadius, ProtonTrackStartRadius, PionL, PionLProton, PionLelectron, ProtonL, ProtonLPion, ProtonLelectron, PionCharge, ProtonCharge, PionTrackLength, ProtonTrackLength, CosThetaStar;
	double PionNHitsdEdx, ProtonNHitsdEdx, PiondEdx, ProtondEdx, invMasskaonCont, invMassphotonCont;
	double PionDCA, ProtonDCA, PionSDCA, ProtonSDCA, Chi2, MassPionPion, MassElecElec, pTrelPion, pTrelProton, RadiusSVtoPV, DecayLength, DeltaDCA;
	double genIvxX, genIvxY, genIvxZ, decayrecX, decayrecY, decayrecZ, strIVtype, pionFRUNZ, protonFRUNZ, Ws, Phi;
	double pTgen, Etagen, xgen, Q2sgen, Ysgen, SVxgen, SVygen, SVzgen, PVzgen, Phigen, genrecAngle;
	int IsBar, FoundGen, s61, s74, s1, s75, s67;
	Bool_t fRunType;
	double PionNuclCorrWeight;
	double ProtonTrackEndRadius, PionTrackEndRadius;
	//TVector3  ProtonTrackEndMomentum, ProtonTrackV0Momentum, PionTrackEndMomentum, PionTrackV0Momentum;
	TVector3* PionTrackV0Momentum = nullptr;
	TVector3* ProtonTrackV0Momentum = nullptr;
	TVector3* PionTrackEndMomentum = nullptr;
	TVector3* ProtonTrackEndMomentum = nullptr;
	//Defining Tree Branches for Combined Lambdas, and seperate ones for Regular Lambdas vs Lambda Bars
	LambdaRegDataTree->SetBranchAddress("invMasslambda", &invMasslambda);
	LambdaRegDataTree->SetBranchAddress("invMasskaonCont", &invMasskaonCont);
	LambdaRegDataTree->SetBranchAddress("invMassphotonCont", &invMassphotonCont);
	LambdaRegDataTree->SetBranchAddress("pT", &pT);
	LambdaRegDataTree->SetBranchAddress("Eta", &Eta);
	LambdaRegDataTree->SetBranchAddress("Phi", &Phi);
	LambdaRegDataTree->SetBranchAddress("x", &x);
	LambdaRegDataTree->SetBranchAddress("Q2s", &Q2s);
	LambdaRegDataTree->SetBranchAddress("Ys", &Ys);
	LambdaRegDataTree->SetBranchAddress("Angle", &Angle);
	LambdaRegDataTree->SetBranchAddress("v0chi2", &v0chi2);
	LambdaRegDataTree->SetBranchAddress("weight1", &weight1);
	LambdaRegDataTree->SetBranchAddress("weight2", &weight2);
	LambdaRegDataTree->SetBranchAddress("SVx", &SVx);
	LambdaRegDataTree->SetBranchAddress("SVy", &SVy);
	LambdaRegDataTree->SetBranchAddress("SVz", &SVz);
	LambdaRegDataTree->SetBranchAddress("PVx", &PVx);
	LambdaRegDataTree->SetBranchAddress("PVy", &PVy);
	LambdaRegDataTree->SetBranchAddress("PVz", &PVz);
	LambdaRegDataTree->SetBranchAddress("LambdaPX", &LambdaPX);
	LambdaRegDataTree->SetBranchAddress("LambdaPY", &LambdaPY);
	LambdaRegDataTree->SetBranchAddress("LambdaPZ", &LambdaPZ);
	LambdaRegDataTree->SetBranchAddress("fRunType", &fRunType);
	LambdaRegDataTree->SetBranchAddress("CosThetaStar", &CosThetaStar);
	LambdaRegDataTree->SetBranchAddress("ystar", &ystar);
	LambdaRegDataTree->SetBranchAddress("ylab", &ylab);
	LambdaRegDataTree->SetBranchAddress("scatEX", &scatEX);
	LambdaRegDataTree->SetBranchAddress("scatEY", &scatEY);
	LambdaRegDataTree->SetBranchAddress("scatEZ", &scatEZ);
	LambdaRegDataTree->SetBranchAddress("FoundGen", &FoundGen);
	LambdaRegDataTree->SetBranchAddress("s61", &s61);
	LambdaRegDataTree->SetBranchAddress("s1", &s1);
	LambdaRegDataTree->SetBranchAddress("s74", &s74);
	LambdaRegDataTree->SetBranchAddress("s75", &s75);
	LambdaRegDataTree->SetBranchAddress("s67", &s67);
	LambdaRegDataTree->SetBranchAddress("PionCharge", &PionCharge);
	LambdaRegDataTree->SetBranchAddress("ProtonCharge", &ProtonCharge);
	LambdaRegDataTree->SetBranchAddress("pionFRUNZ", &pionFRUNZ);
	LambdaRegDataTree->SetBranchAddress("protonFRUNZ", &protonFRUNZ);
	LambdaRegDataTree->SetBranchAddress("PionPt", &PionPt);
	LambdaRegDataTree->SetBranchAddress("ProtonPt", &ProtonPt);
	LambdaRegDataTree->SetBranchAddress("PionTrackStartRadius", &PionTrackStartRadius);
	LambdaRegDataTree->SetBranchAddress("ProtonTrackStartRadius", &ProtonTrackStartRadius);
	LambdaRegDataTree->SetBranchAddress("PionNHitsdEdx", &PionNHitsdEdx);
	LambdaRegDataTree->SetBranchAddress("ProtonNHitsdEdx", &ProtonNHitsdEdx);
	LambdaRegDataTree->SetBranchAddress("PiondEdx", &PiondEdx);
	LambdaRegDataTree->SetBranchAddress("ProtondEdx", &ProtondEdx);
	LambdaRegDataTree->SetBranchAddress("PionL", &PionL);
	LambdaRegDataTree->SetBranchAddress("PionLProton", &PionLProton);
	LambdaRegDataTree->SetBranchAddress("PionLelectron", &PionLelectron);
	LambdaRegDataTree->SetBranchAddress("ProtonL", &ProtonL);
	LambdaRegDataTree->SetBranchAddress("ProtonLPion", &ProtonLPion);
	LambdaRegDataTree->SetBranchAddress("ProtonLelectron", &ProtonLelectron);
	LambdaRegDataTree->SetBranchAddress("PionTrackLength", &PionTrackLength);
	LambdaRegDataTree->SetBranchAddress("ProtonTrackLength", &ProtonTrackLength);
	LambdaRegDataTree->SetBranchAddress("PionDCA", &PionDCA);
	LambdaRegDataTree->SetBranchAddress("ProtonDCA", &ProtonDCA);
	LambdaRegDataTree->SetBranchAddress("PionSDCA", &PionSDCA);
	LambdaRegDataTree->SetBranchAddress("ProtonSDCA", &ProtonSDCA);
	LambdaRegDataTree->SetBranchAddress("Chi2", &Chi2);
	LambdaRegDataTree->SetBranchAddress("MassPionPion", &MassPionPion);
	LambdaRegDataTree->SetBranchAddress("MassElecElec", &MassElecElec);
	LambdaRegDataTree->SetBranchAddress("pTrelPion", &pTrelPion);
	LambdaRegDataTree->SetBranchAddress("pTrelProton", &pTrelProton);
	LambdaRegDataTree->SetBranchAddress("RadiusSVtoPV", &RadiusSVtoPV);
	LambdaRegDataTree->SetBranchAddress("IsBar", &IsBar);
	LambdaRegDataTree->SetBranchAddress("PionNuclCorrWeight", &PionNuclCorrWeight);
	LambdaRegDataTree->SetBranchAddress("ProtonTrackEndRadius", &ProtonTrackEndRadius);
	LambdaRegDataTree->SetBranchAddress("ProtonTrackEndMomentum", &ProtonTrackEndMomentum);
	LambdaRegDataTree->SetBranchAddress("PionTrackEndRadius", &PionTrackEndRadius);
	LambdaRegDataTree->SetBranchAddress("PionTrackEndMomentum", &PionTrackEndMomentum);
	LambdaRegDataTree->SetBranchAddress("ProtonTrackV0Momentum", &ProtonTrackV0Momentum);
	LambdaRegDataTree->SetBranchAddress("PionTrackV0Momentum", &PionTrackV0Momentum);
	LambdaRegDataTree->SetBranchAddress("DecayLength", &DecayLength);
	LambdaRegDataTree->SetBranchAddress("DeltaDCA", &DeltaDCA);

	LambdaBarDataTree->SetBranchAddress("invMasslambda", &invMasslambda);
	LambdaBarDataTree->SetBranchAddress("invMasskaonCont", &invMasskaonCont);
	LambdaBarDataTree->SetBranchAddress("invMassphotonCont", &invMassphotonCont);
	LambdaBarDataTree->SetBranchAddress("pT", &pT);
	LambdaBarDataTree->SetBranchAddress("Eta", &Eta);
	LambdaBarDataTree->SetBranchAddress("Phi", &Phi);
	LambdaBarDataTree->SetBranchAddress("x", &x);
	LambdaBarDataTree->SetBranchAddress("Q2s", &Q2s);
	LambdaBarDataTree->SetBranchAddress("Ys", &Ys);
	LambdaBarDataTree->SetBranchAddress("Angle", &Angle);
	LambdaBarDataTree->SetBranchAddress("v0chi2", &v0chi2);
	LambdaBarDataTree->SetBranchAddress("weight1", &weight1);
	LambdaBarDataTree->SetBranchAddress("weight2", &weight2);
	LambdaBarDataTree->SetBranchAddress("SVx", &SVx);
	LambdaBarDataTree->SetBranchAddress("SVy", &SVy);
	LambdaBarDataTree->SetBranchAddress("SVz", &SVz);
	LambdaBarDataTree->SetBranchAddress("PVx", &PVx);
	LambdaBarDataTree->SetBranchAddress("PVy", &PVy);
	LambdaBarDataTree->SetBranchAddress("PVz", &PVz);
	LambdaBarDataTree->SetBranchAddress("LambdaPX", &LambdaPX);
	LambdaBarDataTree->SetBranchAddress("LambdaPY", &LambdaPY);
	LambdaBarDataTree->SetBranchAddress("LambdaPZ", &LambdaPZ);
	LambdaBarDataTree->SetBranchAddress("fRunType", &fRunType);
	LambdaBarDataTree->SetBranchAddress("CosThetaStar", &CosThetaStar);
	LambdaBarDataTree->SetBranchAddress("ystar", &ystar);
	LambdaBarDataTree->SetBranchAddress("ylab", &ylab);
	LambdaBarDataTree->SetBranchAddress("scatEX", &scatEX);
	LambdaBarDataTree->SetBranchAddress("scatEY", &scatEY);
	LambdaBarDataTree->SetBranchAddress("scatEZ", &scatEZ);
	LambdaBarDataTree->SetBranchAddress("FoundGen", &FoundGen);
	LambdaBarDataTree->SetBranchAddress("s61", &s61);
	LambdaBarDataTree->SetBranchAddress("s1", &s1);
	LambdaBarDataTree->SetBranchAddress("s74", &s74);
	LambdaBarDataTree->SetBranchAddress("s75", &s75);
	LambdaBarDataTree->SetBranchAddress("s67", &s67);
	LambdaBarDataTree->SetBranchAddress("PionCharge", &PionCharge);
	LambdaBarDataTree->SetBranchAddress("ProtonCharge", &ProtonCharge);
	LambdaBarDataTree->SetBranchAddress("pionFRUNZ", &pionFRUNZ);
	LambdaBarDataTree->SetBranchAddress("protonFRUNZ", &protonFRUNZ);
	LambdaBarDataTree->SetBranchAddress("PionPt", &PionPt);
	LambdaBarDataTree->SetBranchAddress("ProtonPt", &ProtonPt);
	LambdaBarDataTree->SetBranchAddress("PionTrackStartRadius", &PionTrackStartRadius);
	LambdaBarDataTree->SetBranchAddress("ProtonTrackStartRadius", &ProtonTrackStartRadius);
	LambdaBarDataTree->SetBranchAddress("PionNHitsdEdx", &PionNHitsdEdx);
	LambdaBarDataTree->SetBranchAddress("ProtonNHitsdEdx", &ProtonNHitsdEdx);
	LambdaBarDataTree->SetBranchAddress("PiondEdx", &PiondEdx);
	LambdaBarDataTree->SetBranchAddress("ProtondEdx", &ProtondEdx);
	LambdaBarDataTree->SetBranchAddress("PionL", &PionL);
	LambdaBarDataTree->SetBranchAddress("PionLProton", &PionLProton);
	LambdaBarDataTree->SetBranchAddress("PionLelectron", &PionLelectron);
	LambdaBarDataTree->SetBranchAddress("ProtonL", &ProtonL);
	LambdaBarDataTree->SetBranchAddress("ProtonLPion", &ProtonLPion);
	LambdaBarDataTree->SetBranchAddress("ProtonLelectron", &ProtonLelectron);
	LambdaBarDataTree->SetBranchAddress("PionTrackLength", &PionTrackLength);
	LambdaBarDataTree->SetBranchAddress("ProtonTrackLength", &ProtonTrackLength);
	LambdaBarDataTree->SetBranchAddress("PionDCA", &PionDCA);
	LambdaBarDataTree->SetBranchAddress("ProtonDCA", &ProtonDCA);
	LambdaBarDataTree->SetBranchAddress("PionSDCA", &PionSDCA);
	LambdaBarDataTree->SetBranchAddress("ProtonSDCA", &ProtonSDCA);
	LambdaBarDataTree->SetBranchAddress("Chi2", &Chi2);
	LambdaBarDataTree->SetBranchAddress("MassPionPion", &MassPionPion);
	LambdaBarDataTree->SetBranchAddress("MassElecElec", &MassElecElec);
	LambdaBarDataTree->SetBranchAddress("pTrelPion", &pTrelPion);
	LambdaBarDataTree->SetBranchAddress("pTrelProton", &pTrelProton);
	LambdaBarDataTree->SetBranchAddress("RadiusSVtoPV", &RadiusSVtoPV);
	LambdaBarDataTree->SetBranchAddress("IsBar", &IsBar);
	LambdaBarDataTree->SetBranchAddress("PionNuclCorrWeight", &PionNuclCorrWeight);
	LambdaBarDataTree->SetBranchAddress("ProtonTrackEndRadius", &ProtonTrackEndRadius);
	LambdaBarDataTree->SetBranchAddress("ProtonTrackEndMomentum", &ProtonTrackEndMomentum);
	LambdaBarDataTree->SetBranchAddress("PionTrackEndRadius", &PionTrackEndRadius);
	LambdaBarDataTree->SetBranchAddress("PionTrackEndMomentum", &PionTrackEndMomentum);
	LambdaBarDataTree->SetBranchAddress("ProtonTrackV0Momentum", &ProtonTrackV0Momentum);
	LambdaBarDataTree->SetBranchAddress("PionTrackV0Momentum", &PionTrackV0Momentum);
	LambdaBarDataTree->SetBranchAddress("DecayLength", &DecayLength);
	LambdaBarDataTree->SetBranchAddress("DeltaDCA", &DeltaDCA);


	//creating file to output histograms to
	//TFile outputfile(outputfilename.c_str(), "RECREATE");
	TFile outputfileLambdaCutsAppliedTree(filelambdaTreeOutputFileName.c_str(), "RECREATE");
	//TFile dummyfile("/data/dust/group/h1/klesthen/BaryonAsym/H1LambdaC/dummyfile.root", "RECREATE");

	//Creating Tree to save Lambda Candidates to that pass all of our cuts
	TTree* lambdaRegTree = new TTree("Lambda Reg", "Lambda Reg");
	lambdaRegTree->SetAutoSave(0);

	//Defining Tree Branches for Cuts Applied Tree
	lambdaRegTree->Branch("invMasslambda", &invMasslambda, "invMasslambda/D");
	lambdaRegTree->Branch("invMasskaonCont", &invMasskaonCont, "invMasskaonCont/D");
	lambdaRegTree->Branch("invMassphotonCont", &invMassphotonCont, "invMassphotonCont/D");
	lambdaRegTree->Branch("pT", &pT, "pT/D");
	lambdaRegTree->Branch("Eta", &Eta, "Eta/D");
	lambdaRegTree->Branch("Phi", &Phi, "Phi/D");
	lambdaRegTree->Branch("x", &x, "x/D");
	lambdaRegTree->Branch("Q2s", &Q2s, "Q2s/D");
	lambdaRegTree->Branch("Ys", &Ys, "Ys/D");
	lambdaRegTree->Branch("Angle", &Angle, "Angle/D");
	lambdaRegTree->Branch("v0chi2", &v0chi2, "v0chi2/D");
	lambdaRegTree->Branch("weight1", &weight1, "weight1/D");
	lambdaRegTree->Branch("weight2", &weight2, "weight2/D");
	lambdaRegTree->Branch("SVx", &SVx, "SVx/D");
	lambdaRegTree->Branch("SVy", &SVy, "SVy/D");
	lambdaRegTree->Branch("SVz", &SVz, "SVz/D");
	lambdaRegTree->Branch("PVx", &PVx, "PVx/D");
	lambdaRegTree->Branch("PVy", &PVy, "PVy/D");
	lambdaRegTree->Branch("PVz", &PVz, "PVz/D");
	lambdaRegTree->Branch("LambdaPX", &LambdaPX, "LambdaPX/D");
	lambdaRegTree->Branch("LambdaPY", &LambdaPY, "LambdaPY/D");
	lambdaRegTree->Branch("LambdaPZ", &LambdaPZ, "LambdaPZ/D");
	lambdaRegTree->Branch("fRunType", &fRunType, "fRunType/O");
	lambdaRegTree->Branch("CosThetaStar", &CosThetaStar, "CosThetaStar/D");
	lambdaRegTree->Branch("ystar", &ystar, "ystar/D");
	lambdaRegTree->Branch("ylab", &ylab, "ylab/D");
	lambdaRegTree->Branch("scatEX", &scatEX, "scatEX/D");
	lambdaRegTree->Branch("scatEY", &scatEY, "scatEY/D");
	lambdaRegTree->Branch("scatEZ", &scatEZ, "scatEZ/D");
	lambdaRegTree->Branch("FoundGen", &FoundGen, "FoundGen/I");
	lambdaRegTree->Branch("s61", &s61, "s61/I");
	lambdaRegTree->Branch("s1", &s1, "s1/I");
	lambdaRegTree->Branch("s74", &s74, "s74/I");
	lambdaRegTree->Branch("s75", &s75, "s75/I");
	lambdaRegTree->Branch("s67", &s67, "s67/I");
	lambdaRegTree->Branch("PionCharge", &PionCharge, "PionCharge/D");
	lambdaRegTree->Branch("ProtonCharge", &ProtonCharge, "ProtonCharge/D");
	lambdaRegTree->Branch("pionFRUNZ", &pionFRUNZ, "pionFRUNZ/D");
	lambdaRegTree->Branch("protonFRUNZ", &protonFRUNZ, "protonFRUNZ/D");
	lambdaRegTree->Branch("PionPt", &PionPt, "PionPt/D");
	lambdaRegTree->Branch("ProtonPt", &ProtonPt, "ProtonPt/D");
	lambdaRegTree->Branch("PionTrackStartRadius", &PionTrackStartRadius, "PionTrackStartRadius/D");
	lambdaRegTree->Branch("ProtonTrackStartRadius", &ProtonTrackStartRadius, "ProtonTrackStartRadius/D");
	lambdaRegTree->Branch("PionNHitsdEdx", &PionNHitsdEdx, "PionNHitsdEdx/D");
	lambdaRegTree->Branch("ProtonNHitsdEdx", &ProtonNHitsdEdx, "ProtonNHitsdEdx/D");
	lambdaRegTree->Branch("PiondEdx", &PiondEdx, "PiondEdx/D");
	lambdaRegTree->Branch("ProtondEdx", &ProtondEdx, "ProtondEdx/D");
	lambdaRegTree->Branch("PionL", &PionL, "PionL/D");
	lambdaRegTree->Branch("PionLProton", &PionLProton, "PionLProton/D");
	lambdaRegTree->Branch("PionLelectron", &PionLelectron, "PionLelectron/D");
	lambdaRegTree->Branch("ProtonL", &ProtonL, "ProtonL/D");
	lambdaRegTree->Branch("ProtonLPion", &ProtonLPion, "ProtonLPion/D");
	lambdaRegTree->Branch("ProtonLelectron", &ProtonLelectron, "ProtonLelectron/D");
	lambdaRegTree->Branch("PionTrackLength", &PionTrackLength, "PionTrackLength/D");
	lambdaRegTree->Branch("ProtonTrackLength", &ProtonTrackLength, "ProtonTrackLength/D");
	lambdaRegTree->Branch("PionDCA", &PionDCA, "PionDCA/D");
	lambdaRegTree->Branch("ProtonDCA", &ProtonDCA, "ProtonDCA/D");
	lambdaRegTree->Branch("PionSDCA", &PionSDCA, "PionSDCA/D");
	lambdaRegTree->Branch("ProtonSDCA", &ProtonSDCA, "ProtonSDCA/D");
	lambdaRegTree->Branch("Chi2", &Chi2, "Chi2/D");
	lambdaRegTree->Branch("MassPionPion", &MassPionPion, "MassPionPion/D");
	lambdaRegTree->Branch("MassElecElec", &MassElecElec, "MassElecElec/D");
	lambdaRegTree->Branch("pTrelPion", &pTrelPion, "pTrelPion/D");
	lambdaRegTree->Branch("pTrelProton", &pTrelProton, "pTrelProton/D");
	lambdaRegTree->Branch("RadiusSVtoPV", &RadiusSVtoPV, "RadiusSVtoPV/D");
	lambdaRegTree->Branch("IsBar", &IsBar, "IsBar/I");
	lambdaRegTree->Branch("PionNuclCorrWeight", &PionNuclCorrWeight, "PionNuclCorrWeight/D");
	lambdaRegTree->Branch("ProtonTrackEndRadius", &ProtonTrackEndRadius, "ProtonTrackEndRadius/D");
	lambdaRegTree->Branch("ProtonTrackEndMomentum", "TVector3", &ProtonTrackEndMomentum);
	lambdaRegTree->Branch("PionTrackEndRadius", &PionTrackEndRadius, "PionTrackEndRadius/D");
	lambdaRegTree->Branch("PionTrackEndMomentum", "TVector3", &PionTrackEndMomentum);
	lambdaRegTree->Branch("ProtonTrackV0Momentum", "TVector3", &ProtonTrackV0Momentum);
	lambdaRegTree->Branch("PionTrackV0Momentum", "TVector3", &PionTrackV0Momentum);
	lambdaRegTree->Branch("DecayLength", &DecayLength, "DecayLength/D");
	lambdaRegTree->Branch("DeltaDCA", &DeltaDCA, "DeltaDCA/D");

	//Creating Tree to save Lambda Candidates to that pass all of our cuts
	TTree* lambdaBarTree = new TTree("Lambda Bar", "Lambda Bar");
	lambdaBarTree->SetAutoSave(0);

	//Defining Tree Branches for Cuts Applied Tree
	lambdaBarTree->Branch("invMasslambda", &invMasslambda, "invMasslambda/D");
	lambdaBarTree->Branch("invMasskaonCont", &invMasskaonCont, "invMasskaonCont/D");
	lambdaBarTree->Branch("invMassphotonCont", &invMassphotonCont, "invMassphotonCont/D");
	lambdaBarTree->Branch("pT", &pT, "pT/D");
	lambdaBarTree->Branch("Eta", &Eta, "Eta/D");
	lambdaBarTree->Branch("Phi", &Phi, "Phi/D");
	lambdaBarTree->Branch("x", &x, "x/D");
	lambdaBarTree->Branch("Q2s", &Q2s, "Q2s/D");
	lambdaBarTree->Branch("Ys", &Ys, "Ys/D");
	lambdaBarTree->Branch("Angle", &Angle, "Angle/D");
	lambdaBarTree->Branch("v0chi2", &v0chi2, "v0chi2/D");
	lambdaBarTree->Branch("weight1", &weight1, "weight1/D");
	lambdaBarTree->Branch("weight2", &weight2, "weight2/D");
	lambdaBarTree->Branch("SVx", &SVx, "SVx/D");
	lambdaBarTree->Branch("SVy", &SVy, "SVy/D");
	lambdaBarTree->Branch("SVz", &SVz, "SVz/D");
	lambdaBarTree->Branch("PVx", &PVx, "PVx/D");
	lambdaBarTree->Branch("PVy", &PVy, "PVy/D");
	lambdaBarTree->Branch("PVz", &PVz, "PVz/D");
	lambdaBarTree->Branch("LambdaPX", &LambdaPX, "LambdaPX/D");
	lambdaBarTree->Branch("LambdaPY", &LambdaPY, "LambdaPY/D");
	lambdaBarTree->Branch("LambdaPZ", &LambdaPZ, "LambdaPZ/D");
	lambdaBarTree->Branch("fRunType", &fRunType, "fRunType/O");
	lambdaBarTree->Branch("CosThetaStar", &CosThetaStar, "CosThetaStar/D");
	lambdaBarTree->Branch("ystar", &ystar, "ystar/D");
	lambdaBarTree->Branch("ylab", &ylab, "ylab/D");
	lambdaBarTree->Branch("scatEX", &scatEX, "scatEX/D");
	lambdaBarTree->Branch("scatEY", &scatEY, "scatEY/D");
	lambdaBarTree->Branch("scatEZ", &scatEZ, "scatEZ/D");
	lambdaBarTree->Branch("FoundGen", &FoundGen, "FoundGen/I");
	lambdaBarTree->Branch("s61", &s61, "s61/I");
	lambdaBarTree->Branch("s1", &s1, "s1/I");
	lambdaBarTree->Branch("s74", &s74, "s74/I");
	lambdaBarTree->Branch("s75", &s75, "s75/I");
	lambdaBarTree->Branch("s67", &s67, "s67/I");
	lambdaBarTree->Branch("PionCharge", &PionCharge, "PionCharge/D");
	lambdaBarTree->Branch("ProtonCharge", &ProtonCharge, "ProtonCharge/D");
	lambdaBarTree->Branch("pionFRUNZ", &pionFRUNZ, "pionFRUNZ/D");
	lambdaBarTree->Branch("protonFRUNZ", &protonFRUNZ, "protonFRUNZ/D");
	lambdaBarTree->Branch("PionPt", &PionPt, "PionPt/D");
	lambdaBarTree->Branch("ProtonPt", &ProtonPt, "ProtonPt/D");
	lambdaBarTree->Branch("PionTrackStartRadius", &PionTrackStartRadius, "PionTrackStartRadius/D");
	lambdaBarTree->Branch("ProtonTrackStartRadius", &ProtonTrackStartRadius, "ProtonTrackStartRadius/D");
	lambdaBarTree->Branch("PionNHitsdEdx", &PionNHitsdEdx, "PionNHitsdEdx/D");
	lambdaBarTree->Branch("ProtonNHitsdEdx", &ProtonNHitsdEdx, "ProtonNHitsdEdx/D");
	lambdaBarTree->Branch("PiondEdx", &PiondEdx, "PiondEdx/D");
	lambdaBarTree->Branch("ProtondEdx", &ProtondEdx, "ProtondEdx/D");
	lambdaBarTree->Branch("PionL", &PionL, "PionL/D");
	lambdaBarTree->Branch("PionLProton", &PionLProton, "PionLProton/D");
	lambdaBarTree->Branch("PionLelectron", &PionLelectron, "PionLelectron/D");
	lambdaBarTree->Branch("ProtonL", &ProtonL, "ProtonL/D");
	lambdaBarTree->Branch("ProtonLPion", &ProtonLPion, "ProtonLPion/D");
	lambdaBarTree->Branch("ProtonLelectron", &ProtonLelectron, "ProtonLelectron/D");
	lambdaBarTree->Branch("PionTrackLength", &PionTrackLength, "PionTrackLength/D");
	lambdaBarTree->Branch("ProtonTrackLength", &ProtonTrackLength, "ProtonTrackLength/D");
	lambdaBarTree->Branch("PionDCA", &PionDCA, "PionDCA/D");
	lambdaBarTree->Branch("ProtonDCA", &ProtonDCA, "ProtonDCA/D");
	lambdaBarTree->Branch("PionSDCA", &PionSDCA, "PionSDCA/D");
	lambdaBarTree->Branch("ProtonSDCA", &ProtonSDCA, "ProtonSDCA/D");
	lambdaBarTree->Branch("Chi2", &Chi2, "Chi2/D");
	lambdaBarTree->Branch("MassPionPion", &MassPionPion, "MassPionPion/D");
	lambdaBarTree->Branch("MassElecElec", &MassElecElec, "MassElecElec/D");
	lambdaBarTree->Branch("pTrelPion", &pTrelPion, "pTrelPion/D");
	lambdaBarTree->Branch("pTrelProton", &pTrelProton, "pTrelProton/D");
	lambdaBarTree->Branch("RadiusSVtoPV", &RadiusSVtoPV, "RadiusSVtoPV/D");
	lambdaBarTree->Branch("IsBar", &IsBar, "IsBar/I");
	lambdaBarTree->Branch("PionNuclCorrWeight", &PionNuclCorrWeight, "PionNuclCorrWeight/D");
	lambdaBarTree->Branch("ProtonTrackEndRadius", &ProtonTrackEndRadius, "ProtonTrackEndRadius/D");
	lambdaBarTree->Branch("ProtonTrackEndMomentum", "TVector3", &ProtonTrackEndMomentum);
	lambdaBarTree->Branch("PionTrackEndRadius", &PionTrackEndRadius, "PionTrackEndRadius/D");
	lambdaBarTree->Branch("PionTrackEndMomentum", "TVector3", &PionTrackEndMomentum);
	lambdaBarTree->Branch("ProtonTrackV0Momentum", "TVector3", &ProtonTrackV0Momentum);
	lambdaBarTree->Branch("PionTrackV0Momentum", "TVector3", &PionTrackV0Momentum);
	lambdaBarTree->Branch("DecayLength", &DecayLength, "DecayLength/D");
	lambdaBarTree->Branch("DeltaDCA", &DeltaDCA, "DeltaDCA/D");

	//Cut values
	double PionLCutValue = 0.001;
	double ProtonLCutValue = 0.001;
	double DecayLengthCutValue = 2;
	double DeltaDCACutValue = 0.5;
	double PionSDCACutValue = 1.0;
	double ProtonSDCACutValue = 1.0;
	double PionPtCutValue = 0.12;
	double DCAProductCutValue = 0.1;
	double MassElecElecCutValue = 0.05;
	double dEdxNHitsCutValue = 10;

	//Accessing the Reg Tree, Apply Cuts, and filling cut by cut Histograms
	int m;
	int entries;
	entries = LambdaRegDataTree->GetEntries();
	double RegCandidateCountCheck = 0;
	for (m = 0; m < entries; m++) {
		LambdaRegDataTree->GetEntry(m);
		//Starting Point Before Further Imposing Cuts
		LambdaRegDataCutProgression[0]->Fill(invMasslambda, weight1 * weight2 * PionNuclCorrWeight);
		if (PionL > PionLCutValue) {
			LambdaRegDataCutProgression[1]->Fill(invMasslambda, weight1 * weight2 * PionNuclCorrWeight);
			if (ProtonL > ProtonLCutValue) {
				LambdaRegDataCutProgression[2]->Fill(invMasslambda, weight1 * weight2 * PionNuclCorrWeight);
				if (DecayLength > DecayLengthCutValue) {
					LambdaRegDataCutProgression[3]->Fill(invMasslambda, weight1 * weight2 * PionNuclCorrWeight);
					if (DeltaDCA > DeltaDCACutValue) {
						LambdaRegDataCutProgression[4]->Fill(invMasslambda, weight1 * weight2 * PionNuclCorrWeight);
						if (abs(PionSDCA) > PionSDCACutValue) {
							LambdaRegDataCutProgression[5]->Fill(invMasslambda, weight1 * weight2 * PionNuclCorrWeight);
							if (abs(ProtonSDCA) > ProtonSDCACutValue) {
								LambdaRegDataCutProgression[6]->Fill(invMasslambda, weight1 * weight2 * PionNuclCorrWeight);
								if (PionPt > PionPtCutValue) {
									LambdaRegDataCutProgression[7]->Fill(invMasslambda, weight1 * weight2 * PionNuclCorrWeight);
									if (ProtonDCA * PionDCA < DCAProductCutValue) {
										LambdaRegDataCutProgression[8]->Fill(invMasslambda, weight1 * weight2 * PionNuclCorrWeight);
										//if (ProtonSDCA * PionSDCA > 25) {
											LambdaRegDataCutProgression[9]->Fill(invMasslambda, weight1 * weight2 * PionNuclCorrWeight);
											if (MassElecElec>MassElecElecCutValue){
												LambdaRegDataCutProgression[10]->Fill(invMasslambda, weight1 * weight2 * PionNuclCorrWeight);
												if (PionNHitsdEdx >= dEdxNHitsCutValue) {
													LambdaRegDataCutProgression[11]->Fill(invMasslambda, weight1 * weight2 * PionNuclCorrWeight);
													if (ProtonNHitsdEdx >= dEdxNHitsCutValue) {
														LambdaRegDataCutProgression[12]->Fill(invMasslambda, weight1 * weight2 * PionNuclCorrWeight);
														//This is our last cut, save to Cuts Applied Tree
														lambdaRegTree->Fill();
														RegCandidateCountCheck++;
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
		}
	}

	//Accessing the Bar Tree, Apply Cuts, and filling cut by cut Histograms
	int p;
	int entries2;
	entries2 = LambdaBarDataTree->GetEntries();
	double BarCandidateCountCheck = 0;
	for (p = 0; p < entries2; p++) {
		LambdaBarDataTree->GetEntry(p);
		//Starting Point Before Further Imposing Cuts
		LambdaBarDataCutProgression[0]->Fill(invMasslambda, weight1 * weight2 * PionNuclCorrWeight);
		if (PionL > PionLCutValue) {
			LambdaBarDataCutProgression[1]->Fill(invMasslambda, weight1 * weight2 * PionNuclCorrWeight);
			if (ProtonL > ProtonLCutValue) {
				LambdaBarDataCutProgression[2]->Fill(invMasslambda, weight1 * weight2 * PionNuclCorrWeight);
				if (DecayLength > DecayLengthCutValue) {
					LambdaBarDataCutProgression[3]->Fill(invMasslambda, weight1 * weight2 * PionNuclCorrWeight);
					if (DeltaDCA > DeltaDCACutValue) {
						LambdaBarDataCutProgression[4]->Fill(invMasslambda, weight1 * weight2 * PionNuclCorrWeight);
						if (abs(PionSDCA) > PionSDCACutValue) {
							LambdaBarDataCutProgression[5]->Fill(invMasslambda, weight1 * weight2 * PionNuclCorrWeight);
							if (abs(ProtonSDCA) > ProtonSDCACutValue) {
								LambdaBarDataCutProgression[6]->Fill(invMasslambda, weight1 * weight2 * PionNuclCorrWeight);
								if (PionPt > PionPtCutValue) {
									LambdaBarDataCutProgression[7]->Fill(invMasslambda, weight1 * weight2 * PionNuclCorrWeight);
									if (ProtonDCA * PionDCA < DCAProductCutValue) {
										LambdaBarDataCutProgression[8]->Fill(invMasslambda, weight1 * weight2 * PionNuclCorrWeight);
										//if (ProtonSDCA * PionSDCA > 25) {
										LambdaBarDataCutProgression[9]->Fill(invMasslambda, weight1 * weight2 * PionNuclCorrWeight);
										if (MassElecElec > MassElecElecCutValue) {
											LambdaBarDataCutProgression[10]->Fill(invMasslambda, weight1 * weight2 * PionNuclCorrWeight);
											if (PionNHitsdEdx >= dEdxNHitsCutValue) {
												LambdaBarDataCutProgression[11]->Fill(invMasslambda, weight1 * weight2 * PionNuclCorrWeight);
												if (ProtonNHitsdEdx >= dEdxNHitsCutValue) {
													LambdaBarDataCutProgression[12]->Fill(invMasslambda, weight1 * weight2 * PionNuclCorrWeight);
													//This is our last cut, save to Cuts Applied Tree
													lambdaBarTree->Fill();
													BarCandidateCountCheck++;
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
		}
	}


	cout << "Number of Lambda Reg Passing Cuts: " << RegCandidateCountCheck << endl;
	cout << "Number of Lambda Bar Passing Cuts: " << BarCandidateCountCheck << endl;
	
	lambdaRegTree->Write();
	lambdaBarTree->Write();
	//outputfileLambdaCutsAppliedTree.Write();
	//outputfileLambdaCutsAppliedTree.ls();
	outputfileLambdaCutsAppliedTree.Close();


	TFile outputfile(outputfilename.c_str(), "RECREATE");
	//Creating and Drawing the Final Stack
	THStack* hsLambdaRegData = new THStack("hs", "2005-2007 H1 Data #Lambda Mass Spectrum");
	THStack* hsLambdaBarData = new THStack("hs", "2005-2007 H1 Data #bar{#Lambda} Mass Spectrum");

	for (int a = 0; a < 16; a++) {
		LambdaRegDataCutProgression[a]->SetStats(0);
		LambdaRegDataCutProgression[a]->SetLineColor(kBlack + a);
		LambdaRegDataCutProgression[a]->SetMarkerColor(kBlack + a);
		LambdaRegDataCutProgression[a]->GetXaxis()->SetTitle("Mass [GeV]");
		LambdaRegDataCutProgression[a]->GetYaxis()->SetTitle("Count");
		LambdaRegDataCutProgression[a]->Draw("l");
		LambdaRegDataCutProgression[a]->Write();

		hsLambdaRegData->Add(LambdaRegDataCutProgression[a],"HIST");

		LambdaBarDataCutProgression[a]->SetStats(0);
		LambdaBarDataCutProgression[a]->SetLineColor(kBlack + a);
		LambdaBarDataCutProgression[a]->SetMarkerColor(kBlack + a);
		LambdaBarDataCutProgression[a]->GetXaxis()->SetTitle("Mass [GeV]");
		LambdaBarDataCutProgression[a]->GetYaxis()->SetTitle("Count");
		LambdaBarDataCutProgression[a]->Draw("l");
		LambdaBarDataCutProgression[a]->Write();

		hsLambdaBarData->Add(LambdaBarDataCutProgression[a], "HIST");
	}


	hsLambdaRegData->Draw("nostack");
	hsLambdaRegData->GetXaxis()->SetTitle("Mass [GeV]");
	hsLambdaRegData->GetYaxis()->SetTitle("Entries per GeV");
	canvas->SetName("Lambda Reg Data Cut Stack");
	canvas->Write();

	hsLambdaBarData->Draw("nostack");
	hsLambdaBarData->GetXaxis()->SetTitle("Mass [GeV]");
	hsLambdaBarData->GetYaxis()->SetTitle("Entries per GeV");
	canvas->SetName("Lambda Bar Data Cut Stack");
	canvas->Write();

	



	//Lambda Data vs MC RAPGAP and DJANGOH
	//THStack* hsLambdaDataVsMC = new THStack("hs", "Lambda Data Vs MC Mass Plots");
	//hsLambdaDataVsMC->Add(finalDataLambdaHist);
	//hsLambdaDataVsMC->Add(finalRAPGAPLambdaHist);
	//hsLambdaDataVsMC->Add(finalDJANGOHLambdaHist);
	//hsLambdaDataVsMC->Draw("nostack");
	//TLegend* legend = new TLegend(0.5513, 0.4144, 0.888, 0.8823);
	//legend->SetHeader(Form("#splitline{Count- %.2f #pm %.2f (stat.)}{Stochastic MLP- %s Network Trained %ix}", kcount, kcounterror, network[particlecode].c_str(), trainnum[particlecode])); // option "C" allows to center the header
	//legend->AddEntry(finalDataLambdaHist, "2005-2007 H1 Data", "l");
	//legend->AddEntry(finalRAPGAPLambdaHist, "#frac{L_{Data}}{L_{RAPGAP}} RAPGAP", "l");
	//legend->AddEntry(finalDJANGOHLambdaHist, "#frac{L_{Data}}{L_{DJANGOH}} DJANGOH", "l");
	//legend->Draw("same");
	//hsLambdaDataVsMC->GetXaxis()->SetTitle("Mass [GeV]");
	//hsLambdaDataVsMC->GetYaxis()->SetTitle("Entries per GeV");
	//canvas->SetName("Lambda Data vs MC");
	//canvas->Write();

	//outputfile.Write();
	outputfile.Close();

	cout << "Done" << endl;

};


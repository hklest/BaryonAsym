#include <TROOT.h>
#include <TApplication.h>
#include <TSystem.h>
#include <TFile.h>

#include "H1Skeleton/H1Tree.h"
#include "H1Geom/H1Constants.h"

#include "H1Calculator/H1Calculator.h"
#include "H1Calculator/H1CalcGenericInterface.h"
#include "H1Calculator/H1CalcSystematic.h"
#include "H1Calculator/H1CalcFs.h"
#include "H1Calculator/H1CalcKine.h"
#include "H1Calculator/H1CalcElec.h"
#include "H1Calculator/H1CalcVertex.h"
#include "H1Calculator/H1CalcTrig.h"
#include "H1Calculator/H1CalcForwDets.h"
#include "H1Calculator/H1CalcTrack.h"
#include "H1Calculator/H1CalcWeight.h"
#include "H1Calculator/H1CalcJet.h"

#include <iostream>

// Initialize the root framework
TROOT h1tree("h1tree", "Test H1Tree");

int main(int argc, char** argv)
{

    TApplication theApp("App", &argc, argv);
    H1Tree* tree = H1Tree::Instance();
    tree->AddFile("/user/southd/mods.3.3.6.django62.99.MRSH_NC_POSI920_Q60_W500.S30910.R90403.DST.A00-A11.root", "MODS");
    tree->AddFile("/user/southd/hat.3.3.6.django62.99.MRSH_NC_POSI920_Q60_W500.S30910.R90403.DST.A00-A11.root", "HAT");

    tree->Open();
    tree->SelectAll();
    tree->Reset();
    H1Calculator::Instance();

    //Set up constants for 99-00 data conditions
    TheConstants().SetConstants(H1Constants::eEplus9900);

    //Eventloop
    Int_t EventNumber = 0;

    // Read the tree event by event
    //  gH1Calc->Weight()->ApplyPsCCWeight(true);
    //  gH1Calc->Weight()->ApplyVertexWeight(true);

    while (tree->Next()) {

        gH1Calc->Reset();

        // Print out the event number from time to time

        cout << "*************************************" << endl;
        cout << "         Next Event " << EventNumber++ << endl;
        cout << "*************************************" << endl;

        Float_t z = gH1Calc->Vertex()->GetZ();
        cout << " Zvertex " << z << endl;

        Float_t ptcalo = gH1Calc->Fs()->GetPtCalo();
        cout << " Ptcalo " << ptcalo << endl;

        Float_t empzgen = gH1Calc->Fs()->GetEmpzGen();
        cout << " E-Pz Gen " << empzgen << endl;

        gH1Calc->Reset();
        H1CalcSystematic* sys = gH1Calc->Systematic();
        sys->ShiftUp(sys->eElecTh);

        ptcalo = gH1Calc->Fs()->GetPtCalo();
        cout << " Ptcalo after shift " << ptcalo << endl;
        Float_t pte = gH1Calc->Elec()->GetFirstElectron().Pt();
        cout << " pte " << pte << endl;
        Float_t empz = gH1Calc->Fs()->GetEmpz();
        cout << " E-Pz after shift " << empz << endl;

        sys->ResetAllShifts();
        gH1Calc->Reset();
        ptcalo = gH1Calc->Fs()->GetPtCalo();
        cout << " Ptcalo after shift  reset " << ptcalo << endl;

        empz = gH1Calc->Fs()->GetEmpz();
        cout << " E-Pz " << empz << endl;

        Int_t nfmu = gH1Calc->ForwDets()->GetNFmu(2);
        cout << " Nfmu 2 " << nfmu << endl;

        Int_t nfmu0 = gH1Calc->ForwDets()->GetNFmu(0);
        cout << " Nfmu 0 " << nfmu0 << endl;

        Int_t nfmutot = gH1Calc->ForwDets()->GetNFmu();
        cout << " Nfmu tot " << nfmutot << endl;

        Int_t ntr = gH1Calc->Track()->GetNumTracks();
        cout << " N tracks " << ntr << endl;

        Int_t nloosetr = gH1Calc->Track()->GetNumLooseTracks();
        cout << " Nloose tracks " << nloosetr << endl;

        Int_t nnvtr = gH1Calc->Track()->GetNumNonvertexTracks();
        cout << " N Nonvertex tracks " << nnvtr << endl;

        Float_t eplug = gH1Calc->ForwDets()->GetEPlug();
        cout << " eplug " << eplug << endl;

        Float_t q2e = gH1Calc->Kine()->GetQ2e();
        cout << " q2e " << q2e << endl;

        Int_t ne = gH1Calc->Elec()->GetNEmParts();
        cout << " numem " << ne << endl;
        pte = gH1Calc->Elec()->GetFirstElectron().Pt();
        cout << " pte " << pte << endl;

        //Generic Access
        using namespace H1CalcGenericInterface;
        Float_t xpom = gH1Calc->GetFloatVariable(DiffKine_Xpom);
        cout << " xpom " << xpom << endl;

        Float_t etamax2 = gH1Calc->GetFloatVariable(Diff_EtaMax_idx, 1);
        cout << " etamax2 " << etamax2 << endl;

        Float_t ptallfs = gH1Calc->GetLorentzVariable(Fs_AllFinalStateParticles).Pt();
        cout << " Pt all FS " << ptallfs << endl;

        Float_t ptjet = gH1Calc->Jet()->GetJet().Pt();
        cout << " Pt Jet " << ptjet << endl;


        if (EventNumber > 100) break;

    }

    cout << "Finished" << endl;

    gSystem->Exit(1);
    theApp.Run(kTRUE);
    return 0;
}


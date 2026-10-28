// McCandidateXScan — WP-D round 3 (h1-mc-expert, 2026-10-28): generator-level gate
// 840x (gens 5643/5644) vs 9012 (gen 4489) in R = {Q2Gki>150, 0.1<YGki<0.9, XpomGki<0.03}.
//
// One H1Tree loop over a comma-separated list of hat/mods pairs (gen-level only: no ODS).
// HAT preselection (SelectHat "Q2Gki>140") so MODS is read for ~1% of the events.
// Per event in R (no further cut; fiducial M_X/M_Y/|t| applied offline) the tree stores:
//   HAT/GKI kinematics (Q2, y, x, x_IP, t, M_X, M_Y, beta = x/x_IP, Pt2Hat, GenRad, VtxZ, run),
//   the HAT EtaMaxGen, and a recomputation of the H1HatDiffractiveFiller::FillGen X/Y split
//   (largest rapidity gap among status-0 GTR particles in the gamma*-p cms, scattered lepton
//   excluded; -ve rapidity = proton side), from which: M_X, M_Y, eta_max of the X system for
//   E thresholds 0 / 0.4 / 0.8 GeV (lab eta of the most forward X particle, origin vertex),
//   charged multiplicities in X, and per charged pi/K/p of X the lab eta, pT, charge and the
//   X-rest-frame x_F, p_T*^2, z* = 2E*/M_X with the axis u = direction of the Pomeron
//   p_IP = P - p_Y boosted into the X rest frame (Pomeron-positive, plan D6 / WP-C binning_record §1).
// Usage: McCandidateXScan --steer S --out O.root --tag T [--maxevents N] [--nosel]
#include <string>
#include <vector>
#include <iostream>
#include <cmath>
#include <cstdlib>
#include "TFile.h"
#include "TTree.h"
#include "TLorentzVector.h"
#include "TVector3.h"
#include "TMath.h"
#include "H1Pointers/H1FloatPtr.h"
#include "H1Pointers/H1IntPtr.h"
#include "H1Pointers/H1ShortPtr.h"
#include "H1Skeleton/H1Tree.h"
#include "H1Steering/H1StdCmdLine.h"
#include "H1Steering/H1SteerManager.h"
#include "H1Mods/H1PartMCArrayPtr.h"
#include "H1Mods/H1PartMC.h"
#include "H1Mods/H1GkiInfoArrayPtr.h"
#include "H1Mods/H1GkiInfo.h"
using namespace std;
static const Int_t kMaxH = 300;
static Int_t HeavyFlavourOf(Int_t pdg) {
    Int_t a = abs(pdg); if (a < 100 || a >= 10000) return 0;
    Int_t q1 = (a / 1000) % 10, q2 = (a / 100) % 10, q3 = (a / 10) % 10;
    if (q1 != 0 && q3 == 0) return 0;
    Int_t hf = 0; if (q1 == 4 || q2 == 4 || q3 == 4) hf = 4; if (q1 == 5 || q2 == 5 || q3 == 5) hf = 5; return hf;
}
int main(int argc, char** argv) {
    string steerFile, outName, tag = "xscan"; long maxEvents = 1000000000L; bool useSel = true;
    { vector<char*> nA; nA.push_back(argv[0]); int nC = 1;
      for (int ia = 1; ia < argc; ia++) { string a = argv[ia]; string v = (ia + 1 < argc) ? argv[ia + 1] : "";
        if (a == "--steer") { steerFile = v; ia++; continue; } if (a == "--out") { outName = v; ia++; continue; }
        if (a == "--tag") { tag = v; ia++; continue; } if (a == "--maxevents") { maxEvents = atol(v.c_str()); ia++; continue; }
        if (a == "--nosel") { useSel = false; continue; } nA.push_back(argv[ia]); nC++; }
      char** cA = nA.data(); H1StdCmdLine opts; opts.Parse(&nC, cA); }
    if (steerFile.empty() || outName.empty()) { cerr << "usage: McCandidateXScan --steer S --out O.root [--tag T] [--maxevents N] [--nosel]" << endl; return 2; }
    gH1SteerManager->ParseFile(steerFile.c_str());
    H1Tree::Instance()->Open();
    cout << "McCandidateXScan[" << tag << "]: isMC=" << gH1Tree->IsMC() << " entries(all)=" << gH1Tree->GetEntries() << endl;
    if (useSel) { Int_t nsel = gH1Tree->SelectHat("Q2Gki>140"); cout << "McCandidateXScan[" << tag << "]: SelectHat(Q2Gki>140) -> " << nsel << endl; }
    H1PartMCArrayPtr partmc("MCParticles");
    H1GkiInfoArrayPtr gkiArr(0); Bool_t gkiOk = gkiArr.SetPointer("GkiInfo");
    H1IntPtr hRun("RunNumber"), hEvent("EventNumber"); H1FloatPtr hW1("Weight1"), hW2("Weight2"); H1ShortPtr hGenRad("GenRad");
    H1FloatPtr hQ2Gki("Q2Gki"), hYGki("YGki"), hXGki("XGki"), hXpomGki("XpomGki"), hTGki("TGki"), hTGen("TGen");
    H1FloatPtr hMXGen("MXGen"), hMYGen("MYGen"), hPt2Hat("Pt2HatGki"), hEtaMaxGen("EtaMaxGen"), hVtxZ("VtxZ"), hQ2sGen("Q2sGen"), hYsGen("YsGen");
    H1FloatPtr hRapU("RapUpperGen"), hRapL("RapLowerGen");
    TFile* outFile = new TFile(outName.c_str(), "RECREATE"); TTree* tr = new TTree("xscan", "WP-D round-3 gen-level X-system scan");
    Int_t run, event, genrad, pdfid2, nX, nY, nchX, nchXfid, nH, hasCharm, nIsr; Float_t w, q2, y, x, xpom, t, tgen, mx, my, beta, pt2hat, etamaxhat, vtxz, q2s, ys, rapu, rapl;
    Float_t mxR, myR, tR, etaX0, etaX04, etaX08, etaX15, xpomR, betaR, pXpt, pIPpt, eIsr; 
    Int_t hPdg[kMaxH]; Float_t hQ[kMaxH], hEta[kMaxH], hPt[kMaxH], hXf[kMaxH], hPt2s[kMaxH], hZs[kMaxH];
    tr->Branch("run", &run, "run/I"); tr->Branch("event", &event, "event/I"); tr->Branch("w", &w, "w/F"); tr->Branch("genrad", &genrad, "genrad/I");
    tr->Branch("pdfid2", &pdfid2, "pdfid2/I"); tr->Branch("q2", &q2, "q2/F"); tr->Branch("y", &y, "y/F"); tr->Branch("x", &x, "x/F");
    tr->Branch("xpom", &xpom, "xpom/F"); tr->Branch("t", &t, "t/F"); tr->Branch("tgen", &tgen, "tgen/F"); tr->Branch("mx", &mx, "mx/F"); tr->Branch("my", &my, "my/F");
    tr->Branch("beta", &beta, "beta/F"); tr->Branch("pt2hat", &pt2hat, "pt2hat/F"); tr->Branch("etamaxhat", &etamaxhat, "etamaxhat/F"); tr->Branch("vtxz", &vtxz, "vtxz/F");
    tr->Branch("q2s", &q2s, "q2s/F"); tr->Branch("ys", &ys, "ys/F"); tr->Branch("rapu", &rapu, "rapu/F"); tr->Branch("rapl", &rapl, "rapl/F");
    tr->Branch("mxR", &mxR, "mxR/F"); tr->Branch("myR", &myR, "myR/F"); tr->Branch("tR", &tR, "tR/F"); tr->Branch("xpomR", &xpomR, "xpomR/F"); tr->Branch("betaR", &betaR, "betaR/F");
    tr->Branch("etaX0", &etaX0, "etaX0/F"); tr->Branch("etaX04", &etaX04, "etaX04/F"); tr->Branch("etaX08", &etaX08, "etaX08/F"); tr->Branch("etaX15", &etaX15, "etaX15/F");
    tr->Branch("pXpt", &pXpt, "pXpt/F"); tr->Branch("pIPpt", &pIPpt, "pIPpt/F"); tr->Branch("nIsr", &nIsr, "nIsr/I"); tr->Branch("eIsr", &eIsr, "eIsr/F");
    tr->Branch("nX", &nX, "nX/I"); tr->Branch("nY", &nY, "nY/I"); tr->Branch("nchX", &nchX, "nchX/I"); tr->Branch("nchXfid", &nchXfid, "nchXfid/I"); tr->Branch("hasCharm", &hasCharm, "hasCharm/I");
    tr->Branch("nH", &nH, "nH/I"); tr->Branch("hPdg", hPdg, "hPdg[nH]/I"); tr->Branch("hQ", hQ, "hQ[nH]/F"); tr->Branch("hEta", hEta, "hEta[nH]/F"); tr->Branch("hPt", hPt, "hPt[nH]/F");
    tr->Branch("hXf", hXf, "hXf[nH]/F"); tr->Branch("hPt2s", hPt2s, "hPt2s[nH]/F"); tr->Branch("hZs", hZs, "hZs[nH]/F");
    long n = 0, nR = 0, nAll = 0; vector<Float_t> rap; vector<Int_t> gidx; vector<Int_t> order;
    while (H1Tree::Instance()->Next()) {
        if (nAll >= maxEvents) break; nAll++;
        q2 = *hQ2Gki; y = *hYGki; xpom = *hXpomGki;
        if (!(q2 > 150. && y > 0.1 && y < 0.9 && xpom < 0.03)) continue;
        n++;
        run = *hRun; event = *hEvent; w = (*hW1) * (*hW2); genrad = *hGenRad; x = *hXGki; t = *hTGki; tgen = *hTGen; mx = *hMXGen; my = *hMYGen;
        beta = (xpom > 0) ? x / xpom : -1; pt2hat = *hPt2Hat; etamaxhat = *hEtaMaxGen; vtxz = *hVtxZ; q2s = *hQ2sGen; ys = *hYsGen; rapu = *hRapU; rapl = *hRapL;
        pdfid2 = -999; if (gkiOk && gkiArr.GetEntries() > 0 && gkiArr[0]) pdfid2 = gkiArr[0]->GetPDFID2();
        // --- record: [0] beam lepton (201), [1] beam proton (201), [3] scattered lepton (status 0, mother 0), radiative photon status 202
        Int_t np = partmc.GetEntries(); TLorentzVector kBeam, pBeam, kScat, gamma; Int_t iScat = -1; hasCharm = 0; nIsr = 0; eIsr = 0;
        Bool_t haveBeams = kFALSE;
        for (Int_t i = 0; i < np; i++) { H1PartMC* p = partmc[i]; if (!p) continue; Int_t st = p->GetStatus(); Int_t a = abs(p->GetPDG());
            if (st == 201 && a == 11) kBeam = p->GetFourVector(); if (st == 201 && a == 2212) { pBeam = p->GetFourVector(); haveBeams = kTRUE; }
            if (st == 0 && a == 11 && iScat < 0 && p->GetMother1() == 0) { iScat = i; kScat = p->GetFourVector(); }
            if (st == 202) { nIsr++; eIsr += p->GetE(); }
            if (HeavyFlavourOf(p->GetPDG()) == 4) hasCharm = 1; }
        if (iScat < 0) { // fallback: highest-energy status-0 lepton
            Double_t emax = -1; for (Int_t i = 0; i < np; i++) { H1PartMC* p = partmc[i]; if (p && p->GetStatus() == 0 && abs(p->GetPDG()) == 11 && p->GetE() > emax) { emax = p->GetE(); iScat = i; kScat = p->GetFourVector(); } } }
        // ISR-only true gamma*: q = k - k' - (ISR photons, mother = beam lepton)
        gamma = kBeam - kScat;
        for (Int_t i = 0; i < np; i++) { H1PartMC* p = partmc[i]; if (p && p->GetStatus() == 202 && p->GetMother1() == 0) gamma -= p->GetFourVector(); }
        // --- X/Y split as in H1HatDiffractiveFiller::FillGen: rapidity in gamma*-p cms of status-0 GTR particles (scattered lepton excluded)
        rap.clear(); gidx.clear();
        TLorentzVector cms = gamma + pBeam; TVector3 bcms = -cms.BoostVector();
        // axis: after boost, rotate so proton along -z (filler: "-ve rapidity is proton direction"); use rapidity along proton axis with sign flip
        TLorentzVector pB = pBeam; pB.Boost(bcms); TVector3 axis = pB.Vect().Unit();
        for (Int_t i = 0; i < np; i++) { H1PartMC* p = partmc[i]; if (!p || p->GetStatus() != 0 || i == iScat || !p->IsGtr()) continue;
            TLorentzVector v = p->GetFourVector(); v.Boost(bcms); Double_t pl = v.Vect().Dot(axis); Double_t e = v.E();
            if (!(e > fabs(pl))) continue; Float_t r = -0.5 * TMath::Log((e + pl) / (e - pl)); // -ve = proton side
            if (TMath::IsNaN(r)) continue; rap.push_back(r); gidx.push_back(i); }
        Int_t nst = rap.size(); order.resize(nst); for (Int_t i = 0; i < nst; i++) order[i] = i;
        if (nst > 1) TMath::Sort(nst, &rap[0], &order[0], kFALSE);
        Int_t bound = -1; Float_t gap = 0; for (Int_t i = 0; i + 1 < nst; i++) { Float_t d = rap[order[i + 1]] - rap[order[i]]; if (d > gap) { gap = d; bound = i; } }
        TLorentzVector pX(0, 0, 0, 0), pY(0, 0, 0, 0); nX = nY = nchX = nchXfid = 0; etaX0 = etaX04 = etaX08 = etaX15 = -99;
        vector<Int_t> xidx;
        for (Int_t k = 0; k < nst; k++) { H1PartMC* p = partmc[gidx[order[k]]]; TLorentzVector v = p->GetFourVector();
            if (k <= bound) { pY += v; nY++; } else { pX += v; nX++; xidx.push_back(gidx[order[k]]);
                Double_t eta = (v.Vect().CosTheta() * v.Vect().CosTheta() < 1.) ? v.Eta() : (v.Pz() > 0 ? 99 : -99); Double_t e = v.E();
                if (eta > etaX0) etaX0 = eta; if (e > 0.4 && eta > etaX04) etaX04 = eta; if (e > 0.8 && eta > etaX08) etaX08 = eta; if (e > 1.5 && eta > etaX15) etaX15 = eta;
                if (fabs(p->GetCharge()) > 0.1) nchX++; } }
        mxR = (pX.M2() > 0) ? pX.M() : 0; myR = (pY.M2() > 0) ? pY.M() : 0; tR = (pBeam - pY).M2();
        TLorentzVector pIP = pBeam - pY; Double_t qP = gamma.Dot(pBeam); xpomR = (haveBeams && qP != 0) ? gamma.Dot(pIP) / qP : -1; // x_IP = q.(P-p_Y)/(q.P)
        betaR = (xpomR > 0) ? x / xpomR : -1; pXpt = pX.Pt(); pIPpt = pIP.Pt();
        // --- X rest frame, u = Pomeron direction
        nH = 0; TVector3 bX = -pX.BoostVector(); TLorentzVector ipX = pIP; ipX.Boost(bX); TVector3 u = ipX.Vect().Unit();
        for (size_t k = 0; k < xidx.size() && nH < kMaxH; k++) { H1PartMC* p = partmc[xidx[k]]; Int_t a = abs(p->GetPDG());
            if (!(a == 211 || a == 321 || a == 2212)) continue; TLorentzVector v = p->GetFourVector(); Float_t qc = p->GetCharge(); if (fabs(qc) < 0.1) continue;
            Double_t eta = v.Eta(), pt = v.Pt(); if (fabs(eta) < 1.6 && pt > 0.15 && pt < 15.) nchXfid++;
            TLorentzVector vs = v; vs.Boost(bX); Double_t pl = vs.Vect().Dot(u);
            hPdg[nH] = p->GetPDG(); hQ[nH] = qc; hEta[nH] = eta; hPt[nH] = pt; hXf[nH] = (mxR > 0) ? 2. * pl / mxR : 0; hPt2s[nH] = vs.Vect().Mag2() - pl * pl; hZs[nH] = (mxR > 0) ? 2. * vs.E() / mxR : 0; nH++; }
        tr->Fill(); nR++;
    }
    outFile->Write(); outFile->Close(); delete outFile;
    cout << "McCandidateXScan[" << tag << "]: done, looped=" << nAll << " inR=" << nR << endl; return 0;
}

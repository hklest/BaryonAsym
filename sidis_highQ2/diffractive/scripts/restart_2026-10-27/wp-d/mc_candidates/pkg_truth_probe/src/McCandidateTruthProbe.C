// McCandidateTruthProbe — WP-D MC-candidate truth probe (h1-mc-expert, 2026-10-28)
//
// Single H1Tree loop over ONE hat/mods pair (gen-level only: no ODS, no FPACK).
// Purpose: decide (a) the flavour content of RG31034 POMC (gen 4490 / ReqID 9022)
// versus POMUDS (gen 4489 / 9012), (b) the hard-subprocess composition of the
// MCWE-weighted gens 5717/5718/5719 (8529/8530/8531), from the GKI bank words
// (MODS "GkiInfo": PDFID1/2 = incoming parton flavours, PT2HAT, NHFHAD/HFHADi =
// heavy-flavour hadron list, SIGMT) and from the generator particle record
// (MODS "MCParticles": any-status PDG scan for charm/beauty hadrons).
//
// Output: TTree "probe" (one entry per event) + JSON summary.
// Usage: McCandidateTruthProbe --steer S --out O.root --json O.json --tag T [--maxevents N]

#include <string>
#include <vector>
#include <map>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <cmath>
#include <cstdlib>

#include "TFile.h"
#include "TTree.h"

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

static const Int_t kNFirst = 16;

// heavy-flavour hadron classification from |PDG| (mesons 4xx/5xx, baryons 4xxx/5xxx;
// excludes bare quarks and diquarks)
static Int_t HeavyFlavourOf(Int_t pdg) {
    Int_t a = abs(pdg);
    if (a < 100 || a >= 10000) return 0;
    Int_t q1 = (a / 1000) % 10, q2 = (a / 100) % 10, q3 = (a / 10) % 10;
    if (q1 != 0 && q3 == 0) return 0;          // diquark
    Int_t hf = 0;
    if (q1 == 4 || q2 == 4 || q3 == 4) hf = 4;
    if (q1 == 5 || q2 == 5 || q3 == 5) hf = 5;
    return hf;
}

int main(int argc, char** argv) {
    string steerFile, outName, jsonName, tag = "probe";
    long maxEvents = 1000000000L;
    {
        vector<char*> newArgv; newArgv.push_back(argv[0]); int newArgc = 1;
        for (int ia = 1; ia < argc; ia++) {
            string a = argv[ia];
            string v = (ia + 1 < argc) ? argv[ia + 1] : "";
            if (a == "--steer")      { steerFile = v; ia++; continue; }
            if (a == "--out")        { outName = v;   ia++; continue; }
            if (a == "--json")       { jsonName = v;  ia++; continue; }
            if (a == "--tag")        { tag = v;       ia++; continue; }
            if (a == "--maxevents")  { maxEvents = atol(v.c_str()); ia++; continue; }
            newArgv.push_back(argv[ia]); newArgc++;
        }
        char** cleanArgv = newArgv.data();
        H1StdCmdLine opts; opts.Parse(&newArgc, cleanArgv);
    }
    if (steerFile.empty() || outName.empty()) {
        cerr << "usage: McCandidateTruthProbe --steer S --out O.root [--json J] [--tag T] [--maxevents N]" << endl;
        return 2;
    }
    cout << "McCandidateTruthProbe[" << tag << "]: steering " << steerFile << endl;
    gH1SteerManager->ParseFile(steerFile.c_str());
    H1Tree::Instance()->Open();
    cout << "McCandidateTruthProbe[" << tag << "]: isMC=" << gH1Tree->IsMC() << endl;

    H1PartMCArrayPtr partmc("MCParticles");
    H1GkiInfoArrayPtr gkiArr(0);
    Bool_t gkiOk = gkiArr.SetPointer("GkiInfo");
    if (!gkiOk) cout << "McCandidateTruthProbe[" << tag << "]: no GkiInfo branch" << endl;

    H1IntPtr   hRun("RunNumber"), hEvent("EventNumber");
    H1FloatPtr hW1("Weight1"), hW2("Weight2");
    H1ShortPtr hGenRad("GenRad");
    H1FloatPtr hQ2Gki("Q2Gki"), hYGki("YGki"), hXGki("XGki"), hXpomGki("XpomGki"), hTGki("TGki");
    H1FloatPtr hMXGen("MXGen"), hMYGen("MYGen"), hPt2Hat("Pt2HatGki"), hSHat("SHatGki");
    H1FloatPtr hQ2sGen("Q2sGen"), hYsGen("YsGen");

    TFile* outFile = new TFile(outName.c_str(), "RECREATE");
    if (!outFile || outFile->IsZombie()) { cerr << "cannot open " << outName << endl; return 3; }
    TTree* tr = new TTree("probe", "WP-D MC candidate truth probe");

    Int_t run, event; Float_t w1, w2; Short_t genrad;
    Float_t q2gki, ygki, xgki, xpom, tgki, mx, my, pt2hat, shat, q2s, ys;
    Int_t irad, pdfid1, pdfid2; Float_t x1, x2, xpdf1, xpdf2, scale1, wtx1, wtx2, wtx3, sigmt;
    Float_t gpt2hat, gshat, gxp, gzp, gxpom; Int_t nhfhad, hfhad1, hfhad2, hfhad3; Float_t zhfhad1;
    Int_t nPart, nStable, nCharmHad, nBeautyHad, nCharmQ, nBottomQ, nGluon, nStrangeQ;
    Int_t firstPdg[kNFirst], firstStat[kNFirst], firstMo1[kNFirst];
    Int_t hasD0, hasDch, hasDstar, hasDs, hasLambdaC;

    tr->Branch("run", &run, "run/I"); tr->Branch("event", &event, "event/I");
    tr->Branch("w1", &w1, "w1/F"); tr->Branch("w2", &w2, "w2/F"); tr->Branch("genrad", &genrad, "genrad/S");
    tr->Branch("q2gki", &q2gki, "q2gki/F"); tr->Branch("ygki", &ygki, "ygki/F"); tr->Branch("xgki", &xgki, "xgki/F");
    tr->Branch("xpom", &xpom, "xpom/F"); tr->Branch("tgki", &tgki, "tgki/F");
    tr->Branch("mx", &mx, "mx/F"); tr->Branch("my", &my, "my/F");
    tr->Branch("pt2hat", &pt2hat, "pt2hat/F"); tr->Branch("shat", &shat, "shat/F");
    tr->Branch("q2s", &q2s, "q2s/F"); tr->Branch("ys", &ys, "ys/F");
    tr->Branch("irad", &irad, "irad/I"); tr->Branch("pdfid1", &pdfid1, "pdfid1/I"); tr->Branch("pdfid2", &pdfid2, "pdfid2/I");
    tr->Branch("x1", &x1, "x1/F"); tr->Branch("x2", &x2, "x2/F"); tr->Branch("xpdf1", &xpdf1, "xpdf1/F"); tr->Branch("xpdf2", &xpdf2, "xpdf2/F");
    tr->Branch("scale1", &scale1, "scale1/F");
    tr->Branch("wtx1", &wtx1, "wtx1/F"); tr->Branch("wtx2", &wtx2, "wtx2/F"); tr->Branch("wtx3", &wtx3, "wtx3/F");
    tr->Branch("sigmt", &sigmt, "sigmt/F");
    tr->Branch("gpt2hat", &gpt2hat, "gpt2hat/F"); tr->Branch("gshat", &gshat, "gshat/F");
    tr->Branch("gxp", &gxp, "gxp/F"); tr->Branch("gzp", &gzp, "gzp/F"); tr->Branch("gxpom", &gxpom, "gxpom/F");
    tr->Branch("nhfhad", &nhfhad, "nhfhad/I"); tr->Branch("hfhad1", &hfhad1, "hfhad1/I");
    tr->Branch("hfhad2", &hfhad2, "hfhad2/I"); tr->Branch("hfhad3", &hfhad3, "hfhad3/I"); tr->Branch("zhfhad1", &zhfhad1, "zhfhad1/F");
    tr->Branch("nPart", &nPart, "nPart/I"); tr->Branch("nStable", &nStable, "nStable/I");
    tr->Branch("nCharmHad", &nCharmHad, "nCharmHad/I"); tr->Branch("nBeautyHad", &nBeautyHad, "nBeautyHad/I");
    tr->Branch("nCharmQ", &nCharmQ, "nCharmQ/I"); tr->Branch("nBottomQ", &nBottomQ, "nBottomQ/I");
    tr->Branch("nStrangeQ", &nStrangeQ, "nStrangeQ/I"); tr->Branch("nGluon", &nGluon, "nGluon/I");
    tr->Branch("firstPdg", firstPdg, "firstPdg[16]/I"); tr->Branch("firstStat", firstStat, "firstStat[16]/I");
    tr->Branch("firstMo1", firstMo1, "firstMo1[16]/I");
    tr->Branch("hasD0", &hasD0, "hasD0/I"); tr->Branch("hasDch", &hasDch, "hasDch/I"); tr->Branch("hasDstar", &hasDstar, "hasDstar/I");
    tr->Branch("hasDs", &hasDs, "hasDs/I"); tr->Branch("hasLambdaC", &hasLambdaC, "hasLambdaC/I");

    // summary accumulators
    long n = 0, nCharmEv = 0, nBeautyEv = 0, nHfhadEv = 0, nPt2Phys = 0, nCharmQEv = 0;
    long nQ2gt150 = 0, nQ2gt150Charm = 0, nQ2gt150Pt2Phys = 0;
    double sw = 0., swQ2gt150 = 0., swQ2gt150Pt2Phys = 0., swCharmEv = 0.;
    double pt2min = 1e30, pt2max = -1e30, sigmtFirst = -1.;
    map<int, long> pdfid1Hist, pdfid2Hist, pdfid2HistQ2gt150, hfhadHist, statHist;
    map<string, long> classHist;  // (pdfid2 kind, pt2hat physical) class

    while (H1Tree::Instance()->Next()) {
        if (n >= maxEvents) break;
        n++;
        run = *hRun; event = *hEvent; w1 = *hW1; w2 = *hW2; genrad = *hGenRad;
        q2gki = *hQ2Gki; ygki = *hYGki; xgki = *hXGki; xpom = *hXpomGki; tgki = *hTGki;
        mx = *hMXGen; my = *hMYGen; pt2hat = *hPt2Hat; shat = *hSHat; q2s = *hQ2sGen; ys = *hYsGen;
        irad = -999; pdfid1 = -999; pdfid2 = -999; x1 = x2 = xpdf1 = xpdf2 = scale1 = -999.;
        wtx1 = wtx2 = wtx3 = sigmt = -999.; gpt2hat = gshat = gxp = gzp = gxpom = -999.;
        nhfhad = -999; hfhad1 = hfhad2 = hfhad3 = 0; zhfhad1 = -999.;
        if (gkiOk && gkiArr.GetEntries() > 0 && gkiArr[0]) {
            H1GkiInfo* g = gkiArr[0];
            irad = g->GetIRAD(); pdfid1 = g->GetPDFID1(); pdfid2 = g->GetPDFID2();
            x1 = g->GetX1(); x2 = g->GetX2(); xpdf1 = g->GetXPDF1(); xpdf2 = g->GetXPDF2(); scale1 = g->GetSCALE1();
            wtx1 = g->GetWTX1(); wtx2 = g->GetWTX2(); wtx3 = g->GetWTX3(); sigmt = g->GetSIGMT();
            gpt2hat = g->GetPT2HAT(); gshat = g->GetSHAT(); gxp = g->GetXP(); gzp = g->GetZP(); gxpom = g->GetXPOM();
            nhfhad = (Int_t) g->GetNHFHAD(); hfhad1 = (Int_t) g->GetHFHAD1(); hfhad2 = (Int_t) g->GetHFHAD2();
            hfhad3 = (Int_t) g->GetHFHAD3(); zhfhad1 = g->GetZHFHAD1();
            if (sigmtFirst < 0. && sigmt > 0.) sigmtFirst = sigmt;
        }
        nPart = partmc.GetEntries();
        nStable = nCharmHad = nBeautyHad = nCharmQ = nBottomQ = nGluon = nStrangeQ = 0;
        hasD0 = hasDch = hasDstar = hasDs = hasLambdaC = 0;
        for (Int_t k = 0; k < kNFirst; k++) { firstPdg[k] = 0; firstStat[k] = -999; firstMo1[k] = -999; }
        for (Int_t i = 0; i < nPart; i++) {
            H1PartMC* p = partmc[i];
            if (!p) continue;
            Int_t pdg = p->GetPDG(); Int_t st = p->GetStatus(); Int_t a = abs(pdg);
            if (i < kNFirst) { firstPdg[i] = pdg; firstStat[i] = st; firstMo1[i] = p->GetMother1(); }
            statHist[st]++;
            if (st == 0) nStable++;
            if (a == 4) nCharmQ++;
            if (a == 5) nBottomQ++;
            if (a == 3) nStrangeQ++;
            if (a == 21) nGluon++;
            Int_t hf = HeavyFlavourOf(pdg);
            if (hf == 4) { nCharmHad++;
                if (a == 421) hasD0 = 1; if (a == 411) hasDch = 1; if (a == 413 || a == 423) hasDstar = 1;
                if (a == 431) hasDs = 1; if (a == 4122) hasLambdaC = 1; }
            if (hf == 5) nBeautyHad++;
        }
        tr->Fill();

        // summaries
        double w = (double) w1 * (double) w2; sw += w;
        bool pt2phys = pt2hat > -1000.;
        if (pt2phys) { nPt2Phys++; if (pt2hat < pt2min) pt2min = pt2hat; if (pt2hat > pt2max) pt2max = pt2hat; }
        if (nCharmHad > 0) { nCharmEv++; swCharmEv += w; }
        if (nBeautyHad > 0) nBeautyEv++;
        if (nhfhad > 0) nHfhadEv++;
        if (nCharmQ > 0) nCharmQEv++;
        pdfid1Hist[pdfid1]++; pdfid2Hist[pdfid2]++;
        if (nhfhad > 0) hfhadHist[hfhad1]++;
        string cls = string(pt2phys ? "ME2to2" : "QPMsent") + "_p2=" + (pdfid2 == 21 ? "g" : (abs(pdfid2) <= 6 ? "q" : "?"));
        classHist[cls]++;
        if (q2gki > 150.) { nQ2gt150++; swQ2gt150 += w; pdfid2HistQ2gt150[pdfid2]++;
            if (nCharmHad > 0) nQ2gt150Charm++; if (pt2phys) { nQ2gt150Pt2Phys++; swQ2gt150Pt2Phys += w; } }
    }
    outFile->Write(); outFile->Close(); delete outFile;

    // JSON summary
    ostream* js = &cout; ofstream jf;
    if (!jsonName.empty()) { jf.open(jsonName.c_str()); js = &jf; }
    *js << "{\n  \"tag\": \"" << tag << "\", \"steer\": \"" << steerFile << "\", \"n\": " << n
        << ", \"sum_w\": " << setprecision(12) << sw << ", \"sigmt_first\": " << sigmtFirst
        << ",\n  \"n_charm_hadron_events\": " << nCharmEv << ", \"sum_w_charm_hadron_events\": " << swCharmEv
        << ", \"n_beauty_hadron_events\": " << nBeautyEv << ", \"n_charm_quark_line_events\": " << nCharmQEv
        << ", \"n_gki_nhfhad_gt0\": " << nHfhadEv
        << ",\n  \"n_pt2hat_physical\": " << nPt2Phys << ", \"pt2hat_min\": " << (nPt2Phys ? pt2min : -1.)
        << ", \"pt2hat_max\": " << (nPt2Phys ? pt2max : -1.)
        << ",\n  \"n_q2gki_gt150\": " << nQ2gt150 << ", \"sum_w_q2gki_gt150\": " << swQ2gt150
        << ", \"n_q2gki_gt150_charm\": " << nQ2gt150Charm << ", \"n_q2gki_gt150_pt2phys\": " << nQ2gt150Pt2Phys
        << ", \"sum_w_q2gki_gt150_pt2phys\": " << swQ2gt150Pt2Phys << ",\n";
    auto dumpMap = [&](const char* name, map<int, long>& m, bool last) {
        *js << "  \"" << name << "\": {"; bool first = true;
        for (map<int, long>::iterator it = m.begin(); it != m.end(); ++it) {
            *js << (first ? "" : ", ") << "\"" << it->first << "\": " << it->second; first = false; }
        *js << "}" << (last ? "\n" : ",\n"); };
    dumpMap("pdfid1_hist", pdfid1Hist, false);
    dumpMap("pdfid2_hist", pdfid2Hist, false);
    dumpMap("pdfid2_hist_q2gt150", pdfid2HistQ2gt150, false);
    dumpMap("hfhad1_hist", hfhadHist, false);
    dumpMap("status_hist", statHist, false);
    *js << "  \"class_hist\": {"; bool first = true;
    for (map<string, long>::iterator it = classHist.begin(); it != classHist.end(); ++it) {
        *js << (first ? "" : ", ") << "\"" << it->first << "\": " << it->second; first = false; }
    *js << "}\n}\n";
    if (jf.is_open()) jf.close();
    cout << "McCandidateTruthProbe[" << tag << "]: done, n=" << n << " charmEv=" << nCharmEv
         << " pt2phys=" << nPt2Phys << " q2>150=" << nQ2gt150 << endl;
    return 0;
}

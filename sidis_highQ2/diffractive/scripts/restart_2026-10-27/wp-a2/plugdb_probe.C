// WP-A2 S1 (restart 2026-10-27): read the run-dependent H1CalibPlug DB entry
// (Thresh, EnCor) the HAT Plug filler uses (H1OO/H1HatFiller/H1HatCaloFiller.C:243-244,
// 278-279) for a list of runs, from the release DB snapshot (H1DBManager::OpenFile:
// $H1ROOTDB or $H1DIST/releases/$H1CURRENT/share/H1OOSnapshot.root).
// Input: CSV "run,period_or_sample,is_mc" (header line skipped).
// Output: CSV "run,period_or_sample,is_mc,thresh,encor,first_run,last_run,found".
// The DB entry is run-dependent only (no MC/data flag): MC events carry real
// run numbers, so an MC row is the entry of the run the MC event is assigned to.
// Usage (login node, < 1 min):
//   root -l -b -q 'plugdb_probe.C("runs.csv","plug_calib.csv")'
#include <fstream>
#include <sstream>
#include <iostream>
#include <string>
#include "TSystem.h"
R__ADD_INCLUDE_PATH($H1DIST/releases/$H1CURRENT)
R__LOAD_LIBRARY(libEG)
R__LOAD_LIBRARY(libGeom)
R__LOAD_LIBRARY(libPhysics)
R__LOAD_LIBRARY(libH1Steering)
R__LOAD_LIBRARY(libH1Skeleton)
R__LOAD_LIBRARY(libH1Geom)
#include "H1Geom/H1DBManager.h"
#include "H1Geom/H1CalibPlug.h"

void plugdb_probe(const char* in, const char* out)
{
  std::ifstream fin(in); std::ofstream fout(out);
  fout << "run,period_or_sample,is_mc,thresh,encor,first_run,last_run,found\n";
  std::string line; std::getline(fin, line);
  int nbad = 0;
  while (std::getline(fin, line)) {
    if (line.empty()) continue;
    std::stringstream ss(line); std::string srun, lab, ismc;
    std::getline(ss, srun, ','); std::getline(ss, lab, ','); std::getline(ss, ismc, ',');
    Int_t run = std::stoi(srun);
    H1CalibPlug* c = static_cast<H1CalibPlug*>(H1DBManager::Instance()->GetDBEntry(H1CalibPlug::Class(), run));
    if (!c) { fout << run << "," << lab << "," << ismc << ",nan,nan,,,0\n"; ++nbad; continue; }
    fout << run << "," << lab << "," << ismc << "," << c->GetThresh() << "," << c->GetEnCor()
         << "," << c->GetFirstRun() << "," << c->GetLastRun() << ",1\n";
  }
  fout.close();
  std::cout << "plugdb_probe: wrote " << out << " (" << nbad << " runs without entry)" << std::endl;
  if (nbad) gSystem->Exit(3);
}

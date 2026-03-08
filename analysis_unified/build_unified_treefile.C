#include <TFile.h>
#include <TTree.h>
#include <TKey.h>
#include <TClass.h>
#include <TDirectory.h>
#include <iostream>
#include <string>

namespace {

void copyTopLevelTrees(TFile &src, TDirectory *dest, const std::string &tag) {
  if (!dest) return;
  TIter next(src.GetListOfKeys());
  TKey *key = nullptr;
  while ((key = (TKey*)next())) {
    const char *className = key->GetClassName();
    TClass *cls = TClass::GetClass(className);
    if (!cls || !cls->InheritsFrom(TTree::Class())) {
      continue;
    }

    TTree *tree = (TTree*)key->ReadObj();
    if (!tree) continue;

    dest->cd();
    TTree *cloned = tree->CloneTree(-1, "fast");
    cloned->Write(tree->GetName(), TObject::kOverwrite);

    std::cout << "[build_unified_treefile] copied tree '" << tree->GetName()
              << "' into " << tag << std::endl;
  }
}

}

void build_unified_treefile(const char *dataRecFile,
                            const char *mcRecFile,
                            const char *mcGenFile,
                            const char *outFile = "UnifiedTrees.root") {
  TFile fData(dataRecFile, "READ");
  TFile fMcRec(mcRecFile, "READ");
  TFile fMcGen(mcGenFile, "READ");

  if (fData.IsZombie()) {
    std::cerr << "Could not open data rec file: " << dataRecFile << std::endl;
    return;
  }
  if (fMcRec.IsZombie()) {
    std::cerr << "Could not open mc rec file: " << mcRecFile << std::endl;
    return;
  }
  if (fMcGen.IsZombie()) {
    std::cerr << "Could not open mc gen file: " << mcGenFile << std::endl;
    return;
  }

  TFile fout(outFile, "RECREATE");
  if (fout.IsZombie()) {
    std::cerr << "Could not create output file: " << outFile << std::endl;
    return;
  }

  TDirectory *dataDir = fout.mkdir("data");
  TDirectory *mcDir = fout.mkdir("mc");
  TDirectory *dataRecDir = dataDir ? dataDir->mkdir("rec") : nullptr;
  TDirectory *mcRecDir = mcDir ? mcDir->mkdir("rec") : nullptr;
  TDirectory *mcGenDir = mcDir ? mcDir->mkdir("gen") : nullptr;

  copyTopLevelTrees(fData, dataRecDir, "data/rec");
  copyTopLevelTrees(fMcRec, mcRecDir, "mc/rec");
  copyTopLevelTrees(fMcGen, mcGenDir, "mc/gen");

  fout.Write();
  fout.Close();

  std::cout << "[build_unified_treefile] wrote unified tree file: " << outFile << std::endl;
}

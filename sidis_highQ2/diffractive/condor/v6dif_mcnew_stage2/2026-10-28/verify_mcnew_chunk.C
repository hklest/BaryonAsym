// verify_mcnew_chunk.C: per-chunk check. root -l -b -q 'verify_mcnew_chunk.C("evls.root","sidistree.root")'
// prints: VERIFY|evls|difftot|reco|sumw1|odsLoadedN|nflag|flagged(name:frac;...)|wmin|wmax
// sentinel = scalar F/D branch with value < -998 on passRecoEnv events; flagged if fraction > 5%.
#include "TFile.h"
#include "TTree.h"
#include "TBranch.h"
#include "TLeaf.h"
#include <vector>
#include <string>
#include <cstdio>
void verify_mcnew_chunk(const char *evls, const char *st) {
  Long64_t ne=-1, nd=-1, nreco=0, nods=0; double sw=0, wmin=1e30, wmax=-1e30; std::string flag; int nflag=0;
  TFile *fe = TFile::Open(evls); if (fe && !fe->IsZombie()) { TTree *t=(TTree*)fe->Get("EvlTree"); if (t) ne=t->GetEntries(); }
  TFile *f = TFile::Open(st);
  if (f && !f->IsZombie()) {
    TTree *t=(TTree*)f->Get("DiffTot");
    if (t) {
      nd=t->GetEntries();
      std::vector<TBranch*> bs; std::vector<std::string> nm; std::vector<char> ty;
      std::vector<Float_t> fv; std::vector<Double_t> dv; // stable storage
      std::vector<Long64_t> cnt;
      TObjArray *bl=t->GetListOfBranches(); int nb=bl->GetEntries();
      fv.assign(nb,0); dv.assign(nb,0); cnt.assign(nb,0); ty.assign(nb,0);
      Bool_t reco=0, ods=0; Float_t w=1;
      t->SetBranchStatus("*",0);
      for (int i=0;i<nb;i++){ TBranch*b=(TBranch*)bl->At(i); std::string n=b->GetName(); TLeaf*l=(TLeaf*)b->GetListOfLeaves()->At(0);
        if(!l||l->GetLeafCount()||l->GetLen()!=1) continue; std::string tn=l->GetTypeName();
        if(n=="passRecoEnv"){t->SetBranchStatus(n.c_str(),1);t->SetBranchAddress(n.c_str(),&reco);}
        else if(n=="odsLoaded"){t->SetBranchStatus(n.c_str(),1);t->SetBranchAddress(n.c_str(),&ods);}
        else if(n=="weight1"){t->SetBranchStatus(n.c_str(),1);t->SetBranchAddress(n.c_str(),&w);}
        else if(tn=="Float_t"){ty[i]='f';t->SetBranchStatus(n.c_str(),1);t->SetBranchAddress(n.c_str(),&fv[i]);}
        else if(tn=="Double_t"){ty[i]='d';t->SetBranchStatus(n.c_str(),1);t->SetBranchAddress(n.c_str(),&dv[i]);}
      }
      if(!t->GetBranch("weight1")) wmin=wmax=-1;
      for (Long64_t e=0;e<nd;e++){ t->GetEntry(e); sw+=w; if(w<wmin)wmin=w; if(w>wmax)wmax=w; if(ods)nods++;
        if(!reco) continue; nreco++;
        for(int i=0;i<nb;i++){ if(ty[i]=='f'&&fv[i]<-998)cnt[i]++; else if(ty[i]=='d'&&dv[i]<-998)cnt[i]++; } }
      for(int i=0;i<nb;i++) if(ty[i]&&nreco>0&&cnt[i]>0.05*nreco){nflag++; char buf[200]; snprintf(buf,200,"%s:%.3f;",bl->At(i)->GetName(),(double)cnt[i]/nreco); flag+=buf;}
    }
  }
  printf("VERIFY|%lld|%lld|%lld|%.6f|%lld|%d|%s|%.5f|%.5f\n",ne,nd,nreco,sw,nods,nflag,flag.empty()?"-":flag.c_str(),wmin,wmax);
}

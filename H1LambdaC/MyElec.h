#ifndef __MYELEC_H
#define __MYELEC_H

#include <TLorentzVector.h>

class MyElec //: public TObject
{

protected:
	MyElec();
	~MyElec();

public:
	static TLorentzVector CalcElecForBoost(Float_t, Float_t, Float_t);
	static bool VertexCuts(Double_t,Double_t,const Double_t,const Double_t,const Double_t,const Double_t);
	static bool FiducialCut(Double_t , Double_t, Int_t);
	static bool ElecCluster(Float_t  elecRClus, Float_t elecEHadSpac, const Double_t elecRClusMax, const Double_t elecEHadrMax);
	static int Id(); 
	static bool NCIbg_Cuts(Double_t );
	static Float_t Prescale(Double_t, Int_t , Int_t , Int_t, const Float_t, const Float_t );
	static Int_t Chooser(Double_t, const Int_t , const Double_t BinArray[] );
	static bool TechCuts(Int_t, const Int_t, const Int_t);// unused

};

#endif

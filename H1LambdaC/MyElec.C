#include "MyElec.h"
//#include "MyEvent_3_3.h"

#include "H1Calculator/H1Calculator.h"

#include "H1Geom/H1DBManager.h"

#include "H1Mods/H1PartEmArrayPtr.h"
#include <H1Mods/H1PartEm.h>

#include "H1Mods/H1PartMuon.h"
#include "H1Mods/H1PartMuonArrayPtr.h"

#include "H1Pointers/H1Ptr.h"

#include <TLorentzVector.h>

#include "H1Pointers/H1FloatPtr.h"   //HAT variables
#include "H1Pointers/H1BytePtr.h"    //HAT variables

#include "H1Calculator/H1CalcKine.h"
#include "H1Calculator/H1CalcTrig.h"

//Double_t Rspac = 0.;


// extern  const Int_t     NBDCHits,
// 			nq2bins,
// 			nq2Rat,
// 			nq23j,
// 			nq23jRat;

// extern  const Double_t  q2bins[],
// 			q2binRat[],
// 			q23j[],
// 			q23jRat[];

//_____________________________________________________________________
MyElec::MyElec()
{   // Constructor
}
//_____________________________________________________________________
MyElec::~MyElec()
{ // Destructor
}


//_____________________________________________________________________
// static
int MyElec::Id()
{
	static 	H1PartEmArrayPtr elecs("EmParticles");	
	int  ScatElecID = 0;
	if(!(elecs -> GetEntries() >= 1)) return -10 ;

	///     Scattering Electron ID idetification
	for (int i = 0; i < elecs -> GetEntries(); ++i){
		if ( elecs[i]->IsScatElec() ) ScatElecID = i;
	}

	///	return ScatElecID ;  /// to include Anti Compton cut comment this line

	///     Anti Compton cut 
	/*	Double_t ScatElEn,  ElEn;
	  Double_t ScatElPhi, ElPhi;
	  if (elecs -> GetEntries() >= 2) {
	  ScatElEn  = elecs[ScatElecID]-> GetE();
	  ScatElPhi = elecs[ScatElecID]-> GetPhi();
		for( int ij = 0; ij < (elecs -> GetEntries()); ij++){
		if(ij == ScatElecID) continue;
		ElEn  = elecs[ij]-> GetE();
		ElPhi = elecs[ij]-> GetPhi();
		if(((ScatElEn+ElEn) > 18.)&&
		(-cos(abs(ScatElPhi - ElPhi)) > 0.95)&&
		((ScatElEn > 4.) && (ElEn > 4.)))  return -20 ;
		}
		}
	*/

	///     Anti Compton cut
	Double_t esel[2];
	Double_t phsel[2];
        if (elecs->GetEntries() >= 2) {
		for( int ij = 0; ij<2; ij++){
			esel[ij]  = elecs[ij]-> GetE();
			phsel[ij] = elecs[ij]-> GetPhi();
		}
		if(((esel[0]+esel[1]) > 18.)&&
		(-cos(fabs(phsel[0]- phsel[1])) > 0.95) &&
		((esel[0] > 4.) && (esel[1] > 4.)))  return -20 ;
	}
	return ScatElecID ;
}



///_____________________________________________________________________
// static
bool MyElec::VertexCuts(Double_t VtxZ, Double_t ElecTheta_e, const Double_t VtxZMax, const Double_t VtxZMin, const Double_t R_Theta_Min, const Double_t czSpacal )
{
	if(!(VtxZ < VtxZMax && VtxZ > VtxZMin) ) return false ; 
	Float_t R_Theta = fabs((VtxZ - czSpacal)*TMath::Sin(ElecTheta_e)/cos(ElecTheta_e));	
	if(!(R_Theta > R_Theta_Min)) return false;
	return true ;
}

///_____________________________________________________________________
// static
bool MyElec::ElecCluster(Float_t  elecRClus, Float_t elecEHadSpac, const Double_t ElecRClusMax, const Double_t ElecEHadrMax)
{
	if( elecRClus > ElecRClusMax ) return false;
	if( elecEHadSpac > ElecEHadrMax ) return false;	
	return true ;
}
/*
///_____________________________________________________________________
bool MyElec::Veto(Int_t pointe)  ///  for HERA-I only
{
	Double_t 	elecEVeto = elecs[pointe]  -> GetEnVeto();
	if (elecEVeto  > elecEVetoMax)  return false;
	return  true ;
}
*/
///_____________________________________________________________________
 // static
bool MyElec::TechCuts(Int_t pointe, const Int_t cElecRBDC, const Int_t cNBDCHits) ///  BPC give bad LUMI/YIELD RATIO  
{
	static 	H1PartEmArrayPtr elecs("EmParticles");	
	Float_t  elecRbdc = elecs[pointe]  -> GetBcRsp(); //Sp.clus&BDC hits dist.extrap.
	if (elecRbdc > cElecRBDC)  return false ;
	Int_t elecNBRbdc = elecs[pointe] ->GetNBcHit();   //Number of hits in BDC track
	if (elecNBRbdc < cNBDCHits) return false ;
	return true ;
}

///_______________________________________________________________________
// static
Int_t MyElec::Chooser(Double_t Variable, const Int_t nBinNumber, const Double_t BinArray[] )
{
	for (Int_t i = 0; i<nBinNumber; i++){
		if ( Variable>=BinArray[i] && Variable<BinArray[i+1] )
			return i;
	}
	return nBinNumber;
}

///_____________________________________________________________________
//	static H1IntPtr Ibgfm("Ibgfm");
// static
bool MyElec::NCIbg_Cuts(Double_t PtBal)
{
	///   A.Nikifirov thesys http://www-h1.desy.de/psfiles/theses/h1th-469.pdf    p. 112
	///     Bg-safe cuts (*Ibg) only don't improve E-Pz behaviour at E-Pz > 65 for 4 Jets.
	Int_t   BgSafe[9] = {0};
	static H1IntPtr Ibg("Ibg"); 

        for(Int_t j =0; j< 9; j++) BgSafe[j] = 0;

	if(PtBal < 0.5 ){
    		for(int i=5; i<7; i++){
		   if((*Ibg)&(1<<i)) return false;
		}
    	}

	if(PtBal < 0.1 ){
		if((*Ibg)&(1<<7)) return false;
	}
	else{
    		for(int i=0; i<9; i++){
			if(i == 5 || i == 6 || i == 7 ) continue;
    			if((*Ibg)&(1<<i)){
				if(i < 2) return false;
				else BgSafe[i] = 1;
			}
		}
		if((BgSafe[2]+BgSafe[3]+BgSafe[4]+BgSafe[8]) >=2) return false;
	}
	
	return true;
}

///_____________________________________________________________________
// static
Float_t MyElec::Prescale(Double_t Q2es, Int_t RunYear, Int_t SigMain, Int_t SigAdd, const Float_t cTrigQ2_10, const Float_t cTrigQ2_25)
{
	H1TrigInfo* trig = static_cast<H1TrigInfo*> (H1DBManager::Instance()->GetDBEntry(H1TrigInfo::Class()));
  
	//if(gH1Calc -> Trig() -> GetL1rw(61))cout<<"Run Numver = "<< gH1Calc->GetRunNumber()<<"     S(61) = "<<gH1Calc -> Trig() -> GetTrigac(61) << "     Prescale = " << trig ->GetPrescale(61)<<endl;
	//if(gH1Calc -> Trig() -> GetL1rw(0)) cout<<"S(0) = "<<gH1Calc -> Trig() -> GetTrigac(0) << "     Prescale = " << trig ->GetPrescale(0)<<endl;
	//if(gH1Calc -> Trig() -> GetL1rw(3)) cout<<"S(3) = "<<gH1Calc -> Trig() -> GetTrigac(3) << "     Prescale = " << trig ->GetPrescale(3)<<endl;
	//if(gH1Calc -> Trig() -> GetL1rw(SigMain)) cout<<"S("<<SigMain<<") = "<<gH1Calc -> Trig() -> GetTrigac(SigMain) << "     Prescale = " << trig ->GetPrescale(SigMain)<<endl;
	//if(gH1Calc -> Trig() -> GetL1rw(SigAdd)) cout<<"S("<<SigAdd<<") = "<<gH1Calc -> Trig() -> GetTrigac(SigAdd) << "     Prescale = " << trig ->GetPrescale(SigAdd)<<endl;
  
	if ( Q2es < cTrigQ2_10 ){ 
		//	if(  gH1Calc -> Trig() -> GetL1ac(61)) return trig ->GetPrescale(61);
		if(  gH1Calc -> Trig() -> GetTrigac(61) ) return trig ->GetPrescale(61);
		if( !(gH1Calc -> Trig() -> GetL1rw(61))&&(gH1Calc->Trig()->GetTrigac(0)) )return trig ->GetPrescale(0);
		//	if(!(gH1Calc -> Trig() -> GetL1rw(61))&&(gH1Calc->Trig()->GetTrigac(0)))return trig ->GetPrescale(0)/(trig ->GetPrescale(61));
	}

	if ( (Q2es >= cTrigQ2_10)&& (Q2es < cTrigQ2_25)) {  
		if(RunYear == 8) { ////  For 2005 there is now Trig. S2
			if(gH1Calc -> Trig() -> GetTrigac(0)) return trig ->GetPrescale(0);
		}
		if(RunYear >= 9) {
			if(gH1Calc -> Trig() -> GetTrigac(SigMain))  return trig ->GetPrescale(SigMain);
			//if(!((gH1Calc->Trig()->GetL1rw(SigMain))&&(gH1Calc->Trig()->GetL1L3rw(SigMain))) &&(gH1Calc->Trig()->GetTrigac(SigAdd)))
			if(!((gH1Calc->Trig()->GetL1rw(SigMain))&&(gH1Calc->Trig()->GetL1L2rw(SigMain))) &&(gH1Calc->Trig()->GetTrigac(SigAdd)))
				return trig ->GetPrescale(SigAdd); 
		}
	}
	if ( Q2es >= cTrigQ2_25 ) {  
		if(gH1Calc  ->Trig()->GetTrigac(3)) return trig ->GetPrescale(3);
		//if(!((gH1Calc->Trig()->GetL1rw(3))&&(gH1Calc->Trig()->GetL1L3rw(3)))) {
		if(!((gH1Calc->Trig()->GetL1rw(3))&&(gH1Calc->Trig()->GetL1L2rw(3)))) {
			if(RunYear == 8) { ////  For 2005 there is now Trig. S2
				if(gH1Calc->Trig()->GetTrigac(0))return trig ->GetPrescale(0);
			}
			else if(RunYear >= 9) {
				if(gH1Calc->Trig()->GetTrigac(SigMain))return trig ->GetPrescale(SigMain);
				//if(!((gH1Calc->Trig()->GetL1rw(SigMain))&&(gH1Calc->Trig()->GetL1L3rw(SigMain)))&&(gH1Calc->Trig()->GetTrigac(SigAdd)) ) return trig->GetPrescale(SigAdd);
				if(!((gH1Calc->Trig()->GetL1rw(SigMain))&&(gH1Calc->Trig()->GetL1L2rw(SigMain)))&&(gH1Calc->Trig()->GetTrigac(SigAdd)) ) return trig->GetPrescale(SigAdd);
			}
		}
	}
	return 0.;
}



///_____________________________________________________________________
bool MyElec::FiducialCut(Double_t xclus, Double_t yclus, Int_t RunYear)
{
/// 	From Emmanuel Sauwan   LAST CUTS from inclusive diffractive analysis
	Double_t rspacal = TMath::Sqrt(xclus*xclus+yclus*yclus);
	///	Double_t elZClus = elecs[pointe] -> GetZClus();	

	///	if(FiduPlot) hFiduCutBF -> Fill(xclus, yclus, fWeight);
	if(RunYear < 0 || RunYear >= 7) {
		//---- (box removed from S3,S2,S9,S0) (seems to be the case)
		if(xclus>-16.2  &&  xclus<8.1    && yclus>-8.1    && yclus<16.2)   return false;
		//         if(rspacal<15||rspacal>70) return false;	//  SpaCal outer radius cut.   Sauwan
		//         if(rspacal<18||rspacal>74) return false;       //  SpaCal inner and outer radius cut.   ELAN

		if(rspacal<15||rspacal>74) return false;	//  SpaCal outer radius cut.   ELAN

	}
	
	// DB new cuts
	if ( RunYear < 0 || RunYear== 8 || RunYear== 9  ) { // new DB: 05e,06e, inefficient SpacalCluster at -55.5, -26.5
	   if(xclus>-54.5  &&  xclus<-56.5  && yclus>-25.5    && yclus<-27.5)    return false;
	}
	if ( RunYear < 0 || RunYear== 8 ) {// new DB: increase cut in xmax by 0.5cm
	   if(xclus>-24.3  &&  xclus<-20.75 && yclus >32     && yclus<40)     return false;
	   if(xclus>-28.   &&  xclus<-20.75 && yclus >32     && yclus<36)     return false;
	}

	if(RunYear < 0 || RunYear== 8) {  //--- new from compton analysis
		if(xclus>-15.0  &&  xclus<-10.0  && yclus>-5.0    && yclus<5.0)    return false;
		if(xclus>-12.5  &&  xclus<8.5    && yclus>-4.5    && yclus<4.5)    return false;
		if(xclus>-12.5  &&  xclus<-8.4   && yclus>-8.5    && yclus<-4.0)   return false;

		if(xclus>0.0    &&  xclus<17.0   && yclus>-5.0    && yclus<5.0)    return false;

		if(xclus>-28.35 &&  xclus<-24.3  && yclus >-52.65 && yclus<-44.55) return false;

		if(xclus>-28.35 &&  xclus<-24.3  && yclus >-52.65 && yclus<-44.55) return false;
		if(xclus>-36.45 &&  xclus<-32.4  && yclus >-36.45 && yclus<-32.4)  return false;
		if(xclus>-24.3  &&  xclus<-20.25 && yclus >32     && yclus<40)     return false;
		if(xclus>-28.   &&  xclus<-20.25 && yclus >32     && yclus<36)     return false;
		if(xclus>28.35  &&  xclus<32.4   && yclus >-24.3  && yclus<-20.25) return false;
		if(xclus>60.75  &&  xclus<64.8   && yclus >20.25  && yclus<24.5)   return false;
		if(xclus>12.15  &&  xclus<20.25  && yclus >4.05   && yclus<8.1)    return false;

		if(xclus>0.0    &&  xclus<20.25  && yclus>0.0     && yclus<8.1)    return false;

		//---- new 12/09/2006
		//---- central part
		if(xclus>-16.2  &&  xclus<20.25  && yclus >-4.05  && yclus<8.1)    return false;
		//---- another dead cells
		if(xclus>-28.35 &&  xclus<-24.3  && yclus >4.05   && yclus<8.1)    return false;
		if(xclus>64.8   &&  xclus<68.85  && yclus >-20.25 && yclus<-16.2)  return false;

		//---- special for S17/S41 ?
		//         if(xclus>-28.35 &&  xclus<-24.3  && yclus >40.5   && yclus<44.55)  return false;
		//         if(xclus>-32.4  &&  xclus<-28.35 && yclus >56.7   && yclus<60.75)  return false;

	}

	if(RunYear < 0 || RunYear==9) {
		//----enlarged central part
		if(xclus>-17    &&  xclus<9      && yclus>-9      && yclus<17)     return false;
		if(xclus>-28.35 &&  xclus<-24.3  && yclus >-52.65 && yclus<-44.55) return false;
		if(xclus>60.75  &&  xclus<64.8   && yclus >20.25  && yclus<24.5)   return false;
		if(xclus>64.8   &&  xclus<68.85  && yclus >-20.25 && yclus<-16.2)  return false;
		if(xclus>-68.85 &&  xclus<-64.8  && yclus >0.     && yclus<8.1)    return false;

		//---- central part
		if(xclus>-16.2  &&  xclus<16.2   && yclus >-4.05  && yclus<4.05)   return false;
		if(xclus>0.0    &&  xclus<17.0   && yclus>-5.0    && yclus<5.0)    return false;

		if(xclus>10.0   &&  xclus<15.0   && yclus>5       && yclus<8.0)    return false;

		//--new one 33
		if(xclus>10.0   &&  xclus<15.0   && yclus>16      && yclus<22)     return false;

	}

	if(RunYear < 0 || RunYear==10) {
		//----enlarged central part
		if(xclus>-17    &&  xclus<9      && yclus>-9      && yclus<17)     return false;

		if(xclus>-28.35 &&  xclus<-24.3  && yclus >-52.65 && yclus<-44.55) return false;
		if(xclus>60.75  &&  xclus<64.8   && yclus >20.25  && yclus<24.5)   return false;
		if(xclus>64.8   &&  xclus<68.85  && yclus >-20.25 && yclus<-16.2)  return false;
		if(xclus>-68.85 &&  xclus<-64.8  && yclus >0.     && yclus<8.1)    return false;

		//---- central part
		if(xclus>-16.2  &&  xclus<16.2   && yclus >-4.05  && yclus<4.05)   return false;
		if(xclus>0.0    &&  xclus<17.0   && yclus>-5.0    && yclus<5.0)    return false;
		//---- from 2006 e-p
		if(xclus>10.0   &&  xclus<15.0   && yclus>5       && yclus<8.0)    return false;

		//--new33
		if(xclus>5      &&  xclus<16.0   && yclus>8       && yclus<13.0)   return false;

		//---- from 0607DST1 data ...
		if(xclus>-10.0  &&  xclus<10.0   && yclus>-17     && yclus<-10.0)  return false;
	}
	//       if(FiduPlot)hFiduCut -> Fill(xclus, yclus, fWeight); 
	//Rspac = rspacal;
	return true; 
}



///_____________________________________________________________________
///Here the method CalcElecForBoost is defined as:
//static
//TLorentzVector MyElec::CalcElecForBoost(Float_t q2, Float_t y, Float_t Phi)
//{
//  // Calculate the scattered electron four-vector for the boost
//  // to the Breit frame. Reconstruct the vector from Q2, y and Phi_e,
//  // to be able to take a different reconstruction method
//
//  Double_t E0 = 27.6; // electron beam energy
//
//  Double_t px = 0;
//  Double_t py = 0;
//  Double_t pz = 0;
//  Double_t Ee = 0;
//
//  if ((q2>0) && (y>0) && (y<1)){
//
//    Ee = q2 / (4*E0) + E0*(1-y);
//
//    Double_t b = 4*E0*E0*(1-y)/q2;
//    Double_t Theta = TMath::ACos( (1-b)/(1+b) );
//
//    px = Ee*TMath::Sin(Theta)*TMath::Cos(Phi);
//    py = Ee*TMath::Sin(Theta)*TMath::Sin(Phi);
//    pz = Ee*TMath::Cos(Theta);
//
//  }
//
//  TLorentzVector ElecBoostVec(px, py, pz, Ee);
//
//  return ElecBoostVec;
//}

//int
//main(int argc, char* argv[])
//{}
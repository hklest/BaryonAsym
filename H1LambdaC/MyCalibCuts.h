////   ADDITIONAL CUTS and BINNINGS FOR CALIBRATION

	const Int_t 	NEEt_C          =       7;
	const Double_t  EEtMin_C        =       2.0,
//			EEtMax_C        =       8.0;
			EEtMax_C        =       9.0;

	const Int_t 	NEPt_C          =      78;
	const Double_t  EPtMin_C        =       2.0,
			EPtMax_C        =      80.0;
	
	const Float_t   ElecEminCalib  	=      12.;  /// Minimal Elec Energy
	const Float_t   ElecEmaxCalib  	=      21.;  /// Minimal Elec Energy
	const Float_t   ElecPtminCalib  =      4.;   /// Elec. Transv. Momenta min
	
	const Float_t   EpzMinCalib   	=      52.,
			EpzMaxCalib   	=      70.;
			
	const Float_t   fptmiss_Calib 	=       6.;	
	
	const Float_t   yMinCalib       =       0.2,		/// was 0!!!
			yMaxCalib       =       0.7;

/// SpaCal Cuts
///	const Float_t   elecRClusCalib 	=  	3.5;
	const Float_t   elecRClusCalib 	=  	4.5;
	
/// Ratios	
	const Float_t   fPtBalMinCalib	=  	1./1.4,  	///  was 0.5,
	   		fPtBalMaxCalib	=  	1./0.8;  	///  was 1.8;

	const Float_t   fPtHadRatCalib  =   	0.35;

#!/bin/zsh

####
#Variable Index: 0=Q2 Dist, 1=Y Dist, 2=pT Dist, 3=Eta Dist, 4=Phi Dist, 5=Radial Decay Vtx, 6=Z Decay Vtx
#Vector Defining Bins for Dist Variable
#Number of bin entries
#Data Lumi
#Radiative Gen MC Lumi
#Non-Radiative Gen MC Lumi
#Kaon Data Tree File
#Lambda Data Tree File
#Kaon MC Tree File
#Lambda MC RAPGAP Tree File
#Lambda MC DJANGOH Tree File
#RAPGAP Radiative Gen MC Tree File for Detector Efficiency
#DJANGOH Radiative Gen MC Tree File for Detector Efficiency
# Non-Radiative Gen MC Tree File for Radiative Corrections
# Radiative Gen MC Tree File for Radiative Corrections
#X axis title for plots
#Vector of parameter initializations for DATA in individual bins
#Vector of Fit/Signal Extraction ranges for individual bins (same for DATA and MC)
#Couts Copied to This File for Troubleshooting
#Copy generic output name to specific one based on inputs so we don't overwrite it
#Remove old output after it is copied to new file
#{-30,-28,-26,-24,-22,-20,-18,-16,-14,-12,-10,-8,-6,-4,-2,0,2,4,6,8,10,12,14,16,18,20,22,24,26,28,30}
#{-30,-27,-24,-21,-18,-15,-12,-9,-6,-3,0,3,6,9,12,15,18,21,24,27,30}
#######################################################################
#######################################################################
###2005-2007 pT
#Before calling root macro, need to write command to edit line defining num of bin entries
#C++ will not let this be a variable input since it defines array lengths
sed -i '35c\'"const int q2entries = 9;" "/data/dust/group/h1/klesthen/BaryonAsym/H1LambdaC/CrossSections.C"
root -l -q '/data/dust/group/h1/klesthen/BaryonAsym/H1LambdaC/CrossSections.C(
2,
{0.5, 0.6, 0.7, 0.8, 0.9, 1.1, 1.3, 1.6, 2.2, 3.5},
297657.8,
4108957.0,
3560233.0,											
"/data/dust/group/h1/klesthen/BaryonAsym/H1LambdaC/KaonTree/merged.root", 	
"/data/dust/group/h1/klesthen/BaryonAsym/H1LambdaC/LambdaTree/merged.root", 			
"/data/dust/group/h1/klesthen/BaryonAsym/H1LambdaC_MC/KaonTreeMC/RAPGAP31/merged.root", 		
"/data/dust/group/h1/klesthen/BaryonAsym/H1LambdaC_MC/LambdaTreeMC/RAPGAP31/merged.root",
"/data/dust/group/h1/klesthen/BaryonAsym/H1LambdaC_MC/LambdaTreeMC/DJANGOH14/merged.root",  		
"/data/dust/group/h1/klesthen/BaryonAsym/H1LambdaC_MC/genMCTree/RAPGAP31/Rad/Save/merged.root",
"/data/dust/group/h1/klesthen/BaryonAsym/H1LambdaC_MC/genMCTree/DJANGOH14/Rad/Save/merged.root", 		
"/data/dust/group/h1/klesthen/BaryonAsym/H1LambdaC_MC/genMCTree/DJANGOH14/NonRad/merged.root", 		
"/data/dust/group/h1/klesthen/BaryonAsym/H1LambdaC_MC/genMCTree/DJANGOH14/Rad/Save/merged.root",
"p_{T} [GeV]",
{{0.0018,3.0,1.1156},{0.0018,3.0,1.1156},{0.0018,3.0,1.1156},{0.0018,3.0,1.1156},{0.0018,3.0,1.1156},{0.0019,3.0,1.1156},{0.0019,4,1.1156},{0.0023,3.0,1.1156},{0.0023,2.0,1.1156}},
{{1.089,1.099,1.134,1.165},{1.089,1.099,1.134,1.165},{1.089,1.099,1.134,1.165},{1.089,1.099,1.134,1.165},{1.089,1.099,1.134,1.165},{1.09,1.096,1.137,1.16},{1.092,1.102,1.135,1.165},{1.095,1.093,1.14,1.165},{1.087,1.093,1.145,1.165}},
{{0.006,3.0,0.49761},{0.006,3.0,0.49761},{0.006,3.0,0.49761},{0.006,3.0,0.49761},{0.006,3.0,0.49761},{0.006,3.0,0.49761},{0.006,3.0,0.49761},{0.006,3.0,0.49761},{0.006,3.0,0.49761}},
{{0.35,0.4,0.58,0.69},{0.35,0.4,0.58,0.69},{0.35,0.4,0.58,0.69},{0.35,0.4,0.58,0.69},{0.35,0.4,0.58,0.69},{0.35,0.4,0.58,0.69},{0.35,0.4,0.58,0.69},{0.35,0.4,0.58,0.69},{0.35,0.41,0.62,0.69}},
{{0.0015,4.0,1.1156},{0.0015,4.0,1.1156},{0.0015,4.0,1.1156},{0.0015,4.0,1.1156},{0.0015,4.0,1.1156},{0.0018,3.0,1.1156},{0.0017,4.0,1.1156},{0.0017,2.0,1.1156},{0.0021,3.0,1.1156}},
{{1.083,1.096,1.138,1.165},{1.083,1.096,1.138,1.165},{1.083,1.096,1.138,1.165},{1.083,1.096,1.138,1.165},{1.083,1.096,1.138,1.165},{1.083,1.094,1.137,1.16},{1.083,1.105,1.1305,1.165},{1.083,1.10,1.134,1.165},{1.083,1.10,1.134,1.165}},
{{0.006,3.0,0.49761},{0.006,3.0,0.49761},{0.006,3.0,0.49761},{0.005,3.5,0.49761},{0.006,3.0,0.49761},{0.006,3.0,0.49761},{0.006,3.0,0.49761},{0.006,3.0,0.49761},{0.006,3.0,0.49761}},
{{0.35,0.4,0.58,0.69},{0.35,0.4,0.58,0.69},{0.35,0.4,0.58,0.69},{0.36,0.4,0.6,0.69},{0.35,0.4,0.58,0.69},{0.35,0.4,0.58,0.69},{0.35,0.4,0.58,0.69},{0.35,0.4,0.58,0.69},{0.35,0.41,0.62,0.69}}
)' > ../AnalysisResultFiles/CrossSectionsPt_All.txt	
/data/dust/group/h1/klesthen/BaryonAsym/H1LambdaC/AnalysisResultFiles									
cp /data/dust/group/h1/klesthen/BaryonAsym/H1LambdaC/AnalysisResultFiles/AnalysisOutput.root /data/dust/group/h1/klesthen/BaryonAsym/H1LambdaC/AnalysisResultFiles/CrossSectionsPt_All.root 							
rm /data/dust/group/h1/klesthen/BaryonAsym/H1LambdaC/AnalysisResultFiles/AnalysisOutput.root
#######################################################################





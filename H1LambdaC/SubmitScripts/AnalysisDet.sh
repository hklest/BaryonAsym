#!/bin/zsh

####
#Variable Index: 0=Q2 Dist, 1=Y Dist, 2=pT Dist, 3=Eta Dist, 4=Phi Dist
#Vector Defining Bins for Dist Variable
#Number of bin entries
#Data Lumi
#Radiative Gen MC Lumi
#Non-Radiative Gen MC Lumi
#Kaon Data Tree File
#Lambda Data Tree File
#Kaon MC Tree File
#Lambda MC Tree File
#Radiative Gen MC Tree File for Detector Efficiency
# Non-Radiative Gen MC Tree File for Radiative Corrections
# Radiative Gen MC Tree File for Radiative Corrections
#Couts Copied to This File for Troubleshooting
#Copy generic output name to specific one based on inputs so we don't overwrite it
#Remove old output after it is copied to new file


#######################################################################
###2005-2007 Eta Data
#Before calling root macro, need to write command to edit line defining num of bin entries
#C++ will not let this be a variable input since it defines array lengths
sed -i '35c\'"const int q2entries = 6;" "/data/dust/group/h1/gtustin/h1oo/H1LambdaC/Analysis.C"
root -l -q '/data/dust/group/h1/gtustin/h1oo/H1LambdaC/Analysis.C(
4,
{ -3.14, -2.0, -1.0, 0, 1.0, 2.0, 3.14 },
297657.8,
4108957.0,
3560233.0,											
"/data/dust/group/h1/gtustin/h1oo/H1LambdaC/KaonTree/merged.root", 	
"/data/dust/group/h1/gtustin/h1oo/H1LambdaC/LambdaTree/merged.root", 			
"/data/dust/group/h1/gtustin/h1oo/H1LambdaC_MC/KaonTreeMC/RAPGAP31/merged.root", 		
"/data/dust/group/h1/gtustin/h1oo/H1LambdaC_MC/LambdaTreeMC/RAPGAP31/merged.root", 		
"/data/dust/group/h1/gtustin/h1oo/H1LambdaC_MC/genMCTree/RAPGAP31/Rad/merged.root", 		
"/data/dust/group/h1/gtustin/h1oo/H1LambdaC_MC/genMCTree/DJANGOH14/NonRad/merged.root", 		
"/data/dust/group/h1/gtustin/h1oo/H1LambdaC_MC/genMCTree/DJANGOH14/Rad/merged.root" 		
)' > ../AnalysisResultFiles/AnalysisPhi_All.txt	
/data/dust/group/h1/gtustin/h1oo/H1LambdaC/AnalysisResultFiles									
cp /data/dust/group/h1/gtustin/h1oo/H1LambdaC/AnalysisResultFiles/AnalysisOutput.root /data/dust/group/h1/gtustin/h1oo/H1LambdaC/AnalysisResultFiles/AnalysisPhi_All.root 							
rm /data/dust/group/h1/gtustin/h1oo/H1LambdaC/AnalysisResultFiles/AnalysisOutput.root
#######################################################################
#######################################################################

#######################################################################
###2005-2007 Rad Decay Data
#Before calling root macro, need to write command to edit line defining num of bin entries
#C++ will not let this be a variable input since it defines array lengths
sed -i '35c\'"const int q2entries = 10;" "/data/dust/group/h1/gtustin/h1oo/H1LambdaC/Analysis.C"
root -l -q '/data/dust/group/h1/gtustin/h1oo/H1LambdaC/Analysis.C(
5,
{0, 3, 6, 9, 12, 15, 18, 21, 24, 27, 30},
297657.8,
4108957.0,
3560233.0,											
"/data/dust/group/h1/gtustin/h1oo/H1LambdaC/KaonTree/merged.root", 	
"/data/dust/group/h1/gtustin/h1oo/H1LambdaC/LambdaTree/merged.root", 			
"/data/dust/group/h1/gtustin/h1oo/H1LambdaC_MC/KaonTreeMC/RAPGAP31/merged.root", 		
"/data/dust/group/h1/gtustin/h1oo/H1LambdaC_MC/LambdaTreeMC/RAPGAP31/merged.root", 		
"/data/dust/group/h1/gtustin/h1oo/H1LambdaC_MC/genMCTree/RAPGAP31/Rad/merged.root", 		
"/data/dust/group/h1/gtustin/h1oo/H1LambdaC_MC/genMCTree/DJANGOH14/NonRad/merged.root", 		
"/data/dust/group/h1/gtustin/h1oo/H1LambdaC_MC/genMCTree/DJANGOH14/Rad/merged.root" 		
)' > ../AnalysisResultFiles/AnalysisRadDecay_All.txt	
/data/dust/group/h1/gtustin/h1oo/H1LambdaC/AnalysisResultFiles									
cp /data/dust/group/h1/gtustin/h1oo/H1LambdaC/AnalysisResultFiles/AnalysisOutput.root /data/dust/group/h1/gtustin/h1oo/H1LambdaC/AnalysisResultFiles/AnalysisRadDecay_All.root 							
rm /data/dust/group/h1/gtustin/h1oo/H1LambdaC/AnalysisResultFiles/AnalysisOutput.root
#######################################################################
#######################################################################








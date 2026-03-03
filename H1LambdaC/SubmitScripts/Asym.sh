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
#Couts Copied to This File for Troubleshooting
#Copy generic output name to specific one based on inputs so we don't overwrite it
#Remove old output after it is copied to new file


#######################################################################
###2005-2007 pT
#Before calling root macro, need to write command to edit line defining num of bin entries
#C++ will not let this be a variable input since it defines array lengths
sed -i '35c\'"const int q2entries = 9;" "/data/dust/group/h1/gtustin/h1oo/H1LambdaC/Asym.C"
root -l -q '/data/dust/group/h1/gtustin/h1oo/H1LambdaC/Asym.C(
2,
{0.5, 0.6, 0.7, 0.8, 0.9, 1.1, 1.3, 1.6, 2.2, 3.5},
297657.8,
4108957.0,
3560233.0,											
"/data/dust/group/h1/gtustin/h1oo/H1LambdaC/KaonTree/merged.root", 	
"/data/dust/group/h1/gtustin/h1oo/H1LambdaC/LambdaTree/merged.root", 			
"/data/dust/group/h1/gtustin/h1oo/H1LambdaC_MC/KaonTreeMC/RAPGAP31/merged.root", 		
"/data/dust/group/h1/gtustin/h1oo/H1LambdaC_MC/LambdaTreeMC/RAPGAP31/merged.root",
"/data/dust/group/h1/gtustin/h1oo/H1LambdaC_MC/LambdaTreeMC/DJANGOH14/merged.root",  		
"/data/dust/group/h1/gtustin/h1oo/H1LambdaC_MC/genMCTree/RAPGAP31/Rad/merged.root",
"/data/dust/group/h1/gtustin/h1oo/H1LambdaC_MC/genMCTree/DJANGOH14/Rad/merged.root", 		
"/data/dust/group/h1/gtustin/h1oo/H1LambdaC_MC/genMCTree/DJANGOH14/NonRad/merged.root", 		
"/data/dust/group/h1/gtustin/h1oo/H1LambdaC_MC/genMCTree/DJANGOH14/Rad/merged.root",
"p_{T} [GeV]" 		
)' > ../AnalysisResultFiles/AsymPt_All.txt	
/data/dust/group/h1/gtustin/h1oo/H1LambdaC/AnalysisResultFiles									
cp /data/dust/group/h1/gtustin/h1oo/H1LambdaC/AnalysisResultFiles/AnalysisOutput.root /data/dust/group/h1/gtustin/h1oo/H1LambdaC/AnalysisResultFiles/AsymPt_All.root 							
rm /data/dust/group/h1/gtustin/h1oo/H1LambdaC/AnalysisResultFiles/AnalysisOutput.root
#######################################################################
#######################################################################
#######################################################################
###2005-2007 Q2
#Before calling root macro, need to write command to edit line defining num of bin entries
#C++ will not let this be a variable input since it defines array lengths
sed -i '35c\'"const int q2entries = 5;" "/data/dust/group/h1/gtustin/h1oo/H1LambdaC/Asym.C"
root -l -q '/data/dust/group/h1/gtustin/h1oo/H1LambdaC/Asym.C(
0,
{ 5.0, 7.0, 10.0, 15.0, 25.0, 100.0 },
297657.8,
4108957.0,
3560233.0,											
"/data/dust/group/h1/gtustin/h1oo/H1LambdaC/KaonTree/merged.root", 	
"/data/dust/group/h1/gtustin/h1oo/H1LambdaC/LambdaTree/merged.root", 			
"/data/dust/group/h1/gtustin/h1oo/H1LambdaC_MC/KaonTreeMC/RAPGAP31/merged.root", 		
"/data/dust/group/h1/gtustin/h1oo/H1LambdaC_MC/LambdaTreeMC/RAPGAP31/merged.root",
"/data/dust/group/h1/gtustin/h1oo/H1LambdaC_MC/LambdaTreeMC/DJANGOH14/merged.root",  		
"/data/dust/group/h1/gtustin/h1oo/H1LambdaC_MC/genMCTree/RAPGAP31/Rad/merged.root",
"/data/dust/group/h1/gtustin/h1oo/H1LambdaC_MC/genMCTree/DJANGOH14/Rad/merged.root", 		
"/data/dust/group/h1/gtustin/h1oo/H1LambdaC_MC/genMCTree/DJANGOH14/NonRad/merged.root", 		
"/data/dust/group/h1/gtustin/h1oo/H1LambdaC_MC/genMCTree/DJANGOH14/Rad/merged.root",
"Q^{2} [GeV^{2}]" 		
)' > ../AnalysisResultFiles/AsymQ2_All.txt	
/data/dust/group/h1/gtustin/h1oo/H1LambdaC/AnalysisResultFiles									
cp /data/dust/group/h1/gtustin/h1oo/H1LambdaC/AnalysisResultFiles/AnalysisOutput.root /data/dust/group/h1/gtustin/h1oo/H1LambdaC/AnalysisResultFiles/AsymQ2_All.root 							
rm /data/dust/group/h1/gtustin/h1oo/H1LambdaC/AnalysisResultFiles/AnalysisOutput.root
#######################################################################
#######################################################################
#######################################################################
###2005-2007 X
#Before calling root macro, need to write command to edit line defining num of bin entries
#C++ will not let this be a variable input since it defines array lengths
sed -i '35c\'"const int q2entries = 4;" "/data/dust/group/h1/gtustin/h1oo/H1LambdaC/Asym.C"
root -l -q '/data/dust/group/h1/gtustin/h1oo/H1LambdaC/Asym.C(
1,
{0.0001, 0.0002, 0.0004, 0.001, 0.01},
297657.8,
4108957.0,
3560233.0,											
"/data/dust/group/h1/gtustin/h1oo/H1LambdaC/KaonTree/merged.root", 	
"/data/dust/group/h1/gtustin/h1oo/H1LambdaC/LambdaTree/merged.root", 			
"/data/dust/group/h1/gtustin/h1oo/H1LambdaC_MC/KaonTreeMC/RAPGAP31/merged.root", 		
"/data/dust/group/h1/gtustin/h1oo/H1LambdaC_MC/LambdaTreeMC/RAPGAP31/merged.root",
"/data/dust/group/h1/gtustin/h1oo/H1LambdaC_MC/LambdaTreeMC/DJANGOH14/merged.root",  		
"/data/dust/group/h1/gtustin/h1oo/H1LambdaC_MC/genMCTree/RAPGAP31/Rad/merged.root",
"/data/dust/group/h1/gtustin/h1oo/H1LambdaC_MC/genMCTree/DJANGOH14/Rad/merged.root", 		
"/data/dust/group/h1/gtustin/h1oo/H1LambdaC_MC/genMCTree/DJANGOH14/NonRad/merged.root", 		
"/data/dust/group/h1/gtustin/h1oo/H1LambdaC_MC/genMCTree/DJANGOH14/Rad/merged.root",
"X" 		
)' > ../AnalysisResultFiles/AsymX_All.txt	
/data/dust/group/h1/gtustin/h1oo/H1LambdaC/AnalysisResultFiles									
cp /data/dust/group/h1/gtustin/h1oo/H1LambdaC/AnalysisResultFiles/AnalysisOutput.root /data/dust/group/h1/gtustin/h1oo/H1LambdaC/AnalysisResultFiles/AsymX_All.root 							
rm /data/dust/group/h1/gtustin/h1oo/H1LambdaC/AnalysisResultFiles/AnalysisOutput.root
#######################################################################
#######################################################################
#######################################################################
###2005-2007 Eta
#Before calling root macro, need to write command to edit line defining num of bin entries
#C++ will not let this be a variable input since it defines array lengths
sed -i '35c\'"const int q2entries = 10;" "/data/dust/group/h1/gtustin/h1oo/H1LambdaC/Asym.C"
root -l -q '/data/dust/group/h1/gtustin/h1oo/H1LambdaC/Asym.C(
3,
{-1.3, -1.0, -0.75, -0.5, -0.25, 0, 0.25, 0.5, 0.75, 1.0, 1.3},
297657.8,
4108957.0,
3560233.0,											
"/data/dust/group/h1/gtustin/h1oo/H1LambdaC/KaonTree/merged.root", 	
"/data/dust/group/h1/gtustin/h1oo/H1LambdaC/LambdaTree/merged.root", 			
"/data/dust/group/h1/gtustin/h1oo/H1LambdaC_MC/KaonTreeMC/RAPGAP31/merged.root", 		
"/data/dust/group/h1/gtustin/h1oo/H1LambdaC_MC/LambdaTreeMC/RAPGAP31/merged.root",
"/data/dust/group/h1/gtustin/h1oo/H1LambdaC_MC/LambdaTreeMC/DJANGOH14/merged.root",  		
"/data/dust/group/h1/gtustin/h1oo/H1LambdaC_MC/genMCTree/RAPGAP31/Rad/merged.root",
"/data/dust/group/h1/gtustin/h1oo/H1LambdaC_MC/genMCTree/DJANGOH14/Rad/merged.root", 		
"/data/dust/group/h1/gtustin/h1oo/H1LambdaC_MC/genMCTree/DJANGOH14/NonRad/merged.root", 		
"/data/dust/group/h1/gtustin/h1oo/H1LambdaC_MC/genMCTree/DJANGOH14/Rad/merged.root",
"#eta" 		
)' > ../AnalysisResultFiles/AsymEta_All.txt	
/data/dust/group/h1/gtustin/h1oo/H1LambdaC/AnalysisResultFiles									
cp /data/dust/group/h1/gtustin/h1oo/H1LambdaC/AnalysisResultFiles/AnalysisOutput.root /data/dust/group/h1/gtustin/h1oo/H1LambdaC/AnalysisResultFiles/AsymEta_All.root 							
rm /data/dust/group/h1/gtustin/h1oo/H1LambdaC/AnalysisResultFiles/AnalysisOutput.root
#######################################################################
#######################################################################
#######################################################################
#######################################################################
###2005-2007 Rad Decay
#Before calling root macro, need to write command to edit line defining num of bin entries
#C++ will not let this be a variable input since it defines array lengths
sed -i '35c\'"const int q2entries = 6;" "/data/dust/group/h1/gtustin/h1oo/H1LambdaC/Asym.C"
root -l -q '/data/dust/group/h1/gtustin/h1oo/H1LambdaC/Asym.C(
5,
{0,5,10,15,20,25,30},
297657.8,
4108957.0,
3560233.0,											
"/data/dust/group/h1/gtustin/h1oo/H1LambdaC/KaonTree/merged.root", 	
"/data/dust/group/h1/gtustin/h1oo/H1LambdaC/LambdaTree/merged.root", 			
"/data/dust/group/h1/gtustin/h1oo/H1LambdaC_MC/KaonTreeMC/RAPGAP31/merged.root", 		
"/data/dust/group/h1/gtustin/h1oo/H1LambdaC_MC/LambdaTreeMC/RAPGAP31/merged.root",
"/data/dust/group/h1/gtustin/h1oo/H1LambdaC_MC/LambdaTreeMC/DJANGOH14/merged.root",  		
"/data/dust/group/h1/gtustin/h1oo/H1LambdaC_MC/genMCTree/RAPGAP31/Rad/merged.root",
"/data/dust/group/h1/gtustin/h1oo/H1LambdaC_MC/genMCTree/DJANGOH14/Rad/merged.root", 		
"/data/dust/group/h1/gtustin/h1oo/H1LambdaC_MC/genMCTree/DJANGOH14/NonRad/merged.root", 		
"/data/dust/group/h1/gtustin/h1oo/H1LambdaC_MC/genMCTree/DJANGOH14/Rad/merged.root",
"Radial Decay Vertex [cm]" 		
)' > ../AnalysisResultFiles/AsymRadDecay_All.txt	
/data/dust/group/h1/gtustin/h1oo/H1LambdaC/AnalysisResultFiles									
cp /data/dust/group/h1/gtustin/h1oo/H1LambdaC/AnalysisResultFiles/AnalysisOutput.root /data/dust/group/h1/gtustin/h1oo/H1LambdaC/AnalysisResultFiles/AsymRadDecay_All.root 							
rm /data/dust/group/h1/gtustin/h1oo/H1LambdaC/AnalysisResultFiles/AnalysisOutput.root
#######################################################################
#######################################################################
#######################################################################
###2005-2007 Z Decay
#Before calling root macro, need to write command to edit line defining num of bin entries
#C++ will not let this be a variable input since it defines array lengths
sed -i '35c\'"const int q2entries = 12;" "/data/dust/group/h1/gtustin/h1oo/H1LambdaC/Asym.C"
root -l -q '/data/dust/group/h1/gtustin/h1oo/H1LambdaC/Asym.C(
6,
{-30,-25,-20,-15,-10,-5,0,5,10,15,20,25,30},
297657.8,
4108957.0,
3560233.0,											
"/data/dust/group/h1/gtustin/h1oo/H1LambdaC/KaonTree/merged.root", 	
"/data/dust/group/h1/gtustin/h1oo/H1LambdaC/LambdaTree/merged.root", 			
"/data/dust/group/h1/gtustin/h1oo/H1LambdaC_MC/KaonTreeMC/RAPGAP31/merged.root", 		
"/data/dust/group/h1/gtustin/h1oo/H1LambdaC_MC/LambdaTreeMC/RAPGAP31/merged.root",
"/data/dust/group/h1/gtustin/h1oo/H1LambdaC_MC/LambdaTreeMC/DJANGOH14/merged.root",  		
"/data/dust/group/h1/gtustin/h1oo/H1LambdaC_MC/genMCTree/RAPGAP31/Rad/merged.root",
"/data/dust/group/h1/gtustin/h1oo/H1LambdaC_MC/genMCTree/DJANGOH14/Rad/merged.root", 		
"/data/dust/group/h1/gtustin/h1oo/H1LambdaC_MC/genMCTree/DJANGOH14/NonRad/merged.root", 		
"/data/dust/group/h1/gtustin/h1oo/H1LambdaC_MC/genMCTree/DJANGOH14/Rad/merged.root",
"Z Decay Vertex [cm]" 		
)' > ../AnalysisResultFiles/AsymZDecay_All.txt	
/data/dust/group/h1/gtustin/h1oo/H1LambdaC/AnalysisResultFiles									
cp /data/dust/group/h1/gtustin/h1oo/H1LambdaC/AnalysisResultFiles/AnalysisOutput.root /data/dust/group/h1/gtustin/h1oo/H1LambdaC/AnalysisResultFiles/AsymZDecay_All.root 							
rm /data/dust/group/h1/gtustin/h1oo/H1LambdaC/AnalysisResultFiles/AnalysisOutput.root
#######################################################################
#######################################################################








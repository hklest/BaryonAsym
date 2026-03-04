#!/bin/zsh

####
#Variable Index: 0=Q2 Dist, 1=Y Dist, 2=pT Dist, 3=Eta Dist
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
###2005-2007 Q2 Data
#Before calling root macro, need to write command to edit line defining num of bin entries
#C++ will not let this be a variable input since it defines array lengths
sed -i '35c\'"const int q2entries = 5;" "/data/dust/group/h1/klesthen/BaryonAsym/H1LambdaC/Analysis.C"
root -l -q '/data/dust/group/h1/klesthen/BaryonAsym/H1LambdaC/Analysis.C(
0,
{ 5.0, 7.0, 10.0, 15.0, 25.0, 100.0 },
297657.8,
4108957.0,
3560233.0,											
"/data/dust/group/h1/klesthen/BaryonAsym/H1LambdaC/KaonTree/merged.root", 	
"/data/dust/group/h1/klesthen/BaryonAsym/H1LambdaC/LambdaTree/merged.root", 			
"/data/dust/group/h1/klesthen/BaryonAsym/H1LambdaC_MC/KaonTreeMC/RAPGAP31/merged.root", 		
"/data/dust/group/h1/klesthen/BaryonAsym/H1LambdaC_MC/LambdaTreeMC/RAPGAP31/merged.root", 		
"/data/dust/group/h1/klesthen/BaryonAsym/H1LambdaC_MC/genMCTree/RAPGAP31/Rad/merged.root", 		
"/data/dust/group/h1/klesthen/BaryonAsym/H1LambdaC_MC/genMCTree/DJANGOH14/NonRad/merged.root", 		
"/data/dust/group/h1/klesthen/BaryonAsym/H1LambdaC_MC/genMCTree/DJANGOH14/Rad/merged.root" 		
)' > ../AnalysisResultFiles/AnalysisQ2_All.txt	
/data/dust/group/h1/klesthen/BaryonAsym/H1LambdaC/AnalysisResultFiles									
cp /data/dust/group/h1/klesthen/BaryonAsym/H1LambdaC/AnalysisResultFiles/AnalysisOutput.root /data/dust/group/h1/klesthen/BaryonAsym/H1LambdaC/AnalysisResultFiles/AnalysisQ2_All.root 							
rm /data/dust/group/h1/klesthen/BaryonAsym/H1LambdaC/AnalysisResultFiles/AnalysisOutput.root
#######################################################################
#######################################################################
###2005 Q2 Data
#Before calling root macro, need to write command to edit line defining num of bin entries
#C++ will not let this be a variable input since it defines array lengths
sed -i '35c\'"const int q2entries = 5;" "/data/dust/group/h1/klesthen/BaryonAsym/H1LambdaC/Analysis.C"
root -l -q '/data/dust/group/h1/klesthen/BaryonAsym/H1LambdaC/Analysis.C(
0,
{ 5.0, 7.0, 10.0, 15.0, 25.0, 100.0 },
106823.1,
1087523.00,
1133362.00,											
"/data/dust/group/h1/klesthen/BaryonAsym/H1LambdaC/KaonTree/2005/merged.root", 	
"/data/dust/group/h1/klesthen/BaryonAsym/H1LambdaC/LambdaTree/2005/merged.root", 			
"/data/dust/group/h1/klesthen/BaryonAsym/H1LambdaC_MC/KaonTreeMC/RAPGAP31/2005/merged.root", 		
"/data/dust/group/h1/klesthen/BaryonAsym/H1LambdaC_MC/LambdaTreeMC/RAPGAP31/2005/merged.root", 		
"/data/dust/group/h1/klesthen/BaryonAsym/H1LambdaC_MC/genMCTree/RAPGAP31/Rad/2005/merged.root", 		
"/data/dust/group/h1/klesthen/BaryonAsym/H1LambdaC_MC/genMCTree/DJANGOH14/NonRad/2005/merged.root", 		
"/data/dust/group/h1/klesthen/BaryonAsym/H1LambdaC_MC/genMCTree/DJANGOH14/Rad/2005/merged.root" 		
)' > ../AnalysisResultFiles/AnalysisQ2_2005.txt	
/data/dust/group/h1/klesthen/BaryonAsym/H1LambdaC/AnalysisResultFiles									
cp /data/dust/group/h1/klesthen/BaryonAsym/H1LambdaC/AnalysisResultFiles/AnalysisOutput.root /data/dust/group/h1/klesthen/BaryonAsym/H1LambdaC/AnalysisResultFiles/AnalysisQ2_2005.root 							
rm /data/dust/group/h1/klesthen/BaryonAsym/H1LambdaC/AnalysisResultFiles/AnalysisOutput.root
#######################################################################
###2006 Q2 Data
#Before calling root macro, need to write command to edit line defining num of bin entries
#C++ will not let this be a variable input since it defines array lengths
sed -i '35c\'"const int q2entries = 5;" "/data/dust/group/h1/klesthen/BaryonAsym/H1LambdaC/Analysis.C"
root -l -q '/data/dust/group/h1/klesthen/BaryonAsym/H1LambdaC/Analysis.C(
0,
{ 5.0, 7.0, 10.0, 15.0, 25.0, 100.0 },
144634.0,
2268210.00,
1944680.00,											
"/data/dust/group/h1/klesthen/BaryonAsym/H1LambdaC/KaonTree/2006/merged.root", 	
"/data/dust/group/h1/klesthen/BaryonAsym/H1LambdaC/LambdaTree/2006/merged.root", 			
"/data/dust/group/h1/klesthen/BaryonAsym/H1LambdaC_MC/KaonTreeMC/RAPGAP31/2006/merged.root", 		
"/data/dust/group/h1/klesthen/BaryonAsym/H1LambdaC_MC/LambdaTreeMC/RAPGAP31/2006/merged.root", 		
"/data/dust/group/h1/klesthen/BaryonAsym/H1LambdaC_MC/genMCTree/RAPGAP31/Rad/2006/merged.root", 		
"/data/dust/group/h1/klesthen/BaryonAsym/H1LambdaC_MC/genMCTree/DJANGOH14/NonRad/2006/merged.root", 		
"/data/dust/group/h1/klesthen/BaryonAsym/H1LambdaC_MC/genMCTree/DJANGOH14/Rad/2006/merged.root" 		
)' > ../AnalysisResultFiles/AnalysisQ2_2006.txt	
/data/dust/group/h1/klesthen/BaryonAsym/H1LambdaC/AnalysisResultFiles									
cp /data/dust/group/h1/klesthen/BaryonAsym/H1LambdaC/AnalysisResultFiles/AnalysisOutput.root /data/dust/group/h1/klesthen/BaryonAsym/H1LambdaC/AnalysisResultFiles/AnalysisQ2_2006.root 							
rm /data/dust/group/h1/klesthen/BaryonAsym/H1LambdaC/AnalysisResultFiles/AnalysisOutput.root
#######################################################################
#######################################################################
###2007 Q2 Data
#Before calling root macro, need to write command to edit line defining num of bin entries
#C++ will not let this be a variable input since it defines array lengths
sed -i '35c\'"const int q2entries = 5;" "/data/dust/group/h1/klesthen/BaryonAsym/H1LambdaC/Analysis.C"
root -l -q '/data/dust/group/h1/klesthen/BaryonAsym/H1LambdaC/Analysis.C(
0,
{ 5.0, 7.0, 10.0, 15.0, 25.0, 100.0 },
46200.7,
753224.00,
482191.00,											
"/data/dust/group/h1/klesthen/BaryonAsym/H1LambdaC/KaonTree/2007/merged.root", 	
"/data/dust/group/h1/klesthen/BaryonAsym/H1LambdaC/LambdaTree/2007/merged.root", 			
"/data/dust/group/h1/klesthen/BaryonAsym/H1LambdaC_MC/KaonTreeMC/RAPGAP31/2007/merged.root", 		
"/data/dust/group/h1/klesthen/BaryonAsym/H1LambdaC_MC/LambdaTreeMC/RAPGAP31/2007/merged.root", 		
"/data/dust/group/h1/klesthen/BaryonAsym/H1LambdaC_MC/genMCTree/RAPGAP31/Rad/2007/merged.root", 		
"/data/dust/group/h1/klesthen/BaryonAsym/H1LambdaC_MC/genMCTree/DJANGOH14/NonRad/2007/merged.root", 		
"/data/dust/group/h1/klesthen/BaryonAsym/H1LambdaC_MC/genMCTree/DJANGOH14/Rad/2007/merged.root" 		
)' > ../AnalysisResultFiles/AnalysisQ2_2007.txt	
/data/dust/group/h1/klesthen/BaryonAsym/H1LambdaC/AnalysisResultFiles									
cp /data/dust/group/h1/klesthen/BaryonAsym/H1LambdaC/AnalysisResultFiles/AnalysisOutput.root /data/dust/group/h1/klesthen/BaryonAsym/H1LambdaC/AnalysisResultFiles/AnalysisQ2_2007.root 							
rm /data/dust/group/h1/klesthen/BaryonAsym/H1LambdaC/AnalysisResultFiles/AnalysisOutput.root
#######################################################################














#######################################################################
###2005-2007 X Data
#Before calling root macro, need to write command to edit line defining num of bin entries
#C++ will not let this be a variable input since it defines array lengths
sed -i '35c\'"const int q2entries = 4;" "/data/dust/group/h1/klesthen/BaryonAsym/H1LambdaC/Analysis.C"
root -l -q '/data/dust/group/h1/klesthen/BaryonAsym/H1LambdaC/Analysis.C(
1,
{0.0001, 0.0002, 0.0004, 0.001, 0.01},
297657.8,
4108957.0,
3560233.0,											
"/data/dust/group/h1/klesthen/BaryonAsym/H1LambdaC/KaonTree/merged.root", 	
"/data/dust/group/h1/klesthen/BaryonAsym/H1LambdaC/LambdaTree/merged.root", 			
"/data/dust/group/h1/klesthen/BaryonAsym/H1LambdaC_MC/KaonTreeMC/RAPGAP31/merged.root", 		
"/data/dust/group/h1/klesthen/BaryonAsym/H1LambdaC_MC/LambdaTreeMC/RAPGAP31/merged.root", 		
"/data/dust/group/h1/klesthen/BaryonAsym/H1LambdaC_MC/genMCTree/RAPGAP31/Rad/merged.root", 		
"/data/dust/group/h1/klesthen/BaryonAsym/H1LambdaC_MC/genMCTree/DJANGOH14/NonRad/merged.root", 		
"/data/dust/group/h1/klesthen/BaryonAsym/H1LambdaC_MC/genMCTree/DJANGOH14/Rad/merged.root" 		
)' > ../AnalysisResultFiles/AnalysisX_All.txt	
/data/dust/group/h1/klesthen/BaryonAsym/H1LambdaC/AnalysisResultFiles									
cp /data/dust/group/h1/klesthen/BaryonAsym/H1LambdaC/AnalysisResultFiles/AnalysisOutput.root /data/dust/group/h1/klesthen/BaryonAsym/H1LambdaC/AnalysisResultFiles/AnalysisX_All.root 							
rm /data/dust/group/h1/klesthen/BaryonAsym/H1LambdaC/AnalysisResultFiles/AnalysisOutput.root
#######################################################################
#######################################################################
###2005 X Data
#Before calling root macro, need to write command to edit line defining num of bin entries
#C++ will not let this be a variable input since it defines array lengths
sed -i '35c\'"const int q2entries = 4;" "/data/dust/group/h1/klesthen/BaryonAsym/H1LambdaC/Analysis.C"
root -l -q '/data/dust/group/h1/klesthen/BaryonAsym/H1LambdaC/Analysis.C(
1,
{0.0001, 0.0002, 0.0004, 0.001, 0.01},
106823.1,
1087523.00,
1133362.00,											
"/data/dust/group/h1/klesthen/BaryonAsym/H1LambdaC/KaonTree/2005/merged.root", 	
"/data/dust/group/h1/klesthen/BaryonAsym/H1LambdaC/LambdaTree/2005/merged.root", 			
"/data/dust/group/h1/klesthen/BaryonAsym/H1LambdaC_MC/KaonTreeMC/RAPGAP31/2005/merged.root", 		
"/data/dust/group/h1/klesthen/BaryonAsym/H1LambdaC_MC/LambdaTreeMC/RAPGAP31/2005/merged.root", 		
"/data/dust/group/h1/klesthen/BaryonAsym/H1LambdaC_MC/genMCTree/RAPGAP31/Rad/2005/merged.root", 		
"/data/dust/group/h1/klesthen/BaryonAsym/H1LambdaC_MC/genMCTree/DJANGOH14/NonRad/2005/merged.root", 		
"/data/dust/group/h1/klesthen/BaryonAsym/H1LambdaC_MC/genMCTree/DJANGOH14/Rad/2005/merged.root" 		
)' > ../AnalysisResultFiles/AnalysisX_2005.txt	
/data/dust/group/h1/klesthen/BaryonAsym/H1LambdaC/AnalysisResultFiles									
cp /data/dust/group/h1/klesthen/BaryonAsym/H1LambdaC/AnalysisResultFiles/AnalysisOutput.root /data/dust/group/h1/klesthen/BaryonAsym/H1LambdaC/AnalysisResultFiles/AnalysisX_2005.root 							
rm /data/dust/group/h1/klesthen/BaryonAsym/H1LambdaC/AnalysisResultFiles/AnalysisOutput.root
#######################################################################
###2006 X Data
#Before calling root macro, need to write command to edit line defining num of bin entries
#C++ will not let this be a variable input since it defines array lengths
sed -i '35c\'"const int q2entries = 4;" "/data/dust/group/h1/klesthen/BaryonAsym/H1LambdaC/Analysis.C"
root -l -q '/data/dust/group/h1/klesthen/BaryonAsym/H1LambdaC/Analysis.C(
1,
{0.0001, 0.0002, 0.0004, 0.001, 0.01},
144634.0,
2268210.00,
1944680.00,											
"/data/dust/group/h1/klesthen/BaryonAsym/H1LambdaC/KaonTree/2006/merged.root", 	
"/data/dust/group/h1/klesthen/BaryonAsym/H1LambdaC/LambdaTree/2006/merged.root", 			
"/data/dust/group/h1/klesthen/BaryonAsym/H1LambdaC_MC/KaonTreeMC/RAPGAP31/2006/merged.root", 		
"/data/dust/group/h1/klesthen/BaryonAsym/H1LambdaC_MC/LambdaTreeMC/RAPGAP31/2006/merged.root", 		
"/data/dust/group/h1/klesthen/BaryonAsym/H1LambdaC_MC/genMCTree/RAPGAP31/Rad/2006/merged.root", 		
"/data/dust/group/h1/klesthen/BaryonAsym/H1LambdaC_MC/genMCTree/DJANGOH14/NonRad/2006/merged.root", 		
"/data/dust/group/h1/klesthen/BaryonAsym/H1LambdaC_MC/genMCTree/DJANGOH14/Rad/2006/merged.root" 		
)' > ../AnalysisResultFiles/AnalysisX_2006.txt	
/data/dust/group/h1/klesthen/BaryonAsym/H1LambdaC/AnalysisResultFiles									
cp /data/dust/group/h1/klesthen/BaryonAsym/H1LambdaC/AnalysisResultFiles/AnalysisOutput.root /data/dust/group/h1/klesthen/BaryonAsym/H1LambdaC/AnalysisResultFiles/AnalysisX_2006.root 							
rm /data/dust/group/h1/klesthen/BaryonAsym/H1LambdaC/AnalysisResultFiles/AnalysisOutput.root
#######################################################################
#######################################################################
###2007 X Data
#Before calling root macro, need to write command to edit line defining num of bin entries
#C++ will not let this be a variable input since it defines array lengths
sed -i '35c\'"const int q2entries = 4;" "/data/dust/group/h1/klesthen/BaryonAsym/H1LambdaC/Analysis.C"
root -l -q '/data/dust/group/h1/klesthen/BaryonAsym/H1LambdaC/Analysis.C(
1,
{0.0001, 0.0002, 0.0004, 0.001, 0.01},
46200.7,
753224.00,
482191.00,											
"/data/dust/group/h1/klesthen/BaryonAsym/H1LambdaC/KaonTree/2007/merged.root", 	
"/data/dust/group/h1/klesthen/BaryonAsym/H1LambdaC/LambdaTree/2007/merged.root", 			
"/data/dust/group/h1/klesthen/BaryonAsym/H1LambdaC_MC/KaonTreeMC/RAPGAP31/2007/merged.root", 		
"/data/dust/group/h1/klesthen/BaryonAsym/H1LambdaC_MC/LambdaTreeMC/RAPGAP31/2007/merged.root", 		
"/data/dust/group/h1/klesthen/BaryonAsym/H1LambdaC_MC/genMCTree/RAPGAP31/Rad/2007/merged.root", 		
"/data/dust/group/h1/klesthen/BaryonAsym/H1LambdaC_MC/genMCTree/DJANGOH14/NonRad/2007/merged.root", 		
"/data/dust/group/h1/klesthen/BaryonAsym/H1LambdaC_MC/genMCTree/DJANGOH14/Rad/2007/merged.root" 		
)' > ../AnalysisResultFiles/AnalysisX_2007.txt	
/data/dust/group/h1/klesthen/BaryonAsym/H1LambdaC/AnalysisResultFiles									
cp /data/dust/group/h1/klesthen/BaryonAsym/H1LambdaC/AnalysisResultFiles/AnalysisOutput.root /data/dust/group/h1/klesthen/BaryonAsym/H1LambdaC/AnalysisResultFiles/AnalysisX_2007.root 							
rm /data/dust/group/h1/klesthen/BaryonAsym/H1LambdaC/AnalysisResultFiles/AnalysisOutput.root
#######################################################################











#######################################################################
###2005-2007 pT Data
#Before calling root macro, need to write command to edit line defining num of bin entries
#C++ will not let this be a variable input since it defines array lengths
sed -i '35c\'"const int q2entries = 9;" "/data/dust/group/h1/klesthen/BaryonAsym/H1LambdaC/Analysis.C"
root -l -q '/data/dust/group/h1/klesthen/BaryonAsym/H1LambdaC/Analysis.C(
2,
{0.5, 0.6, 0.7, 0.8, 0.9, 1.1, 1.3, 1.6, 2.2, 3.5},
297657.8,
4108957.0,
3560233.0,											
"/data/dust/group/h1/klesthen/BaryonAsym/H1LambdaC/KaonTree/merged.root", 	
"/data/dust/group/h1/klesthen/BaryonAsym/H1LambdaC/LambdaTree/merged.root", 			
"/data/dust/group/h1/klesthen/BaryonAsym/H1LambdaC_MC/KaonTreeMC/RAPGAP31/merged.root", 		
"/data/dust/group/h1/klesthen/BaryonAsym/H1LambdaC_MC/LambdaTreeMC/RAPGAP31/merged.root", 		
"/data/dust/group/h1/klesthen/BaryonAsym/H1LambdaC_MC/genMCTree/RAPGAP31/Rad/merged.root", 		
"/data/dust/group/h1/klesthen/BaryonAsym/H1LambdaC_MC/genMCTree/DJANGOH14/NonRad/merged.root", 		
"/data/dust/group/h1/klesthen/BaryonAsym/H1LambdaC_MC/genMCTree/DJANGOH14/Rad/merged.root" 		
)' > ../AnalysisResultFiles/AnalysispT_All.txt	
/data/dust/group/h1/klesthen/BaryonAsym/H1LambdaC/AnalysisResultFiles									
cp /data/dust/group/h1/klesthen/BaryonAsym/H1LambdaC/AnalysisResultFiles/AnalysisOutput.root /data/dust/group/h1/klesthen/BaryonAsym/H1LambdaC/AnalysisResultFiles/AnalysispT_All.root 							
rm /data/dust/group/h1/klesthen/BaryonAsym/H1LambdaC/AnalysisResultFiles/AnalysisOutput.root
#######################################################################
#######################################################################
###2005 pT Data
#Before calling root macro, need to write command to edit line defining num of bin entries
#C++ will not let this be a variable input since it defines array lengths
sed -i '35c\'"const int q2entries = 9;" "/data/dust/group/h1/klesthen/BaryonAsym/H1LambdaC/Analysis.C"
root -l -q '/data/dust/group/h1/klesthen/BaryonAsym/H1LambdaC/Analysis.C(
2,
{0.5, 0.6, 0.7, 0.8, 0.9, 1.1, 1.3, 1.6, 2.2, 3.5},
106823.1,
1087523.00,
1133362.00,											
"/data/dust/group/h1/klesthen/BaryonAsym/H1LambdaC/KaonTree/2005/merged.root", 	
"/data/dust/group/h1/klesthen/BaryonAsym/H1LambdaC/LambdaTree/2005/merged.root", 			
"/data/dust/group/h1/klesthen/BaryonAsym/H1LambdaC_MC/KaonTreeMC/RAPGAP31/2005/merged.root", 		
"/data/dust/group/h1/klesthen/BaryonAsym/H1LambdaC_MC/LambdaTreeMC/RAPGAP31/2005/merged.root", 		
"/data/dust/group/h1/klesthen/BaryonAsym/H1LambdaC_MC/genMCTree/RAPGAP31/Rad/2005/merged.root", 		
"/data/dust/group/h1/klesthen/BaryonAsym/H1LambdaC_MC/genMCTree/DJANGOH14/NonRad/2005/merged.root", 		
"/data/dust/group/h1/klesthen/BaryonAsym/H1LambdaC_MC/genMCTree/DJANGOH14/Rad/2005/merged.root" 	
)' > ../AnalysisResultFiles/AnalysispT_2005.txt	
/data/dust/group/h1/klesthen/BaryonAsym/H1LambdaC/AnalysisResultFiles									
cp /data/dust/group/h1/klesthen/BaryonAsym/H1LambdaC/AnalysisResultFiles/AnalysisOutput.root /data/dust/group/h1/klesthen/BaryonAsym/H1LambdaC/AnalysisResultFiles/AnalysispT_2005.root 							
rm /data/dust/group/h1/klesthen/BaryonAsym/H1LambdaC/AnalysisResultFiles/AnalysisOutput.root
#######################################################################
###2006 pT Data
#Before calling root macro, need to write command to edit line defining num of bin entries
#C++ will not let this be a variable input since it defines array lengths
sed -i '35c\'"const int q2entries = 9;" "/data/dust/group/h1/klesthen/BaryonAsym/H1LambdaC/Analysis.C"
root -l -q '/data/dust/group/h1/klesthen/BaryonAsym/H1LambdaC/Analysis.C(
2,
{0.5, 0.6, 0.7, 0.8, 0.9, 1.1, 1.3, 1.6, 2.2, 3.5},
144634.0,
2268210.00,
1944680.00,											
"/data/dust/group/h1/klesthen/BaryonAsym/H1LambdaC/KaonTree/2006/merged.root", 	
"/data/dust/group/h1/klesthen/BaryonAsym/H1LambdaC/LambdaTree/2006/merged.root", 			
"/data/dust/group/h1/klesthen/BaryonAsym/H1LambdaC_MC/KaonTreeMC/RAPGAP31/2006/merged.root", 		
"/data/dust/group/h1/klesthen/BaryonAsym/H1LambdaC_MC/LambdaTreeMC/RAPGAP31/2006/merged.root", 		
"/data/dust/group/h1/klesthen/BaryonAsym/H1LambdaC_MC/genMCTree/RAPGAP31/Rad/2006/merged.root", 		
"/data/dust/group/h1/klesthen/BaryonAsym/H1LambdaC_MC/genMCTree/DJANGOH14/NonRad/2006/merged.root", 		
"/data/dust/group/h1/klesthen/BaryonAsym/H1LambdaC_MC/genMCTree/DJANGOH14/Rad/2006/merged.root" 	
)' > ../AnalysisResultFiles/AnalysispT_2006.txt	
/data/dust/group/h1/klesthen/BaryonAsym/H1LambdaC/AnalysisResultFiles									
cp /data/dust/group/h1/klesthen/BaryonAsym/H1LambdaC/AnalysisResultFiles/AnalysisOutput.root /data/dust/group/h1/klesthen/BaryonAsym/H1LambdaC/AnalysisResultFiles/AnalysispT_2006.root 							
rm /data/dust/group/h1/klesthen/BaryonAsym/H1LambdaC/AnalysisResultFiles/AnalysisOutput.root
#######################################################################
#######################################################################
###2007 pT Data
#Before calling root macro, need to write command to edit line defining num of bin entries
#C++ will not let this be a variable input since it defines array lengths
sed -i '35c\'"const int q2entries = 9;" "/data/dust/group/h1/klesthen/BaryonAsym/H1LambdaC/Analysis.C"
root -l -q '/data/dust/group/h1/klesthen/BaryonAsym/H1LambdaC/Analysis.C(
2,
{0.5, 0.6, 0.7, 0.8, 0.9, 1.1, 1.3, 1.6, 2.2, 3.5},
46200.7,
753224.00,
482191.00,											
"/data/dust/group/h1/klesthen/BaryonAsym/H1LambdaC/KaonTree/2007/merged.root", 	
"/data/dust/group/h1/klesthen/BaryonAsym/H1LambdaC/LambdaTree/2007/merged.root", 			
"/data/dust/group/h1/klesthen/BaryonAsym/H1LambdaC_MC/KaonTreeMC/RAPGAP31/2007/merged.root", 		
"/data/dust/group/h1/klesthen/BaryonAsym/H1LambdaC_MC/LambdaTreeMC/RAPGAP31/2007/merged.root", 		
"/data/dust/group/h1/klesthen/BaryonAsym/H1LambdaC_MC/genMCTree/RAPGAP31/Rad/2007/merged.root", 		
"/data/dust/group/h1/klesthen/BaryonAsym/H1LambdaC_MC/genMCTree/DJANGOH14/NonRad/2007/merged.root", 		
"/data/dust/group/h1/klesthen/BaryonAsym/H1LambdaC_MC/genMCTree/DJANGOH14/Rad/2007/merged.root" 	
)' > ../AnalysisResultFiles/AnalysispT_2007.txt	
/data/dust/group/h1/klesthen/BaryonAsym/H1LambdaC/AnalysisResultFiles									
cp /data/dust/group/h1/klesthen/BaryonAsym/H1LambdaC/AnalysisResultFiles/AnalysisOutput.root /data/dust/group/h1/klesthen/BaryonAsym/H1LambdaC/AnalysisResultFiles/AnalysispT_2007.root 							
rm /data/dust/group/h1/klesthen/BaryonAsym/H1LambdaC/AnalysisResultFiles/AnalysisOutput.root
#######################################################################













#######################################################################
###2005-2007 Eta Data
#Before calling root macro, need to write command to edit line defining num of bin entries
#C++ will not let this be a variable input since it defines array lengths
sed -i '35c\'"const int q2entries = 10;" "/data/dust/group/h1/klesthen/BaryonAsym/H1LambdaC/Analysis.C"
root -l -q '/data/dust/group/h1/klesthen/BaryonAsym/H1LambdaC/Analysis.C(
3,
{-1.3, -1.0, -0.75, -0.5, -0.25, 0, 0.25, 0.5, 0.75, 1.0, 1.3},
297657.8,
4108957.0,
3560233.0,											
"/data/dust/group/h1/klesthen/BaryonAsym/H1LambdaC/KaonTree/merged.root", 	
"/data/dust/group/h1/klesthen/BaryonAsym/H1LambdaC/LambdaTree/merged.root", 			
"/data/dust/group/h1/klesthen/BaryonAsym/H1LambdaC_MC/KaonTreeMC/RAPGAP31/merged.root", 		
"/data/dust/group/h1/klesthen/BaryonAsym/H1LambdaC_MC/LambdaTreeMC/RAPGAP31/merged.root", 		
"/data/dust/group/h1/klesthen/BaryonAsym/H1LambdaC_MC/genMCTree/RAPGAP31/Rad/merged.root", 		
"/data/dust/group/h1/klesthen/BaryonAsym/H1LambdaC_MC/genMCTree/DJANGOH14/NonRad/merged.root", 		
"/data/dust/group/h1/klesthen/BaryonAsym/H1LambdaC_MC/genMCTree/DJANGOH14/Rad/merged.root" 		
)' > ../AnalysisResultFiles/AnalysisEta_All.txt	
/data/dust/group/h1/klesthen/BaryonAsym/H1LambdaC/AnalysisResultFiles									
cp /data/dust/group/h1/klesthen/BaryonAsym/H1LambdaC/AnalysisResultFiles/AnalysisOutput.root /data/dust/group/h1/klesthen/BaryonAsym/H1LambdaC/AnalysisResultFiles/AnalysisEta_All.root 							
rm /data/dust/group/h1/klesthen/BaryonAsym/H1LambdaC/AnalysisResultFiles/AnalysisOutput.root
#######################################################################
#######################################################################
#######################################################################
###2005 Eta Data
#Before calling root macro, need to write command to edit line defining num of bin entries
#C++ will not let this be a variable input since it defines array lengths
sed -i '35c\'"const int q2entries = 10;" "/data/dust/group/h1/klesthen/BaryonAsym/H1LambdaC/Analysis.C"
root -l -q '/data/dust/group/h1/klesthen/BaryonAsym/H1LambdaC/Analysis.C(
3,
{-1.3, -1.0, -0.75, -0.5, -0.25, 0, 0.25, 0.5, 0.75, 1.0, 1.3},
106823.1,
1087523.00,
1133362.00,										
"/data/dust/group/h1/klesthen/BaryonAsym/H1LambdaC/KaonTree/2005/merged.root", 	
"/data/dust/group/h1/klesthen/BaryonAsym/H1LambdaC/LambdaTree/2005/merged.root", 			
"/data/dust/group/h1/klesthen/BaryonAsym/H1LambdaC_MC/KaonTreeMC/RAPGAP31/2005/merged.root", 		
"/data/dust/group/h1/klesthen/BaryonAsym/H1LambdaC_MC/LambdaTreeMC/RAPGAP31/2005/merged.root", 		
"/data/dust/group/h1/klesthen/BaryonAsym/H1LambdaC_MC/genMCTree/RAPGAP31/Rad/2005/merged.root", 		
"/data/dust/group/h1/klesthen/BaryonAsym/H1LambdaC_MC/genMCTree/DJANGOH14/NonRad/2005/merged.root", 		
"/data/dust/group/h1/klesthen/BaryonAsym/H1LambdaC_MC/genMCTree/DJANGOH14/Rad/2005/merged.root" 		
)' > ../AnalysisResultFiles/AnalysisEta_2005.txt	
/data/dust/group/h1/klesthen/BaryonAsym/H1LambdaC/AnalysisResultFiles									
cp /data/dust/group/h1/klesthen/BaryonAsym/H1LambdaC/AnalysisResultFiles/AnalysisOutput.root /data/dust/group/h1/klesthen/BaryonAsym/H1LambdaC/AnalysisResultFiles/AnalysisEta_2005.root 							
rm /data/dust/group/h1/klesthen/BaryonAsym/H1LambdaC/AnalysisResultFiles/AnalysisOutput.root
#######################################################################
#######################################################################
#######################################################################
###2006 Eta Data
#Before calling root macro, need to write command to edit line defining num of bin entries
#C++ will not let this be a variable input since it defines array lengths
sed -i '35c\'"const int q2entries = 10;" "/data/dust/group/h1/klesthen/BaryonAsym/H1LambdaC/Analysis.C"
root -l -q '/data/dust/group/h1/klesthen/BaryonAsym/H1LambdaC/Analysis.C(
3,
{-1.3, -1.0, -0.75, -0.5, -0.25, 0, 0.25, 0.5, 0.75, 1.0, 1.3},
144634.0,
2268210.00,
1944680.00,											
"/data/dust/group/h1/klesthen/BaryonAsym/H1LambdaC/KaonTree/2006/merged.root", 	
"/data/dust/group/h1/klesthen/BaryonAsym/H1LambdaC/LambdaTree/2006/merged.root", 			
"/data/dust/group/h1/klesthen/BaryonAsym/H1LambdaC_MC/KaonTreeMC/RAPGAP31/2006/merged.root", 		
"/data/dust/group/h1/klesthen/BaryonAsym/H1LambdaC_MC/LambdaTreeMC/RAPGAP31/2006/merged.root", 		
"/data/dust/group/h1/klesthen/BaryonAsym/H1LambdaC_MC/genMCTree/RAPGAP31/Rad/2006/merged.root", 		
"/data/dust/group/h1/klesthen/BaryonAsym/H1LambdaC_MC/genMCTree/DJANGOH14/NonRad/2006/merged.root", 		
"/data/dust/group/h1/klesthen/BaryonAsym/H1LambdaC_MC/genMCTree/DJANGOH14/Rad/2006/merged.root" 		
)' > ../AnalysisResultFiles/AnalysisEta_2006.txt	
/data/dust/group/h1/klesthen/BaryonAsym/H1LambdaC/AnalysisResultFiles									
cp /data/dust/group/h1/klesthen/BaryonAsym/H1LambdaC/AnalysisResultFiles/AnalysisOutput.root /data/dust/group/h1/klesthen/BaryonAsym/H1LambdaC/AnalysisResultFiles/AnalysisEta_2006.root 							
rm /data/dust/group/h1/klesthen/BaryonAsym/H1LambdaC/AnalysisResultFiles/AnalysisOutput.root
#######################################################################
###2007 Eta Data
#Before calling root macro, need to write command to edit line defining num of bin entries
#C++ will not let this be a variable input since it defines array lengths
sed -i '35c\'"const int q2entries = 10;" "/data/dust/group/h1/klesthen/BaryonAsym/H1LambdaC/Analysis.C"
root -l -q '/data/dust/group/h1/klesthen/BaryonAsym/H1LambdaC/Analysis.C(
3,
{-1.3, -1.0, -0.75, -0.5, -0.25, 0, 0.25, 0.5, 0.75, 1.0, 1.3},
46200.7,
753224.00,
482191.00,										
"/data/dust/group/h1/klesthen/BaryonAsym/H1LambdaC/KaonTree/2007/merged.root", 	
"/data/dust/group/h1/klesthen/BaryonAsym/H1LambdaC/LambdaTree/2007/merged.root", 			
"/data/dust/group/h1/klesthen/BaryonAsym/H1LambdaC_MC/KaonTreeMC/RAPGAP31/2007/merged.root", 		
"/data/dust/group/h1/klesthen/BaryonAsym/H1LambdaC_MC/LambdaTreeMC/RAPGAP31/2007/merged.root", 		
"/data/dust/group/h1/klesthen/BaryonAsym/H1LambdaC_MC/genMCTree/RAPGAP31/Rad/2007/merged.root", 		
"/data/dust/group/h1/klesthen/BaryonAsym/H1LambdaC_MC/genMCTree/DJANGOH14/NonRad/2007/merged.root", 		
"/data/dust/group/h1/klesthen/BaryonAsym/H1LambdaC_MC/genMCTree/DJANGOH14/Rad/2007/merged.root" 		
)' > ../AnalysisResultFiles/AnalysisEta_2007.txt	
/data/dust/group/h1/klesthen/BaryonAsym/H1LambdaC/AnalysisResultFiles									
cp /data/dust/group/h1/klesthen/BaryonAsym/H1LambdaC/AnalysisResultFiles/AnalysisOutput.root /data/dust/group/h1/klesthen/BaryonAsym/H1LambdaC/AnalysisResultFiles/AnalysisEta_2007.root 							
rm /data/dust/group/h1/klesthen/BaryonAsym/H1LambdaC/AnalysisResultFiles/AnalysisOutput.root
#######################################################################




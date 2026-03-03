#!/bin/zsh

####DATA - Event Lists

cd /data/dust/group/h1/gtustin/h1oo/H1LambdaC/EventLists/2005
rm merged.root
hadd -k -f merged.root ***.root
cp merged.root /data/dust/group/h1/gtustin/h1oo/H1LambdaC/EventLists/Events2005.root

cd /data/dust/group/h1/gtustin/h1oo/H1LambdaC/EventLists/2006
rm merged.root
hadd -k -f merged.root ***.root
cp merged.root /data/dust/group/h1/gtustin/h1oo/H1LambdaC/EventLists/Events2006.root

cd /data/dust/group/h1/gtustin/h1oo/H1LambdaC/EventLists/2007
rm merged.root
hadd -k -f merged.root ***.root
cp merged.root /data/dust/group/h1/gtustin/h1oo/H1LambdaC/EventLists/Events2007.root

cd /data/dust/group/h1/gtustin/h1oo/H1LambdaC/EventLists/2007med
rm merged.root
hadd -k -f merged.root ***.root

cd /data/dust/group/h1/gtustin/h1oo/H1LambdaC/EventLists/2007low
rm merged.root
hadd -k -f merged.root ***.root

cd /data/dust/group/h1/gtustin/h1oo/H1LambdaC/EventLists
rm merged.root
hadd -k -f merged.root ***.root



####Monte Carlo - Event Lists
#######RAPGAP31
cd /data/dust/group/h1/gtustin/h1oo/H1LambdaC_MC/EventLists/RAPGAP31/2005
rm merged.root
hadd -k -f merged.root ***.root
cp merged.root /data/dust/group/h1/gtustin/h1oo/H1LambdaC_MC/EventLists/RAPGAP31/Events_RAPGAP31_2005.root

cd /data/dust/group/h1/gtustin/h1oo/H1LambdaC_MC/EventLists/RAPGAP31/2006
rm merged.root
hadd -k -f merged.root ***.root
cp merged.root /data/dust/group/h1/gtustin/h1oo/H1LambdaC_MC/EventLists/RAPGAP31/Events_RAPGAP31_2006.root

cd /data/dust/group/h1/gtustin/h1oo/H1LambdaC_MC/EventLists/RAPGAP31/2007
rm merged.root
hadd -k -f merged.root ***.root
cp merged.root /data/dust/group/h1/gtustin/h1oo/H1LambdaC_MC/EventLists/RAPGAP31/Events_RAPGAP31_2007.root

cd /data/dust/group/h1/gtustin/h1oo/H1LambdaC_MC/EventLists/RAPGAP31
rm merged.root
hadd -k -f merged.root ***.root

####Monte Carlo - Event Lists
#######DJANGOH14
cd /data/dust/group/h1/gtustin/h1oo/H1LambdaC_MC/EventLists/DJANGOH14/2005
rm merged.root
hadd -k -f merged.root ***.root
cp merged.root /data/dust/group/h1/gtustin/h1oo/H1LambdaC_MC/EventLists/DJANGOH14/Events_DJANGOH14_2005.root

cd /data/dust/group/h1/gtustin/h1oo/H1LambdaC_MC/EventLists/DJANGOH14/2006
rm merged.root
hadd -k -f merged.root ***.root
cp merged.root /data/dust/group/h1/gtustin/h1oo/H1LambdaC_MC/EventLists/DJANGOH14/Events_DJANGOH14_2006.root

cd /data/dust/group/h1/gtustin/h1oo/H1LambdaC_MC/EventLists/DJANGOH14/2007
rm merged.root
hadd -k -f merged.root ***.root
cp merged.root /data/dust/group/h1/gtustin/h1oo/H1LambdaC_MC/EventLists/DJANGOH14/Events_DJANGOH14_2007.root

cd /data/dust/group/h1/gtustin/h1oo/H1LambdaC_MC/EventLists/DJANGOH14
rm merged.root
hadd -k -f merged.root ***.root

#####################################################################################################################
#####################################################################################################################
#####################################################################################################################


####DATA - Candidates

cd /data/dust/group/h1/gtustin/h1oo/H1LambdaC/KaonTree/2005
rm merged.root
hadd -k -f merged.root ***.root
cp merged.root /data/dust/group/h1/gtustin/h1oo/H1LambdaC/KaonTree/Candidates2005.root

cd /data/dust/group/h1/gtustin/h1oo/H1LambdaC/KaonTree/2006
rm merged.root
hadd -k -f merged.root ***.root
cp merged.root /data/dust/group/h1/gtustin/h1oo/H1LambdaC/KaonTree/Candidates2006.root

cd /data/dust/group/h1/gtustin/h1oo/H1LambdaC/KaonTree/2007
rm merged.root
hadd -k -f merged.root ***.root
cp merged.root /data/dust/group/h1/gtustin/h1oo/H1LambdaC/KaonTree/Candidates2007.root

cd /data/dust/group/h1/gtustin/h1oo/H1LambdaC/KaonTree/2007low
rm merged.root
hadd -k -f merged.root ***.root

cd /data/dust/group/h1/gtustin/h1oo/H1LambdaC/KaonTree/2007med
rm merged.root
hadd -k -f merged.root ***.root

cd /data/dust/group/h1/gtustin/h1oo/H1LambdaC/KaonTree
rm merged.root
hadd -k -f merged.root ***.root



cd /data/dust/group/h1/gtustin/h1oo/H1LambdaC/LambdaTree/2005
rm merged.root
hadd -k -f merged.root ***.root
cp merged.root /data/dust/group/h1/gtustin/h1oo/H1LambdaC/LambdaTree/Candidates2005.root

cd /data/dust/group/h1/gtustin/h1oo/H1LambdaC/LambdaTree/2006
rm merged.root
hadd -k -f merged.root ***.root
cp merged.root /data/dust/group/h1/gtustin/h1oo/H1LambdaC/LambdaTree/Candidates2006.root

cd /data/dust/group/h1/gtustin/h1oo/H1LambdaC/LambdaTree/2007
rm merged.root
hadd -k -f merged.root ***.root
cp merged.root /data/dust/group/h1/gtustin/h1oo/H1LambdaC/LambdaTree/Candidates2007.root

cd /data/dust/group/h1/gtustin/h1oo/H1LambdaC/LambdaTree/2007low
rm merged.root
hadd -k -f merged.root ***.root

cd /data/dust/group/h1/gtustin/h1oo/H1LambdaC/LambdaTree/2007med
rm merged.root
hadd -k -f merged.root ***.root

cd /data/dust/group/h1/gtustin/h1oo/H1LambdaC/LambdaTree
rm merged.root
hadd -k -f merged.root ***.root

####Monte Carlo - Candidates
#######DJANGOH14
cd /data/dust/group/h1/gtustin/h1oo/H1LambdaC_MC/KaonTreeMC/DJANGOH14/2005
rm merged.root
hadd -k -f merged.root ***.root
cp merged.root /data/dust/group/h1/gtustin/h1oo/H1LambdaC_MC/KaonTreeMC/DJANGOH14/Candidates2005.root

cd /data/dust/group/h1/gtustin/h1oo/H1LambdaC_MC/KaonTreeMC/DJANGOH14/2006
rm merged.root
hadd -k -f merged.root ***.root
cp merged.root /data/dust/group/h1/gtustin/h1oo/H1LambdaC_MC/KaonTreeMC/DJANGOH14/Candidates2006.root

cd /data/dust/group/h1/gtustin/h1oo/H1LambdaC_MC/KaonTreeMC/DJANGOH14/2007
rm merged.root
hadd -k -f merged.root ***.root
cp merged.root /data/dust/group/h1/gtustin/h1oo/H1LambdaC_MC/KaonTreeMC/DJANGOH14/Candidates2007.root

cd /data/dust/group/h1/gtustin/h1oo/H1LambdaC_MC/KaonTreeMC/DJANGOH14
rm merged.root
hadd -k -f merged.root ***.root



cd /data/dust/group/h1/gtustin/h1oo/H1LambdaC_MC/LambdaTreeMC/DJANGOH14/2005
rm merged.root
hadd -k -f merged.root ***.root
cp merged.root /data/dust/group/h1/gtustin/h1oo/H1LambdaC_MC/LambdaTreeMC/DJANGOH14/Candidates2005.root

cd /data/dust/group/h1/gtustin/h1oo/H1LambdaC_MC/LambdaTreeMC/DJANGOH14/2006
rm merged.root
hadd -k -f merged.root ***.root
cp merged.root /data/dust/group/h1/gtustin/h1oo/H1LambdaC_MC/LambdaTreeMC/DJANGOH14/Candidates2006.root

cd /data/dust/group/h1/gtustin/h1oo/H1LambdaC_MC/LambdaTreeMC/DJANGOH14/2007
rm merged.root
hadd -k -f merged.root ***.root
cp merged.root /data/dust/group/h1/gtustin/h1oo/H1LambdaC_MC/LambdaTreeMC/DJANGOH14/Candidates2007.root

cd /data/dust/group/h1/gtustin/h1oo/H1LambdaC_MC/LambdaTreeMC/DJANGOH14
rm merged.root
hadd -k -f merged.root ***.root



####Monte Carlo - Candidates
#######RAPGAP31
cd /data/dust/group/h1/gtustin/h1oo/H1LambdaC_MC/KaonTreeMC/RAPGAP31/2005
rm merged.root
hadd -k -f merged.root ***.root
cp merged.root /data/dust/group/h1/gtustin/h1oo/H1LambdaC_MC/KaonTreeMC/RAPGAP31/Candidates2005.root

cd /data/dust/group/h1/gtustin/h1oo/H1LambdaC_MC/KaonTreeMC/RAPGAP31/2006
rm merged.root
hadd -k -f merged.root ***.root
cp merged.root /data/dust/group/h1/gtustin/h1oo/H1LambdaC_MC/KaonTreeMC/RAPGAP31/Candidates2006.root

cd /data/dust/group/h1/gtustin/h1oo/H1LambdaC_MC/KaonTreeMC/RAPGAP31/2007
rm merged.root
hadd -k -f merged.root ***.root
cp merged.root /data/dust/group/h1/gtustin/h1oo/H1LambdaC_MC/KaonTreeMC/RAPGAP31/Candidates2007.root

cd /data/dust/group/h1/gtustin/h1oo/H1LambdaC_MC/KaonTreeMC/RAPGAP31
rm merged.root
hadd -k -f merged.root ***.root



cd /data/dust/group/h1/gtustin/h1oo/H1LambdaC_MC/LambdaTreeMC/RAPGAP31/2005
rm merged.root
hadd -k -f merged.root ***.root
cp merged.root /data/dust/group/h1/gtustin/h1oo/H1LambdaC_MC/LambdaTreeMC/RAPGAP31/Candidates2005.root

cd /data/dust/group/h1/gtustin/h1oo/H1LambdaC_MC/LambdaTreeMC/RAPGAP31/2006
rm merged.root
hadd -k -f merged.root ***.root
cp merged.root /data/dust/group/h1/gtustin/h1oo/H1LambdaC_MC/LambdaTreeMC/RAPGAP31/Candidates2006.root

cd /data/dust/group/h1/gtustin/h1oo/H1LambdaC_MC/LambdaTreeMC/RAPGAP31/2007
rm merged.root
hadd -k -f merged.root ***.root
cp merged.root /data/dust/group/h1/gtustin/h1oo/H1LambdaC_MC/LambdaTreeMC/RAPGAP31/Candidates2007.root

cd /data/dust/group/h1/gtustin/h1oo/H1LambdaC_MC/LambdaTreeMC/RAPGAP31
rm merged.root
hadd -k -f merged.root ***.root



####Monte Carlo - genMCTree
#######DJANGOH14 Rad
cd /data/dust/group/h1/gtustin/h1oo/H1LambdaC_MC/genMCTree/DJANGOH14/Rad/2005
rm merged.root
hadd -k -f merged.root ***.root
cp merged.root /data/dust/group/h1/gtustin/h1oo/H1LambdaC_MC/genMCTree/DJANGOH14/Rad/genMCTree2005.root

cd /data/dust/group/h1/gtustin/h1oo/H1LambdaC_MC/genMCTree/DJANGOH14/Rad/2006
rm merged.root
hadd -k -f merged.root ***.root
cp merged.root /data/dust/group/h1/gtustin/h1oo/H1LambdaC_MC/genMCTree/DJANGOH14/Rad/genMCTree2006.root

cd /data/dust/group/h1/gtustin/h1oo/H1LambdaC_MC/genMCTree/DJANGOH14/Rad/2007
rm merged.root
hadd -k -f merged.root ***.root
cp merged.root /data/dust/group/h1/gtustin/h1oo/H1LambdaC_MC/genMCTree/DJANGOH14/Rad/genMCTree2007.root

cd /data/dust/group/h1/gtustin/h1oo/H1LambdaC_MC/genMCTree/DJANGOH14/Rad
rm merged.root
hadd -k -f merged.root ***.root
#######DJANGOH14 NonRad
cd /data/dust/group/h1/gtustin/h1oo/H1LambdaC_MC/genMCTree/DJANGOH14/NonRad/2005
rm merged.root
hadd -k -f merged.root ***.root
cp merged.root /data/dust/group/h1/gtustin/h1oo/H1LambdaC_MC/genMCTree/DJANGOH14/NonRad/genMCTree2005.root

cd /data/dust/group/h1/gtustin/h1oo/H1LambdaC_MC/genMCTree/DJANGOH14/NonRad/2006
rm merged.root
hadd -k -f merged.root ***.root
cp merged.root /data/dust/group/h1/gtustin/h1oo/H1LambdaC_MC/genMCTree/DJANGOH14/NonRad/genMCTree2006.root

cd /data/dust/group/h1/gtustin/h1oo/H1LambdaC_MC/genMCTree/DJANGOH14/NonRad/2007
rm merged.root
hadd -k -f merged.root ***.root
cp merged.root /data/dust/group/h1/gtustin/h1oo/H1LambdaC_MC/genMCTree/DJANGOH14/NonRad/genMCTree2007.root

cd /data/dust/group/h1/gtustin/h1oo/H1LambdaC_MC/genMCTree/DJANGOH14/NonRad
rm merged.root
hadd -k -f merged.root ***.root

#######RAPGAP31 Rad
cd /data/dust/group/h1/gtustin/h1oo/H1LambdaC_MC/genMCTree/RAPGAP31/Rad/2005
rm merged.root
hadd -k -f merged.root ***.root
cp merged.root /data/dust/group/h1/gtustin/h1oo/H1LambdaC_MC/genMCTree/RAPGAP31/Rad/genMCTree2005.root

cd /data/dust/group/h1/gtustin/h1oo/H1LambdaC_MC/genMCTree/RAPGAP31/Rad/2006
rm merged.root
hadd -k -f merged.root ***.root
cp merged.root /data/dust/group/h1/gtustin/h1oo/H1LambdaC_MC/genMCTree/RAPGAP31/Rad/genMCTree2006.root

cd /data/dust/group/h1/gtustin/h1oo/H1LambdaC_MC/genMCTree/RAPGAP31/Rad/2007
rm merged.root
hadd -k -f merged.root ***.root
cp merged.root /data/dust/group/h1/gtustin/h1oo/H1LambdaC_MC/genMCTree/RAPGAP31/Rad/genMCTree2007.root

cd /data/dust/group/h1/gtustin/h1oo/H1LambdaC_MC/genMCTree/RAPGAP31/Rad
rm merged.root
hadd -k -f merged.root ***.root
#######RAPGAP31 NonRad
cd /data/dust/group/h1/gtustin/h1oo/H1LambdaC_MC/genMCTree/RAPGAP31/NonRad/2005
rm merged.root
hadd -k -f merged.root ***.root
cp merged.root /data/dust/group/h1/gtustin/h1oo/H1LambdaC_MC/genMCTree/RAPGAP31/NonRad/genMCTree2005.root

cd /data/dust/group/h1/gtustin/h1oo/H1LambdaC_MC/genMCTree/RAPGAP31/NonRad/2006
rm merged.root
hadd -k -f merged.root ***.root
cp merged.root /data/dust/group/h1/gtustin/h1oo/H1LambdaC_MC/genMCTree/RAPGAP31/NonRad/genMCTree2006.root

cd /data/dust/group/h1/gtustin/h1oo/H1LambdaC_MC/genMCTree/RAPGAP31/NonRad/2007
rm merged.root
hadd -k -f merged.root ***.root
cp merged.root /data/dust/group/h1/gtustin/h1oo/H1LambdaC_MC/genMCTree/RAPGAP31/NonRad/genMCTree2007.root

cd /data/dust/group/h1/gtustin/h1oo/H1LambdaC_MC/genMCTree/RAPGAP31/NonRad
rm merged.root
hadd -k -f merged.root ***.root
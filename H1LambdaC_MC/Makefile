# A prototype Makefile for H1 MC
#
# last update: $Date: 2010/10/08 13:39:31 $
# by         : $Author: msteder $
#
# The name of this module
MODULENAME = H1Examples
#
# macro definitions
# DEFINES = 
#
# debug on or off (comment next line to disable debugger information)
DEBUG = -g 
#
# The shared library to make (if any): e.g. libH1Tracks.so
# SLIB_TARGETS = 
SLIB = libUser.so
#
# The static library to make (if any): e.g. libH1Tracks.a
# LIB_TARGETS = 
LIB = 
#
# The binaries you would like to make, you have to provide the final linking
BIN = kaonfind_ods kaonfind lambdaMC lambdaTreeMC lambdaCTreeMC kaonMC kaonTreeMC binfindl binfindk lcuttests kcuttests create_evlsMC create_evlsMCcharm create_evlsSCB create_evlsMCchannel genlevelcount genMCTree test ratiopt ratioptnew 
# cintsteering

ifeq ($(STANDALONE),yes)
  # no rerun_rec
else
  BIN += rerun_rec 
endif

#
# Objects you need for the libraries
OBJS = UserEvent.o UserFiller.o

# OBJS += UserEvent.o UserFiller.o

# Dictionary objects, default is: DICTOBJS = $(OBJS:.o=Dict.o)

# Now your special libs and includes
# EXTRA_INCLUDEDIRS = ~/my_favourite_library/include
# EXTRA_LIBDIRS = ~/my_favourite_library/lib
# EXTRA_LINKFLAGS = -lmy_favourite_library
# Dummy objects
# DUMMYOBJS =

# Dummy dictionary objects
# DUMMYDICTOBJS = $(DUMMYOBJS:.o=Dict.o)

#extra files to be removed when calling 'make clean' or 'make clobber'
EXTRA_CLEAN_RULES=TLorentzVectorPtr.C
EXTRA_DISTCLEAN_RULES=TLorentzVectorPtr.h

#extra files for fjcontrib for H1Example 'centauro'
#FASTJET_VERS = $(FASTJET__HOME)/lib 
ifdef FJCONTRIB__HOME
  EXTRA_INCLUDEDIRS = $(FASTJET__HOME)/include $(FJCONTRIB__HOME)/include
endif

# defines LIBS
# defines Rules for .c .C .cpp .f Dict.C .so
ifeq ($(H1BASICMK),local)
  include  ../oo_tools/H1Basic.mk
else
  include  $(H1DIST)/releases/$(H1CURRENT)/oo_tools/H1Basic.mk
endif                                                             

# include the dependencies if created via 'make depend'
-include Makefile.dep


# the executables

kaonfind_ods: outputdirs $(BUILDDIR)/kaonfind_ods.o
	@echo ""
	@echo Linking physics test program for ods access: $@ ...
	$(LOADER) \
		$(BUILDDIR)/kaonfind_ods.o  \
		\
		$(OO_LIBS) \
		-lH1Mods -lH1Hat -lH1Ods -lH1Skeleton -lH1Tracks -lH1Clusters \
		-lH1OOBanks -lH1Geom -lH1Arrays -lH1Tools -lH1Pointers -lH1Steering \
		-lH1HadronicCalibration  -lH1PhysUtils -lH1JetFinder -lH1Finder \
		-lH1MagfieldOO \
		$(ROOTLIBS) -lEG -l${FORTRANLIB} \
		\
		-o $(BINDIR)/kaonfind_ods
	@echo $@ created ...
	@echo ""

kaonfind: outputdirs $(BUILDDIR)/kaonfind.o
	@echo ""
	@echo Linking physics test program for ods access: $@ ...
	$(LOADER) \
		$(BUILDDIR)/kaonfind.o  \
		\
		$(OO_LIBS) \
		-lH1Mods -lH1Hat -lH1Ods -lH1Skeleton -lH1Tracks -lH1Clusters \
		-lH1OOBanks -lH1Geom -lH1Arrays -lH1Tools -lH1Pointers -lH1Steering \
		-lH1HadronicCalibration  -lH1PhysUtils -lH1JetFinder -lH1Finder \
		-lH1MagfieldOO \
		$(ROOTLIBS) -lEG -l${FORTRANLIB} \
		\
		-o $(BINDIR)/kaonfind
	@echo $@ created ...
	@echo ""

lambdaMC: outputdirs $(BUILDDIR)/lambdaMC.o
	@echo ""
	@echo Linking physics test program for ods access: $@ ...
	$(LOADER) \
		$(BUILDDIR)/lambdaMC.o  \
		\
		$(OO_LIBS) \
		-lH1Mods -lH1Hat -lH1Ods -lH1Skeleton -lH1Tracks -lH1Clusters \
		-lH1OOBanks -lH1Geom -lH1Arrays -lH1Tools -lH1Pointers -lH1Steering \
		-lH1HadronicCalibration  -lH1PhysUtils -lH1JetFinder -lH1Finder \
		-lH1MagfieldOO \
		$(ROOTLIBS) -lEG -l${FORTRANLIB} \
		\
		-o $(BINDIR)/lambdaMC
	@echo $@ created ...
	@echo ""


lambdaTreeMC: outputdirs $(BUILDDIR)/lambdaTreeMC.o
	@echo ""
	@echo Linking physics test program for ods access: $@ ...
	$(LOADER) \
		$(BUILDDIR)/lambdaTreeMC.o  \
		\
		$(OO_LIBS) \
		\
		-lH1Geom -lH1Ods -lH1OOBanks -lH1Tracks -lH1Clusters -lH1Mods -lH1Hat -lH1Skeleton -lH1Steering \
		-lH1Pointers -lH1Arrays -lH1Tools -lH1Benchmarks -lH1Arrays -lH1Binning \
		-lH1PhysUtils  -lH1JetFinder -lH1Finder -lH1HadronicCalibration -lH1CaloTrigger \
		-lH1Calculator -lH1CalcPointers -lH1CalcWeights -lH1SoftLeptonId  -lH1Analysis \
		-lH1MagfieldOO -lH1Cuts	-lH1TrkFinder -lH1Selection -lH1ElecCalibration -lH1QCDFunc $(LHAPDFLIB) \
		\
		-L$(H1_LIBS) -L$(CERN_LIBS) \
		$(ROOTLIBS) -lEG -l${FORTRANLIB} \
		-L$(FJCONTRIB__HOME)/lib -lCentauro \
		\
		-o $(BINDIR)/$@
	@echo $@ created ...
	@echo ""




lambdaCTreeMC: outputdirs $(BUILDDIR)/lambdaCTreeMC.o
	@echo ""
	@echo Linking physics test program for ods access: $@ ...
	$(LOADER) \
		$(BUILDDIR)/lambdaCTreeMC.o  \
		\
		$(OO_LIBS) \
		-lH1Mods -lH1Hat -lH1Ods -lH1Skeleton -lH1Tracks -lH1Clusters \
		-lH1OOBanks -lH1Geom -lH1Arrays -lH1Tools -lH1Pointers -lH1Steering \
		-lH1HadronicCalibration  -lH1PhysUtils -lH1JetFinder -lH1Finder \
		-lH1MagfieldOO \
		$(ROOTLIBS) -lEG -l${FORTRANLIB} \
		\
		-o $(BINDIR)/lambdaCTreeMC
	@echo $@ created ...
	@echo ""

kaonMC: outputdirs $(BUILDDIR)/kaonMC.o
	@echo ""
	@echo Linking physics test program for ods access: $@ ...
	$(LOADER) \
		$(BUILDDIR)/kaonMC.o  \
		\
		$(OO_LIBS) \
		-lH1Mods -lH1Hat -lH1Ods -lH1Skeleton -lH1Tracks -lH1Clusters \
		-lH1OOBanks -lH1Geom -lH1Arrays -lH1Tools -lH1Pointers -lH1Steering \
		-lH1HadronicCalibration  -lH1PhysUtils -lH1JetFinder -lH1Finder \
		-lH1MagfieldOO \
		$(ROOTLIBS) -lEG -l${FORTRANLIB} \
		\
		-o $(BINDIR)/kaonMC
	@echo $@ created ...
	@echo ""

kaonTreeMC: outputdirs $(BUILDDIR)/kaonTreeMC.o
	@echo ""
	@echo Linking physics test program for ods access: $@ ...
	$(LOADER) \
		$(BUILDDIR)/kaonTreeMC.o  \
		\
		$(OO_LIBS) \
		\
		-lH1Geom -lH1Ods -lH1OOBanks -lH1Tracks -lH1Clusters -lH1Mods -lH1Hat -lH1Skeleton -lH1Steering \
		-lH1Pointers -lH1Arrays -lH1Tools -lH1Benchmarks -lH1Arrays -lH1Binning \
		-lH1PhysUtils  -lH1JetFinder -lH1Finder -lH1HadronicCalibration -lH1CaloTrigger \
		-lH1Calculator -lH1CalcPointers -lH1CalcWeights -lH1SoftLeptonId  -lH1Analysis \
		-lH1MagfieldOO -lH1Cuts	-lH1TrkFinder -lH1Selection -lH1ElecCalibration -lH1QCDFunc $(LHAPDFLIB) \
		\
		-L$(H1_LIBS) -L$(CERN_LIBS) \
		$(ROOTLIBS) -lEG -l${FORTRANLIB} \
		-L$(FJCONTRIB__HOME)/lib -lCentauro \
		\
		-o $(BINDIR)/$@
	@echo $@ created ...
	@echo ""



binfindl: outputdirs $(BUILDDIR)/binfindl.o
	@echo ""
	@echo Linking physics test program for ods access: $@ ...
	$(LOADER) \
		$(BUILDDIR)/binfindl.o  \
		\
		$(OO_LIBS) \
		-lH1Mods -lH1Hat -lH1Ods -lH1Skeleton -lH1Tracks -lH1Clusters \
		-lH1OOBanks -lH1Geom -lH1Arrays -lH1Tools -lH1Pointers -lH1Steering \
		-lH1HadronicCalibration  -lH1PhysUtils -lH1JetFinder -lH1Finder \
		-lH1MagfieldOO \
		$(ROOTLIBS) -lEG -l${FORTRANLIB} \
		\
		-o $(BINDIR)/binfindl
	@echo $@ created ...
	@echo ""

binfindk: outputdirs $(BUILDDIR)/binfindk.o
	@echo ""
	@echo Linking physics test program for ods access: $@ ...
	$(LOADER) \
		$(BUILDDIR)/binfindk.o  \
		\
		$(OO_LIBS) \
		-lH1Mods -lH1Hat -lH1Ods -lH1Skeleton -lH1Tracks -lH1Clusters \
		-lH1OOBanks -lH1Geom -lH1Arrays -lH1Tools -lH1Pointers -lH1Steering \
		-lH1HadronicCalibration  -lH1PhysUtils -lH1JetFinder -lH1Finder \
		-lH1MagfieldOO \
		$(ROOTLIBS) -lEG -l${FORTRANLIB} \
		\
		-o $(BINDIR)/binfindk
	@echo $@ created ...
	@echo ""

lcuttests: outputdirs $(BUILDDIR)/lcuttests.o
	@echo ""
	@echo Linking physics test program for ods access: $@ ...
	$(LOADER) \
		$(BUILDDIR)/lcuttests.o  \
		\
		$(OO_LIBS) \
		-lH1Mods -lH1Hat -lH1Ods -lH1Skeleton -lH1Tracks -lH1Clusters \
		-lH1OOBanks -lH1Geom -lH1Arrays -lH1Tools -lH1Pointers -lH1Steering \
		-lH1HadronicCalibration  -lH1PhysUtils -lH1JetFinder -lH1Finder \
		-lH1MagfieldOO \
		$(ROOTLIBS) -lEG -l${FORTRANLIB} \
		\
		-o $(BINDIR)/lcuttests
	@echo $@ created ...
	@echo ""

kcuttests: outputdirs $(BUILDDIR)/kcuttests.o
	@echo ""
	@echo Linking physics test program for ods access: $@ ...
	$(LOADER) \
		$(BUILDDIR)/kcuttests.o  \
		\
		$(OO_LIBS) \
		-lH1Mods -lH1Hat -lH1Ods -lH1Skeleton -lH1Tracks -lH1Clusters \
		-lH1OOBanks -lH1Geom -lH1Arrays -lH1Tools -lH1Pointers -lH1Steering \
		-lH1HadronicCalibration  -lH1PhysUtils -lH1JetFinder -lH1Finder \
		-lH1MagfieldOO \
		$(ROOTLIBS) -lEG -l${FORTRANLIB} \
		\
		-o $(BINDIR)/kcuttests
	@echo $@ created ...
	@echo ""

create_evlsMC: outputdirs $(BUILDDIR)/create_evlsMC.o
	@echo ""
	@echo Linking physics test program for ods access: $@ ...
	$(LOADER) \
		$(BUILDDIR)/create_evlsMC.o  \
		\
		$(OO_LIBS) \
		\
		-lH1Geom -lH1Ods -lH1OOBanks -lH1Tracks -lH1Clusters -lH1Mods -lH1Hat -lH1Skeleton -lH1Steering \
		-lH1Pointers -lH1Arrays -lH1Tools -lH1Benchmarks -lH1Arrays -lH1Binning \
		-lH1PhysUtils  -lH1JetFinder -lH1Finder -lH1HadronicCalibration -lH1CaloTrigger \
		-lH1Calculator -lH1CalcPointers -lH1CalcWeights -lH1SoftLeptonId  -lH1Analysis \
		-lH1MagfieldOO -lH1Cuts	-lH1TrkFinder -lH1Selection -lH1ElecCalibration -lH1QCDFunc $(LHAPDFLIB) \
		\
		-L$(H1_LIBS) -L$(CERN_LIBS) \
		$(ROOTLIBS) -lEG -l${FORTRANLIB} \
		-L$(FJCONTRIB__HOME)/lib -lCentauro \
		\
		-o $(BINDIR)/$@
	@echo $@ created ...
	@echo ""


create_evlsMCcharm: outputdirs $(BUILDDIR)/create_evlsMCcharm.o
	@echo ""
	@echo Linking physics test program for ods access: $@ ...
	$(LOADER) \
		$(BUILDDIR)/create_evlsMCcharm.o \
		\
		$(OO_LIBS) \
		\
		-lH1Geom -lH1Ods -lH1OOBanks -lH1Tracks -lH1Clusters -lH1Mods -lH1Hat -lH1Skeleton -lH1Steering \
		-lH1Pointers -lH1Arrays -lH1Tools -lH1Benchmarks -lH1Arrays -lH1Binning \
		-lH1PhysUtils  -lH1JetFinder -lH1Finder -lH1HadronicCalibration -lH1CaloTrigger \
		-lH1Calculator -lH1CalcPointers -lH1CalcWeights -lH1SoftLeptonId  -lH1Analysis \
		-lH1MagfieldOO -lH1Cuts	-lH1TrkFinder -lH1Selection -lH1ElecCalibration -lH1QCDFunc  -l${LHAPDFLIB} \
		\
		-L$(H1_LIBS) -L$(CERN_LIBS) \
		$(ROOTLIBS) -lEG -l${FORTRANLIB} \
		-L$(FJCONTRIB__HOME)/lib -lCentauro \
                -L$(FASTJET__HOME)/lib -lfastjet \
		\
		-o $(BINDIR)/$@
	@echo ""
	#@echo "copying .pcm files to libdir"
	#cp *.pcm $(LIBDIR)
	@echo ""
	@echo $@ created ...
	@echo Please run: $(BINDIR)/$@
	@echo ""

create_evlsHERA1MC: outputdirs $(BUILDDIR)/create_evlsHERA1MC.o
	@echo ""
	@echo Linking physics test program for ods access: $@ ...
	$(LOADER) \
		$(BUILDDIR)/create_evlsHERA1MC.o \
		\
		$(OO_LIBS) \
		\
		-lH1Geom -lH1Ods -lH1OOBanks -lH1Tracks -lH1Clusters -lH1Mods -lH1Hat -lH1Skeleton -lH1Steering \
		-lH1Pointers -lH1Arrays -lH1Tools -lH1Benchmarks -lH1Arrays -lH1Binning \
		-lH1PhysUtils  -lH1JetFinder -lH1Finder -lH1HadronicCalibration -lH1CaloTrigger \
		-lH1Calculator -lH1CalcPointers -lH1CalcWeights -lH1SoftLeptonId  -lH1Analysis \
		-lH1MagfieldOO -lH1Cuts	-lH1TrkFinder -lH1Selection -lH1ElecCalibration -lH1QCDFunc  -l${LHAPDFLIB} \
		\
		-L$(H1_LIBS) -L$(CERN_LIBS) \
		$(ROOTLIBS) -lEG -l${FORTRANLIB} \
		-L$(FJCONTRIB__HOME)/lib -lCentauro \
                -L$(FASTJET__HOME)/lib -lfastjet \
		\
		-o $(BINDIR)/$@
	@echo ""
	#@echo "copying .pcm files to libdir"
	#cp *.pcm $(LIBDIR)
	@echo ""
	@echo $@ created ...
	@echo Please run: $(BINDIR)/$@
	@echo ""

create_evlsSCB: outputdirs $(BUILDDIR)/create_evlsSCB.o
	@echo ""
	@echo Linking physics test program for ods access: $@ ...
	$(LOADER) \
		$(BUILDDIR)/create_evlsSCB.o  \
		\
		$(OO_LIBS) \
		-lH1Mods -lH1Hat -lH1Ods -lH1Skeleton -lH1Tracks -lH1Clusters \
		-lH1OOBanks -lH1Geom -lH1Arrays -lH1Tools -lH1Pointers -lH1Steering \
		-lH1HadronicCalibration  -lH1PhysUtils -lH1JetFinder -lH1Finder \
		-lH1MagfieldOO \
		$(ROOTLIBS) -lEG -l${FORTRANLIB} \
		\
		-o $(BINDIR)/create_evlsSCB
	@echo $@ created ...
	@echo ""

create_evlsMCchannel: outputdirs $(BUILDDIR)/create_evlsMCchannel.o
	@echo ""
	@echo Linking physics test program for ods access: $@ ...
	$(LOADER) \
		$(BUILDDIR)/create_evlsMCchannel.o  \
		\
		$(OO_LIBS) \
		-lH1Mods -lH1Hat -lH1Ods -lH1Skeleton -lH1Tracks -lH1Clusters \
		-lH1OOBanks -lH1Geom -lH1Arrays -lH1Tools -lH1Pointers -lH1Steering \
		-lH1HadronicCalibration  -lH1PhysUtils -lH1JetFinder -lH1Finder \
		-lH1MagfieldOO \
		$(ROOTLIBS) -lEG -l${FORTRANLIB} \
		\
		-o $(BINDIR)/create_evlsMCchannel
	@echo $@ created ...
	@echo ""

genlevelcount: outputdirs $(BUILDDIR)/genlevelcount.o
	@echo ""
	@echo Linking physics test program for ods access: $@ ...
	$(LOADER) \
		$(BUILDDIR)/genlevelcount.o  \
		\
		$(OO_LIBS) \
		-lH1Mods -lH1Hat -lH1Ods -lH1Skeleton -lH1Tracks -lH1Clusters \
		-lH1OOBanks -lH1Geom -lH1Arrays -lH1Tools -lH1Pointers -lH1Steering \
		-lH1HadronicCalibration  -lH1PhysUtils -lH1JetFinder -lH1Finder \
		-lH1MagfieldOO \
		$(ROOTLIBS) -lEG -l${FORTRANLIB} \
		\
		-o $(BINDIR)/genlevelcount
	@echo $@ created ...
	@echo ""

genMCTree: outputdirs $(BUILDDIR)/genMCTree.o
	@echo ""
	@echo Linking physics test program for ods access: $@ ...
	$(LOADER) \
		$(BUILDDIR)/genMCTree.o  \
		\
		$(OO_LIBS) \
		\
		-lH1Geom -lH1Ods -lH1OOBanks -lH1Tracks -lH1Clusters -lH1Mods -lH1Hat -lH1Skeleton -lH1Steering \
		-lH1Pointers -lH1Arrays -lH1Tools -lH1Benchmarks -lH1Arrays -lH1Binning \
		-lH1PhysUtils  -lH1JetFinder -lH1Finder -lH1HadronicCalibration -lH1CaloTrigger \
		-lH1Calculator -lH1CalcPointers -lH1CalcWeights -lH1SoftLeptonId  -lH1Analysis \
		-lH1MagfieldOO -lH1Cuts	-lH1TrkFinder -lH1Selection -lH1ElecCalibration -lH1QCDFunc $(LHAPDFLIB) \
		\
		-L$(H1_LIBS) -L$(CERN_LIBS) \
		$(ROOTLIBS) -lEG -l${FORTRANLIB} \
		-L$(FJCONTRIB__HOME)/lib -lCentauro \
		\
		-o $(BINDIR)/$@
	@echo $@ created ...
	@echo ""





test: outputdirs $(BUILDDIR)/test.o
	@echo ""
	@echo Linking physics test program for ods access: $@ ...
	$(LOADER) \
		$(BUILDDIR)/test.o  \
		\
		$(OO_LIBS) \
		-lH1Mods -lH1Hat -lH1Ods -lH1Skeleton -lH1Tracks -lH1Clusters \
		-lH1OOBanks -lH1Geom -lH1Arrays -lH1Tools -lH1Pointers -lH1Steering \
		-lH1HadronicCalibration  -lH1PhysUtils -lH1JetFinder -lH1Finder \
		-lH1MagfieldOO \
		$(ROOTLIBS) -lEG -l${FORTRANLIB} \
		\
		-o $(BINDIR)/test
	@echo $@ created ...
	@echo ""

ratiopt: outputdirs $(BUILDDIR)/ratiopt.o
	@echo ""
	@echo Linking physics test program for ods access: $@ ...
	$(LOADER) \
		$(BUILDDIR)/ratiopt.o  \
		\
		$(OO_LIBS) \
		-lH1Mods -lH1Hat -lH1Ods -lH1Skeleton -lH1Tracks -lH1Clusters \
		-lH1OOBanks -lH1Geom -lH1Arrays -lH1Tools -lH1Pointers -lH1Steering \
		-lH1HadronicCalibration  -lH1PhysUtils -lH1JetFinder -lH1Finder \
		-lH1MagfieldOO \
		$(ROOTLIBS) -lEG -l${FORTRANLIB} \
		\
		-o $(BINDIR)/ratiopt
	@echo $@ created ...
	@echo ""

ratioptnew: outputdirs $(BUILDDIR)/ratioptnew.o
	@echo ""
	@echo Linking physics test program for ods access: $@ ...
	$(LOADER) \
		$(BUILDDIR)/ratioptnew.o  \
		\
		$(OO_LIBS) \
		-lH1Mods -lH1Hat -lH1Ods -lH1Skeleton -lH1Tracks -lH1Clusters \
		-lH1OOBanks -lH1Geom -lH1Arrays -lH1Tools -lH1Pointers -lH1Steering \
		-lH1HadronicCalibration  -lH1PhysUtils -lH1JetFinder -lH1Finder \
		-lH1MagfieldOO \
		$(ROOTLIBS) -lEG -l${FORTRANLIB} \
		\
		-o $(BINDIR)/ratioptnew
	@echo $@ created ...
	@echo ""

# to remove the generated *Ptr.[hC] files:
#clean::
#	rm  *Ptr.[Ch] 2>/dev/null
#	-cd $(BUILDDIR) &&\
#        rm Makefile.dep *.o *~ core *.bak 2>/dev/null


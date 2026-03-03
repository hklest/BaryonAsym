# A prototype Makefile for H1
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
BIN = kaonfind_ods kaonfind lambda lambdaTree lambdaCTree kaon kaonTree photonconv binfindl binfindk lcuttests kcuttests create_evls ratiopt ratioptnew LambdaDists
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

lambda: outputdirs $(BUILDDIR)/lambda.o
	@echo ""
	@echo Linking physics test program for ods access: $@ ...
	$(LOADER) \
		$(BUILDDIR)/lambda.o  \
		\
		$(OO_LIBS) \
		-lH1Mods -lH1Hat -lH1Ods -lH1Skeleton -lH1Tracks -lH1Clusters \
		-lH1OOBanks -lH1Geom -lH1Arrays -lH1Tools -lH1Pointers -lH1Steering \
		-lH1HadronicCalibration  -lH1PhysUtils -lH1JetFinder -lH1Finder \
		-lH1MagfieldOO \
		$(ROOTLIBS) -lEG -l${FORTRANLIB} \
		\
		-o $(BINDIR)/lambda
	@echo $@ created ...
	@echo ""

lambdaTree: outputdirs $(BUILDDIR)/lambdaTree.o
	@echo ""
	@echo Linking physics test program for ods access: $@ ...
	$(LOADER) \
		$(BUILDDIR)/lambdaTree.o  \
		\
		$(OO_LIBS) \
		-lH1Mods -lH1Hat -lH1Ods -lH1Skeleton -lH1Tracks -lH1Clusters \
		-lH1OOBanks -lH1Geom -lH1Arrays -lH1Tools -lH1Pointers -lH1Steering \
		-lH1HadronicCalibration  -lH1PhysUtils -lH1JetFinder -lH1Finder \
		-lH1MagfieldOO \
		$(ROOTLIBS) -lEG -l${FORTRANLIB} \
		\
		-o $(BINDIR)/lambdaTree
	@echo $@ created ...
	@echo ""

lambdaCTree: outputdirs $(BUILDDIR)/lambdaCTree.o
	@echo ""
	@echo Linking physics test program for ods access: $@ ...
	$(LOADER) \
		$(BUILDDIR)/lambdaCTree.o  \
		\
		$(OO_LIBS) \
		-lH1Mods -lH1Hat -lH1Ods -lH1Skeleton -lH1Tracks -lH1Clusters \
		-lH1OOBanks -lH1Geom -lH1Arrays -lH1Tools -lH1Pointers -lH1Steering \
		-lH1HadronicCalibration  -lH1PhysUtils -lH1JetFinder -lH1Finder \
		-lH1MagfieldOO \
		$(ROOTLIBS) -lEG -l${FORTRANLIB} \
		\
		-o $(BINDIR)/lambdaCTree
	@echo $@ created ...
	@echo ""

kaon: outputdirs $(BUILDDIR)/kaon.o
	@echo ""
	@echo Linking physics test program for ods access: $@ ...
	$(LOADER) \
		$(BUILDDIR)/kaon.o  \
		\
		$(OO_LIBS) \
		-lH1Mods -lH1Hat -lH1Ods -lH1Skeleton -lH1Tracks -lH1Clusters \
		-lH1OOBanks -lH1Geom -lH1Arrays -lH1Tools -lH1Pointers -lH1Steering \
		-lH1HadronicCalibration  -lH1PhysUtils -lH1JetFinder -lH1Finder \
		-lH1MagfieldOO \
		$(ROOTLIBS) -lEG -l${FORTRANLIB} \
		\
		-o $(BINDIR)/kaon
	@echo $@ created ...
	@echo ""

kaonTree: outputdirs $(BUILDDIR)/kaonTree.o
	@echo ""
	@echo Linking physics test program for ods access: $@ ...
	$(LOADER) \
		$(BUILDDIR)/kaonTree.o  \
		\
		$(OO_LIBS) \
		-lH1Mods -lH1Hat -lH1Ods -lH1Skeleton -lH1Tracks -lH1Clusters \
		-lH1OOBanks -lH1Geom -lH1Arrays -lH1Tools -lH1Pointers -lH1Steering \
		-lH1HadronicCalibration  -lH1PhysUtils -lH1JetFinder -lH1Finder \
		-lH1MagfieldOO \
		$(ROOTLIBS) -lEG -l${FORTRANLIB} \
		\
		-o $(BINDIR)/kaonTree
	@echo $@ created ...
	@echo ""

photonconv: outputdirs $(BUILDDIR)/photonconv.o
	@echo ""
	@echo Linking physics test program for ods access: $@ ...
	$(LOADER) \
		$(BUILDDIR)/photonconv.o  \
		\
		$(OO_LIBS) \
		-lH1Mods -lH1Hat -lH1Ods -lH1Skeleton -lH1Tracks -lH1Clusters \
		-lH1OOBanks -lH1Geom -lH1Arrays -lH1Tools -lH1Pointers -lH1Steering \
		-lH1HadronicCalibration  -lH1PhysUtils -lH1JetFinder -lH1Finder \
		-lH1MagfieldOO \
		$(ROOTLIBS) -lEG -l${FORTRANLIB} \
		\
		-o $(BINDIR)/photonconv
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

create_evls: outputdirs $(BUILDDIR)/create_evls.o
	@echo ""
	@echo Linking physics test program for ods access: $@ ...
	$(LOADER) \
		$(BUILDDIR)/create_evls.o  \
		\
		$(OO_LIBS) \
		-lH1Mods -lH1Hat -lH1Ods -lH1Skeleton -lH1Tracks -lH1Clusters \
		-lH1OOBanks -lH1Geom -lH1Arrays -lH1Tools -lH1Pointers -lH1Steering \
		-lH1HadronicCalibration  -lH1PhysUtils -lH1JetFinder -lH1Finder \
		-lH1MagfieldOO \
		$(ROOTLIBS) -lEG -l${FORTRANLIB} \
		\
		-o $(BINDIR)/create_evls
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

LambdaDists: outputdirs $(BUILDDIR)/LambdaDists.o
	@echo ""
	@echo Linking physics test program for ods access: $@ ...
	$(LOADER) \
		$(BUILDDIR)/LambdaDists.o  \
		\
		$(OO_LIBS) \
		-lH1Mods -lH1Hat -lH1Ods -lH1Skeleton -lH1Tracks -lH1Clusters \
		-lH1OOBanks -lH1Geom -lH1Arrays -lH1Tools -lH1Pointers -lH1Steering \
		-lH1HadronicCalibration  -lH1PhysUtils -lH1JetFinder -lH1Finder \
		-lH1MagfieldOO \
		$(ROOTLIBS) -lEG -l${FORTRANLIB} \
		\
		-o $(BINDIR)/LambdaDists
	@echo $@ created ...
	@echo ""

# to remove the generated *Ptr.[hC] files:
#clean::
#	rm  *Ptr.[Ch] 2>/dev/null
#	-cd $(BUILDDIR) &&\
#        rm Makefile.dep *.o *~ core *.bak 2>/dev/null


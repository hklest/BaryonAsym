##############################################################################
# Makefile for pkg_diffractive_gendump — T1 GEN-LEVEL EXTRACTOR for the
# diffractive high-Q2 charged-SIDIS port (sidis_highQ2/diffractive).
#
# Builds ONE binary:
#   McCandidateTruthProbe  (from src/, single H1Tree loop over ONE hat/mods pair,
#   gen-level only: no ODS, no FPACK, no detector content)
#
# Output dir: $(MAKEDIR)/bin/$(SYSTEM)  (per-package bin, never the shared
# /data/dust/.../BaryonAsym/bin/ and never H1ChargedSidis_HighQ2/bin).
#
# This package is NOT central production. It never builds or replaces any
# v6 binary. Modeled on pkg_diffractive_smoke/Makefile (itself modeled on
# H1ChargedSidis_HighQ2/Makefile, Phase 1A pattern).
#
# Usage:
#   make bin
#   make clean
#
# Environment (same as parent workspace):
#   source /afs/desy.de/group/h1/root/checkout2/oo-releases/relvol10/releases/4.2.1/thish1.sh
#   (sets H1DIST, H1CURRENT, ROOTSYS)
##############################################################################

MODULENAME = H1McCandidateTruthProbeStandalone
SLIB       = libMcCandidateTruthProbeUser.so

BIN = McCandidateTruthProbe

EXTRA_INCLUDEDIRS = src

vpath %.C src
vpath %.h src

# Full h1oo library set (identical to pkg_diffractive_smoke/Makefile).
# Gen-only task needs H1Mods (H1PartMC/H1GetPartMCId/H1GkiInfo),
# H1PhysUtils (H1MakeKine/H1Boost), H1Skeleton/H1Steering/H1Pointers; the
# full set is kept so the package compiles in the same environment as the
# smoke package (proven link set).
H1OO_LIBS = \
  -lH1Geom -lH1Ods -lH1OOBanks -lH1Tracks -lH1Clusters \
  -lH1Mods -lH1Hat -lH1Skeleton -lH1Steering \
  -lH1Pointers -lH1Arrays -lH1Tools -lH1Benchmarks -lH1Binning \
  -lH1PhysUtils -lH1JetFinder -lH1Finder -lH1HadronicCalibration \
  -lH1CaloTrigger \
  -lH1Calculator -lH1CalcPointers -lH1CalcWeights -lH1SoftLeptonId \
  -lH1Analysis \
  -lH1MagfieldOO -lH1Cuts -lH1TrkFinder -lH1Selection \
  -lH1ElecCalibration -lH1QCDFunc \
  -lH1Filler -lH1HatFiller -lH1NonepBgFinder -lH1HfsFinder \
  -lH1MuonFinder -lH1SubDetInfo -lH1PartEmFinder

ALL_LIBS = $(OO_LIBS) $(H1OO_LIBS) $(LHAPDFLIB) \
           -L$(FJCONTRIB__HOME)/lib -lCentauro \
           $(ROOTLIBS) -lEG -l${FORTRANLIB}

include $(H1DIST)/releases/$(H1CURRENT)/oo_tools/H1Basic.mk

-include Makefile.dep

# Per-package BINDIR override (Phase 1A pattern): everything lands in
# $(MAKEDIR)/bin/$(SYSTEM), NOT the shared workspace bin.
PKG_BINDIR := $(MAKEDIR)/bin/$(SYSTEM)

McCandidateTruthProbe: BINDIR := $(PKG_BINDIR)

McCandidateTruthProbe: outputdirs $(BUILDDIR)/McCandidateTruthProbe.o
	$(LOADER) $(BUILDDIR)/McCandidateTruthProbe.o $(ALL_LIBS) -o $(BINDIR)/$@

///////////////////////////////////////////////////////
// Creating an eventlist by entering events one by one
// and write it to a file afterwards.
// Also writes a separate file of various Event Distributions.
//
// Author     : Gage Tustin
///////////////////////////////////////////////////////

#include <stdlib.h>
#include <iostream>

// ROOT includes
#include <TFile.h>
#include <TROOT.h>
#include <THStack.h>
#include <TH1.h>
#include <TH2.h>
#include <TClonesArray.h>
#include <TCanvas.h>
#include <TTree.h>
#include <TDatabasePDG.h>
#include <TApplication.h>
#include <TMultiLayerPerceptron.h>
#include <TSynapse.h>
#include <TObject.h>
#include <TString.h>
#include <TObjArray.h>
#include <TMatrixD.h>
#include <TNeuron.h>
#include <TMath.h>
#include <TF1.h>
#include <TLegend.h>
#include <TGraph.h>
#include <TGraphErrors.h>

// H1 OO includes
#include "H1Skeleton/H1Tree.h"
#include "H1Skeleton/H1EventList.h"
#include "H1Pointers/H1FloatPtr.h"
#include "H1Steering/H1StdCmdLine.h"
#include "H1Mods/H1PartEmArrayPtr.h"
#include "H1Mods/H1PartEm.h"
#include "H1Tools/H1RunList.h"
#include "H1Geom/H1DetectorStatus.h"

#include "H1Clusters/H1Cell.h"
#include "H1Geom/H1CellGeometry.h"
#include "H1Geom/H1SpaCalCellGeo.h"
#include "H1Geom/H1CaloGeometry.h"
#include "H1Geom/H1SpacalGeometry.h"
#include "H1Geom/H1DBManager.h"
#include "H1Calculator/H1CalcGenericInterface.h"

#include "H1Calculator/H1Calculator.h"
#include "H1Calculator/H1CalcTrig.h"
#include "H1Calculator/H1CalcWeight.h"
#include "H1Calculator/H1CalcVertex.h"
#include "H1Calculator/H1CalcEvent.h"
#include "H1Calculator/H1CalcKine.h"
#include "H1Calculator/H1CalcElec.h"
#include "H1Calculator/H1CalcFs.h"
#include "H1Calculator/H1CalcHad.h"
#include "H1Calculator/H1CalcTrack.h"
#include "H1Calculator/H1CalcBgTiming.h"
#include "H1Calculator/H1CalcSystematic.h"

// Electron Cut includes from Daniel's low Q2 jet analysis
#include "/nfs/dust/h1/group/gtustin/h1oo/H1LambdaC/MyCalibCuts.h"
#include "/nfs/dust/h1/group/gtustin/h1oo/H1LambdaC/MyElec.h"			// Outer cut oin SpaCal Radius = 74cm (ELAN)
#include "/nfs/dust/h1/group/gtustin/h1oo/H1LambdaC/elecCut.h"
#include "/nfs/dust/h1/group/gtustin/h1oo/H1LambdaC/SpacLinearity.h"
#include "/nfs/dust/h1/group/gtustin/h1oo/H1LambdaC/Alignment.h"

using namespace std;

//Initializing some variables used in the following functions
Float_t fLinearityGrid[2];
Float_t fLinearityOffset[2];
Int_t fRunType;
Int_t fMyRunYear;
TVector3 fposE;
SpacLinearity fSpacLinearity;
Alignment fSpacAlignment;
TLorentzVector fElecSpacAlign;
Double_t fVtxZ;
Double_t fCutR;

void DoTilt(TLorentzVector& val, const TVector2& TiltVec) {
    TLorentzVector tilted = val;
    Double_t tz = TMath::Sqrt(1. - TiltVec.Mod2());
    tilted[2] = tz * val[2] + TiltVec.X() * val[0] + TiltVec.Y() * val[1];
    tilted[0] -= TiltVec.X() * val[2];
    tilted[1] -= TiltVec.Y() * val[2];
    Double_t ptprime = tilted.Vect().Pt();
    if (ptprime > 0) {
        Double_t f = TMath::Sqrt(val.Vect().Mag2() - tilted[2] * tilted[2]) / ptprime;
        if (TMath::Finite(f)) {
            tilted[0] *= f;
            tilted[1] *= f;
        }
    }
    val = tilted;
}

TLorentzVector GetHelixMom(Double_t e, const TVector3& posrel, Int_t charge) {
    // calculate 4-vector of momentum at vertex
    // posrel: vector pointing from vertex to measured space point
    // e: energy
    // charge: particle charge
    TLorentzVector mom;
    // first approximation: straight line
    mom.SetVectM(posrel, 0.);
    mom *= e / mom.E();

    Double_t z = posrel.Z();
    Double_t r = posrel.Pt();

    if (charge != 1 && charge != -1) {
        cout << "[GetHelixMom] invalid charge=" << charge << "\n";
        exit(10);
    }

    Double_t phi1 = posrel.Phi();
    for (int ii = 0; ii < 5; ii++) {
        // present guess of transverse momentum
        Double_t pt = mom.Pt();
        if (pt <= 0.0) break;
        // present guess of signed inverse radius
        const double B_TO_KAPPA = -(0.00029979) * 11.6083;
        Double_t kappa = charge * B_TO_KAPPA / pt;
        //
        Double_t asin_arg = 0.5 * r * kappa;
        if (asin_arg >= 1.0) asin_arg = 1.0;
        if (asin_arg < -1.0) asin_arg = -1.0;
        // present guess of difference in phi
        // between posrel and momentum at vertex
        // (note: the full arc length is given by 2*dphi/kappa )
        Double_t dphi = asin(asin_arg);
        mom.SetPhi(phi1 - dphi);
        // adjust Theta angle
        // not safe if kappa=0 !!!
        // but this can not happen, because
        //   the energy is finite and charge!=0
        mom.SetTheta(atan2(2. * dphi / kappa, z));
    }
    return mom;

}

void SetSpacalAlignmentConstants()
{

    // spacal linearity
    fLinearityGrid[0] = 4.05;
    fLinearityGrid[1] = 4.05;
    if (fRunType == 0) {
        if (fMyRunYear == 0) {
            fLinearityOffset[0] = 0.0156408;
            fLinearityOffset[1] = 0.114676;
            fSpacLinearity.SetAlpha("0.242819,-0.0205778,0.0260432,0.0383076,0.00562798");
            fSpacLinearity.SetSuperX("0.0167276,0.0299261,-0.00953007,0.0179944,-0.00972528,-0.0168475,-0.0319348,-0.0684563,-0.086717,     0.00352905,0.0693822,0.0442551,0.0216813,-0.0303227,-0.064708,-0.140036,-0.104978,-0.104978,     -0.0623242,0.0570739,0.0723443,0.00377095,0.00792301,-0.0689632,-0.156968,-0.210344,-0.154463,     0.105895,-0.0180959,-0.0503164,-0.0626292,-0.0209485,-0.115061,-0.114641,-0.210933,-0.0985816,     -0.0177701,-0.0033432,-0.0284922,-0.245953,-0.165167,-0.330278,-0.227396,-0.286239,-0.283827,     0.0458903,-0.00337742,-0.00804871,-0.0686681,-0.06349,-0.0585203,-0.0880737,-0.153037,-0.22619,     -0.0711327,-0.00400469,0.0395916,-0.0445892,-0.0199598,-0.0210018,-0.0910451,-0.208566,-0.217378,     0.00934288,0.0898184,0.000884791,0.0418213,0.031366,0.0286092,0.0496734,0.0150122,0.0150122,     0.0204303,0.0315178,-0.0267829,0.0643862,0.0987499,0.190735,0.152378,0.0836953,0.0493537");
            fSpacLinearity.SetSuperY("-0.184199,-0.117911,-0.131192,0.0389027,0.115862,0.0307898,-0.00752875,0.0168387,0.0290224,     -0.250487,-0.104629,-0.134111,0.0322301,0.277894,0.0913244,0.088867,0.0412062,0.0412062,     -0.396345,-0.165978,-0.052143,-0.0679408,0.094628,0.000819872,0.0634958,-0.0925063,-0.158756,     -0.427338,-0.247281,-0.152371,-0.0822678,0.14742,-0.017831,-0.04241,-0.0952114,-0.225006,     -0.42054,-0.226968,-0.174085,-0.0912892,0.00987576,-0.0897532,-0.0868805,-0.2153,-0.293616,     -0.376793,-0.271311,-0.213815,-0.035344,0.0731252,-0.104741,-0.126334,-0.216685,-0.36775,     -0.418636,-0.311764,-0.241163,-0.0179121,0.174335,-0.0133711,-0.0981814,-0.216164,-0.291957,     -0.383396,-0.348156,-0.278025,-0.106292,0.106834,-0.0755157,-0.000586344,-0.144734,-0.144734,     -0.297412,-0.211428,-0.0746996,-0.0728105,0.139452,0.00418469,0.0516676,-0.046533,-0.0956334");
        }
        else if (fMyRunYear == 1) {
            fLinearityOffset[0] = -0.00917979;
            fLinearityOffset[1] = -0.00399605;
            fSpacLinearity.SetAlpha("0.215161,-0.0182998,0.02826,0.0813706,0.0425845");
            fSpacLinearity.SetSuperX("0.113454,0.0932743,0.00347954,0.0244398,0.000883084,-0.02592,-0.048668,-0.048668,-0.048668,     0.133634,0.183069,0.0783529,0.00478777,-0.006267,-0.0698909,-0.198329,-0.186984,-0.194305,     0.0841982,0.162651,0.122075,-0.00819927,-0.00605708,-0.0732004,-0.197964,-0.175639,-0.201626,     -0.0110601,0.0824709,-0.0221144,-0.0785848,-0.043309,-0.085669,-0.157766,-0.208162,-0.227613,     0.0962547,0.160219,-0.0573068,-0.0154746,-0.0723762,-0.180757,-0.235262,-0.347882,-0.267003,     0.114549,0.154212,-0.00724216,-0.0806062,-0.0499646,-0.0119954,-0.0830394,-0.173053,-0.227015,     0.0996372,0.010951,0.041264,-0.0171611,-0.0106343,-0.0112487,-0.0913166,-0.16963,-0.198323,     -0.0758361,-0.25131,-0.00221716,0.0891758,0.07131,0.0383883,0.0679292,-0.077471,-0.077471,     -0.0727452,-0.0696542,0.112001,0.121105,0.160444,0.162914,0.108163,0.0153459,-0.0310625");
            fSpacLinearity.SetSuperY("-0.0568055,0.00757551,-0.0138492,0.239303,0.49331,0.234933,0.131305,0.131305,0.131305,     -0.121187,0.0290002,0.0804248,0.202316,0.44011,0.270634,0.33842,0.213046,0.106836,     -0.271373,-0.0327233,0.104232,0.0892096,0.229055,0.140664,0.217952,0.0876717,0.000627167,     -0.33823,-0.12907,-0.0255489,0.0386677,0.168288,0.0900059,0.108489,0.0692465,-0.0864174,     -0.340812,-0.178303,-0.0309631,0.0522595,0.12588,0.0191011,0.0384868,-0.0643555,-0.166005,     -0.360134,-0.184179,-0.0944194,0.118833,0.263871,0.074698,0.00841553,-0.12264,-0.282888,     -0.376758,-0.259897,-0.171783,0.0444287,0.293484,0.0922775,-0.0279638,-0.129469,-0.206179,     -0.341813,-0.306869,-0.161932,-0.0779615,0.120118,0.00289883,0.092326,-0.0608599,-0.0608599,     -0.270403,-0.198992,-0.0911157,-0.106834,0.17157,0.0265606,0.0858969,0.0125185,-0.0241707");
        }
        else if (fMyRunYear == 2 || fMyRunYear == 3) {
            fLinearityOffset[0] = -0.0259109;
            fLinearityOffset[1] = -0.0682793;
            fSpacLinearity.SetAlpha("0.201435,-0.0164299,0.0289763,0.0623963,0.0485568");
            fSpacLinearity.SetSuperX("-0.027421,-0.0661337,-0.118057,-0.0725348,-0.122883,-0.126813,-0.194818,-0.236544,-0.257407,    0.0112918,-0.0142107,-0.0606004,-0.0844394,-0.106726,-0.124312,-0.266982,-0.27827,-0.27827,     0.0367943,0.0839472,-0.00353726,-0.0446818,-0.0403773,-0.126336,-0.280727,-0.300782,-0.278092,     -0.0828938,0.0461634,-0.0276134,-0.0743554,-0.0567722,-0.097099,-0.210123,-0.289722,-0.255402,     0.0953872,0.0384836,-0.0606722,0.00199704,-0.0779063,-0.198728,-0.225476,-0.322192,-0.322667,     0.119855,0.0727173,-0.0258439,-0.100627,-0.0581218,-0.0153945,-0.0890133,-0.161109,-0.156465,     0.100099,0.143794,0.106222,0.0163566,0.0180538,0.0290426,-0.0439715,-0.106904,-0.131684,     0.158211,0.216323,0.196453,0.1841,0.135436,0.120477,0.165764,0.0440787,0.0440787,     0.191282,0.224353,0.232383,0.277551,0.235759,0.287543,0.274074,0.159077,0.101578");
            fSpacLinearity.SetSuperY("0.0832672,0.116299,0.0796698,0.288301,0.444733,0.138438,0.0511331,0.0745593,0.0862724,     0.0502349,0.152929,0.155398,0.274107,0.406879,0.188989,0.153456,0.0979855,0.0979855,     -0.0524594,0.132172,0.19865,0.109674,0.189768,0.0639354,0.0773207,-0.0848675,-0.158384,     -0.170991,-0.0389322,0.0138136,0.0409491,0.141092,0.0129768,0.005133,-0.0741054,-0.231901,     -0.19702,-0.0765572,-0.00642098,0.0551221,0.101666,-0.0377834,-0.0310269,-0.18888,-0.314892,     -0.210198,-0.0787831,-0.0256518,0.131021,0.248235,0.0305683,-0.0664505,-0.23373,-0.377147,     -0.214609,-0.148201,-0.0631045,0.0980578,0.281142,0.0286138,-0.0880433,-0.267968,-0.322557,     -0.197529,-0.18045,-0.159395,-0.0431372,0.157218,-0.0682303,-0.00545162,-0.187903,-0.187903,     -0.165193,-0.132856,-0.0852625,-0.0632842,0.157503,-0.0372449,0.0230227,-0.0824403,-0.135172");
        }
    }
    else { // MC
        if (fMyRunYear == 0) {
            fLinearityOffset[0] = 0.0756862;
            fLinearityOffset[1] = -0.0219959;
            fSpacLinearity.SetAlpha("0.229871,-0.017306,0.0232054,0.131317,-0.00428949");
            fSpacLinearity.SetSuperX("-0.112575,-0.074685,0.00748334,0.0883198,0.032299,0.0341294,0.0318277,0.0767779,0.104297,     -0.150466,-0.156853,-0.01483,0.0817488,0.00842957,0.00376256,0.0253491,0.121728,0.131816,     -0.144078,-0.0998387,0.00125859,0.0250847,0.0153244,-0.000281896,0.0352053,0.0952164,0.141903,     -0.188003,-0.0436979,0.0033839,0.0428401,0.0115921,-0.00891917,0.0286446,0.0824917,0.14654,     -0.133778,-0.0711502,-0.00898231,0.197168,0.0316641,-0.0800458,0.013842,0.0817766,0.199707,     -0.0802596,-0.0862033,-0.0240626,0.0152991,-0.00205817,-0.0276893,0.0195639,0.0441681,0.145387,     -0.202458,-0.164271,-0.040604,0.000968214,0.00519532,-0.027744,0.0278173,0.0765082,0.147582,     -0.163364,-0.124269,-0.0300521,0.00723123,0.0209032,-0.0251591,0.0633327,0.0699204,0.147582,     -0.13167,-0.099976,-0.0756829,0.0142271,0.00926576,0.00534871,0.0123014,0.0123014,0.0799415");
            fSpacLinearity.SetSuperY("-0.144135,-0.173582,-0.181029,-0.139002,-0.131031,-0.0966886,-0.137053,-0.133316,-0.0971309,     -0.114688,-0.166135,-0.0206866,-0.110896,-0.0956497,-0.0864086,-0.074221,-0.129578,-0.0609461,     -0.0632406,-0.130691,-0.022945,-0.0270785,0.0021297,-0.0255188,0.00418772,-0.0362467,0.00768569,     0.0128899,0.00582616,0.0144889,0.0452309,0.101753,0.0288762,0.0225374,0.0230434,0.038765,     0.0540212,0.0402587,0.0640518,0.0548586,0.0192469,0.0559277,0.029758,0.110632,0.0191278,     0.0241061,0.0172935,-0.0257002,-0.0355205,-0.135551,-0.0119597,0.00692445,0.0181148,0.0184323,     0.00932207,0.0530526,-0.000444617,0.0199773,-0.00914643,0.0109903,-0.00238056,0.144719,0.0978406,     0.0421419,0.0749617,0.0649934,0.0339293,0.0282025,0.0816079,0.118511,0.131615,0.0978406,     0.0813916,0.120641,0.166321,0.11437,0.213517,0.178427,0.216286,0.216286,0.157063");
        }
        else if (fMyRunYear == 1) {
            fLinearityOffset[0] = 0.0450836;
            fLinearityOffset[1] = -0.0213424;
            fSpacLinearity.SetAlpha("0.222361,-0.0168027,0.0236539,0.152251,0.0535807");
            fSpacLinearity.SetSuperX("-0.0814931,-0.0624145,-0.0186974,0.00286625,-0.00761901,-0.00697797,0.0572935,0.00603995,0.014152,     -0.100572,-0.106132,0.0365898,0.0349152,0.0184479,-0.0158802,0.0355114,-0.0452136,0.022264,     -0.0950118,-0.0606038,-0.0034592,0.0290239,0.0115734,-0.0194702,0.0165276,-0.0110035,0.0897416,     -0.069195,-0.0209813,0.00326107,0.0194679,0.0199152,-0.011225,0.0187123,0.0778457,0.0614661,     -0.0538265,-0.0351037,0.0135774,0.0913752,0.0248889,-0.0635821,0.03024,0.0173539,-0.0136285,     -0.178637,-0.0857887,-0.0446763,0.014262,0.0518472,0.00376849,0.0147571,0.0646203,0.0863166,     -0.167387,-0.178946,-0.0270536,0.00840976,-0.00116491,-0.0165572,-0.00484165,0.0575968,0.17801,     -0.125565,-0.0837435,-0.0236064,-0.0101158,-0.0198898,-0.0291642,-0.0167258,0.0756735,0.126842,     -0.100742,-0.0759191,-0.0680946,-0.0267012,-0.0314931,-0.0214365,0.015769,0.0457212,0.0862816");
            fSpacLinearity.SetSuperY("-0.0478533,-0.0734058,-0.102354,-0.0901828,-0.102747,-0.135577,-0.13048,-0.125585,-0.0937399,     -0.0223007,-0.0444575,-0.0635622,-0.0654469,-0.109459,-0.0840868,-0.0960087,-0.12069,-0.0618948,     -0.000143904,-0.0169939,-0.0110756,-0.0198622,-0.0115892,-0.0140679,-0.00176534,-0.0620212,-0.00309938,     -0.0136523,-0.00114039,0.0142631,0.0298079,0.104713,0.00984711,-0.0126593,0.0328655,-0.0690261,     0.0388193,0.0171203,0.0636595,0.0528344,0.0153614,-0.0185018,0.026403,0.00649027,-0.0131804,     -0.0369861,-0.0182689,-0.0200602,-0.0117647,-0.0775996,-0.00403396,-0.0104298,0.0142977,-0.0251816,     0.00863487,-0.00211022,-0.0127156,-0.0054499,0.0213396,0.00636913,-0.0337177,0.0291456,0.0105337,     0.107779,0.206924,0.0367384,0.0348488,0.0080741,0.0713293,0.0852914,0.0296444,0.020089,     0.13577,0.16376,0.120596,0.0584171,0.111699,0.108132,0.128273,0.0789586,0.0495238");
        }
        else if (fMyRunYear == 2 || fMyRunYear == 3) {
            fLinearityOffset[0] = 0.0766019;
            fLinearityOffset[1] = 0.00496387;
            fSpacLinearity.SetAlpha("0.23006,-0.0170495,0.0218784,0.133303,0.0680848");
            fSpacLinearity.SetSuperX("-0.105101,-0.0864725,-0.0864725,-0.0254642,-0.01637,-0.0774192,-0.0543677,0.0650274,0.0865292,     -0.123729,-0.0667832,-0.00246041,0.02645,0.0130464,-0.037152,-0.0673978,0.184422,0.108031,     -0.123729,-0.131106,-0.00889165,0.0195066,0.00343204,-0.0168376,0.00830687,0.0131382,0.0316398,     -0.152101,-0.0989825,-0.0192033,0.00705822,0.0129416,-0.00145142,-0.0268912,0.0506005,0.0559731,     -0.0932843,-0.0139406,0.00979134,0.10799,0.0109557,-0.0708491,-0.000634028,0.0772012,0.0856791,     -0.0208314,-0.0346946,-0.0356812,-0.0106477,-0.0062601,0.0247147,0.0507293,0.129753,0.123757,     -0.0968788,-0.0683252,-0.00403248,0.0253514,0.0143435,-0.0104396,0.0555418,0.0970307,0.178005,     -0.0921231,-0.0873674,0.178199,0.00990737,0.00882421,0.0165779,0.0338397,0.0654352,0.178005,     -0.0529985,-0.0138738,0.0596198,0.064537,0.0366806,0.0165779,0.0338397,0.0496375,0.178005");
            fSpacLinearity.SetSuperY("-0.0696401,-0.123768,-0.123768,-0.0843785,-0.0929632,-0.115565,-0.144531,-0.170222,-0.152396,     -0.0155126,-0.0192892,0.0358808,-0.0364046,-0.0900375,-0.0842288,-0.105893,-0.195914,-0.13457,     -0.0155126,-0.0744593,0.0258037,-0.024498,-0.0622983,-0.0254199,-0.0137218,-0.0974735,-0.073227,     0.027563,0.0460149,0.0182597,0.0148962,0.0231636,0.00223306,0.0136436,-0.0248595,-0.0231475,     0.00941376,0.052282,0.000749501,-0.0531546,-0.00549001,0.0568937,0.0616965,0.0705135,0.028644,     -0.0678167,0.0111752,-0.0143891,-0.00210435,-0.0488627,-0.00326151,-0.0110533,0.0236442,-0.0163954,     0.00447923,0.0746244,0.0401569,0.0505877,0.00819511,0.0236468,-0.097244,-0.0199611,0.0658008,     0.0807088,0.156938,-0.0127381,0.0540481,0.0685823,0.039811,0.0117737,-0.00409371,0.0658008,     0.116129,0.15155,0.146162,0.25611,0.162346,0.039811,0.0117737,0.00384001,0.0658008");
        }
    }
    //fSpacLinearity.PrintSteer(std::cout);


    // spacal alignment 
    if (fRunType == 0) {
        if (fMyRunYear == 0) {
            fSpacAlignment.SetPhi(0.000179124);
            fSpacAlignment.SetOrigin("-0.0193911,-0.0159895,0.0710439");
            fSpacAlignment.SetZ0(-162.648);
        }
        else if (fMyRunYear == 1) {
            fSpacAlignment.SetPhi(0.000589021);
            fSpacAlignment.SetOrigin("-0.0380174,0.00789926,-1.07628");
            fSpacAlignment.SetZ0(-162.49);
        }
        else if (fMyRunYear == 2 || fMyRunYear == 3) {
            fSpacAlignment.SetPhi(-0.000705192);
            fSpacAlignment.SetOrigin("-0.0212224,0.0212467,-0.204696");
            fSpacAlignment.SetZ0(-161.639);
        }
    }
    else { // MC
        if (fMyRunYear == 0) {
            fSpacAlignment.SetPhi(0.0001963);
            fSpacAlignment.SetOrigin("0.00405899,0.0144147,0.0468047");
            fSpacAlignment.SetZ0(-162.188);
        }
        else if (fMyRunYear == 1) {
            fSpacAlignment.SetPhi(0.00050441);
            fSpacAlignment.SetOrigin("0.00461259,0.0155858,0.260419");
            fSpacAlignment.SetZ0(-161.985);
        }
        else if (fMyRunYear == 2 || fMyRunYear == 3) {
            fSpacAlignment.SetPhi(-0.00125922);
            fSpacAlignment.SetOrigin("0.0129104,0.0243385,0.206787");
            fSpacAlignment.SetZ0(-162.045);
        }
    }
}


int
main(int argc, char* argv[])
{
    // parse the command line
    H1StdCmdLine opts;
    opts.Parse(&argc, argv);

    // Load mODS/HAT files
    H1Tree::Instance()->Open();            // this statement must be there!

     // do a selection
    // Can implement some selections here, I chose to use H1FloatPtr instead to variables and do a check later
     
    //H1Tree::Instance()->SelectHat("Q2e>5.");
    //H1Tree::Instance()->SelectHat("Q2e<100.");
    //H1Tree::Instance()->SelectHat("ElecE>10.");
    //H1Tree::Instance()->SelectHat("Ye>0.1");
    //H1Tree::Instance()->SelectHat("Ye<0.6");
    //H1Tree::Instance()->SelectHat("Epz>35.");
    //H1Tree::Instance()->SelectHat("Epz<70.");
    H1Tree::Instance()->SelectHat("fIl1ac[61]>0");
    
      // create new H1EventList
    H1EventList *list= new H1EventList("mylist");

      // and enter interesting events into this list
    //H1FloatPtr q2e("Q2e"); //Q2 variable aquired using electron method
    H1FloatPtr q2s("Q2s"); //Q2 variable aquired using sigma method
    //H1FloatPtr q2egen("Q2eGen");//Q2 gen level variable aquired using electron method
    H1FloatPtr q2sgen("Q2sGen"); //Q2 gen level variable aquired using sigma method
    H1FloatPtr eE("ElecE"); //Energy of electron as deposited in SpaCal
    //H1FloatPtr ye("Ye"); //Inelasticity aquired through sigma method
    H1FloatPtr ys("Ys"); //Inelasticity aquired through sigma method
    H1FloatPtr vtxZ("VtxZ");
    H1FloatPtr epz("Epz");

    //Values for Epz calculation
    H1FloatPtr ElecTheta("ElecTheta"); //Theta of scattered electron
    H1FloatPtr ElecPhi("ElecPhi");
    H1FloatPtr ElecE("ElecE"); //Energy of scattered electron
    H1FloatPtr EBeamE("EBeamE"); //Energy of electron beam from DMIS 
    H1FloatPtr EBeamP("EBeamP"); //Energy of proton beam from DMIS
    H1FloatPtr HadPzX("HadPzX");
    H1FloatPtr HadEX("HadEX");
    H1FloatPtr hfsE("FullHfsCombE");
    H1FloatPtr hfsTheta("FullHfsCombTheta");
    H1FloatPtr btx("BeamTiltX0");
    H1FloatPtr bty("BeamTiltY0");

    Int_t fElecID;

    //Defining Root Canvas to work on
    TCanvas* canvas = new TCanvas("ODS", "Plot", 10, 10, 800, 800);

    TH2F* hXYclusterBefore = new TH2F("XY Cluster Position Before", "XY Cluster Position Before", 1000, -80, 80, 1000, -80, 80);
    TH2F* hXYclusterAfter = new TH2F("XY Cluster Position After", "XY Cluster Position After", 1000, -80, 80, 1000, -80, 80);

    static H1PartEmArrayPtr PartEm;
    static H1PartEmArrayPtr elecs("EmParticles");

    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //Consistenct with Lumi calculation using SelectedRuns.root oolumi output file
    TString goodRunFileName("/nfs/dust/h1/group/gtustin/h1oo/H1LambdaC/SelectedRuns.root");
    TFile goodRunFile(goodRunFileName);
    if (!goodRunFile.IsOpen()) {
        cerr << "Error: could not open file " << goodRunFileName << "\n";
        return 2;
    }

    H1RunList* goodRunList
        = (H1RunList*)goodRunFile.Get("H1RunList");

    if (!goodRunList) {
        cerr << "Error: no runlist in file - return!\n";
        return 2;
    }

    H1DetectorStatus* detectorStatus
        = (H1DetectorStatus*)goodRunFile.Get("MyDetectorStatus");

    if (!detectorStatus) {
        cerr << "Error: no detector status in file - return!\n";
        return 3;
    }
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //Defining TTrees to syore event info
    TTree* EventTree = new TTree("Data Event Tree", "Data Event Tree");
    double elecPhi, elecTheta, Epz, elecE, x, Q2s, Ys, RTheta, ClusterRadius, Zvertex, ProtonEnergy, ElectronEnergy, Xcluster, Ycluster, Zcluster, charge;
    //Defining Tree Branches
    EventTree->Branch("elecPhi", &elecPhi, "elecPhi/D");
    EventTree->Branch("elecTheta", &elecTheta, "elecTheta/D");
    EventTree->Branch("x", &x, "x/D");
    EventTree->Branch("Q2s", &Q2s, "Q2s/D");
    EventTree->Branch("Ys", &Ys, "Ys/D");
    EventTree->Branch("Epz", &Epz, "Epz/D");
    EventTree->Branch("elecE", &elecE, "elecE/D");
    EventTree->Branch("RTheta", &RTheta, "RTheta/D");
    EventTree->Branch("ClusterRadius", &ClusterRadius, "ClusterRadius/D");
    EventTree->Branch("Zvertex", &Zvertex, "Zvertex/D");
    EventTree->Branch("ProtonEnergy", &ProtonEnergy, "ProtonEnergy/D");
    EventTree->Branch("ElectronEnergy", &ElectronEnergy, "ElectronEnergy/D");
    EventTree->Branch("Xcluster", &Xcluster, "Xcluster/D");
    EventTree->Branch("Ycluster", &Ycluster, "Ycluster/D");
    EventTree->Branch("Zcluster", &Zcluster, "Zcluster/D");
    EventTree->Branch("charge", &charge, "charge/D");
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //initializing counts, counting events after each selection just as a consistency check
    double TriggerEvents = 0;
    double Q2Events = 0;
    double yEvents = 0;
    double eEnergyEvents = 0;
    double zvtxEvents = 0;
    double ClusterREvents = 0;
    double NoScattPartEm = 0;
    double EnergyBalanceEvents = 0;
    double SpaCalEvents = 0;
    double GoodDetectorEvents = 0;
    double GoodRunEvents = 0;
    //double DeadCellEvents = 0;
    //double DeadTriggerEvents = 0;
    //double RadClusterEvents = 0;
    double BoxCutEvents = 0;
    double ElipseCutEvents = 0;
    double goodelectronEvents = 0;

    //initializing PartEM checks
    double scatelCount = 0;

    
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //Initializing SpaCal dead cells and triggers for scattered electron cut
    //Valid for 2006 data

    

    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    gH1Calc = H1Calculator::Instance();
    Int_t sel=0;
    while (H1Tree::Instance()->Next()) {
        gH1Calc->Reset();
        TriggerEvents++;

        int RunNumber = H1Tree::Instance()->GetRunNumber();
        //cout << "Run Number: " << RunNumber << endl;


       scatelCount = 0;

        for (Int_t loop = 0; loop < PartEm.GetEntries(); loop++) {
            
            if (PartEm[loop]->IsScatElec() == true) { 
                scatelCount++;
                //ClusterRadius = PartEm[loop]->GetRadiusFromVectors();
                ClusterRadius = PartEm[loop]->GetEcra();
                Xcluster = PartEm[loop]->GetXClus();
                Ycluster = PartEm[loop]->GetYClus();
                Zcluster = PartEm[loop]->GetZClus();
            }

        }

        //cout << "Scattered e count: " << scatelCount << endl;

        

        
        if (*q2s <= 5 || *q2s >= 100) { continue; } // 5 < Q2 [GeV] < 100
        Q2Events++;
        
        if (*ys <= 0.1 || *ys >= 0.6) { continue; } // 0.1 < y < 0.6
        yEvents++;

        if (*eE <= 11) { continue; } //E_electron [GeV] > 11
        eEnergyEvents++;

        if (*vtxZ<=-30 || *vtxZ>=30) { continue; } //-30 < Z_vertex [cm] < 30
        zvtxEvents++;

        if (*epz<=35 || *epz>=70) { continue; } // 35 < E-pz [GeV] < 70
        EnergyBalanceEvents++;

        if (abs((*vtxZ + 160) * tan(*ElecTheta)) < 9.1) { continue; } //R_Theta [cm] > 9.1
        SpaCalEvents++;

        if (scatelCount != 1) {
            //cout << "Scattered Electron Count != 1" << endl;
            continue;
        } //skip event if scattered electron is not found in PartEm class
        NoScattPartEm++;

        if (ClusterRadius > 3.5) { continue; } // e Cluster Radius [cm] < 3.5
        ClusterREvents++;

        // skip runs not in list of good runs
        if (!goodRunList->FindRun(H1Tree::Instance()->GetRunNumber())) { continue; }
        GoodRunEvents++;
        // skip data events with bad detector status
        if (!detectorStatus->IsOn()) { continue; }
        GoodDetectorEvents++;
        


        hXYclusterBefore->Fill(Xcluster,Ycluster);


     ///////////////////////////////////////////////////////////////////////////////////////
        //SpaCal box cut for scattered electron
        if ((Xcluster >= 9.0 && Xcluster <= 16.24) && (Ycluster >= -4.105 && Ycluster <= 4.377)) { continue; }
        BoxCutEvents++;


        //SpaCal elipse cut 
        if ((Xcluster + 78.4461) * (Xcluster + 78.4461) / ((11.027) * (11.027)) + (Ycluster) * (Ycluster) / ((20.5) * (20.5)) < 1.0) { continue; }
        ElipseCutEvents++;

     //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

        //Daniel's electron cuts from low Q2 jet analysis
        

        fElecID = MyElec::Id();
        if (fElecID < 0) { continue; }
        static elecCut myElecCut = 0;
        bool retval;
        //cout << "check1" << endl;
        bool bFiducialCuts = myElecCut.goodElec(elecs[fElecID], RunNumber);
        //cout << "check2" << endl;
        if (!bFiducialCuts) { 
            //cout << "Cuts didn't go through" << endl;
            continue;
        }
        //fCutsEPS["SpacalFiducial1"] = bFiducialCuts;
        //The work for this cut is done in elecCut.C and elecCut.h codes, running goodElec() will return true if the scattered electron is a "good electron"
        //cout << bFiducialCuts << endl;
        if (bFiducialCuts == false) { continue; }
        goodelectronEvents++;

     //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
     // 
     // // ------------------------------------- //
	// --- --- calculate Spacal Elec --- --- //
        //const  double          charge = fMyRunYear > 1 ? +1 : -1;
        // --- realign electron cluster
        fRunType = gH1Tree->IsMC();	//fRunType = : 0 for data, 8 for Dja(CDM),  11 for RapGap,   16 for PHOJET, 5 for Pythia 
        fVtxZ = gH1Calc->Vertex()->GetZ();
        int fMyRunYear = 2; //Old tag from old code, run year of 2 or year means 2006 data
        SetSpacalAlignmentConstants();
        fposE = TVector3(Xcluster, Ycluster, Zcluster); // HAT-cluster position 
        fposE = fSpacLinearity.GetPosCorr(fposE, fLinearityGrid, fLinearityOffset); // spacal linearity
        fposE = fSpacAlignment.Transform(fposE); // spacal alignment
        fposE -= TVector3(gH1Calc->Vertex()->GetX(), gH1Calc->Vertex()->GetY(), gH1Calc->Vertex()->GetZ());// subtract vertex pos
        // spacal electron four-vector
        fElecSpacAlign = GetHelixMom(*eE, fposE, charge);
        DoTilt(fElecSpacAlign, TVector2((*btx), (*bty)));

        // --- calculate cutR
        // correct for spacal nom. position (not needed for 4-vector)
        const double spacalnom = -161;
        fposE *= (spacalnom - fVtxZ) / fposE[2]; // correct for spacal nom. position
        // beam tilt correction
        fposE[0] -= (*btx) * fposE[2];
        fposE[1] -= (*bty) * fposE[2];
        fCutR = fposE.Pt();
        // derived from work.run_sAllTrigStefan_37_VtxZRW26neu_RcutStudy6_06p_alignbugfix_noSpacCuts
              // for fixed cuts
              //if ( fposE[0]>-16 && fposE[0]<0  && fposE[1] > -8  && fposE[1] <  8 ) retval=false; // all y, eE regions bad
              // non-aligned
        if (Xcluster > -16 && Xcluster<0 && Ycluster > -10 && Ycluster < 6) { continue; } // all y, eE regions bad
        if (Xcluster > -18 && Xcluster && Ycluster > -4 && Ycluster < 2) { continue; } // all y, eE regions bad
        // aligned
        if (fposE[0] > -14 && fposE[0] < 10 && fposE[1] >  0 && fposE[1] < 14) { continue; } // all y, eE regions bad
        if (fposE[0] > -14 && fposE[0]<8 && fposE[1] > -12 && fposE[1] < 0) { continue; } // all y, eE regions bad
        if (fCutR < 12.5) { continue; }
        //if (fCutR > c_Spacal_Rxy_max - 0.5) { continue; }

        //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

        hXYclusterAfter->Fill(Xcluster, Ycluster);



      //if (*ye < 0.6 && *ye>0.1 && *q2s > 5 && *q2s < 100 && *eE>10 && *epz > 35 && *epz < 70 && *vtxZ < 30 && *vtxZ > -30)    {
        //cout << "E: " << *Echeck << endl;
        //cout << "EPZ: " << *epz << endl;
        //cout << "EPZ calc: " <<  ( * ElecE) + abs((*ElecE)*cos(*ElecTheta)) + (*HadEX)-(*HadPzX) << endl;
        //cout << "EPZ calc: " <<  ( * ElecE) + abs((*ElecE)*cos(*ElecTheta)) + (*hfsE) - abs((*hfsE) * cos(*hfsTheta)) << endl;
          
          elecE = *eE;
          Epz = *epz;
          elecPhi = *ElecPhi;
          elecTheta = *ElecTheta;
          //x = (*q2e) / ((*ye) * 920 * 27.6 * 4); //Bjorken X calculated from Electron Method
          x = ((*eE) / (*EBeamP)) * (cos((*ElecTheta) / (2))) * (cos((*ElecTheta) / (2))) / (*ys);
          Q2s = *q2s;
          Ys = *ys;
          RTheta = abs((*vtxZ + 160) * tan(*ElecTheta));
          Zvertex = *vtxZ;
          ProtonEnergy = *EBeamP;
          ElectronEnergy = *EBeamE;

          EventTree->Fill();

          sel++;
          list->Enter();
      //}
    }
    cout << "Selected "<<sel<<" entries"<<endl;

    cout << "check" << endl;
      // Write eventlist to file
    TFile file(opts.GetOutput(), "RECREATE");
    list->Write();
    list->Print();
    // close
    file.Write();
    file.Close();

    //Write Event Distributions to File
    //TFile fileEventDist("/nfs/dust/h1/group/gtustin/h1oo/H1LambdaC/EventDists06.root","RECREATE");
    //string str1 = "123";
    //string str2 = str1.substr(0, str1.length() - 1);

    //string StdDirectory = "/nfs/dust/h1/group/gtustin/h1oo/H1LambdaC/";
    //string StdOutput = opts.GetOutput();
    //string EventDistsOutput = StdOutput.substr(0, StdOutput.length()-5) + "_EventDists06.root";
    //TFile fileEventDist(EventDistsOutput.c_str(), "RECREATE");
    //EventTree->Write();

    //hXYclusterBefore->SetStats(0);
    //hXYclusterBefore->GetXaxis()->SetTitle("X");
    //hXYclusterBefore->GetYaxis()->SetTitle("Y");
    ////XYvtxdensitytot->GetXaxis()->SetTitleSize(0.03);
    ////XYvtxdensitytot->GetXaxis()->SetTitleOffset(1);
    ////XYvtxdensitytot->GetYaxis()->SetTitleOffset(1);
    ////XYvtxdensitytot->GetYaxis()->SetTitleSize(0.03);
    //hXYclusterBefore->Draw("colz");
    //canvas->SetName("SpaCal XY Positions Before");
    //canvas->Write();

    //hXYclusterAfter->SetStats(0);
    //hXYclusterAfter->GetXaxis()->SetTitle("X");
    //hXYclusterAfter->GetYaxis()->SetTitle("Y");
    ////XYvtxdensitytot->GetXaxis()->SetTitleSize(0.03);
    ////XYvtxdensitytot->GetXaxis()->SetTitleOffset(1);
    ////XYvtxdensitytot->GetYaxis()->SetTitleOffset(1);
    ////XYvtxdensitytot->GetYaxis()->SetTitleSize(0.03);
    //hXYclusterAfter->Draw("colz");
    //canvas->SetName("SpaCal XY Positions After");
    //canvas->Write();

    //fileEventDist.Write();
    //fileEventDist.Close();

    cout << "Eventlist written to " << opts.GetOutput() << endl;
    //cout << "Event Dists written to " << EventDistsOutput << endl;

    //initial amount of events in 2006 e+p and e-p: 
    cout << "Events After Trigger: " << TriggerEvents << endl;
    cout << "Events After Q2: " << Q2Events << endl;
    cout << "Events After y: " << yEvents << endl;
    cout << "Events After eE: " << eEnergyEvents << endl;
    cout << "Events After zvtx: " << zvtxEvents << endl;
    cout << "Events After Epz: " << EnergyBalanceEvents << endl;
    cout << "Events After RTheta: " << SpaCalEvents << endl;
    cout << "Events After Requiring Part Em Scattered electron: " << NoScattPartEm << endl;
    cout << "Events After Cluster Radius: " << ClusterREvents << endl;
    cout << "Events Run Check: " << GoodRunEvents << endl;
    cout << "Events After Detector Check: " << GoodDetectorEvents << endl;
    //cout << "Events After Dead Cell Check: " << DeadCellEvents << endl;
    //cout << "Events After Dead Trigger Check: " << DeadTriggerEvents << endl;
    //cout << "Events After Radial Cluster Pos Check: " << RadClusterEvents << endl;
    //cout << "Events After Trigger: " << TriggerEvents << endl;
    cout << "Events After Box Cut: " << BoxCutEvents << endl;
    cout << "Events After Elipse Cut: " << ElipseCutEvents << endl;
    cout << "Events After Electron Cut: " << goodelectronEvents << endl;

    //double TriggerEvents = 0;
    //double Q2Events = 0;
    //double yEvents = 0;
    //double eEnergyEvents = 0;
    //double zvtxEvents = 0;
    //double ClusterREvents = 0;
    //double EnergyBalanceEvents = 0;
    //double SpaCalEvents = 0;
    
    return 0;
}



#ifndef __H1CALCULATOR_H
#define __H1CALCULATOR_H

#include "H1Pointers/H1IntPtr.h"
#include "H1Pointers/H1ShortPtr.h"
#include "H1CalcPointers/H1IntFunctionPtr.h"
#include "H1CalcPointers/H1FloatFunctionPtr.h"
#include "H1CalcPointers/H1BoolFunctionPtr.h"
#include "H1CalcPointers/H1LorentzFunctionPtr.h"
#include "H1Geom/H1Constants.h"
#include "H1CalcPointers/H1VariableLookUpTableEntry.h"
#include <iostream>
#include <iomanip>

class H1CalcBgTiming;
class H1CalcCC;
class H1CalcDiff;
class H1CalcDiffKine;
class H1CalcElec;
class H1CalcEvent;
class H1CalcFnc;
class H1CalcForwDets;
class H1CalcFs;
class H1CalcHad;
class H1CalcJet;
class H1CalcK0;
class H1CalcKine;
class H1CalcMuon;
class H1CalcPolarisation;
class H1CalcSystematic;
class H1CalcTaggers;
class H1CalcTrack;
class H1CalcTrackExtrapol;
class H1CalcTrig;
class H1CalcVertex;
class H1CalcW;
class H1CalcWeight;
class H1CalcZ;

using namespace std;

class H1Calculator : public TObject
{

private:

    static H1Calculator* fInstance;  // The H1Calculator singleton

    H1CalcBgTiming* fBgTiming; //!
    H1CalcCC* fCC; //!
    H1CalcDiff* fDiff; //!
    H1CalcDiffKine* fDiffKine; //!
    H1CalcElec* fElec; //!
    H1CalcEvent* fEvent; //!
    H1CalcFnc* fFnc; //!
    H1CalcForwDets* fForwDets; //!
    H1CalcFs* fFs; //!
    H1CalcHad* fHad; //!
    H1CalcJet* fJet; //!
    H1CalcK0* fK0; //!
    H1CalcKine* fKine; //!
    H1CalcMuon* fMuon; //!
    H1CalcPolarisation* fPolarisation; //!
    H1CalcSystematic* fSystematic; //!
    H1CalcTaggers* fTaggers; //!
    H1CalcTrack* fTrack; //!
    H1CalcTrackExtrapol* fTrackExtrapol; //!
    H1CalcTrig* fTrig; //!
    H1CalcVertex* fVertex; //!
    H1CalcW* fW; //!
    H1CalcWeight* fWeight; //!
    H1CalcZ* fZ; //!

    static H1ShortPtr RunType;
    static H1IntPtr   RunNumber;
    static H1IntPtr   EventNumber;

    static TLorentzVector fDummyVector;
    static TObjArray fNameLookUp;
    static TObjArray fGetterFunctionLookUp;
    static Int_t fNVariables;
    void InitialiseGenericInterface();

public:

    H1Calculator();
    ~H1Calculator();
    static H1Calculator* Instance();

    enum { kNC, kCC, kW };

    void SetFinalStateToNC();
    void SetFinalStateToCC();
    void SetFinalStateToW();

    void CalibrateHadrooII();
    void DoNotCalibrateHadrooII();
    void CalibrateLatestHadrooII();
    void DoNotCalibrateLatestHadrooII();
    void CalibrateElectrons();
    void DoNotCalibrateElectrons();
    void ApplyForwardElectronCalibration();
    void DoNotApplyForwardElectronCalibration();

    void Reset();

    void AddFloatVariable(const char* name, H1FloatFunctionPtr::FloatFunctionPtr getterfunction);

    void AddBoolVariable(const char* name, H1BoolFunctionPtr::BoolFunctionPtr getterfunction);
    void AddIntVariable(const char* name, H1IntFunctionPtr::IntFunctionPtr getterfunction);

    Int_t GetEventNumber() const
    {
        return *EventNumber;
    }

    Int_t GetRunNumber() const
    {
        return *RunNumber;
    }

    Bool_t IsMC() const
    {
        return *RunType;
    }

    H1Constants* Const() {
        return gH1Constants;
    }

    H1CalcBgTiming* BgTiming() {
        return fBgTiming;
    }

    H1CalcCC* CC() {
        return fCC;
    }

    H1CalcDiff* Diff() {
        return fDiff;
    }

    H1CalcDiffKine* DiffKine() {
        return fDiffKine;
    }

    H1CalcElec* Elec() {
        return fElec;
    }

    H1CalcEvent* Event() {
        return fEvent;
    }

    H1CalcFnc* Fnc() {
        return fFnc;
    }

    H1CalcForwDets* ForwDets() {
        return fForwDets;
    }

    H1CalcFs* Fs() {
        return fFs;
    }

    H1CalcHad* Had() {
        return fHad;
    }

    H1CalcJet* Jet() {
        return fJet;
    }

    H1CalcK0* K0() {
        return fK0;
    }

    H1CalcKine* Kine() {
        return fKine;
    }

    H1CalcMuon* Muon() {
        return fMuon;
    }

    H1CalcPolarisation* Polarisation() {
        return fPolarisation;
    }

    H1CalcSystematic* Systematic() {
        return fSystematic;
    }

    H1CalcTaggers* Taggers() {
        return fTaggers;
    }

    H1CalcTrack* Track() {
        return fTrack;
    }

    H1CalcTrackExtrapol* TrackExtrapol() {
        return fTrackExtrapol;
    }

    H1CalcTrig* Trig() {
        return fTrig;
    }

    H1CalcVertex* Vertex() {
        return fVertex;
    }

    H1CalcW* W() {
        return fW;
    }

    H1CalcWeight* Weight() {
        return fWeight;
    }

    H1CalcZ* Z() {
        return fZ;
    }

    Int_t GetID(TString name) const
    {
        Int_t i = 0;

        while (i < fNVariables &&
            ((H1VariableLookUpTableEntry*)fNameLookUp.UncheckedAt(i))->GetVariableName().CompareTo(name) != 0) {
            i++;
        }
        if (i == fNVariables) {
            cerr << "Warning in <H1Calculator::GetID>: " << endl;
            cerr << "  variable with name " << name << " not known !" << endl;
            return -1; // variable with given name not found
        }
        else
            return i;
    }

    Int_t GetID(const char* name) const
    {
        return GetID(TString(name));
    }

    TString GetName(Int_t enumID) const
    {
        // Get the string name of a variable from its enum
        if (enumID < fNVariables) {
            return ((H1VariableLookUpTableEntry*)fNameLookUp.UncheckedAt(enumID))->GetVariableName();
        }
        else {
            return "Warning in <H1Calculator::GetName>: Variable not known";
        }
    }

    Int_t GetNumberOfVariables() const
    {
        return fNVariables;
    };

    Int_t GetNVariables() const
    {
        return fNVariables;
    };

    Float_t GetFloatVariable(const Int_t ID) const
    {
        if (ID >= 0 && ID < fNVariables) {
            return static_cast <H1FloatFunctionPtr*> (fGetterFunctionLookUp.UncheckedAt(ID))->GetValue();
        }
        else {
            cerr << "Error in <H1Calculator::GetFloatVariable>: id " << ID << " exceeds range !" << endl;
            return 0;
        }
    }

    Float_t GetFloatVariable(const Int_t ID, const Int_t index) const
    {
        if (ID >= 0 && ID < fNVariables) {
            return static_cast <H1FloatFunctionPtr*> (fGetterFunctionLookUp.UncheckedAt(ID))->GetValue(index);
        }
        else {
            cerr << "Error in <H1Calculator::GetFloatVariable(index)>: id " << ID << " exceeds range !" << endl;
            return 0;
        }
    }

    Bool_t GetBoolVariable(const Int_t ID) const
    {
        if (ID >= 0 && ID < fNVariables) {
            return static_cast <H1BoolFunctionPtr*> (fGetterFunctionLookUp.UncheckedAt(ID))->GetValue();
        }
        else {
            cerr << "Error in <H1Calculator::GetBoolVariable>: id " << ID << " exceeds range !" << endl;
            return 0;
        }
    }

    Bool_t GetBoolVariable(const Int_t ID, const Int_t index) const
    {
        if (ID >= 0 && ID < fNVariables)
            return static_cast <H1BoolFunctionPtr*> (fGetterFunctionLookUp.UncheckedAt(ID))->GetValue(index);
        else {
            cerr << "Error in H1Calculator::GetVariable: id " << ID << " exceeds range !" << endl;
            return 0;
        }
    }

    Int_t  GetIntVariable(const Int_t ID) const
    {
        if (ID >= 0 && ID < fNVariables)
            return static_cast <H1IntFunctionPtr*> (fGetterFunctionLookUp.UncheckedAt(ID))->GetValue();
        else {
            cerr << "Error in H1Calculator::GetVariable: id " << ID << " exceeds range !" << endl;
            return 0;
        }
    }

    Int_t GetIntVariable(const Int_t ID, const Int_t index) const
    {
        if (ID >= 0 && ID < fNVariables)
            return static_cast <H1IntFunctionPtr*> (fGetterFunctionLookUp.UncheckedAt(ID))->GetValue(index);
        else {
            cerr << "Error in H1Calculator::GetVariable: id " << ID << " exceeds range !" << endl;
            return 0;
        }
    }

    const TLorentzVector& GetLorentzVariable(const Int_t ID) const
    {
        if (ID >= 0 && ID < fNVariables) {
            return static_cast <H1LorentzFunctionPtr*> (fGetterFunctionLookUp.UncheckedAt(ID))->GetValue();
        }
        else {
            cerr << "Error in <H1Calculator::GetLorentzVariable>: id " << ID << " exceeds range !" << endl;
            return fDummyVector;
        }
    }

    const TLorentzVector& GetLorentzVariable(const Int_t ID, const Int_t index) const
    {
        if (ID >= 0 && ID < fNVariables) {
            return static_cast <H1LorentzFunctionPtr*> (fGetterFunctionLookUp.UncheckedAt(ID))->GetValue(index);
        }
        else {
            cerr << "Error in <H1Calculator::GetLorentzVariable(index)>: id " << ID << " exceeds range !" << endl;
            return fDummyVector;
        }
    }

    Float_t GetVariable(const Int_t ID) const
    {
        if (!(ID >= 0 && ID < fNVariables)) {
            cerr << "Error in <H1Calculator::GetVariable>: " << endl;
            cerr << "  id " << ID << " exceeds range !" << endl;
            return 0;
        }

        switch (static_cast <H1VariableLookUpTableEntry*> (fNameLookUp.UncheckedAt(ID))->GetVariableType()) {
        case 'f':
            return static_cast <H1FloatFunctionPtr*> (fGetterFunctionLookUp.UncheckedAt(ID))->GetValue();
            break;
        case 'i':
            return static_cast <H1IntFunctionPtr*> (fGetterFunctionLookUp.UncheckedAt(ID))->GetValue();
            break;
        case 'b':
            return static_cast <H1BoolFunctionPtr*> (fGetterFunctionLookUp.UncheckedAt(ID))->GetValue() ? 1 : 0;
            break;
        default:
            cerr << "Error in <H1Calculator::GetVariable>:" << endl;
            cerr << "variable type TLorentzVector not yet supported !" << endl;
            return 0;
        }
    }

    Float_t GetVariable(const Int_t ID, const Int_t index) const
    {
        if (!(ID >= 0 && ID < fNVariables)) {
            cerr << "Error in <H1Calculator::GetVariable(index)>: id " << ID << " exceeds range !" << endl;
            return 0;
        }

        switch (static_cast <H1VariableLookUpTableEntry*> (fNameLookUp.UncheckedAt(ID))->GetVariableType()) {
        case 'f':
            return static_cast <H1FloatFunctionPtr*> (fGetterFunctionLookUp.UncheckedAt(ID))->GetValue(index);
            break;
        case 'i':
            return static_cast <H1IntFunctionPtr*> (fGetterFunctionLookUp.UncheckedAt(ID))->GetValue(index);
            break;
        case 'b':
            return static_cast <H1BoolFunctionPtr*> (fGetterFunctionLookUp.UncheckedAt(ID))->GetValue(index) ? 1 : 0;
            break;
        default:
            cerr << "Error in <H1Calculator::GetVariable>:" << endl;
            cerr << "variable type TLorentzVector not yet supported!" << endl;
            return 0;
        }
    }
    ClassDef(H1Calculator, 1) // Calculator base class
};

H1Calculator& TheCalculator();
extern H1Calculator* gH1Calc;

#endif

#ifndef MFDRunAction_h
#define MFDRunAction_h 1

#include "G4UserRunAction.hh"
#include "globals.hh"
#include <vector>

class G4Run;

class MFDRunAction : public G4UserRunAction
{
  public:
    MFDRunAction();
   ~MFDRunAction();

  public:
    virtual G4Run* GenerateRun();
    void    BeginOfRunAction(const G4Run*);
    void    EndOfRunAction(const G4Run*);

};

#endif

#ifndef novaSteppingAction_h
#define novaSteppingAction_h

#include "G4UserSteppingAction.hh"

class G4Step;
class G4SteppingManager;

class novaSteppingAction : public G4UserSteppingAction
{
public:
  novaSteppingAction();
  ~novaSteppingAction();
public:
  void UserSteppingAction(const G4Step*);
private:
  int PreviousTrack;
};
#endif

#ifndef novaTrackingAction_h
#define novaTracingAction_h 1

#include "G4UserTrackingAction.hh"
#include "G4Track.hh"

class novaTrackingAction : public G4UserTrackingAction
{
public:
  novaTrackingAction();
  ~novaTrackingAction();

  void PreUserTrackingAction(const G4Track*);
  void PostUserTrackingAction(const G4Track*);
};
#endif

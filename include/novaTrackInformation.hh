//This class stores the statistics for each photon
//It is attached by novaTrackingAction
#ifndef novaTrackInformation_h
#define novaTrackInformation_h 1

#include "globals.hh"
#include "G4ThreeVector.hh"
#include "G4Allocator.hh"
#include "G4Track.hh"
#include "G4VUserTrackInformation.hh"

class novaTrackInformation : public G4VUserTrackInformation
{
 public:
  novaTrackInformation();
  novaTrackInformation(const G4Track *aTrack);
  ~novaTrackInformation();
  
  inline void *operator new(size_t);
  inline void operator delete(void *aTrackInfo);
  
 private:
  G4ThreeVector BirthPosition;
  G4double BirthLambda;
  G4double DistBeforeWLS;
  G4double DistAfterWLS;
  G4double TotalTrackLength;

  G4int ReflectBeforeWLS;
  G4int ReflectAfterWLS;
  G4int Reflections;
  G4int TotalInternalReflections;

  G4int WLSCount;

  
 public:

  G4ThreeVector GetBirthPosition() const {return BirthPosition;}
  G4double GetBirthLambda() const {return BirthLambda;}
  G4double GetDistBeforeWLS() const {return DistBeforeWLS;}
  G4double GetDistAfterWLS() const {return DistAfterWLS;}
  G4double GetTotalTrackLength() const {return TotalTrackLength;}

  G4int GetReflections() const {return Reflections;}
  G4int GetReflectBeforeWLS() const {return ReflectBeforeWLS;}
  G4int GetReflectAfterWLS() const {return ReflectAfterWLS;}
  G4int GetWLSCount() const {return WLSCount;}
  G4int GetTotalInternalReflections() const {return TotalInternalReflections;}

  void CountWLS(){WLSCount++;}
  void CountReflections();
  void CountTotalInternalReflections();


  void SetDistBeforeWLS(G4double dist){DistBeforeWLS=dist;}
  void Print() const {;}
};

extern G4Allocator<novaTrackInformation> TrackInformationAllocator;

inline void* novaTrackInformation::operator new(size_t)
{ void* aTrackInfo;
  aTrackInfo = (void*)TrackInformationAllocator.MallocSingle();
  return aTrackInfo;
}

inline void novaTrackInformation::operator delete(void *aTrackInfo)
{ TrackInformationAllocator.FreeSingle((novaTrackInformation*)aTrackInfo);}

#endif

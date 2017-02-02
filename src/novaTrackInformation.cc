#include "novaTrackInformation.hh"
#include "G4VProcess.hh"
#include "G4SystemOfUnits.hh"

G4Allocator<novaTrackInformation> TrackInformationAllocator;
novaTrackInformation::novaTrackInformation()
{}
novaTrackInformation::novaTrackInformation(const G4Track *aTrack)
{
  //set new TrackInformation for each photon at scintillation
  if(!aTrack->GetUserInformation()){
    BirthPosition=aTrack->GetVertexPosition();
    BirthLambda=1240/(aTrack->GetTotalEnergy()/eV);
    DistBeforeWLS=0;
    ReflectBeforeWLS=0;
    ReflectAfterWLS=0;
    WLSCount=0;
  }
  //set new TrackInformation for WLS photons, attach parent info
  else{
    novaTrackInformation *anInfo=(novaTrackInformation*)(aTrack->GetUserInformation());
    BirthPosition=aTrack->GetVertexPosition();
    BirthLambda=1240/(aTrack->GetTotalEnergy()/eV);
    DistBeforeWLS=aTrack->GetTrackLength();
    ReflectBeforeWLS=anInfo->GetReflectBeforeWLS();
    ReflectAfterWLS=anInfo->GetReflectAfterWLS();
    WLSCount=anInfo->GetWLSCount()+1;
  }
  
}

void novaTrackInformation::CountReflections()
{
  if(WLSCount==0)
    ReflectBeforeWLS++;
  else
    ReflectAfterWLS++;
}

void novaTrackInformation::CountTotalInternalReflections()
{
  //  if(WLSCount == 0)
    TotalInternalReflections++;
    //  else
    //    TotalInternalReflections++;

}

novaTrackInformation::~novaTrackInformation()
{}


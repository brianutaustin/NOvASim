
// Geometry Header File 
//   novaGeometry.hh generated by Geant4 Geometry Editor at Fri Oct 28 17:12:28 CDT 2005  


#ifndef novaGeometry_h
#define  novaGeometry_h  1

#include "G4VUserDetectorConstruction.hh"
#include "G4LogicalVolume.hh"
#include "G4MaterialTable.hh"
#include "NOVAstrip.hh"
#include "G4Material.hh"
#include "G4OpticalSurface.hh"
#include "G4LogicalSkinSurface.hh"
#include "G4MaterialPropertiesTable.hh"

class G4VPhysicalVolume;


class  novaGeometry:  public G4VUserDetectorConstruction
   {
   public:
      novaGeometry();
     ~novaGeometry();

   public:
     G4VPhysicalVolume* Construct();
     //returns the logical world volume, used in the main nova.cc file
     G4LogicalVolume* GetLWorld() {return l_world;}
   private:

     NOVAstrip *thestrip;
     
     G4Material* Vacuum;
     
     G4LogicalVolume *stripmodule;
     G4LogicalVolume *l_PMT;
     G4LogicalVolume *l_world;
     
     G4VPhysicalVolume *p_PMT1;
     G4VPhysicalVolume *p_PMT2;  
     
     G4MaterialPropertiesTable *vacMPT;
     G4MaterialPropertiesTable *pmtMPT;
     
     G4OpticalSurface *opt_pmt;
     
     G4LogicalSkinSurface *skin_pmt;
     
     
     G4VisAttributes* invis;
     G4VisAttributes *black;
     G4VisAttributes *green;
     G4VisAttributes * blue;
     
     
     
     
     G4double fiberradius;
     G4int numfibers;
     G4double paintthickness;
     G4double stripheight;
     G4double fiblength;
     G4double stripwidth;
     G4double curve_radius;
     G4double off_axis_dist;

  };
#endif

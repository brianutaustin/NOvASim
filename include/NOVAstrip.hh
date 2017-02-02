/*This class is used to create a NOVA plastic scintillator strip
GetModule() returns the logical volume containing all the components
GetLogicStrip() returns the scintillator for use as an active detector
GetOptVac() returns the MPT for Vacuum, to avoid having to redefine
*/

#ifndef NOVAstrip_h
#define NOVAstrip_h

#include "G4Element.hh"
#include "G4LogicalVolume.hh"
#include "G4MaterialPropertiesTable.hh"
#include "G4OpticalSurface.hh"
#include "G4SubtractionSolid.hh"
#include "G4LogicalSkinSurface.hh"
#include "G4UnionSolid.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4Torus.hh"

class WLSfiber;

class NOVAstrip
{
public:
  //constructor, makes default strip except for specified values

  //  NOVAstrip(G4double length,G4double fiberradius,G4double fiberlength,G4int numfibers);

  //constructor, builds from keys specified in buildfile
  NOVAstrip(G4String buildfile);
  ~NOVAstrip();
  void Construct();
  
public:
  //returns the logical volume containing the strip,paint,fibers,and epoxy
  G4LogicalVolume *GetModule(){return l_holder;}
  //returns the scintillator strip logical volume
  G4LogicalVolume *GetLogicStrip(){return l_strip;}
  //returns the G4MaterialPropertiesTable for vacuum
  //use this to assign the vacuum in the mother volume optical properties
  G4MaterialPropertiesTable *GetOptVac(){return vacMPT;}

  G4int GetNumFibers(){return numfibers;}
  G4double GetFibRadius(){return fibradius;}
  G4double GetFibLength(){return fiblength;}
  G4double GetStripLength(){return striplength;}
  G4double GetStripWidth(){return stripwidth;}
  G4double GetStripHeight(){return stripheight;}
  G4double GetPaintThickness(){return paintthickness;}
  G4double GetCurveRadius(){return curve_radius;}
  G4double GetOffAxisDist(){return off_axis_dist;}



  void SetValuestoDefault();
private:

  WLSfiber *FibMaker;


  G4Element *elementH;
  G4Element *elementC;
  G4Element *elementO;
  G4Element *elementTi;

  G4Material *scintillator;
  G4Material *Polystyrene;
  G4Material *Vacuum;
  G4Material *Ti02;
  G4Material *coating;

  G4MaterialPropertiesTable *vacMPT;
  G4MaterialPropertiesTable *scintMPT;
  G4MaterialPropertiesTable *paintMPT;


  
  G4Box *s_holder;
  G4Tubs *s_fiber;
  G4Box *startstrip;
  G4Box *startstrip2;
  G4UnionSolid *add_box_strip;
  G4Tubs *s_inner_corner1;
  G4Tubs *s_inner_corner2;
  G4Tubs *s_inner_corner3;
  G4Tubs *s_inner_corner4;
  G4UnionSolid *add_corner1;
  G4UnionSolid *add_corner2;
  G4UnionSolid *add_corner3;
  G4UnionSolid *add_corner4;
  G4Box *startpaint1;
  G4Tubs *s_outer_corner1;
  G4UnionSolid *add_outer_corner1;
  G4Box *startpaint2;
  G4UnionSolid *add_second_wall;
  G4Tubs *s_outer_corner2;
  G4UnionSolid *add_outer_corner2;
  G4UnionSolid *add_third_wall;
  G4Tubs *s_outer_corner3;
  G4UnionSolid *add_outer_corner3;
  G4UnionSolid *add_fourth_wall;
  G4Tubs *s_outer_corner4;
  G4UnionSolid *add_outer_corner4;
  G4Torus *s_curve_fiber;
  G4SubtractionSolid *s_strip3;

  G4OpticalSurface *op_paint;

  G4VisAttributes *clear;
  G4VisAttributes *white;
  G4VisAttributes *invis;
  G4VisAttributes *green;

  G4SubtractionSolid *s_strip;
  G4SubtractionSolid *s_strip2;
  G4SubtractionSolid *nextpaint;
  G4SubtractionSolid *nextpaint2;


  G4LogicalVolume *l_holder;
  G4LogicalVolume *l_strip;
  G4LogicalVolume *l_paint;
  G4LogicalVolume *l_fiber;


  G4VPhysicalVolume *p_strip;

  G4VPhysicalVolume *p_fiber;
  G4VPhysicalVolume *p_paint;

  G4LogicalSkinSurface *b_paint;



  G4double striplength;
  G4double fibradius;
  G4double fiblength;
  G4double paintthickness;
  G4double stripwidth;
  G4double stripheight;
  G4int numfibers;
  G4int paintends;
  G4int paintoneend;
  G4bool usegroove;
  G4double groovedepth;
  G4double groovewidth;
  G4double grooveindex;
  G4double lightyield;
  G4double curve_radius;
  G4double off_axis_dist;

};
#endif

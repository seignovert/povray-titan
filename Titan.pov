// @brief    Render a synthetic image of Saturn and its rings under specific geometry conditions
// @author   B.Seignovert (univ-reims@seignovert.fr)
// @date     2016/09/30
//----------------------------------------------------

#version 3.5;

#include "colors.inc"
#include "transforms.inc"

// Titan configuration from Image: N1827942759_1
#declare SC_lat =       0; // Subspacecraft  latitude  [deg_N]
#declare SC_lon =     302; // Subspacecraft  longitude [deg_W]
#declare SS_lat =      26; // Subsolar       latitude  [deg_N]
#declare SS_lon =     161; // Subsolar       longitude [deg_W]
#local D_Obs    =   1.6e6; // Observer distance        [km]
#declare North  =     176; // Target North clock angle [deg]

// Parameters
#local D_Sun      = 1.5e9; // Sun  distance [km]
#local Inst_Angle =  .352; // ISS Narrow Angle Camera

// Convert Latitude/Longitude coordinates in XYZ
#macro XYZ(lon,lat)
  <cos(radians(-lon))*cos(radians(lat)),sin(radians(lat)),sin(radians(-lon))*cos(radians(lat))>
#end

// Sub-solar point
#local SS = XYZ(SS_lon,SS_lat);
#local SC = XYZ(SC_lon,SC_lat);

camera {
   angle Inst_Angle
   location SC * D_Obs
   look_at <0,0,0>
   right x*image_width/image_height
   Axis_Rotate_Trans(SC,-North)
}            
        
light_source{ 
   SS * D_Sun
   color White
} 

#include "src/Titan.inc"
object { Titan }
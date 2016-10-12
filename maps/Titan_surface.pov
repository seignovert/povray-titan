// @brief    Render a synthetic image of Saturn and its rings under specific geometry conditions
// @author   B.Seignovert (univ-reims@seignovert.fr)
// @date     2016/09/30
//----------------------------------------------------

#version 3.5;

#include "colors.inc"
#include "transforms.inc"

// Titan configuration

#declare SC_lon =     329; // Subspacecraft  longitude [deg_W]
#declare SC_lat =     -16; // Subspacecraft  latitude  [deg_N]
#local D_Obs    =    1.e6; // Observer distance        [km]
#declare North  =     322; // Target North clock angle [deg]

#declare SS_lon =       0; // Subsolar       longitude [deg_W]
#declare SS_lat =     -13; // Subsolar       latitude  [deg_N]

#local D_Sun      = 1.5e9; // Sun  distance [km]
#local Inst_Angle =  .352; // ISS Narrow Angle Camera

#declare Map_Surface = "Titan-ISS-color.jpg" // Titan surface map [Optional]

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

#include "../src/Titan.inc"
object { Titan_surface }
object { Titan_grid }

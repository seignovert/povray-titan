// @brief    PovRay model of Titan and its atmosphere
// @author   B.Seignovert (univ-reims@seignovert.fr)
// @date     2016/10/12
//----------------------------------------------------

// External package
#include "colors.inc"

// Local parameters
#local R_Titan    =          2575; // Titan radius                 [km]

// Haze
#local Haze_bot   =           300; // Titan bottom main haze layer [km]
#local Haze_top   =           350; // Titan top    main haze layer [km]
#local Haze_color = <.40,.55,.60>; // Titan color  main haze layer [km]

// Detached haze
#local DHL_bot   =        400; // Titan bottom detached haze layer [km]
#local DHL_top   =        420; // Titan top    detached haze layer [km]
#local DHL_color = <.2,.3,.8>; // Titan color  detached haze layer [km]

// Convert Latitude/Longitude coordinates in XYZ
#macro XYZ(lon,lat)
  <cos(radians(-lon))*cos(radians(lat)),sin(radians(lat)),sin(radians(-lon))*cos(radians(lat))>
#end

// Latitude / Longitude grid
#macro LonLatGrid(nLon,nLat,R)
  #if ( nLon >0 ) #local dLon = 360. / nLon; #end
  #if ( nLat >0 ) #local dLat = 180. / nLat; #end
  #local c = 0;
  #while (c < nLon)
    #local lon = c * dLon - 90;
    difference {
      torus { R_Titan,R }
      plane { -z, 0 }
      rotate -90 * z
      rotate lon * y
    }
    #local c = c + 1;
  #end
  #local c = 1;
  #while (c < nLat)
    #local lat = radians(c * dLat - 90);
    torus {
      R_Titan*cos(lat),R
      translate R_Titan * sin(lat) * y
    }
    #local c = c + 1;
  #end
#end

// Draw circle on the planet
#macro circle(lon,lat,R,Color)
  torus {
    R_Titan, R
    pigment{ color Color }
    finish { ambient .7 }
    rotate (lat+90) * z
    rotate lon * y
  }
  #local Vector = XYZ(lon,lat);
  sphere {
    Vector * R_Titan, 100
    pigment{ color Color }
    finish { ambient 1 }
  }
#end

// Titan Surface
#local Surface = object {
   sphere {
      <0,0,0> R_Titan
      texture {
         pigment{
            image_map {
                #ifdef (Map_Surface)
                    jpeg Map_Surface
               #else
                    jpeg "maps/Titan-ISS-color.jpg"
                #end
                map_type 1
                interpolate 4
            }
         }
         finish { 
            ambient .25
            diffuse 1
            phong .1
         }
      }
   }
}

// Titan Haze
#local Haze = sphere {
    0, 1
    pigment {rgbt .9} hollow
    interior {
        media{
            scattering { 3, Haze_color }
            intervals 1
            density{
                spherical
                density_map {
                  [ 0. * (Haze_top - Haze_bot) rgb .0 ]
                  [ .1 * (Haze_top - Haze_bot) rgb .3 ]
                  [ .5 * (Haze_top - Haze_bot) rgb .7 ]
                  [ .3 * (Haze_top - Haze_bot) rgb .3 ]
                  [ 1. * (Haze_top - Haze_bot) rgb .0 ]
                }
            }
        }
    }
    scale R_Titan + Haze_top
}

// Titan Detached Haze Layer
#local DHL = sphere {
    0, 1
    pigment {rgbt 1.} hollow
    interior {
        media{
            scattering { 4, DHL_color }
            intervals 1
            density{
                spherical
                density_map {
                  [ 0. * (DHL_top - DHL_bot) rgb .0  ]
                  [ .1 * (DHL_top - DHL_bot) rgb .05 ]
                  [ 1. * (DHL_top - DHL_bot) rgb .0  ]
                }
            }
        }
    }
    scale R_Titan + DHL_top
}

// Titan grid
#local Grid = difference {
    #ifndef ( SS_lat ) #local SS_lat = 0.; #warning "SubSolar      latitude  not defined." #end
    #ifndef ( SS_lon ) #local SS_lon = 0.; #warning "SubSolar      longitude not defined." #end
    #ifndef ( SC_lat ) #local SC_lat = 0.; #warning "SubSpacecraft latitude  not defined." #end
    #ifndef ( SC_lon ) #local SC_lon = 0.; #warning "SubSpacecraft longitude not defined." #end
    union{
        union {
            LonLatGrid(36,18, 2)
            LonLatGrid(12, 6,10)
            LonLatGrid(0,2,30)
            LonLatGrid(1,0,30)
            pigment { color Red }
            finish { ambient .7 }
        }
        circle(SS_lon,SS_lat,20,Yellow)
        circle(SC_lon,SC_lat,20,Blue)
    }
    plane { XYZ(SC_lon,SC_lat), 0 }
}

#declare Titan_surface = object{ Surface }
#declare Titan_grid    = object{ Grid }
#declare Titan         = object{
    union{ 
        object{ Haze }
        object{ DHL }
    }
}
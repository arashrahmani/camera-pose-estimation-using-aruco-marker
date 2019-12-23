#ifndef CameraConfig
#define CameraConfig

#include <vector>
using namespace std;
struct CameraParams{
 double sX ,sY;    // are coefficients for (meters per every vertical pixel) and (meters per every horizontal pixel)
  
 double fX ,fY;    // focal distance X and focal distance Y
  
 double cX ,cY;    // centerX and centerY of camera
  
 double k1 ,k2 ,p1 ,p2 ,k3;    // distortion coefficients
                                     //p[i]s are for tangential distortion and k[i]s are for radial distortion
};

#endif

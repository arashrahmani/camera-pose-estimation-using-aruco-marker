#include <vector>
using namespace std;
struct CameraParams{
  long double sX ,sY;    // are coefficients for (meters per every vertical pixel) and (meters per every horizontal pixel)
  
  long double fX ,fY;    // focal distance X and focal distance Y
  
 long  double cX ,cY;    // centerX and centerY of camera
  
 long  double k1 ,k2 ,k3 ,p1 ,p2;    // distortion coefficients
};

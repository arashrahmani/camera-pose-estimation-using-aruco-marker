#include <vector>
#include <opencv4/opencv2/highgui.hpp>
#include <opencv4/opencv2/imgproc.hpp>
using namespace std;
using namespace cv;
struct square{
  
  // u is horizontal coordinate and v is vertical coordinate on the sensor for any point or (coordinates of pixels on the sensor to meter)
  long double uA,vA,uB,vB,uC,vC,uD,vD;
  long double u[4];
  long double v[4];
  // the square side lengh of arucoes -example: 0.1 (m)
  float length = 0.1;
  
  // k will be u/fX and c will be v/fY
  long double k[4];
  long double c[4];
  
  // r0 , r1 , r2 , r3 are real coordinates for A,B,C,D on the square with respect to the camera sensor or (coordinates of A,B,C,D in real 3D world with respect to camera lens) 
  long double r[4][3];
  
  int squareID;
};

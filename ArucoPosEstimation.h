#ifndef ArucoposEstimation
#define ArucoposEstimation

#include <vector>
#include <opencv4/opencv2/core.hpp>
using namespace std;
using namespace cv;
struct square{
  
  // the position of Aruco corners on the sensor to meter (u,v)
  Point2d imageCorners[4];
  
  // the position of Aruco corners in real world to meter (X,Y,Z) . The origin of world is the Center of lens (center of aperture in pinhole camera model)
  Point3d realCorners[4];
  
  // the square side lengh of arucoes -example: 0.1 (m)
  float length = 0.1;

  // every vertex have a pair of (k , c) such that: k is u/fX and c is v/fY
  double kc[4][2];
  
  int squareID;
};

#endif

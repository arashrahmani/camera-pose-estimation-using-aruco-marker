#include <sys/sysmacros.h>
#include "ArucoPosEstimation.h"
#include "cameraConfig.h"
#include <vector>
#include "squareMat.h"
#include <opencv4/opencv2/highgui.hpp>
#include <opencv4/opencv2/imgproc.hpp>
#include <opencv4/opencv2/aruco.hpp>
#include <opencv4/opencv2/calib3d.hpp>
#include <iostream>
#include <math.h>
#include <opencv4/opencv2/core.hpp>

#define a 0.1
using namespace cv;
using namespace std;

int main(){
  CameraParams LogitechC922;
  
  // 1080p is resolution for LogitechC922 that size of image plane is 1920*1080(pixel)
  
  // sX(m) = sensor windth / number of pixels in horizontal axis (this case: 1920)
  // sY(m) = height of sensor / number of pixels in vertical axis (this case: 1080)
  LogitechC922.sX = 0.0000025;  
  LogitechC922.sY = 0.0000033;   
  
  LogitechC922.fX = 619.4993;
  LogitechC922.fY = 620.8682;
  LogitechC922.cX = 311.6270;
  LogitechC922.cY = 227.3101;
  
  // distortion coefficients
  LogitechC922.k1 = 0.1287;
  LogitechC922.k2 = -0.2772;
  LogitechC922.k3 = 0.1752;
  LogitechC922.p1 = -0.0007;
  LogitechC922.p2 = -0.0064;
  
  //    creating and filling cameraMatrix such that
  //                                  {LogitechC922.fX ,        0      , LogitechC922.cX}
  //             cameraMatrix =       {        0       ,LogitechC922.fY, LogitechC922.cY}
  //                                  {        0       ,        0      ,        1       }
  Mat cameraMatrix = Mat::eye(3, 3, CV_64F);
  cameraMatrix.at<double>(0,0) = LogitechC922.fX;
  cameraMatrix.at<double>(1,1) = LogitechC922.fY;
  cameraMatrix.at<double>(0,2) = LogitechC922.cX;
  cameraMatrix.at<double>(1,2) = LogitechC922.cY;
  cameraMatrix.at<double>(2,2) = 1;
   
  // creating and filling distCoeffs matrix with coefficients
  Mat distCoeffs = Mat::zeros(8, 1, CV_64F);
  distCoeffs.at<double>(0,0) = LogitechC922.k1;
  distCoeffs.at<double>(1,0) = LogitechC922.k2;
  distCoeffs.at<double>(2,0) = LogitechC922.p1;
  distCoeffs.at<double>(3,0) = LogitechC922.p2;
  distCoeffs.at<double>(4,0) = LogitechC922.k3;

  // converting (pixel) to (meter)
  LogitechC922.fX *= LogitechC922.sX;
  LogitechC922.fY *= LogitechC922.sY;
  LogitechC922.cX *= LogitechC922.sX;
  LogitechC922.cY *= LogitechC922.sY;
  
  // assigning a dictionary for desired arucos
  cv::Ptr <cv::aruco::Dictionary> dictionary = cv::aruco::getPredefinedDictionary(cv::aruco::DICT_6X6_250);
  
  // saving the square size (meter)
//   float a = 0.1;

  Mat frame;
  
  // creating an object from cv::VideoCapture that opens /dev/video2
  VideoCapture cap(2);
    while(1){
      cap>>frame;
//      cap.set(3,1920);
//      cap.set(4,1080);

      Mat tempMat = frame.clone();
      
      // undistorting the image using known distortion coefficiets and intrinsic camera matrix
      undistort(tempMat,frame,cameraMatrix,distCoeffs);
      std::vector<vector<Point2f > >corners;
      std::vector<int >IDS;
      
      // detecting aruco markers with detectMarkers function in opencv library
      cv::aruco::detectMarkers(frame,dictionary,corners,IDS);
      
      // printing IDs of detected aucoes
      for(int i = 0;i < IDS.size();i ++){
        cout<<"your IDs are : "<<IDS[i]<<endl;
      }
      
      vector<square >arucoes(IDS.size());
      for(int i  = 0;i < IDS.size();i ++){
        for(int j = 0;j < 4;j ++){
          
          // setting the square ID
          arucoes[i].squareID = IDS[i];
          
          // converting pixel-meter for every vertex with coordinate (u , v)
          arucoes[i].imageCorners[j].x = (corners[i][j].x * LogitechC922.sX) - LogitechC922.cX;
          arucoes[i].imageCorners[j].y = (corners[i][j].y * LogitechC922.sY) - LogitechC922.cY;
          
          // filling the k for corner j such that k = u/fX
          arucoes[i].kc[j][0] = arucoes[i].imageCorners[j].x / LogitechC922.fX;
          
          // filling the c for corner j such that c = v/fY
          arucoes[i].kc[j][1] = arucoes[i].imageCorners[j].y / LogitechC922.fY;
        }

        // A,B,C,D are vertices of arucoes and are clockwise starting from top-left point
        cout<<"ID of Aruco is: "<<IDS[i]<<endl;
        cout<<"uA = "<<arucoes[i].imageCorners[0].x<<" "<<"vA = "<<arucoes[i].imageCorners[0].y<<endl;
        cout<<"uB = "<<arucoes[i].imageCorners[1].x<<" "<<"vB = "<<arucoes[i].imageCorners[1].y<<endl;
        cout<<"uC = "<<arucoes[i].imageCorners[2].x<<" "<<"vC = "<<arucoes[i].imageCorners[2].y<<endl;
        cout<<"uD = "<<arucoes[i].imageCorners[3].x<<" "<<"vD = "<<arucoes[i].imageCorners[3].y<<endl;
        
          //     creating and filling Q matrix such that
// // // // // //           double Q[3][3] =
// // // // // //           {
// // // // // //             {kB, kD, -kA},
// // // // // //             {cB, cD, -cA},
// // // // // //             { 1,  1,  -1},
// // // // // //           };
          double Q[3][3] = 
          {
            {arucoes[i].kc[1][0], arucoes[i].kc[3][0], -arucoes[i].kc[0][0]},
            {arucoes[i].kc[1][1], arucoes[i].kc[3][1], -arucoes[i].kc[0][1]},
            {         1         ,         1          ,          -1         }
          };
          Mat QMat = Mat(Size(3,3),CV_64F,Q);
          Mat Qinv = Mat(Size(3,3),CV_64F);
          cout<<"Q is:"<<endl;
          for(int i = 0; i < 3; i++){
            for(int j = 0; j < 3; j++){
              cout<<Q[i][j]<<" ";
              cout<<QMat.at<double>(i,j)<<" ";
              if(j==2){
                cout<<endl;
              }
            }
          }
          Qinv = QMat.inv();
//           double L0[3] = {kC, cC, 1};
          double L0[3] = {arucoes[i].kc[2][0], arucoes[i].kc[2][1], 1};
          double L[3] = {0, 0, 0};
          for(int i=0;i<3;i++){
            for(int j=0;j<3;j++){
              L[i] += Qinv.at<double>(i,j)*L0[j];
            }
          }
          double M = pow(arucoes[i].kc[1][0]*L[0]-arucoes[i].kc[0][0]*L[2], 2)+pow(arucoes[i].kc[1][1]*L[0]-arucoes[i].kc[0][1]*L[2],2)+pow(L[0]-L[2],2);
          // rC[2] = a/sqrt(M);
          // rA[2] = L[2]*rC[2];
          // rB[2] = L[0]*rC[2];
          // rD[2] = L[1]*rC[2];
          arucoes[i].realCorners[2].z = a/sqrt(M);
          arucoes[i].realCorners[0].z = L[2]*arucoes[i].realCorners[2].z;
          arucoes[i].realCorners[1].z = L[0]*arucoes[i].realCorners[2].z;
          arucoes[i].realCorners[3].z = L[1]*arucoes[i].realCorners[2].z;

          for (int cornerNum = 0;cornerNum < 4;cornerNum ++){
            arucoes[i].realCorners[cornerNum].x = arucoes[i].kc[cornerNum][0] * arucoes[i].realCorners[cornerNum].z;
            arucoes[i].realCorners[cornerNum].y = arucoes[i].kc[cornerNum][1] * arucoes[i].realCorners[cornerNum].z;
          }
          // arucoes[i].realCorners[0].x = [i].kc[0][0] * arucoes[i].realCorners[0].z;
          // arucoes[i].realCorners[0].y = arucoes[i].kc[0][1] * arucoes[i].realCorners[0].z;
          // arucoes[i].realCorners[1].x = arucoes[i].kc[1][0] * arucoes[i].realCorners[1].z;
          // arucoes[i].realCorners[1].y = arucoes[i].kc[1][1] * arucoes[i].realCorners[1].z;
          // arucoes[i].realCorners[2].x = arucoes[i].kc[2][0] * arucoes[i].realCorners[2].z;
          // arucoes[i].realCorners[2].y = arucoes[i].kc[2][1] * arucoes[i].realCorners[2].z;
          // arucoes[i].realCorners[3].x = arucoes[i].kc[3][0] * arucoes[i].realCorners[3].z;
          // arucoes[i].realCorners[3].y = arucoes[i].kc[3][1] * arucoes[i].realCorners[3].z;
    ///////////////////////////////////////////////////////////////
          Point2f center(0,0);
          Point3d realCenter(0,0,0);
  //         realCenter = {(rA[0]+rB[0]+rC[0]+rD[0])/4.0,(rA[1]+rB[1]+rC[1]+rD[1])/4.0,(rA[2]+rB[2]+rC[2]+rD[2])/4.0};
          cout<<"the square id is:"<<arucoes[i].squareID<<endl;
          cout<<"real A is :"<<arucoes[i].realCorners[0].x<<"  "<<arucoes[i].realCorners[0].y<<" "<<arucoes[i].realCorners[0].x<<endl;
          cout<<"real B is :"<<arucoes[i].realCorners[1].x<<"  "<<arucoes[i].realCorners[1].y<<" "<<arucoes[i].realCorners[1].x<<endl;
          cout<<"real C is :"<<arucoes[i].realCorners[2].x<<"  "<<arucoes[i].realCorners[2].y<<" "<<arucoes[i].realCorners[2].x<<endl;
          cout<<"real D is :"<<arucoes[i].realCorners[3].x<<"  "<<arucoes[i].realCorners[3].y<<" "<<arucoes[i].realCorners[3].x<<endl;
          for( int k = 0; k < 4; k++ ){
            line( frame, corners[i][k], corners[i][(k+1)%4], Scalar(0,0,150), 1, 8 );
            circle(frame,corners[i][k],1,Scalar(0,255,0),1);
            // ostringstream co;
            center+=corners[i][k];
          }
        center/=4;
        // putText(frame,co.str(),center,0,1,Scalar(0,0,150),4);
        circle(frame,center,2,Scalar(0,255,0),3);
      }
      cout<<IDS.size()<<" arucoes detected\n";
      imshow("",frame);
      cout<<"Fx is:       "<<LogitechC922.fX<<endl;
      cout<<"Fy is:       "<<LogitechC922.fY<<endl;   
      cv::waitKey(100); 
    }
}

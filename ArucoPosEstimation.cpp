#include "cameraConfig.h"
#include "squareMat.h"
#include <vector>
#include <opencv4/opencv2/highgui.hpp>
#include <opencv4/opencv2/imgproc.hpp>
#include <opencv4/opencv2/aruco.hpp>
#include <opencv4/opencv2/aruco/dictionary.hpp>
#include <opencv4/opencv2/calib3d.hpp>
#include <iostream>
#include <math.h>
#include <opencv4/opencv2/core.hpp>

using namespace cv;
using namespace std;
int main(){
  CameraParams LogitechC922;
  
  // 1080p is resolution for LogitechC922 that size of image plane is 1920*1080(pixel)
  LogitechC922.sX = 0.0000025;   //sX(m) == width of sensor 4.8(mm)/ 1920(pixel)
  LogitechC922.sY = 0.0000033;   //sY(m) == height of sensor 3.5(mm)/ 1080(pixel)
  
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
  
  //    crating and filling cameraMatrix such that
  //                                  {LogitechC922.fX ,        0      , LogitechC922.cX}
  //             cameraMatrix =       {        0       ,LogitechC922.fY, LogitechC922.cY}
  //                                  {        0       ,        0      ,        1       }
  Mat cameraMatrix = Mat::eye(3, 3, CV_64F);
  cameraMatrix.at<double>(0,0) = LogitechC922.fX;
  cameraMatrix.at<double>(1,1) = LogitechC922.fY;
  cameraMatrix.at<double>(0,2) = LogitechC922.cX;
  cameraMatrix.at<double>(1,2) = LogitechC922.cY;
  cameraMatrix.at<double>(2,2) = 1;
   
  //    creating and filling distCoeffs matrix with coefficients
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
  // intrinsic matrix of camera
  Mat intrinsic = Mat(3, 3, CV_32FC1);
  intrinsic.ptr<float>(0)[0] = LogitechC922.fX;
  intrinsic.ptr<float>(0)[1] = 0;
  intrinsic.ptr<float>(0)[2] = LogitechC922.cX;
  intrinsic.ptr<float>(1)[0] = 0;
  intrinsic.ptr<float>(1)[1] = LogitechC922.fY;
  intrinsic.ptr<float>(1)[2] = LogitechC922.cY;
  intrinsic.ptr<float>(2)[0] = 0;
  intrinsic.ptr<float>(2)[1] = 0;
  intrinsic.ptr<float>(2)[2] = 1;
  
  
  // assigning a dictionary for desired arucos
  cv::Ptr <cv::aruco::Dictionary> dictionary = cv::aruco::getPredefinedDictionary(cv::aruco::DICT_6X6_250);
  
  long double uA,vA,uB,vB,uC,vC,uD,vD;
  float a = 0.1;
  long double kA,kB,kC,kD;
  long double cA,cB,cC,cD;
  long double rA[3] = {0,0,0};
  long double rB[3] = {0,0,0};
  long double rC[3] = {0,0,0};
  long double rD[3] = {0,0,0};
  
  
  Mat frame;
  VideoCapture cap(2);
    while(1){
      cap>>frame;
      Mat temp = frame.clone();
      undistort(temp,frame,cameraMatrix,distCoeffs);
      imshow("he",frame);
      std::vector<vector<Point2f>>corners;
      std::vector<int >IDS;
      cv::aruco::detectMarkers(frame,dictionary,corners,IDS);
      for(int i=0;i<IDS.size();i++){
        cout<<"your id's are : "<<IDS[i]<<endl;
      }
      for(int i = 0;i<corners.size();i++){
        uA = (corners[i][0].x * LogitechC922.sX) - LogitechC922.cX;
        uB = (corners[i][1].x * LogitechC922.sX) - LogitechC922.cX;
        uC = (corners[i][2].x * LogitechC922.sX) - LogitechC922.cX;
        uD = (corners[i][3].x * LogitechC922.sX) - LogitechC922.cX;
        vA = (corners[i][0].y * LogitechC922.sY) - LogitechC922.cY;
        vB = (corners[i][1].y * LogitechC922.sY) - LogitechC922.cY;
        vC = (corners[i][2].y * LogitechC922.sY) - LogitechC922.cY;
        vD = (corners[i][3].y * LogitechC922.sY) - LogitechC922.cY;
        cout<<"uA = "<<uA<<" "<<"vA = "<<vA<<endl;
        cout<<"uB = "<<uB<<" "<<"vB = "<<vB<<endl;
        cout<<"uC = "<<uC<<" "<<"vC = "<<vC<<endl;
        cout<<"uD = "<<uD<<" "<<"vD = "<<vD<<endl;
        
        //////////////////////////////////////////////////////
        kA = uA / LogitechC922.fX;
        kB = uB / LogitechC922.fX;
        kC = uC / LogitechC922.fX;
        kD = uD / LogitechC922.fX;
        cA = vA / LogitechC922.fY;
        cB = vB / LogitechC922.fY;
        cC = vC / LogitechC922.fY;
        cD = vD / LogitechC922.fY;
        vector<vector<long double>> Q =
        {
          {kB, kD, -kA},
          {cB, cD, -cA},
          { 1,  1,  -1},
        };
        
        cout<<"Q is:"<<endl;
        for(int i = 0; i < 3; i++){
          for(int j = 0; j < 3; j++){
            cout<<Q[i][j]<<" ";
            if(j==2){
              cout<<endl;
            }
          }
        }
        long double Qdeterminant;
        Qdeterminant = kB*(-cD+cA)-kD*(-cB+cA)-kA*(cB-cD);
        cout<<"manually determinant is ::" <<Qdeterminant<<endl;
        long double hamsazeQ[3][3];
        long double invQ[3][3],TQ[3][3];
// // // //         Arash::squareMat QMat(Q,3),Qinverse(3);
// // // //         vector<vector<long double>>cofacs(3,vector<long double>(3));
// // // //         long double mydet;
// // // //         mydet = QMat.det();
// // // //         cout<<"i found this det:  "<<mydet<<endl;
// // // //         Qinverse = QMat.inverse();
// // // //         cout<<"i found this inverse:"<<endl;
// // // // // //         for(int i = 0;i < Qinverse.size ;i++){
// // // // // //           cout<<endl;
// // // // // //           for(int j = 0;j < Qinverse.size ;j++){
// // // // // //             Qinverse.getCofactor(Q,cofacs,i,j,3);
// // // // // //             cout<<endl;
// // // // // // //             cout<<Qinverse(i,j)<<"\t";
// // // // // //               for(int k = 0;k < Qinverse.size ;k++){
// // // // // //                 cout<<endl;
// // // // // //                 for(int u = 0;u < Qinverse.size ;u++){
// // // // // //                   cout<<cofacs[k][u]<<"\t";
// // // // // //                 }
// // // // // //               }
// // // // // //           }
// // // // // //         }
        //matrice hamsaze 
        hamsazeQ[0][0] = (-cD+cA);
        hamsazeQ[0][1] = -(-cB+cA);
        hamsazeQ[0][2] = (cB-cD);
        hamsazeQ[1][0] = -(-kD+kA);
        hamsazeQ[1][1] = (-kB+kA);
        hamsazeQ[1][2] = -(kB-kD);
        hamsazeQ[2][0] = ((-kD*cA)+(cD*kA));
        hamsazeQ[2][1] = -((-cA*kB)+(cB*kA));
        hamsazeQ[2][2] = ((kB*cD)-(kD*cB));
        //matrise elhaqi == Taranahadeye hamsazeh
        for(int i=0;i<3;i++){
          for(int j=0;j<3;j++){
            TQ[j][i] = hamsazeQ[i][j];
          }
        }
        // inverseQ == 1/det*(matrise elhaqi)
        cout<<"manually inverse of Q is: "<<endl;
        for(int i=0;i<3;i++){
          for(int j=0;j<3;j++){
            invQ[i][j] = TQ[i][j]/Qdeterminant;
            cout<<invQ[i][j]<<" ";
            if(j==2)
              cout<<endl;
          }
        }
        long double L0[3] = {kC, cC, 1};
        long double L[3] = {0, 0, 0};
        for(int i=0;i<3;i++){
          for(int j=0;j<3;j++){
            L[i] += invQ[i][j]*L0[j];
          }
        }
        long double M = pow(kB*L[0]-kA*L[2], 2)+pow(cB*L[0]-cA*L[2],2)+pow(L[0]-L[2],2);
        rC[2] = a/sqrt(M);
        rA[2] = L[2]*rC[2];
        rB[2] = L[0]*rC[2];
        rD[2] = L[1]*rC[2];
        
        rA[0] = kA * rA[2];
        rA[1] = cA * rA[2];
        rB[0] = kB * rB[2];
        rB[1] = cB * rB[2];
        rC[0] = kC * rC[2];
        rC[1] = cC * rC[2];
        rD[0] = kD * rD[2];
        rD[1] = cD * rD[2];
  ///////////////////////////////////////////////////////////////
        Point2f center(0,0);
        Point3d realCenter(0,0,0);
//         realCenter = {(rA[0]+rB[0]+rC[0]+rD[0])/4.0,(rA[1]+rB[1]+rC[1]+rD[1])/4.0,(rA[2]+rB[2]+rC[2]+rD[2])/4.0};
        cout<<"real A is :"<<rA[0]<<"  "<<rA[1]<<" "<<rA[2]<<endl;
        cout<<"real B is :"<<rB[0]<<"  "<<rB[1]<<" "<<rB[2]<<endl;
        cout<<"real C is :"<<rC[0]<<"  "<<rC[1]<<" "<<rC[2]<<endl;
        cout<<"real D is :"<<rD[0]<<"  "<<rD[1]<<" "<<rD[2]<<endl;
        for( int k = 0; k < 4; k++ ){
          line( frame, corners[i][k], corners[i][(k+1)%4], Scalar(0,0,150), 1, 8 );
          circle(frame,corners[i][k],1,Scalar(0,255,0),1);
    //      ostringstream co;
          center+=corners[i][k];
        }
        center/=4;
/// 	    putText(frame,co.str(),center,0,1,Scalar(0,0,150),4);
        circle(frame,center,2,Scalar(0,255,0),3);
      }
      imshow("detected arucos",frame);
      cout<<"Fx is:       "<<LogitechC922.fX<<endl;
      cout<<"Fy is:       "<<LogitechC922.fY<<endl;   



      //test
      vector<vector<long double>>example = {{15, 2,  3,   4,16},
                                            {5, 6,  7,   8,17},
                                            {9, 10, 11, 12,18},
                                            {13, 14, 15, 15,19},
                                            {20,21,22,23,24}
      };
      Arash::squareMat s1(example,5),inverses(5);
      cout<<"det is:  "<<s1.det()<<endl;
      inverses = s1.inverse();
      for(int i = 0;i < inverses.size;i ++){
        cout<<endl;
        for(int j = 0;j < inverses.size;j ++){
          cout<<inverses(i,j)<<"\t";
        }
      }
      cv::waitKey(100);
      
      
    }
//     for(int i=0;i<200;i++){
//       cv::aruco::drawMarker(dictionary, i, size_of_output, markerimage, 1);
//       imwrite ("markerimage"+std::to_string(i)+".png",markerimage);
//     }
}

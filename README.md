# getting-the-position-of-a-detected-square-in-3D-world-with-single-camera
implementation of getting the position of a detected square with respect to the camera with known intrinsic parameters and square size. this project is implemented with aruco markers but you can use any square else. 

for using this program\
  Prerequisites : \
    1. first of all you should calibrate the intrinsic camera parameters with any tool that you have (like Matlab calibration tool , opencv 3DCalib , etc.)\
    2. then you should putting the parameters of camera in ArucoPosEstimation.cpp in an object from cameraConfig \
    3. next step is importing the Aruco length (meters) in "a" variable \
    4. now you can compile the project using provided Makefile \

  Compilation : \
    in project directory use this command -> " make ArucoPosEstimation " \
    
attention please : this code is written for LogitechC920 & LogitechC922 but in latest versions i'll try to generalize it. \

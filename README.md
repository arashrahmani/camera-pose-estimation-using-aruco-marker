# getting-the-position-of-a-detected-square-in-3D-world-with-single-camera
## Introduction
Determining the position and orientation of a camera given its intrinsic parameters and a set of n correspondences between 3D points and their 2D projections is known as the Perspective-n-Point (PnP) problem .
In this project we provide an efﬁcient and robust solution to the P4P problem for estimating the 3D pose of a camera relative to a known sized square .

## Problem formulation
another form to define our problem(assuming that the intrinsic parameters are calculated): how to get the 3D pose of 4 square vertices relative to a single camera.

The pinhole camera model describes the mathematical relationship between the coordinates of a point in three-dimensional space and its projection onto the image plane of an ideal pinhole camera, where the camera aperture is described as a point and no lenses are used to focus light.

according to the fig(1),which shows a light ray that emits from an arbitrary point(A) on the 3D object and projection of that point(A') on the image plane, using pinhole camera model we can write the equations for every 4 vertices as shown in fig(2).

we sorted our vertices clockwise starting from top left and named them A-D as shown in fig(2).

<p align="center">
    <img width="100%" src="https://github.com/arashrahmani/getting-the-position-of-a-detected-square-in-3D-world-with-single-camera/blob/master/images/pinhole_camera_model.jpg" style="max-width:100%;">
    </a>
</p>
<p align="center">
    fig(1)
</p>
<p align="center">
    <img width="100%" src="https://github.com/arashrahmani/getting-the-position-of-a-detected-square-in-3D-world-with-single-camera/blob/master/images/fig2.jpg" style="max-width:100%;">
    </a>
</p>
<p align="center">
    fig(2)
</p>

* Now we should calculate (x,y,z) for any vertex.

* The coordinates of any pixel on the image is known and we should just transform the image points to the camera coordinate system and change the unit of coordinates (from pixels to meters). we will show you how to do that at the end of this article.

* There are 8 equations above with 12 unknowns (4*(x,y,z)).

### to get a better picture of what is going on let me ask you some questions.
_ First consider the position of our square in coordinate system {1}.

_ Consider the point "A" from the square.
<p align="center">
    <img width="30%" src="https://github.com/arashrahmani/getting-the-position-of-a-detected-square-in-3D-world-with-single-camera/blob/master/images/fig3.jpg" style="max-width:30%;">
    </a>
</p>
<p align="center">
    fig(3)
</p>
1. In what position can point "B" be relative to the point "A"?

    answer: Point "B" is located on a sphere with center "A" and radius "a".(fig(4))
_ Now consider the point "D".

</p>
2. In what position can point "D" be relative to the points "A" and "B"?

    answer: Point "D" is as far away from point "A" as "a" is. but "AB" is perpendicular to "AD".
    so the points that "D" can be placed there, is a circle that "AB" is perpendicular to it's plane.(fig(4))
<p align="center">
    <img width="50%" src="https://github.com/arashrahmani/getting-the-position-of-a-detected-square-in-3D-world-with-single-camera/blob/master/images/fig4.jpg" style="max-width:50%;">
    </a>
</p>
<p align="center">
    fig(4)
</p>

We create the new coordinate system {2} centered at point "A" and in the same direction of the coordinate system {1}(camera coordinate system). the new coordinate system is shown in fig(5).
<p align="center">
    <img width="100%" src="https://github.com/arashrahmani/getting-the-position-of-a-detected-square-in-3D-world-with-single-camera/blob/master/images/fig5.jpg" style="max-width:100%;">
    </a>
</p>
<p align="center">
    fig(5)
</p>
We saw that with a given point "A" at some arbitrary coordinate,"B" lies on a sphere and "D" lies on a circle.
Now if we want to evaluate the forth point ,"C" has a exact coordinate in the space.
For define the coordinate of point "C" ,we define the coordinate system{3} as shown in fig(6).
<p align="center">
    <img width="100%" src="https://github.com/arashrahmani/getting-the-position-of-a-detected-square-in-3D-world-with-single-camera/blob/master/images/fig6.jpg" style="max-width:100%;">
    </a>
</p>
<p align="center">
    fig(6)
</p>
The unit vectors(ex,ey,ez) are defined as follows:
<p align="center">
    <img width="100%" src="https://github.com/arashrahmani/getting-the-position-of-a-detected-square-in-3D-world-with-single-camera/blob/master/images/fig7.jpg" style="max-width:100%;">
    </a>
</p>
<p align="center">
    fig(7)
</p>
We have coordinates of points in coordinate system{3}(fig6), we want to get the point coordinates in the coordinate system{2}.
The coordinate system{3} is rotated with respect to the coordinate system{2} with rotation matrix R.
<p align="center">
    <img width="100%" src="https://github.com/arashrahmani/myreadme/blob/master/fig8.jpg" style="max-width:100%;">
    </a>
</p>
<p align="center">
    fig(8)
</p>
Now that the coordinates of all the 4 points have been found in coordinate system {3} (fig6),using the rotation matrix shown above, we can get the coordinates of the 4 points in coordinate system{2}(fig9).
<p align="center">
    <img width="100%" src="https://github.com/arashrahmani/getting-the-position-of-a-detected-square-in-3D-world-with-single-camera/blob/master/images/fig9.jpg" style="max-width:100%;">
    </a>
</p>
<p align="center">
    fig(9)
</p>
So the coordinates of points have been found in coordinate system {2},now using equations in fig(5), just using a single translation we can get the coordinates of points in coordinate system{1}.(shown in image below)
<p align="center">
    <img width="100%" src="https://github.com/arashrahmani/getting-the-position-of-a-detected-square-in-3D-world-with-single-camera/blob/master/images/fig10.jpg" style="max-width:100%;">
    </a>
</p>
<p align="center">
    fig(10)
</p>
** equation is from fig(1) for point "C".

using * and ** equatinos, we have the equations shown in fig(11).
<p align="center">
    <img width="100%" src="https://github.com/arashrahmani/getting-the-position-of-a-detected-square-in-3D-world-with-single-camera/blob/master/images/fig11.jpg" style="max-width:100%;">
    </a>
</p>
<p align="center">
    fig(11)
</p>
So far we have written three unknowns in terms of "Zc"(fig(11)), and the only unknown for our problem is "Zc". if we find the equation for "Zc", we solved the problem.(fig(12))

Usnig the equation for distance between points "A" and "B" we will get the answer of "Zc".(fig(12))
And because we set the positive direction of the Z axis in front of the camera, the positive answers are acceptable.(objects observed by a camera exist on the front of it)
<p align="center">
    <img width="100%" src="https://github.com/arashrahmani/getting-the-position-of-a-detected-square-in-3D-world-with-single-camera/blob/master/images/fig12.jpg" style="max-width:100%;">
    </a>
</p>
<p align="center">
    fig(12)
</p>

Well done !!!
Thank you for reading. I hope you enjoyed it. :)

solved by my dear friend Alireza Karimi (Mechatronics Research Laboratory)

a good day in cafe Toosi : Alireza Karimi and Arash Rahmani 2019-11-08
## Prerequisites :
    1. first of all you should calibrate the intrinsic camera parameters with any tool that you have (like Matlab calibration tool , opencv 3DCalib , etc.)
    2. then you should putting the parameters of camera in ArucoPosEstimation.cpp in an object from cameraConfig 
    3. next step is importing the Aruco length (meters) in "a" variable 
    4. now you can compile the project using provided Makefile  

  Compilation :
    in project directory use this command -> " make ArucoPosEstimation "
    
attention please : this code is written for LogitechC920 & LogitechC922 but in latest versions i'll try to generalize it.

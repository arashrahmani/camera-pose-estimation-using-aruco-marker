CF = cflags
LD = g++
TARGETS = all clean
CFLAGS = -cflags
PKGCONF = pkg-config
LIBS = -libs opencv
OPENCVCONTRIB = -lm -lopencv_core -lopencv_imgproc -lopencv_highgui -lopencv_ml -lopencv_video -lopencv_features2d -lopencv_calib3d -lopencv_objdetect -lopencv_stitching -lopencv_aruco
.PHONY: $(TARGETS)
all: ArucoPosEstimation.o squareMat.o

ArucoPosEstimation: ArucoPosEstimation.o squareMat.o
	$(LD) ArucoPosEstimation.o squareMat.o -o ArucoPosEstimation `$(PKGCONF) -$(CFLAGS) -$(LIBS)` $(OPENCVCONTRIB)
squareMat.o: squareMat.cpp squareMat.h
	$(LD) -c squareMat.cpp 
ArucoPosEstimation.o: ArucoPosEstimation.cpp ArucoPosEstimation.h cameraConfig.h
	$(LD) -c ArucoPosEstimation.cpp
clean:
	rm -rf *o ArucoPosEstimation

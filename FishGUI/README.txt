Need to add the following to Makefile after running qmake on the line LIBS:

-L/usr/local/lib -lopencv_core -lopencv_imgproc -lopencv_highgui -lopencv_videoio -Wl,-rpath=/usr/local/lib





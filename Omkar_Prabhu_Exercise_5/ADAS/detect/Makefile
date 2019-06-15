all: run

run: detect
	./detect --cascade="stop_haar.xml" stop9.png

detect: detect.cpp
	g++ detect.cpp -lopencv_core -lopencv_imgproc -lopencv_highgui -lopencv_objdetect -o detect

clean:
	rm detect

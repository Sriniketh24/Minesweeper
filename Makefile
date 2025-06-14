project2.out: src/main.cpp
	g++ -std=c++11 -o project2.out src/main.cpp

tasks: project2.out
	./project2.out output/part11.tga input/circles.tga multiply input/layer1.tga
	./project2.out output/part12.tga input/layer1.tga flip
	./project2.out output/part13.tga input/layer1.tga subtract input/layer2.tga

run: project2.out
	./project2.out

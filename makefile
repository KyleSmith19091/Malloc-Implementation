run:
	clang++ -std=c++11 -g ./src/*.cpp -I ./ -o ./build/main
	./build/main

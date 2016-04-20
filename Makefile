all: maxcut

maxcut: maxcut.cpp
	g++ -o maxcut -O3 maxcut.cpp

run: maxcut
	./maxcut data.in result.out

clean:
	rm maxcut

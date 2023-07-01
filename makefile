CC=g++

main:main.cpp
	$(CC) main.cpp -lpthread -o main
clean:
	rm -f main

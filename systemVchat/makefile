.PHONY:all clean
all:client server
client:client.cc comm.hpp
	g++ -o client client.cc -std=c++11
server:server.cc comm.hpp
	g++ -o server server.cc -std=c++11
clean:
	rm -f server client
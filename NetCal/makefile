.PHONY:all
all:tcpserver_netcal client_netcal

tcpserver_netcal :main.cc
	g++ -o $@ $^ -std=c++17 -ljsoncpp

client_netcal:TcpClient.cc
	g++ -o $@ $^ -std=c++17 -ljsoncpp


.PHONY:clean
clean:
	rm -f tcpserver_netcal client_netcal
.PHONY:all
all:Tcpclient Tcpserver	

Tcpclient:TcpClient.cc
	g++ -o Tcpclient TcpClient.cc -std=c++17 -lpthread
Tcpserver:TcpServer.cc
	g++ -o Tcpserver TcpServer.cc -std=c++17

.PHONY:clean
clean:
	rm -rf Tcpclient Tcpserver
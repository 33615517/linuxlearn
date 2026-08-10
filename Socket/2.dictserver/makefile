.PHONY:all
all:Udpclient Udpserver	

Udpclient:UdpClient.cc
	g++ -o Udpclient UdpClient.cc -std=c++17 -static
Udpserver:UdpServer.cc
	g++ -o Udpserver UdpServer.cc -std=c++17

.PHONY:clean
clean:
	rm -rf Udpclient Udpserver
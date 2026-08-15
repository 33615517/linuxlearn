.PHONY:all
all:tcpserver_netcal client_netcal

tcpserver_netcal :main.cc
	g++ -o $@ $^ -std=c++17 -ljsoncpp

client_netcal:TcpClient.cc
	g++ -o $@ $^ -std=c++17 -ljsoncpp

.PHONY:output
output:
	@mkdir output
	@mkdir -p output/bin
	@mkdir -p output/conf
	@mkdir -p output/log
	@cp tcpserver_netcal output/bin
	@cp test.conf output/conf
	@cp client_netcal output/bin
	@cp install.sh output/
	@cp uninstall.sh output/
	@tar czf output.tgz output

.PHONY:clean
clean:
	rm -rf tcpserver_netcal client_netcal output output.tgz
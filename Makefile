.PHONY:all
all:tcp_server tcp_client

LDFLAG =# -D SelfDefine=1

tcp_client:TcpClient_Main.cc
	g++ $^ -o $@ $(LDFLAG) -std=c++14 -l jsoncpp

tcp_server:TcpServer_Main.cc
	g++ $^ -o $@ $(LDFLAG) -std=c++14 -l pthread -l jsoncpp
.PHONY:clean
clean:
	rm -f tcp_server tcp_client

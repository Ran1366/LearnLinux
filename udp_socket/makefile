all: server client
server:server.cpp
	g++ $< -o $@ 
client:client.cpp 
	g++ $< -o $@
.PHONY:clean
clean:
	rm -f server 
	rm -f client

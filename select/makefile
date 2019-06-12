all:select client 
select:select.cpp
	g++ $< -o $@ 
client:client.cpp 
	g++ $< -o $@ 
.PHONY:clean
clean:
	rm -f select
	rm -f client

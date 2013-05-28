all:server client
server:server.c mycommon.h
	cc -o $@ $<
client:client.c mycommon.h
	cc -o $@ $<
clean:
	rm -rf server
	rm -rf client

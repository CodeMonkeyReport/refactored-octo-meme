all: assign4-0 assign4-thread assign4-part3 assign5-server

assign4-0: a4Driver.c nqueens_library.c
	cc -g -o assign4-0.exe a4Driver.c nqueens_library.c

assign4-thread: a4ThreadDriver.c nqueens_library.c
	cc -g -pthread -o assign4.exe a4ThreadDriver.c nqueens_library.c

assign4-part3: a4Part3Driver.c nqueens_library.c
	cc -g -o assign4-3.exe a4Part3Driver.c nqueens_library.c

assign5-server: n_queens_server.c nqueens_library.c
	cc -g -o assign5-server.exe n_queens_server.c nqueens_library.c restart.c uici.c uiciname.c

assign5-server: n_queens_client.c nqueens_library.c
	cc -g -o assign5-client.exe n_queens_client.c nqueens_library.c restart.c uici.c uiciname.c


clean:
	rm -f assign4-0.exe assign4.exe assign4-3.exe *.o assign5-server.exe

run:
	assign4 8

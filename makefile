all: assign4-0 assign4-thread assign4-part3 

assign4-0: a4Driver.c nqueens_library.c
	cc -g -o assign4-0.exe a4Driver.c nqueens_library.c

assign4-thread: a4ThreadDriver.c nqueens_library.c
	cc -g -pthread -o assign4.exe a4ThreadDriver.c nqueens_library.c

assign4-part3: a4Part3Driver.c nqueens_library.c
	cc -g -o assign4-3.exe a4Part3Driver.c nqueens_library.c

clean:
	rm -f assign4-0.exe assign4.exe assign4-3.exe *.o

run:
	assign4 8

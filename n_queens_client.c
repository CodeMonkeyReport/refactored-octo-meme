#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "restart.h"
#include "uici.h"
#define BUFFSIZE 10

int main(int argc, char *argv[])
{
    int bytescopied;
    int communfd;
    u_port_t portnumber;
    char char_n[BUFFSIZE];
    char char_first[BUFFSIZE];
    int n;
    int i;

    if (argc != 4)
    {
        fprintf(stderr, "Usage: %s n host port\n", argv[0]);
        return 1;
    }
    portnumber = (u_port_t)atoi(argv[3]);
    n = atoi(argv[1]);
    sprintf(char_n, "%d\n", n);
    if ((communfd = u_connect(portnumber, argv[2])) == -1)
    {
        perror("Failed to make connection");
        return 1;
    }
    fprintf(stderr, "[%ld]:connected %s\n", (long)getpid(), argv[1]);

    for (i = 0; i < n; i++)
    {
        sprintf(char_first, "%d\n", i);
        r_write(communfd, char_n, sizeof char_n);
        r_write(communfd, char_first, sizeof char_first);


    }
    fprintf(stderr, "[%ld]:connected %s\n", (long)getpid(), argv[1]);
    bytescopied = copyfile(STDIN_FILENO, communfd);
    fprintf(stderr, "[%ld]:sent %d bytes\n", (long)getpid(), bytescopied);
    fprintf(stderr, "[%ld]:sent %d bytes\n", (long)getpid(), bytescopied);
    return 0;
}

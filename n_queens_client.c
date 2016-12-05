#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdint.h>
#include "restart.h"
#include "uici.h"
#define BUFFSIZE 10

int main(int argc, char *argv[])
{
    int bytescopied;

    u_port_t portnumber;
    char char_n[BUFFSIZE];
    char char_first[BUFFSIZE];
    char results[BUFFSIZE];
    uint32_t result_nl;
    int result;
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
    int communfd[n];

    fprintf(stderr, "looping %d times\n", n);
    for (i = 0; i < n; i++)
    {
        // Write
        if ((communfd[i] = u_connect(portnumber, argv[2])) == -1)
        {
            perror("Failed to make connection");
            return 1;
        }

        sprintf(char_first, "%d\n", i);
        r_write(communfd[i], char_n, sizeof char_n);
        r_write(communfd[i], char_first, sizeof char_first);
    }

    for (i = 0; i < n; i++)
    {
        // Read
        r_read(communfd[i], &result_nl, sizeof(uint32_t));
        result = (int)ntohl(result_nl);
        fprintf(stderr, "%d", i+1);
        fprintf(stderr, " Has: %d solutions\n", result);
    }
    return 0;
}

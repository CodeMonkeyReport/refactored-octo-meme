/**********************************************************************
  a4Driver.c by Michael Geyer 
Purpose:
This program uses threads to solve the n-queens problem
Command Parameters:
n   The number of queens to calculate the algorithm for

Commands:
N/A
Results:
If the print flag is set each step in calculating the algorithm
will be printed to stdout along with the final result.
Returns:
0       Normal
3       Processing Error
Notes:

**********************************************************************/
#define PRINT_FLAG 1 
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "a4Driver.h"
#include "nqueens_library.h"

int main(int argc, char *argv[])
{
    int result = 0;
    int i;
    int n = 8;
    int print_flag = 1;

    printf("CS 3733 Assignment 4 by Michael Geyer\n");

    // Error checking
    if (argc != 2)
    {
        // Error wrong command arguments
        exitError(ERR_USAGE, "Missing parameter (n)"); 
    }
    n = atoi(argv[1]);


    if (n <= 0 || n >= 9999)
    {
        exitError(ERR_INVALID_INPUT, "n must be between 0 and 9999");
    }
    
    result += generate_n_queens_serial(n, PRINT_FLAG);
    printf("%d\n", result);    
    return 0;
}

/******************** exitError *****************************
  void exitError(char *pszMessage, char *pszDiagnosticInfo)
Purpose:
Prints an error message and diagnostic to stderr.  Exits with
ERROR_PROCESSING.
Parameters:
I char *pszMessage              error message to print
I char *pszDiagnosticInfo       supplemental diagnostic information
Notes:
This routine causes the program to exit.
 **************************************************************************/
void exitError(const char *pszMessage, const char *pszDiagnosticInfo)
{
    fprintf(stderr, "Error: %s %s\n"
            , pszMessage
            , pszDiagnosticInfo);
    exit(ERROR_PROCESSING);
}



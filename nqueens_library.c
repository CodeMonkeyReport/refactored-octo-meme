#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "nqueens_library.h"
#include <pthread.h>

pthread_mutex_t count_mutex;

/*****************************************************************************
    int generate_n_queens_serial(int n, int print_flag)\
Purpose:
    This function generates the solution to the n queens problem
Parameters:
    I n             The number of queens to calculate the solution for
    I print_flag    Tells the program if it should print each step in 
                    the algorithm or simply print the final result.
Returns:
    int             The number of solutions generated by the function
Notes:
    
*****************************************************************************/
int generate_n_queens_serial(int n, int print_flag)
{
    int i;
    int iPermCount = 0;
    int testPermutation[n];
    char *permutationString = NULL;
   
    for (i = 0; i < n; i++)
    {
        testPermutation[i] = i;
    }

    permutationString = malloc((n*5) * sizeof(char));

    do 
    {
        if (check_diagonal(testPermutation, n))
        {
            if(print_flag)
            { 
                get_array_string(testPermutation, n, permutationString);

                pthread_mutex_lock(&count_mutex);
                printf("%s", permutationString);
                pthread_mutex_unlock(&count_mutex);

            }
            iPermCount++;
        }
    } while (!next_permutation(testPermutation, n));

    return iPermCount;
}

/*****************************************************************************
    int generate_n_queens_serial_one(int n, int print_flag, int first)
Purpose:
    This function generates some solutions to the n queens problem where
    the permutation set we are calculating the solution for starts with
    first
Parameters:
    I n             The number of queens to calculate the solution for
    I print_flag    Tells the program if it should print each step in 
                    the algorithm or simply print the final result.
    I first         The number all permutations calculated will start with
Returns:
    int             Representing the number of permutations that are solutions
Notes:
    
*****************************************************************************/
int generate_n_queens_serial_one(int n, int print_flag, int first)
{
    int i;
    int iPermCount = 0;
    int startPermutation[n];
    char *permutationString = NULL;
    
    startPermutation[0] = first;
    
    for (i = 1; i < n; i++)
    {
        if (i <= first)
            startPermutation[i] = i-1;
        else
            startPermutation[i] = i;
    }

    permutationString = malloc((n*5) * sizeof(char));

    do 
    {
        if (check_diagonal(startPermutation, n))
        {
            if(print_flag)
            { 
                get_array_string(startPermutation, n, permutationString);
                printf("%s", permutationString);
            }
            iPermCount++;
        }
    } while (!next_permutation(startPermutation, n) && startPermutation[0] == first);

    return iPermCount;
}

/*****************************************************************************
    void *queens_thread(void *infopointer)
Purpose:
    Serves as an interface for a POSIX thread to call generate_n_queens_serial_one
Parameters:
    I/O void *infopointer   pointer to a thread_info_t 
Notes:

*****************************************************************************/
void *queens_thread(void *infopointer)
{
    thread_info_t *info = (thread_info_t *)infopointer;

    info->result = generate_n_queens_serial_one(info->n, info->out_flag, info->first);
    
    pthread_mutex_lock(&count_mutex);
    printf("%d %d\n", info->first, info->result);
    pthread_mutex_unlock(&count_mutex);

    return NULL;
}

/*****************************************************************************
    void get_array_string(int *list, int n, char *s)
Purpose:
    Converts an array of integers into a printable string
Parameters:
    I int *list         The array of integers to pe put into the string
    I int n             The number of integers in the array
    O char *s           The resulting string
Notes:
    The resulting string contains a newline (\n) charater at the end and
    is terminated by a null byte (\0)
*****************************************************************************/
void get_array_string(int *list, int n, char *s)
{
    int i;
    int iPrintfCount = 0;
    for (i = 0; i < n; i++) 
    {
        iPrintfCount = sprintf(&(s[i*5]), "%4d ", list[i]);
        
        if (iPrintfCount != 5)       // If we didn't print exactly 5 characters
        {
            printf("ERROR printfCount\n");
            return;
        }
    }
    s[i*5-1] = '\n';
    s[i*5] = '\0';
}

/*****************************************************************************
    void next_permutation(int *list, int n)
Purpose:
    This function generates the next permutation in the list in place using
    lexicographic ordering
Parameters:
    I/O int *list           The list of integers to be re-aranged
    I   int n               The number of integers in the list
Notes:

*****************************************************************************/
int next_permutation(int *list, int n)
{
    int k, l, i;
    int start, end;
    int temp;

    if (n <= 1)
        return 1;
    // find LARGEST index such that list[k] < list[k+1]
    for (k = n-2; k >= 0; k--)
    {
        // Exit on first instance that meets condition
        if (list[k] < list[k+1])
            break;
        if (k == 0) // If we reach the end and the condition is false, this is the last permutation
            return 1;
    }
    
    // find LARGEST index l > k s.t. list[k] < list[l]
    for (l = n-1; l > k; l--)
    {
        // Exit on first instance that meets condition
        if (list[k] < list[l])
            break;
    }
    // swap the values at l & k
    temp = list[k];
    list[k] = list[l];
    list[l] = temp;

    start = k+1;
    end = n-1;
    // reverse the sequense AFTER element k
    while (start < end)
    {
        temp = list[start];
        list[start] = list[end];
        list[end] = temp;
        start++;
        end--;
    }
    return 0;
}

/*****************************************************************************
    int check_diagonal(int *list, int n)
Purpose:
    This function checks to see if the permutation given by the list satisfies
    the diagonal property of the n-queens problem.
Parameters:
    I/O int *list           The permutation to be checked
    I   int n               The number of integers in the list
Returns:
    TRUE    The permutation satisfies the condition
    FALSE   The permutation does not satisfy the condition
Notes:
    b1 and b2 represent the Y intercept if the queens diagonals are
    represented as a line of the form Y = X + b1 and Y = -x + b2
*****************************************************************************/
int check_diagonal(int *list, int n)
{
    int i;
    int b1Check[2*n - 1];
    int b2Check[2*n - 1];
    int b1, b2;
    // For each item in the list
    memset(b1Check, 0, sizeof b1Check);
    memset(b2Check, 0, sizeof b2Check);
    
    for (i = 0; i < n; i++)
    {
        // Compute b1 and b2
        b1 = i + list[i];
        b2 = i - list[i] + (n - 1);
        // If both intercepts are unique then the queens can not capture one another
        if (b1Check[b1] == 0 && b2Check[b2] == 0)
        {
            b1Check[b1] = 1;
            b2Check[b2] = 1;
        }
        else // If one of the intercepts already exist then the queen shares a diagonal with another
        {
            return FALSE;
        }
    }
    return TRUE;
}

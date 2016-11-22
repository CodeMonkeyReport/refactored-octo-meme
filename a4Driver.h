#define TRUE 1
#define FALSE 0

#define ERROR_PROCESSING 3      // Error processing exit value

#define ERR_INVALID_INPUT   "Number was not in expected range"
#define ERR_USAGE           "prog (n)"

void exitError(const char *pszMessage, const char *pszDiagnosticInfo);

#include "config.h"

#if MEGAQR_DEBUG == 1
// Local dev server test endpoint
    const char *url = "aHR0cDovLzE5Mi4xNjguMS4zNzo1MDAwL2FwaS90Lw";
    //const char *url = "http://192.168.1.37:5000/api/t/";
#else
    const char *ulr = "http://192.168.1.37:5000/api/s/";
#endif

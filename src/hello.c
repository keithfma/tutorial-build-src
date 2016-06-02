#include <stdio.h>
main()
{

#ifdef LOUD 
    (void) printf("HELLO, WORLD!\n");
#else
    (void) printf("Hello World\n");
#endif

    return (0);
}


#include <string.h>
#include "lexique.h"

unsigned long lexique_str_hash(void * v)
{
    unsigned long hash = 5381;
    int c;

    unsigned char * str = (unsigned char *)v;
    
    while( (c = *str++) )
	hash = ((hash << 5) + hash) + c;
    
    return hash;
}

int lexique_str_equals(void * k1, void * k2)
{
    return !strcmp( (char*)k1, (char*)k2 );
}

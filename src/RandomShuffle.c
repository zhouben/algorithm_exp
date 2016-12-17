#include <stdlib.h>
#include <time.h>

void RandomSeedInit()
{
   __time32_t t;
    _time32( &t );
    //srand( (unsigned int)t );
	srand( 5 );
 }

void RandomShuffle( int * Array, int Size )
{
    int i, j;
    int temp;
    for( i = 0; i < ( Size - 1 ); i++)
    {
        j = rand() % ( Size - i );
        temp = Array[ i ];
        Array[ i ] = Array[ j ];
        Array[ j ] = temp;
    }
}

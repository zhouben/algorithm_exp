/*
题(20)－最长公共子串
题目：如果字符串一的所有字符按其在字符串中的顺序出现在另外一个字符串二中，则字符串一称之为字
符串二的子串。注意，并不要求子串（字符串一）的字符必须连续出现在字符串二中。请编写一个函数，
输入两个字符串，求它们的最长公共子串，并打印出最长公共子串。
例如：输入两个字符串BDCABA 和ABCBDAB，字符串BCBA 和BDAB 都是是它们的最长公共子串，则
输出它们的长度4，并打印任意一个子串。
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

enum DIR_TYPE{
    INIT = 0,
    LEFT = 1,
    UP   = 2,
    LEFTUP = 3
};

char DirToChar( enum DIR_TYPE Type )
{
    switch( Type )
    {
    case INIT:
        return '*';
    case LEFT:
        return '-';
    case UP:
        return '|';
    case LEFTUP:
        return '\\';
    default:
        return '?';
    }
}
void DisplayLCS(char* StrX, char* StrY, int m, int n, int* matrix_length, enum DIR_TYPE* matrix_dir )
{
	int i;
	int j;
    char* lcs;
    int index;
	int pos;
    i = m - 1;
    j = n - 1;

    pos = i * n + j;
    printf("LCS length: %d\n", matrix_length[ pos ] );
    index = matrix_length[ pos ];
    lcs = malloc( index + 1 );
    lcs[ index ] = '\0';
    index--;
    do{
        pos = i * n + j;
        switch ( matrix_dir[ pos ] )
        {
        case LEFTUP:
            lcs[ index ] = StrX[ i ];
			printf("(%2d, %2d) ", i, j );
            i--;
            j--;
            index--;
            break;
        case UP:
            i--;
            break;
        case LEFT:
            j--;
            break;
		case INIT:
			printf("(%2d, %2d) ", i, j );
			i--;
			j--;
			break;
        default:
            printf("ERROR i,y,dir = %d %d %c\n", i, j, DirToChar( matrix_dir[ pos ] ) );
            break;
        }
    }while( ( i >= 0 ) && ( j >= 0 ) );
	printf("\n");
    printf("LCS      : %s\n", lcs );
	free( lcs );
}

void SearchLCS( char* StrX, char* StrY, char* LCS )
{
    int i;
    int j;
    int m;
    int n;
    int pos;
    int* matrix_length;
    enum DIR_TYPE* matrix_dir;

    m = (int)strlen( StrX );
    n = (int)strlen( StrY );

    matrix_length = (int*)malloc( m * n * sizeof( int ) );
    matrix_dir    = (enum DIR_TYPE*)malloc( m * n * sizeof( enum DIR_TYPE ) );
    for( i = 0; i < m; i++)
    {
        for( j = 0; j < n; j++)
        {
            pos = i * n + j;
            if ( ( i == 0 ) || ( j == 0 ) )
            {
                if ( StrX[ i ] == StrY[ j ] )
                {
                    matrix_length[ pos ] = 1;
                    matrix_dir[ pos ] = LEFTUP;
				}else{
					if ( ( i == 0 ) && ( j == 0 ) )
					{
						matrix_length[ pos ] = 0;
						matrix_dir[ pos ] = INIT;
					}else{
						if ( i == 0 )
						{
							matrix_length[ pos ] = matrix_length[ pos - 1 ];
							matrix_dir[ pos ] = LEFT;
						}else{
							matrix_length[ pos ] = matrix_length[ pos - n ];
							matrix_dir[ pos ] = UP;
						}
					}
                }
            }else if ( StrX[ i ] == StrY[ j ] )
            {
                matrix_length[ pos ] = matrix_length[ pos - n - 1 ] + 1;
                matrix_dir[ pos ] = LEFTUP;
            }else if ( matrix_length[ pos - n ] >= matrix_length[ pos - 1 ] )
            {
                matrix_length[ pos ] = matrix_length[ pos - n ];
                matrix_dir[ pos ] = UP;
			}else
            {
                matrix_length[ pos ] = matrix_length[ pos - 1 ];
                matrix_dir[ pos ] = LEFT;
            }
        }
    }
	printf("     ");
	for( i = 0; i < n; i++)
	{
		printf("%5d ", i );
		
	}
	printf("\n");
	printf("     ");
	for( i = 0; i < n; i++)
	{
		printf("%5c ", StrY[ i ] );
	}
	printf("\n");

    for( i = 0; i < m; i++)
    {
		printf("%2d %c ", i, StrX[ i ] );
        for( j = 0; j < n; j++)
        {
            printf( "%2d(%c) ", matrix_length[ i * n + j ], DirToChar( matrix_dir[ i * n + j ] ) );
        }
		printf("\n");
    }
	DisplayLCS( StrX, StrY, m, n, matrix_length, matrix_dir );

    free( matrix_dir );
    free( matrix_length );
}

void TestLCS()
{
#define MAX_LEN 32
    //char strX[] = "BDCABA";
    //char strY[] = "ABCBDAB";
	char strX[] = "ABCDEFGABC";
	char strY[] = "BDFGAABBDC";
    char strLCS[ MAX_LEN ] = { 0 };

    SearchLCS( strX, strY, strLCS );

}


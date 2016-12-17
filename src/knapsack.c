/***************************************************************\

Knapsack problem
背包问题

使用 m[W] 数组记录 当前 物品之前的每一个 Weight 的最优解，即 m[ i-1, W]
然后比较 m[ i-1, W], m[ i-1, j-W] 的大小，将较大值更新到 m[i, W]
算法复杂度是 O(nW)。

2014-04-29

\***************************************************************/
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <tchar.h>
#include <memory.h>
#include "RandomShuffle.h"


#define ITEM_NUM 10
#define KNAPSACK_CAPACITY 30
typedef struct _weight_trace
{
    int price;
    unsigned char Items[ ITEM_NUM ];
}WEIGHT_TRACE;

static void DisplayTrace( WEIGHT_TRACE* Wt, int* Weight, int* Price, int Size );

int KnapsackProblem( int* Weight, int* Price, int Num, int KnapsackCapacity, WEIGHT_TRACE* WeightTrace )
{
    int currWeight;
    int prevPrice;
    int currPrice;
    int i;
	int n;
    int j;
	int maxPrice;
    WEIGHT_TRACE *weightTable;


    prevPrice = 0;
    currPrice = 0;
    weightTable = (WEIGHT_TRACE* )malloc( sizeof( WEIGHT_TRACE ) * ( KnapsackCapacity + 1 ));
	memset( weightTable, 0, sizeof( WEIGHT_TRACE ) * ( KnapsackCapacity + 1 ) );

    for( i = 0; i < Num; i++)
    {
        if ( Weight[ i ] > KNAPSACK_CAPACITY )
        {
            continue;
        }

//        for( n = ( KNAPSACK_CAPACITY - Weight[ i ] ); n > 0; n-- )
		currWeight = Weight[ i ];
		for( j = KnapsackCapacity; j >= currWeight; j-- )
		{
			prevPrice = weightTable[ j ].price;
			currPrice = Price[ i ] + weightTable[ j - currWeight ].price;
			if ( currPrice > prevPrice )
			{
				weightTable[ j ].price = currPrice;
				for( n = 0; n < i; n++)
				{
					weightTable[ j ].Items[ n ] = weightTable[ j - currWeight ].Items[ n ];
				}
				weightTable[ j ].Items[ i ] = 1;

			}
		}
        
    }
    maxPrice = 0;
	j = 0;
    for( i = KnapsackCapacity; i > 0; i-- )
    {
        if ( weightTable[ i ].price > maxPrice )
        {
			maxPrice = weightTable[ i ].price;
			j = i;
        }
    }
	WeightTrace->price = weightTable[ j ].price;
	memcpy( WeightTrace->Items, weightTable[ j ].Items, sizeof( unsigned char) * Num );

    free( weightTable );
    return i;
}

int KnapsackPermunation( int* Weight, int* Price, int Num, int KnapsackCapacity, WEIGHT_TRACE *WeightTrace )
{
    long long bitmap;
    long long i;
	int n;
    int currWeight;
    int currPrice;
    int maxPrice;
    int maxWeight;
    int overflow;
    WEIGHT_TRACE wt;

    if ( Num > 64 )
    {
        WeightTrace->price = 0;
        return 0;
    }
    bitmap = ( Num == 64 ) ? 0xFFFFFFFFFFFFFFFF : ( ( 1 << Num ) - 1 );
    wt.price = 0;
    memset( wt.Items, 0, sizeof( unsigned char ) * Num );
    maxPrice = 0;
    maxWeight = 0;
    for( i = 1; i < bitmap; i++)
    {
        currWeight = 0;
        currPrice  = 0;
        overflow = 0;
        for( n = 0; n < Num; n++)
        {
            if ( ( i >> n ) & 1 ) 
            {
                if ( currWeight + Weight[ n ] <= KnapsackCapacity )
                {
                    wt.Items[ n ] = 1;
                    currWeight += Weight[ n ];
                    currPrice += Price[ n ];
                }else{
                    overflow = 1;
                    break;
                }
            }else{
                wt.Items[ n ] = 0;
            }
        }
        if ( ( overflow == 0 ) && ( currPrice > maxPrice ) )
        {
            maxPrice = currPrice;
            maxWeight = currWeight;
            wt.price = currPrice;
            memcpy( WeightTrace, &wt, sizeof( WEIGHT_TRACE ) );
        }
    }
    return maxWeight;
}
void DisplayTrace( WEIGHT_TRACE* Wt, int* Weight, int* Price, int Size )
{
	int i;
    for( i = 0; i < Size; i++)
    {
        if ( Wt->Items[ i ] == 1 )
        {
        _tprintf( _T("%2d "), Weight[ i ] );
        }else{
        _tprintf( _T("   ") );
        }
    }
	_tprintf( _T("\n") );
    for( i = 0; i < Size; i++)
    {
        if ( Wt->Items[ i ] == 1 )
        {
        _tprintf( _T("%2d "), Price[ i ] );
        }else{
        _tprintf( _T("   ") );
        }
    }
	_tprintf( _T("\n") );

}

void TestKnapsackProblem()
{
    int i;
	int loop;
#define RANDOM_SEQUENCE

#ifdef RANDOM_SEQUENCE
	int maxLoop = 1000000;
    int weight[ ITEM_NUM ] = { 0 };
    int price[ ITEM_NUM ] = { 0 };
#else
	int maxLoop = 1;
    int weight[ ITEM_NUM ] = { 1,  9,  3,  6,  5,  4,  7,  2,  8, 10 };
    int price[ ITEM_NUM ] = { 2,  8,  9,  4,  7,  3,  1,  6,  5, 10 };
#endif
    WEIGHT_TRACE wtA;
	WEIGHT_TRACE wtB;
    //WEIGHT_TRACE wtB;
    int maxWeightA;
	int maxWeightB;
	unsigned char bErrorFlag;

#ifdef RANDOM_SEQUENCE
    for( i = 0; i < ITEM_NUM; i++)
    {
        weight[ i ] = i+1;
        price[ i ] = i+1;
    }
    RandomSeedInit();
#endif


	for( loop = 0; loop < maxLoop; loop++ )
	{
#ifdef RANDOM_SEQUENCE
		RandomShuffle( weight, ITEM_NUM );
		RandomShuffle( price, ITEM_NUM );
#endif
		if ( ( loop % 1024 ) == 0 )
		{
			_tprintf( _T("Loop %5d is completed.\n"), loop );
		}

		// Classic solution.
		maxWeightA = KnapsackProblem( weight, price, ITEM_NUM, KNAPSACK_CAPACITY, &wtA );
		// display 

		// permutate solution.
		maxWeightB = KnapsackPermunation( weight, price, ITEM_NUM, KNAPSACK_CAPACITY, &wtB );


		bErrorFlag = 0;

		if ( wtA.price != wtB.price )
		{
			bErrorFlag = 1;
		}else{
			for( i = 0; i < ITEM_NUM; i++)
			{
				if ( wtA.Items[ i ] != wtB.Items[ i ] )
				{
					bErrorFlag = 1;
					break;
				}
			}
		}
		if ( bErrorFlag == 1 )
		{
			_tprintf( _T("Loop %d ERROE!\n"), loop  );
			_tprintf( _T("weight sequence: ") );
			for( i = 0; i < ITEM_NUM; i++)
			{
				_tprintf( _T("%2d "), weight[ i ] );
			}
			_tprintf( _T("\n") );
			_tprintf( _T("price  sequence: ") );
			for( i = 0; i < ITEM_NUM; i++)
			{
				_tprintf( _T("%2d "), price[ i ] );
			}
			_tprintf( _T("\n") );

			_tprintf( _T("The result of classic algorithm: max weight %d, max price %d\n"), maxWeightA, wtA.price );
			DisplayTrace( &wtA, weight, price, ITEM_NUM );
			_tprintf( _T("The result of permute algorithm: max weight %d, max price %d\n"), maxWeightB, wtB.price );
			DisplayTrace( &wtB, weight, price, ITEM_NUM );
			break;
		}
	}

}




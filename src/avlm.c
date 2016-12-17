/***************************************************************\
*
*  1. random shuffle
*  2. quick sort
*  3. AVL tree, insertion, deletion and search
*
\***************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <tchar.h>
#include <assert.h>

#include "RandomShuffle.h"

#define LH  1
#define EH  0
#define RH  -1

#define TRUE 1
#define FALSE 0
#define NODE_NUM 31

#define RANDOM_SEQUENCE
#define MAX_LOOP 100000000


typedef struct _avlnode{
    int value;
    int bf;
    struct _avlnode* lc;
    struct _avlnode* rc;
}AVL_NODE, *PAVL_NODE;

static void VisitNode( PAVL_NODE Node );
static void InOrderTraverse( PAVL_NODE Node );
static void PreOrderTraverse( PAVL_NODE Node );
void LeftBalance( PAVL_NODE* Node );
void RightBalance( PAVL_NODE* Node );

unsigned char CheckAVL( PAVL_NODE Node );
int _get_avl_deepth_recursive_method( PAVL_NODE Node );

void QuickSortInPlace( int *Array, int Size );
void DisplayArray( int* Array, int Size );
void DestroyAVL( PAVL_NODE Node );

unsigned char InsertNodeIntoAVL( PAVL_NODE *Node, int Value, unsigned char* Taller );
unsigned char DeleteNodeFromAVL( PAVL_NODE* Node, int Value, unsigned char* Shorter );

void RebalanceForLeftShorter( PAVL_NODE *Node, unsigned char *Shorter );
void RebalanceForRightShorter(PAVL_NODE *Node, unsigned char *Shorter );

static int gIndex;
static int gDataForAVL[ NODE_NUM ];


static void VisitNode( PAVL_NODE Node )
{
    assert( gIndex < NODE_NUM );
    gDataForAVL[ gIndex ] = Node->value;
    gIndex++;
}

void InOrderTraverse( PAVL_NODE Node )
{
    if ( Node != NULL )
    {
        InOrderTraverse( Node->lc );
        VisitNode( Node );
        InOrderTraverse( Node->rc );
    }
}

void PreOrderTraverse( PAVL_NODE Node )
{
    if ( Node != NULL )
    {
        VisitNode( Node );
        InOrderTraverse( Node->lc );
        InOrderTraverse( Node->rc );
    }
}
void LeftBalance( PAVL_NODE* Node )
{
    PAVL_NODE pNode;
    pNode = *Node;
    // include LL, LR;
    switch( pNode->lc->bf )
    {
    case LH:
        // LL
        pNode->lc->bf = EH;
        pNode->bf     = EH;
        *Node = pNode->lc;
        pNode->lc = (*Node)->rc;
        (*Node)->rc = pNode;


        break;
    case RH:
        switch( pNode->lc->rc->bf )
        {
        case LH:
            pNode->lc->bf = EH;
            pNode->bf     = RH;
            break;
        case RH:
            pNode->lc->bf = LH;
            pNode->bf     = EH;
            break;
        case EH:
            pNode->lc->bf = EH;
            pNode->bf     = EH;
            break;
        }
        *Node = pNode->lc->rc;
        pNode->lc->rc = (*Node)->lc;
        (*Node)->lc = pNode->lc;
        pNode->lc = (*Node)->rc;
        (*Node)->rc = pNode;
        (*Node)->bf = EH;
        break;
    case EH:
        // Only in the case of deletion.
        pNode->lc->bf = RH;
        pNode->bf = LH;
        (*Node) = pNode->lc;
        pNode->lc = (*Node)->rc;
        (*Node)->rc = pNode;

        //assert( FALSE );
        break;
    }

}
void RightBalance( PAVL_NODE* Node )
{
    PAVL_NODE pNode;
    //PAVL_NODE temp;
    pNode = *Node;
    // include LL, LR;
    switch( pNode->rc->bf )
    {
    case RH:
        // RR
        pNode->bf = EH;
        pNode->rc->bf = EH;
        *Node = pNode->rc;
        pNode->rc = (*Node)->lc;
        (*Node)->lc = pNode;
        break;
    case LH:
        switch( pNode->rc->lc->bf )
        {
        case RH:
            pNode->rc->bf = EH;
            pNode->bf     = LH;
            break;
        case LH:
            pNode->rc->bf = RH;
            pNode->bf     = EH;
            break;
        case EH:
            pNode->rc->bf = EH;
            pNode->bf     = EH;
            break;
        }
        *Node = pNode->rc->lc;
        pNode->rc->lc = (*Node)->rc;
        (*Node)->rc = pNode->rc;
        pNode->rc = (*Node)->lc;
        (*Node)->lc = pNode;
        (*Node)->bf = EH;
        break;
    case EH:
        //assert( FALSE );
        pNode->bf = RH;
        pNode->rc->bf = LH;
        *Node = pNode->rc;
        pNode->rc = (*Node)->lc;
        (*Node)->lc = pNode;
        break;
    }

}

void RightBalanceDel( PAVL_NODE* Node, unsigned char *Shorter )
{
    PAVL_NODE pNode;
    //PAVL_NODE temp;
    pNode = *Node;
    // include LL, LR;
    switch( pNode->rc->bf )
    {
    case RH:
        // RR
        pNode->bf = EH;
        pNode->rc->bf = EH;
        *Node = pNode->rc;
        pNode->rc = (*Node)->lc;
        (*Node)->lc = pNode;
		*Shorter = TRUE;
        break;
    case LH:
        switch( pNode->rc->lc->bf )
        {
        case RH:
            pNode->rc->bf = EH;
            pNode->bf     = LH;
            break;
        case LH:
            pNode->rc->bf = RH;
            pNode->bf     = EH;
            break;
        case EH:
            pNode->rc->bf = EH;
            pNode->bf     = EH;
            break;
        }
        *Node = pNode->rc->lc;
        pNode->rc->lc = (*Node)->rc;
        (*Node)->rc = pNode->rc;
        pNode->rc = (*Node)->lc;
        (*Node)->lc = pNode;
        (*Node)->bf = EH;
        break;
    case EH:
        //assert( FALSE );
        pNode->bf = RH;
        pNode->rc->bf = LH;
        *Node = pNode->rc;
        pNode->rc = (*Node)->lc;
        (*Node)->lc = pNode;
        break;
    }

}


unsigned char InsertNodeIntoAVL( PAVL_NODE *Node, int Value, unsigned char* Taller )
{
    unsigned char ret;
    PAVL_NODE pNode;
    unsigned char taller;

    pNode = *Node;

    if ( pNode == NULL )
    {
        pNode = (PAVL_NODE)malloc( sizeof( AVL_NODE ) );
        pNode->value = Value;
        pNode->lc = NULL;
        pNode->rc = NULL;
        pNode->bf = EH;
        *Taller = TRUE;
        *Node = pNode;
        return TRUE;
    }else{
        if ( Value == pNode->value )
        {
            *Taller = FALSE;
            return FALSE;
        }
        if ( Value < pNode->value )
        {
            ret = InsertNodeIntoAVL( &(pNode->lc), Value, &taller );
            if ( ret == FALSE )
            {
                return FALSE;
            }
            if ( taller == TRUE )
            {
                switch( pNode->bf )
                {
                case EH:
                    *Taller = TRUE;
                    pNode->bf = LH;
                    break;
                case LH:
                    LeftBalance( Node );
                    *Taller = FALSE;
                    break;
                case RH:
                    *Taller = FALSE;
                    pNode->bf = EH;
                    break;
                }
            }
        }else{
            ret = InsertNodeIntoAVL( &(pNode->rc), Value, &taller );
            if ( ret == FALSE )
            {
                return FALSE;
            }
            if ( taller == TRUE )
            {
                switch( pNode->bf )
                {
                case EH:
                    *Taller = TRUE;
                    pNode->bf = RH;
                    break;
                case LH:
                    *Taller = FALSE;
                    pNode->bf = EH;
                    break;
                case RH:
                    RightBalance( Node );
                    *Taller = FALSE;
                    break;
                }
            }
        }
        return TRUE;
    }
}
void RebalanceForLeftShorter( PAVL_NODE *Node, unsigned char *Shorter )
{
	PAVL_NODE pNode;

	pNode = *Node;
	switch( pNode->bf )
	{
	case EH:
		pNode->bf = RH;
		*Shorter = FALSE;
		break;
	case LH:
		pNode->bf = EH;
		*Shorter = TRUE;
		break;
	case RH:
		if ( pNode->rc->bf == EH )
		{
			*Shorter = FALSE;
		}else{
			*Shorter = TRUE;
		}
		RightBalance ( Node );
		break;
	}
}
void RebalanceForRightShorter(PAVL_NODE *Node, unsigned char *Shorter )
{
	PAVL_NODE pNode;

	pNode = *Node;
	switch( pNode->bf )
	{
	case EH:
		pNode->bf = LH;
		*Shorter = FALSE;
		break;
	case RH:
		pNode->bf = EH;
		*Shorter = TRUE;
		break;
	case LH:
		if ( pNode->lc->bf == EH )
		{
			*Shorter = FALSE;
		}else{
			*Shorter = TRUE;
		}
		LeftBalance( Node );
		break;

	}

}
PAVL_NODE DeleteLeftLeaf( PAVL_NODE* Node, unsigned char* Shorter )
{
    PAVL_NODE pNode;
    PAVL_NODE ret;
    unsigned char shorter;
    pNode = *Node;
    if ( pNode->lc == NULL )
    {
        *Node = pNode->rc;
        *Shorter = TRUE;
        ret = pNode;
    }else{
        ret = DeleteLeftLeaf( &( pNode->lc ), &shorter );
		
        if ( shorter )
        {
			RebalanceForLeftShorter( Node, Shorter );
        }else{
            *Shorter = FALSE;
        }
    }
    return ret;
}
PAVL_NODE DeleteRightLeaf( PAVL_NODE* Node, unsigned char* Shorter )
{
    PAVL_NODE pNode;
    PAVL_NODE ret;
    unsigned char shorter;
    pNode = *Node;
    if ( pNode->rc == NULL )
    {
        *Node = pNode->lc;
        *Shorter = TRUE;
        return pNode;
    }else{
        ret = DeleteRightLeaf( &pNode->rc, &shorter );
        if ( shorter )
        {
			RebalanceForRightShorter( Node, Shorter );
        }else{
            *Shorter = FALSE;
        }
        return ret;
    }
}
unsigned char DeleteNodeFromAVL( PAVL_NODE* Node, int Value, unsigned char* Shorter )
{
    unsigned char ret;
    PAVL_NODE pNode;
    PAVL_NODE pLeaf;
    unsigned char shorter;
    pNode = *Node;
    if ( pNode == NULL )
    {
        *Shorter = FALSE;
        return FALSE;
    }
    if ( Value < pNode->value )
    {
        ret = DeleteNodeFromAVL( &( pNode->lc ), Value, &shorter );
        if ( ret && shorter )
        {
			RebalanceForLeftShorter( Node, Shorter );
        }else{
            *Shorter = FALSE;
        }
    }else if ( Value > pNode->value ){
        ret = DeleteNodeFromAVL( &( pNode->rc ), Value, &shorter );
        if ( ret && shorter )
        {
			RebalanceForRightShorter( Node, Shorter );
        }else{
            *Shorter = FALSE;
        }
    }else{
        pLeaf = NULL;
        shorter = TRUE;
		/*
		We will delete the node which is in the taller child tree..
		So after the calling of DeletexxxxLeaf, the current tree is still balance.
		Don't need to re-balance the current tree.
		*/
        switch( pNode->bf )
        {
        case LH:
			pLeaf = DeleteRightLeaf( &(pNode->lc), &shorter );
			if ( shorter )
			{
				pLeaf->bf = EH;
				*Shorter = TRUE;
			}else{
				pLeaf->bf = LH;
				*Shorter = FALSE;
			}

            break;
        case RH:
                pLeaf = DeleteLeftLeaf( &(pNode->rc), &shorter );
                if ( shorter )
                {
                    pLeaf->bf = EH;
					*Shorter = TRUE;
                }else{
                    pLeaf->bf = RH;
					*Shorter = FALSE;
                }

            break;
        case EH:
            if ( pNode->lc )
            {
                pLeaf = DeleteRightLeaf( &(pNode->lc), &shorter );
                if ( shorter )
                {
                    pLeaf->bf = RH;
                }else{
                    pLeaf->bf = EH;
                }
				*Shorter = FALSE;
            }
            break;
        }
        if ( pLeaf )
        {
            pLeaf->lc = pNode->lc;
            pLeaf->rc = pNode->rc;
        }
        (*Node) = pLeaf;
        free( pNode );
        ret = TRUE;
    }

    return ret;
}

unsigned char CheckAVL( PAVL_NODE Node )
{
    int ld;
    int rd;
    int diff;
    // check the deepth for each leaf.
    if ( Node )
    {
        ld = _get_avl_deepth_recursive_method( Node->lc );
        rd = _get_avl_deepth_recursive_method( Node->rc );
        switch( Node->bf )
        {
        case EH:
            assert( ld == rd );
            break;
        case LH:
            assert( ld > rd );
            break;
        case RH:
            assert( ld < rd );
            break;
        }
        
        diff = ld - rd;
        if ( ( diff > 1 ) || ( diff < -1 ) )
        {
            _tprintf( _T("Deepth Error: diff %d, left deepth %d right deepth %d value %d\n"), 
                diff, ld, rd, Node->value );
            return FALSE;
        }else{
            return TRUE;
        }
    }else{
        return TRUE;
    }
}
int _get_avl_deepth_recursive_method( PAVL_NODE Node )
{
    int leftDeepth;
    int rightDeepth;
    int diff;
    if ( Node == NULL )
    {
        return 0;
    }
    leftDeepth = 1 + _get_avl_deepth_recursive_method( Node->lc );
    rightDeepth = 1 + _get_avl_deepth_recursive_method( Node->rc );
    diff = leftDeepth - rightDeepth;
    if ( ( diff > 1 ) || ( diff < -1 ) )
    {
        _tprintf( _T("Deepth Error: diff %d, left deepth %d right deepth %d value %d\n"), 
            diff, leftDeepth, rightDeepth, Node->value );
    }else{
        //_tprintf( _T("Node %d diff %d OK!\n"), Node->value, diff );
    }

    return ( ( leftDeepth > rightDeepth ) ? leftDeepth : rightDeepth );

}
void QuickSortInPlace( int *Array, int Size )
{
    int i, j;
    int key;
    key = Array[ 0 ];
    i = 0;
    j = Size - 1;
    if ( Size < 2 )
    {
        return;
    }
    while( i < j )
    {
        for( ; i < j; j--)
        {
            if( Array[ j ] < key )
            {
                Array[ i ] = Array[ j ];
                break;
            }
        }
        for( ; i < j; i++)
        {
            if ( Array[ i ] > key )
            {
                Array[ j ] = Array[ i ];
                break;
            }
        }
    }
    Array[ i ] = key;
    QuickSortInPlace( Array, i );
    QuickSortInPlace( Array + i + 1, ( Size - i - 1 ) );
}

void DisplayArray( int* Array, int Size )
{
    int i;
    if ( Size < 40 )
    {
        for( i = 0; i < Size; i++)
        {
            _tprintf( _T("%2d, "), Array[ i ] );
        }
        _tprintf( _T("\n") );
    }else{

        for( i = 0; i < 36; i++)
        {
            _tprintf( _T("%2d "), Array[ i ] );
        }
        _tprintf( _T("... "));
        for( i = Size - 5; i < Size; i++)
        {
            _tprintf( _T("%2d "), Array[ i ] );
        }
        _tprintf( _T("\n") );
    }
}
void DestroyAVL( PAVL_NODE Node )
{
    if ( Node == NULL )
    {
        return;
    }
    DestroyAVL( Node->lc );
    DestroyAVL( Node->rc );
    free( Node );
}

void TestAVL()
{
	int data[ NODE_NUM ] = { 0 };
    //int data[ NODE_NUM ] = { 2, 7, 9, 6, 10, 5, 11, 4, 12, 3, 13, 8, 14, 1, 15 };
    //int data[ NODE_NUM ] = { 9, 8, 7, 6, 5, 4, 3, 2, 1, 15, 14, 13, 12, 11, 10 };
	//int data[ NODE_NUM ] = { 13, 14, 10,  7,  1,  2,  8,  3, 11,  6,  4,  9,  5, 12, 15 };
    PAVL_NODE root;
    int i;
    int j;
    unsigned char taller;

	RandomSeedInit();

#ifdef RANDOM_SEQUENCE
    i = 0;
#else
    i = MAX_LOOP - 1;
#endif
    root = NULL;

    for( ; i < MAX_LOOP; i++)
    {

#ifdef RANDOM_SEQUENCE
        for( j = 0; j < NODE_NUM; j++)
        {
            data[ j ] = j + 1;
        }
        RandomShuffle( data, NODE_NUM );
#endif
		if ( ( i % 1024 ) == 0 )
		{
			_tprintf( _T("No.%5dk loop\n"), ( i / 1024 ) );
		}
        //DisplayArray( data, NODE_NUM );



        root = NULL;
        for( j = 0; j < NODE_NUM; j++ )
        {
            InsertNodeIntoAVL( &root, data[ j ], &taller );

            if ( FALSE == CheckAVL( root ) )
            {
                _tprintf( _T("No.%2d item %d, AVL error\n"), i, data[ j ] );
                break;
            }

        }
        for( j = 0; j < NODE_NUM; j++ )
        {
            DeleteNodeFromAVL( &root, data[ j ], &taller );


            if ( FALSE == CheckAVL( root ) )
            {
                _tprintf( _T("No.%2d item %d, AVL error\n"), i, data[ j ] );
                break;
            }

        }
    }


    DestroyAVL( root );

}

#if 0
    for( i = 0; i < NODE_NUM; i++)
    {
        dataForQS[ i ] = data[ i ];
    }
    QuickSortInPlace( dataForQS, NODE_NUM );
    DisplayArray( dataForQS, NODE_NUM );
#endif


#ifndef WET2_DATA_STRUCTURES_STATIC_CONST_H
#define WET2_DATA_STRUCTURES_STATIC_CONST_H


typedef int Player;

const static int ZERO = 0;
static const int ONE = 1;
static const int TWO = 2;

static const int PLAYER_NOT_FOUND = -1;

const static int NODE_OWN_HEIGHT = 1;
const static int NULL_NODE_HEIGHT = -1;

static const int LEFT_IMBALANCE = 2;
static const int RIGHT_IMBALANCE = -2;

static const int INITIAL_SIZE = 10; //the initial size of the hashtable dynamic array
static const int EXPANSION_MULT = 2; // the factor with which we expand the array
static const double MAX_LOAD_FACTOR = 2;
static const double MIN_LOAD_FACTOR =0.5;
static const int MIN_ARRAY_SIZE = 10;

static const int EMPTY_MAX_RANK = -1;
static const int NOT_FOUND = -1;

//int max(int a, int b){
//    return a > b ? a : b;
//}



#endif //WET2_DATA_STRUCTURES_STATIC_CONST_H

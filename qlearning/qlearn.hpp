#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <time.h>
#include <string>
#include <sstream>
using namespace std;
#define STATEMIN 0
#define STATEMAX 9
#define ACTIONMAX 4
#define ACTIONMIN 0
#define RCMIN 0
#define RCMAX 2
#define TEST 4

enum direction
{
    UP = 0,
    DOWN,
    LEFT,
    RIGHT
};


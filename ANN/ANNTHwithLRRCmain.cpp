/*******************************************************************************
 *
 * File Name: ANNTHwithLRRCmain.c
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2012 Baxter AI (baxterai.com)
 * Project: Advanced Neural Network (ANN)
 * Project Version: 3a8a 14-June-2012
 * Comments: TH = Test Harness
 *
 *******************************************************************************/

#include <ctime>
#include <cstdlib>	//for random number generation
#include <cmath>

#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <string.h>
#include <iostream>
#include <fstream>
using namespace std;


#include "ANNglobalDefs.h"

#ifdef USE_LRRC
#include "LRRCTHgame.h"
#endif



int main()
{

/*
#ifdef COMPILE_TH_RT
	printf("1");
	exit(0);
#elif defined COMPILE_TH_ANN_WITH_LRRC
	printf("2");
	exit(0);
#else
	printf("3");
	exit(0);
#endif
*/

	bool result = true;

	//srand(static_cast<unsigned>(time(0)));
	srand( (unsigned)time(NULL) );	//seeds randomness

	result = THtestANNusingCombatExperiences();

	return result;
}





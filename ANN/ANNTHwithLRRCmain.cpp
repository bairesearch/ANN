 /*******************************************************************************
 *
 * License: GNU Affero General Public License version 3 (AGPLv3)
 *
 *******************************************************************************/

/*******************************************************************************
 *
 * File Name: ANNTHwithLRRCmain.cpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2022 Baxter AI (baxterai.com)
 * Project: Artificial Neural Network (ANN)
 * Project Version: 3q1a 05-June-2022
 * Comments: TH = Test Harness
 * /
 *******************************************************************************/


#include "ANNglobalDefs.hpp"

#ifdef USE_LRRC
#include "LRRCTHgame.hpp"
#endif



int main()
{
	bool result = true;

	//srand(static_cast<unsigned>(time(0)));
	srand( (unsigned)time(NULL) );	//seeds randomness

	result = LRRCTHgameClass().THtestANNusingCombatExperiences();

	return result;
}





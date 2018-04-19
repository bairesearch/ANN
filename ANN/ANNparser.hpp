/*******************************************************************************
 *
 * No License
 * 
 * This work is under exclusive copyright (c) Baxter AI (baxterai.com). 
 * Nobody else can use, copy, distribute, or modify this work without being 
 * at risk of take-downs, shake-downs, or litigation. 
 * 
 * By publishing this source code in a public repository on GitHub, Terms of 
 * Service have been accepted by which Baxter AI have allowed others to view 
 * and fork their repository.
 * 
 * If you find software that doesn't have a license, that generally means you 
 * have no permission from the creators of the software to use, modify, or 
 * share the software. Although a code host such as GitHub may allow you to 
 * view and fork the code, this does not imply that you are permitted to use, 
 * modify, or share the software for any purpose.
 *
 * This notice has been derived from https://choosealicense.com/no-permission 
 * (https://web.archive.org/web/20180312144938/https://choosealicense.com/no-permission)
 *
 *******************************************************************************/

/*******************************************************************************
 *
 * File Name: ANNparser.hpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2018 Baxter AI (baxterai.com)
 * Project: Artificial Neural Network (ANN)
 * Project Version: 3m13b 22-February-2018
 * Comments: TH = Test Harness
 * /
 *******************************************************************************/


#include "ANNdata.hpp"
#include "ANNexperienceClass.hpp"
#include "SHAREDvars.hpp"

#ifndef HEADER_ANN_PARSER
#define HEADER_ANN_PARSER

#define EXPERIENCE_DATASET_MAX_NUM_ROWS_ANNTH 1000	//max num experiences in dataset
#define EXPERIENCE_DATASET_MAX_NUM_COLS_ANNTH 101 	//max num input neurons +1

#define	SOME_REALLY_SMALL_FLOAT 0.0F
#define	SOME_REALLY_LARGE_FLOAT 500.0F

class ANNparserClass
{
	private: SHAREDvarsClass SHAREDvars;
	public: void ANNparseDataFile(string nameOfExperiencesDataSetFile);
};

#endif

 /*******************************************************************************
 *
 * License: GNU Affero General Public License version 3 (AGPLv3)
 *
 *******************************************************************************/

/*******************************************************************************
 *
 * File Name: ANNparser.hpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2022 Baxter AI (baxterai.com)
 * Project: Artificial Neural Network (ANN)
 * Project Version: 3q1a 05-June-2022
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

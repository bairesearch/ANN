/*******************************************************************************
 *
 * File Name: ANNData.h
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2010 Baxter AI (baxterai.com)
 * Project: Advanced Neural Network (ANN)
 * Project Version: 3a6b 30-Apr-2012
 * Comments: TH = Test Harness
 *
 *******************************************************************************/


#include "ANNexperienceClass.h"

#ifndef HEADER_ANN_DATA
#define HEADER_ANN_DATA





//for specific Thyroid data {215lines, 6 columns - comma delimited}


/*
extern float normalisedInputDataTemp[EXPERIENCE_DATASET_NUM_ROWS_ANNTH][EXPERIENCE_DATASET_NUM_COLUMNS_ANNTH];
extern bool normalisedInputDataTempFilled[EXPERIENCE_DATASET_NUM_ROWS_ANNTH];
extern float normalisedInputData[EXPERIENCE_DATASET_NUM_ROWS_ANNTH][EXPERIENCE_DATASET_NUM_COLUMNS_ANNTH];
extern float inputData[EXPERIENCE_DATASET_NUM_ROWS_ANNTH+1][EXPERIENCE_DATASET_NUM_COLUMNS_ANNTH+1];		//max sets of rows/columns+1 so that arrays are not overruritten during parse
extern char dataFileName[MAX_LENGTH_OF_DATA_FILE_NAME_ANNTH];
*/

extern long numInputNeurons;
extern long numOutputNeurons;
extern long numExperiences;
extern Experience * firstExperienceInDataSet;

#endif

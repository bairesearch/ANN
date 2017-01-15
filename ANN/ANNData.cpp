/*******************************************************************************
 *
 * File Name: ANNData.c
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2010 Baxter AI (baxterai.com)
 * Project: Advanced Neural Network (ANN)
 * Project Version: 3a6c 07-May-2012
 * Comments: TH = Test Harness
 *
 *******************************************************************************/

#include "ANNData.h"





//for specific Thyroid data {215lines, 6 columns - comma delimited}

/*
float normalisedInputDataTemp[EXPERIENCE_DATASET_NUM_ROWS_ANNTH][EXPERIENCE_DATASET_NUM_COLUMNS_ANNTH];
bool normalisedInputDataTempFilled[EXPERIENCE_DATASET_NUM_ROWS_ANNTH];
float normalisedInputData[EXPERIENCE_DATASET_NUM_ROWS_ANNTH][EXPERIENCE_DATASET_NUM_COLUMNS_ANNTH];
float inputData[EXPERIENCE_DATASET_NUM_ROWS_ANNTH+1][EXPERIENCE_DATASET_NUM_COLUMNS_ANNTH+1];		//max sets of rows/columns+1 so that arrays are not overruritten during parse
char dataFileName[MAX_LENGTH_OF_DATA_FILE_NAME_ANNTH];
*/


long numInputNeurons;
long numOutputNeurons;
long numExperiences;
Experience * firstExperienceInDataSet;

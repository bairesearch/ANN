/*******************************************************************************
 * 
 * This file is part of BAIPROJECT.
 * 
 * BAIPROJECT is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License version 3
 * only, as published by the Free Software Foundation.
 * 
 * BAIPROJECT is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License version 3 for more details
 * (a copy is included in the LICENSE file that accompanied this code).
 * 
 * You should have received a copy of the GNU Affero General Public License
 * version 3 along with BAIPROJECT.  If not, see <http://www.gnu.org/licenses/>
 * for a copy of the AGPLv3 License.
 * 
 *******************************************************************************/
 
/*******************************************************************************
 *
 * File Name: ANNData.c
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2012 Baxter AI (baxterai.com)
 * Project: Advanced Neural Network (ANN)
 * Project Version: 3a12a 31-July-2012
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

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
 * File Name: ANNparser.cpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2018 Baxter AI (baxterai.com)
 * Project: Artificial Neural Network (ANN)
 * Project Version: 3m12b 15-January-2018
 * Comments: TH = Test Harness
 *
 *******************************************************************************/


#include "ANNparser.hpp"





void ANNparserClass::ANNparseDataFile(string nameOfExperiencesDataSetFile)
{
	float* inputData = new float[EXPERIENCE_DATASET_MAX_NUM_ROWS_ANNTH*EXPERIENCE_DATASET_MAX_NUM_COLS_ANNTH];


	string currentNumberString = "";

	/* Data file layout

	1:	Class attribute (1 = normal, 2 = hyper, 3 = hypo)
	2:	T3-resin uptake test. (A percentage)
	3:	Total Serum thyroxin as measured by the isotopic
		displacement method.
	4: 	Total serum triiodothyronine as measured by radioimmuno
		assay.
	5: 	basal thyroid-stimulating hormone (TSH) as measured by
		radioimmuno assay.
	6: 	Maximal absolute difference of TSH value after injection of
		200 micro grams of thyrotropin-releasing hormone as compared
		to the basal value.

	*/



	ifstream textFile(nameOfExperiencesDataSetFile.c_str());
	char c;

	int currentRow = 0;
	int currentColumn = 0;
	int maxRows = 0;
	int maxCols = 0;

	while ((textFile).get(c))
	{
		switch(c)
		{
			case CHAR_NEWLINE:
			{
				inputData[currentRow*EXPERIENCE_DATASET_MAX_NUM_COLS_ANNTH + currentColumn] = SHAREDvars.convertStringToDouble(currentNumberString);
				currentNumberString = "";


				if(currentColumn > maxCols)
				{
					maxCols = currentColumn;
				}

				currentRow++;
				currentColumn = 0;
				break;
			}
			case CHAR_COMMA:
			{
				inputData[currentRow*EXPERIENCE_DATASET_MAX_NUM_COLS_ANNTH + currentColumn] = SHAREDvars.convertStringToDouble(currentNumberString);
				currentNumberString = "";
				currentColumn++;
				break;
			}
			default:
			{
				currentNumberString = currentNumberString + c;
				break;
			}
		}
	}


	int q, i, j;


	//initialise shared variables
	//now calculate the number of input neurons, the number of output neurons, and the number of datasets...

	maxRows = currentRow;
	maxCols = maxCols+1;
	numInputNeurons = maxCols-1;
	numExperiences = maxRows;


	long maxClassTarget = 0;
	for(i=0;i<numExperiences;i++)
	{
		if(inputData[i*EXPERIENCE_DATASET_MAX_NUM_COLS_ANNTH + 0] > maxClassTarget)
		{
			maxClassTarget = inputData[i*EXPERIENCE_DATASET_MAX_NUM_COLS_ANNTH + 0];
		}
	}
	numOutputNeurons = maxClassTarget;


	//now normalise data;
	float* normalisedInputDataTemp = new float[maxRows* maxCols];
	bool* normalisedInputDataTempFilled = new bool[maxRows];
	float* normalisedInputData = new float[maxRows* maxCols];


	//1. calculate max/min values

	float* min = new float[maxCols];
	float* max = new float[maxCols];
	//float maxMin[EXPERIENCE_DATASET_MAX_NUM_COL_ANNTH][2];
		//stores maximum [0] and minimum [1] value for each type of inputData


	for(q=0;q<maxCols;q++)
	{
		min[q] = SOME_REALLY_LARGE_FLOAT;  //500
		max[q] = SOME_REALLY_SMALL_FLOAT; //0
	}

	for(i=0;i<maxRows;i++)
	{
		for(j=0;j<maxCols;j++)
		{

			if(inputData[i*EXPERIENCE_DATASET_MAX_NUM_COLS_ANNTH + j] > max[j])
			{
				max[j] = inputData[i*EXPERIENCE_DATASET_MAX_NUM_COLS_ANNTH + j];
			}


			if(inputData[i*EXPERIENCE_DATASET_MAX_NUM_COLS_ANNTH + j] < min[j])
			{
				min[j] = inputData[i*EXPERIENCE_DATASET_MAX_NUM_COLS_ANNTH + j];

			}
		}
	}



	//2. insert normalised data entries
	for(i=0;i<maxRows;i++)
	{
		for(j=0;j<maxCols;j++)
		{
			if(j==0)
			{
				normalisedInputData[i*maxCols + j] = inputData[i*EXPERIENCE_DATASET_MAX_NUM_COLS_ANNTH + j];
			}
			else
			{
				normalisedInputData[i*maxCols + j] = ((inputData[i*EXPERIENCE_DATASET_MAX_NUM_COLS_ANNTH + j] - min[j]) / (max[j] - min[j]));
			}
		}
	}


	//randomise normalisedInputData {so that classes are evenly distributed between rows)


	bool stillToFill = true;

	for(int i=0;i < maxRows; i++)
	{
		normalisedInputDataTempFilled[i] = false;
	}

	int cnt = 0;


	#ifndef ANN_DEBUG_DO_NOT_RANDOMISE_INPUT_DATA
	while(stillToFill)
	{
		#ifndef DEBUG_TRAIN_NETWORK_WITH_NON_RANDOM_VARS
		int res = (int) (float(rand()* float(maxRows))/(float(RAND_MAX)+1.0));
		#endif

		if(normalisedInputDataTempFilled[res] == false)
		{//fill it
			for(int j=0;j<maxCols;j++)
			{
				normalisedInputDataTemp[res*maxCols + j] = normalisedInputData[cnt*maxCols + j];

			}
			normalisedInputDataTempFilled[res] = true;
			cnt++;
		}
		else
		{//do not fill it

		}

		//check if still entries to fill...
		stillToFill = false;
		for(int i=0;i<maxRows;i++)
		{
			if(normalisedInputDataTempFilled[i] == false)
			{
				stillToFill = true;
			}
			else
			{
				//do nothing
			}
		}
	}

	//now copy all normalisedInputDataTemp back into normalisedInputData
	for(int i=0;i<maxRows;i++)
	{
		for(int j=0;j<maxCols;j++)
		{
			normalisedInputData[i*maxCols + j] = normalisedInputDataTemp[i*maxCols + j];
		}
	}
	#endif




	//new now copy normalisedInputData into experienceClass Linked Lists



	firstExperienceInDataSet = new ANNexperience();
	ANNexperience* currentExperienceInDataSet = firstExperienceInDataSet;

	for(int i=0;i<maxRows;i++)
	{
		currentExperienceInDataSet->classTargetValue = normalisedInputData[i*maxCols + 0] - 1;
			//+1 added 4 Sept 10 (to ensure class targets start at 0, not 1

		ANNexperienceInput* currentExperienceInputInExperience = currentExperienceInDataSet->firstExperienceInput;

		for(int j=1;j<maxCols;j++)
		{
			currentExperienceInputInExperience->inputValue = normalisedInputData[i*maxCols + j];
				//currentExperienceInputInExperience->inputValue = (normalisedInputData[i*maxCols + j]*2.0 - 1.0) - allows negative input values

			ANNexperienceInput* newExperienceInput = new ANNexperienceInput();
			currentExperienceInputInExperience->next = newExperienceInput;
			currentExperienceInputInExperience = currentExperienceInputInExperience->next;
		}

		ANNexperience* newExperience = new ANNexperience();
		currentExperienceInDataSet->next = newExperience;
		currentExperienceInDataSet = currentExperienceInDataSet->next;
	}


	//delete inputData;
	delete normalisedInputDataTemp;
	delete normalisedInputDataTempFilled;
	delete normalisedInputData;
	delete min;
	delete max;
}

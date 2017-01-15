/*******************************************************************************
 *
 * This file is part of BAIPROJECT.
 *
 * BAIPROJECT is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License version 3
 * only, as published by the Free Software Foundation. The use of
 * intermediary programs or interfaces including file i/o is considered
 * remote network interaction. This does not imply such arrangements
 * do not constitute derivative works.
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
 * File Name: ANNparser.cpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2017 Baxter AI (baxterai.com)
 * Project: Artificial Neural Network (ANN)
 * Project Version: 3j1a 14-January-2017
 * Comments: TH = Test Harness
 *
 *******************************************************************************/


#include "ANNdata.h"
#include "ANNparser.h"
#include "ANNexperienceClass.h"
#include "SHAREDvars.h"





void ANNparseDataFile(string nameOfExperiencesDataSetFile)
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
				inputData[currentRow*EXPERIENCE_DATASET_MAX_NUM_COLS_ANNTH + currentColumn] = convertStringToDouble(currentNumberString);
				currentNumberString = "";

					#ifdef ANN_DEBUG
					//cout << "DEBUG: inputData[" << currentRow << "][" << currentColumn << "] = " << inputData[currentRow][currentColumn] << endl;
					#endif

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
				inputData[currentRow*EXPERIENCE_DATASET_MAX_NUM_COLS_ANNTH + currentColumn] = convertStringToDouble(currentNumberString);
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

	#ifdef ANN_DEBUG
	//cout << "maxRows = " << maxRows << endl;
	//cout << "maxCols = " << maxCols << endl;
	#endif

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
			#ifdef ANN_DEBUG
			//cout << "DEBUG: inputData[" << i << "][" << j << "] = " << inputData[i][j] << endl;
			cout << "DEBUG: maxMin[" << j << "][1] = " << maxMin[j][1] << endl;
			#endif

			if(inputData[i*EXPERIENCE_DATASET_MAX_NUM_COLS_ANNTH + j] > max[j])
			{
				max[j] = inputData[i*EXPERIENCE_DATASET_MAX_NUM_COLS_ANNTH + j];
			}


			if(inputData[i*EXPERIENCE_DATASET_MAX_NUM_COLS_ANNTH + j] < min[j])
			{
				min[j] = inputData[i*EXPERIENCE_DATASET_MAX_NUM_COLS_ANNTH + j];

				#ifdef ANN_DEBUG
				/*
				if(inputData[i][j] < -100.0F)
				{
					cout << "DEBUG: error found inputData[" << i << "][" << j << "] = " << inputData[i][j] << endl;
				}
				*/
				#endif
			}
		}
	}

	#ifdef ANN_DEBUG
	for(j=0;j<maxCols;j++)
	{
		cout << "Max value of input type (column) = " << j << " = " << max[j] << endl;
		cout << "Min value of input type (column) = " << j << " = " << min[j] << endl;
	}
	#endif


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
			#ifdef ANN_DEBUG
			//Testing
			/*
			cout << "DEBUG:inputData[i][j] = " << inputData[i][j] << endl;
			cout << "DEBUG:normalisedInputData[i][j] = " << normalisedInputData[i][j] << endl;
			cout << "DEBUG:maxMin[j][0] = " << maxMin[j][0] << endl;
			cout << "DEBUG: maxMin[j][1] = " << maxMin[j][1] << endl;
			cout << "\n" << endl;
			*/
			#endif
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
	#ifdef DEBUG_TRAIN_NETWORK_WITH_NON_RANDOM_VARS
	//cout << "RAND_MAX = " << RAND_MAX << endl;
	int index = 0;
	#endif
	while(stillToFill)
	{
		#ifdef DEBUG_TRAIN_NETWORK_WITH_NON_RANDOM_VARS
		int res = index;
		index++;
		if(index == maxRows)
		{
			index = 0;
		}
		#else
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
			#ifdef ANN_DEBUG
			cout << "normalisedInputData[" << i << "*maxCols + " << j << "] = " << normalisedInputData[i*maxCols + j] << endl;
			#endif
		}
	}
	#endif


	#ifdef ANN_DEBUG
	/*
	cout << "inputData[i][j] = " << inputData[0][1] << "\n" << endl;
	cout << "inputData[i][j] = " << inputData[0][2] << "\n" << endl;
	cout << "inputData[i][j] = " << inputData[0][3] << "\n" << endl;
	cout << "inputData[i][j] = " << inputData[0][4] << "\n" << endl;
	cout << "inputData[i][j] = " << inputData[0][5] << "\n" << endl;

	cout << "normalisedInputData[i][j] = " << normalisedInputData[0][1] << "\n" << endl;
	cout << "normalisedInputData[i][j] = " << normalisedInputData[0][2] << "\n" << endl;
	cout << "normalisedInputData[i][j] = " << normalisedInputData[0][3] << "\n" << endl;
	cout << "normalisedInputData[i][j] = " << normalisedInputData[0][4] << "\n" << endl;
	cout << "normalisedInputData[i][j] = " << normalisedInputData[0][5] << "\n" << endl;
	*/
	//cout << "normalisedInputData[i*maxCols + j] = " << normalisedInputData[0*maxCols + 0] << "\n" << endl;
	#endif


	//new now copy normalisedInputData into experienceClass Linked Lists



	firstExperienceInDataSet = new ANNexperience();
	ANNexperience* currentExperienceInDataSet = firstExperienceInDataSet;

	for(int i=0;i<maxRows;i++)
	{
		currentExperienceInDataSet->classTargetValue = normalisedInputData[i*maxCols + 0] - 1;
			//+1 added 4 Sept 10 (to ensure class targets start at 0, not 1
		#ifdef ANN_DEBUG
		//cout << "currentExperienceInDataSet->classTargetValue = " << currentExperienceInDataSet->classTargetValue << endl;
		#endif

		ANNexperienceInput* currentExperienceInputInExperience = currentExperienceInDataSet->firstExperienceInput;

		#ifdef ANN_DEBUG_GENERATE_NORMALISED_DATA_FILE
		cout << currentExperienceInDataSet->classTargetValue << " ";
		#endif
		for(int j=1;j<maxCols;j++)
		{
			currentExperienceInputInExperience->inputValue = normalisedInputData[i*maxCols + j];
				//currentExperienceInputInExperience->inputValue = (normalisedInputData[i*maxCols + j]*2.0 - 1.0) - allows negative input values
			#ifdef ANN_DEBUG
			//cout << "currentExperienceInputInExperience->inputValue = " << currentExperienceInputInExperience->inputValue  << endl;
			#endif
			#ifdef ANN_DEBUG_GENERATE_NORMALISED_DATA_FILE
			cout << currentExperienceInputInExperience->inputValue << " ";
			#endif

			ANNexperienceInput* newExperienceInput = new ANNexperienceInput();
			currentExperienceInputInExperience->next = newExperienceInput;
			currentExperienceInputInExperience = currentExperienceInputInExperience->next;
		}
		#ifdef ANN_DEBUG_GENERATE_NORMALISED_DATA_FILE
		cout << endl;
		#endif

		ANNexperience* newExperience = new ANNexperience();
		currentExperienceInDataSet->next = newExperience;
		currentExperienceInDataSet = currentExperienceInDataSet->next;
	}

	#ifdef ANN_DEBUG_GENERATE_NORMALISED_DATA_FILE
	exit(0);
	#endif

	//delete inputData;
	delete normalisedInputDataTemp;
	delete normalisedInputDataTempFilled;
	delete normalisedInputData;
	delete min;
	delete max;
}

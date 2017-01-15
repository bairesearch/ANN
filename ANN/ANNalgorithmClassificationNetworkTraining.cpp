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
 * File Name: ANNalgorithmClassificationNetworkTraining.cpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2016 Baxter AI (baxterai.com)
 * Project: Artificial Neural Network (ANN)
 * Project Version: 4a3a 02-May-2016
 * Comments:
 *
 *******************************************************************************/


#include "ANNglobalDefs.h"
#include "ANNneuronClass.h"
#include "ANNalgorithmClassificationNetworkTraining.h"
#include "ANNalgorithmClassificationNetworkUpdate.h"
#include "ANNdata.h"

#ifdef ANN_ALGORITHM_CLASSIFICATION_NETWORK

void trainNeuralNetworkClassificationSimple(ANNneuron* firstInputNeuron, ANNneuron** firstOutputNeuron, int numberOfInputNeurons, int* numberOfOutputNeurons, ANNexperience* firstExperienceInDataSet, long numberOfExperiences)
{
	int numberOfExperiencesTrain = 0; 
	int numberOfExperiencesTest = 0; 
			
	//training
	ANNexperience* currentExperience = firstExperienceInDataSet;
	for(int experienceNum = 0; experienceNum < numberOfExperiences; experienceNum++)
	{
		vector<double> inputValues;	//NOTUSED
		vector<bool> inputValuesCategoryFound;
		ANNexperienceInput* currentExperienceInput = currentExperience->firstExperienceInput;
		ANNneuron* currentNeuron = firstInputNeuron;
		while(currentExperienceInput->next != NULL)
		{
			currentNeuron->output = currentExperienceInput->inputValue;
			inputValues.push_back(currentExperienceInput->inputValue);	//NOTUSED
			inputValuesCategoryFound.push_back(false);
			//Classification Net ignores classTarget information
			currentExperienceInput = currentExperienceInput->next;
			currentNeuron = currentNeuron->nextNeuron;
		}
		
		ANNneuron* experienceClassificationTopLevelCategoryNeuron = new ANNneuron();
		experienceClassificationTopLevelCategoryNeuron->hasBackLayer = true;
		
		vector<bool> inputValuesCategoryFound(inputValuesCategoryFound.size(), false);
		
		bool foundCategoryNeuronsThatUseAllInputs = false;

		//YET TO DO: for stages 1->3; update ideal values
		
		//createIntermediaryNeuronsStage 1
		ANNneuron* currentNeuron = firstInputNeuron;
		while(currentNeuron->nextNeuron != NULL)
		{	
			if(!foundCategoryNeuronsThatUseAllInputs)
			{
				vector<bool> inputValuesCategoryFoundTemp(inputValuesCategoryFound.size(), false);
				findCategoriesForExperienceWrapper(currentNeuron, &inputValuesCategoryFound, &experienceClassificationTopLevelCategoryNeuron, 1);
				
				//prevent the needless rexecution of findCategoriesForExperienceWrapper:
				foundCategoryNeuronsThatUseAllInputs = true;
				for(vector<bool*>::iterator inputValuesCategoryFoundTempIter = inputValuesCategoryFoundTemp.begin(); inputValuesCategoryFoundTempIter != inputValuesCategoryFoundTemp.end(); inputValuesCategoryFoundTempIter++)
				{
					if(*inputValuesCategoryFoundTempIter == false)
					{
						foundCategoryNeuronsThatUseAllInputs = false;
					}
				}
			}
			currentNeuron = currentNeuron->nextNeuron;
		}
		
		//createIntermediaryNeuronsStage 2
		if(!foundCategoryNeuronsThatUseAllInputs)
		{
			ANNneuron* currentNeuron = firstInputNeuron;
			bool foundCategoryNeuronsThatUseAllInputs = false;
			while(currentNeuron->nextNeuron != NULL)
			{	
				if(!foundCategoryNeuronsThatUseAllInputs)
				{
					vector<bool> inputValuesCategoryFoundTemp(inputValuesCategoryFound.size(), false);
					findCategoriesForExperienceWrapper(currentNeuron, &inputValuesCategoryFound, &experienceClassificationTopLevelCategoryNeuron, 2);

					//prevent the needless rexecution of findCategoriesForExperienceWrapper:
					foundCategoryNeuronsThatUseAllInputs = true;
					for(vector<bool*>::iterator inputValuesCategoryFoundTempIter = inputValuesCategoryFoundTemp.begin(); inputValuesCategoryFoundTempIter != inputValuesCategoryFoundTemp.end(); inputValuesCategoryFoundTempIter++)
					{
						if(*inputValuesCategoryFoundTempIter == false)
						{
							foundCategoryNeuronsThatUseAllInputs = false;
						}
					}
				}
				currentNeuron = currentNeuron->nextNeuron;
			}
		}
		
		//createIntermediaryNeuronsStage 3
		if(!foundCategoryNeuronsThatUseAllInputs)
		{
			createIntermediaryNeurons = true;	
			ANNneuron* currentNeuron = firstInputNeuron;
			bool foundCategoryNeuronsThatUseAllInputs = false;
			while(currentNeuron->nextNeuron != NULL)
			{	
				if(!foundCategoryNeuronsThatUseAllInputs)
				{
					vector<bool> inputValuesCategoryFoundTemp(inputValuesCategoryFound.size(), false);
					findCategoriesForExperienceWrapper(currentNeuron, &inputValuesCategoryFound, &experienceClassificationTopLevelCategoryNeuron, 3);

					//prevent the needless rexecution of findCategoriesForExperienceWrapper:
					foundCategoryNeuronsThatUseAllInputs = true;
					for(vector<bool*>::iterator inputValuesCategoryFoundTempIter = inputValuesCategoryFoundTemp.begin(); inputValuesCategoryFoundTempIter != inputValuesCategoryFoundTemp.end(); inputValuesCategoryFoundTempIter++)
					{
						if(*inputValuesCategoryFoundTempIter == false)
						{
							foundCategoryNeuronsThatUseAllInputs = false;
						}
					}
				}
				currentNeuron = currentNeuron->nextNeuron;
			}
		}
		
		if(!foundCategoryNeuronsThatUseAllInputs)
		{
			//link experienceClassificationTopLevelCategoryNeuron and set idealValues
			
			ANNneuron* currentNeuron = firstInputNeuron;
			while(currentNeuron->nextNeuron != NULL)
			{	
				ANNneuronConnection* connection = connectNeurons(experienceClassificationTopLevelCategoryNeuron, currentNeuron);
				connection->idealValue = currentNeuron->output;
			}			
		}
		
		
		/*
		bool newInputNeuronStatesDetected = false;
		for(vector<bool*>::iterator inputValuesCategoryFoundIter = inputValuesCategoryFound.begin(); inputValuesCategoryFoundIter != inputValuesCategoryFound.end(); inputValuesCategoryFoundIter++)
		{
			if(*inputValuesCategoryFoundIter == false)
			{
				newInputNeuronStatesDetected = true;
			}
		}
				
		ANNneuron* currentNeuron = firstInputNeuron;
		int = experienceInputIndex;
		while(currentNeuron->nextNeuron != NULL)
		{	
			

			currentNeuron = currentNeuron->nextNeuron;
			experienceInputIndex++;
		}		
		*/

		/*
		resetInputs(firstInputNeuron, numberOfInputNeurons, currentExperience);
		string trainingClassificationResult = "";	//NOT USED
		double trainingMemoryResult = 0.0;	//NOT USED
		ANNclassificationAndMemoryPass(firstInputNeuron, firstOutputNeuron, &trainingClassificationResult, &trainingMemoryResult);
		#ifdef ANN_DEBUG
		#endif
		*/
		
		currentExperience = currentExperience->next;
		numberOfExperiencesTrain++;
		
	}
	
	/*
	//testing	
	double testingMemoryResultSum = 0.0;
	//creates and stores copy of the trained neural network
	storeNeuralNetworkMemoryTrace(firstInputNeuron);
	//from start of test segment -> end of test segment
	currentExperience = firstExperienceInDataSet;
	for(int experienceNum = 0; experienceNum < numberOfExperiences; experienceNum++)
	{
		restoreNeuralNetworkWithStoredMemoryTrace(firstInputNeuron);
		resetInputs(firstInputNeuron, numberOfInputNeurons, currentExperience);
		string testingClassificationResult = "";	//NOT USED
		double testingMemoryResult = 0.0;
		ANNclassificationAndMemoryPass(firstInputNeuron, firstOutputNeuron, &testingClassificationResult, &testingMemoryResult);
		testingMemoryResultSum = testingMemoryResultSum + testingMemoryResult;
		#ifdef ANN_DEBUG
		//cout << "current testingMemoryResult = " << testingMemoryResult << endl;
		#endif
		currentExperience = currentExperience->next;
		numberOfExperiencesTest++;
	}

	double testingMemoryResultAverage = testingMemoryResultSum/numberOfExperiencesTest;
	cout << "testingMemoryResultAverage = " << testingMemoryResultAverage << endl;

	//resets the trained neural network to the stored copy
	restoreNeuralNetworkWithStoredMemoryTrace(firstInputNeuron);
	*/
}




void findCategoriesForExperienceWrapper(ANNneuron* categoryNeuron, vector<bool>* inputValuesCategoryFound, ANNneuron** experienceClassificationTopLevelCategoryNeuron, int createIntermediaryNeuronsStage)
{	
	for(vector<ANNneuronConnection*>::iterator connectionIter = categoryNeuron->frontANNneuronConnectionList.begin(); connectionIter != categoryNeuron->frontANNneuronConnectionList.end(); connectionIter++)
	{
		hasFrontNeurons = true;
		ANNneuronConnection* currentANNneuronConnection = *connectionIter;
		ANNneuron* frontNeuron = currentANNneuronConnection->frontNeuron;
		double inputValue = categoryNeuron->output;
		if(calculateDiff(currentANNneuronConnection->idealValue, inputValue) < ANN_ALGORITHM_CLASSIFICATION_NETWORK_CATEGORY_AVERAGE_DIFF_THRESHOLD)	//CHECKTHIS - use the same threshold?
		{
			vector<bool> inputValuesCategoryFoundTemp(inputValuesCategoryFound->size(), false);	//ensure that values are initialised to false
			if(findCategoriesForExperience(frontNeuron, &inputValuesCategoryFoundTemp))
			{
				bool categoryNeuronUsesAllInputs = true;	//if this is true, then the frontNeuron will be the final (highest level) classification neuron for the input data
				for(vector<bool*>::iterator inputValuesCategoryFoundTempIter = inputValuesCategoryFoundTemp.begin(); inputValuesCategoryFoundTempIter != inputValuesCategoryFoundTemp.end(); inputValuesCategoryFoundTempIter++)
				{
					if(*inputValuesCategoryFoundTempIter == false)
					{
						categoryNeuronUsesAllInputs = false;
					}	
				}

				if(categoryNeuronUsesAllInputs)
				{
					if(createIntermediaryNeuronsStage == 1)
					{
						//algorithm assumptions: frontNeuron is a top level network category neuron
						if(frontNeuron->frontANNneuronConnectionList.size() == 0)
						{
							//top level category neuron detected

							//set experienceClassificationTopLevelCategoryNeuron to an existing category neuron in the network 
							delete(*experienceClassificationTopLevelCategoryNeuron);
							*experienceClassificationTopLevelCategoryNeuron = frontNeuron;
						}
						else
						{
							cout << "findCategoriesForExperienceWrapper{}: error: categoryNeuronUsesAllInputs && (frontNeuron->frontANNneuronConnectionList.size() != 0)" << endl;
							exit(0);
						}
					}
				}
				else
				{
					if(createIntermediaryNeuronsStage == 2)
					{
						//add the partially (input) satisfactory category neuron to experienceClassificationTopLevelCategoryNeuron
						ANNneuronConnection* connection = connectNeurons(experienceClassificationTopLevelCategoryNeuron, frontNeuron);
						updateConnectionIdealValue(connection);
						//DOING: set output


						/*
						//only update experienceClassificationTopLevelCategoryNeuron based on a partially (input) satisfactory category neuron if inputValuesCategoryFound is not complete (ie prioritise categorisation/connectivity based on the occurance of categoryNeuronUsesAllInputs above)
						bool alreadyFoundCategoryNeuronUsesAllInputs = true;
						for(vector<bool*>::iterator inputValuesCategoryFoundIter = inputValuesCategoryFound.begin(); inputValuesCategoryFoundIter != inputValuesCategoryFound.end(); inputValuesCategoryFoundIter++)
						{
							if(*inputValuesCategoryFoundTempIter == false)
							{
								alreadyFoundCategoryNeuronUsesAllInputs = false;
							}	
						}
						if(!alreadyFoundCategoryNeuronUsesAllInputs)
						{
							//add the partially (input) satisfactory category neuron to experienceClassificationTopLevelCategoryNeuron
							connectNeurons(experienceClassificationTopLevelCategoryNeuron, frontNeuron);
						}
						*/
					}
				}	

				
				//now update the inputValuesCategoryFound list before recursing
				vector<bool*>::iterator inputValuesCategoryFoundIter = inputValuesCategoryFound.begin();
				for(vector<bool*>::iterator inputValuesCategoryFoundTempIter = inputValuesCategoryFoundTemp.begin(); inputValuesCategoryFoundTempIter != inputValuesCategoryFoundTemp.end(); inputValuesCategoryFoundTempIter++)
				{
					if(*inputValuesCategoryFoundTempIter)
					{
						*inputValuesCategoryFoundIter = true;
					}
					inputValuesCategoryFoundIter++
				}	
				
				//recurse
				if(findCategoriesForExperienceWrapper(frontNeuron, inputValuesCategoryFound, createIntermediaryNeuronsStage))
				{

				}
			}
			else
			{
				if(createIntermediaryNeuronsStage == 3)
				{
					/*
					//CHECKTHIS requirement
					if(frontNeuron->frontANNneuronConnectionList.size() == 0)	
					{
						//top level category neuron detected
					*/
					//create a new intermediary category neuron for every criteria satisfied, and connect this to experienceClassificationTopLevelCategoryNeuron
					ANNneuron* intermediaryCategoryNeuron = new ANNneuron();
					intermediaryCategoryNeuron->hasFrontLayer = true;
					intermediaryCategoryNeuron->hasBackLayer = true;

					int numberOfInputMatches = 0;
					for(vector<ANNneuronConnection*>::iterator connectionIter = frontNeuron->backANNneuronConnectionList.begin(); connectionIter != frontNeuron->backANNneuronConnectionList.end(); connectionIter++)
					{
						ANNneuronConnection* currentANNneuronConnection = *connectionIter;
						ANNneuron* frontNeuronInputNeuron = currentANNneuronConnection->backNeuron;
						if(frontNeuronInputNeuron->inputNeuronMatchTemp)
						{
							numberOfInputMatches++;
						}
					}
					bool partialCriteriaSatisfaction = false;
					if(numberOfInputMatches >= ANN_ALGORITHM_CLASSIFICATION_NETWORK_MINIMUM_NUMBER_INPUTS_FOR_CATEGORY_NEURON)	//disregard cases where only a single input neuron matches
					{
						partialCriteriaSatisfaction = true;
						for(vector<ANNneuronConnection*>::iterator connectionIter = frontNeuron->backANNneuronConnectionList.begin(); connectionIter != frontNeuron->backANNneuronConnectionList.end(); connectionIter++)
						{
							ANNneuronConnection* currentANNneuronConnection = *connectionIter;
							ANNneuron* frontNeuronInputNeuron = currentANNneuronConnection->backNeuron;
							if(frontNeuronInputNeuron->inputNeuronMatchTemp)
							{
								//modify connection to insert intermediary category neuron
								currentANNneuronConnection->frontNeuron = intermediaryCategoryNeuron;
								intermediaryCategoryNeuron->backANNneuronConnectionList.push_back(currentANNneuronConnection);
								ANNneuronConnection* connection = connectNeurons(frontNeuron, intermediaryCategoryNeuron);
								
								updateConnectionIdealValue(currentANNneuronConnection);
								updateConnectionIdealValue(connection);
								//DOING: set output
								//DOING: set output
								

								//OLD: connectNeurons(intermediaryCategoryNeuron, frontNeuronInputNeuron);
							}
						}
					}
					if(partialCriteriaSatisfaction)
					{
						connectNeurons(experienceClassificationTopLevelCategoryNeuron, intermediaryCategoryNeuron);	
						
						//now update the inputValuesCategoryFound list
						vector<bool*>::iterator inputValuesCategoryFoundTempIter = inputValuesCategoryFoundTemp.begin();
						for(vector<bool*>::iterator inputValuesCategoryFoundIter = inputValuesCategoryFound.begin(); inputValuesCategoryFoundIter != inputValuesCategoryFound.end(); inputValuesCategoryFoundIter++)
						{
							if(*inputValuesCategoryFoundTempIter)
							{
								*inputValuesCategoryFoundIter = true;
							}
							inputValuesCategoryFoundTempIter++
						}
					}
					else
					{
						delete intermediaryCategoryNeuron;
					}
					/*
					}
					*/
				}
			}
		}
	}
}

void updateConnectionIdealValue(ANNneuronConnection* connection)
{
	connection->idealValue = connection->idealValue + connection->backNeuron->output/(connection->numberOfTimesConnectionHasBeenAccessedOrConnectionStrength);
	connection->numberOfTimesConnectionHasBeenAccessedOrConnectionStrength = connection->numberOfTimesConnectionHasBeenAccessedOrConnectionStrength + 1;
}


bool findCategoriesForExperience(ANNneuron* categoryNeuron, vector<bool>* inputValuesCategoryFound)
{
	bool allInputNeuronsForCategoryHaveMatchingValues = true; 
	
	int i=0;
	double totalDiff = 0;
	double totalOutput = 0;	//DOING: work out how to calculate this
	for(vector<ANNneuronConnection*>::iterator connectionIter = categoryNeuron->backANNneuronConnectionList.begin(); connectionIter != categoryNeuron->backANNneuronConnectionList.end(); connectionIter++)
	{
		ANNneuronConnection* currentANNneuronConnection = *connectionIter;
		ANNneuron* inputNeuron = currentANNneuronConnection->backNeuron;
		
		if(findCategoriesForExperience(currentANNneuronConnection->backNeuron, inputValuesCategoryFound))
		{
			inputNeuron->inputNeuronMatchTemp = true;
			totalDiff = totalDiff + calculateDiff(currentANNneuronConnection->idealValue, inputNeuron->output);
			totalOutput = totalOutput + calculateSum(inputNeuron->output);
		}
		else
		{
			inputNeuron->inputNeuronMatchTemp = false;
			allInputNeuronsForCategoryHaveMatchingValues = false;
		}
		i++;
	}

	if(allInputNeuronsForCategoryHaveMatchingValues)
	{
		int numberOfCategoryInputValues = categoryNeuron->backANNneuronConnectionList.size();
		if(numberOfCategoryInputValues > 0)
		{	
			double averageDiff = totalDiff/numberOfCategoryInputValues;
			allInputNeuronsForCategoryHaveMatchingValues = false;
			if(averageDiff < ANN_ALGORITHM_CLASSIFICATION_NETWORK_CATEGORY_AVERAGE_DIFF_THRESHOLD)
			{
				allInputNeuronsForCategoryHaveMatchingValues = true;

				for(vector<ANNneuronConnection*>::iterator connectionIter = categoryNeuron->backANNneuronConnectionList.begin(); connectionIter != categoryNeuron->backANNneuronConnectionList.end(); connectionIter++)
				{
					ANNneuronConnection* currentANNneuronConnection = *connectionIter;
					ANNneuron* inputNeuron = currentANNneuronConnection->backNeuron;
					int numberOfInputNeuronInputValues = inputNeuron->backANNneuronConnectionList.size();
					if(numberOfInputNeuronInputValues == 0)
					{
						//network input neuron detected
						inputValuesCategoryFound[inputNeuron->orderIDcounter] = true;
					}
				}
				
				double averageOutput = totalOutput/numberOfCategoryInputValues;
				categoryNeuron->output = averageOutput;
			}
		}
	}
	
	return allInputsForCategoryHaveMatchingValues;
}

ANNneuronConnection* connectNeurons(ANNneuron* parentNeuron, ANNneuron* childNeuron)
{
	childNeuron->hasFrontLayer = true;

	ANNneuronConnection* newANNneuronConnection = new ANNneuronConnection();
	newANNneuronConnection->backNeuron = childNeuron;
	newANNneuronConnection->frontNeuron = parentNeuron;
	childNeuron->frontANNneuronConnectionList.push_back(newANNneuronConnection);
	parentNeuron->backANNneuronConnectionList.push_back(newANNneuronConnection);
	
	return newANNneuronConnection;
}


double calculateDiff(double idealValue, double value)
{
	double diff = abs(value - idealValue);
	return diff;
}

double calculateSum(double value)
{
	double sum = abs(value); //verify this
	return sum;
}

//verify this

#endif


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
 * Project Version: 4a3c 02-May-2016
 * Comments:
 *
 *******************************************************************************/


#include "ANNglobalDefs.h"
#include "ANNneuronClass.h"
#include "ANNalgorithmClassificationNetworkTraining.h"
#include "ANNalgorithmClassificationNetworkUpdate.h"
#include "ANNdata.h"

#ifdef ANN_ALGORITHM_CLASSIFICATION_NETWORK

#ifdef ANN_ALGORITHM_CLASSIFICATION_NETWORK_PRUNING
multimap<int, ANNneuron*> neuronUsageList;
#endif

void trainNeuralNetworkClassificationSimple(ANNneuron* firstInputNeuron, ANNneuron** firstOutputNeuron, int numberOfInputNeurons, int* numberOfOutputNeurons, ANNexperience* firstExperienceInDataSet, long numberOfExperiences)
{
	int numberOfExperiencesTrain = 0; 
	int numberOfExperiencesTest = 0; 
			
	//training
	bool firstOutputNeuronHasBeenSet = false;
	ANNexperience* currentExperience = firstExperienceInDataSet;
	#ifdef ANN_ALGORITHM_CLASSIFICATION_NETWORK_STORE_OUTPUT_NEURONS
	ANNneuron* experienceClassificationTopLevelCategoryNeuronPrevious = NULL;
	#endif
	for(int experienceNum = 0; experienceNum < numberOfExperiences; experienceNum++)
	{
		vector<double> inputValues;	//NOTUSED
		vector<bool> inputValuesCategoryFound;
		ANNexperienceInput* currentExperienceInput = currentExperience->firstExperienceInput;
		ANNneuron* currentNeuron = firstInputNeuron;
		while(currentExperienceInput->next != NULL)
		{
			#ifdef ANN_ALGORITHM_CLASSIFICATION_NETWORK_PRUNING
			//neuronUsageList.insert(pair<int, ANNneuron*>(currentNeuron->memoryTrace, currentNeuron));	//do not insert input neurons into neuronUsageList
			#endif
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
		
		
		multimap<int, ANNneuron*>::iterator it = neuronUsageList.find(experienceClassificationTopLevelCategoryNeuron);
		if(it != neuronUsageList.last())	//does not find element (see [1*])
		{
			#ifdef ANN_ALGORITHM_CLASSIFICATION_NETWORK_PRUNING
			neuronUsageList.insert(pair<int, ANNneuron*>(experienceClassificationTopLevelCategoryNeuron->memoryTrace, experienceClassificationTopLevelCategoryNeuron));
			#endif
			#ifdef ANN_ALGORITHM_CLASSIFICATION_NETWORK_STORE_OUTPUT_NEURONS
			if(firstOutputNeuronHasBeenSet)
			{
				experienceClassificationTopLevelCategoryNeuronPrevious->next = experienceClassificationTopLevelCategoryNeuron;
			}
			else
			{
				*firstOutputNeuron = experienceClassificationTopLevelCategoryNeuron;
				firstOutputNeuronHasBeenSet = true;
			}
			experienceClassificationTopLevelCategoryNeuronPrevious = experienceClassificationTopLevelCategoryNeuron;
			(*numberOfOutputNeurons) = (*numberOfOutputNeurons) + 1;
			#endif
		}
						
		currentExperience = currentExperience->next;
		numberOfExperiencesTrain++;
	}
	
	#ifdef ANN_ALGORITHM_CLASSIFICATION_NETWORK_PRUNING
	ANNneuron* currentNeuron = firstInputNeuron;
	while(currentNeuron->nextNeuron != NULL)
	{
		pruneNetorkBasedOnRarelyUsedNeurons(currentNeuron);
		currentNeuron = currentNeuron->nextNeuron;
	}	
	#endif
}

#ifdef ANN_ALGORITHM_CLASSIFICATION_NETWORK_PRUNING
void pruneNetorkBasedOnRarelyUsedNeurons(ANNneuron* categoryNeuron)
{
	for(vector<ANNneuronConnection*>::iterator connectionIter = categoryNeuron->frontANNneuronConnectionList.begin(); connectionIter != categoryNeuron->frontANNneuronConnectionList.end(); connectionIter++)
	{
		ANNneuronConnection* currentANNneuronConnection = *connectionIter;
		ANNneuron* frontNeuron = currentANNneuronConnection->frontNeuron;
				
		if(frontNeuron->memoryTrace < ANN_ALGORITHM_CLASSIFICATION_NETWORK_PRUNING_MINIMUM_MEMORY_TRACE_TO_RETAIN_CATEGORY_NEURON)
		{
			multimap<int, ANNneuron*>::iterator it = neuronUsageList.find(experienceClassificationTopLevelCategoryNeuron);
			if(it != neuronUsageList.last())	//does not find element (see [1*])
			{
				neuronUsageList.remove(it);
			}
			
			//CHECKTHIS:
			for(vector<ANNneuronConnection*>::iterator connectionIter2 = frontNeuron->backANNneuronConnectionList.begin(); connectionIter2 != frontNeuron->backANNneuronConnectionList.end(); connectionIter2++)
			{
				ANNneuronConnection* currentANNneuronConnection2 = *connectionIter2;
				ANNneuron* inputNeuron = currentANNneuronConnection2->backNeuron;
				for(vector<ANNneuronConnection*>::iterator connectionIter3 = inputNeuron->frontANNneuronConnectionList.begin(); connectionIter3 != inputNeuron->frontANNneuronConnectionList.end(); )
				{
					if(*connectionIter3 == *connectionIter2)
					{
						connectionIter3 = inputNeuron->frontANNneuronConnectionList.remove(connectionIter3);
					}
				}
				delete currentANNneuronConnection2;
			}
			delete frontNeuron;	
		}
		
		pruneNetorkBasedOnRarelyUsedNeurons(frontNeuron);
	}
}
#endif



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

							//set experienceClassificationTopLevelCategoryNeuron to an existing category neuron in the network [1*]
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
								
							}
						}
					}
					if(partialCriteriaSatisfaction)
					{
						connectNeurons(experienceClassificationTopLevelCategoryNeuron, intermediaryCategoryNeuron);	
						#ifdef ANN_ALGORITHM_CLASSIFICATION_NETWORK_PRUNING
						intermediaryCategoryNeuron->memoryTrace = frontNeuron->memoryTrace + 1; //CHECKTHIS
						neuronUsageList.insert(pair<int, ANNneuron*>(intermediaryCategoryNeuron->memoryTrace, intermediaryCategoryNeuron));
						#endif
				
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
				}
			}
		}
	}
}

/*
void updateConnectionIdealValue(ANNneuronConnection* connection)
{
	connection->idealValue = connection->idealValue + (connection->backNeuron->output - connection->idealValue)/(connection->numberOfTimesConnectionHasBeenAccessedOrConnectionStrength);
	connection->numberOfTimesConnectionHasBeenAccessedOrConnectionStrength = connection->numberOfTimesConnectionHasBeenAccessedOrConnectionStrength + 1;
}
*/

void updateConnectionIdealValue(ANNneuronConnection* connection)
{
	connection->idealValue = connection->idealValue + (connection->backNeuron->output - connection->idealValue)/(connection->frontNeuron->memoryTrace); //nb this will evaluate to output if ideal value is 0 and memory trace is 1
	//connection->frontNeuron->memoryTrace = connection->frontNeuron->memoryTrace + 1;	//this is done in findCategoriesForExperience
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
				
				//double averageOutput = totalOutput/numberOfCategoryInputValues;
				//categoryNeuron->output = averageOutput;
				categoryNeuron->output = totalOutput;	//using totalOutput provides greater information resolution than averageOutput
				
				#ifdef ANN_ALGORITHM_CLASSIFICATION_NETWORK_PRUNING
				//if(createIntermediaryNeuronsStage == 1)	//prevent updating memory trace too often
				//{
					categoryNeuron->memoryTrace = categoryNeuron->memoryTrace + 1;
					multimap<int, ANNneuron*>::iterator it = neuronUsageList.find(categoryNeuron);
					it.first = categoryNeuron->memoryTrace;	//update position of neuron in neuronUsageList based on its current usage
				//}
				#endif
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


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
 * Project Version: 4a3r 02-May-2016
 * Comments:
 *
 *******************************************************************************/


#include "ANNglobalDefs.h"
#include "ANNneuronClass.h"
#include "ANNalgorithmClassificationNetworkTraining.h"
#include "ANNalgorithmClassificationNetworkUpdate.h"
#include "ANNdata.h"

#ifdef ANN_ALGORITHM_CLASSIFICATION_NETWORK

#ifdef ANN_ALGORITHM_CLASSIFICATION_NETWORK_PRUNING_OPTIMISE
multimap<int, ANNneuron*> neuronUsageList;
#endif

long IDCounter;

void trainNeuralNetworkClassificationSimple(ANNneuron* firstInputNeuron, ANNneuron** firstOutputNeuron, long numberOfInputNeurons, long* numberOfOutputNeurons, ANNexperience* firstExperienceInDataSet, long numberOfExperiences)
{
	int numberOfExperiencesTrain = 0; 
	int numberOfExperiencesTest = 0; 
			
	//training
	bool firstOutputNeuronHasBeenSet = false;
	ANNexperience* currentExperience = firstExperienceInDataSet;
	#ifdef ANN_ALGORITHM_CLASSIFICATION_NETWORK_STORE_OUTPUT_NEURONS
	ANNneuron* experienceClassificationTopLevelCategoryNeuronPrevious = NULL;
	#endif
	
	IDCounter = numberOfInputNeurons+1;
	
	for(int experienceNum = 0; experienceNum < numberOfExperiences; experienceNum++)
	{
		//#ifdef ANN_DEBUG_ALGORITHM_CLASSIFICATION_NETWORK
		cout <<"\texperienceNum = " << experienceNum << endl;
		//#endif
		
		vector<double> inputValues;	//NOTUSED
		ANNexperienceInput* currentExperienceInput = currentExperience->firstExperienceInput;
		ANNneuron* currentNeuron = firstInputNeuron;
		while(currentExperienceInput->next != NULL)
		{
			#ifdef ANN_ALGORITHM_CLASSIFICATION_NETWORK_PRUNING_OPTIMISE
			//neuronUsageList.insert(pair<int, ANNneuron*>(currentNeuron->memoryTrace, currentNeuron));	//do not insert input neurons into neuronUsageList
			#endif
			currentNeuron->output = currentExperienceInput->inputValue;
			inputValues.push_back(currentExperienceInput->inputValue);	//NOTUSED
			//Classification Net ignores classTarget information
			currentExperienceInput = currentExperienceInput->next;
			currentNeuron = currentNeuron->nextNeuron;
		}
		
		ANNneuron* experienceClassificationTopLevelCategoryNeuron = new ANNneuron();
		experienceClassificationTopLevelCategoryNeuron->nextNeuron = new ANNneuron();	//class architecture required to create a blank neuron
		experienceClassificationTopLevelCategoryNeuron->topLevelCategoryNeuron = true;
		experienceClassificationTopLevelCategoryNeuron->xPosRelFrac = (*numberOfOutputNeurons);
		experienceClassificationTopLevelCategoryNeuron->yPosRelFrac = ANN_ALGORITHM_CLASSIFICATION_NETWORK_NETWORK_DISPLAY_HEIGHT;
		experienceClassificationTopLevelCategoryNeuron->hasBackLayer = true;
		
		vector<bool> inputValuesCategoryFound(numberOfInputNeurons, false);
		
		bool foundCategoryNeuronsThatUseAllInputs = false;

		//OLD: YET TO DO: for stages 1->3; update ideal values
		
		int createIntermediaryNeuronsStage = 1;
		currentNeuron = firstInputNeuron;
		while(currentNeuron->nextNeuron != NULL)
		{	
			if(!foundCategoryNeuronsThatUseAllInputs)
			{
				#ifdef ANN_DEBUG_ALGORITHM_CLASSIFICATION_NETWORK
				cout <<"\t\tcreateIntermediaryNeuronsStage = 1" << endl;
				#endif
				//vector<bool> inputValuesCategoryFoundTemp(inputValuesCategoryFound.size(), false);
				findCategoriesForExperienceWrapper(currentNeuron, &inputValuesCategoryFound, &experienceClassificationTopLevelCategoryNeuron, createIntermediaryNeuronsStage);
				
				//prevent the needless rexecution of findCategoriesForExperienceWrapper:
				foundCategoryNeuronsThatUseAllInputs = true;
				for(vector<bool>::iterator inputValuesCategoryFoundIter = inputValuesCategoryFound.begin(); inputValuesCategoryFoundIter != inputValuesCategoryFound.end(); inputValuesCategoryFoundIter++)
				{
					if(*inputValuesCategoryFoundIter == false)
					{
						foundCategoryNeuronsThatUseAllInputs = false;
					}
				}
			}
			currentNeuron = currentNeuron->nextNeuron;
		}
				
		if(!foundCategoryNeuronsThatUseAllInputs)
		{
			createIntermediaryNeuronsStage = 2;
			ANNneuron* currentNeuron = firstInputNeuron;
			bool foundCategoryNeuronsThatUseAllInputs = false;
			while(currentNeuron->nextNeuron != NULL)
			{	
				if(!foundCategoryNeuronsThatUseAllInputs)
				{
					#ifdef ANN_DEBUG_ALGORITHM_CLASSIFICATION_NETWORK
					cout <<"\t\tcreateIntermediaryNeuronsStage = 2" << endl;
					#endif
				
					//vector<bool> inputValuesCategoryFoundTemp(inputValuesCategoryFound.size(), false);
					findCategoriesForExperienceWrapper(currentNeuron, &inputValuesCategoryFound, &experienceClassificationTopLevelCategoryNeuron, createIntermediaryNeuronsStage);

					//prevent the needless rexecution of findCategoriesForExperienceWrapper:
					foundCategoryNeuronsThatUseAllInputs = true;
					for(vector<bool>::iterator inputValuesCategoryFoundIter = inputValuesCategoryFound.begin(); inputValuesCategoryFoundIter != inputValuesCategoryFound.end(); inputValuesCategoryFoundIter++)
					{
						if(*inputValuesCategoryFoundIter == false)
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
			createIntermediaryNeuronsStage = 3;
			ANNneuron* currentNeuron = firstInputNeuron;
			bool foundCategoryNeuronsThatUseAllInputs = false;
			while(currentNeuron->nextNeuron != NULL)
			{	
				if(!foundCategoryNeuronsThatUseAllInputs)
				{
					#ifdef ANN_DEBUG_ALGORITHM_CLASSIFICATION_NETWORK
					cout <<"\t\tcreateIntermediaryNeuronsStage = 3" << endl;
					#endif
					
					//vector<bool> inputValuesCategoryFoundTemp(inputValuesCategoryFound.size(), false);
					findCategoriesForExperienceWrapper(currentNeuron, &inputValuesCategoryFound, &experienceClassificationTopLevelCategoryNeuron, createIntermediaryNeuronsStage);
					
					//prevent the needless rexecution of findCategoriesForExperienceWrapper:
					foundCategoryNeuronsThatUseAllInputs = true;
					for(vector<bool>::iterator inputValuesCategoryFoundIter = inputValuesCategoryFound.begin(); inputValuesCategoryFoundIter != inputValuesCategoryFound.end(); inputValuesCategoryFoundIter++)
					{
						if(*inputValuesCategoryFoundIter == false)
						{
							foundCategoryNeuronsThatUseAllInputs = false;
						}
					}
				}
				currentNeuron = currentNeuron->nextNeuron;
			}
			
			resetIntermediaryNeuronCreatedThisRoundFlag(currentNeuron);
		}
	
		if(!foundCategoryNeuronsThatUseAllInputs)
		{
			//link experienceClassificationTopLevelCategoryNeuron and set idealValues
			ANNneuron* currentNeuron = firstInputNeuron;
			int i = 0;
			while(currentNeuron->nextNeuron != NULL)
			{	
				if(inputValuesCategoryFound[i] == false)
				{
					ANNneuronConnection* connection = connectNeurons(experienceClassificationTopLevelCategoryNeuron, currentNeuron);
					#ifdef ANN_DEBUG_ALGORITHM_CLASSIFICATION_NETWORK
					if(experienceClassificationTopLevelCategoryNeuron == currentNeuron)
					{
						cout << "trainNeuralNetworkClassificationSimple{} error: (experienceClassificationTopLevelCategoryNeuron == currentNeuron)" << endl;
						exit(0);
					}
					#endif
					connection->idealValue = currentNeuron->output;
				}
				i++;
				currentNeuron = currentNeuron->nextNeuron;
			}
		}

		bool alreadyAddedOutputNeuronToList = false;
		#ifdef ANN_ALGORITHM_CLASSIFICATION_NETWORK_PRUNING_OPTIMISE
		multimap<int, ANNneuron*>::iterator it = neuronUsageList.find(experienceClassificationTopLevelCategoryNeuron);
		if(it == neuronUsageList.last())	//does not find element (see [1*])
		{
			alreadyAddedOutputNeuronToList = true;
		}
		#else
		if(createIntermediaryNeuronsStage == 1)
		{
			alreadyAddedOutputNeuronToList = true;
		}
		#endif
		if(!alreadyAddedOutputNeuronToList)
		{	
			experienceClassificationTopLevelCategoryNeuron->id = IDCounter;
			IDCounter++;
			
			#ifdef ANN_ALGORITHM_CLASSIFICATION_NETWORK_PRUNING_OPTIMISE
			neuronUsageList.insert(pair<int, ANNneuron*>(experienceClassificationTopLevelCategoryNeuron->memoryTrace, experienceClassificationTopLevelCategoryNeuron));
			#endif
			#ifdef ANN_ALGORITHM_CLASSIFICATION_NETWORK_STORE_OUTPUT_NEURONS
			if(firstOutputNeuronHasBeenSet)
			{	
				//cout << "experienceClassificationTopLevelCategoryNeuronPrevious->nextNeuron->id = " << experienceClassificationTopLevelCategoryNeuronPrevious->nextNeuron->id << endl;
				delete(experienceClassificationTopLevelCategoryNeuronPrevious->nextNeuron);
				experienceClassificationTopLevelCategoryNeuronPrevious->nextNeuron = experienceClassificationTopLevelCategoryNeuron;
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

	/*//DIAGNOSIS: output neuron list is being created fine (the issue is that some output neurons have front layers for some strange reason...):
	ANNneuron* currentNeuron = *firstOutputNeuron;
	while(currentNeuron->nextNeuron != NULL)
	{
		cout << "outputneuron = " << currentNeuron->id << endl;
		currentNeuron = currentNeuron->nextNeuron;
	}
	*/
	
		
	/*
	#ifdef ANN_ALGORITHM_CLASSIFICATION_NETWORK_PRUNING
	ANNneuron* currentNeuron = firstInputNeuron;
	while(currentNeuron->nextNeuron != NULL)
	{
		pruneNetorkBasedOnRarelyUsedNeurons(currentNeuron);
		currentNeuron = currentNeuron->nextNeuron;
	}	
	#endif
	cout << "af8" << endl;
	*/
}

void resetIntermediaryNeuronCreatedThisRoundFlag(ANNneuron* categoryNeuron)
{
	#ifdef ANN_DEBUG_ALGORITHM_CLASSIFICATION_NETWORK
	//cout << "resetIntermediaryNeuronCreatedThisRoundFlag{} start" << endl;
	#endif
		
	for(vector<ANNneuronConnection*>::iterator frontConnectionIter = categoryNeuron->frontANNneuronConnectionList.begin(); frontConnectionIter != categoryNeuron->frontANNneuronConnectionList.end(); frontConnectionIter++)
	{
		ANNneuronConnection* currentANNneuronConnection = *frontConnectionIter;
		ANNneuron* frontNeuron = currentANNneuronConnection->frontNeuron;
		
		if(frontNeuron->intermediaryNeuronCreatedThisRound)
		{
			frontNeuron->intermediaryNeuronCreatedThisRound = false;
		}
		
		resetIntermediaryNeuronCreatedThisRoundFlag(frontNeuron);
	}
}

#ifdef ANN_ALGORITHM_CLASSIFICATION_NETWORK_PRUNING
void pruneNetorkBasedOnRarelyUsedNeurons(ANNneuron* categoryNeuron)
{
	for(vector<ANNneuronConnection*>::iterator connectionIter = categoryNeuron->frontANNneuronConnectionList.begin(); connectionIter != categoryNeuron->frontANNneuronConnectionList.end(); )
	{
		ANNneuronConnection* currentANNneuronConnection = *connectionIter;
		ANNneuron* frontNeuron = currentANNneuronConnection->frontNeuron;
		
		cout << "af0" << endl;
		
		if(frontNeuron->memoryTrace < ANN_ALGORITHM_CLASSIFICATION_NETWORK_PRUNING_MINIMUM_MEMORY_TRACE_TO_RETAIN_CATEGORY_NEURON)
		{
			cout << "af1" << endl;
			
			#ifdef ANN_ALGORITHM_CLASSIFICATION_NETWORK_PRUNING_OPTIMISE
			multimap<int, ANNneuron*>::iterator it = neuronUsageList.find(experienceClassificationTopLevelCategoryNeuron);
			if(it != neuronUsageList.last())	//does not find element (see [1*])
			{
				neuronUsageList.erase(it);
			}
			#endif
			
			
			//CHECKTHIS:
			for(vector<ANNneuronConnection*>::iterator connectionIter2 = frontNeuron->backANNneuronConnectionList.begin(); connectionIter2 != frontNeuron->backANNneuronConnectionList.end(); connectionIter2++)
			{
				ANNneuronConnection* currentANNneuronConnection2 = *connectionIter2;
				ANNneuron* backNeuron = currentANNneuronConnection2->backNeuron;
				if(backNeuron != categoryNeuron)
				{
					cout << "af2" << endl;
					for(vector<ANNneuronConnection*>::iterator connectionIter3 = backNeuron->frontANNneuronConnectionList.begin(); connectionIter3 != backNeuron->frontANNneuronConnectionList.end(); )
					{
						if(*connectionIter3 == *connectionIter2)
						{
							connectionIter3 = backNeuron->frontANNneuronConnectionList.erase(connectionIter3);
						}
						else
						{
							connectionIter3++;
						}
					}
				}				
				//delete currentANNneuronConnection2;	//NOT POSSIBLE as it will erase frontNeuron
			}
			
			
			cout << "af3" << endl;
			
			connectionIter = categoryNeuron->frontANNneuronConnectionList.erase(connectionIter);
			
			cout << "af3b" << endl;
			//delete currentANNneuronConnection;	//CREATES BUG
			
			//delete frontNeuron;	//REDUNDANT (see ANNneuronConnection::~ANNneuronConnection)
			cout << "af4" << endl;

		}
		else
		{
			cout << "af5" << endl;
			pruneNetorkBasedOnRarelyUsedNeurons(frontNeuron);
			cout << "af6" << endl;
			connectionIter++;
		}
		
		cout << "af7" << endl;

	}
	
	cout << "af7" << endl;
}
#endif



void findCategoriesForExperienceWrapper(ANNneuron* categoryNeuron, vector<bool>* inputValuesCategoryFound, ANNneuron** experienceClassificationTopLevelCategoryNeuron, int createIntermediaryNeuronsStage)
{
	#ifdef ANN_DEBUG_ALGORITHM_CLASSIFICATION_NETWORK
	cout << "findCategoriesForExperienceWrapper{} start" << endl;
	#endif
	
	//int numberOfInputMatchesMax = 0;
	
	for(vector<ANNneuronConnection*>::iterator frontConnectionIter = categoryNeuron->frontANNneuronConnectionList.begin(); frontConnectionIter != categoryNeuron->frontANNneuronConnectionList.end(); frontConnectionIter++)
	{
		ANNneuronConnection* currentANNneuronConnection = *frontConnectionIter;
		ANNneuron* frontNeuron = currentANNneuronConnection->frontNeuron;
		if(frontNeuron != *experienceClassificationTopLevelCategoryNeuron)
		{
			if(!(frontNeuron->intermediaryNeuronCreatedThisRound))	//added 4a3h
			{
				double inputValue = categoryNeuron->output;
				if(calculateDiff(currentANNneuronConnection->idealValue, inputValue) < ANN_ALGORITHM_CLASSIFICATION_NETWORK_CATEGORY_AVERAGE_DIFF_THRESHOLD)	//CHECKTHIS - use the same threshold?
				{
					vector<bool> inputValuesCategoryFoundAbove(inputValuesCategoryFound->size(), false);	//ensure that values are initialised to false
					findCategoriesForExperienceWrapper(frontNeuron, &inputValuesCategoryFoundAbove, NULL, 4);
					int numberOfInputMatchesMaxAbove = 0;
					for(vector<bool>::iterator inputValuesCategoryFoundAboveIter = inputValuesCategoryFoundAbove.begin(); inputValuesCategoryFoundAboveIter != inputValuesCategoryFoundAbove.end(); inputValuesCategoryFoundAboveIter++)
					{
						if(*inputValuesCategoryFoundAboveIter == true)
						{
							numberOfInputMatchesMaxAbove++;
						}
					}
					
					vector<bool> inputValuesCategoryFoundTemp(inputValuesCategoryFound->size(), false);	//ensure that values are initialised to false
					bool foundCategoriesForExperience = findCategoriesForExperience(frontNeuron, &inputValuesCategoryFoundTemp);
					
					#ifdef ANN_DEBUG_ALGORITHM_CLASSIFICATION_NETWORK
					cout << "findCategoriesForExperience{} passed" << endl;
					#endif
					
					int numberOfInputMatches = 0;
					bool aboveLayerSatisfiesAllCurrentLayerInputs = true;
					bool categoryNeuronUsesAllInputs = true;	//if this is true, then the frontNeuron will be the final (highest level) classification neuron for the input data
					int i = 0;
					for(vector<bool>::iterator inputValuesCategoryFoundTempIter = inputValuesCategoryFoundTemp.begin(); inputValuesCategoryFoundTempIter != inputValuesCategoryFoundTemp.end(); inputValuesCategoryFoundTempIter++)
					{
						if(*inputValuesCategoryFoundTempIter == false)
						{
							categoryNeuronUsesAllInputs = false;
						}
						else
						{
							numberOfInputMatches++;
							if(!inputValuesCategoryFoundAbove[i])
							{
								aboveLayerSatisfiesAllCurrentLayerInputs = false;
							}
						}
						i++;
					}
					/*
					if(numberOfInputMatches > numberOfInputMatchesMax)
					{
						numberOfInputMatchesMax = numberOfInputMatches;
					}
					*/
					
					int numberOfBackMatches = 0;
					for(vector<ANNneuronConnection*>::iterator connectionIter = frontNeuron->backANNneuronConnectionList.begin(); connectionIter != frontNeuron->backANNneuronConnectionList.end(); connectionIter++)
					{
						ANNneuronConnection* currentANNneuronConnection = *connectionIter;
						ANNneuron* frontNeuronBackNeuron = currentANNneuronConnection->backNeuron;
						if(frontNeuronBackNeuron->inputNeuronMatchTemp)
						{
							numberOfBackMatches++;
						}
					}
					
					if((numberOfInputMatches >= numberOfInputMatchesMaxAbove) || !aboveLayerSatisfiesAllCurrentLayerInputs)	//added 4a3p
					{
						//assume that front neuron must have back neurons
							//if(frontNeuron->backANNneuronConnectionList.size() >= ANN_ALGORITHM_CLASSIFICATION_NETWORK_PRUNING_MINIMUM_MEMORY_TRACE_TO_RETAIN_CATEGORY_NEURON)
						if(numberOfBackMatches >= ANN_ALGORITHM_CLASSIFICATION_NETWORK_PRUNING_MINIMUM_MEMORY_TRACE_TO_RETAIN_CATEGORY_NEURON)
						{
							if(foundCategoriesForExperience)
							{
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
											delete((*experienceClassificationTopLevelCategoryNeuron)->nextNeuron);
											//*experienceClassificationTopLevelCategoryNeuron = frontNeuron;
											#ifdef ANN_DEBUG_ALGORITHM_CLASSIFICATION_NETWORK
											//exit(0);
											#endif
											createIntermediaryNeuronsStage = 10;	//added 4a3k - to prevent overwriting of experienceClassificationTopLevelCategoryNeuron
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
										if(frontNeuron->intermediaryNeuron)
										{
											#ifdef ANN_DEBUG_ALGORITHM_CLASSIFICATION_NETWORK
											cout << "\t\tvd" << endl;
											#endif
											//add the partially (input) satisfactory category neuron to experienceClassificationTopLevelCategoryNeuron
											bool alreadyConnectedNeurons = false;
											for(vector<ANNneuronConnection*>::iterator backConnectionIter = (*experienceClassificationTopLevelCategoryNeuron)->backANNneuronConnectionList.begin(); backConnectionIter != (*experienceClassificationTopLevelCategoryNeuron)->backANNneuronConnectionList.end(); backConnectionIter++)
											{
												ANNneuronConnection* currentANNneuronConnection = *backConnectionIter;
												ANNneuron* backNeuron = currentANNneuronConnection->backNeuron;
												if(backNeuron == frontNeuron)
												{
													cout << "numberOfBackMatches = " << numberOfBackMatches << endl;
													alreadyConnectedNeurons = true;
												}
											}
											if(!alreadyConnectedNeurons)
											{
												ANNneuronConnection* connection = connectNeurons(*experienceClassificationTopLevelCategoryNeuron, frontNeuron);
												updateConnectionIdealValue(connection);
												//DOING: set output
											}
										}
										else
										{
											cout << "findCategoriesForExperienceWrapper{} error:  findCategoriesForExperience && !categoryNeuronUsesAllInputs && (createIntermediaryNeuronsStage == 2) && !(frontNeuron->intermediaryNeuron)" << endl;
										}
									}
								}	
	
								#ifdef ANN_DEBUG_ALGORITHM_CLASSIFICATION_NETWORK
								cout << "at2" << endl;
								#endif
								//recurse
								//findCategoriesForExperienceWrapper(frontNeuron, inputValuesCategoryFound, experienceClassificationTopLevelCategoryNeuron, createIntermediaryNeuronsStage);
							}
							else
							{
								if(createIntermediaryNeuronsStage == 3)
								{
									#ifdef ANN_DEBUG_ALGORITHM_CLASSIFICATION_NETWORK
									//exit(0);
									#endif
	
									//create a new intermediary category neuron for every criteria satisfied, and connect this to experienceClassificationTopLevelCategoryNeuron
									ANNneuron* intermediaryCategoryNeuron = new ANNneuron();
									intermediaryCategoryNeuron->nextNeuron = new ANNneuron();	//class architecture required to create a blank neuron
									intermediaryCategoryNeuron->id = IDCounter;
									intermediaryCategoryNeuron->intermediaryNeuron = true;
									IDCounter++;
									intermediaryCategoryNeuron->hasFrontLayer = true;
									intermediaryCategoryNeuron->hasBackLayer = true;
	
									bool partialCriteriaSatisfaction = false;
									partialCriteriaSatisfaction = true;
									double frontNeuronBackNeuronXposAvg = 0.0;
									double frontNeuronBackNeuronYposAvg = 0.0;
									int numberOfBackMatchesTemp = 0;
									for(vector<ANNneuronConnection*>::iterator connectionIter = frontNeuron->backANNneuronConnectionList.begin(); connectionIter != frontNeuron->backANNneuronConnectionList.end(); connectionIter++)
									{
										ANNneuronConnection* currentANNneuronConnection = *connectionIter;
										ANNneuron* frontNeuronBackNeuron = currentANNneuronConnection->backNeuron;
										if(frontNeuronBackNeuron->inputNeuronMatchTemp)
										{
											frontNeuronBackNeuronXposAvg = frontNeuronBackNeuronXposAvg + frontNeuronBackNeuron->xPosRelFrac;
											frontNeuronBackNeuronYposAvg = frontNeuronBackNeuronYposAvg + frontNeuronBackNeuron->yPosRelFrac;
	
											//modify connection to insert intermediary category neuron
											currentANNneuronConnection->frontNeuron = intermediaryCategoryNeuron;
											intermediaryCategoryNeuron->backANNneuronConnectionList.push_back(currentANNneuronConnection);
											updateConnectionIdealValue(currentANNneuronConnection);
					
											numberOfBackMatchesTemp++;
											//DOING: set output
											//DOING: set output
										}
									}
									
									if(numberOfBackMatchesTemp != numberOfBackMatches)
									{
										cout << "implementation error: (numberOfBackMatchesTemp != numberOfBackMatches)" << endl;
										exit(0);
									}
									frontNeuronBackNeuronXposAvg = frontNeuronBackNeuronXposAvg/numberOfBackMatches;
									frontNeuronBackNeuronYposAvg = frontNeuronBackNeuronYposAvg/numberOfBackMatches;
									
									ANNneuronConnection* connection = connectNeurons(frontNeuron, intermediaryCategoryNeuron);
									updateConnectionIdealValue(connection);
									
									intermediaryCategoryNeuron->intermediaryNeuronCreatedThisRound = true;
									intermediaryCategoryNeuron->xPosRelFrac = (frontNeuronBackNeuronXposAvg + frontNeuron->xPosRelFrac)/2.0;
									intermediaryCategoryNeuron->yPosRelFrac = (frontNeuronBackNeuronYposAvg + frontNeuron->yPosRelFrac)/2.0;
									//cout << "intermediaryCategoryNeuron->xPosRelFrac = " << intermediaryCategoryNeuron->xPosRelFrac << endl;
									//cout << "intermediaryCategoryNeuron->yPosRelFrac = " << intermediaryCategoryNeuron->yPosRelFrac << endl;
									
									connectNeurons(*experienceClassificationTopLevelCategoryNeuron, intermediaryCategoryNeuron);
									intermediaryCategoryNeuron->memoryTrace = frontNeuron->memoryTrace + 1; //CHECKTHIS
									#ifdef ANN_ALGORITHM_CLASSIFICATION_NETWORK_PRUNING_OPTIMISE
									neuronUsageList.insert(pair<int, ANNneuron*>(intermediaryCategoryNeuron->memoryTrace, intermediaryCategoryNeuron));
									#endif
	
								}
							}
							
							//now update the inputValuesCategoryFound list before recursing
							vector<bool>::iterator inputValuesCategoryFoundIter = inputValuesCategoryFound->begin();
							for(vector<bool>::iterator inputValuesCategoryFoundTempIter = inputValuesCategoryFoundTemp.begin(); inputValuesCategoryFoundTempIter != inputValuesCategoryFoundTemp.end(); inputValuesCategoryFoundTempIter++)
							{
								if(*inputValuesCategoryFoundTempIter)
								{
									*inputValuesCategoryFoundIter = true;
								}
								inputValuesCategoryFoundIter++;
							}	

							#ifdef ANN_DEBUG_ALGORITHM_CLASSIFICATION_NETWORK
							cout << "at3" << endl;
							#endif
								
						}
					}
					else
					{
						if(foundCategoriesForExperience || (createIntermediaryNeuronsStage == 4))	//NB if this criterion fails, then the above level can be accessed via another route (ie via another back/input neuron)
						{
							//recurse
							findCategoriesForExperienceWrapper(frontNeuron, inputValuesCategoryFound, experienceClassificationTopLevelCategoryNeuron, createIntermediaryNeuronsStage);
						}
					}
				}
			}
		}
	}
	#ifdef ANN_DEBUG_ALGORITHM_CLASSIFICATION_NETWORK
	cout << "findCategoriesForExperienceWrapper{} end" << endl;
	#endif
	
	//return numberOfInputMatchesMax;
}

bool findCategoriesForExperience(ANNneuron* categoryNeuron, vector<bool>* inputValuesCategoryFound)
{
	#ifdef ANN_DEBUG_ALGORITHM_CLASSIFICATION_NETWORK
	cout << "findCategoriesForExperience{} start" << endl;
	#endif
		
	bool allInputNeuronsForCategoryHaveMatchingValues = true; 
	
	int i=0;
	double totalDiff = 0;
	double totalOutput = 0;	//DOING: work out how to calculate this
	for(vector<ANNneuronConnection*>::iterator backConnectionIter = categoryNeuron->backANNneuronConnectionList.begin(); backConnectionIter != categoryNeuron->backANNneuronConnectionList.end(); backConnectionIter++)
	{
		#ifdef ANN_DEBUG_ALGORITHM_CLASSIFICATION_NETWORK
		cout << "backNeuron = " << endl;
		#endif
		
		ANNneuronConnection* currentANNneuronConnection = *backConnectionIter;
		ANNneuron* backNeuron = currentANNneuronConnection->backNeuron;
		
		if(findCategoriesForExperience(backNeuron, inputValuesCategoryFound))
		{
			backNeuron->inputNeuronMatchTemp = true;
			totalDiff = totalDiff + calculateDiff(currentANNneuronConnection->idealValue, backNeuron->output);
			totalOutput = totalOutput + calculateSum(backNeuron->output);
		}
		else
		{
			backNeuron->inputNeuronMatchTemp = false;
			allInputNeuronsForCategoryHaveMatchingValues = false;
		}
		i++;
	}

	if(allInputNeuronsForCategoryHaveMatchingValues)
	{
		#ifdef ANN_DEBUG_ALGORITHM_CLASSIFICATION_NETWORK
		cout << "allInputNeuronsForCategoryHaveMatchingValues" << endl;
		#endif
		
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
					ANNneuron* backNeuron = currentANNneuronConnection->backNeuron;
					if(backNeuron->inputNeuron)
					{
						//network input neuron detected
						(*inputValuesCategoryFound)[backNeuron->orderID - 1] = true;
					}
				}
				
				/*
				double averageOutput = totalOutput/numberOfCategoryInputValues;
				categoryNeuron->output = averageOutput;
				*/
				categoryNeuron->output = totalOutput;	//using totalOutput provides greater information resolution than averageOutput
				
				//if(createIntermediaryNeuronsStage == 1)	//prevent updating memory trace too often
				//{
					categoryNeuron->memoryTrace = categoryNeuron->memoryTrace + 1;
					#ifdef ANN_ALGORITHM_CLASSIFICATION_NETWORK_PRUNING_OPTIMISE
					multimap<int, ANNneuron*>::iterator it = neuronUsageList.find(categoryNeuron);
					it.first = categoryNeuron->memoryTrace;	//update position of neuron in neuronUsageList based on its current usage
					#endif
				//}
			}
		}
	}
	
	#ifdef ANN_DEBUG_ALGORITHM_CLASSIFICATION_NETWORK
	cout << "findCategoriesForExperience{} end" << endl;
	#endif
	
	return allInputNeuronsForCategoryHaveMatchingValues;
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
	#ifdef ANN_DEBUG_ALGORITHM_CLASSIFICATION_NETWORK
	cout << "connection->idealValue = " << connection->idealValue << endl;
	#endif
	//connection->frontNeuron->memoryTrace = connection->frontNeuron->memoryTrace + 1;	//this is done in findCategoriesForExperience
}

ANNneuronConnection* connectNeurons(ANNneuron* parentNeuron, ANNneuron* childNeuron)
{
	childNeuron->hasFrontLayer = true;

	ANNneuronConnection* newANNneuronConnection = new ANNneuronConnection();
	newANNneuronConnection->backNeuron = childNeuron;
	newANNneuronConnection->frontNeuron = parentNeuron;
	childNeuron->frontANNneuronConnectionList.push_back(newANNneuronConnection);
	parentNeuron->backANNneuronConnectionList.push_back(newANNneuronConnection);
	
	if(parentNeuron == childNeuron)
	{
		cout << "connectNeurons{} error: (parentNeuron == childNeuron)" << endl;
		exit(0);
	}
	
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


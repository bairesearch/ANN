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
 * Project Version: 4a5a 06-June-2016
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
	ANNneuron* experienceClassificationneuronTypeTopLevelCategoryPrevious = NULL;
	#endif
	
	IDCounter = numberOfInputNeurons+1;
	
	for(int experienceNum = 0; experienceNum < numberOfExperiences; experienceNum++)
	{
		//#ifdef ANN_DEBUG_ALGORITHM_CLASSIFICATION_NETWORK1
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
		
		ANNneuron* experienceClassificationneuronTypeTopLevelCategory = new ANNneuron();
		experienceClassificationneuronTypeTopLevelCategory->nextNeuron = new ANNneuron();	//class architecture required to create a blank neuron
		experienceClassificationneuronTypeTopLevelCategory->neuronTypeTopLevelCategory = true;
		experienceClassificationneuronTypeTopLevelCategory->xPosRelFrac = (*numberOfOutputNeurons);
		experienceClassificationneuronTypeTopLevelCategory->yPosRelFrac = ANN_ALGORITHM_CLASSIFICATION_NETWORK_NETWORK_DISPLAY_HEIGHT;
		experienceClassificationneuronTypeTopLevelCategory->hasBackLayer = true;
		
		vector<bool> inputValuesCategoryFound(numberOfInputNeurons);
		
		bool foundCategoryNeuronsThatUseAllInputs = false;

		//OLD: YET TO DO: for stages 1->3; update ideal values
		
		int createIntermediaryNeuronsStage = 1;
		currentNeuron = firstInputNeuron;
		while(currentNeuron->nextNeuron != NULL)
		{	
			if(!foundCategoryNeuronsThatUseAllInputs)
			{
				#ifdef ANN_DEBUG_ALGORITHM_CLASSIFICATION_NETWORK1
				cout <<"\t\tcreateIntermediaryNeuronsStage = 1" << endl;
				#endif
				//vector<bool> inputValuesCategoryFoundTemp(inputValuesCategoryFound.size());
				findCategoriesForExperienceWrapper(currentNeuron, &inputValuesCategoryFound, &experienceClassificationneuronTypeTopLevelCategory, createIntermediaryNeuronsStage);
				
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
		
		//added 4a3u
		bool topLevelCategoryAlreadyExistsDeleteThisNewCopy = experienceClassificationneuronTypeTopLevelCategory->topLevelCategoryAlreadyExistsDeleteThisNewCopy;
		if(topLevelCategoryAlreadyExistsDeleteThisNewCopy)
		{
			if(foundCategoryNeuronsThatUseAllInputs)
			{
				delete(experienceClassificationneuronTypeTopLevelCategory->nextNeuron);
				experienceClassificationneuronTypeTopLevelCategory->nextNeuron = NULL;
				delete(experienceClassificationneuronTypeTopLevelCategory);
				cout << "experienceClassificationneuronTypeTopLevelCategory->topLevelCategoryAlreadyExistsDeleteThisNewCopy" << endl;
			}
			else
			{
				cout << "trainNeuralNetworkClassificationSimple{} error: experienceClassificationneuronTypeTopLevelCategory->topLevelCategoryAlreadyExistsDeleteThisNewCopy && !foundCategoryNeuronsThatUseAllInputs" << endl;
				exit(0);
			}
		}
				
		if(!foundCategoryNeuronsThatUseAllInputs)
		{
			createIntermediaryNeuronsStage = 2;
			ANNneuron* currentNeuron = firstInputNeuron;
			while(currentNeuron->nextNeuron != NULL)
			{	
				if(!foundCategoryNeuronsThatUseAllInputs)
				{
					#ifdef ANN_DEBUG_ALGORITHM_CLASSIFICATION_NETWORK1
					cout <<"\t\tcreateIntermediaryNeuronsStage = 2" << endl;
					#endif
				
					//vector<bool> inputValuesCategoryFoundTemp(inputValuesCategoryFound.size());
					findCategoriesForExperienceWrapper(currentNeuron, &inputValuesCategoryFound, &experienceClassificationneuronTypeTopLevelCategory, createIntermediaryNeuronsStage);

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
			while(currentNeuron->nextNeuron != NULL)
			{	
				if(!foundCategoryNeuronsThatUseAllInputs)
				{
					#ifdef ANN_DEBUG_ALGORITHM_CLASSIFICATION_NETWORK1
					cout <<"\t\tcreateIntermediaryNeuronsStage = 3" << endl;
					#endif
					
					//vector<bool> inputValuesCategoryFoundTemp(inputValuesCategoryFound.size());
					findCategoriesForExperienceWrapper(currentNeuron, &inputValuesCategoryFound, &experienceClassificationneuronTypeTopLevelCategory, createIntermediaryNeuronsStage);
					
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
			
			currentNeuron = firstInputNeuron;
			while(currentNeuron->nextNeuron != NULL)
			{
				resetIntermediaryNeuronCreatedThisRoundFlag(currentNeuron);
				currentNeuron = currentNeuron->nextNeuron;
			}
		}
									
		if(!foundCategoryNeuronsThatUseAllInputs)
		{
			//link experienceClassificationneuronTypeTopLevelCategory and set idealValues
			ANNneuron* currentNeuron = firstInputNeuron;
			int i = 0;
			#ifdef ANN_DEBUG_ALGORITHM_CLASSIFICATION_NETWORK1
			cout << "(!foundCategoryNeuronsThatUseAllInputs): update all relevant connections of experienceClassificationneuronTypeTopLevelCategory" << endl;
			#endif
			
			while(currentNeuron->nextNeuron != NULL)
			{	
				if(inputValuesCategoryFound[i] == false)
				{
					ANNneuronConnection* connection = connectNeurons(currentNeuron, experienceClassificationneuronTypeTopLevelCategory);
					#ifdef ANN_DEBUG_ALGORITHM_CLASSIFICATION_NETWORK1
					cout << "connection->idealValue = " << connection->idealValue << endl;
					if(experienceClassificationneuronTypeTopLevelCategory == currentNeuron)
					{
						cout << "trainNeuralNetworkClassificationSimple{} error: (experienceClassificationneuronTypeTopLevelCategory == currentNeuron)" << endl;
						exit(0);
					}
					#endif
				}
				i++;
				currentNeuron = currentNeuron->nextNeuron;
			}
		}

		
		#ifdef ANN_DEBUG_ALGORITHM_CLASSIFICATION_NETWORK5
		if(!topLevelCategoryAlreadyExistsDeleteThisNewCopy)
		{
			vector<bool> inputFound(inputValuesCategoryFound.size());
			findInputs(experienceClassificationneuronTypeTopLevelCategory, &inputFound, 0);
			bool foundAllInputs = true;
			for(int i=0; i<inputValuesCategoryFound.size(); i++)
			{
				if(!(inputFound[i]))
				{
					foundAllInputs = false;
					cout << "i = " << i << endl;
				}
			}
			if(!foundAllInputs)
			{
				cout << "main:" << endl;
				cout << "trainNeuralNetworkClassificationSimple{} error; top level category neuron is not connected to all inputs" << endl;
				exit(0);
			}
		}
		#endif

		bool alreadyAddedOutputNeuronToList = false;
		#ifdef ANN_ALGORITHM_CLASSIFICATION_NETWORK_PRUNING_OPTIMISE
		multimap<int, ANNneuron*>::iterator it = neuronUsageList.find(experienceClassificationneuronTypeTopLevelCategory);
		if(it == neuronUsageList.last())	//does not find element (see [1*])
		{
			alreadyAddedOutputNeuronToList = true;
		}
		#else
		if(createIntermediaryNeuronsStage == 1)
		{
			if(foundCategoryNeuronsThatUseAllInputs)
			{
				alreadyAddedOutputNeuronToList = true;
			}
		}
		#endif
		if(!alreadyAddedOutputNeuronToList)
		{	
			experienceClassificationneuronTypeTopLevelCategory->id = IDCounter;
			IDCounter++;
			
			#ifdef ANN_ALGORITHM_CLASSIFICATION_NETWORK_PRUNING_OPTIMISE
			neuronUsageList.insert(pair<int, ANNneuron*>(experienceClassificationneuronTypeTopLevelCategory->memoryTrace, experienceClassificationneuronTypeTopLevelCategory));
			#endif
			#ifdef ANN_ALGORITHM_CLASSIFICATION_NETWORK_STORE_OUTPUT_NEURONS
			if(firstOutputNeuronHasBeenSet)
			{	
				//cout << "experienceClassificationneuronTypeTopLevelCategoryPrevious->nextNeuron->id = " << experienceClassificationneuronTypeTopLevelCategoryPrevious->nextNeuron->id << endl;
				delete(experienceClassificationneuronTypeTopLevelCategoryPrevious->nextNeuron);
				experienceClassificationneuronTypeTopLevelCategoryPrevious->nextNeuron = experienceClassificationneuronTypeTopLevelCategory;
			}
			else
			{
				*firstOutputNeuron = experienceClassificationneuronTypeTopLevelCategory;
				firstOutputNeuronHasBeenSet = true;
			}
			experienceClassificationneuronTypeTopLevelCategoryPrevious = experienceClassificationneuronTypeTopLevelCategory;
			(*numberOfOutputNeurons) = (*numberOfOutputNeurons) + 1;
			#endif
		}
			
		#ifdef ANN_DEBUG_ALGORITHM_CLASSIFICATION_NETWORK3
		currentNeuron = firstInputNeuron;
		while(currentNeuron->nextNeuron != NULL)
		{
			checkRobustnessOfIdealValues(currentNeuron);
			currentNeuron = currentNeuron->nextNeuron;
		}
		#endif

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
		
	#ifdef ANN_ALGORITHM_CLASSIFICATION_NETWORK_PRUNING
	ANNneuron* currentNeuron = firstInputNeuron;
	while(currentNeuron->nextNeuron != NULL)
	{
		pruneNetorkBasedOnRarelyUsedNeurons(currentNeuron);
		currentNeuron = currentNeuron->nextNeuron;
	}	
	#endif
}

void resetIntermediaryNeuronCreatedThisRoundFlag(ANNneuron* categoryNeuron)
{
	#ifdef ANN_DEBUG_ALGORITHM_CLASSIFICATION_NETWORK1
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

#ifdef ANN_DEBUG_ALGORITHM_CLASSIFICATION_NETWORK3
void checkRobustnessOfIdealValues(ANNneuron* categoryNeuron)
{		
	for(vector<ANNneuronConnection*>::iterator frontConnectionIter = categoryNeuron->frontANNneuronConnectionList.begin(); frontConnectionIter != categoryNeuron->frontANNneuronConnectionList.end(); frontConnectionIter++)
	{
		ANNneuronConnection* currentANNneuronConnection = *frontConnectionIter;
		ANNneuron* frontNeuron = currentANNneuronConnection->frontNeuron;

		double sum = 0.0;
		for(vector<ANNneuronConnection*>::iterator backConnectionIter = frontNeuron->backANNneuronConnectionList.begin(); backConnectionIter != frontNeuron->backANNneuronConnectionList.end(); backConnectionIter++)
		{
			ANNneuronConnection* currentANNneuronConnection = *backConnectionIter;
			ANNneuron* backNeuron = currentANNneuronConnection->backNeuron;
			
			cout << "currentANNneuronConnection->idealValue = " << currentANNneuronConnection->idealValue << endl;
			sum = sum + currentANNneuronConnection->idealValue;
		}
		cout << "sum idealValue = " << sum << endl;
		cout << "categoryNeuron->output = " << categoryNeuron->output << endl;
		
		checkRobustnessOfIdealValues(frontNeuron);
	}
}
#endif



#ifdef ANN_ALGORITHM_CLASSIFICATION_NETWORK_PRUNING
void pruneNetorkBasedOnRarelyUsedNeurons(ANNneuron* categoryNeuron)
{
	for(vector<ANNneuronConnection*>::iterator connectionIter = categoryNeuron->frontANNneuronConnectionList.begin(); connectionIter != categoryNeuron->frontANNneuronConnectionList.end(); )
	{
		ANNneuronConnection* currentANNneuronConnection = *connectionIter;
		ANNneuron* frontNeuron = currentANNneuronConnection->frontNeuron;
		
		//cout << "af0" << endl;
		
		if(frontNeuron->memoryTrace < ANN_ALGORITHM_CLASSIFICATION_NETWORK_PRUNING_MINIMUM_MEMORY_TRACE_TO_RETAIN_CATEGORY_NEURON)
		{
			//cout << "af1" << endl;
			cout << "pruneNetorkBasedOnRarelyUsedNeurons{}: pruning neuron" << endl;
			
			#ifdef ANN_ALGORITHM_CLASSIFICATION_NETWORK_PRUNING_OPTIMISE
			multimap<int, ANNneuron*>::iterator it = neuronUsageList.find(experienceClassificationneuronTypeTopLevelCategory);
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
					//cout << "af2" << endl;
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
			
			
			//cout << "af3" << endl;
			
			connectionIter = categoryNeuron->frontANNneuronConnectionList.erase(connectionIter);
			
			//cout << "af3b" << endl;
			//delete currentANNneuronConnection;	//CREATES BUG
			
			//delete frontNeuron;	//REDUNDANT (see ANNneuronConnection::~ANNneuronConnection)
			//cout << "af4" << endl;

		}
		else
		{
			//cout << "af5" << endl;
			pruneNetorkBasedOnRarelyUsedNeurons(frontNeuron);
			//cout << "af6" << endl;
			connectionIter++;
		}
		
		//cout << "af7" << endl;

	}
	
	//cout << "af7" << endl;
}
#endif



void findCategoriesForExperienceWrapper(ANNneuron* categoryNeuron, vector<bool>* inputValuesCategoryFound, ANNneuron** experienceClassificationneuronTypeTopLevelCategory, int createIntermediaryNeuronsStage)
{
	#ifdef ANN_DEBUG_ALGORITHM_CLASSIFICATION_NETWORK1
	cout << "findCategoriesForExperienceWrapper{} start" << endl;
	#endif
		
	for(vector<ANNneuronConnection*>::iterator frontConnectionIter = categoryNeuron->frontANNneuronConnectionList.begin(); frontConnectionIter != categoryNeuron->frontANNneuronConnectionList.end(); frontConnectionIter++)
	{
		ANNneuronConnection* currentANNneuronConnectionFront = *frontConnectionIter;
		ANNneuron* frontNeuron = currentANNneuronConnectionFront->frontNeuron;
					
		if(!((*experienceClassificationneuronTypeTopLevelCategory)->topLevelCategoryAlreadyExistsDeleteThisNewCopy))
		{
			if(*experienceClassificationneuronTypeTopLevelCategory != frontNeuron)	//restored 4a3v
			{			
				if(!(frontNeuron->intermediaryNeuronCreatedThisRound))	//added 4a3h
				{
					double inputValue = categoryNeuron->output;
					if(calculateDiff(currentANNneuronConnectionFront->idealValue, inputValue) < ANN_ALGORITHM_CLASSIFICATION_NETWORK_CATEGORY_INDIVIDUAL_DIFF_THRESHOLD)	//CHECKTHIS - use the same threshold?
					{

						int createIntermediaryNeuronsStageAbove = 0;
						if((createIntermediaryNeuronsStage == ANN_ALGORITHM_CLASSIFICATION_NETWORK_STAGE_1) || (createIntermediaryNeuronsStage == ANN_ALGORITHM_CLASSIFICATION_NETWORK_STAGE_2))
						{
							createIntermediaryNeuronsStageAbove = ANN_ALGORITHM_CLASSIFICATION_NETWORK_STAGE_12above;
						}
						else if(createIntermediaryNeuronsStage == ANN_ALGORITHM_CLASSIFICATION_NETWORK_STAGE_3)
						{
							createIntermediaryNeuronsStageAbove = ANN_ALGORITHM_CLASSIFICATION_NETWORK_STAGE_3above;
						}

						ANNneuron experienceClassificationneuronTypeTopLevelCategoryTEMP;		
						ANNneuron* experienceClassificationneuronTypeTopLevelCategoryTEMPpointer = &experienceClassificationneuronTypeTopLevelCategoryTEMP;
						vector<bool> inputValuesCategoryFoundAbove(inputValuesCategoryFound->size());	//ensure that values are initialised to false
						findCategoriesForExperienceWrapper(frontNeuron, &inputValuesCategoryFoundAbove, &experienceClassificationneuronTypeTopLevelCategoryTEMPpointer, createIntermediaryNeuronsStageAbove);

						int numberOfInputMatchesMaxAbove = 0;
						for(vector<bool>::iterator inputValuesCategoryFoundAboveIter = inputValuesCategoryFoundAbove.begin(); inputValuesCategoryFoundAboveIter != inputValuesCategoryFoundAbove.end(); inputValuesCategoryFoundAboveIter++)
						{
							if(*inputValuesCategoryFoundAboveIter == true)
							{
								numberOfInputMatchesMaxAbove++;
							}
						}

						vector<bool> inputValuesCategoryFoundTemp(inputValuesCategoryFound->size());	//ensure that values are initialised to false
						bool foundCategoriesForExperience = findCategoriesForExperience(frontNeuron, &inputValuesCategoryFoundTemp, 0);

						#ifdef ANN_DEBUG_ALGORITHM_CLASSIFICATION_NETWORK1
						cout << "findCategoriesForExperience{} passed" << endl;
						#endif

						int numberOfInputMatches = 0;
						bool categoryNeuronUsesAndMatchesAllInputs = true;	//if this is true, then the frontNeuron will be the final (highest level) classification neuron for the input data
						for(vector<bool>::iterator inputValuesCategoryFoundTempIter = inputValuesCategoryFoundTemp.begin(); inputValuesCategoryFoundTempIter != inputValuesCategoryFoundTemp.end(); inputValuesCategoryFoundTempIter++)
						{
							if(*inputValuesCategoryFoundTempIter == false)
							{
								categoryNeuronUsesAndMatchesAllInputs = false;
							}
							else
							{
								numberOfInputMatches++;
							}
						}

						int numberOfBackMatches = 0;
						for(vector<ANNneuronConnection*>::iterator backConnectionIter = frontNeuron->backANNneuronConnectionList.begin(); backConnectionIter != frontNeuron->backANNneuronConnectionList.end(); backConnectionIter++)
						{
							ANNneuronConnection* currentANNneuronConnection = *backConnectionIter;
							ANNneuron* frontNeuronBackNeuron = currentANNneuronConnection->backNeuron;
							if(frontNeuronBackNeuron->backNeuronMatchTemp)
							{
								numberOfBackMatches++;
							}
						}

						//cout << "numberOfInputMatches = " << numberOfInputMatches << ", numberOfInputMatchesMaxAbove = " << numberOfInputMatchesMaxAbove << endl;

						if(numberOfInputMatches >= numberOfInputMatchesMaxAbove)	//added 4a3p
						{
							//assume that front neuron must have back neurons
								//if(frontNeuron->backANNneuronConnectionList.size() >= ANN_ALGORITHM_CLASSIFICATION_NETWORK_PRUNING_MINIMUM_MEMORY_TRACE_TO_RETAIN_CATEGORY_NEURON)
							if(numberOfBackMatches >= ANN_ALGORITHM_CLASSIFICATION_NETWORK_PRUNING_MINIMUM_MEMORY_TRACE_TO_RETAIN_CATEGORY_NEURON)
							{
								bool passedRound = false;

								if(foundCategoriesForExperience)
								{
									if(categoryNeuronUsesAndMatchesAllInputs)
									{
										if(createIntermediaryNeuronsStage == ANN_ALGORITHM_CLASSIFICATION_NETWORK_STAGE_1)
										{	
											//#ifdef ANN_DEBUG_ALGORITHM_CLASSIFICATION_NETWORK1
											cout << "\t\t (createIntermediaryNeuronsStage == 1)" << endl;
											//#endif
											#ifdef ANN_DEBUG_ALGORITHM_CLASSIFICATION_NETWORK1
											cout << "frontNeuron = " << frontNeuron << endl;
											cout << "frontNeuron->neuronTypeTopLevelCategory = " << frontNeuron->neuronTypeTopLevelCategory << endl;
											cout << "numberOfInputMatches = " << numberOfInputMatches << endl;
											#endif

											//old assumptions: frontNeuron is a top level network category neuron
											#ifndef ANN_ALGORITHM_CLASSIFICATION_NETWORK_ALLOW_FOR_CASE_WHERE_INTERMEDIARY_NEURON_IS_CONNECTED_TO_ALL_INPUTS
											if(frontNeuron->frontANNneuronConnectionList.size() == 0)
											{
											#endif
												passedRound = true;

												//top level category neuron detected
												(*experienceClassificationneuronTypeTopLevelCategory)->topLevelCategoryAlreadyExistsDeleteThisNewCopy = true;
												/*
												delete((*experienceClassificationneuronTypeTopLevelCategory)->nextNeuron);
												delete(*experienceClassificationneuronTypeTopLevelCategory);
												//set experienceClassificationneuronTypeTopLevelCategory to an existing category neuron in the network [1*]
												*experienceClassificationneuronTypeTopLevelCategory = frontNeuron;
												createIntermediaryNeuronsStage = 10;	//added 4a3k - to prevent overwriting of experienceClassificationneuronTypeTopLevelCategory
												*/
											#ifndef ANN_ALGORITHM_CLASSIFICATION_NETWORK_ALLOW_FOR_CASE_WHERE_INTERMEDIARY_NEURON_IS_CONNECTED_TO_ALL_INPUTS
											}
											else
											{
												cout << "findCategoriesForExperienceWrapper{}: error: categoryNeuronUsesAndMatchesAllInputs && (frontNeuron->frontANNneuronConnectionList.size() != 0)" << endl;
												cout << "numberOfInputMatches = " << numberOfInputMatches << endl;
												cout << "numberOfInputMatchesMaxAbove = " << numberOfInputMatchesMaxAbove << endl;
												cout << "frontNeuron->neuronTypeTopLevelCategory = " << frontNeuron->neuronTypeTopLevelCategory << endl;
												cout << "frontNeuron->neuronTypeIntermediary = " << frontNeuron->neuronTypeIntermediary << endl;
												cout << "frontNeuron->frontANNneuronConnectionList.size() = " << frontNeuron->frontANNneuronConnectionList.size() << endl;
												exit(0);
											}
											#endif
										}
									}
									else
									{
										if(createIntermediaryNeuronsStage == ANN_ALGORITHM_CLASSIFICATION_NETWORK_STAGE_2)
										{
											if(frontNeuron->neuronTypeIntermediary)
											{
												passedRound = true;

												//add the partially (input) satisfactory category neuron to experienceClassificationneuronTypeTopLevelCategory
												bool alreadyConnectedNeurons = false;
												for(vector<ANNneuronConnection*>::iterator backConnectionIter = (*experienceClassificationneuronTypeTopLevelCategory)->backANNneuronConnectionList.begin(); backConnectionIter != (*experienceClassificationneuronTypeTopLevelCategory)->backANNneuronConnectionList.end(); backConnectionIter++)
												{
													ANNneuronConnection* currentANNneuronConnection = *backConnectionIter;
													ANNneuron* backNeuron = currentANNneuronConnection->backNeuron;
													if(backNeuron == frontNeuron)
													{
														#ifdef ANN_DEBUG_ALGORITHM_CLASSIFICATION_NETWORK1
														cout << "numberOfBackMatches = " << numberOfBackMatches << endl;
														#endif
														alreadyConnectedNeurons = true;
													}
												}
												if(!alreadyConnectedNeurons)
												{
													//#ifdef ANN_DEBUG_ALGORITHM_CLASSIFICATION_NETWORK1
													cout << "\t\t (createIntermediaryNeuronsStage == 2)" << endl;
													//#endif
													
													ANNneuronConnection* connection = connectNeurons(frontNeuron, *experienceClassificationneuronTypeTopLevelCategory);
													//NOTREQUIRED: set output of experienceClassificationneuronTypeTopLevelCategory
													//(*experienceClassificationneuronTypeTopLevelCategory)->output = (*experienceClassificationneuronTypeTopLevelCategory)->output + connection->idealValue;
												
													#ifdef ANN_DEBUG_ALGORITHM_CLASSIFICATION_NETWORK5
													vector<bool> inputFound(inputValuesCategoryFound->size());
													findInputs(frontNeuron, &inputFound, 0);
													bool foundAllInputs = true;
													for(int i=0; i<inputValuesCategoryFound->size(); i++)
													{
														if(!(inputFound[i]))
														{
															foundAllInputs = false;
														}
													}
													if(foundAllInputs)
													{
														cout << "trainNeuralNetworkClassificationSimple{} error; intermediary neuron is connected to all inputs" << endl;
													}
													#endif
												}
											}
											else
											{
												cout << "findCategoriesForExperienceWrapper{} error: findCategoriesForExperience && !categoryNeuronUsesAndMatchesAllInputs && (createIntermediaryNeuronsStage == 2) && !(frontNeuron->neuronTypeIntermediary)" << endl;
												cout << "frontNeuron->neuronTypeTopLevelCategory = " << frontNeuron->neuronTypeTopLevelCategory << endl;
												cout << "frontNeuron->frontANNneuronConnectionList.size() = " << frontNeuron->frontANNneuronConnectionList.size() << endl;
												cout << "numberOfInputMatches = " << numberOfInputMatches << endl;

												#ifdef ANN_DEBUG_ALGORITHM_CLASSIFICATION_NETWORK5
												vector<bool> inputFound(inputValuesCategoryFound->size());
												findInputs(frontNeuron, &inputFound, 0);
												int numberInputsUsed = 0;
												for(int i=0; i<inputValuesCategoryFound->size(); i++)
												{
													if(inputFound[i])
													{
														numberInputsUsed++;
													}
												}
												cout << "numberInputsUsed = " << numberInputsUsed << endl;
												if(*experienceClassificationneuronTypeTopLevelCategory == frontNeuron)
												{
													cout << "error: (*experienceClassificationneuronTypeTopLevelCategory == frontNeuron)" << endl;
												}
												#endif
												exit(0);
											}
										}
									}

									if(createIntermediaryNeuronsStage == ANN_ALGORITHM_CLASSIFICATION_NETWORK_STAGE_12above)
									{
										passedRound = true;
									}	

									#ifdef ANN_DEBUG_ALGORITHM_CLASSIFICATION_NETWORK1
									cout << "at2" << endl;
									#endif
									//recurse
									//findCategoriesForExperienceWrapper(frontNeuron, inputValuesCategoryFound, experienceClassificationneuronTypeTopLevelCategory, createIntermediaryNeuronsStage);
								}
								else
								{
									if(createIntermediaryNeuronsStage == ANN_ALGORITHM_CLASSIFICATION_NETWORK_STAGE_3)
									{
										passedRound = true;

										//#ifdef ANN_DEBUG_ALGORITHM_CLASSIFICATION_NETWORK1
										cout << "\t\t (createIntermediaryNeuronsStage == 3)" << endl;
										//#endif

										//create a new intermediary category neuron for every criteria satisfied, and connect this to experienceClassificationneuronTypeTopLevelCategory
										ANNneuron* intermediaryCategoryNeuron = new ANNneuron();
										intermediaryCategoryNeuron->nextNeuron = new ANNneuron();	//class architecture required to create a blank neuron
										intermediaryCategoryNeuron->id = IDCounter;
										intermediaryCategoryNeuron->neuronTypeIntermediary = true;
										IDCounter++;
										intermediaryCategoryNeuron->hasFrontLayer = true;
										intermediaryCategoryNeuron->hasBackLayer = true;

										double frontNeuronBackNeuronXposAvg = 0.0;
										double frontNeuronBackNeuronYposAvg = 0.0;
										int numberOfBackMatchesNew = 0;
										bool foundAtLeastOneBackMismatch = false;
										double intermediaryCategoryNeuronTotalOutput = 0;
										double frontNeuronTotalOutputNew = 0;
										int intermediaryCategoryNeuronNumInputConnections = 0;
										int frontNeuronNumInputConnectionsNew = 0;

										for(vector<ANNneuronConnection*>::iterator backConnectionIter = frontNeuron->backANNneuronConnectionList.begin(); backConnectionIter != frontNeuron->backANNneuronConnectionList.end();)
										{
											ANNneuronConnection* currentANNneuronConnection = *backConnectionIter;
											ANNneuron* frontNeuronBackNeuron = currentANNneuronConnection->backNeuron;
											if(frontNeuronBackNeuron->backNeuronMatchTemp)
											{
												frontNeuronBackNeuronXposAvg = frontNeuronBackNeuronXposAvg + frontNeuronBackNeuron->xPosRelFrac;
												frontNeuronBackNeuronYposAvg = frontNeuronBackNeuronYposAvg + frontNeuronBackNeuron->yPosRelFrac;

												//modify connection to insert intermediary category neuron
												backConnectionIter = frontNeuron->backANNneuronConnectionList.erase(backConnectionIter);
												currentANNneuronConnection->frontNeuron = intermediaryCategoryNeuron;
												intermediaryCategoryNeuron->backANNneuronConnectionList.push_back(currentANNneuronConnection);
												updateConnectionIdealValue(currentANNneuronConnection);

												intermediaryCategoryNeuronTotalOutput = intermediaryCategoryNeuronTotalOutput + frontNeuronBackNeuron->output;

												numberOfBackMatchesNew++;
												intermediaryCategoryNeuronNumInputConnections++;

											}
											else
											{	
												frontNeuronTotalOutputNew = frontNeuronTotalOutputNew + frontNeuronBackNeuron->output;

												backConnectionIter++;
												foundAtLeastOneBackMismatch = true;
												frontNeuronNumInputConnectionsNew++;
											}
										}
										intermediaryCategoryNeuron->output = calculateOutput(intermediaryCategoryNeuronTotalOutput, intermediaryCategoryNeuronNumInputConnections);
										frontNeuron->output = calculateOutput(frontNeuronTotalOutputNew, frontNeuronNumInputConnectionsNew);
										//NOTREQUIRED: set output of experienceClassificationneuronTypeTopLevelCategory

										if(!foundAtLeastOneBackMismatch)
										{
											cout << "error: = !foundAtLeastOneBackMismatch" << endl;
											cout << "numberOfBackMatchesNew = " << numberOfBackMatchesNew << endl;
											exit(0);
										}

										frontNeuronBackNeuronXposAvg = frontNeuronBackNeuronXposAvg/numberOfBackMatches;
										frontNeuronBackNeuronYposAvg = frontNeuronBackNeuronYposAvg/numberOfBackMatches;

										ANNneuronConnection* connection1 = connectNeurons(intermediaryCategoryNeuron, frontNeuron);

										updateConnectionIdealValuesParent(frontNeuron);	//added 4a3t

										intermediaryCategoryNeuron->intermediaryNeuronCreatedThisRound = true;
										intermediaryCategoryNeuron->xPosRelFrac = (frontNeuronBackNeuronXposAvg + frontNeuron->xPosRelFrac)/2.0;
										intermediaryCategoryNeuron->yPosRelFrac = (frontNeuronBackNeuronYposAvg + frontNeuron->yPosRelFrac)/2.0;
										//cout << "intermediaryCategoryNeuron->xPosRelFrac = " << intermediaryCategoryNeuron->xPosRelFrac << endl;
										//cout << "intermediaryCategoryNeuron->yPosRelFrac = " << intermediaryCategoryNeuron->yPosRelFrac << endl;

										ANNneuronConnection* connection2 = connectNeurons(intermediaryCategoryNeuron, *experienceClassificationneuronTypeTopLevelCategory);


										#ifdef ANN_DEBUG_ALGORITHM_CLASSIFICATION_NETWORK5
										/*
										vector<bool> inputFound(inputValuesCategoryFound->size());
										findInputs(*experienceClassificationneuronTypeTopLevelCategory, &inputFound, 0);
										bool foundAllInputs = true;
										for(int i=0; i<inputValuesCategoryFound->size(); i++)
										{
											if(!(inputFound[i]))
											{
												foundAllInputs = false;
												cout << "i = " << i << endl;
											}
										}
										if(!foundAllInputs)
										{
											cout << "trainNeuralNetworkClassificationSimple{} error; top level category neuron is not connected to all inputs" << endl;
											exit(0);
										}
										*/
										vector<bool> inputFound(inputValuesCategoryFound->size());
										findInputs(intermediaryCategoryNeuron, &inputFound, 0);
										bool foundAllInputs = true;
										for(int i=0; i<inputValuesCategoryFound->size(); i++)
										{
											if(!(inputFound[i]))
											{
												foundAllInputs = false;
											}
										}
										if(foundAllInputs)
										{
											cout << "trainNeuralNetworkClassificationSimple{} error; intermediary neuron is connected to all inputs" << endl;
											exit(0);	
										}
										#endif

										#ifndef ANN_ALGORITHM_CLASSIFICATION_NETWORK_ALLOW_FOR_CASE_WHERE_INTERMEDIARY_NEURON_IS_CONNECTED_TO_ALL_INPUTS
										vector<bool> inputFound(inputValuesCategoryFound->size());
										findInputs(intermediaryCategoryNeuron, &inputFound, 0);
										bool foundAllInputs = true;
										for(int i=0; i<inputValuesCategoryFound->size(); i++)
										{
											if(!(inputFound[i]))
											{
												foundAllInputs = false;
											}
										}
										if(foundAllInputs)
										{
											cout << "trainNeuralNetworkClassificationSimple{} error; intermediary neuron is connected to all inputs" << endl;
											exit(0);	
										}
										#endif


											/*
											cout << "\n\tRESULTS:" << endl;
											cout << "frontNeuron->output = " << frontNeuron->output << endl;
											for(vector<ANNneuronConnection*>::iterator connectionIter = frontNeuron->backANNneuronConnectionList.begin(); connectionIter != frontNeuron->backANNneuronConnectionList.end(); connectionIter++)
											{
												ANNneuronConnection* currentANNneuronConnection = *connectionIter;
												cout << "\tcurrentANNneuronConnection->idealValue = " << currentANNneuronConnection->idealValue << endl;
												cout << "\tcurrentANNneuronConnection->backNeuron->output = " << currentANNneuronConnection->backNeuron->output << endl;
											}
											for(vector<ANNneuronConnection*>::iterator connectionIter = frontNeuron->frontANNneuronConnectionList.begin(); connectionIter != frontNeuron->frontANNneuronConnectionList.end(); connectionIter++)
											{
												ANNneuronConnection* currentANNneuronConnection2 = *connectionIter;
												cout << "\tcurrentANNneuronConnection2->idealValue = " << currentANNneuronConnection2->idealValue << endl;
												cout << "\tcurrentANNneuronConnection2->frontNeuron->output = " << currentANNneuronConnection2->frontNeuron->output << endl;
											}
											cout << "intermediaryCategoryNeuron->output = " << intermediaryCategoryNeuron->output << endl;
											for(vector<ANNneuronConnection*>::iterator connectionIter = intermediaryCategoryNeuron->backANNneuronConnectionList.begin(); connectionIter != intermediaryCategoryNeuron->backANNneuronConnectionList.end(); connectionIter++)
											{
												ANNneuronConnection* currentANNneuronConnection = *connectionIter;
												cout << "\tcurrentANNneuronConnection->idealValue = " << currentANNneuronConnection->idealValue << endl;
												cout << "\tcurrentANNneuronConnection->backNeuron->output = " << currentANNneuronConnection->backNeuron->output << endl;
											}
											for(vector<ANNneuronConnection*>::iterator connectionIter = intermediaryCategoryNeuron->frontANNneuronConnectionList.begin(); connectionIter != intermediaryCategoryNeuron->frontANNneuronConnectionList.end(); connectionIter++)
											{
												ANNneuronConnection* currentANNneuronConnection2 = *connectionIter;
												cout << "\tcurrentANNneuronConnection2->idealValue = " << currentANNneuronConnection2->idealValue << endl;
												cout << "\tcurrentANNneuronConnection2->frontNeuron->output = " << currentANNneuronConnection2->frontNeuron->output << endl;
											}
											cout << "experienceClassificationneuronTypeTopLevelCategory->output = " << (*experienceClassificationneuronTypeTopLevelCategory)->output << endl;
											for(vector<ANNneuronConnection*>::iterator connectionIter = (*experienceClassificationneuronTypeTopLevelCategory)->backANNneuronConnectionList.begin(); connectionIter != (*experienceClassificationneuronTypeTopLevelCategory)->backANNneuronConnectionList.end(); connectionIter++)
											{
												ANNneuronConnection* currentANNneuronConnection = *connectionIter;
												cout << "\tcurrentANNneuronConnection->idealValue = " << currentANNneuronConnection->idealValue << endl;
												cout << "\tcurrentANNneuronConnection->backNeuron->output = " << currentANNneuronConnection->backNeuron->output << endl;
											}
											for(vector<ANNneuronConnection*>::iterator connectionIter = (*experienceClassificationneuronTypeTopLevelCategory)->frontANNneuronConnectionList.begin(); connectionIter != (*experienceClassificationneuronTypeTopLevelCategory)->frontANNneuronConnectionList.end(); connectionIter++)
											{
												ANNneuronConnection* currentANNneuronConnection2 = *connectionIter;
												cout << "\tcurrentANNneuronConnection2->idealValue = " << currentANNneuronConnection2->idealValue << endl;
												cout << "\tcurrentANNneuronConnection2->frontNeuron->output = " << currentANNneuronConnection2->frontNeuron->output << endl;
											}
											*/


										intermediaryCategoryNeuron->memoryTrace = frontNeuron->memoryTrace + 1; //CHECKTHIS
										#ifdef ANN_ALGORITHM_CLASSIFICATION_NETWORK_PRUNING_OPTIMISE
										neuronUsageList.insert(pair<int, ANNneuron*>(intermediaryCategoryNeuron->memoryTrace, intermediaryCategoryNeuron));
										#endif
									}
									if(createIntermediaryNeuronsStage == ANN_ALGORITHM_CLASSIFICATION_NETWORK_STAGE_3above)
									{
										passedRound = true;
									}

								}

								if(passedRound)
								{
									//now update the inputValuesCategoryFound list
									vector<bool>::iterator inputValuesCategoryFoundIter = inputValuesCategoryFound->begin();
									for(vector<bool>::iterator inputValuesCategoryFoundTempIter = inputValuesCategoryFoundTemp.begin(); inputValuesCategoryFoundTempIter != inputValuesCategoryFoundTemp.end(); inputValuesCategoryFoundTempIter++)
									{
										if(*inputValuesCategoryFoundTempIter)
										{
											*inputValuesCategoryFoundIter = true;
										}
										inputValuesCategoryFoundIter++;
									}	

									#ifdef ANN_DEBUG_ALGORITHM_CLASSIFICATION_NETWORK1
									cout << "at3" << endl;
									#endif
								}
							}
						}
						else
						{
							//cout << "FF" << endl;
							//exit(0);

							if(foundCategoriesForExperience || (createIntermediaryNeuronsStage == ANN_ALGORITHM_CLASSIFICATION_NETWORK_STAGE_3above))	//NB if this criterion fails, then the above level can be accessed via another route (ie via another back/input neuron)
							{
								//recurse
								findCategoriesForExperienceWrapper(frontNeuron, inputValuesCategoryFound, experienceClassificationneuronTypeTopLevelCategory, createIntermediaryNeuronsStage);
							}
						}
					}
				}
			}
		}
	}
	#ifdef ANN_DEBUG_ALGORITHM_CLASSIFICATION_NETWORK1
	cout << "findCategoriesForExperienceWrapper{} end" << endl;
	#endif
}

string printTabsDEBUG(int level)
{
	string tabs = "";
	for(int i=0; i<level; i++)
	{
		tabs = tabs + "\t";
	}
	return tabs;
}

#ifdef ANN_ALGORITHM_CLASSIFICATION_NETWORK_ALLOW_FOR_CASE_WHERE_INTERMEDIARY_NEURON_IS_CONNECTED_TO_ALL_INPUTS
void findInputs(ANNneuron* categoryNeuron, vector<bool>* inputFound, int level)
{
	//cout << printTabsDEBUG(level) << "findInputs{} start" << endl;
		
	for(vector<ANNneuronConnection*>::iterator backConnectionIter = categoryNeuron->backANNneuronConnectionList.begin(); backConnectionIter != categoryNeuron->backANNneuronConnectionList.end(); backConnectionIter++)
	{
		ANNneuronConnection* currentANNneuronConnection = *backConnectionIter;
		ANNneuron* backNeuron = currentANNneuronConnection->backNeuron;
		
		findInputs(backNeuron, inputFound, (level+1));

		if(backNeuron->neuronTypeInput)
		{
			//cout << printTabsDEBUG(level) << "backNeuron->orderID = " << backNeuron->orderID << endl;
			(*inputFound)[backNeuron->orderID - 1] = true;
		}
	}
	
	//cout << printTabsDEBUG(level) << "findInputs{} end" << endl;
}
#endif

bool findCategoriesForExperience(ANNneuron* categoryNeuron, vector<bool>* inputValuesCategoryFound, int level)
{
	#ifdef ANN_DEBUG_ALGORITHM_CLASSIFICATION_NETWORK2
	cout << printTabsDEBUG(level) << "findCategoriesForExperience{} start" << endl;
	#endif
		
	bool allInputNeuronsForCategoryHaveMatchingValues = true; 
	
	int numberOfBackConnections = 0;
	double totalDiff = 0;
	double totalOutput = 0;	//DOING: work out how to calculate this
	for(vector<ANNneuronConnection*>::iterator backConnectionIter = categoryNeuron->backANNneuronConnectionList.begin(); backConnectionIter != categoryNeuron->backANNneuronConnectionList.end(); backConnectionIter++)
	{
		ANNneuronConnection* currentANNneuronConnection = *backConnectionIter;
		ANNneuron* backNeuron = currentANNneuronConnection->backNeuron;
		
		vector<bool> inputValuesCategoryFoundTemp(inputValuesCategoryFound->size());
		if(findCategoriesForExperience(backNeuron, &inputValuesCategoryFoundTemp, (level+1)))
		{
			backNeuron->backNeuronMatchTemp = true;
			#ifdef ANN_DEBUG_ALGORITHM_CLASSIFICATION_NETWORK2
			cout << printTabsDEBUG(level) << "backNeuron->backNeuronMatchTemp = true " << endl;
			#endif

			//if(!(backNeuron->neuronTypeInput))	//redundant
			//{
			int i=0;
			for(vector<bool>::iterator inputValuesCategoryFoundTempIter = inputValuesCategoryFoundTemp.begin(); inputValuesCategoryFoundTempIter != inputValuesCategoryFoundTemp.end(); inputValuesCategoryFoundTempIter++)
			{
				if(*inputValuesCategoryFoundTempIter == true)
				{
					(*inputValuesCategoryFound)[i] = true;
				}
				i++;
			}
			//}
		}
		else
		{
			backNeuron->backNeuronMatchTemp = false;
			#ifdef ANN_DEBUG_ALGORITHM_CLASSIFICATION_NETWORK2
			cout << printTabsDEBUG(level) << "backNeuron->backNeuronMatchTemp = false " << endl;
			#endif
			allInputNeuronsForCategoryHaveMatchingValues = false;
		}
		
		double currentDiff = calculateDiff(currentANNneuronConnection->idealValue, backNeuron->output);
		totalDiff = totalDiff + currentDiff;
		totalOutput = totalOutput + calculateSum(backNeuron->output);
		
		#ifdef ANN_DEBUG_ALGORITHM_CLASSIFICATION_NETWORK2
		cout << printTabsDEBUG(level) << "currentDiff = " << currentDiff << endl;
		cout << printTabsDEBUG(level) << "backNeuron->output = " << backNeuron->output << endl;
		cout << printTabsDEBUG(level) << "currentANNneuronConnection->idealValue = " << currentANNneuronConnection->idealValue << endl;
		#endif
		
		#ifdef ANN_DEBUG_ALGORITHM_CLASSIFICATION_NETWORK4
		if(backNeuron->neuronTypeInput)
		{
			cout << printTabsDEBUG(level) << "backNeuron->orderID = " << backNeuron->orderID << endl;
		}
		#endif
		
		numberOfBackConnections++;
	}

	if(!(categoryNeuron->neuronTypeInput))
	{	
		categoryNeuron->output = calculateOutput(totalOutput, numberOfBackConnections);
	
		#ifdef ANN_DEBUG_ALGORITHM_CLASSIFICATION_NETWORK2
		cout << printTabsDEBUG(level) << "categoryNeuron->output = " << categoryNeuron->output << endl;
		#endif				
	}	
				
				
	int numberOfCategoryInputValues = categoryNeuron->backANNneuronConnectionList.size();
	//if(numberOfCategoryInputValues > 0)
	if(!(categoryNeuron->neuronTypeInput))
	{	
		#ifdef ANN_DEBUG_ALGORITHM_CLASSIFICATION_NETWORK2
		cout << printTabsDEBUG(level) << "!(categoryNeuron->neuronTypeInput)" << endl;
		#endif
				
		if(allInputNeuronsForCategoryHaveMatchingValues)
		{
			#ifdef ANN_DEBUG_ALGORITHM_CLASSIFICATION_NETWORK2
			cout << printTabsDEBUG(level) << "allInputNeuronsForCategoryHaveMatchingValues" << endl;
			#endif

			double averageDiff = totalDiff/numberOfCategoryInputValues;
			allInputNeuronsForCategoryHaveMatchingValues = false;
			if(averageDiff < ANN_ALGORITHM_CLASSIFICATION_NETWORK_CATEGORY_AVERAGE_DIFF_THRESHOLD)
			{
				allInputNeuronsForCategoryHaveMatchingValues = true;

				for(vector<ANNneuronConnection*>::iterator backConnectionIter = categoryNeuron->backANNneuronConnectionList.begin(); backConnectionIter != categoryNeuron->backANNneuronConnectionList.end(); backConnectionIter++)
				{
					ANNneuronConnection* currentANNneuronConnection = *backConnectionIter;
					ANNneuron* backNeuron = currentANNneuronConnection->backNeuron;
					if(backNeuron->neuronTypeInput)
					{
						//network input neuron detected
						(*inputValuesCategoryFound)[backNeuron->orderID - 1] = true;
						//cout << printTabsDEBUG(level) << " " << backNeuron->orderID << " inputValuesCategoryFound = " << (*inputValuesCategoryFound)[backNeuron->orderID - 1] << " PASS" << endl;
					}
				}

				//if(createIntermediaryNeuronsStage == 1)	//prevent updating memory trace too often
				//{
					categoryNeuron->memoryTrace = categoryNeuron->memoryTrace + 1;
					#ifdef ANN_ALGORITHM_CLASSIFICATION_NETWORK_PRUNING_OPTIMISE
					multimap<int, ANNneuron*>::iterator it = neuronUsageList.find(categoryNeuron);
					it.first = categoryNeuron->memoryTrace;	//update position of neuron in neuronUsageList based on its current usage
					#endif
				//}
			}
			#ifdef ANN_DEBUG_ALGORITHM_CLASSIFICATION_NETWORK2
			else
			{
				cout << printTabsDEBUG(level) << "!(averageDiff < ANN_ALGORITHM_CLASSIFICATION_NETWORK_CATEGORY_AVERAGE_DIFF_THRESHOLD)" << endl;
				cout << printTabsDEBUG(level) << "averageDiff = " << averageDiff << endl;
			}
			#endif
		}
		else
		{
			#ifdef ANN_DEBUG_ALGORITHM_CLASSIFICATION_NETWORK2
			cout << printTabsDEBUG(level) << "!allInputNeuronsForCategoryHaveMatchingValues" << endl;
			#endif		
		}
		
		//added 4a3t
		bool allIndividualBackConnectionsAreSatisfiedDebug = true;
		for(vector<ANNneuronConnection*>::iterator backConnectionIter = categoryNeuron->backANNneuronConnectionList.begin(); backConnectionIter != categoryNeuron->backANNneuronConnectionList.end(); backConnectionIter++)
		{
			ANNneuronConnection* currentANNneuronConnection = *backConnectionIter;
			ANNneuron* backNeuron = currentANNneuronConnection->backNeuron;

			double currentDiff = calculateDiff(currentANNneuronConnection->idealValue, backNeuron->output);
			#ifdef ANN_DEBUG_ALGORITHM_CLASSIFICATION_NETWORK2
			cout << printTabsDEBUG(level) << "currentDiff = " << currentDiff << endl;
			#endif

			if(backNeuron->neuronTypeInput)
			{
				#ifdef ANN_DEBUG_ALGORITHM_CLASSIFICATION_NETWORK2
				cout << printTabsDEBUG(level) << "backNeuron->neuronTypeInput" << endl;
				#endif
				if(currentDiff < ANN_ALGORITHM_CLASSIFICATION_NETWORK_CATEGORY_INDIVIDUAL_DIFF_THRESHOLD)
				{
					backNeuron->backNeuronMatchTemp = true;	
				}
				else
				{
					backNeuron->backNeuronMatchTemp = false;
					allIndividualBackConnectionsAreSatisfiedDebug = false;
				}

				//cout << printTabsDEBUG(level) << " " << backNeuron->orderID << " backNeuron->backNeuronMatchTemp = " << backNeuron->backNeuronMatchTemp << endl;
			}

			#ifdef ANN_DEBUG_ALGORITHM_CLASSIFICATION_NETWORK2
			cout << printTabsDEBUG(level) << "backNeuron->backNeuronMatchTemp = " << backNeuron->backNeuronMatchTemp << endl;
			#endif
		}
								
	}
	#ifdef ANN_DEBUG_ALGORITHM_CLASSIFICATION_NETWORK2
	else
	{
		cout << printTabsDEBUG(level) << "(categoryNeuron->neuronTypeInput)" << endl;
	}
	#endif

	
	#ifdef ANN_DEBUG_ALGORITHM_CLASSIFICATION_NETWORK2
	cout << printTabsDEBUG(level) << "findCategoriesForExperience{} end" << endl;
	#endif
	
	return allInputNeuronsForCategoryHaveMatchingValues;
}


ANNneuronConnection* connectNeurons(ANNneuron* childNeuron, ANNneuron* parentNeuron)
{
	childNeuron->hasFrontLayer = true;
	parentNeuron->hasBackLayer = true;	//redundant

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
	
	updateConnectionIdealValue(newANNneuronConnection);
	
	return newANNneuronConnection;
}

void updateConnectionIdealValuesParent(ANNneuron* categoryNeuron)
{
	for(vector<ANNneuronConnection*>::iterator frontConnectionIter = categoryNeuron->frontANNneuronConnectionList.begin(); frontConnectionIter != categoryNeuron->frontANNneuronConnectionList.end(); frontConnectionIter++)
	{
		ANNneuronConnection* currentANNneuronConnection = *frontConnectionIter;
		ANNneuron* frontNeuron = currentANNneuronConnection->frontNeuron;
		updateConnectionIdealValue(currentANNneuronConnection);
		updateConnectionIdealValuesParent(frontNeuron);
	}
}

void updateConnectionIdealValue(ANNneuronConnection* connection)
{
	#ifdef ANN_DEBUG_ALGORITHM_CLASSIFICATION_NETWORK_DISABLE_IDEAL_VALUE_UPDATING
	//test without network updates;
	//if(connection->idealValue == 0)
	//{
		if(connection->backNeuron->neuronTypeInput)
		{
			if(connection->idealValue == 0)
			{
				connection->idealValue = connection->backNeuron->output;
			}
		}
		else
		{
			connection->idealValue = calculateIdealOutputValue(connection->backNeuron);
		}
	//}
	#else
	if(connection->backNeuron->neuronTypeInput)
	{
		connection->idealValue = connection->idealValue + (connection->backNeuron->output - connection->idealValue)/(connection->frontNeuron->memoryTrace); //nb this will evaluate to output if ideal value is 0 and memory trace is 1
	}
	else
	{
		connection->idealValue = connection->idealValue + (calculateIdealOutputValue(connection->backNeuron) - connection->idealValue)/(connection->frontNeuron->memoryTrace); //nb this will evaluate to output if ideal value is 0 and memory trace is 1
	}
	#endif
	#ifdef ANN_DEBUG_ALGORITHM_CLASSIFICATION_NETWORK1
	cout << "connection->idealValue = " << connection->idealValue << endl;
	#endif
}

double calculateIdealOutputValue(ANNneuron* categoryNeuron)
{
	double totalIdealOutputValue = 0.0;
	int i = 0;
	for(vector<ANNneuronConnection*>::iterator backConnectionIter = categoryNeuron->backANNneuronConnectionList.begin(); backConnectionIter != categoryNeuron->backANNneuronConnectionList.end(); backConnectionIter++)
	{
		ANNneuronConnection* currentANNneuronConnection = *backConnectionIter;
		totalIdealOutputValue = totalIdealOutputValue + currentANNneuronConnection->idealValue;
		i++;
	}
	double idealOutputValue = calculateOutput(totalIdealOutputValue, i);
	return idealOutputValue;
}

double calculateOutput(double totalOutput, double numberOfCategoryInputValues)
{
	#ifdef ANN_ALGORITHM_CLASSIFICATION_NETWORK_CALCULATE_OUTPUT_AS_TOTAL_RATHER_THAN_AVERAGE
	double output = totalOutput;	//NO: using totalOutput provides greater information resolution than averageOutput
	#else
	double averageOutput = totalOutput/numberOfCategoryInputValues;
	double output = averageOutput;
	#endif
	return output;			
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


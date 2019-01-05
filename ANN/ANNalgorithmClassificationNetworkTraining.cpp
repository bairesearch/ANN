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
 * File Name: ANNalgorithmClassificationNetworkTraining.cpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2018 Baxter AI (baxterai.com)
 * Project: Artificial Neural Network (ANN)
 * Project Version: 3m15a 24-April-2018
 * Comments:
 * /
 *******************************************************************************/


#include "ANNalgorithmClassificationNetworkTraining.hpp"
#ifdef ANN_ALGORITHM_CLASSIFICATION_NETWORK_DISPLAY_EVOLUTION_OF_NETWORK
#include "ANNdisplay.hpp"
#endif

#ifdef ANN_ALGORITHM_CLASSIFICATION_NETWORK

#ifdef ANN_ALGORITHM_CLASSIFICATION_NETWORK_PRUNING_OPTIMISE
multimap<int, ANNneuron*> neuronUsageList;
#endif

int64_t IDCounter;

void ANNalgorithmClassificationNetworkTrainingClass::trainNeuralNetworkClassificationSimple(ANNneuron* firstInputNeuron, ANNneuron** firstOutputNeuron, int64_t numberOfInputNeurons, int64_t* numberOfOutputNeurons, ANNexperience* firstExperienceInDataSet, int64_t numberOfExperiences)
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

		int createIntermediaryNeuronsStage = ANN_ALGORITHM_CLASSIFICATION_NETWORK_STAGE_1;
		currentNeuron = firstInputNeuron;
		while(currentNeuron->nextNeuron != NULL)
		{
			if(!foundCategoryNeuronsThatUseAllInputs)
			{
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
				cerr << "trainNeuralNetworkClassificationSimple{} error: experienceClassificationneuronTypeTopLevelCategory->topLevelCategoryAlreadyExistsDeleteThisNewCopy && !foundCategoryNeuronsThatUseAllInputs" << endl;
				exit(EXIT_ERROR);
			}
		}

		if(!foundCategoryNeuronsThatUseAllInputs)
		{
			createIntermediaryNeuronsStage = ANN_ALGORITHM_CLASSIFICATION_NETWORK_STAGE_2;
			ANNneuron* currentNeuron = firstInputNeuron;
			while(currentNeuron->nextNeuron != NULL)
			{
				if(!foundCategoryNeuronsThatUseAllInputs)
				{

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
			createIntermediaryNeuronsStage = ANN_ALGORITHM_CLASSIFICATION_NETWORK_STAGE_3;
			ANNneuron* currentNeuron = firstInputNeuron;
			while(currentNeuron->nextNeuron != NULL)
			{
				if(!foundCategoryNeuronsThatUseAllInputs)
				{

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

			while(currentNeuron->nextNeuron != NULL)
			{
				if(inputValuesCategoryFound[i] == false)
				{
					ANNneuronConnection* connection = connectNeurons(currentNeuron, experienceClassificationneuronTypeTopLevelCategory);
				}
				i++;
				currentNeuron = currentNeuron->nextNeuron;
			}
		}



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


		#ifdef ANN_ALGORITHM_CLASSIFICATION_NETWORK_DISPLAY_EVOLUTION_OF_NETWORK
		bool addSprites = true;
		bool allowRaytrace = false;
		bool displayInOpenGL = false;
		bool useOutputLDRFile = false;
		bool useOutputPPMFile = false;
		bool useOutputSVGFile = true;
		string indexString = SHAREDvars.convertIntToString(experienceNum);
		string frameFileNameBase = "neuralNetFrame" + indexString;
		string outputLDRFileName = frameFileNameBase + ".ldr";
		string outputSVGFileName = frameFileNameBase + ".svg";
		string outputPPMFileName = frameFileNameBase + ".ppm";
		string outputPPMFileNameRaytraced = "";
		string outputTALFileName = "";
		int rasterImageWidth = 1024;
		int rasterImageHeight = 768;
		ANNdisplay.outputNeuralNetworkToVectorGraphicsAndRaytrace(firstInputNeuron, addSprites, allowRaytrace, displayInOpenGL, useOutputLDRFile, useOutputSVGFile, useOutputPPMFile, outputLDRFileName, outputSVGFileName, outputPPMFileName, outputPPMFileNameRaytraced, outputTALFileName, rasterImageWidth, rasterImageHeight);
		if(experienceNum == 3)
		{
			cerr << "ANN_ALGORITHM_CLASSIFICATION_NETWORK_DISPLAY_EVOLUTION_OF_NETWORK debug: exiting early" << endl;
			exit(EXIT_ERROR);
		}
		#endif

		#ifdef ANN_VERBOSE_ALGORITHM_CLASSIFICATION_NETWORK_ENABLE_IDEAL_VALUE_UPDATING_FOR_PARENTS
		currentNeuron = firstInputNeuron;
		while(currentNeuron->nextNeuron != NULL)
		{
			resetBackNeuronMatchTemp(currentNeuron);
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
		pruneNetorkBasedOnRarelyUsedNeurons(currentNeuron, firstOutputNeuron);
		currentNeuron = currentNeuron->nextNeuron;
	}
	#endif
	#ifdef ANN_VERBOSE_ALGORITHM_CLASSIFICATION_NETWORK_MERGE_SIMILAR_NEURONS
	currentNeuron = firstInputNeuron;
	while(currentNeuron->nextNeuron != NULL)
	{
		mergeSimilarNeurons(currentNeuron, firstOutputNeuron);
		currentNeuron = currentNeuron->nextNeuron;
	}
	#endif
	#ifdef ANN_VERBOSE_ALGORITHM_CLASSIFICATION_NETWORK_CULL_REDUNDANT_NEURONS
	currentNeuron = firstInputNeuron;
	while(currentNeuron->nextNeuron != NULL)
	{
		cullRedundantNeurons(currentNeuron, NULL, firstOutputNeuron);
		currentNeuron = currentNeuron->nextNeuron;
	}
	#endif
}

void ANNalgorithmClassificationNetworkTrainingClass::resetIntermediaryNeuronCreatedThisRoundFlag(ANNneuron* categoryNeuron)
{

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

#ifdef ANN_VERBOSE_ALGORITHM_CLASSIFICATION_NETWORK_ENABLE_IDEAL_VALUE_UPDATING_FOR_PARENTS
void ANNalgorithmClassificationNetworkTrainingClass::resetBackNeuronMatchTemp(ANNneuron* categoryNeuron)
{
	for(vector<ANNneuronConnection*>::iterator frontConnectionIter = categoryNeuron->frontANNneuronConnectionList.begin(); frontConnectionIter != categoryNeuron->frontANNneuronConnectionList.end(); frontConnectionIter++)
	{
		ANNneuronConnection* currentANNneuronConnection = *frontConnectionIter;
		ANNneuron* frontNeuron = currentANNneuronConnection->frontNeuron;

		if(frontNeuron->backNeuronMatchTemp)
		{
			frontNeuron->backNeuronMatchTemp = false;
			resetBackNeuronMatchTemp(frontNeuron);
		}
	}
}
#endif







#ifdef ANN_ALGORITHM_CLASSIFICATION_NETWORK_PRUNING
void ANNalgorithmClassificationNetworkTrainingClass::pruneNetorkBasedOnRarelyUsedNeurons(ANNneuron* categoryNeuron, ANNneuron** firstOutputNeuron)
{
	for(vector<ANNneuronConnection*>::iterator connectionIter = categoryNeuron->frontANNneuronConnectionList.begin(); connectionIter != categoryNeuron->frontANNneuronConnectionList.end(); )
	{
		ANNneuronConnection* currentANNneuronConnection = *connectionIter;
		ANNneuron* frontNeuron = currentANNneuronConnection->frontNeuron;

		if(frontNeuron->memoryTrace < ANN_ALGORITHM_CLASSIFICATION_NETWORK_PRUNING_MINIMUM_MEMORY_TRACE_TO_RETAIN_CATEGORY_NEURON)
		{
			cout << "pruneNetorkBasedOnRarelyUsedNeurons{}: pruning neuron" << endl;
			deleteNeuron(frontNeuron, categoryNeuron, firstOutputNeuron);
			connectionIter = categoryNeuron->frontANNneuronConnectionList.erase(connectionIter);
		}
		else
		{
			pruneNetorkBasedOnRarelyUsedNeurons(frontNeuron, firstOutputNeuron);
			connectionIter++;
		}
	}
}
#endif

#ifdef ANN_VERBOSE_ALGORITHM_CLASSIFICATION_NETWORK_MERGE_SIMILAR_NEURONS
void ANNalgorithmClassificationNetworkTrainingClass::mergeSimilarNeurons(ANNneuron* categoryNeuron, ANNneuron** firstOutputNeuron)
{
	int i1 = 0;
	for(vector<ANNneuronConnection*>::iterator connectionIter = categoryNeuron->frontANNneuronConnectionList.begin(); connectionIter != categoryNeuron->frontANNneuronConnectionList.end(); connectionIter++)
	{
		ANNneuronConnection* currentANNneuronConnection = *connectionIter;
		ANNneuron* frontNeuron = currentANNneuronConnection->frontNeuron;

		int i2 = 0;
		for(vector<ANNneuronConnection*>::iterator connectionIter2 = categoryNeuron->frontANNneuronConnectionList.begin(); connectionIter2 != categoryNeuron->frontANNneuronConnectionList.end(); )
		{
			ANNneuronConnection* currentANNneuronConnection2 = *connectionIter2;
			ANNneuron* frontNeuron2 = currentANNneuronConnection2->frontNeuron;
			bool mergeNeuron = false;
			if(frontNeuron2 != frontNeuron)
			{
				int numberOfMatches = 0;
				int numberOfMismatches = 0;
				for(vector<ANNneuronConnection*>::iterator backConnectionIter = frontNeuron->backANNneuronConnectionList.begin(); backConnectionIter != frontNeuron->backANNneuronConnectionList.end(); backConnectionIter++)
				{
					ANNneuronConnection* currentANNneuronConnectionBack = *backConnectionIter;
					ANNneuron* frontNeuronBackNeuron = currentANNneuronConnectionBack->backNeuron;

					for(vector<ANNneuronConnection*>::iterator backConnectionIter2 = frontNeuron2->backANNneuronConnectionList.begin(); backConnectionIter2 != frontNeuron2->backANNneuronConnectionList.end(); backConnectionIter2++)
					{
						ANNneuronConnection* currentANNneuronConnectionBack2 = *backConnectionIter2;
						ANNneuron* frontNeuronBackNeuron2 = currentANNneuronConnectionBack2->backNeuron;
						if(frontNeuronBackNeuron2 == frontNeuronBackNeuron)
						{
							if(SHAREDvars.absDouble(currentANNneuronConnectionBack2->idealValue - currentANNneuronConnectionBack->idealValue) < ANN_VERBOSE_ALGORITHM_CLASSIFICATION_NETWORK_MERGE_SIMILAR_NEURONS_THRESHOLD)
							{
								numberOfMatches++;
								currentANNneuronConnectionBack->tempIdealValue = currentANNneuronConnectionBack->idealValue;
							}
							else
							{
								numberOfMismatches++;
							}
						}
					}
				}
				if((numberOfMatches >= 2) && (numberOfMismatches == 0))
				{
					cout << "ANN_VERBOSE_ALGORITHM_CLASSIFICATION_NETWORK_MERGE_SIMILAR_NEURONS{}: mergeSimilarNeurons - merging identical neurons" << endl;
					mergeNeuron = true;
				}
			}
			if(mergeNeuron)
			{
				if(i2 < i1)
				{
					cerr << "ANN_VERBOSE_ALGORITHM_CLASSIFICATION_NETWORK_MERGE_SIMILAR_NEURONS{}: mergeSimilarNeurons error (i2 < i1)" << endl;
					exit(EXIT_ERROR);
				}

				int numberOfMismatches = 0;
				for(vector<ANNneuronConnection*>::iterator backConnectionIter = frontNeuron->backANNneuronConnectionList.begin(); backConnectionIter != frontNeuron->backANNneuronConnectionList.end(); backConnectionIter++)
				{
					ANNneuronConnection* currentANNneuronConnectionBack = *backConnectionIter;
					currentANNneuronConnectionBack->idealValue = (currentANNneuronConnectionBack->idealValue + currentANNneuronConnectionBack->tempIdealValue)/2.0;
				}

				deleteNeuron(frontNeuron2, categoryNeuron, firstOutputNeuron);
				connectionIter2 = categoryNeuron->frontANNneuronConnectionList.erase(connectionIter2);
			}
			else
			{
				connectionIter2++;
			}
			i2++;
		}

		i1++;
		mergeSimilarNeurons(frontNeuron, firstOutputNeuron);
	}
}
#endif

#ifdef ANN_VERBOSE_ALGORITHM_CLASSIFICATION_NETWORK_CULL_REDUNDANT_NEURONS
//remove neurons which are being connected to an intermediary neuron and its child only
//LIMITATION: only checks grand children (not great grand children) - verify whether this is a practical limitation (it may not be)
bool ANNalgorithmClassificationNetworkTrainingClass::cullRedundantNeurons(ANNneuron* categoryNeuron, ANNneuron* parentNeuron, ANNneuron** firstOutputNeuron)
{
	bool neuronIsRedundant = false;

	for(vector<ANNneuronConnection*>::iterator backConnectionIter = categoryNeuron->backANNneuronConnectionList.begin(); backConnectionIter != categoryNeuron->backANNneuronConnectionList.end(); )
	{
		ANNneuronConnection* currentANNneuronConnectionBack = *backConnectionIter;
		ANNneuron* backNeuron = currentANNneuronConnectionBack->backNeuron;		//assume this is the child neuron
		bool neuronIsBeingConnectedToAnIntermediaryNeuronAndItsChild = false;
		bool foundIntermediaryNeuronThatIsNotConnectedToTheChild = false;

		for(vector<ANNneuronConnection*>::iterator backConnectionIter2 = categoryNeuron->backANNneuronConnectionList.begin(); backConnectionIter2 != categoryNeuron->backANNneuronConnectionList.end(); backConnectionIter2++)
		{
			ANNneuronConnection* currentANNneuronConnectionBack2 = *backConnectionIter2;
			ANNneuron* backNeuron2 = currentANNneuronConnectionBack2->backNeuron;
			if(backNeuron2 != backNeuron)
			{
				for(vector<ANNneuronConnection*>::iterator backConnectionIter2back = backNeuron2->backANNneuronConnectionList.begin(); backConnectionIter2back != backNeuron2->backANNneuronConnectionList.end(); backConnectionIter2back++)
				{
					ANNneuronConnection* currentANNneuronConnectionBack2Back = *backConnectionIter2back;
					ANNneuron* backNeuron2back = currentANNneuronConnectionBack2Back->backNeuron;
					if(backNeuron2back == backNeuron)
					{
						neuronIsBeingConnectedToAnIntermediaryNeuronAndItsChild = true;
					}
				}
				if(!neuronIsBeingConnectedToAnIntermediaryNeuronAndItsChild)
				{
					foundIntermediaryNeuronThatIsNotConnectedToTheChild = true;
				}
			}
		}

		if(neuronIsBeingConnectedToAnIntermediaryNeuronAndItsChild && foundIntermediaryNeuronThatIsNotConnectedToTheChild)
		{
			//just cull the redundant connection
			cout << "ANN_VERBOSE_ALGORITHM_CLASSIFICATION_NETWORK_CULL_REDUNDANT_NEURONS{}: cullRedundantNeurons - culling redundant connection" << endl;
			backConnectionIter = categoryNeuron->backANNneuronConnectionList.erase(backConnectionIter);
		}
		else
		{
			if(neuronIsBeingConnectedToAnIntermediaryNeuronAndItsChild && !foundIntermediaryNeuronThatIsNotConnectedToTheChild)
			{
				neuronIsRedundant = true;
			}
			backConnectionIter++;
		}
	}

	if(neuronIsRedundant)
	{
		cout << "ANN_VERBOSE_ALGORITHM_CLASSIFICATION_NETWORK_CULL_REDUNDANT_NEURONS{}: cullRedundantNeurons - culling redundant neuron" << endl;
		deleteNeuron(categoryNeuron, parentNeuron, firstOutputNeuron);
		cout << "deleted neuron" << endl;
	}
	else
	{
		for(vector<ANNneuronConnection*>::iterator connectionIter = categoryNeuron->frontANNneuronConnectionList.begin(); connectionIter != categoryNeuron->frontANNneuronConnectionList.end(); )
		{
			ANNneuronConnection* currentANNneuronConnection = *connectionIter;
			ANNneuron* frontNeuron = currentANNneuronConnection->frontNeuron;
			if(cullRedundantNeurons(frontNeuron, categoryNeuron, firstOutputNeuron))
			{
				connectionIter = categoryNeuron->frontANNneuronConnectionList.erase(connectionIter);
			}
			else
			{
				connectionIter++;
			}
		}
	}

	return neuronIsRedundant;
}
#endif

void ANNalgorithmClassificationNetworkTrainingClass::deleteNeuron(ANNneuron* frontNeuron, ANNneuron* categoryNeuron, ANNneuron** firstOutputNeuron)
{
	#ifdef ANN_ALGORITHM_CLASSIFICATION_NETWORK_PRUNING_OPTIMISE
	multimap<int, ANNneuron*>::iterator it = neuronUsageList.find(experienceClassificationneuronTypeTopLevelCategory);
	if(it != neuronUsageList.last())	//does not find element (see [1*])
	{
		neuronUsageList.erase(it);
	}
	#endif

	for(vector<ANNneuronConnection*>::iterator connectionIter2 = frontNeuron->backANNneuronConnectionList.begin(); connectionIter2 != frontNeuron->backANNneuronConnectionList.end(); connectionIter2++)
	{
		ANNneuronConnection* currentANNneuronConnection2 = *connectionIter2;
		ANNneuron* frontNeuronBackNeuron = currentANNneuronConnection2->backNeuron;
		if(frontNeuronBackNeuron != categoryNeuron)	//prevent deleting currentANNneuronConnection (do this later)
		{
			for(vector<ANNneuronConnection*>::iterator connectionIter3 = frontNeuronBackNeuron->frontANNneuronConnectionList.begin(); connectionIter3 != frontNeuronBackNeuron->frontANNneuronConnectionList.end(); )
			{
				if(*connectionIter3 == *connectionIter2)
				{
					connectionIter3 = frontNeuronBackNeuron->frontANNneuronConnectionList.erase(connectionIter3);
				}
				else
				{
					connectionIter3++;
				}
			}
		}
		updateConnectionIdealValuesParent(frontNeuronBackNeuron, false);
	}

	for(vector<ANNneuronConnection*>::iterator connectionIter2 = frontNeuron->frontANNneuronConnectionList.begin(); connectionIter2 != frontNeuron->frontANNneuronConnectionList.end(); connectionIter2++)
	{
		ANNneuronConnection* currentANNneuronConnection2 = *connectionIter2;
		ANNneuron* frontNeuronFrontNeuron = currentANNneuronConnection2->frontNeuron;

		for(vector<ANNneuronConnection*>::iterator connectionIter3 = frontNeuronFrontNeuron->backANNneuronConnectionList.begin(); connectionIter3 != frontNeuronFrontNeuron->backANNneuronConnectionList.end(); )
		{
			if(*connectionIter3 == *connectionIter2)
			{
				connectionIter3 = frontNeuronFrontNeuron->backANNneuronConnectionList.erase(connectionIter3);
			}
			else
			{
				connectionIter3++;
			}
		}
	}

	if(frontNeuron->neuronTypeTopLevelCategory)
	{
		//remove frontNeuron from outputNeuron list
		ANNneuron* currentOutputNeuron = *firstOutputNeuron;
		ANNneuron* previousOutputNeuron = NULL;
		while(currentOutputNeuron->nextNeuron != NULL)
		{
			if(currentOutputNeuron == frontNeuron)
			{
				if(previousOutputNeuron == NULL)
				{
					*firstOutputNeuron = currentOutputNeuron->nextNeuron;
				}
				else
				{
					previousOutputNeuron->nextNeuron = currentOutputNeuron->nextNeuron;
				}
			}
			else
			{
				previousOutputNeuron = currentOutputNeuron;
			}
			currentOutputNeuron = currentOutputNeuron->nextNeuron;
		}
	}
	delete frontNeuron;
}

void ANNalgorithmClassificationNetworkTrainingClass::findCategoriesForExperienceWrapper(ANNneuron* categoryNeuron, vector<bool>* inputValuesCategoryFound, ANNneuron** experienceClassificationneuronTypeTopLevelCategory, int createIntermediaryNeuronsStage)
{

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


						if(numberOfInputMatches >= numberOfInputMatchesMaxAbove)	//added 4a3p
						{
							//assume that front neuron must have back neurons
								//if(frontNeuron->backANNneuronConnectionList.size() >= ANN_ALGORITHM_CLASSIFICATION_NETWORK_MINIMUM_NUMBER_INPUTS_FOR_CATEGORY_NEURON)
							if(numberOfBackMatches >= ANN_ALGORITHM_CLASSIFICATION_NETWORK_MINIMUM_NUMBER_INPUTS_FOR_CATEGORY_NEURON)
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
												cerr << "findCategoriesForExperienceWrapper{}: error: categoryNeuronUsesAndMatchesAllInputs && (frontNeuron->frontANNneuronConnectionList.size() != 0)" << endl;
												cerr << "numberOfInputMatches = " << numberOfInputMatches << endl;
												cerr << "numberOfInputMatchesMaxAbove = " << numberOfInputMatchesMaxAbove << endl;
												cerr << "frontNeuron->neuronTypeTopLevelCategory = " << frontNeuron->neuronTypeTopLevelCategory << endl;
												cerr << "frontNeuron->neuronTypeIntermediary = " << frontNeuron->neuronTypeIntermediary << endl;
												cerr << "frontNeuron->frontANNneuronConnectionList.size() = " << frontNeuron->frontANNneuronConnectionList.size() << endl;
												exit(EXIT_ERROR);
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

												}
											}
											else
											{
												cerr << "findCategoriesForExperienceWrapper{} error: findCategoriesForExperience && !categoryNeuronUsesAndMatchesAllInputs && (createIntermediaryNeuronsStage == 2) && !(frontNeuron->neuronTypeIntermediary)" << endl;
												cerr << "frontNeuron->neuronTypeTopLevelCategory = " << frontNeuron->neuronTypeTopLevelCategory << endl;
												cerr << "frontNeuron->frontANNneuronConnectionList.size() = " << frontNeuron->frontANNneuronConnectionList.size() << endl;
												cerr << "numberOfInputMatches = " << numberOfInputMatches << endl;

												exit(EXIT_ERROR);
											}
										}
									}

									if(createIntermediaryNeuronsStage == ANN_ALGORITHM_CLASSIFICATION_NETWORK_STAGE_12above)
									{
										passedRound = true;
									}

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

										/*
										//DEBUG:
										int numberInputsFoundTemp = 0;
										for(vector<bool>::iterator inputValuesCategoryFoundTempIter = inputValuesCategoryFoundTemp.begin(); inputValuesCategoryFoundTempIter != inputValuesCategoryFoundTemp.end(); inputValuesCategoryFoundTempIter++)
										{
											if(*inputValuesCategoryFoundTempIter)
											{
												numberInputsFoundTemp++;
											}
										}
										cerr << "numberOfInputMatches = " << numberOfInputMatches << endl;
										cerr << "numberInputsFoundTemp = " << numberInputsFoundTemp << endl;
										//exit(EXIT_ERROR);
										*/


										if(!foundAtLeastOneBackMismatch)
										{
											cerr << "error: = !foundAtLeastOneBackMismatch" << endl;
											cerr << "numberOfBackMatchesNew = " << numberOfBackMatchesNew << endl;
											exit(EXIT_ERROR);
										}
										if(numberOfBackMatchesNew < 2)
										{
											cerr << "error: (numberOfBackMatchesNew < 2)" << endl;
											exit(EXIT_ERROR);
										}

										frontNeuronBackNeuronXposAvg = frontNeuronBackNeuronXposAvg/numberOfBackMatchesNew;
										frontNeuronBackNeuronYposAvg = frontNeuronBackNeuronYposAvg/numberOfBackMatchesNew;

										ANNneuronConnection* connection1 = connectNeurons(intermediaryCategoryNeuron, frontNeuron);

										//updateConnectionIdealValuesParent(frontNeuron);	//added 4a3t, removed 4a11b

										intermediaryCategoryNeuron->intermediaryNeuronCreatedThisRound = true;
										intermediaryCategoryNeuron->xPosRelFrac = (frontNeuronBackNeuronXposAvg + frontNeuron->xPosRelFrac)/2.0;
										intermediaryCategoryNeuron->yPosRelFrac = (frontNeuronBackNeuronYposAvg + frontNeuron->yPosRelFrac)/2.0;

										ANNneuronConnection* connection2 = connectNeurons(intermediaryCategoryNeuron, *experienceClassificationneuronTypeTopLevelCategory);

										intermediaryCategoryNeuron->backNeuronMatchTemp = true;
										updateConnectionIdealValuesChildren(intermediaryCategoryNeuron, true);	//added 4a11b




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
											cerr << "trainNeuralNetworkClassificationSimple{} error; intermediary neuron is connected to all inputs" << endl;
											exit(EXIT_ERROR);
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
								}
							}
						}
						else
						{
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
}

string ANNalgorithmClassificationNetworkTrainingClass::printTabsDEBUG(int level)
{
	string tabs = "";
	for(int i=0; i<level; i++)
	{
		tabs = tabs + "\t";
	}
	return tabs;
}

#ifdef ANN_ALGORITHM_CLASSIFICATION_NETWORK_ALLOW_FOR_CASE_WHERE_INTERMEDIARY_NEURON_IS_CONNECTED_TO_ALL_INPUTS
void ANNalgorithmClassificationNetworkTrainingClass::findInputs(ANNneuron* categoryNeuron, vector<bool>* inputFound, int level)
{

	for(vector<ANNneuronConnection*>::iterator backConnectionIter = categoryNeuron->backANNneuronConnectionList.begin(); backConnectionIter != categoryNeuron->backANNneuronConnectionList.end(); backConnectionIter++)
	{
		ANNneuronConnection* currentANNneuronConnection = *backConnectionIter;
		ANNneuron* backNeuron = currentANNneuronConnection->backNeuron;

		findInputs(backNeuron, inputFound, (level+1));

		if(backNeuron->neuronTypeInput)
		{

			(*inputFound)[backNeuron->orderID - 1] = true;
		}
	}

}
#endif

bool ANNalgorithmClassificationNetworkTrainingClass::findCategoriesForExperience(ANNneuron* categoryNeuron, vector<bool>* inputValuesCategoryFound, int level)
{

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
			allInputNeuronsForCategoryHaveMatchingValues = false;
		}

		double currentDiff = calculateDiff(currentANNneuronConnection->idealValue, backNeuron->output);
		totalDiff = totalDiff + currentDiff;
		totalOutput = totalOutput + calculateSum(backNeuron->output);



		numberOfBackConnections++;
	}

	if(!(categoryNeuron->neuronTypeInput))
	{
		categoryNeuron->output = calculateOutput(totalOutput, numberOfBackConnections);

	}


	int numberOfCategoryInputValues = categoryNeuron->backANNneuronConnectionList.size();
	//if(numberOfCategoryInputValues > 0)
	if(!(categoryNeuron->neuronTypeInput))
	{

		if(allInputNeuronsForCategoryHaveMatchingValues)
		{

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
		}
		else
		{
		}

		//added 4a3t
		bool allIndividualBackConnectionsAreSatisfiedDebug = true;
		for(vector<ANNneuronConnection*>::iterator backConnectionIter = categoryNeuron->backANNneuronConnectionList.begin(); backConnectionIter != categoryNeuron->backANNneuronConnectionList.end(); backConnectionIter++)
		{
			ANNneuronConnection* currentANNneuronConnection = *backConnectionIter;
			ANNneuron* backNeuron = currentANNneuronConnection->backNeuron;

			double currentDiff = calculateDiff(currentANNneuronConnection->idealValue, backNeuron->output);

			if(backNeuron->neuronTypeInput)
			{
				if(currentDiff < ANN_ALGORITHM_CLASSIFICATION_NETWORK_CATEGORY_INDIVIDUAL_DIFF_THRESHOLD)
				{
					backNeuron->backNeuronMatchTemp = true;
					(*inputValuesCategoryFound)[backNeuron->orderID - 1] = true;	//added ANN3i9b (changed ANN3i8b implementation) - when creating an intermediary neuron, prevent direct connections being added between input neurons and the new experience neuron
				}
				else
				{
					backNeuron->backNeuronMatchTemp = false;
					allIndividualBackConnectionsAreSatisfiedDebug = false;
				}

			}

		}

	}



	return allInputNeuronsForCategoryHaveMatchingValues;
}


ANNneuronConnection* ANNalgorithmClassificationNetworkTrainingClass::connectNeurons(ANNneuron* childNeuron, ANNneuron* parentNeuron)
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
		cerr << "connectNeurons{} error: (parentNeuron == childNeuron)" << endl;
		exit(EXIT_ERROR);
	}

	updateConnectionIdealValue(newANNneuronConnection);

	return newANNneuronConnection;
}

void ANNalgorithmClassificationNetworkTrainingClass::updateConnectionIdealValuesParent(ANNneuron* categoryNeuron, bool updateIdealValues)
{
	for(vector<ANNneuronConnection*>::iterator frontConnectionIter = categoryNeuron->frontANNneuronConnectionList.begin(); frontConnectionIter != categoryNeuron->frontANNneuronConnectionList.end(); frontConnectionIter++)
	{
		ANNneuronConnection* currentANNneuronConnection = *frontConnectionIter;
		ANNneuron* frontNeuron = currentANNneuronConnection->frontNeuron;
		#ifdef ANN_VERBOSE_ALGORITHM_CLASSIFICATION_NETWORK_ENABLE_IDEAL_VALUE_UPDATING_FOR_PARENTS
		if(frontNeuron->backNeuronMatchTemp && updateIdealValues)
		{
			frontNeuron->backNeuronMatchTemp = false;
			updateConnectionIdealValue(currentANNneuronConnection);
		}
		else
		{
		#endif
			updateConnectionIdealValueNoUpdating(currentANNneuronConnection);
		#ifdef ANN_VERBOSE_ALGORITHM_CLASSIFICATION_NETWORK_ENABLE_IDEAL_VALUE_UPDATING_FOR_PARENTS
		}
		#endif
		updateConnectionIdealValuesParent(frontNeuron, updateIdealValues);
	}
}

void ANNalgorithmClassificationNetworkTrainingClass::updateConnectionIdealValuesChildren(ANNneuron* categoryNeuron, bool updateIdealValues)
{
	if(!updateIdealValues || categoryNeuron->backNeuronMatchTemp)
	{
		for(vector<ANNneuronConnection*>::iterator backConnectionIter = categoryNeuron->backANNneuronConnectionList.begin(); backConnectionIter != categoryNeuron->backANNneuronConnectionList.end(); backConnectionIter++)
		{
			ANNneuronConnection* currentANNneuronConnection = *backConnectionIter;
			ANNneuron* backNeuron = currentANNneuronConnection->backNeuron;

			updateConnectionIdealValuesChildren(backNeuron, updateIdealValues);
		}

		updateConnectionIdealValuesParent(categoryNeuron, updateIdealValues);
	}
}

void ANNalgorithmClassificationNetworkTrainingClass::updateConnectionIdealValue(ANNneuronConnection* connection)
{
	#ifndef ANN_VERBOSE_ALGORITHM_CLASSIFICATION_NETWORK_ENABLE_IDEAL_VALUE_UPDATING
	//test without network updates;
	//if(connection->idealValue == 0)
	//{
		updateConnectionIdealValueNoUpdating(connection);
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
}

void ANNalgorithmClassificationNetworkTrainingClass::updateConnectionIdealValueNoUpdating(ANNneuronConnection* connection)
{
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
}

double ANNalgorithmClassificationNetworkTrainingClass::calculateIdealOutputValue(ANNneuron* categoryNeuron)
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

double ANNalgorithmClassificationNetworkTrainingClass::calculateOutput(double totalOutput, double numberOfCategoryInputValues)
{
	#ifdef ANN_ALGORITHM_CLASSIFICATION_NETWORK_CALCULATE_OUTPUT_AS_TOTAL_RATHER_THAN_AVERAGE
	double output = totalOutput;	//NO: using totalOutput provides greater information resolution than averageOutput
	#else
	double averageOutput = totalOutput/numberOfCategoryInputValues;
	double output = averageOutput;
	#endif
	return output;
}




double ANNalgorithmClassificationNetworkTrainingClass::calculateDiff(double idealValue, double value)
{
	double diff = abs(value - idealValue);
	return diff;
}

double ANNalgorithmClassificationNetworkTrainingClass::calculateSum(double value)
{
	double sum = abs(value); //verify this
	return sum;
}

//verify this

#endif


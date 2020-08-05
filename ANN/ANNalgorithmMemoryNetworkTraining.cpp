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
 * File Name: ANNalgorithmMemoryNetworkTraining.cpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2020 Baxter AI (baxterai.com)
 * Project: Artificial Neural Network (ANN)
 * Project Version: 3n4a 02-June-2020
 * Comments:
 * /
 *******************************************************************************/


#include "ANNalgorithmMemoryNetworkTraining.hpp"

#ifdef ANN_ALGORITHM_MEMORY_NETWORK


void ANNalgorithmMemoryNetworkTrainingClass::trainNeuralNetworkMemorySimple(ANNneuron* firstInputNeuron, const ANNneuron* firstOutputNeuron, const int64_t numberOfInputNeurons, const int64_t numberOfOutputNeurons, ANNexperience* firstExperienceInDataSet, const int64_t numberOfExperiences)
{
	/*
	network structure

	X input neurons
	arbitary number hidden layers/subnets with arbitary number hidden neurons with
	Y output neurons
	*/

	#ifndef ANN_ALGORITHM_SEPARATE_CLASSIFICATION_AND_MEMORY_NETWORKS_OPTIMISE_CLASSIFICATION_NET
	resetNeuralNetworkWithRandomBiasAndWeightsAndEraseMemoryTrace(firstInputNeuron);
	#endif

	int numberOfExperiencesTrain = 0;
	int numberOfExperiencesTest = 0;

	//training
	ANNexperience* currentExperience = firstExperienceInDataSet;
	for(int experienceNum = 0; experienceNum < numberOfExperiences; experienceNum++)
	{
		resetInputs(firstInputNeuron, numberOfInputNeurons, currentExperience);
		string trainingClassificationResult = "";	//NOT USED
		double trainingMemoryResult = 0.0;	//NOT USED
		ANNalgorithmMemoryNetworkUpdate.ANNclassificationAndMemoryPass(firstInputNeuron, firstOutputNeuron, &trainingClassificationResult, &trainingMemoryResult);
		currentExperience = currentExperience->next;
		numberOfExperiencesTrain++;
	}

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
		ANNalgorithmMemoryNetworkUpdate.ANNclassificationAndMemoryPass(firstInputNeuron, firstOutputNeuron, &testingClassificationResult, &testingMemoryResult);
		testingMemoryResultSum = testingMemoryResultSum + testingMemoryResult;
		currentExperience = currentExperience->next;
		numberOfExperiencesTest++;
	}

	double testingMemoryResultAverage = testingMemoryResultSum/numberOfExperiencesTest;
	cout << "testingMemoryResultAverage = " << testingMemoryResultAverage << endl;

	//resets the trained neural network to the stored copy
	restoreNeuralNetworkWithStoredMemoryTrace(firstInputNeuron);
}



void ANNalgorithmMemoryNetworkTrainingClass::trainNeuralNetworkMemory(ANNneuron* firstInputNeuron, const ANNneuron* firstOutputNeuron, const int64_t numberOfInputNeurons, const int64_t numberOfOutputNeurons, const int maxFolds, ANNexperience* firstExperienceInDataSet, const int64_t numberOfExperiences)
{
	/*
	network structure

	X input neurons
	arbitary number hidden layers/subnets with arbitary number hidden neurons with
	Y output neurons
	*/

	#ifndef ANN_ALGORITHM_SEPARATE_CLASSIFICATION_AND_MEMORY_NETWORKS_OPTIMISE_CLASSIFICATION_NET
	resetNeuralNetworkWithRandomBiasAndWeightsAndEraseMemoryTrace(firstInputNeuron);
	#endif

	for(int foldNum=0; foldNum < maxFolds; foldNum++)
	{
		cout << "foldNum = " << foldNum << endl;

		int numberOfExperiencesTrain = 0;
		int numberOfExperiencesTest = 0;

		int64_t indexOfFirstExperienceInFoldTrainPartA = 0;
		int64_t indexOfLastExperienceInFoldTrainPartA = int(float(numberOfExperiences)* float(foldNum)/float(maxFolds));
		ANNexperience* firstExperienceInFoldTrainPartA = firstExperienceInDataSet;

		int64_t indexOfFirstExperienceInFoldTrainPartB = int(float(numberOfExperiences)* float(foldNum)/float(maxFolds))+int(float(numberOfExperiences)/float(maxFolds));
		int64_t indexOfLastExperienceInFoldTrainPartB = numberOfExperiences;
		ANNexperience* firstExperienceInFoldTrainPartB = ANNexperienceClass.findExperience(firstExperienceInDataSet, indexOfFirstExperienceInFoldTrainPartB);

		int64_t indexOfFirstExperienceInFoldTestPart = (int(float(numberOfExperiences)* float(foldNum)/float(maxFolds)));
		int64_t indexOfLastExperienceInFoldTestPart = (int(float(numberOfExperiences)* float(foldNum)/float(maxFolds))+int(float(numberOfExperiences)/float(maxFolds)));
		ANNexperience* firstExperienceInFoldTestPart = ANNexperienceClass.findExperience(firstExperienceInDataSet, indexOfFirstExperienceInFoldTestPart);

		/*
		cout << "indexOfFirstExperienceInFoldTrainPartA = " << indexOfFirstExperienceInFoldTrainPartA << endl;
		cout << "indexOfLastExperienceInFoldTrainPartA = " << indexOfLastExperienceInFoldTrainPartA << endl;
		cout << "indexOfFirstExperienceInFoldTrainPartB = " << indexOfFirstExperienceInFoldTrainPartB << endl;
		cout << "indexOfLastExperienceInFoldTrainPartB = " << indexOfLastExperienceInFoldTrainPartB << endl;
		cout << "indexOfFirstExperienceInFoldTestPart = " << indexOfFirstExperienceInFoldTestPart << endl;
		cout << "indexOfLastExperienceInFoldTestPart = " << indexOfLastExperienceInFoldTestPart << endl;
		*/

		//training
		//from start of dataSet -> beginning of test segment
		ANNexperience* currentExperienceInFold;
		currentExperienceInFold = firstExperienceInFoldTrainPartA;
		for(int experienceNum = indexOfFirstExperienceInFoldTrainPartA; experienceNum < indexOfLastExperienceInFoldTrainPartA; experienceNum++)
		{
			resetInputs(firstInputNeuron, numberOfInputNeurons, currentExperienceInFold);
			string trainingClassificationResult = "";	//NOT USED
			double trainingMemoryResult = 0.0;	//NOT USED
			ANNalgorithmMemoryNetworkUpdate.ANNclassificationAndMemoryPass(firstInputNeuron, firstOutputNeuron, &trainingClassificationResult, &trainingMemoryResult);
			currentExperienceInFold = currentExperienceInFold->next;
			numberOfExperiencesTrain++;
		}
		//from end of test segment -> end of dataSet
		currentExperienceInFold = firstExperienceInFoldTrainPartB;
		for(int experienceNum = indexOfFirstExperienceInFoldTrainPartB; experienceNum<indexOfLastExperienceInFoldTrainPartB; experienceNum++)
		{
			resetInputs(firstInputNeuron, numberOfInputNeurons, currentExperienceInFold);
			string trainingClassificationResult = "";	//NOT USED
			double trainingMemoryResult = 0.0;	//NOT USED
			ANNalgorithmMemoryNetworkUpdate.ANNclassificationAndMemoryPass(firstInputNeuron, firstOutputNeuron, &trainingClassificationResult, &trainingMemoryResult);
			currentExperienceInFold = currentExperienceInFold->next;
			numberOfExperiencesTrain++;
		}

		//testing
		double testingMemoryResultSum = 0.0;
		//creates and stores copy of the trained neural network
		storeNeuralNetworkMemoryTrace(firstInputNeuron);
		//from start of test segment -> end of test segment
		currentExperienceInFold = firstExperienceInFoldTestPart;
		for(int experienceNum = indexOfFirstExperienceInFoldTestPart; experienceNum < indexOfLastExperienceInFoldTestPart; experienceNum++)
		{
			restoreNeuralNetworkWithStoredMemoryTrace(firstInputNeuron);
			resetInputs(firstInputNeuron, numberOfInputNeurons, currentExperienceInFold);
			string testingClassificationResult = "";	//NOT USED
			double testingMemoryResult = 0.0;
			ANNalgorithmMemoryNetworkUpdate.ANNclassificationAndMemoryPass(firstInputNeuron, firstOutputNeuron, &testingClassificationResult, &testingMemoryResult);
			testingMemoryResultSum = testingMemoryResultSum + testingMemoryResult;
			currentExperienceInFold = currentExperienceInFold->next;
			numberOfExperiencesTest++;
		}

		double testingMemoryResultAverage = testingMemoryResultSum/numberOfExperiencesTest;
		cout << "testingMemoryResultAverage = " << testingMemoryResultAverage << endl;

		//resets the trained neural network to the stored copy
		restoreNeuralNetworkWithStoredMemoryTrace(firstInputNeuron);
	}
}











void ANNalgorithmMemoryNetworkTrainingClass::storeNeuralNetworkMemoryTrace(ANNneuron* neuronBeingAccessed)
{

	ANNneuron* currentNeuronReference = neuronBeingAccessed;

	while(currentNeuronReference->nextNeuron != NULL)
	{

		if(neuronBeingAccessed->hasFrontLayer)	//output layer does not have front neuron connection weights or subnets [CHECK ANNTHIS; NB in ANN, an output layer of a subnet may have weights, however regardless of this, storeNeuralNetworkBiasAndWeights will take into account this, in the higher level subnet pass]
		{
			for(vector<ANNneuronConnection*>::iterator connectionIter = currentNeuronReference->frontANNneuronConnectionList.begin(); connectionIter != currentNeuronReference->frontANNneuronConnectionList.end(); connectionIter++)
			{
				ANNneuronConnection* currentANNneuronConnection = *connectionIter;

				currentANNneuronConnection->storedMemoryTrace = currentANNneuronConnection->memoryTrace;

			}

			#ifdef ANN_SUBNETS
			if(currentNeuronReference->isSubnet)
			{
				storeNeuralNetworkMemoryTrace(currentNeuronReference->firstNeuronInBackLayerOfSubnet);
			}
			#endif
		}

		currentNeuronReference = currentNeuronReference->nextNeuron;
	}

	//recursion [NB there will be unnecessay redundancy here, as the same neuron will be accessed more than once]
	if(neuronBeingAccessed->hasFrontLayer)
	{
		storeNeuralNetworkMemoryTrace(neuronBeingAccessed->firstNeuronInFrontLayer);
	}
}

void ANNalgorithmMemoryNetworkTrainingClass::restoreNeuralNetworkWithStoredMemoryTrace(ANNneuron* neuronBeingAccessed)
{

	ANNneuron* currentNeuronReference = neuronBeingAccessed;

	while(currentNeuronReference->nextNeuron != NULL)
	{

		if(neuronBeingAccessed->hasFrontLayer)	//output layer does not have front neuron connection weights or subnets [CHECK ANNTHIS; NB in ANN, an output layer of a subnet may have weights, however regardless of this, restoreNeuralNetworkWithStoredBiasAndWeights will take into account this, in the higher level subnet pass]
		{
			for(vector<ANNneuronConnection*>::iterator connectionIter = currentNeuronReference->frontANNneuronConnectionList.begin(); connectionIter != currentNeuronReference->frontANNneuronConnectionList.end(); connectionIter++)
			{
				ANNneuronConnection* currentANNneuronConnection = *connectionIter;

				currentANNneuronConnection->memoryTrace = currentANNneuronConnection->storedMemoryTrace;

			}

			#ifdef ANN_SUBNETS
			if(currentNeuronReference->isSubnet)
			{
				restoreNeuralNetworkWithStoredMemoryTrace(currentNeuronReference->firstNeuronInBackLayerOfSubnet);
			}
			#endif
		}

		currentNeuronReference = currentNeuronReference->nextNeuron;
	}

	//recursion [NB there will be unnecessay redundancy here, as the same neuron will be accessed more than once]
	if(neuronBeingAccessed->hasFrontLayer)
	{
		restoreNeuralNetworkWithStoredMemoryTrace(neuronBeingAccessed->firstNeuronInFrontLayer);
	}
}

void ANNalgorithmMemoryNetworkTrainingClass::resetNeuralNetworkWithRandomBiasAndWeightsAndEraseMemoryTrace(ANNneuron* neuronBeingAccessed)
{

	ANNneuron* currentNeuronReference = neuronBeingAccessed;

	while(currentNeuronReference->nextNeuron != NULL)
	{

		if(neuronBeingAccessed->hasBackLayer)	//input layer does not have biases [CHECK ANNTHIS; NB in ANN, an input layer to a subnet may have a bias, however regardless of this, resetNeuralNetworkWithRandomBiasAndWeightsAndEraseMemoryTrace will take into account this, in the higher level subnet pass]
		{
			#ifndef DEBUG_TH_OR_IMAGE_CATEGORISTION_NN_DO_NOT_USE_NEGATIVE_BIASES_AND_WEIGHTS
			#ifdef TH_OR_IMAGE_CATEGORISTION_NN_USE_HEAVY_RANDOMISATION_OF_BIASES_AND_WEIGHTS
			currentNeuronReference->bias = ((double(rand()* 2.0F)/(double(RAND_MAX)))-1.0F)*TH_OR_IMAGE_CATEGORISTION_NN_USE_HEAVY_RANDOMISATION_OF_BIASES_AND_WEIGHTS_BIAS_MULT;
			#else
			#ifndef DEBUG_TH_OR_IMAGE_CATEGORISTION_NN_DO_NOT_RANDOMISE_BIASES
			#ifndef DEBUG_TRAIN_NETWORK_WITH_NON_RANDOM_VARS
			#ifndef DEBUG_TH_ANN_USE_ORIGINAL_RANDOMISATION
			currentNeuronReference->bias = (double(rand()* 2.0F)/(double(RAND_MAX)))-1.0F;
			#endif
			#endif
			#endif
			#endif
			#endif

		}

		if(neuronBeingAccessed->hasFrontLayer)	//output layer does not have front neuron connection weights or subnets [CHECK ANNTHIS; NB in ANN, an output layer of a subnet may have weights, however regardless of this, resetNeuralNetworkWithRandomBiasAndWeightsAndEraseMemoryTrace will take into account this, in the higher level subnet pass]
		{
			for(vector<ANNneuronConnection*>::iterator connectionIter = currentNeuronReference->frontANNneuronConnectionList.begin(); connectionIter != currentNeuronReference->frontANNneuronConnectionList.end(); connectionIter++)
			{
				ANNneuronConnection* currentANNneuronConnection = *connectionIter;

				#ifndef DEBUG_TH_OR_IMAGE_CATEGORISTION_NN_DO_NOT_USE_NEGATIVE_BIASES_AND_WEIGHTS
				#ifdef TH_OR_IMAGE_CATEGORISTION_NN_USE_HEAVY_RANDOMISATION_OF_BIASES_AND_WEIGHTS
				currentANNneuronConnection->weight = ((double(rand()* 2.0F)/(double(RAND_MAX)))-1.0F)*TH_OR_IMAGE_CATEGORISTION_NN_USE_HEAVY_RANDOMISATION_OF_BIASES_AND_WEIGHTS_WEIGHT_MULT;
				#else
				#ifndef DEBUG_TH_OR_IMAGE_CATEGORISTION_NN_DO_NOT_RANDOMISE_WEIGHTS
				#ifndef DEBUG_TRAIN_NETWORK_WITH_NON_RANDOM_VARS
				#ifndef DEBUG_TH_ANN_USE_ORIGINAL_RANDOMISATION
				currentANNneuronConnection->weight = (double(rand()* 2.0F)/(double(RAND_MAX)))-1.0F;
				#endif
				#endif
				#endif
				#endif
				#endif


				currentANNneuronConnection->memoryTrace = 0.0;
			}

			#ifdef ANN_SUBNETS
			if(currentNeuronReference->isSubnet)
			{
				resetNeuralNetworkWithRandomBiasAndWeightsAndEraseMemoryTrace(currentNeuronReference->firstNeuronInBackLayerOfSubnet);
			}
			#endif
		}

		currentNeuronReference = currentNeuronReference->nextNeuron;
	}


	//recursion [NB there will be unnecessay redundancy here, as the same neuron will be accessed more than once]
	if(neuronBeingAccessed->hasFrontLayer)
	{
		resetNeuralNetworkWithRandomBiasAndWeightsAndEraseMemoryTrace(neuronBeingAccessed->firstNeuronInFrontLayer);
	}
}

void ANNalgorithmMemoryNetworkTrainingClass::resetInputs(ANNneuron* firstInputNeuron, const int64_t numberOfInputNeurons, ANNexperience* currentExperienceInDataSet)
{

	//sets inputData into ANN
	//(NB normalisedInputData[i][0] is target class

	ANNneuron* currentNeuronReference = firstInputNeuron;
	ANNexperienceInput* currentExperienceInputInExperience = currentExperienceInDataSet->firstExperienceInput;
	for(int64_t i = 0; i < numberOfInputNeurons; i++)
	{
		currentNeuronReference->output = currentExperienceInputInExperience->inputValue;	 //normalisedInputData[testSegment][(i+1)];
		currentNeuronReference = currentNeuronReference->nextNeuron;
		currentExperienceInputInExperience = currentExperienceInputInExperience->next;
	}

#ifdef ANN_SUBNETS
	/*CHECK ANNTHIS: anything required for subnet inputs/outputs?*/
#endif

}

#endif


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
 * File Name: ANNalgorithmMemoryNetworkTraining.cpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2017 Baxter AI (baxterai.com)
 * Project: Artificial Neural Network (ANN)
 * Project Version: 3j2a 17-January-2017
 * Comments:
 *
 *******************************************************************************/


#include "ANNalgorithmMemoryNetworkTraining.hpp"

#ifdef ANN_ALGORITHM_MEMORY_NETWORK


void ANNalgorithmMemoryNetworkTrainingClass::trainNeuralNetworkMemorySimple(ANNneuron* firstInputNeuron, const ANNneuron* firstOutputNeuron, const long numberOfInputNeurons, const long numberOfOutputNeurons, ANNexperience* firstExperienceInDataSet, const long numberOfExperiences)
{
	/*
	network structure

	X input neurons
	arbitary number hidden layers/subnets with arbitary number hidden neurons with
	Y output neurons
	*/

	#ifndef ANN_ALGORITHM_SEPARATE_CLASSIFICATION_AND_MEMORY_NETWORKS_OPTIMISE_CLASSIFICATION_NET
	this->resetNeuralNetworkWithRandomBiasAndWeightsAndEraseMemoryTrace(firstInputNeuron);
	#endif

	int numberOfExperiencesTrain = 0;
	int numberOfExperiencesTest = 0;

	//training
	ANNexperience* currentExperience = firstExperienceInDataSet;
	for(int experienceNum = 0; experienceNum < numberOfExperiences; experienceNum++)
	{
		this->resetInputs(firstInputNeuron, numberOfInputNeurons, currentExperience);
		string trainingClassificationResult = "";	//NOT USED
		double trainingMemoryResult = 0.0;	//NOT USED
		ANNalgorithmMemoryNetworkUpdate.ANNclassificationAndMemoryPass(firstInputNeuron, firstOutputNeuron, &trainingClassificationResult, &trainingMemoryResult);
		currentExperience = currentExperience->next;
		numberOfExperiencesTrain++;
	}

	//testing
	double testingMemoryResultSum = 0.0;
	//creates and stores copy of the trained neural network
	this->storeNeuralNetworkMemoryTrace(firstInputNeuron);
	//from start of test segment -> end of test segment
	currentExperience = firstExperienceInDataSet;
	for(int experienceNum = 0; experienceNum < numberOfExperiences; experienceNum++)
	{
		this->restoreNeuralNetworkWithStoredMemoryTrace(firstInputNeuron);
		this->resetInputs(firstInputNeuron, numberOfInputNeurons, currentExperience);
		string testingClassificationResult = "";	//NOT USED
		double testingMemoryResult = 0.0;
		ANNalgorithmMemoryNetworkUpdate.ANNclassificationAndMemoryPass(firstInputNeuron, firstOutputNeuron, &testingClassificationResult, &testingMemoryResult);
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
	this->restoreNeuralNetworkWithStoredMemoryTrace(firstInputNeuron);
}



void ANNalgorithmMemoryNetworkTrainingClass::trainNeuralNetworkMemory(ANNneuron* firstInputNeuron, const ANNneuron* firstOutputNeuron, const long numberOfInputNeurons, const long numberOfOutputNeurons, const int maxFolds, ANNexperience* firstExperienceInDataSet, const long numberOfExperiences)
{
	/*
	network structure

	X input neurons
	arbitary number hidden layers/subnets with arbitary number hidden neurons with
	Y output neurons
	*/

	#ifndef ANN_ALGORITHM_SEPARATE_CLASSIFICATION_AND_MEMORY_NETWORKS_OPTIMISE_CLASSIFICATION_NET
	this->resetNeuralNetworkWithRandomBiasAndWeightsAndEraseMemoryTrace(firstInputNeuron);
	#endif

	for(int foldNum=0; foldNum < maxFolds; foldNum++)
	{
		cout << "foldNum = " << foldNum << endl;

		int numberOfExperiencesTrain = 0;
		int numberOfExperiencesTest = 0;

		long indexOfFirstExperienceInFoldTrainPartA = 0;
		long indexOfLastExperienceInFoldTrainPartA = int(float(numberOfExperiences)* float(foldNum)/float(maxFolds));
		ANNexperience* firstExperienceInFoldTrainPartA = firstExperienceInDataSet;

		long indexOfFirstExperienceInFoldTrainPartB = int(float(numberOfExperiences)* float(foldNum)/float(maxFolds))+int(float(numberOfExperiences)/float(maxFolds));
		long indexOfLastExperienceInFoldTrainPartB = numberOfExperiences;
		ANNexperience* firstExperienceInFoldTrainPartB = ANNexperienceClass.findExperience(firstExperienceInDataSet, indexOfFirstExperienceInFoldTrainPartB);

		long indexOfFirstExperienceInFoldTestPart = (int(float(numberOfExperiences)* float(foldNum)/float(maxFolds)));
		long indexOfLastExperienceInFoldTestPart = (int(float(numberOfExperiences)* float(foldNum)/float(maxFolds))+int(float(numberOfExperiences)/float(maxFolds)));
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
			this->resetInputs(firstInputNeuron, numberOfInputNeurons, currentExperienceInFold);
			string trainingClassificationResult = "";	//NOT USED
			double trainingMemoryResult = 0.0;	//NOT USED
			ANNalgorithmMemoryNetworkUpdate.ANNclassificationAndMemoryPass(firstInputNeuron, firstOutputNeuron, &trainingClassificationResult, &trainingMemoryResult);
			#ifdef ANN_DEBUG
			#endif
			currentExperienceInFold = currentExperienceInFold->next;
			numberOfExperiencesTrain++;
		}
		//from end of test segment -> end of dataSet
		currentExperienceInFold = firstExperienceInFoldTrainPartB;
		for(int experienceNum = indexOfFirstExperienceInFoldTrainPartB; experienceNum<indexOfLastExperienceInFoldTrainPartB; experienceNum++)
		{
			this->resetInputs(firstInputNeuron, numberOfInputNeurons, currentExperienceInFold);
			string trainingClassificationResult = "";	//NOT USED
			double trainingMemoryResult = 0.0;	//NOT USED
			ANNalgorithmMemoryNetworkUpdate.ANNclassificationAndMemoryPass(firstInputNeuron, firstOutputNeuron, &trainingClassificationResult, &trainingMemoryResult);
			#ifdef ANN_DEBUG
			#endif
			currentExperienceInFold = currentExperienceInFold->next;
			numberOfExperiencesTrain++;
		}

		//testing
		double testingMemoryResultSum = 0.0;
		//creates and stores copy of the trained neural network
		this->storeNeuralNetworkMemoryTrace(firstInputNeuron);
		//from start of test segment -> end of test segment
		currentExperienceInFold = firstExperienceInFoldTestPart;
		for(int experienceNum = indexOfFirstExperienceInFoldTestPart; experienceNum < indexOfLastExperienceInFoldTestPart; experienceNum++)
		{
			this->restoreNeuralNetworkWithStoredMemoryTrace(firstInputNeuron);
			this->resetInputs(firstInputNeuron, numberOfInputNeurons, currentExperienceInFold);
			string testingClassificationResult = "";	//NOT USED
			double testingMemoryResult = 0.0;
			ANNalgorithmMemoryNetworkUpdate.ANNclassificationAndMemoryPass(firstInputNeuron, firstOutputNeuron, &testingClassificationResult, &testingMemoryResult);
			testingMemoryResultSum = testingMemoryResultSum + testingMemoryResult;
			#ifdef ANN_DEBUG
			//cout << "current testingMemoryResult = " << testingMemoryResult << endl;
			#endif
			currentExperienceInFold = currentExperienceInFold->next;
			numberOfExperiencesTest++;
		}

		double testingMemoryResultAverage = testingMemoryResultSum/numberOfExperiencesTest;
		cout << "testingMemoryResultAverage = " << testingMemoryResultAverage << endl;

		//resets the trained neural network to the stored copy
		this->restoreNeuralNetworkWithStoredMemoryTrace(firstInputNeuron);
	}
}











void ANNalgorithmMemoryNetworkTrainingClass::storeNeuralNetworkMemoryTrace(ANNneuron* neuronBeingAccessed)
{
	#ifdef ANN_DEBUG
	cout << "storeNeuralNetworkMemoryTrace{}:" << endl;
	#endif

	ANNneuron* currentNeuronReference = neuronBeingAccessed;

	while(currentNeuronReference->nextNeuron != NULL)
	{
		#ifdef ANN_DEBUG
		cout << "\nA neuron has been selected for recording front neuron memory trace" << endl;
		cout << "currentNeuronReference ID = " << currentNeuronReference->id << endl;
		cout << "currentNeuronReference ID Order = " << currentNeuronReference->orderID << endl;
		cout << "currentNeuronReference ID Layer = " << currentNeuronReference->layerID << endl;
		cout << "currentNeuronReference ID Subnet = " << currentNeuronReference->subnetID << endl;
		#endif

		if(neuronBeingAccessed->hasFrontLayer)	//output layer does not have front neuron connection weights or subnets [CHECK ANNTHIS; NB in ANN, an output layer of a subnet may have weights, however regardless of this, storeNeuralNetworkBiasAndWeights will take into account this, in the higher level subnet pass]
		{
			for(vector<ANNneuronConnection*>::iterator connectionIter = currentNeuronReference->frontANNneuronConnectionList.begin(); connectionIter != currentNeuronReference->frontANNneuronConnectionList.end(); connectionIter++)
			{
				ANNneuronConnection* currentANNneuronConnection = *connectionIter;

				currentANNneuronConnection->storedMemoryTrace = currentANNneuronConnection->memoryTrace;

				#ifdef ANN_DEBUG
				cout << "Front ANNneuron Connection Memory Trace = " << currentANNneuronConnection->memoryTrace << endl;
				#endif
			}

			#ifdef ANN_SUBNETS
			if(currentNeuronReference->isSubnet)
			{
				this->storeNeuralNetworkMemoryTrace(currentNeuronReference->firstNeuronInBackLayerOfSubnet);
			}
			#endif
		}

		currentNeuronReference = currentNeuronReference->nextNeuron;
	}

	//recursion [NB there will be unnecessay redundancy here, as the same neuron will be accessed more than once]
	if(neuronBeingAccessed->hasFrontLayer)
	{
		this->storeNeuralNetworkMemoryTrace(neuronBeingAccessed->firstNeuronInFrontLayer);
	}
}

void ANNalgorithmMemoryNetworkTrainingClass::restoreNeuralNetworkWithStoredMemoryTrace(ANNneuron* neuronBeingAccessed)
{
	#ifdef ANN_DEBUG
	cout << "restoreNeuralNetworkWithStoredMemoryTrace{}:" << endl;
	#endif

	ANNneuron* currentNeuronReference = neuronBeingAccessed;

	while(currentNeuronReference->nextNeuron != NULL)
	{
		#ifdef ANN_DEBUG
		cout << "\nA neuron has been selected for Reseting with previously stored front neuron memory trace" << endl;
		cout << "currentNeuronReference ID = " << currentNeuronReference->id << endl;
		cout << "currentNeuronReference ID Order = " << currentNeuronReference->orderID << endl;
		cout << "currentNeuronReference ID Layer = " << currentNeuronReference->layerID << endl;
		cout << "currentNeuronReference ID Subnet = " << currentNeuronReference->subnetID << endl;
		#endif

		if(neuronBeingAccessed->hasFrontLayer)	//output layer does not have front neuron connection weights or subnets [CHECK ANNTHIS; NB in ANN, an output layer of a subnet may have weights, however regardless of this, restoreNeuralNetworkWithStoredBiasAndWeights will take into account this, in the higher level subnet pass]
		{
			for(vector<ANNneuronConnection*>::iterator connectionIter = currentNeuronReference->frontANNneuronConnectionList.begin(); connectionIter != currentNeuronReference->frontANNneuronConnectionList.end(); connectionIter++)
			{
				ANNneuronConnection* currentANNneuronConnection = *connectionIter;

				currentANNneuronConnection->memoryTrace = currentANNneuronConnection->storedMemoryTrace;

				#ifdef ANN_DEBUG
				cout << "Front ANNneuron Connection Memory Trace = " << currentANNneuronConnection->memoryTrace << endl;
				#endif
			}

			#ifdef ANN_SUBNETS
			if(currentNeuronReference->isSubnet)
			{
				this->restoreNeuralNetworkWithStoredMemoryTrace(currentNeuronReference->firstNeuronInBackLayerOfSubnet);
			}
			#endif
		}

		currentNeuronReference = currentNeuronReference->nextNeuron;
	}

	//recursion [NB there will be unnecessay redundancy here, as the same neuron will be accessed more than once]
	if(neuronBeingAccessed->hasFrontLayer)
	{
		this->restoreNeuralNetworkWithStoredMemoryTrace(neuronBeingAccessed->firstNeuronInFrontLayer);
	}
}

void ANNalgorithmMemoryNetworkTrainingClass::resetNeuralNetworkWithRandomBiasAndWeightsAndEraseMemoryTrace(ANNneuron* neuronBeingAccessed)
{
	#ifdef ANN_DEBUG
	cout << "resetNeuralNetworkWithRandomBiasAndWeightsAndEraseMemoryTrace{}:" << endl;
	#endif

	ANNneuron* currentNeuronReference = neuronBeingAccessed;

	while(currentNeuronReference->nextNeuron != NULL)
	{
		#ifdef ANN_DEBUG
		cout << "\nA neuron has been selected for Reseting with Random Bias And front neuron connection Weights" << endl;
		cout << "currentNeuronReference ID = " << currentNeuronReference->id << endl;
		cout << "currentNeuronReference ID Order = " << currentNeuronReference->orderID << endl;
		cout << "currentNeuronReference ID Layer = " << currentNeuronReference->layerID << endl;
		cout << "currentNeuronReference ID Subnet = " << currentNeuronReference->subnetID << endl;
		#endif

		if(neuronBeingAccessed->hasBackLayer)	//input layer does not have biases [CHECK ANNTHIS; NB in ANN, an input layer to a subnet may have a bias, however regardless of this, resetNeuralNetworkWithRandomBiasAndWeightsAndEraseMemoryTrace will take into account this, in the higher level subnet pass]
		{
			#ifdef DEBUG_TH_OR_IMAGE_CATEGORISTION_NN_DO_NOT_USE_NEGATIVE_BIASES_AND_WEIGHTS
			currentNeuronReference->bias = (double(rand()* 1.0F)/(double(RAND_MAX)+0.0F));
			#else
			#ifdef TH_OR_IMAGE_CATEGORISTION_NN_USE_HEAVY_RANDOMISATION_OF_BIASES_AND_WEIGHTS
			currentNeuronReference->bias = ((double(rand()* 2.0F)/(double(RAND_MAX)))-1.0F)*TH_OR_IMAGE_CATEGORISTION_NN_USE_HEAVY_RANDOMISATION_OF_BIASES_AND_WEIGHTS_BIAS_MULT;
			#else
			#ifdef DEBUG_TH_OR_IMAGE_CATEGORISTION_NN_DO_NOT_RANDOMISE_BIASES
			currentNeuronReference->bias = 0.5;
			#else
			#ifdef DEBUG_TRAIN_NETWORK_WITH_NON_RANDOM_VARS
			currentNeuronReference->bias = (double(777* 2.0F)/(double(RAND_MAX)))-1.0F;
			#else
			#ifdef DEBUG_TH_ANN_USE_ORIGINAL_RANDOMISATION
			currentNeuronReference->bias = (double(rand()* 2.0F)/(double(RAND_MAX)+1.0F))-1.0F;
			#else
			currentNeuronReference->bias = (double(rand()* 2.0F)/(double(RAND_MAX)))-1.0F;
			#endif
			#endif
			#endif
			#endif
			#endif

			#ifdef ANN_DEBUG
			cout << "ANNneuron Bias = " << currentNeuronReference->bias << endl;
			#endif
		}

		if(neuronBeingAccessed->hasFrontLayer)	//output layer does not have front neuron connection weights or subnets [CHECK ANNTHIS; NB in ANN, an output layer of a subnet may have weights, however regardless of this, resetNeuralNetworkWithRandomBiasAndWeightsAndEraseMemoryTrace will take into account this, in the higher level subnet pass]
		{
			for(vector<ANNneuronConnection*>::iterator connectionIter = currentNeuronReference->frontANNneuronConnectionList.begin(); connectionIter != currentNeuronReference->frontANNneuronConnectionList.end(); connectionIter++)
			{
				ANNneuronConnection* currentANNneuronConnection = *connectionIter;

				#ifdef DEBUG_TH_OR_IMAGE_CATEGORISTION_NN_DO_NOT_USE_NEGATIVE_BIASES_AND_WEIGHTS
				currentANNneuronConnection->weight = (double(rand()* 1.0F)/(double(RAND_MAX)+0.0F));
				#else
				#ifdef TH_OR_IMAGE_CATEGORISTION_NN_USE_HEAVY_RANDOMISATION_OF_BIASES_AND_WEIGHTS
				currentANNneuronConnection->weight = ((double(rand()* 2.0F)/(double(RAND_MAX)))-1.0F)*TH_OR_IMAGE_CATEGORISTION_NN_USE_HEAVY_RANDOMISATION_OF_BIASES_AND_WEIGHTS_WEIGHT_MULT;
				#else
				#ifdef DEBUG_TH_OR_IMAGE_CATEGORISTION_NN_DO_NOT_RANDOMISE_WEIGHTS
				currentANNneuronConnection->weight = -0.1;
				#else
				#ifdef DEBUG_TRAIN_NETWORK_WITH_NON_RANDOM_VARS
				currentANNneuronConnection->weight = (double(888* 2.0F)/(double(RAND_MAX)))-1.0F;
				#else
				#ifdef DEBUG_TH_ANN_USE_ORIGINAL_RANDOMISATION
				currentANNneuronConnection->weight = (double(rand()* 2.0F)/(double(RAND_MAX)+1.0F))-1.0F;
				#else
				currentANNneuronConnection->weight = (double(rand()* 2.0F)/(double(RAND_MAX)))-1.0F;
				#endif
				#endif
				#endif
				#endif
				#endif

				#ifdef ANN_DEBUG
				cout << "Front ANNneuron Connection Weight = " << currentANNneuronConnection->weight << endl;
				#endif

				currentANNneuronConnection->memoryTrace = 0.0;
			}

			#ifdef ANN_SUBNETS
			if(currentNeuronReference->isSubnet)
			{
				this->resetNeuralNetworkWithRandomBiasAndWeightsAndEraseMemoryTrace(currentNeuronReference->firstNeuronInBackLayerOfSubnet);
			}
			#endif
		}

		currentNeuronReference = currentNeuronReference->nextNeuron;
	}


	//recursion [NB there will be unnecessay redundancy here, as the same neuron will be accessed more than once]
	if(neuronBeingAccessed->hasFrontLayer)
	{
		this->resetNeuralNetworkWithRandomBiasAndWeightsAndEraseMemoryTrace(neuronBeingAccessed->firstNeuronInFrontLayer);
	}
}

void ANNalgorithmMemoryNetworkTrainingClass::resetInputs(ANNneuron* firstInputNeuron, const long numberOfInputNeurons, ANNexperience* currentExperienceInDataSet)
{
	#ifdef ANN_DEBUG
	cout << "resetInputs{}:" << endl;
	#endif

	//sets inputData into ANN
	//(NB normalisedInputData[i][0] is target class

	ANNneuron* currentNeuronReference = firstInputNeuron;
	ANNexperienceInput* currentExperienceInputInExperience = currentExperienceInDataSet->firstExperienceInput;
	for(long i = 0; i < numberOfInputNeurons; i++)
	{
		#ifdef DEBUG_TH_OR_IMAGE_CATEGORISTION_NN_2
		cout << "currentExperienceInputInExperience->inputValue = " << currentExperienceInputInExperience->inputValue << endl;
		#endif
		currentNeuronReference->output = currentExperienceInputInExperience->inputValue;	 //normalisedInputData[testSegment][(i+1)];
		currentNeuronReference = currentNeuronReference->nextNeuron;
		currentExperienceInputInExperience = currentExperienceInputInExperience->next;
	}

#ifdef ANN_SUBNETS
	/*CHECK ANNTHIS: anything required for subnet inputs/outputs?*/
#endif

}

#endif


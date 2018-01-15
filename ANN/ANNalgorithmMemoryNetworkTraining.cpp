/*******************************************************************************
 *
 * This file is part of BAIPROJECT.
 *
 * BAIPROJECT is licensed under the GNU Affero General Public License
 * version 3, as published by the Free Software Foundation. The use of
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
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2018 Baxter AI (baxterai.com)
 * Project: Artificial Neural Network (ANN)
 * Project Version: 3m11a 10-January-2018
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



void ANNalgorithmMemoryNetworkTrainingClass::trainNeuralNetworkMemory(ANNneuron* firstInputNeuron, const ANNneuron* firstOutputNeuron, const long numberOfInputNeurons, const long numberOfOutputNeurons, const int maxFolds, ANNexperience* firstExperienceInDataSet, const long numberOfExperiences)
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

void ANNalgorithmMemoryNetworkTrainingClass::resetInputs(ANNneuron* firstInputNeuron, const long numberOfInputNeurons, ANNexperience* currentExperienceInDataSet)
{

	//sets inputData into ANN
	//(NB normalisedInputData[i][0] is target class

	ANNneuron* currentNeuronReference = firstInputNeuron;
	ANNexperienceInput* currentExperienceInputInExperience = currentExperienceInDataSet->firstExperienceInput;
	for(long i = 0; i < numberOfInputNeurons; i++)
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


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
 * File Name: ANNalgorithmMemoryNetworkUpdate.cpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2019 Baxter AI (baxterai.com)
 * Project: Artificial Neural Network (ANN)
 * Project Version: 3m16a 24-April-2019
 * Comments:
 * /
 *******************************************************************************/


#include "ANNalgorithmMemoryNetworkUpdate.hpp"

#ifdef ANN_ALGORITHM_MEMORY_NETWORK

double ANNalgorithmMemoryNetworkUpdateClass::ANNclassificationAndMemoryPass(ANNneuron* firstInputNeuronInNetwork, const ANNneuron* firstOutputNeuronInNetwork, string* trainingClassificationResult, double* trainingMemoryResult)
{

	//classification routine
	ANNalgorithmBackpropagationUpdate.backPropogationForwardPassStep(firstInputNeuronInNetwork);

	calculateBinaryOutputCode(firstOutputNeuronInNetwork, trainingClassificationResult);

	//memory trace update (write) and detect (read) routine
	memoryTraceForwardPassStep(firstInputNeuronInNetwork, trainingMemoryResult);

}

void ANNalgorithmMemoryNetworkUpdateClass::calculateBinaryOutputCode(const ANNneuron* firstOutputNeuronInNetwork, string* trainingClassificationResult)
{
	//NB trainingClassificationResult = binaryOutputCode

	const ANNneuron* currentNeuron = firstOutputNeuronInNetwork;
	int outputNeuronIndex = 0;
	while(currentNeuron->nextNeuron != NULL)
	{
		if(isNeuronOutputFire(currentNeuron))
		{
			*trainingClassificationResult = *trainingClassificationResult + '1';
		}
		else
		{
			//redundant
			*trainingClassificationResult = *trainingClassificationResult + '0';
		}
		outputNeuronIndex++;
		currentNeuron = currentNeuron->nextNeuron;
	}

	#ifdef ANN_VERBOSE_ALGORITHM_MEMORY_NETWORK
	cout << "trainingClassificationResult = " << *trainingClassificationResult << endl;
	#endif
}

bool ANNalgorithmMemoryNetworkUpdateClass::isNeuronOutputFire(const ANNneuron* currentNeuron)
{
	bool neuronOutputFire = false;
	if(currentNeuron->output > ANN_ALGORITHM_MEMORY_NETWORK_OUTPUT_NEURON_FIRE_TRESHOLD_FOR_CLASSIFICATION_BIT)
	{
		neuronOutputFire = true;
	}
	return neuronOutputFire;
}

void ANNalgorithmMemoryNetworkUpdateClass::memoryTraceForwardPassStep(ANNneuron* neuronBeingAccessed, double* trainingMemoryResult)
{

	ANNneuron* currentNeuron = neuronBeingAccessed->firstNeuronInFrontLayer;

	while(currentNeuron->nextNeuron != NULL)
	{

		if(!(currentNeuron->isSubnet))
		{
			readAndUpdateMemoryTrace(currentNeuron, trainingMemoryResult);
		}

		currentNeuron = currentNeuron->nextNeuron;
	}

	//ANN recursion
	#ifdef ANN_SUBNETS
	//NB if !isInputSubnet, subnets require a back layer to exist in the top level network
	currentNeuron = neuronBeingAccessed;
	while(currentNeuron->nextNeuron != NULL)
	{
		if(currentNeuron->isSubnet)
		{

			if(!(currentNeuron->isInputSubnet))
			{
				ANNalgorithmBackpropagationUpdate.copyANNneuronConnectionContainerListToNeuronContainerList(currentNeuron->firstNeuronInBackLayerOfSubnet, &(currentNeuron->backANNneuronConnectionList), false);
			}

			ANNalgorithmBackpropagationUpdate.copyANNneuronConnectionContainerListToNeuronContainerList(currentNeuron->firstNeuronInFrontLayerOfSubnet, &(currentNeuron->frontANNneuronConnectionList), true);
			memoryTraceForwardPassStep(currentNeuron->firstNeuronInBackLayerOfSubnet, trainingMemoryResult);	//?ISSUE HERE

			if(!(currentNeuron->isInputSubnet))
			{
				ANNalgorithmBackpropagationUpdate.copyNeuronContainerListToANNneuronConnectionContainerList(&(currentNeuron->backANNneuronConnectionList), currentNeuron->firstNeuronInBackLayerOfSubnet, false);
			}

			ANNalgorithmBackpropagationUpdate.copyNeuronContainerListToANNneuronConnectionContainerList(&(currentNeuron->frontANNneuronConnectionList), currentNeuron->firstNeuronInFrontLayerOfSubnet, true);
		}
		currentNeuron = currentNeuron->nextNeuron;
	}
	#endif

	//recursion
	if(neuronBeingAccessed->firstNeuronInFrontLayer->hasFrontLayer)
	{
		memoryTraceForwardPassStep(neuronBeingAccessed->firstNeuronInFrontLayer, trainingMemoryResult);
	}
}

void ANNalgorithmMemoryNetworkUpdateClass::readAndUpdateMemoryTrace(ANNneuron* currentNeuronInLayer, double* trainingMemoryResult)
{
	for(vector<ANNneuronConnection*>::iterator connectionIter = currentNeuronInLayer->sideANNneuronConnectionList.begin(); connectionIter != currentNeuronInLayer->sideANNneuronConnectionList.end(); connectionIter++)
	{
		ANNneuronConnection* currentANNneuronConnectionSide = *connectionIter;
		bool leftNeuronFire = isNeuronOutputFire(currentANNneuronConnectionSide->backNeuron);
		bool rightNeuronFire = isNeuronOutputFire(currentANNneuronConnectionSide->frontNeuron);
		bool updateMemoryTrace = false;
		if(leftNeuronFire && rightNeuronFire)
		{
			updateMemoryTrace = true;
			updateMemoryTraceBasedOnSimultaneouslyFiredNeuronsIeAssocation(currentANNneuronConnectionSide);
			incrementMemoryTraceTallyBasedOnSimultaneouslyFiredNeuronsIeAssocation(trainingMemoryResult);
		}
	}
}

void ANNalgorithmMemoryNetworkUpdateClass::updateMemoryTraceBasedOnSimultaneouslyFiredNeuronsIeAssocation(ANNneuronConnection* currentANNneuronConnectionSide)
{
	currentANNneuronConnectionSide->memoryTrace = currentANNneuronConnectionSide->memoryTrace + ANN_ALGORITHM_MEMORY_NETWORK_MEMORY_TRACE_UPDATE_VALUE;
}

void ANNalgorithmMemoryNetworkUpdateClass::incrementMemoryTraceTallyBasedOnSimultaneouslyFiredNeuronsIeAssocation(double* trainingMemoryResult)
{
	*trainingMemoryResult = *trainingMemoryResult + ANN_ALGORITHM_MEMORY_NETWORK_MEMORY_TRACE_TALLY_INCREMENT_VALUE;
}


#endif














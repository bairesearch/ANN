 /*******************************************************************************
 *
 * License: GNU Affero General Public License version 3 (AGPLv3)
 *
 *******************************************************************************/

/*******************************************************************************
 *
 * File Name: ANNalgorithmMemoryNetworkUpdate.cpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2022 Baxter AI (baxterai.com)
 * Project: Artificial Neural Network (ANN)
 * Project Version: 3q1a 05-June-2022
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














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
 * File Name: ANNalgorithmMemoryNetworkUpdate.cpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2017 Baxter AI (baxterai.com)
 * Project: Artificial Neural Network (ANN)
 * Project Version: 3l1a 02-June-2017
 * Comments:
 *
 *******************************************************************************/


#include "ANNalgorithmMemoryNetworkUpdate.hpp"

#ifdef ANN_ALGORITHM_MEMORY_NETWORK

double ANNalgorithmMemoryNetworkUpdateClass::ANNclassificationAndMemoryPass(ANNneuron* firstInputNeuronInNetwork, const ANNneuron* firstOutputNeuronInNetwork, string* trainingClassificationResult, double* trainingMemoryResult)
{

	//classification routine
	ANNalgorithmBackpropagationUpdate.backPropogationForwardPassStep(firstInputNeuronInNetwork);

	this->calculateBinaryOutputCode(firstOutputNeuronInNetwork, trainingClassificationResult);

	//memory trace update (write) and detect (read) routine
	this->memoryTraceForwardPassStep(firstInputNeuronInNetwork, trainingMemoryResult);

}

void ANNalgorithmMemoryNetworkUpdateClass::calculateBinaryOutputCode(const ANNneuron* firstOutputNeuronInNetwork, string* trainingClassificationResult)
{
	//NB trainingClassificationResult = binaryOutputCode

	const ANNneuron* currentNeuron = firstOutputNeuronInNetwork;
	int outputNeuronIndex = 0;
	while(currentNeuron->nextNeuron != NULL)
	{
		if(this->isNeuronOutputFire(currentNeuron))
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
			this->readAndUpdateMemoryTrace(currentNeuron, trainingMemoryResult);
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
			this->memoryTraceForwardPassStep(currentNeuron->firstNeuronInBackLayerOfSubnet, trainingMemoryResult);	//?ISSUE HERE

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
		this->memoryTraceForwardPassStep(neuronBeingAccessed->firstNeuronInFrontLayer, trainingMemoryResult);
	}
}

void ANNalgorithmMemoryNetworkUpdateClass::readAndUpdateMemoryTrace(ANNneuron* currentNeuronInLayer, double* trainingMemoryResult)
{
	for(vector<ANNneuronConnection*>::iterator connectionIter = currentNeuronInLayer->sideANNneuronConnectionList.begin(); connectionIter != currentNeuronInLayer->sideANNneuronConnectionList.end(); connectionIter++)
	{
		ANNneuronConnection* currentANNneuronConnectionSide = *connectionIter;
		bool leftNeuronFire = this->isNeuronOutputFire(currentANNneuronConnectionSide->backNeuron);
		bool rightNeuronFire = this->isNeuronOutputFire(currentANNneuronConnectionSide->frontNeuron);
		bool updateMemoryTrace = false;
		if(leftNeuronFire && rightNeuronFire)
		{
			updateMemoryTrace = true;
			this->updateMemoryTraceBasedOnSimultaneouslyFiredNeuronsIeAssocation(currentANNneuronConnectionSide);
			this->incrementMemoryTraceTallyBasedOnSimultaneouslyFiredNeuronsIeAssocation(trainingMemoryResult);
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














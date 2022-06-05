 /*******************************************************************************
 *
 * License: GNU Affero General Public License version 3 (AGPLv3)
 *
 *******************************************************************************/

/*******************************************************************************
 *
 * File Name: ANNalgorithmMemoryNetworkUpdate.hpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2022 Baxter AI (baxterai.com)
 * Project: Artificial Neural Network (ANN)
 * Project Version: 3q1a 05-June-2022
 * Comments:
 * /
 *******************************************************************************/


#ifndef HEADER_ANN_ALGORITHM_MEMORY_NETWORK_UPDATE
#define HEADER_ANN_ALGORITHM_MEMORY_NETWORK_UPDATE

#include "ANNglobalDefs.hpp"
#include "ANNneuronClass.hpp"
#include "ANNneuronConnectionClass.hpp"
#include "ANNalgorithmBackpropagationUpdate.hpp"	//required for backPropogationForwardPassStep

#ifdef ANN_ALGORITHM_MEMORY_NETWORK

class ANNalgorithmMemoryNetworkUpdateClass
{
	private: ANNalgorithmBackpropagationUpdateClass ANNalgorithmBackpropagationUpdate;
	public: double ANNclassificationAndMemoryPass(ANNneuron* firstInputNeuronInNetwork, const ANNneuron* firstOutputNeuronInNetwork, string* trainingClassificationResult, double* trainingMemoryResult);
		private: void calculateBinaryOutputCode(const ANNneuron* firstOutputNeuronInNetwork, string* trainingClassificationResult);
		private: bool isNeuronOutputFire(const ANNneuron* currentNeuron);
		private: void memoryTraceForwardPassStep(ANNneuron* neuronBeingAccessed, double* trainingMemoryResult);
			private: void readAndUpdateMemoryTrace(ANNneuron* currentNeuronInLayer, double* trainingMemoryResult);
				private: void updateMemoryTraceBasedOnSimultaneouslyFiredNeuronsIeAssocation(ANNneuronConnection* currentANNneuronConnectionSide);
				private: void incrementMemoryTraceTallyBasedOnSimultaneouslyFiredNeuronsIeAssocation(double* trainingMemoryResult);
};



#endif

#endif


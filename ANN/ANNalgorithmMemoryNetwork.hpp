 /*******************************************************************************
 *
 * License: GNU Affero General Public License version 3 (AGPLv3)
 *
 *******************************************************************************/

/*******************************************************************************
 *
 * File Name: ANNalgorithmMemoryNetwork.hpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2022 Baxter AI (baxterai.com)
 * Project: Artificial Neural Network (ANN)
 * Project Version: 3q1a 05-June-2022
 * Comments:
 * /
 *******************************************************************************/


#ifndef HEADER_ANN_ALGORITHM_MEMORY_NETWORK
#define HEADER_ANN_ALGORITHM_MEMORY_NETWORK

#include "ANNglobalDefs.hpp"
#include "ANNneuronClass.hpp"
#include "ANNneuronConnectionClass.hpp"
#include "ANNexperienceClass.hpp"
#include "ANNalgorithmMemoryNetworkUpdate.hpp"
#include "ANNdata.hpp"

#ifdef ANN_ALGORITHM_MEMORY_NETWORK

class ANNalgorithmMemoryNetworkClass
{
	private: ANNalgorithmMemoryNetworkUpdateClass ANNalgorithmMemoryNetworkUpdate;
	private: ANNexperienceClassClass ANNexperienceClass;
	public: void trainNeuralNetworkMemorySimple(ANNneuron* firstInputNeuron, const ANNneuron* firstOutputNeuron, const int64_t numberOfInputNeurons, const int64_t numberOfOutputNeurons, ANNexperience* firstExperienceInDataSet, const int64_t numberOfExperiences);
	public: void trainNeuralNetworkMemory(ANNneuron* firstInputNeuron, const ANNneuron* firstOutputNeuron, const int64_t numberOfInputNeurons, const int64_t numberOfOutputNeurons, const int maxFolds, ANNexperience* firstExperienceInDataSet, const int64_t numberOfExperiences);

	private: void storeNeuralNetworkMemoryTrace(ANNneuron* neuronBeingAccessed);
	private: void restoreNeuralNetworkWithStoredMemoryTrace(ANNneuron* neuronBeingAccessed);
	private: void resetNeuralNetworkWithRandomBiasAndWeightsAndEraseMemoryTrace(ANNneuron* neuronBeingAccessed);
	private: void resetInputs(ANNneuron* firstInputNeuron, const int64_t numberOfInputNeurons, ANNexperience* currentExperienceInDataSet);
};

#endif
#endif

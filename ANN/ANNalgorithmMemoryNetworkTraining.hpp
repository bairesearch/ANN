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
 * File Name: ANNalgorithmMemoryNetworkTraining.hpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2018 Baxter AI (baxterai.com)
 * Project: Artificial Neural Network (ANN)
 * Project Version: 3m14a 20-April-2018
 * Comments:
 * /
 *******************************************************************************/


#ifndef HEADER_ANN_ALGORITHM_MEMORY_NETWORK_TRAINING
#define HEADER_ANN_ALGORITHM_MEMORY_NETWORK_TRAINING

#include "ANNglobalDefs.hpp"
#include "ANNneuronClass.hpp"
#include "ANNneuronConnectionClass.hpp"
#include "ANNexperienceClass.hpp"
#include "ANNalgorithmMemoryNetworkUpdate.hpp"
#include "ANNdata.hpp"

#ifdef ANN_ALGORITHM_MEMORY_NETWORK

class ANNalgorithmMemoryNetworkTrainingClass
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

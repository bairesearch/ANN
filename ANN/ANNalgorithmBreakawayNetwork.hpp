 /*******************************************************************************
 *
 * License: GNU Affero General Public License version 3 (AGPLv3)
 *
 *******************************************************************************/

/*******************************************************************************
 *
 * File Name: ANNalgorithmBreakawayNetwork.hpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2022 Baxter AI (baxterai.com)
 * Project: Artificial Neural Network (ANN)
 * Project Version: 3q1a 05-June-2022
 * Comments:
 * /
 *******************************************************************************/


#ifndef HEADER_ANN_ALGORITHM_BREAKAWAY_NETWORK
#define HEADER_ANN_ALGORITHM_BREAKAWAY_NETWORK

#include "ANNglobalDefs.hpp"
#include "ANNneuronClass.hpp"
#include "ANNneuronConnectionClass.hpp"
#include "ANNexperienceClass.hpp"
#include "ANNformation.hpp"
#include "ANNalgorithmBackpropagation.hpp"
#include "ANNdata.hpp"
#include "SHAREDvars.hpp"

#ifdef ANN_ALGORITHM_BREAKAWAY_NETWORK

class ANNalgorithmBreakawayNetworkClass
{
	private: SHAREDvarsClass SHAREDvars;
	private: ANNformationClass ANNformation;
	private: ANNalgorithmBackpropagationClass ANNalgorithmBackpropagation;
	public: bool trainNeuralNetworkBreakaway(ANNneuron* firstInputNeuronInNetwork, ANNneuron** firstOutputNeuronInNetwork, const int numberOfInputNeurons, const int numberOfOutputNeurons, const bool simple, const int numEpochs, const int maxFolds, const int maxEpochs, ANNexperience* firstExperienceInDataSet, const int64_t numberOfExperiences, const int sizeOfSupernet, int numberOfLayersOriginal);
};

#endif
#endif

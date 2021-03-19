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
 * File Name: ANNalgorithmBreakawayNetwork.hpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2021 Baxter AI (baxterai.com)
 * Project: Artificial Neural Network (ANN)
 * Project Version: 3p3a 19-March-2021
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

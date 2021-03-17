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
 * File Name: ANNalgorithmBackpropagationUpdate.hpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2021 Baxter AI (baxterai.com)
 * Project: Artificial Neural Network (ANN)
 * Project Version: 3p2a 17-March-2021
 * Comments:
 * /
 *******************************************************************************/


#ifndef HEADER_ANN_ALGORITHM_BACKPROPAGATION_UPDATE
#define HEADER_ANN_ALGORITHM_BACKPROPAGATION_UPDATE

#include "ANNglobalDefs.hpp"
#include "ANNneuronClass.hpp"
#include "ANNneuronConnectionClass.hpp"



class ANNalgorithmBackpropagationUpdateClass
{
	private: ANNneuronClassClass ANNneuronClass;
	#ifdef ANN_ALGORITHM_BACKPROPAGATION
	public: double ANNbackPropogationPass(ANNneuron* firstInputNeuronInNetwork, ANNneuron* firstOutputNeuronInNetwork);
		public: double calculateErrorOfBackPropPass(const ANNneuron* firstOutputNeuronInNetwork);
		private: void calculateErrorOfOutputNeurons(ANNneuron* firstOutputNeuronInNetwork);
	#endif
		public: void backPropogationForwardPassStep(ANNneuron* neuronBeingAccessed);
			private: void backpropagationAdjustOutputValueOfANeuronBasedOnBackNeurons(ANNneuron* neuronBeingAccessed);
				private: float calculateOValue(float netValue);
		#ifdef ANN_ALGORITHM_BACKPROPAGATION
		private: void backPropogationBackwardPassStep(ANNneuron* neuronBeingAccessed, const int isOutputLayer, const bool isSubnet);
			private: void calculateOutputErrorOfOutputNeuron(ANNneuron* neuronBeingAccessed);
			private: void calculateOutputErrorOfNonoutputNeuron(ANNneuron* neuronBeingAccessed);
			private: void calculateNewBackConnectionWeightsOfNeuron(ANNneuron* neuronBeingAccessed);
			private: void calculateNewBiasOfNeuron(ANNneuron* neuronBeingAccessed);
		#endif

	#ifdef ANN_SUBNETS
	public: void copyNeuronContainerListToANNneuronConnectionContainerList(vector<ANNneuronConnection*>* ANNneuronConnectionListToUpdate, ANNneuron* firstNeuronInListToCopy, const bool frontOrBack);
	public: void copyANNneuronConnectionContainerListToNeuronContainerList(ANNneuron* firstNeuronInListToUpdate, vector<ANNneuronConnection*>* ANNneuronConnectionListToCopy, const bool frontOrBack);
	#endif
};





#endif


 /*******************************************************************************
 *
 * License: GNU Affero General Public License version 3 (AGPLv3)
 *
 *******************************************************************************/

/*******************************************************************************
 *
 * File Name: ANNalgorithmBackpropagationUpdate.hpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2022 Baxter AI (baxterai.com)
 * Project: Artificial Neural Network (ANN)
 * Project Version: 3q1a 05-June-2022
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


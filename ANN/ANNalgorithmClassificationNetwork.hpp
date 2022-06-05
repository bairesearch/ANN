 /*******************************************************************************
 *
 * License: GNU Affero General Public License version 3 (AGPLv3)
 *
 *******************************************************************************/

/*******************************************************************************
 *
 * File Name: ANNalgorithmClassificationNetwork.hpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2022 Baxter AI (baxterai.com)
 * Project: Artificial Neural Network (ANN)
 * Project Version: 3q1a 05-June-2022
 * Comments:
 * /
 *******************************************************************************/


#ifndef HEADER_ANN_ALGORITHM_CLASSIFICATION_NETWORK
#define HEADER_ANN_ALGORITHM_CLASSIFICATION_NETWORK

#include "ANNglobalDefs.hpp"
#include "ANNneuronClass.hpp"
#include "ANNneuronConnectionClass.hpp"
#include "ANNexperienceClass.hpp"
#include "ANNdata.hpp"
#ifdef ANN_ALGORITHM_CLASSIFICATION_NETWORK_DISPLAY_EVOLUTION_OF_NETWORK
#include "ANNdisplay.hpp"
#endif
#include "SHAREDvars.hpp"

#ifdef ANN_ALGORITHM_CLASSIFICATION_NETWORK

class ANNalgorithmClassificationNetworkClass
{
	private: SHAREDvarsClass SHAREDvars;
	#ifdef ANN_ALGORITHM_CLASSIFICATION_NETWORK_DISPLAY_EVOLUTION_OF_NETWORK
	private: ANNdisplayClass ANNdisplay;
	#endif
	public: void trainNeuralNetworkClassificationSimple(ANNneuron* firstInputNeuron, ANNneuron** firstOutputNeuron, int64_t numberOfInputNeurons, int64_t* numberOfOutputNeurons, ANNexperience* firstExperienceInDataSet, int64_t numberOfExperiences);
		private: void resetIntermediaryNeuronCreatedThisRoundFlag(ANNneuron* categoryNeuron);
	#ifdef ANN_VERBOSE_ALGORITHM_CLASSIFICATION_NETWORK_ENABLE_IDEAL_VALUE_UPDATING_FOR_PARENTS
		private: void resetBackNeuronMatchTemp(ANNneuron* categoryNeuron);
	#endif
	#ifdef ANN_ALGORITHM_CLASSIFICATION_NETWORK_PRUNING
		private: void pruneNetorkBasedOnRarelyUsedNeurons(ANNneuron* categoryNeuron, ANNneuron** firstOutputNeuron);
	#endif
		private: void deleteNeuron(ANNneuron* frontNeuron, ANNneuron* categoryNeuron, ANNneuron** firstOutputNeuron);
	#ifdef ANN_VERBOSE_ALGORITHM_CLASSIFICATION_NETWORK_MERGE_SIMILAR_NEURONS
		private: void mergeSimilarNeurons(ANNneuron* categoryNeuron, ANNneuron** firstOutputNeuron);
	#endif
	#ifdef ANN_VERBOSE_ALGORITHM_CLASSIFICATION_NETWORK_CULL_REDUNDANT_NEURONS
		private: bool cullRedundantNeurons(ANNneuron* categoryNeuron, ANNneuron* parentNeuron, ANNneuron** firstOutputNeuron);
	#endif
		private: void findCategoriesForExperienceWrapper(ANNneuron* categoryNeuron, vector<bool>* inputValuesCategoryFound, ANNneuron** experienceClassificationneuronTypeTopLevelCategory, int createIntermediaryNeuronsStage);
			private: bool findCategoriesForExperience(ANNneuron* categoryNeuron, vector<bool>* inputValuesCategoryFound, int level);
			private: ANNneuronConnection* connectNeurons(ANNneuron* childNeuron, ANNneuron* parentNeuron);
			private: void updateConnectionIdealValuesParent(ANNneuron* categoryNeuron, bool updateIdealValues);
			private: void updateConnectionIdealValuesChildren(ANNneuron* categoryNeuron, bool updateIdealValues);
			private: void updateConnectionIdealValue(ANNneuronConnection* connection);
			private: void updateConnectionIdealValueNoUpdating(ANNneuronConnection* connection);
			private: double calculateIdealOutputValue(ANNneuron* categoryNeuron);
			private: double calculateOutput(double totalOutput, double numberOfCategoryInputValues);
			private: double calculateDiff(double idealValue, double value);
			private: double calculateSum(double value);

	private: string printTabsDEBUG(int level);
#ifdef ANN_ALGORITHM_CLASSIFICATION_NETWORK_ALLOW_FOR_CASE_WHERE_INTERMEDIARY_NEURON_IS_CONNECTED_TO_ALL_INPUTS
	private: void findInputs(ANNneuron* categoryNeuron, vector<bool>* inputFound, int level);
#endif

};


#endif
#endif

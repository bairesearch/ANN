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
 * File Name: ANNalgorithmClassificationNetworkTraining.hpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2020 Baxter AI (baxterai.com)
 * Project: Artificial Neural Network (ANN)
 * Project Version: 3n7b 17-August-2020
 * Comments:
 * /
 *******************************************************************************/


#ifndef HEADER_ANN_ALGORITHM_CLASSIFICATION_NETWORK_TRAINING
#define HEADER_ANN_ALGORITHM_CLASSIFICATION_NETWORK_TRAINING

#include "ANNglobalDefs.hpp"
#include "ANNneuronClass.hpp"
#include "ANNneuronConnectionClass.hpp"
#include "ANNexperienceClass.hpp"
#include "ANNalgorithmClassificationNetworkUpdate.hpp"
#include "ANNdata.hpp"
#ifdef ANN_ALGORITHM_CLASSIFICATION_NETWORK_DISPLAY_EVOLUTION_OF_NETWORK
#include "ANNdisplay.hpp"
#endif
#include "SHAREDvars.hpp"

#ifdef ANN_ALGORITHM_CLASSIFICATION_NETWORK

class ANNalgorithmClassificationNetworkTrainingClass
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

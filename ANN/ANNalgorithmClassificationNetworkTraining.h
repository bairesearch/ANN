/*******************************************************************************
 *
 * This file is part of BAIPROJECT.
 *
 * BAIPROJECT is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License version 3
 * only, as published by the Free Software Foundation. The use of
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
 * File Name: ANNalgorithmMemoryNetworkTraining.h
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2016 Baxter AI (baxterai.com)
 * Project: Artificial Neural Network (ANN)
 * Project Version: 3i19b 15-December-2016
 * Comments:
 *
 *******************************************************************************/


#ifndef HEADER_ANN_ALGORITHM_CLASSIFICATION_NETWORK_TRAINING
#define HEADER_ANN_ALGORITHM_CLASSIFICATION_NETWORK_TRAINING

#include "ANNglobalDefs.h"
#include "ANNneuronClass.h"
#include "ANNneuronConnectionClass.h"
#include "ANNexperienceClass.h"

#ifdef ANN_ALGORITHM_CLASSIFICATION_NETWORK

void trainNeuralNetworkClassificationSimple(ANNneuron* firstInputNeuron, ANNneuron** firstOutputNeuron, long numberOfInputNeurons, long* numberOfOutputNeurons, ANNexperience* firstExperienceInDataSet, long numberOfExperiences);
	void resetIntermediaryNeuronCreatedThisRoundFlag(ANNneuron* categoryNeuron);
	#ifdef ANN_DEBUG_ALGORITHM_CLASSIFICATION_NETWORK_ENABLE_IDEAL_VALUE_UPDATING_FOR_PARENTS
	void resetBackNeuronMatchTemp(ANNneuron* categoryNeuron);
	#endif
	#ifdef ANN_ALGORITHM_CLASSIFICATION_NETWORK_PRUNING
	void pruneNetorkBasedOnRarelyUsedNeurons(ANNneuron* categoryNeuron, ANNneuron** firstOutputNeuron);
	#endif
	void deleteNeuron(ANNneuron* frontNeuron, ANNneuron* categoryNeuron, ANNneuron** firstOutputNeuron);
	#ifdef ANN_DEBUG_ALGORITHM_CLASSIFICATION_NETWORK_MERGE_SIMILAR_NEURONS
	void mergeSimilarNeurons(ANNneuron* categoryNeuron, ANNneuron** firstOutputNeuron);
	#endif
	#ifdef ANN_DEBUG_ALGORITHM_CLASSIFICATION_NETWORK_CULL_REDUNDANT_NEURONS
	bool cullRedundantNeurons(ANNneuron* categoryNeuron, ANNneuron* parentNeuron, ANNneuron** firstOutputNeuron);
	#endif
	void findCategoriesForExperienceWrapper(ANNneuron* categoryNeuron, vector<bool>* inputValuesCategoryFound, ANNneuron** experienceClassificationneuronTypeTopLevelCategory, int createIntermediaryNeuronsStage);
		bool findCategoriesForExperience(ANNneuron* categoryNeuron, vector<bool>* inputValuesCategoryFound, int level);
		ANNneuronConnection* connectNeurons(ANNneuron* childNeuron, ANNneuron* parentNeuron);
		void updateConnectionIdealValuesParent(ANNneuron* categoryNeuron, bool updateIdealValues);
		void updateConnectionIdealValuesChildren(ANNneuron* categoryNeuron, bool updateIdealValues);
		void updateConnectionIdealValue(ANNneuronConnection* connection);
		void updateConnectionIdealValueNoUpdating(ANNneuronConnection* connection);
		double calculateIdealOutputValue(ANNneuron* categoryNeuron);
		double calculateOutput(double totalOutput, double numberOfCategoryInputValues);
		double calculateDiff(double idealValue, double value);
		double calculateSum(double value);

string printTabsDEBUG(int level);
#ifdef ANN_ALGORITHM_CLASSIFICATION_NETWORK_ALLOW_FOR_CASE_WHERE_INTERMEDIARY_NEURON_IS_CONNECTED_TO_ALL_INPUTS
void findInputs(ANNneuron* categoryNeuron, vector<bool>* inputFound, int level);
#endif

#ifdef ANN_DEBUG_ALGORITHM_CLASSIFICATION_NETWORK3
void checkRobustnessOfIdealValues(ANNneuron* categoryNeuron);
#endif
#ifdef ANN_DEBUG_ALGORITHM_CLASSIFICATION_NETWORK6
void checkMinNumberOfInputNeurons(ANNneuron* categoryNeuron);
#endif

#endif
#endif

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
 * Project Version: 4a3s 02-May-2016
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
	#ifdef ANN_ALGORITHM_CLASSIFICATION_NETWORK_PRUNING
	void pruneNetorkBasedOnRarelyUsedNeurons(ANNneuron* categoryNeuron);
	#endif
	void findCategoriesForExperienceWrapper(ANNneuron* categoryNeuron, vector<bool>* inputValuesCategoryFound, ANNneuron** experienceClassificationneuronTypeTopLevelCategory, int createIntermediaryNeuronsStage);
		bool findCategoriesForExperience(ANNneuron* categoryNeuron, vector<bool>* inputValuesCategoryFound);
		void updateConnectionIdealValue(ANNneuronConnection* connection);
		ANNneuronConnection* connectNeurons(ANNneuron* parentNeuron, ANNneuron* childNeuron);
		double calculateDiff(double idealValue, double value);
		double calculateSum(double value);

#endif
#endif

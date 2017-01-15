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
 * File Name: ANNalgorithmBackpropagationTraining.h
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2016 Baxter AI (baxterai.com)
 * Project: Artificial Neural Network (ANN)
 * Project Version: 4a6a 06-June-2016
 * Comments:
 *
 *******************************************************************************/


#ifndef HEADER_ANN_ALGORITHM_BACKPROPAGATION_TRAINING
#define HEADER_ANN_ALGORITHM_BACKPROPAGATION_TRAINING

#include "ANNglobalDefs.h"
#include "ANNneuronClass.h"
#include "ANNneuronConnectionClass.h"
#include "ANNexperienceClass.h"

#define	MAX_NUM_FOLDS_ANNTH 10

#define IRRELEVANT 0.0
#define	ACCEPTABLE_ERROR_RATE 0.005
//#define	ACCEPTABLE_ERROR_RATE 0.01F	//after original, before 4 sept 10 to below
//#define	ACCEPTABLE_ERROR_RATE 0.005F	//orig
	//99.5% accuracy is defined as the acceptable error rate. no more epochs will be calculated once this is reached.

#define	ANN_DEFAULT_MAX_NUMBER_OF_EPOCHS 5000
#define	ANN_DEFAULT_SIMPLE_TRAIN_DEFAULT_NUM_OF_TRAINING_EPOCHS 320


//#define ERROR_VARIATION_ALLOWED 0.002F	//after original, before 4 sept 10 to below
//#define ERROR_VARIATION_ALLOWED 0.01F	//1%	//original
#define ERROR_VARIATION_ALLOWED 0.002F

#define REAL_LIFE_ACCURACY_DIAGNOSTICS_CUTOFF 0.1

#ifdef ANN_ALGORITHM_BACKPROPAGATION

double calculateExperienceErrorForHypotheticalDecisionBackpropagation(ANNneuron* firstInputNeuronInNetwork, ANNneuron* firstOutputNeuronInNetwork, long numberOfInputNeurons, long numberOfOutputNeurons, ANNexperience* experience);

void feedNeuralNetworkWithASetOfExperiencesBackpropagation(ANNneuron* firstInputNeuron, ANNneuron* firstOutputNeuron, int numberOfInputNeurons, int numberOfOutputNeurons, ANNexperience* firstExperienceInList);

void trainNeuralNetworkBackpropagationSimple(ANNneuron* firstInputNeuron, ANNneuron* firstOutputNeuron, int numberOfInputNeurons, int numberOfOutputNeurons, int numEpochs, ANNexperience* firstExperienceInDataSet, long numberOfExperiences);
void trainNeuralNetworkBackpropagation(ANNneuron* firstInputNeuron, ANNneuron* firstOutputNeuron, int numberOfInputNeurons, int numberOfOutputNeurons, int maxFolds, ANNexperience* firstExperienceInDataSet, long numberOfExperiences, int maxEpochs);

	void storeNeuralNetworkBiasAndWeights(ANNneuron* neuronBeingAccessed);
	void restoreNeuralNetworkWithStoredBiasAndWeights(ANNneuron* neuronBeingAccessed);
	void resetNeuralNetworkWithRandomBiasAndWeights(ANNneuron* neuronBeingAccessed);
	void resetInputsAndClassTargets(ANNneuron* firstInputNeuron, ANNneuron* firstOutputNeuron, long numberOfInputNeurons, long numberOfOutputNeurons, ANNexperience* currentExperienceInDataSet);
	float calculateStandardDeviation(float* array, int length, float average);


#endif
#endif

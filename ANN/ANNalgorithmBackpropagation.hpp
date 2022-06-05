 /*******************************************************************************
 *
 * License: GNU Affero General Public License version 3 (AGPLv3)
 *
 *******************************************************************************/

/*******************************************************************************
 *
 * File Name: ANNalgorithmBackpropagation.hpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2022 Baxter AI (baxterai.com)
 * Project: Artificial Neural Network (ANN)
 * Project Version: 3q1a 05-June-2022
 * Comments:
 * /
 *******************************************************************************/


#ifndef HEADER_ANN_ALGORITHM_BACKPROPAGATION
#define HEADER_ANN_ALGORITHM_BACKPROPAGATION

#include "ANNglobalDefs.hpp"
#include "ANNneuronClass.hpp"
#include "ANNneuronConnectionClass.hpp"
#include "ANNexperienceClass.hpp"
#include "ANNalgorithmBackpropagationUpdate.hpp"
#include "ANNdata.hpp"

#define	MAX_NUM_FOLDS_ANNTH 10

#define IRRELEVANT 0.0
#define	ACCEPTABLE_ERROR_RATE 0.005
//#define	ACCEPTABLE_ERROR_RATE 0.01F	//after original, before 4 sept 10 to below
//#define	ACCEPTABLE_ERROR_RATE 0.005F	//orig
	//99.5% accuracy is defined as the acceptable error rate. no more epochs will be calculated once this is reached.

#define	ANN_DEFAULT_MAX_NUMBER_OF_EPOCHS 5000
#define	ANN_DEFAULT_SIMPLE_TRAIN_DEFAULT_NUM_OF_TRAINING_EPOCHS 1000 //OLD: 320


//#define ERROR_VARIATION_ALLOWED 0.002F	//after original, before 4 sept 10 to below
//#define ERROR_VARIATION_ALLOWED 0.01F	//1%	//original
#define ERROR_VARIATION_ALLOWED 0.002F

#define REAL_LIFE_ACCURACY_DIAGNOSTICS_CUTOFF 0.1

#ifdef ANN_ALGORITHM_BACKPROPAGATION

class ANNalgorithmBackpropagationClass
{
	private: ANNalgorithmBackpropagationUpdateClass ANNalgorithmBackpropagationUpdate;
	private: ANNexperienceClassClass ANNexperienceClass;
	public: double calculateExperienceErrorForHypotheticalDecisionBackpropagation(ANNneuron* firstInputNeuronInNetwork, ANNneuron* firstOutputNeuronInNetwork, const int64_t numberOfInputNeurons, const int64_t numberOfOutputNeurons, ANNexperience* experience);
	public: bool calculateIdealClassTargetOfInputExperience(ANNneuron* firstInputNeuronInNetwork, ANNneuron* firstOutputNeuronInNetwork, const int64_t numberOfInputNeurons, const int64_t numberOfOutputNeurons, ANNexperience* experience, int* idealClassTarget, double* experienceBackPropagationPassError);

	public: void feedNeuralNetworkWithASetOfExperiencesBackpropagation(ANNneuron* firstInputNeuron, ANNneuron* firstOutputNeuron, const int numberOfInputNeurons, const int numberOfOutputNeurons, ANNexperience* firstExperienceInList);
		public: void feedNeuralNetworkWithExperienceBackpropagation(ANNneuron* firstInputNeuron, ANNneuron* firstOutputNeuron, const int numberOfInputNeurons, const int numberOfOutputNeurons, ANNexperience* currentExperience);

	public: void trainNeuralNetworkBackpropagationSimple(ANNneuron* firstInputNeuron, ANNneuron* firstOutputNeuron, const int numberOfInputNeurons, const int numberOfOutputNeurons, const int numEpochs, ANNexperience* firstExperienceInDataSet, const int64_t numberOfExperiences);
		public: void trainNeuralNetworkBackpropagationSimpleNoReset(ANNneuron* firstInputNeuron, ANNneuron* firstOutputNeuron, const int numberOfInputNeurons, const int numberOfOutputNeurons, const int numEpochs, ANNexperience* firstExperienceInDataSet, const int64_t numberOfExperiences);
	public: void trainNeuralNetworkBackpropagation(ANNneuron* firstInputNeuron, ANNneuron* firstOutputNeuron, const int numberOfInputNeurons, const int numberOfOutputNeurons, const int maxFolds, ANNexperience* firstExperienceInDataSet, const int64_t numberOfExperiences, const int maxEpochs);

		private: void storeNeuralNetworkBiasAndWeights(ANNneuron* neuronBeingAccessed);
		private: void restoreNeuralNetworkWithStoredBiasAndWeights(ANNneuron* neuronBeingAccessed);
		public: void resetNeuralNetworkWithRandomBiasAndWeights(ANNneuron* neuronBeingAccessed);
		private: void resetInputsAndClassTargets(ANNneuron* firstInputNeuron, ANNneuron* firstOutputNeuron, const int64_t numberOfInputNeurons, const int64_t numberOfOutputNeurons, ANNexperience* currentExperienceInDataSet);
		private: float calculateStandardDeviation(const float* array, const int length, const float average);
};


#endif
#endif

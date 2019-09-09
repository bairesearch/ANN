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
 * File Name: ANNalgorithmBackpropagationTraining.hpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2019 Baxter AI (baxterai.com)
 * Project: Artificial Neural Network (ANN)
 * Project Version: 3n1b 10-August-2019
 * Comments:
 * /
 *******************************************************************************/


#ifndef HEADER_ANN_ALGORITHM_BACKPROPAGATION_TRAINING
#define HEADER_ANN_ALGORITHM_BACKPROPAGATION_TRAINING

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

class ANNalgorithmBackpropagationTrainingClass
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

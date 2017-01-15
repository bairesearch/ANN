/*******************************************************************************
 *
 * File Name: ANNTraining.h
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2012 Baxter AI (baxterai.com)
 * Project: Advanced Neural Network (ANN)
 * Project Version: 3a8a 14-June-2012
 * Comments:
 *
 *******************************************************************************/

#ifndef HEADER_ANN_TRAINING
#define HEADER_ANN_TRAINING

#include "ANNneuronClass.h"
#include "ANNexperienceClass.h"
#include "ANNglobalDefs.h"

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





double calculateExperienceErrorForHypotheticalDecision(NeuronContainer * firstInputNeuronInNetwork, NeuronContainer * firstOutputNeuronInNetwork, long numberOfInputNeurons, long numberOfOutputNeurons, Experience * experience);

void feedNeuralNetworkWithASetOfExperiences(NeuronContainer * firstInputNeuron, NeuronContainer * firstOutputNeuron, int numberOfInputNeurons, int numberOfOutputNeurons, Experience * firstExperienceInList);

void trainNeuralNetworkSimple(NeuronContainer * firstInputNeuron, NeuronContainer * firstOutputNeuron, int numberOfInputNeurons, int numberOfOutputNeurons, int numEpochs, Experience * firstExperienceInDataSet, long numberOfExperiences);
void trainNeuralNetwork(NeuronContainer * firstInputNeuron, NeuronContainer * firstOutputNeuron, int numberOfInputNeurons, int numberOfOutputNeurons, int maxFolds, Experience * firstExperienceInDataSet, long numberOfExperiences, int maxEpochs);

	void storeNeuralNetworkBiasAndWeights(NeuronContainer * neuronBeingAccessed);
	void restoreNeuralNetworkWithStoredBiasAndWeights(NeuronContainer * neuronBeingAccessed);
	void resetNeuralNetworkWithRandomBiasAndWeights(NeuronContainer * neuronBeingAccessed);
	void resetInputsAndClassTargets(NeuronContainer * firstInputNeuron, NeuronContainer * firstOutputNeuron, long numberOfInputNeurons, long numberOfOutputNeurons, Experience * currentExperienceInDataSet);
	float calculateStandardDeviation(float* array, int length, float average);


#endif

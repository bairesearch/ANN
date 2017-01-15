/*******************************************************************************
 *
 * File Name: ANNUpdateAlgorithm.h
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2012 Baxter AI (baxterai.com)
 * Project: Advanced Neural Network (ANN)
 * Project Version: 3a8b 14-June-2012
 * Comments:
 *
 *******************************************************************************/

#ifndef HEADER_ANN_UPDATE_ALGORITHM
#define HEADER_ANN_UPDATE_ALGORITHM

#include "ANNneuronClass.h"

#include "ANNglobalDefs.h"
#ifdef DEBUG_TH_OR_IMAGE_CATEGORISTION_NN_4
extern bool debugPrintNNOutputs;
#endif

double ANNBackPropogationPass(NeuronContainer * firstInputNeuronInNetwork, NeuronContainer * firstOutputNeuronInNetwork);
	void forwardPassStep(NeuronContainer * neuronBeingAccessed);
		void adjustOutputValueOfANeuronBasedOnBackNeurons(NeuronContainer * neuronBeingAccessed);
	void backwardPassStep(NeuronContainer * neuronBeingAccessed, int isOutputLayer, bool isSubnet);
		void calculateOutputErrorOfOutputNeuron(NeuronContainer * neuronBeingAccessed);
		void calculateOutputErrorOfNonoutputNeuron(NeuronContainer * neuronBeingAccessed);
		void calculateNewBackConnectionWeightsOfNeuron(NeuronContainer * neuronBeingAccessed);
		void calculateNewBiasOfNeuron(NeuronContainer * neuronBeingAccessed);
	double calculateErrorOfBackPropPass(NeuronContainer * firstOutputNeuronInNetwork);

	void calculateErrorOfOutputNeurons(NeuronContainer * firstOutputNeuronInNetwork);







#ifdef ANN

void copyNeuronContainerListToNeuronConnectionContainerList(NeuronConnectionContainer * firstNeuronConnectionInListToUpdate, NeuronContainer * firstNeuronInListToCopy);
void copyNeuronConnectionContainerListToNeuronContainerList(NeuronContainer * firstNeuronInListToUpdate, NeuronConnectionContainer * firstNeuronConnectionInListToCopy);

//void copyNeuronListIntoNeuronReferenceArray(NeuronContainer * neuronReferenceArrayToUpdate[], NeuronContainer * firstNeuronInListToCopy);
//void copyNeuronReferenceArrayIntoNeuronList(NeuronContainer * firstNeuronInListToUpdate, NeuronContainer * neuronReferenceArrayToCopy[]);

#endif

float calculateOValue(float netValue);


void DEBUGsetDataSetNum(int newdataSetNum);


#endif


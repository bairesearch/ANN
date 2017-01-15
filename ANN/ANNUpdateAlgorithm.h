/*******************************************************************************
 * 
 * This file is part of BAIPROJECT.
 * 
 * BAIPROJECT is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License version 3
 * only, as published by the Free Software Foundation.
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
 * File Name: ANNUpdateAlgorithm.h
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2012 Baxter AI (baxterai.com)
 * Project: Advanced Neural Network (ANN)
 * Project Version: 3a12a 31-July-2012
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


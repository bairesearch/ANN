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
 * File Name: ANNupdateAlgorithm.h
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2014 Baxter AI (baxterai.com)
 * Project: Artificial Neural Network (ANN)
 * Project Version: 3e2a 29-August-2014
 * Comments:
 *
 *******************************************************************************/

#ifndef HEADER_ANN_UPDATE_ALGORITHM
#define HEADER_ANN_UPDATE_ALGORITHM

#include "ANNneuronClass.h"
#include "ANNneuronConnectionClass.h"

#include "ANNglobalDefs.h"
#ifdef DEBUG_TH_OR_IMAGE_CATEGORISTION_NN_4
extern bool debugPrintNNOutputs;
#endif

double ANNBackPropogationPass(Neuron * firstInputNeuronInNetwork, Neuron * firstOutputNeuronInNetwork);
	void forwardPassStep(Neuron * neuronBeingAccessed);
		void adjustOutputValueOfANeuronBasedOnBackNeurons(Neuron * neuronBeingAccessed);
	void backwardPassStep(Neuron * neuronBeingAccessed, int isOutputLayer, bool isSubnet);
		void calculateOutputErrorOfOutputNeuron(Neuron * neuronBeingAccessed);
		void calculateOutputErrorOfNonoutputNeuron(Neuron * neuronBeingAccessed);
		void calculateNewBackConnectionWeightsOfNeuron(Neuron * neuronBeingAccessed);
		void calculateNewBiasOfNeuron(Neuron * neuronBeingAccessed);
	double calculateErrorOfBackPropPass(Neuron * firstOutputNeuronInNetwork);

	void calculateErrorOfOutputNeurons(Neuron * firstOutputNeuronInNetwork);



#ifdef ANN_ADVANCED

void copyNeuronContainerListToNeuronConnectionContainerList(vector<NeuronConnection*> * neuronConnectionListToUpdate, Neuron * firstNeuronInListToCopy, bool frontOrBack);
void copyNeuronConnectionContainerListToNeuronContainerList(Neuron * firstNeuronInListToUpdate, vector<NeuronConnection*> * neuronConnectionListToCopy, bool frontOrBack);

#endif

float calculateOValue(float netValue);

#endif


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
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2015 Baxter AI (baxterai.com)
 * Project: Artificial Neural Network (ANN)
 * Project Version: 3f1a 10-May-2015
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

double ANNBackPropogationPass(ANNneuron* firstInputNeuronInNetwork, ANNneuron* firstOutputNeuronInNetwork);
	void forwardPassStep(ANNneuron* neuronBeingAccessed);
		void adjustOutputValueOfANeuronBasedOnBackNeurons(ANNneuron* neuronBeingAccessed);
	void backwardPassStep(ANNneuron* neuronBeingAccessed, int isOutputLayer, bool isSubnet);
		void calculateOutputErrorOfOutputNeuron(ANNneuron* neuronBeingAccessed);
		void calculateOutputErrorOfNonoutputNeuron(ANNneuron* neuronBeingAccessed);
		void calculateNewBackConnectionWeightsOfNeuron(ANNneuron* neuronBeingAccessed);
		void calculateNewBiasOfNeuron(ANNneuron* neuronBeingAccessed);
	double calculateErrorOfBackPropPass(ANNneuron* firstOutputNeuronInNetwork);

	void calculateErrorOfOutputNeurons(ANNneuron* firstOutputNeuronInNetwork);



#ifdef ANN_ADVANCED

void copyNeuronContainerListToANNneuronConnectionContainerList(vector<ANNneuronConnection*>* ANNneuronConnectionListToUpdate, ANNneuron* firstNeuronInListToCopy, bool frontOrBack);
void copyANNneuronConnectionContainerListToNeuronContainerList(ANNneuron* firstNeuronInListToUpdate, vector<ANNneuronConnection*>* ANNneuronConnectionListToCopy, bool frontOrBack);

#endif

float calculateOValue(float netValue);

#endif


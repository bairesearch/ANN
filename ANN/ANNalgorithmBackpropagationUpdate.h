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
 * File Name: ANNalgorithmBackpropagationUpdate.h
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2016 Baxter AI (baxterai.com)
 * Project: Artificial Neural Network (ANN)
 * Project Version: 4a10a 07-June-2016
 * Comments:
 *
 *******************************************************************************/


#ifndef HEADER_ANN_ALGORITHM_BACKPROPAGATION_UPDATE
#define HEADER_ANN_ALGORITHM_BACKPROPAGATION_UPDATE

#include "ANNglobalDefs.h"
#include "ANNneuronClass.h"
#include "ANNneuronConnectionClass.h"


#ifdef DEBUG_TH_OR_IMAGE_CATEGORISTION_NN_4
extern bool debugPrintNNOutputs;
#endif

#ifdef ANN_ALGORITHM_BACKPROPAGATION
double ANNbackPropogationPass(ANNneuron* firstInputNeuronInNetwork, ANNneuron* firstOutputNeuronInNetwork);
	double calculateErrorOfBackPropPass(ANNneuron* firstOutputNeuronInNetwork);
	void calculateErrorOfOutputNeurons(ANNneuron* firstOutputNeuronInNetwork);
#endif
	void backPropogationForwardPassStep(ANNneuron* neuronBeingAccessed);
		void backpropagationAdjustOutputValueOfANeuronBasedOnBackNeurons(ANNneuron* neuronBeingAccessed);
			float calculateOValue(float netValue);
#ifdef ANN_ALGORITHM_BACKPROPAGATION
	void backPropogationBackwardPassStep(ANNneuron* neuronBeingAccessed, int isOutputLayer, bool isSubnet);
		void calculateOutputErrorOfOutputNeuron(ANNneuron* neuronBeingAccessed);
		void calculateOutputErrorOfNonoutputNeuron(ANNneuron* neuronBeingAccessed);
		void calculateNewBackConnectionWeightsOfNeuron(ANNneuron* neuronBeingAccessed);
		void calculateNewBiasOfNeuron(ANNneuron* neuronBeingAccessed);
#endif

#ifdef ANN_SUBNETS
void copyNeuronContainerListToANNneuronConnectionContainerList(vector<ANNneuronConnection*>* ANNneuronConnectionListToUpdate, ANNneuron* firstNeuronInListToCopy, bool frontOrBack);
void copyANNneuronConnectionContainerListToNeuronContainerList(ANNneuron* firstNeuronInListToUpdate, vector<ANNneuronConnection*>* ANNneuronConnectionListToCopy, bool frontOrBack);
#endif




#endif


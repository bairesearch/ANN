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
 * File Name: ANNalgorithmMemoryNetworkUpdate.h
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2016 Baxter AI (baxterai.com)
 * Project: Artificial Neural Network (ANN)
 * Project Version: 3i17a 20-September-2016
 * Comments:
 *
 *******************************************************************************/


#ifndef HEADER_ANN_ALGORITHM_MEMORY_NETWORK_UPDATE
#define HEADER_ANN_ALGORITHM_MEMORY_NETWORK_UPDATE

#include "ANNglobalDefs.h"
#include "ANNneuronClass.h"
#include "ANNneuronConnectionClass.h"

#ifdef ANN_ALGORITHM_MEMORY_NETWORK

double ANNclassificationAndMemoryPass(ANNneuron* firstInputNeuronInNetwork, ANNneuron* firstOutputNeuronInNetwork, string* trainingClassificationResult, double* trainingMemoryResult);
	void calculateBinaryOutputCode(ANNneuron* firstOutputNeuronInNetwork, string* trainingClassificationResult);
	bool isNeuronOutputFire(ANNneuron* currentNeuron);
	void memoryTraceForwardPassStep(ANNneuron* neuronBeingAccessed, double* trainingMemoryResult);
		void readAndUpdateMemoryTrace(ANNneuron* currentNeuronInLayer, double* trainingMemoryResult);
			void updateMemoryTraceBasedOnSimultaneouslyFiredNeuronsIeAssocation(ANNneuronConnection* currentANNneuronConnectionSide);
			void incrementMemoryTraceTallyBasedOnSimultaneouslyFiredNeuronsIeAssocation(double* trainingMemoryResult);
	

	
#endif

#endif


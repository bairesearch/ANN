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
 * File Name: ANNneuronClass.h
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2016 Baxter AI (baxterai.com)
 * Project: Artificial Neural Network (ANN)
 * Project Version: 4a13b 14-June-2016
 * Comments:
 *
 *******************************************************************************/

 //IMPORTANT CODING NOTE - on 10-dec-06/1b6b I have started to remove the neuronReference class as circular referencing does not appear to be allowed in C++
//NB when create NeuronList class change all referecnes to "...List->firstFrontANNneuronConnectionContainer.." to "...List->neuronReferences"

/************************************************************ Neural Network Class Definitions* *****************************************************/


#ifndef HEADER_ANN_NEURON_CLASS
#define HEADER_ANN_NEURON_CLASS

#include "ANNglobalDefs.h"


#define ANN_SUBNETS
//#define ANN_DEBUG
//#define DEBUG_TRAIN_NETWORK_WITH_NON_RANDOM_VARS	//must also be defined in ANNparser.cpp
#ifdef DEBUG_TRAIN_NETWORK_WITH_NON_RANDOM_VARS
	#define ANN_DEBUG
#endif




#define DEFAULT_FIRST_INPUT_NEURON_ID 1
#define DEFAULT_FIRST_OUTPUT_NEURON_ID 2

class ANNneuronConnection;

class ANNneuron
{
public:

	ANNneuron(void);
	ANNneuron(long IDinit, long orderIDinit, long layerIDinit, long subnetIDinit);
	~ANNneuron(void);

	long id;
	long orderID;
	long layerID;
	long subnetID;

	double bias;
		double storedBias;					// (storedBias used for reset)
	double output;
	double classTarget;					//only relevant to output layer neurons
	double error;
	#ifdef ANN_ALGORITHM_CLASSIFICATION_NETWORK
	bool backNeuronMatchTemp;
	long memoryTrace;	//popularity
	bool intermediaryNeuronCreatedThisRound;
	bool neuronTypeInput;
	bool neuronTypeTopLevelCategory;
	bool neuronTypeIntermediary;
	bool printed;
	bool printedXML;
	bool topLevelCategoryAlreadyExistsDeleteThisNewCopy;
	#endif
	
	
	//for visualising highly structured networks [Eg used to visualise a 2D visual processing unit/subnet]
	bool spatialCoordinatesSet2D;
	long xPosRel;
	long yPosRel;
	long zPosRel;
	#ifdef ANN_ALGORITHM_CLASSIFICATION_NETWORK
	bool spatialCoordinatesSetClassification;
	double xPosRelFrac;
	double yPosRelFrac;
	double zPosRelFrac;
	#endif
	
	ANNneuron* nextNeuron;		//OLD: nextNeuronContainer

	bool hasFrontLayer;
	bool hasBackLayer;
	ANNneuron* firstNeuronInFrontLayer;
	ANNneuron* firstNeuronInBackLayer;

	vector<ANNneuronConnection*> frontANNneuronConnectionList;
	vector<ANNneuronConnection*> backANNneuronConnectionList;

	#ifdef ANN_ALGORITHM_MEMORY_NETWORK
	vector<ANNneuronConnection*> sideANNneuronConnectionList;
	#endif
	
	#ifdef ANN_SUBNETS
	bool isSubnet;	 	//alternative to being a subnet is being a distinct neuron

	//variables only used by subnets (abstract neurons)
	ANNneuron* firstNeuronInBackLayerOfSubnet;	//OLDname: firstNeuronContainerInBackLayerOfSubnet //this variable only used if this object is a subnet, not a neuron - the properties of the neurons in this list are direct copies of those in firstNeuronInFrontLayer
	ANNneuron* firstNeuronInFrontLayerOfSubnet;	//OLDname: firstNeuronContainerInFrontLayerOfSubnet

	bool isInputSubnet;		//input subnets are a special case - they do not require linking of their input neurons with a back layer (firstBackANNneuronConnectionContainer)
		//input subnets are used when a network is created with a selection of predefined subnets to cator for the preprocessing of different kinds of input information (Eg visual/audio etc)
	bool isOutputSubnet;	//output subnets are a special case - they do not require linking of their output neurons with a front layer (firstFrontANNneuronConnectionContainer)
	#endif
};


void fillInNeuronIDProperties(ANNneuron* neuronToUpdate, long IDinit, long orderIDinit, long layerIDinit, long subnetIDinit);


void copyNeuronKeyProperties(ANNneuron* neuronToUpdate, ANNneuron* neuronToCopy);

void averageNeuronKeyProperties(ANNneuron* neuronToUpdate, ANNneuron* neuronToCopy);

#endif

/************************************************************ End Neural Network Class Definitions* *************************************************/


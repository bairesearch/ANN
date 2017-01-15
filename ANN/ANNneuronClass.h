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
 * File Name: ANNneuronClass.h
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2012 Baxter AI (baxterai.com)
 * Project: Artificial Neural Network (ANN)
 * Project Version: 3c5c 21-December-2012
 * Comments:
 *
 *******************************************************************************/

 //IMPORTANT CODING NOTE - on 10-dec-06/1b6b I have started to remove the neuronReference class as circular referencing does not appear to be allowed in C++
//NB when create NeuronList class change all referecnes to "...List->firstFrontNeuronConnectionContainer.." to "...List->neuronReferences"

/************************************************************ Neural Network Class Definitions ******************************************************/


#ifndef HEADER_ANN_NEURON_CLASS
#define HEADER_ANN_NEURON_CLASS


#define ANN_ADVANCED
//#define ANN_DEBUG
//#define DEBUG_TRAIN_NETWORK_WITH_NON_RANDOM_VARS	//must also be defined in ANNparser.cpp
#ifdef DEBUG_TRAIN_NETWORK_WITH_NON_RANDOM_VARS
	#define ANN_DEBUG
#endif




#define DEFAULT_FIRST_INPUT_NEURON_ID 1
#define DEFAULT_FIRST_OUTPUT_NEURON_ID 2


class Neuron
{
public:

	Neuron(void);
	Neuron(long IDinit, long orderIDinit, long layerIDinit, long subnetIDinit);
	~Neuron(void);

	long id;
	long orderID;
	long layerID;
	long subnetID;

	double bias;
		double storedBias;					// (storedBias used for reset)
	double output;
	double classTarget;					//only relevant to output layer neurons
	double error;

	//for visualising highly structured networks [Eg used to visualise a 2D visual processing unit/subnet]
	bool spatialCoordinatesSet;
	long xPosRel;	//not currently used
	long yPosRel;
	long zPosRel;
};


class NeuronConnection
{
public:

	NeuronConnection(void);
	~NeuronConnection(void);

	double weight;
	double storedWeight;
};



class NeuronConnectionContainer
{
public:

	NeuronConnectionContainer(void);
	NeuronConnectionContainer(Neuron * initialNeuron);
	NeuronConnectionContainer(Neuron * initialNeuron, NeuronConnection * initialConnection);
	~NeuronConnectionContainer(void);

	long neuronID;		//temporary variable required for neural net creation from xml files
	Neuron * neuron;
	NeuronConnection * neuronConnection;
	NeuronConnectionContainer * nextNeuronConnectionContainer;
};


class NeuronContainer
{
public:

	NeuronContainer(void);
	NeuronContainer(long IDinit, long orderIDinit, long layerIDinit, long subnetIDinit);
	~NeuronContainer(void);

	Neuron * neuron;

			//neuron relationships

		//whole layer relationships
	NeuronContainer * nextNeuronContainer;		//next neuron on same layer
	//NeuronContainer * previousNeuronContainer;	REDUNDANT
	bool hasFrontLayer;
	bool hasBackLayer;
	NeuronContainer * firstNeuronInFrontLayer;
	NeuronContainer * firstNeuronInBackLayer;

		//partial layer relationships [NB all neurons on layer a do not have to be connected to all neurons on layer b]
	//long numFrontNeuronConnections;	REDUNDANT
	//long numBackNeuronConnections;	REDUNDANT
	NeuronConnectionContainer * firstFrontNeuronConnectionContainer;
	NeuronConnectionContainer * firstBackNeuronConnectionContainer;
	NeuronConnectionContainer * currentFrontNeuronConnectionContainer;	//this variable is temporary and is used for neural network Formation only
	NeuronConnectionContainer * currentBackNeuronConnectionContainer;	//this variable is temporary and is used for neural network Formation only

#ifdef ANN_ADVANCED

	bool isSubnet;	 	//alternative to being a subnet is being a distinct neuron

	//variables only used by subnets (abstract neurons)
	//long numNeuronsInBackLayerOfSubnet;	REDUNDANT
	//long numNeuronsInFrontLayerOfSubnet;	REDUNDANT
	NeuronContainer * firstNeuronContainerInBackLayerOfSubnet;	//this variable only used if this object is a subnet, not a neuron - the properties of the neurons in this list are direct copies of those in firstNeuronInFrontLayer
	NeuronContainer * firstNeuronContainerInFrontLayerOfSubnet;

	bool isInputSubnet;		//input subnets are a special case - they do not require linking of their input neurons with a back layer (firstBackNeuronConnectionContainer)
		//input subnets are used when a network is created with a selection of predefined subnets to cator for the preprocessing of different kinds of input information (Eg visual/audio etc)
	bool isOutputSubnet;	//output subnets are a special case - they do not require linking of their output neurons with a front layer (firstFrontNeuronConnectionContainer)

#endif
};





void fillInNeuronIDProperties(Neuron * neuronToUpdate, long IDinit, long orderIDinit, long layerIDinit, long subnetIDinit);


void copyNeuronKeyProperties(Neuron * neuronToUpdate, Neuron * neuronToCopy);

void averageNeuronKeyProperties(Neuron * neuronToUpdate, Neuron * neuronToCopy);

#endif

/************************************************************ End Neural Network Class Definitions **************************************************/


/*******************************************************************************
 *
 * File Name: ANNneuronClass.h
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2010 Baxter AI (baxterai.com)
 * Project: Advanced Neural Network (ANN)
 * Project Version: 3a6a 20-Mar-2012
 * Comments:
 *
 *******************************************************************************/

 //IMPORTANT CODING NOTE - on 10-dec-06/1b6b I have started to remove the neuronReference class as circular referencing does not appear to be allowed in C++
//NB when create NeuronList class change all referecnes to "...List->firstFrontNeuronConnectionContainer.." to "...List->neuronReferences"

/************************************************************ Neural Network Class Definitions ******************************************************/


#ifndef HEADER_ANN_NEURON_CLASS
#define HEADER_ANN_NEURON_CLASS


#define ANN
//#define DEBUG
//#define DEBUG_TRAIN_NETWORK_WITH_NON_RANDOM_VARS	//must also be defined in ANNParser.cpp
#ifdef DEBUG_TRAIN_NETWORK_WITH_NON_RANDOM_VARS
	#define DEBUG
#endif




#define DEFAULT_FIRST_INPUT_NEURON_ID 1
#define DEFAULT_FIRST_OUTPUT_NEURON_ID 2


class Neuron
{
public:

	Neuron(void);
	Neuron(long IDInit, long orderIDInit, long layerIDInit, long subnetIDInit);
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
	NeuronContainer(long IDInit, long orderIDInit, long layerIDInit, long subnetIDInit);
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

#ifdef ANN

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





void fillInNeuronIDProperties(Neuron * neuronToUpdate, long IDInit, long orderIDInit, long layerIDInit, long subnetIDInit);


void copyNeuronKeyProperties(Neuron * neuronToUpdate, Neuron * neuronToCopy);

void averageNeuronKeyProperties(Neuron * neuronToUpdate, Neuron * neuronToCopy);

#endif

/************************************************************ End Neural Network Class Definitions **************************************************/


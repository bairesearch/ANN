/*******************************************************************************
 *
 * No License
 * 
 * This work is under exclusive copyright (c) Baxter AI (baxterai.com). 
 * Nobody else can use, copy, distribute, or modify this work without being 
 * at risk of take-downs, shake-downs, or litigation. 
 * 
 * By publishing this source code in a public repository on GitHub, Terms of 
 * Service have been accepted by which Baxter AI have allowed others to view 
 * and fork their repository.
 * 
 * If you find software that doesn't have a license, that generally means you 
 * have no permission from the creators of the software to use, modify, or 
 * share the software. Although a code host such as GitHub may allow you to 
 * view and fork the code, this does not imply that you are permitted to use, 
 * modify, or share the software for any purpose.
 *
 * This notice has been derived from https://choosealicense.com/no-permission 
 * (https://web.archive.org/web/20180312144938/https://choosealicense.com/no-permission)
 *
 *******************************************************************************/

/*******************************************************************************
 *
 * File Name: ANNneuronClass.hpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2020 Baxter AI (baxterai.com)
 * Project: Artificial Neural Network (ANN)
 * Project Version: 3n7b 17-August-2020
 * Comments:
 * /
 *******************************************************************************/


#ifndef HEADER_ANN_NEURON_CLASS
#define HEADER_ANN_NEURON_CLASS

#include "ANNglobalDefs.hpp"


#define ANN_SUBNETS
//#define ANN_DEBUG
//#define DEBUG_TRAIN_NETWORK_WITH_NON_RANDOM_VARS	//must also be defined in ANNparser.cpp




#define DEFAULT_FIRST_INPUT_NEURON_ID 1
#define DEFAULT_FIRST_OUTPUT_NEURON_ID 2

class ANNneuronConnection;

class ANNneuron
{
public:

	ANNneuron(void);
	ANNneuron(int64_t IDinit, int64_t orderIDinit, int64_t layerIDinit, int64_t subnetIDinit);
	~ANNneuron(void);
	void initialiseNeuron();

	int64_t id;
	int64_t orderID;
	int64_t layerID;
	int64_t subnetID;

	double bias;
		double storedBias;					// (storedBias used for reset)
	double output;
	double classTarget;					//only relevant to output layer neurons
	double error;
	#ifdef ANN_ALGORITHM_CLASSIFICATION_NETWORK
	bool backNeuronMatchTemp;
	int64_t memoryTrace;	//popularity
	bool intermediaryNeuronCreatedThisRound;
	bool neuronTypeInput;
	bool neuronTypeTopLevelCategory;
	bool neuronTypeIntermediary;
	bool topLevelCategoryAlreadyExistsDeleteThisNewCopy;
	#endif
	#ifdef ANN_DRAW_PREVENT_REPRINT
	bool printed;
	#endif
	bool printedXML;

	//for visualising highly structured networks [Eg used to visualise a 2D visual processing unit/subnet]
	bool spatialCoordinatesSet2D;
	bool spatialCoordinatesSet3D;
	int64_t xPosRel;
	int64_t yPosRel;
	int64_t zPosRel;
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
	
	#ifdef ANN_ALGORITHM_GIA_NEURAL_NETWORK
	bool GIAisConceptEntity;	//#ifdefGIA_NEURAL_NETWORK_SYMBOLIC_CORE: else instance neuron. #ifdefGIA_NEURAL_NETWORK_NON_SEMANTIC: else synapse artificial instance neuron (ie synapse between dendrites; or perhaps interneuron). Dendrodendritic synapses are connections between the dendrites of two different neurons. This is in contrast to the more common axodendritic synapse 
	string GIAentityName;
	bool GIAactiveForSubnetIdentification;
	bool GIAalreadyParsed;
	int64_t GIAactivationAge;
	int GIAconceptIndexBitID;	//concept index bit neurons only
	//#ifdef GIA_NEURAL_NETWORK_SYMBOLIC_CORE_CONCEPT_INDEX_BITS
	int GIAconceptIndexType;	//concept neurons only
	int GIAconceptIndex;		//concept neurons only
	//#endif
	//#ifdef GIA_POS_REL_TRANSLATOR_NEURAL_NETWORK_SEQUENCE_GRAMMAR_WEIGHTS
	double GIAneuronStrength;
	//#endif
	#ifdef ANN_ALGORITHM_SEQUENCE_GRAMMAR_NETWORK_PRINT_COLOURS_ACTIVE
	int activationLevel;	//0: inactive, 1: partially active, 2: fully active 
	#endif
	#endif
};


class ANNneuronClassClass
{
	public: void fillInNeuronIDProperties(ANNneuron* neuronToUpdate, int64_t IDinit, int64_t orderIDinit, int64_t layerIDinit, int64_t subnetIDinit);

	private: void copyNeuronKeyProperties(ANNneuron* neuronToUpdate, ANNneuron* neuronToCopy);

	public: void averageNeuronKeyProperties(ANNneuron* neuronToUpdate, ANNneuron* neuronToCopy);
};

#endif



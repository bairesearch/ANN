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
 * File Name: ANNneuronClass.cpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2018 Baxter AI (baxterai.com)
 * Project: Artificial Neural Network (ANN)
 * Project Version: 3m13d 22-February-2018
 * Comments:
 * /
 *******************************************************************************/


#include "ANNneuronClass.hpp"

/************************************************************ Neural Network Class Definitions* *****************************************************/


ANNneuron::ANNneuron(void)
{
	id = 0;		//none = 0
	orderID = 0;	//none = 0
	layerID = 0;	//none = 0
	subnetID = 1;
	
	initialiseNeuron();
}

ANNneuron::ANNneuron(long IDinit, long orderIDinit, long layerIDinit, long subnetIDinit)
{
	id = IDinit;
	orderID = orderIDinit;
	layerID = layerIDinit;
	subnetID = subnetIDinit;

	initialiseNeuron();
}

ANNneuron::~ANNneuron(void)
{
	for(vector<ANNneuronConnection*>::iterator connectionIter = frontANNneuronConnectionList.begin(); connectionIter != frontANNneuronConnectionList.end(); connectionIter++)
	{
		if(*connectionIter != NULL)
		{
			delete *connectionIter;
		}
	}

	/*
	for(vector<ANNneuronConnection*>::iterator connectionIter = backANNneuronConnectionList.begin(); connectionIter != backANNneuronConnectionList.end(); connectionIter++)
	{
		if(*connectionIter != NULL)
		{
			#ifdef GIA_FREE_MEMORY_DEBUG
			//cout << "deleting: backANNneuronConnectionList connection: " << (*connectionIter)->frontNeuronID << endl;
			#endif
			delete* connectionIter;
		}
	}
	*/

	#ifndef ANN_ALGORITHM_CLASSIFICATION_NETWORK

	if(firstNeuronInBackLayerOfSubnet != NULL)
	{
		delete firstNeuronInBackLayerOfSubnet;
	}

	if(firstNeuronInFrontLayerOfSubnet != NULL)
	{
		delete firstNeuronInFrontLayerOfSubnet;
	}

	if(nextNeuron != NULL)
	{
		delete nextNeuron;
	}
	else
	{
		if(firstNeuronInFrontLayer != NULL)
		{
			delete firstNeuronInFrontLayer;
		}
	}

	#endif
}

void ANNneuron::initialiseNeuron()
{	
	//incase network is never trained, but output for visualisation purposes, set these values to dummy values
	bias = 0;		//this value must be initialised explicity
	storedBias = 0; 	//this value must be initialised explicity
	output = 0;		//this value must be initialised explicity
	classTarget = 0;	//this value must be initialised explicity
	error = 0;		//this value must be initialised explicity
	#ifdef ANN_ALGORITHM_CLASSIFICATION_NETWORK
	backNeuronMatchTemp = false;
	memoryTrace = 1;
	intermediaryNeuronCreatedThisRound = false;
	neuronTypeInput = false;
	neuronTypeTopLevelCategory = false;
	neuronTypeIntermediary = false;
	printed = false;
	topLevelCategoryAlreadyExistsDeleteThisNewCopy = false;
	#endif
	printedXML = false;

	spatialCoordinatesSet2D = false;
	spatialCoordinatesSet3D = false;
	xPosRel = 0;
	yPosRel = 0;
	zPosRel = 0;
	#ifdef ANN_ALGORITHM_CLASSIFICATION_NETWORK
	spatialCoordinatesSetClassification = true;
	xPosRelFrac = 0.0;
	yPosRelFrac = 0.0;
	zPosRelFrac = 0.0;
	#endif

	nextNeuron = NULL;

	hasFrontLayer = false;
	hasBackLayer = false;
	firstNeuronInFrontLayer = NULL;
	firstNeuronInBackLayer = NULL;

	#ifdef ANN_SUBNETS
	isSubnet = false;

	firstNeuronInBackLayerOfSubnet = NULL;
	firstNeuronInFrontLayerOfSubnet = NULL;

	isInputSubnet = false;
	isOutputSubnet = false;
	#endif
	
	#ifdef ANN_ALGORITHM_GIA_NEURAL_NETWORK
	GIAisConceptEntity = false;
	GIAentityName = "";
	GIAactiveForSubnetIdentification = false;
	GIAalreadyParsed = false;
	GIAactivationAge = 0;
	GIAconceptIndexBitID = INT_DEFAULT_VALUE;
	//#ifdef GIA_NEURAL_NETWORK_SYMBOLIC_CORE_CONCEPT_INDEX_BITS
	GIAconceptIndexType = INT_DEFAULT_VALUE;	//GIA_NEURAL_NETWORK_SYMBOLIC_CORE_CONCEPT_INDEX_BITS_TYPE_UNKNOWN;
	GIAconceptIndex = INT_DEFAULT_VALUE;
	//#endif
	#endif
}






void ANNneuronClassClass::fillInNeuronIDProperties(ANNneuron* neuronToUpdate, long IDinit, long orderIDinit, long layerIDinit, long subnetIDinit)
{
	neuronToUpdate->id = IDinit;
	neuronToUpdate->orderID = orderIDinit;
	neuronToUpdate->layerID = layerIDinit;
	neuronToUpdate->subnetID = subnetIDinit;
}


void ANNneuronClassClass::copyNeuronKeyProperties(ANNneuron* neuronToUpdate, ANNneuron* neuronToCopy)
{
	neuronToUpdate->bias = neuronToCopy->bias;
	neuronToUpdate->storedBias = neuronToCopy->storedBias;
	neuronToUpdate->output = neuronToCopy->output;
	neuronToUpdate->classTarget = neuronToCopy->classTarget;
	neuronToUpdate->error = neuronToCopy->error;
}

void ANNneuronClassClass::averageNeuronKeyProperties(ANNneuron* neuronToUpdate, ANNneuron* neuronToCopy)
{
	neuronToUpdate->bias = (neuronToUpdate->bias + neuronToCopy->bias)/2.0;
	neuronToUpdate->storedBias = (neuronToUpdate->storedBias + neuronToCopy->storedBias)/2.0;
	neuronToUpdate->output = (neuronToUpdate->output + neuronToCopy->output)/2.0;
	neuronToUpdate->classTarget = (neuronToUpdate->classTarget + neuronToCopy->classTarget)/2.0;
	neuronToUpdate->error = (neuronToUpdate->error + neuronToCopy->error)/2.0;
}


/************************************************************ End Neural Network Class Definitions* *************************************************/



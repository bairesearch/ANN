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
 * File Name: ANNANNneuronConnectionClass.cpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2020 Baxter AI (baxterai.com)
 * Project: Artificial Neural Network (ANN)
 * Project Version: 3o4a 17-November-2020
 * Comments:
 * /
 *******************************************************************************/


#include "ANNneuronConnectionClass.hpp"

/************************************************************ Neural Network Class Definitions* *****************************************************/


ANNneuronConnection::ANNneuronConnection(void)
{
	frontNeuronID = 0;

	weight = 0.0;		//this value must be initialised explicity
	storedWeight = 0.0; 	//this value must be initialised explicity
	#ifdef ANN_ALGORITHM_MEMORY_NETWORK
	memoryTraceConnection = false;
	memoryTrace = 0.0;
	storedMemoryTrace = 0.0;
	#endif
	#ifdef ANN_ALGORITHM_CLASSIFICATION_NETWORK
	idealValue = 0.0;	//this value must be initialised explicity
	#endif

	#ifdef ANN_ALGORITHM_GIA_NEURAL_NETWORK
	GIAconnectionType = GIA_ANN_CONNECTION_TYPE_UNDEFINED;
	GIAalreadyParsed = false;
	GIAbidirectionalSynapse = false;
	#endif
	#ifdef ANN_ALGORITHM_SANI_SEQUENCE_GRAMMAR
	//#ifdef GIA_POS_REL_TRANSLATOR_NEURAL_NETWORK_SEQUENCE_GRAMMAR_WEIGHTS
	SANIconnectionStrength = 0.0;
	//#endif
	#ifdef ANN_ALGORITHM_SANI_SEQUENCE_GRAMMAR_NETWORK_PRINT_COLOURS_COMPONENT_ORDER
	#ifdef ANN_ALGORITHM_SANI_SEQUENCE_GRAMMAR_NETWORK_PRINT_COLOURS_COMPONENT_ORDER_EXACT
	SANIcomponentIndexFirst = false;
	#else
	SANIcomponentIndex = 0;
	#endif
	#endif
	#endif
	
	frontNeuron = NULL;
	backNeuron = NULL;
}

ANNneuronConnection::~ANNneuronConnection(void)
{
	if(frontNeuron != NULL)
	{
		delete frontNeuron;
	}

	/*
	if(backNeuron != NULL)
	{
		delete backNeuron;
	}
	*/

	//do not delete neurons in this destructor
}

/************************************************************ End Neural Network Class Definitions* *************************************************/



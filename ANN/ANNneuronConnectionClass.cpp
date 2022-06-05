 /*******************************************************************************
 *
 * License: GNU Affero General Public License version 3 (AGPLv3)
 *
 *******************************************************************************/

/*******************************************************************************
 *
 * File Name: ANNANNneuronConnectionClass.cpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2022 Baxter AI (baxterai.com)
 * Project: Artificial Neural Network (ANN)
 * Project Version: 3q1a 05-June-2022
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
	#ifdef ANN_ALGORITHM_SANI_SEQUENCE_GRAMMAR_NETWORK_PRINT_COLOURS_POS
	SANIrefSetConnectionType = LRP_SHARED_ENTITY_CONNECTION_TYPE_UNDEFINED;
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



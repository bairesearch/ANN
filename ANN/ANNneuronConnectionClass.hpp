 /*******************************************************************************
 *
 * License: GNU Affero General Public License version 3 (AGPLv3)
 *
 *******************************************************************************/

/*******************************************************************************
 *
 * File Name: ANNneuronConnectionClass.hpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2022 Baxter AI (baxterai.com)
 * Project: Artificial Neural Network (ANN)
 * Project Version: 3q1a 05-June-2022
 * Comments:
 * /
 *******************************************************************************/


#ifndef HEADER_ANN_NEURON_CONNECTION_CLASS
#define HEADER_ANN_NEURON_CONNECTION_CLASS

#include "ANNglobalDefs.hpp"
#ifdef ANN_ALGORITHM_SANI_SEQUENCE_GRAMMAR
#include "LRPglobalDefs.hpp"
#endif

#ifdef ANN_ALGORITHM_GIA_NEURAL_NETWORK
#define GIA_ANN_CONNECTION_TYPE_UNDEFINED (0)
#define GIA_ANN_CONNECTION_TYPE_CONCEPT_TO_CONCEPT (1)			//primarily vertical	//concept definition
#define GIA_ANN_CONNECTION_TYPE_CONCEPT_TO_CONCEPT_PROPERTIES (2)	//primarily vertical
#define GIA_ANN_CONNECTION_TYPE_CONCEPT_TO_INSTANCE (3)			//primarily vertical
#define GIA_ANN_CONNECTION_TYPE_INSTANCE_TO_INSTANCE (4)		//primarily horizontal 
//#ifdef GIA_NEURAL_NETWORK_SYMBOLIC_CORE_CONCEPT_INDEX_BITS
#define GIA_ANN_CONNECTION_TYPE_CONCEPT_TO_CONCEPT_INDEX_BIT (5)	//primarily vertical
#define GIA_ANN_CONNECTION_TYPE_CONCEPT_INDEX_BIT_TO_INSTANCE (6)	//primarily vertical
//#endif
#endif

class ANNneuron;

class ANNneuronConnection
{
public:

	int64_t frontNeuronID;		//temporary variable required for neural net creation from xml files

	ANNneuronConnection(void);
	~ANNneuronConnection(void);

	double weight;
	double storedWeight;
	#ifdef ANN_ALGORITHM_MEMORY_NETWORK
	bool memoryTraceConnection;
	double memoryTrace;
	double storedMemoryTrace;
	#endif
	#ifdef ANN_ALGORITHM_CLASSIFICATION_NETWORK
	double idealValue;
	#ifdef ANN_VERBOSE_ALGORITHM_CLASSIFICATION_NETWORK_MERGE_SIMILAR_NEURONS
	double tempIdealValue;
	#endif
	#endif
	
	#ifdef ANN_ALGORITHM_GIA_NEURAL_NETWORK
	int GIAconnectionType;
	bool GIAalreadyParsed;
	bool GIAbidirectionalSynapse;
	#endif
	#ifdef ANN_ALGORITHM_SANI_SEQUENCE_GRAMMAR
	//#ifdef GIA_POS_REL_TRANSLATOR_NEURAL_NETWORK_SEQUENCE_GRAMMAR_WEIGHTS
	double SANIconnectionStrength;
	//#endif
	#ifdef ANN_ALGORITHM_SANI_SEQUENCE_GRAMMAR_NETWORK_PRINT_COLOURS_COMPONENT_ORDER
	#ifdef ANN_ALGORITHM_SANI_SEQUENCE_GRAMMAR_NETWORK_PRINT_COLOURS_COMPONENT_ORDER_EXACT
	bool SANIcomponentIndexFirst;
	#else
	int SANIcomponentIndex;
	#endif
	#endif
	#ifdef ANN_ALGORITHM_SANI_SEQUENCE_GRAMMAR_NETWORK_PRINT_COLOURS_POS
	int SANIrefSetConnectionType;
	#endif
	#endif

	ANNneuron* frontNeuron;
	ANNneuron* backNeuron;
};

#endif



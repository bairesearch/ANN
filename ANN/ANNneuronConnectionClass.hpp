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
 * File Name: ANNneuronConnectionClass.hpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2018 Baxter AI (baxterai.com)
 * Project: Artificial Neural Network (ANN)
 * Project Version: 3m12b 15-January-2018
 * Comments:
 *
 *******************************************************************************/



#ifndef HEADER_ANN_NEURON_CONNECTION_CLASS
#define HEADER_ANN_NEURON_CONNECTION_CLASS

#include "ANNglobalDefs.hpp"

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

	long frontNeuronID;		//temporary variable required for neural net creation from xml files

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

	ANNneuron* frontNeuron;
	ANNneuron* backNeuron;
};

#endif



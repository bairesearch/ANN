/*******************************************************************************
 *
 * This file is part of BAIPROJECT.
 *
 * BAIPROJECT is licensed under the GNU Affero General Public License
 * version 3, as published by the Free Software Foundation. The use of
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
 * File Name: ANNneuronConnectionClass.hpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2017 Baxter AI (baxterai.com)
 * Project: Artificial Neural Network (ANN)
 * Project Version: 3m6a 10-December-2017
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



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
 * Project Version: 3j3e 17-January-2017
 * Comments:
 *
 *******************************************************************************/

 //IMPORTANT CODING NOTE - on 10-dec-06/1b6b I have started to remove the neuronReference class as circular referencing does not appear to be allowed in C++
//NB when create NeuronList class change all referecnes to "...List->firstFrontANNneuronConnectionContainer.." to "...List->neuronReferences"

/************************************************************ Neural Network Class Definitions* *****************************************************/


#ifndef HEADER_ANN_NEURON_CONNECTION_CLASS
#define HEADER_ANN_NEURON_CONNECTION_CLASS

#include "ANNglobalDefs.hpp"

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
	#ifdef ANN_DEBUG_ALGORITHM_CLASSIFICATION_NETWORK_MERGE_SIMILAR_NEURONS
	double tempIdealValue;
	#endif
	#endif

	ANNneuron* frontNeuron;
	ANNneuron* backNeuron;
};

#endif

/************************************************************ End Neural Network Class Definitions* *************************************************/


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
 * File Name: ANNalgorithmMemoryNetworkTraining.hpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2017 Baxter AI (baxterai.com)
 * Project: Artificial Neural Network (ANN)
 * Project Version: 3m5a 07-December-2017
 * Comments:
 *
 *******************************************************************************/


#ifndef HEADER_ANN_ALGORITHM_MEMORY_NETWORK_TRAINING
#define HEADER_ANN_ALGORITHM_MEMORY_NETWORK_TRAINING

#include "ANNglobalDefs.hpp"
#include "ANNneuronClass.hpp"
#include "ANNneuronConnectionClass.hpp"
#include "ANNexperienceClass.hpp"
#include "ANNalgorithmMemoryNetworkUpdate.hpp"
#include "ANNdata.hpp"

#ifdef ANN_ALGORITHM_MEMORY_NETWORK

class ANNalgorithmMemoryNetworkTrainingClass
{
	private: ANNalgorithmMemoryNetworkUpdateClass ANNalgorithmMemoryNetworkUpdate;
	private: ANNexperienceClassClass ANNexperienceClass;
	public: void trainNeuralNetworkMemorySimple(ANNneuron* firstInputNeuron, const ANNneuron* firstOutputNeuron, const long numberOfInputNeurons, const long numberOfOutputNeurons, ANNexperience* firstExperienceInDataSet, const long numberOfExperiences);
	public: void trainNeuralNetworkMemory(ANNneuron* firstInputNeuron, const ANNneuron* firstOutputNeuron, const long numberOfInputNeurons, const long numberOfOutputNeurons, const int maxFolds, ANNexperience* firstExperienceInDataSet, const long numberOfExperiences);

	private: void storeNeuralNetworkMemoryTrace(ANNneuron* neuronBeingAccessed);
	private: void restoreNeuralNetworkWithStoredMemoryTrace(ANNneuron* neuronBeingAccessed);
	private: void resetNeuralNetworkWithRandomBiasAndWeightsAndEraseMemoryTrace(ANNneuron* neuronBeingAccessed);
	private: void resetInputs(ANNneuron* firstInputNeuron, const long numberOfInputNeurons, ANNexperience* currentExperienceInDataSet);
};

#endif
#endif

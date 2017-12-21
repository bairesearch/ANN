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
 * File Name: ANNalgorithmBreakawayNetworkTraining.hpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2017 Baxter AI (baxterai.com)
 * Project: Artificial Neural Network (ANN)
 * Project Version: 3m6a 10-December-2017
 * Comments:
 *
 *******************************************************************************/


#ifndef HEADER_ANN_ALGORITHM_BREAKAWAY_NETWORK_TRAINING
#define HEADER_ANN_ALGORITHM_BREAKAWAY_NETWORK_TRAINING

#include "ANNglobalDefs.hpp"
#include "ANNneuronClass.hpp"
#include "ANNneuronConnectionClass.hpp"
#include "ANNexperienceClass.hpp"
#include "ANNformation.hpp"
#include "ANNalgorithmBreakawayNetworkUpdate.hpp"
#include "ANNalgorithmBackpropagationTraining.hpp"
#include "ANNdata.hpp"
#include "SHAREDvars.hpp"

#ifdef ANN_ALGORITHM_BREAKAWAY_NETWORK

class ANNalgorithmBreakawayNetworkTrainingClass
{
	private: SHAREDvarsClass SHAREDvars;
	private: ANNformationClass ANNformation;
	private: ANNalgorithmBackpropagationTrainingClass ANNalgorithmBackpropagationTraining;
	public: bool trainNeuralNetworkBreakaway(ANNneuron* firstInputNeuronInNetwork, ANNneuron** firstOutputNeuronInNetwork, const int numberOfInputNeurons, const int numberOfOutputNeurons, const bool simple, const int numEpochs, const int maxFolds, const int maxEpochs, ANNexperience* firstExperienceInDataSet, const long numberOfExperiences, const int sizeOfSupernet, int numberOfLayersOriginal);
};

#endif
#endif

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
 * Project Version: 3l2a 12-June-2017
 * Comments:
 *
 *******************************************************************************/


#ifndef HEADER_ANN_ALGORITHM_CLASSIFICATION_NETWORK_TRAINING
#define HEADER_ANN_ALGORITHM_CLASSIFICATION_NETWORK_TRAINING

#include "ANNglobalDefs.hpp"
#include "ANNneuronClass.hpp"
#include "ANNneuronConnectionClass.hpp"
#include "ANNexperienceClass.hpp"
#include "ANNalgorithmClassificationNetworkUpdate.hpp"
#include "ANNdata.hpp"
#include "SHAREDvars.hpp"

#ifdef ANN_ALGORITHM_CLASSIFICATION_NETWORK

class ANNalgorithmClassificationNetworkTrainingClass
{
	private: SHAREDvarsClass SHAREDvars;
	public: void trainNeuralNetworkClassificationSimple(ANNneuron* firstInputNeuron, ANNneuron** firstOutputNeuron, long numberOfInputNeurons, long* numberOfOutputNeurons, ANNexperience* firstExperienceInDataSet, long numberOfExperiences);
		private: void resetIntermediaryNeuronCreatedThisRoundFlag(ANNneuron* categoryNeuron);
	#ifdef ANN_VERBOSE_ALGORITHM_CLASSIFICATION_NETWORK_ENABLE_IDEAL_VALUE_UPDATING_FOR_PARENTS
		private: void resetBackNeuronMatchTemp(ANNneuron* categoryNeuron);
	#endif
	#ifdef ANN_ALGORITHM_CLASSIFICATION_NETWORK_PRUNING
		private: void pruneNetorkBasedOnRarelyUsedNeurons(ANNneuron* categoryNeuron, ANNneuron** firstOutputNeuron);
	#endif
		private: void deleteNeuron(ANNneuron* frontNeuron, ANNneuron* categoryNeuron, ANNneuron** firstOutputNeuron);
	#ifdef ANN_VERBOSE_ALGORITHM_CLASSIFICATION_NETWORK_MERGE_SIMILAR_NEURONS
		private: void mergeSimilarNeurons(ANNneuron* categoryNeuron, ANNneuron** firstOutputNeuron);
	#endif
	#ifdef ANN_VERBOSE_ALGORITHM_CLASSIFICATION_NETWORK_CULL_REDUNDANT_NEURONS
		private: bool cullRedundantNeurons(ANNneuron* categoryNeuron, ANNneuron* parentNeuron, ANNneuron** firstOutputNeuron);
	#endif
		private: void findCategoriesForExperienceWrapper(ANNneuron* categoryNeuron, vector<bool>* inputValuesCategoryFound, ANNneuron** experienceClassificationneuronTypeTopLevelCategory, int createIntermediaryNeuronsStage);
			private: bool findCategoriesForExperience(ANNneuron* categoryNeuron, vector<bool>* inputValuesCategoryFound, int level);
			private: ANNneuronConnection* connectNeurons(ANNneuron* childNeuron, ANNneuron* parentNeuron);
			private: void updateConnectionIdealValuesParent(ANNneuron* categoryNeuron, bool updateIdealValues);
			private: void updateConnectionIdealValuesChildren(ANNneuron* categoryNeuron, bool updateIdealValues);
			private: void updateConnectionIdealValue(ANNneuronConnection* connection);
			private: void updateConnectionIdealValueNoUpdating(ANNneuronConnection* connection);
			private: double calculateIdealOutputValue(ANNneuron* categoryNeuron);
			private: double calculateOutput(double totalOutput, double numberOfCategoryInputValues);
			private: double calculateDiff(double idealValue, double value);
			private: double calculateSum(double value);

	private: string printTabsDEBUG(int level);
#ifdef ANN_ALGORITHM_CLASSIFICATION_NETWORK_ALLOW_FOR_CASE_WHERE_INTERMEDIARY_NEURON_IS_CONNECTED_TO_ALL_INPUTS
	private: void findInputs(ANNneuron* categoryNeuron, vector<bool>* inputFound, int level);
#endif

};


#endif
#endif

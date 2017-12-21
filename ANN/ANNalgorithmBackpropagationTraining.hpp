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
 * File Name: ANNalgorithmBackpropagationTraining.hpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2017 Baxter AI (baxterai.com)
 * Project: Artificial Neural Network (ANN)
 * Project Version: 3m6a 10-December-2017
 * Comments:
 *
 *******************************************************************************/


#ifndef HEADER_ANN_ALGORITHM_BACKPROPAGATION_TRAINING
#define HEADER_ANN_ALGORITHM_BACKPROPAGATION_TRAINING

#include "ANNglobalDefs.hpp"
#include "ANNneuronClass.hpp"
#include "ANNneuronConnectionClass.hpp"
#include "ANNexperienceClass.hpp"
#include "ANNalgorithmBackpropagationUpdate.hpp"
#include "ANNdata.hpp"

#define	MAX_NUM_FOLDS_ANNTH 10

#define IRRELEVANT 0.0
#define	ACCEPTABLE_ERROR_RATE 0.005
//#define	ACCEPTABLE_ERROR_RATE 0.01F	//after original, before 4 sept 10 to below
//#define	ACCEPTABLE_ERROR_RATE 0.005F	//orig
	//99.5% accuracy is defined as the acceptable error rate. no more epochs will be calculated once this is reached.

#define	ANN_DEFAULT_MAX_NUMBER_OF_EPOCHS 5000
#define	ANN_DEFAULT_SIMPLE_TRAIN_DEFAULT_NUM_OF_TRAINING_EPOCHS 1000 //OLD: 320


//#define ERROR_VARIATION_ALLOWED 0.002F	//after original, before 4 sept 10 to below
//#define ERROR_VARIATION_ALLOWED 0.01F	//1%	//original
#define ERROR_VARIATION_ALLOWED 0.002F

#define REAL_LIFE_ACCURACY_DIAGNOSTICS_CUTOFF 0.1

#ifdef ANN_ALGORITHM_BACKPROPAGATION

class ANNalgorithmBackpropagationTrainingClass
{
	private: ANNalgorithmBackpropagationUpdateClass ANNalgorithmBackpropagationUpdate;
	private: ANNexperienceClassClass ANNexperienceClass;
	private: double calculateExperienceErrorForHypotheticalDecisionBackpropagation(ANNneuron* firstInputNeuronInNetwork, ANNneuron* firstOutputNeuronInNetwork, const long numberOfInputNeurons, const long numberOfOutputNeurons, ANNexperience* experience);

	private: void feedNeuralNetworkWithASetOfExperiencesBackpropagation(ANNneuron* firstInputNeuron, ANNneuron* firstOutputNeuron, const int numberOfInputNeurons, const int numberOfOutputNeurons, ANNexperience* firstExperienceInList);

	public: void trainNeuralNetworkBackpropagationSimple(ANNneuron* firstInputNeuron, ANNneuron* firstOutputNeuron, const int numberOfInputNeurons, const int numberOfOutputNeurons, const int numEpochs, ANNexperience* firstExperienceInDataSet, const long numberOfExperiences);
		public: void trainNeuralNetworkBackpropagationSimpleNoReset(ANNneuron* firstInputNeuron, ANNneuron* firstOutputNeuron, const int numberOfInputNeurons, const int numberOfOutputNeurons, const int numEpochs, ANNexperience* firstExperienceInDataSet, const long numberOfExperiences);
	public: void trainNeuralNetworkBackpropagation(ANNneuron* firstInputNeuron, ANNneuron* firstOutputNeuron, const int numberOfInputNeurons, const int numberOfOutputNeurons, const int maxFolds, ANNexperience* firstExperienceInDataSet, const long numberOfExperiences, const int maxEpochs);

		private: void storeNeuralNetworkBiasAndWeights(ANNneuron* neuronBeingAccessed);
		private: void restoreNeuralNetworkWithStoredBiasAndWeights(ANNneuron* neuronBeingAccessed);
		public: void resetNeuralNetworkWithRandomBiasAndWeights(ANNneuron* neuronBeingAccessed);
		private: void resetInputsAndClassTargets(ANNneuron* firstInputNeuron, ANNneuron* firstOutputNeuron, const long numberOfInputNeurons, const long numberOfOutputNeurons, ANNexperience* currentExperienceInDataSet);
		private: float calculateStandardDeviation(const float* array, const int length, const float average);
};


#endif
#endif

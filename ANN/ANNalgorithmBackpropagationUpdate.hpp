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
 * File Name: ANNalgorithmBackpropagationUpdate.hpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2017 Baxter AI (baxterai.com)
 * Project: Artificial Neural Network (ANN)
 * Project Version: 3m7a 13-December-2017
 * Comments:
 *
 *******************************************************************************/


#ifndef HEADER_ANN_ALGORITHM_BACKPROPAGATION_UPDATE
#define HEADER_ANN_ALGORITHM_BACKPROPAGATION_UPDATE

#include "ANNglobalDefs.hpp"
#include "ANNneuronClass.hpp"
#include "ANNneuronConnectionClass.hpp"



class ANNalgorithmBackpropagationUpdateClass
{
	private: ANNneuronClassClass ANNneuronClass;
	#ifdef ANN_ALGORITHM_BACKPROPAGATION
	public: double ANNbackPropogationPass(ANNneuron* firstInputNeuronInNetwork, ANNneuron* firstOutputNeuronInNetwork);
		private: double calculateErrorOfBackPropPass(const ANNneuron* firstOutputNeuronInNetwork);
		private: void calculateErrorOfOutputNeurons(ANNneuron* firstOutputNeuronInNetwork);
	#endif
		public: void backPropogationForwardPassStep(ANNneuron* neuronBeingAccessed);
			private: void backpropagationAdjustOutputValueOfANeuronBasedOnBackNeurons(ANNneuron* neuronBeingAccessed);
				private: float calculateOValue(float netValue);
		#ifdef ANN_ALGORITHM_BACKPROPAGATION
		private: void backPropogationBackwardPassStep(ANNneuron* neuronBeingAccessed, const int isOutputLayer, const bool isSubnet);
			private: void calculateOutputErrorOfOutputNeuron(ANNneuron* neuronBeingAccessed);
			private: void calculateOutputErrorOfNonoutputNeuron(ANNneuron* neuronBeingAccessed);
			private: void calculateNewBackConnectionWeightsOfNeuron(ANNneuron* neuronBeingAccessed);
			private: void calculateNewBiasOfNeuron(ANNneuron* neuronBeingAccessed);
		#endif

	#ifdef ANN_SUBNETS
	public: void copyNeuronContainerListToANNneuronConnectionContainerList(vector<ANNneuronConnection*>* ANNneuronConnectionListToUpdate, ANNneuron* firstNeuronInListToCopy, const bool frontOrBack);
	public: void copyANNneuronConnectionContainerListToNeuronContainerList(ANNneuron* firstNeuronInListToUpdate, vector<ANNneuronConnection*>* ANNneuronConnectionListToCopy, const bool frontOrBack);
	#endif
};





#endif


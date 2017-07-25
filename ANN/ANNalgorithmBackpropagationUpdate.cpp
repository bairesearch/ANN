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
 * File Name: ANNalgorithmBackpropagationUpdate.cpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2017 Baxter AI (baxterai.com)
 * Project: Artificial Neural Network (ANN)
 * Project Version: 3m3a 18-July-2017
 * Comments:
 *
 *******************************************************************************/


#include "ANNalgorithmBackpropagationUpdate.hpp"



int dataSetNum;


/************************************************************ Main Neural Network Learning Routines with a class Target set* *****************************************************/
//preconditions; neural network is already filled with all necessary weights/biases/inputneuron values




#ifdef ANN_ALGORITHM_BACKPROPAGATION
double ANNalgorithmBackpropagationUpdateClass::ANNbackPropogationPass(ANNneuron* firstInputNeuronInNetwork, ANNneuron* firstOutputNeuronInNetwork)
{

	this->backPropogationForwardPassStep(firstInputNeuronInNetwork);

	this->backPropogationBackwardPassStep(firstOutputNeuronInNetwork, true, false);

	return this->calculateErrorOfBackPropPass(firstOutputNeuronInNetwork);
}

double ANNalgorithmBackpropagationUpdateClass::calculateErrorOfBackPropPass(const ANNneuron* firstOutputNeuronInNetwork)
{
	/*

	Error for signle training example over all output neurons:

	E = 1/2 SUM(0->i) e(i)^2 = 1/2 SUM (d(i) - o(i))^2

	therefore...

	*/

	double total2xError = 0.0;

	const ANNneuron* currentNeuron = firstOutputNeuronInNetwork;
	while(currentNeuron->nextNeuron != NULL)
	{

		total2xError = total2xError + pow((currentNeuron->classTarget - currentNeuron->output), 2.0);

		/*total2xError = total2xError + pow((classTarget[i] - outputs2[i]),2);*/


		currentNeuron = currentNeuron->nextNeuron;
	}

	float totalError = 0.5F* total2xError;



	return total2xError;
}

void ANNalgorithmBackpropagationUpdateClass::calculateErrorOfOutputNeurons(ANNneuron* firstOutputNeuronInNetwork)
{
	//NB calculate error of output neurons
	ANNneuron* currentNeuron = firstOutputNeuronInNetwork;
	while(currentNeuron->nextNeuron != NULL)
	{
		this->calculateOutputErrorOfOutputNeuron(currentNeuron);
		currentNeuron = currentNeuron->nextNeuron;
	}
}
#endif
/************************************************************ End Main Neural Network Learning Routines with a class Target set* **************************************************/







/************************************************************ Forward Pass Step Routines* *****************************************************/

void ANNalgorithmBackpropagationUpdateClass::backPropogationForwardPassStep(ANNneuron* neuronBeingAccessed)
{

	ANNneuron* currentNeuron = neuronBeingAccessed->firstNeuronInFrontLayer;

	while(currentNeuron->nextNeuron != NULL)
	{

		if(!(currentNeuron->isSubnet))
		{
			this->backpropagationAdjustOutputValueOfANeuronBasedOnBackNeurons(currentNeuron);
		}

		currentNeuron = currentNeuron->nextNeuron;
	}

	//ANN recursion
	#ifdef ANN_SUBNETS
	//NB if !isInputSubnet, subnets require a back layer to exist in the top level network
	currentNeuron = neuronBeingAccessed;
	while(currentNeuron->nextNeuron != NULL)
	{
		if(currentNeuron->isSubnet)
		{

			if(!(currentNeuron->isInputSubnet))
			{
				this->copyANNneuronConnectionContainerListToNeuronContainerList(currentNeuron->firstNeuronInBackLayerOfSubnet, &(currentNeuron->backANNneuronConnectionList), false);
			}

			this->copyANNneuronConnectionContainerListToNeuronContainerList(currentNeuron->firstNeuronInFrontLayerOfSubnet, &(currentNeuron->frontANNneuronConnectionList), true);
			this->backPropogationForwardPassStep(currentNeuron->firstNeuronInBackLayerOfSubnet);	//?ISSUE HERE

			if(!(currentNeuron->isInputSubnet))
			{
				this->copyNeuronContainerListToANNneuronConnectionContainerList(&(currentNeuron->backANNneuronConnectionList), currentNeuron->firstNeuronInBackLayerOfSubnet, false);
			}

			this->copyNeuronContainerListToANNneuronConnectionContainerList(&(currentNeuron->frontANNneuronConnectionList), currentNeuron->firstNeuronInFrontLayerOfSubnet, true);
		}
		currentNeuron = currentNeuron->nextNeuron;
	}
	#endif

	//recursion
	if(neuronBeingAccessed->firstNeuronInFrontLayer->hasFrontLayer)
	{
		this->backPropogationForwardPassStep(neuronBeingAccessed->firstNeuronInFrontLayer);
	}
}


//Only [top level]/[foward level]/[final] Output Neurons have "Class Target" values
void ANNalgorithmBackpropagationUpdateClass::backpropagationAdjustOutputValueOfANeuronBasedOnBackNeurons(ANNneuron* neuronBeingAccessed)
{

	//for every ANNneuron being accessed

	float netI = 0.0;
	for(vector<ANNneuronConnection*>::iterator connectionIter = neuronBeingAccessed->backANNneuronConnectionList.begin(); connectionIter != neuronBeingAccessed->backANNneuronConnectionList.end(); connectionIter++)
	{
		ANNneuronConnection* currentANNneuronConnection = *connectionIter;

		netI = netI + (currentANNneuronConnection->backNeuron->output* currentANNneuronConnection->weight);


	}

	netI = netI + neuronBeingAccessed->bias;

	neuronBeingAccessed->output = this->calculateOValue(netI);	//set ("hidden") ANNneuron output value

}

float ANNalgorithmBackpropagationUpdateClass::calculateOValue(float netValue)
{
	#ifndef DEBUG_TH_OR_IMAGE_CATEGORISTION_NN_USE_LINEAR_COMBINATION_NETWORK
	float f = (1.0/(1.0 + float(exp(-netValue))));	//modified 1-6-04
	#endif

	return f;
}



/************************************************************ End Forward Pass Step Routines* *************************************************/











#ifdef ANN_ALGORITHM_BACKPROPAGATION





/************************************************************ Backward Pass Step Routines* *****************************************************/


//preconditions - assumes neuronBeingAccessed has a back neuron references. (ie neuronBeingAccessed->firstBackNeuronReference != NULL)
void ANNalgorithmBackpropagationUpdateClass::backPropogationBackwardPassStep(ANNneuron* neuronBeingAccessed, const int isOutputLayer, const bool isSubnet)
{

	if((isOutputLayer) && (!isSubnet))
	{	//calculate errors of output neurons - errors2 (output errors)

		//NB calculate error of neurons at current level
		ANNneuron* currentNeuron = neuronBeingAccessed;
		while(currentNeuron->nextNeuron != NULL)
		{
			this->calculateOutputErrorOfOutputNeuron(currentNeuron);
			currentNeuron = currentNeuron->nextNeuron;
		}
	}
	else
	{//calculates the errors of the hidden neurons
		ANNneuron* currentNeuron = neuronBeingAccessed;
		while(currentNeuron->nextNeuron != NULL)
		{
			if(!(currentNeuron->isSubnet))
			{
				this->calculateOutputErrorOfNonoutputNeuron(currentNeuron);
			}
			currentNeuron = currentNeuron->nextNeuron;
		}
	}

	//NB calculate back Weight of neurons at current level {NB if at the back layer of a subnet, nothing will happen}
	ANNneuron* currentNeuron = neuronBeingAccessed;
	while(currentNeuron->nextNeuron != NULL)
	{
		if(!(currentNeuron->isSubnet))
		{
			this->calculateNewBackConnectionWeightsOfNeuron(currentNeuron);
		}
		currentNeuron = currentNeuron->nextNeuron;
	}

	//NB calculate bias for neurons at current level
	currentNeuron = neuronBeingAccessed;
	while(currentNeuron->nextNeuron != NULL)
	{
		if(!(currentNeuron->isSubnet))
		{
			this->calculateNewBiasOfNeuron(currentNeuron);
		}
		currentNeuron = currentNeuron->nextNeuron;
	}

	//ANN recursion
#ifdef ANN_SUBNETS
	//NBOLD if !isOutputSubnet, subnets require a front layer to exist in the top level network
	currentNeuron = neuronBeingAccessed;
	while(currentNeuron->nextNeuron != NULL)
	{
		if(currentNeuron->isSubnet)
		{

			this->copyANNneuronConnectionContainerListToNeuronContainerList(currentNeuron->firstNeuronInBackLayerOfSubnet, &(currentNeuron->backANNneuronConnectionList), false);
			if(!(currentNeuron->isOutputSubnet))
			{
				this->copyANNneuronConnectionContainerListToNeuronContainerList(currentNeuron->firstNeuronInFrontLayerOfSubnet, &(currentNeuron->frontANNneuronConnectionList), true);
			}

			this->backPropogationBackwardPassStep(currentNeuron->firstNeuronInFrontLayerOfSubnet, true, true);
				//NB CHECK ANNTHIS new requirement; each subnet must have at least 3 layers

			this->copyNeuronContainerListToANNneuronConnectionContainerList(&(currentNeuron->backANNneuronConnectionList), currentNeuron->firstNeuronInBackLayerOfSubnet, false);
			if(!(currentNeuron->isOutputSubnet))
			{
				this->copyNeuronContainerListToANNneuronConnectionContainerList(&(currentNeuron->frontANNneuronConnectionList), currentNeuron->firstNeuronInFrontLayerOfSubnet, true);
			}
		}
		currentNeuron = currentNeuron->nextNeuron;
	}
#endif

	//recursion
	if(isSubnet)
	{
		if(neuronBeingAccessed->hasBackLayer)
		{
			this->backPropogationBackwardPassStep(neuronBeingAccessed->firstNeuronInBackLayer, false, isSubnet);
		}
	}
	else
	{
		if(neuronBeingAccessed->firstNeuronInBackLayer->hasBackLayer)
		{
			this->backPropogationBackwardPassStep(neuronBeingAccessed->firstNeuronInBackLayer, false, isSubnet);
		}
	}
}


void ANNalgorithmBackpropagationUpdateClass::calculateOutputErrorOfOutputNeuron(ANNneuron* neuronBeingAccessed)
{

	double outputError = (neuronBeingAccessed->classTarget - neuronBeingAccessed->output)* neuronBeingAccessed->output* (1.0 - neuronBeingAccessed->output);
	neuronBeingAccessed->error = outputError;

}

void ANNalgorithmBackpropagationUpdateClass::calculateOutputErrorOfNonoutputNeuron(ANNneuron* neuronBeingAccessed)
{


	float partOfHiddenError = 0.0;

	for(vector<ANNneuronConnection*>::iterator connectionIter = neuronBeingAccessed->frontANNneuronConnectionList.begin(); connectionIter != neuronBeingAccessed->frontANNneuronConnectionList.end(); connectionIter++)
	{
		ANNneuronConnection* currentANNneuronConnection = *connectionIter;

		//numberOfChickens++;

		partOfHiddenError = partOfHiddenError + ((currentANNneuronConnection->weight)* (currentANNneuronConnection->frontNeuron->error));
		/*partOfHiddenError = partOfHiddenError + (weights2[i*numberOfOutputNeurons+j]*errors2[j]);*/

	}

	neuronBeingAccessed->error = (neuronBeingAccessed->output)* (1.0 - (neuronBeingAccessed->output))* partOfHiddenError;

}


void ANNalgorithmBackpropagationUpdateClass::calculateNewBackConnectionWeightsOfNeuron(ANNneuron* neuronBeingAccessed)
{


	for(vector<ANNneuronConnection*>::iterator connectionIter = neuronBeingAccessed->backANNneuronConnectionList.begin(); connectionIter != neuronBeingAccessed->backANNneuronConnectionList.end(); connectionIter++)
	{
		ANNneuronConnection* currentANNneuronConnection = *connectionIter;

		float weightChange;
		weightChange = MU* neuronBeingAccessed->error* currentANNneuronConnection->backNeuron->output;

		currentANNneuronConnection->weight = currentANNneuronConnection->weight + weightChange;

	}
}

void ANNalgorithmBackpropagationUpdateClass::calculateNewBiasOfNeuron(ANNneuron* neuronBeingAccessed)
{

	double biasChange;
	biasChange = MU* neuronBeingAccessed->error* 1.0;	//BIASES ARE WEIGHTED WITH INPUT 1
	neuronBeingAccessed->bias = neuronBeingAccessed->bias + biasChange;

}



/************************************************************ End Backward Pass Step Routines* *************************************************/


#endif




#ifdef ANN_SUBNETS

void ANNalgorithmBackpropagationUpdateClass::copyNeuronContainerListToANNneuronConnectionContainerList(vector<ANNneuronConnection*>* ANNneuronConnectionListToUpdate, ANNneuron* firstNeuronInListToCopy, const bool frontOrBack)
{
	ANNneuron* currentNeuronToCopy = firstNeuronInListToCopy;

	for(vector<ANNneuronConnection*>::iterator ANNneuronConnectionListToUpdateIter = ANNneuronConnectionListToUpdate->begin(); ANNneuronConnectionListToUpdateIter != ANNneuronConnectionListToUpdate->end(); ANNneuronConnectionListToUpdateIter++)
	{
		//average
		if(frontOrBack)
		{
			ANNneuronClass.averageNeuronKeyProperties((*ANNneuronConnectionListToUpdateIter)->frontNeuron, currentNeuronToCopy);
		}
		else
		{
			ANNneuronClass.averageNeuronKeyProperties((*ANNneuronConnectionListToUpdateIter)->backNeuron, currentNeuronToCopy);
		}
		currentNeuronToCopy = currentNeuronToCopy->nextNeuron;
	}
}

void ANNalgorithmBackpropagationUpdateClass::copyANNneuronConnectionContainerListToNeuronContainerList(ANNneuron* firstNeuronInListToUpdate, vector<ANNneuronConnection*>* ANNneuronConnectionListToCopy, const bool frontOrBack)
{
	ANNneuron* currentNeuronToUpdate = firstNeuronInListToUpdate;
	vector<ANNneuronConnection*>::iterator ANNneuronConnectionListToCopyIter = ANNneuronConnectionListToCopy->begin();

	while(currentNeuronToUpdate->nextNeuron != NULL)		//?this will fail; it appears as the neuron connection containers have less neuron connections than neurons in the layer - this is a mistake
	//while(currentANNneuronConnectionContainerToCopy -> nextANNneuronConnectionContainer != NULL)
	{
		//average
		if(frontOrBack)
		{
			ANNneuronClass.averageNeuronKeyProperties(currentNeuronToUpdate, (*ANNneuronConnectionListToCopyIter)->frontNeuron);
		}
		else
		{
			ANNneuronClass.averageNeuronKeyProperties(currentNeuronToUpdate, (*ANNneuronConnectionListToCopyIter)->backNeuron);
		}
		currentNeuronToUpdate = currentNeuronToUpdate->nextNeuron;
		ANNneuronConnectionListToCopyIter++;
	}
}

#endif

















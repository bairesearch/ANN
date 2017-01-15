/*******************************************************************************
 *
 * This file is part of BAIPROJECT.
 *
 * BAIPROJECT is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License version 3
 * only, as published by the Free Software Foundation. The use of
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
 * File Name: ANNupdateAlgorithm.cpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2015 Baxter AI (baxterai.com)
 * Project: Artificial Neural Network (ANN)
 * Project Version: 3h15b 29-February-2016
 * Comments:
 *
 *******************************************************************************/


#include "ANNupdateAlgorithm.h"
#include "ANNneuronClass.h"

#define	MU 0.1F




int dataSetNum;


/************************************************************ Main Neural Network Learning Routines with a class Target set* *****************************************************/
//preconditions; neural network is already filled with all necessary weights/biases/inputneuron values


#ifdef DEBUG_TH_OR_IMAGE_CATEGORISTION_NN_4
bool debugPrintNNOutputs;
#endif

double calculateErrorOfBackPropPass(ANNneuron* firstOutputNeuronInNetwork)
{
	/*

	Error for signle training example over all output neurons:

	E = 1/2 SUM(0->i) e(i)^2 = 1/2 SUM (d(i) - o(i))^2

	therefore...

	*/

	double total2xError = 0.0;

	ANNneuron* currentNeuron = firstOutputNeuronInNetwork;
	while(currentNeuron->nextNeuron != NULL)
	{
		#ifdef DEBUG_TH_OR_IMAGE_CATEGORISTION_NN_4
		if(debugPrintNNOutputs)
		{
			cout << "currentNeuron->classTarget = " << currentNeuron->classTarget << endl;
			cout << "currentNeuron->output = " << currentNeuron->output << endl;
		}
		#endif

		total2xError = total2xError + pow((currentNeuron->classTarget - currentNeuron->output), 2.0);

		/*total2xError = total2xError + pow((classTarget[i] - outputs2[i]),2);*/

		#ifdef ANN_DEBUG
		cout << "\nNeural Network Total Error Calculations" << endl;
		cout << "ANNneuronContainer ID = " << currentNeuron->id << endl;
		cout << "ANNneuronContainer ID Order = " << currentNeuron->orderID << endl;
		cout << "ANNneuronContainer ID Layer = " << currentNeuron->layerID << endl;
		cout << "ANNneuronContainer ID Subnet = " << currentNeuron->subnetID << endl;
		cout << "ANNneuronContainer classTarget = " << currentNeuron->classTarget << endl;
		cout << "ANNneuronContainer Output = " << currentNeuron->output << endl;
		cout << "total2xError = " << total2xError << "\n" << endl;
		#endif

		currentNeuron = currentNeuron->nextNeuron;
	}

	float totalError = 0.5F* total2xError;

	#ifdef DEBUG_TH_OR_IMAGE_CATEGORISTION_NN_4
	if(debugPrintNNOutputs)
	{
		cout << "\ttotal error = " << totalError << "\n" << endl;
	}
	#endif

	#ifdef ANN_DEBUG
	cout << "total error = " << totalError << "\n" << endl;
	#endif

	return total2xError;
}

double ANNBackPropogationPass(ANNneuron* firstInputNeuronInNetwork, ANNneuron* firstOutputNeuronInNetwork)
{
	#ifdef ANN_DEBUG
	cout << "\ndouble ANNBackPropogationPass(ANNneuron* firstInputNeuronInNetwork, ANNneuron* firstOutputNeuronInNetwork)" << endl;
	#endif

	forwardPassStep(firstInputNeuronInNetwork);

	backwardPassStep(firstOutputNeuronInNetwork, true, false);

	return calculateErrorOfBackPropPass(firstOutputNeuronInNetwork);
}
/************************************************************ End Main Neural Network Learning Routines with a class Target set* **************************************************/





#ifdef ANN_ADVANCED

void copyNeuronContainerListToANNneuronConnectionContainerList(vector<ANNneuronConnection*>* ANNneuronConnectionListToUpdate, ANNneuron* firstNeuronInListToCopy, bool frontOrBack)
{
	ANNneuron* currentNeuronToCopy = firstNeuronInListToCopy;

	for(vector<ANNneuronConnection*>::iterator ANNneuronConnectionListToUpdateIter = ANNneuronConnectionListToUpdate->begin(); ANNneuronConnectionListToUpdateIter != ANNneuronConnectionListToUpdate->end(); ANNneuronConnectionListToUpdateIter++)
	{
		//average
		if(frontOrBack)
		{
			averageNeuronKeyProperties((*ANNneuronConnectionListToUpdateIter)->frontNeuron, currentNeuronToCopy);
		}
		else
		{
			averageNeuronKeyProperties((*ANNneuronConnectionListToUpdateIter)->backNeuron, currentNeuronToCopy);
		}
		currentNeuronToCopy = currentNeuronToCopy->nextNeuron;
	}
}

void copyANNneuronConnectionContainerListToNeuronContainerList(ANNneuron* firstNeuronInListToUpdate, vector<ANNneuronConnection*>* ANNneuronConnectionListToCopy, bool frontOrBack)
{
	ANNneuron* currentNeuronToUpdate = firstNeuronInListToUpdate;
	vector<ANNneuronConnection*>::iterator ANNneuronConnectionListToCopyIter = ANNneuronConnectionListToCopy->begin();

	while(currentNeuronToUpdate->nextNeuron != NULL)		//?this will fail; it appears as the neuron connection containers have less neuron connections than neurons in the layer - this is a mistake
	//while(currentANNneuronConnectionContainerToCopy -> nextANNneuronConnectionContainer != NULL)
	{
		//average
		if(frontOrBack)
		{
			averageNeuronKeyProperties(currentNeuronToUpdate, (*ANNneuronConnectionListToCopyIter)->frontNeuron);
		}
		else
		{
			averageNeuronKeyProperties(currentNeuronToUpdate, (*ANNneuronConnectionListToCopyIter)->backNeuron);
		}
		currentNeuronToUpdate = currentNeuronToUpdate->nextNeuron;
		ANNneuronConnectionListToCopyIter++;
	}
}

#endif


/************************************************************ Forward Pass Step Routines* *****************************************************/

void forwardPassStep(ANNneuron* neuronBeingAccessed)
{
	#ifdef ANN_DEBUG
	cout << "\nstatic void forwardPassStep(ANNneuron* neuronBeingAccessed)" << endl;
	cout << "ANNneuronContainer ID = " << neuronBeingAccessed->id << endl;
	cout << "ANNneuronContainer ID Order = " << neuronBeingAccessed->orderID << endl;
	cout << "ANNneuronContainer ID Layer = " << neuronBeingAccessed->layerID << endl;
	cout << "ANNneuronContainer ID Subnet = " << neuronBeingAccessed->subnetID << endl;
	#endif

	ANNneuron* currentNeuron = neuronBeingAccessed->firstNeuronInFrontLayer;

	while(currentNeuron->nextNeuron != NULL)
	{
		#ifdef ANN_DEBUG
		cout << "\nA Hidden level ANNneuronContainer has been selected for Learning" << endl;
		cout << "ANNneuronContainer ID = " << currentNeuron->id << endl;
		cout << "ANNneuronContainer ID Order = " << currentNeuron->orderID << endl;
		cout << "ANNneuronContainer ID Layer = " << currentNeuron->layerID << endl;
		cout << "ANNneuronContainer ID Subnet = " << currentNeuron->subnetID << endl;
		#endif

		if(!(currentNeuron->isSubnet))
		{
			adjustOutputValueOfANeuronBasedOnBackNeurons(currentNeuron);
			//cout << "currentNeuron->output  = " << currentNeuron->output << endl;
		}

		currentNeuron = currentNeuron->nextNeuron;
	}

	//ANN recursion
	#ifdef ANN_ADVANCED
	//NB if !isInputSubnet, subnets require a back layer to exist in the top level network
	currentNeuron = neuronBeingAccessed;
	while(currentNeuron->nextNeuron != NULL)
	{
		if(currentNeuron->isSubnet)
		{
			#ifdef ANN_DEBUG
			cout << "\nAbout to perform ANN recursion in forwardPassStep ... " << endl;
			cout << "currentNeuron ANNneuronContainer ID = " << currentNeuron->id << endl;
			cout << "currentNeuron ANNneuronContainer ID Order = " << currentNeuron->orderID << endl;
			cout << "currentNeuron ANNneuronContainer ID Layer = " << currentNeuron->layerID << endl;
			cout << "currentNeuron ANNneuronContainer ID Subnet = " << currentNeuron->subnetID << endl;
			//cout << "numberOfInputNeurons = " << currentNeuron->numNeuronsInBackLayerOfSubnet << endl;
			//cout << "numberOfOutputNeurons = " <<  currentNeuron->numNeuronsInFrontLayerOfSubnet << endl;
			#endif

			if(!(currentNeuron->isInputSubnet))
			{
				copyANNneuronConnectionContainerListToNeuronContainerList(currentNeuron->firstNeuronInBackLayerOfSubnet, &(currentNeuron->backANNneuronConnectionList), false);
			}

			copyANNneuronConnectionContainerListToNeuronContainerList(currentNeuron->firstNeuronInFrontLayerOfSubnet, &(currentNeuron->frontANNneuronConnectionList), true);
			forwardPassStep(currentNeuron->firstNeuronInBackLayerOfSubnet);	//?ISSUE HERE

			if(!(currentNeuron->isInputSubnet))
			{
				copyNeuronContainerListToANNneuronConnectionContainerList(&(currentNeuron->backANNneuronConnectionList), currentNeuron->firstNeuronInBackLayerOfSubnet, false);
			}

			copyNeuronContainerListToANNneuronConnectionContainerList(&(currentNeuron->frontANNneuronConnectionList), currentNeuron->firstNeuronInFrontLayerOfSubnet, true);
		}
		currentNeuron = currentNeuron->nextNeuron;
	}
	#endif

	//recursion
	if(neuronBeingAccessed->firstNeuronInFrontLayer->hasFrontLayer)
	{
		forwardPassStep(neuronBeingAccessed->firstNeuronInFrontLayer);
	}
}


//Only [top level]/[foward level]/[final] Output Neurons have "Class Target" values
void adjustOutputValueOfANeuronBasedOnBackNeurons(ANNneuron* neuronBeingAccessed)
{
	#ifdef ANN_DEBUG
	cout << "\nstatic void adjustOutputValueOfANeuronBasedOnBackNeurons(ANNneuron* neuronBeingAccessed)" << endl;
	cout << "\nNeuron Output Value Adjusted" << endl;
	cout << "ANNneuronContainer ID = " << neuronBeingAccessed->id << endl;
	cout << "ANNneuronContainer ID Order = " << neuronBeingAccessed->orderID << endl;
	cout << "ANNneuronContainer ID Layer = " << neuronBeingAccessed->layerID << endl;
	cout << "ANNneuronContainer ID Subnet = " << neuronBeingAccessed->subnetID << endl;
	#endif

	//for every ANNneuronContainer being accessed

	float netI = 0.0;
	for(vector<ANNneuronConnection*>::iterator connectionIter = neuronBeingAccessed->backANNneuronConnectionList.begin(); connectionIter != neuronBeingAccessed->backANNneuronConnectionList.end(); connectionIter++)
	{
		ANNneuronConnection* currentANNneuronConnection = *connectionIter;
		#ifdef ANN_DEBUG
		//cout << "currentANNneuronConnection->backNeuron->output = " << currentANNneuronConnection->backNeuron->output << endl;
		#endif

		netI = netI + (currentANNneuronConnection->backNeuron->output* currentANNneuronConnection->weight);

		#ifdef DEBUG_TRAIN_NETWORK_WITH_NON_RANDOM_VARS
		cout << "currentANNneuronConnection->backNeuron->id = " << currentANNneuronConnection->backNeuron->id << endl;
		#endif

		#ifdef ANN_DEBUG
		cout << "Back ANNneuron Output = " << currentANNneuronConnection->backNeuron->output << endl;
		cout << "Back ANNneuron Connection Weight = " << currentANNneuronConnection->weight << endl;
		cout << "ANNneuronContainer netI = " << netI << endl;
		/*
		cout << "currentANNneuronConnection->weight > 100" << endl;
		cout << "currentANNneuronConnection->weight = " << currentANNneuronConnection->weight << endl;
		cout << "dataSetNum = " << dataSetNum << endl;
		cout << "\tneuronBeingAccessed ID = " << neuronBeingAccessed->id << endl;
		cout << "\tneuronBeingAccessed ID Order = " << neuronBeingAccessed->orderID << endl;
		cout << "\tneuronBeingAccessed ID Layer = " << neuronBeingAccessed->layerID << endl;
		cout << "\tneuronBeingAccessed ID Subnet = " << neuronBeingAccessed->subnetID << endl;
		cout << "\t\tcurrentANNneuronConnection->backNeuron ID = " << currentANNneuronConnection->backNeuron->id << endl;
		cout << "\t\tcurrentANNneuronConnection->backNeuron ID Order = " << currentANNneuronConnection->backNeuron->orderID << endl;
		cout << "\t\tcurrentANNneuronConnection->backNeuron ID Layer = " << currentANNneuronConnection->backNeuron->layerID << endl;
		cout << "\t\tcurrentANNneuronConnection->backNeuron ID Subnet = " << currentANNneuronConnection->backNeuron->subnetID << endl;
		*/
		#endif
	}

	netI = netI + neuronBeingAccessed->bias;

	neuronBeingAccessed->output = calculateOValue(netI);	//set ("hidden") ANNneuronContainer output value

	#ifdef ANN_DEBUG
	cout << "neuronBeingAccessed netI = " << netI << endl;		//this may not be necessary or meaningful
	cout << "neuronBeingAccessed Bias = " << neuronBeingAccessed->bias << endl;
	cout << "neuronBeingAccessed Output = " << neuronBeingAccessed->output << endl;
	#endif
}

/************************************************************ End Forward Pass Step Routines* *************************************************/
















/************************************************************ Backward Pass Step Routines* *****************************************************/

void calculateErrorOfOutputNeurons(ANNneuron* firstOutputNeuronInNetwork)
{
	//NB calculate error of output neurons
	ANNneuron* currentNeuron = firstOutputNeuronInNetwork;
	while(currentNeuron->nextNeuron != NULL)
	{
		calculateOutputErrorOfOutputNeuron(currentNeuron);
		currentNeuron = currentNeuron->nextNeuron;
	}
}


//preconditions - assumes neuronBeingAccessed has a back neuron references. (ie neuronBeingAccessed->firstBackNeuronReference != NULL)
void backwardPassStep(ANNneuron* neuronBeingAccessed, int isOutputLayer, bool isSubnet)
{
	#ifdef ANN_DEBUG
	cout << "\nstatic void backwardPassStep(ANNneuron* neuronBeingAccessed, int isOutputLayer)" << endl;
	cout << "neuronBeingAccessed ID = " << neuronBeingAccessed->id << endl;
	cout << "neuronBeingAccessed ID Order = " << neuronBeingAccessed->orderID << endl;
	cout << "neuronBeingAccessed ID Layer = " << neuronBeingAccessed->layerID << endl;
	cout << "neuronBeingAccessed ID Subnet = " << neuronBeingAccessed->subnetID << endl;
	#endif

	if((isOutputLayer) && (!isSubnet))
	{	//calculate errors of output neurons - errors2 (output errors)

		//NB calculate error of neurons at current level
		ANNneuron* currentNeuron = neuronBeingAccessed;
		while(currentNeuron->nextNeuron != NULL)
		{
			calculateOutputErrorOfOutputNeuron(currentNeuron);
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
				calculateOutputErrorOfNonoutputNeuron(currentNeuron);
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
			calculateNewBackConnectionWeightsOfNeuron(currentNeuron);
		}
		currentNeuron = currentNeuron->nextNeuron;
	}

	//NB calculate bias for neurons at current level
	currentNeuron = neuronBeingAccessed;
	while(currentNeuron->nextNeuron != NULL)
	{
		if(!(currentNeuron->isSubnet))
		{
			calculateNewBiasOfNeuron(currentNeuron);
		}
		currentNeuron = currentNeuron->nextNeuron;
	}

	//ANN recursion
#ifdef ANN_ADVANCED
	//NBOLD if !isOutputSubnet, subnets require a front layer to exist in the top level network
	currentNeuron = neuronBeingAccessed;
	while(currentNeuron->nextNeuron != NULL)
	{
		if(currentNeuron->isSubnet)
		{
			#ifdef ANN_DEBUG
			cout << "\nAbout to perform ANN recursion in backwardPassStep ... " << endl;
			cout << "currentNeuron ANNneuronContainer ID = " << currentNeuron->id << endl;
			cout << "currentNeuron ANNneuronContainer ID Order = " << currentNeuron->orderID << endl;
			cout << "currentNeuron ANNneuronContainer ID Layer = " << currentNeuron->layerID << endl;
			cout << "currentNeuron ANNneuronContainer ID Subnet = " << currentNeuron->subnetID << endl;
			//cout << "numberOfInputNeurons = " << currentNeuron->numNeuronsInBackLayerOfSubnet << endl;
			//cout << "numberOfOutputNeurons = " <<  currentNeuron->numNeuronsInFrontLayerOfSubnet << endl;
			#endif

			copyANNneuronConnectionContainerListToNeuronContainerList(currentNeuron->firstNeuronInBackLayerOfSubnet, &(currentNeuron->backANNneuronConnectionList), false);
			if(!(currentNeuron->isOutputSubnet))
			{
				copyANNneuronConnectionContainerListToNeuronContainerList(currentNeuron->firstNeuronInFrontLayerOfSubnet, &(currentNeuron->frontANNneuronConnectionList), true);
			}

			backwardPassStep(currentNeuron->firstNeuronInFrontLayerOfSubnet, true, true);
				//NB CHECK ANNTHIS new requirement; each subnet must have at least 3 layers

			copyNeuronContainerListToANNneuronConnectionContainerList(&(currentNeuron->backANNneuronConnectionList), currentNeuron->firstNeuronInBackLayerOfSubnet, false);
			if(!(currentNeuron->isOutputSubnet))
			{
				copyNeuronContainerListToANNneuronConnectionContainerList(&(currentNeuron->frontANNneuronConnectionList), currentNeuron->firstNeuronInFrontLayerOfSubnet, true);
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
			backwardPassStep(neuronBeingAccessed->firstNeuronInBackLayer, false, isSubnet);
		}
	}
	else
	{
		if(neuronBeingAccessed->firstNeuronInBackLayer->hasBackLayer)
		{
			backwardPassStep(neuronBeingAccessed->firstNeuronInBackLayer, false, isSubnet);
		}
	}
}


void calculateOutputErrorOfOutputNeuron(ANNneuron* neuronBeingAccessed)
{
	#ifdef ANN_DEBUG
	cout << "\nstatic void calculateOutputErrorOfOutputNeuron(ANNneuron* neuronBeingAccessed)" << endl;
	#endif

	double outputError = (neuronBeingAccessed->classTarget - neuronBeingAccessed->output)* neuronBeingAccessed->output* (1.0 - neuronBeingAccessed->output);
	neuronBeingAccessed->error = outputError;

	#ifdef ANN_DEBUG
	cout << "\nNeuron Output error Adjusted" << endl;
	cout << "ANNneuronContainer ID = " << neuronBeingAccessed->id << endl;
	cout << "ANNneuronContainer ID Order = " << neuronBeingAccessed->orderID << endl;
	cout << "ANNneuronContainer ID Layer = " << neuronBeingAccessed->layerID << endl;
	cout << "ANNneuronContainer ID Subnet = " << neuronBeingAccessed->subnetID << endl;
	cout << "ANNneuronContainer classTarget = " << neuronBeingAccessed->classTarget << endl;
	cout << "ANNneuronContainer Error = " << neuronBeingAccessed->error << endl;
	cout << "ANNneuronContainer Output = " << neuronBeingAccessed->output << endl;
	#endif
}

void calculateOutputErrorOfNonoutputNeuron(ANNneuron* neuronBeingAccessed)
{
	#ifdef ANN_DEBUG
	cout << "\nstatic void calculateOutputErrorOfNonoutputNeuron(ANNneuron* neuronBeingAccessed)" << endl;
	#endif

	#ifdef ANN_DEBUG
	cout << "\nNeuron Output error Adjusted" << endl;
	cout << "ANNneuronContainer ID = " << neuronBeingAccessed->id << endl;
	cout << "ANNneuronContainer ID Order = " << neuronBeingAccessed->orderID << endl;
	cout << "ANNneuronContainer ID Layer = " << neuronBeingAccessed->layerID << endl;
	cout << "ANNneuronContainer ID Subnet = " << neuronBeingAccessed->subnetID << endl;
	#endif

	float partOfHiddenError = 0.0;

	for(vector<ANNneuronConnection*>::iterator connectionIter = neuronBeingAccessed->frontANNneuronConnectionList.begin(); connectionIter != neuronBeingAccessed->frontANNneuronConnectionList.end(); connectionIter++)
	{
		ANNneuronConnection* currentANNneuronConnection = *connectionIter;

		//numberOfChickens++;

		partOfHiddenError = partOfHiddenError + ((currentANNneuronConnection->weight)* (currentANNneuronConnection->frontNeuron->error));
		/*partOfHiddenError = partOfHiddenError + (weights2[i*numberOfOutputNeurons+j]*errors2[j]);*/

		#ifdef ANN_DEBUG
		cout << "Front ANNneuron Error = " << currentANNneuronConnection->frontNeuron->error << endl;
		cout << "Front ANNneuron Connection Weight = " << currentANNneuronConnection->weight << endl;
		cout << "partOfHiddenError = " << partOfHiddenError << "\n" << endl;
		#endif
	}

	neuronBeingAccessed->error = (neuronBeingAccessed->output)* (1.0 - (neuronBeingAccessed->output))* partOfHiddenError;

	#ifdef ANN_DEBUG
	cout << "\npartOfHiddenError = " << partOfHiddenError << "\n" << endl;
	cout << "ANNneuronContainer output  = " << neuronBeingAccessed->output << endl;
	cout << "ANNneuronContainer Error = " << neuronBeingAccessed->error << endl;
	#endif
}


void calculateNewBackConnectionWeightsOfNeuron(ANNneuron* neuronBeingAccessed)
{
	#ifdef ANN_DEBUG
	cout << "\nstatic void calculateNewBackConnectionWeightsOfNeuron(ANNneuron* neuronBeingAccessed)" << endl;
	#endif

	#ifdef ANN_DEBUG
	cout << "\nNeuron Back Weight Value Adjusted" << endl;
	cout << "neuronBeingAccessed ID = " << neuronBeingAccessed->id << endl;
	cout << "neuronBeingAccessed ID Order = " << neuronBeingAccessed->orderID << endl;
	cout << "neuronBeingAccessed ID Layer = " << neuronBeingAccessed->layerID << endl;
	cout << "neuronBeingAccessed ID Subnet = " << neuronBeingAccessed->subnetID << endl;
	cout << "MU = " << MU << endl;
	cout << "neuronBeingAccessed Error = " << neuronBeingAccessed->error << endl;
	#endif

	for(vector<ANNneuronConnection*>::iterator connectionIter = neuronBeingAccessed->backANNneuronConnectionList.begin(); connectionIter != neuronBeingAccessed->backANNneuronConnectionList.end(); connectionIter++)
	{
		ANNneuronConnection* currentANNneuronConnection = *connectionIter;

		float weightChange;
		weightChange = MU* neuronBeingAccessed->error* currentANNneuronConnection->backNeuron->output;

		currentANNneuronConnection->weight = currentANNneuronConnection->weight + weightChange;

		#ifdef ANN_DEBUG
		cout << "Back ANNneuron Output = " << currentANNneuronConnection->backNeuron->output << endl;
		cout << "Back ANNneuron Connection Weight = " << currentANNneuronConnection->weight << endl;
		#endif
	}
}

void calculateNewBiasOfNeuron(ANNneuron* neuronBeingAccessed)
{
	#ifdef ANN_DEBUG
	cout << "\nstatic void calculateNewBiasOfNeuron(ANNneuron* neuronBeingAccessed)" << endl;
	#endif

	double biasChange;
	biasChange = MU* neuronBeingAccessed->error* 1.0;	//BIASES ARE WEIGHTED WIANNTH INPUT 1
	neuronBeingAccessed->bias = neuronBeingAccessed->bias + biasChange;

	#ifdef ANN_DEBUG
	cout << "\nNeuron Bias Value Adjusted" << endl;
	cout << "neuron ID = " << neuronBeingAccessed->id << endl;
	cout << "ANNneuronContainer ID Order = " << neuronBeingAccessed->orderID << endl;
	cout << "ANNneuronContainer ID Layer = " << neuronBeingAccessed->layerID << endl;
	cout << "ANNneuronContainer ID Subnet = " << neuronBeingAccessed->subnetID << endl;
	cout << "MU = " << MU << endl;
	cout << "neuron Error = " << neuronBeingAccessed->error << endl;
	cout << "neuron Bias = " << neuronBeingAccessed->bias << endl;
	#endif
}



/************************************************************ End Backward Pass Step Routines* *************************************************/







float calculateOValue(float netValue)
{
	#ifdef DEBUG_TH_OR_IMAGE_CATEGORISTION_NN_USE_LINEAR_COMBINATION_NETWORK
	float f = netValue;	//modified 1-6-04
	#else
	float f = (1.0/(1.0 + float(exp(-netValue))));	//modified 1-6-04
	#endif

	return f;
}

















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
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2014 Baxter AI (baxterai.com)
 * Project: Artificial Neural Network (ANN)
 * Project Version: 3e2d 29-August-2014
 * Comments:
 *
 *******************************************************************************/

#include "ANNupdateAlgorithm.h"
#include "ANNneuronClass.h"

#include <ctime>
#include <cstdlib>	//for random number generation
#include <cmath>

#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <string.h>
#include <iostream>
#include <fstream>
using namespace std;

#define	MU 0.1F




int dataSetNum;


/************************************************************ Main Neural Network Learning Routines with a class Target set ******************************************************/
//preconditions; neural network is already filled with all necessary weights/biases/inputneuron values


#ifdef DEBUG_TH_OR_IMAGE_CATEGORISTION_NN_4
bool debugPrintNNOutputs;
#endif

double calculateErrorOfBackPropPass(Neuron * firstOutputNeuronInNetwork)
{
	/*

	Error for signle training example over all output neurons:

	E = 1/2 SUM(0->i) e(i)^2 = 1/2 SUM (d(i) - o(i))^2

	therefore...

	*/

	double total2xError = 0.0;

	Neuron * currentNeuron = firstOutputNeuronInNetwork;
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
		cout << "NeuronContainer ID = " << currentNeuron->id << endl;
		cout << "NeuronContainer ID Order = " << currentNeuron->orderID << endl;
		cout << "NeuronContainer ID Layer = " << currentNeuron->layerID << endl;
		cout << "NeuronContainer ID Subnet = " << currentNeuron->subnetID << endl;
		cout << "NeuronContainer classTarget = " << currentNeuron->classTarget << endl;
		cout << "NeuronContainer Output = " << currentNeuron->output << endl;
		cout << "total2xError = " << total2xError << "\n" << endl;
		#endif

		currentNeuron = currentNeuron->nextNeuron;
	}

	float totalError = 0.5F * total2xError;

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

double ANNBackPropogationPass(Neuron * firstInputNeuronInNetwork, Neuron * firstOutputNeuronInNetwork)
{
	#ifdef ANN_DEBUG
	cout << "\ndouble ANNBackPropogationPass(Neuron * firstInputNeuronInNetwork, Neuron * firstOutputNeuronInNetwork)" << endl;
	#endif

	forwardPassStep(firstInputNeuronInNetwork);

	backwardPassStep(firstOutputNeuronInNetwork, true, false);

	return calculateErrorOfBackPropPass(firstOutputNeuronInNetwork);
}
/************************************************************ End Main Neural Network Learning Routines with a class Target set ***************************************************/





#ifdef ANN_ADVANCED

void copyNeuronContainerListToNeuronConnectionContainerList(vector<NeuronConnection*> * neuronConnectionListToUpdate, Neuron * firstNeuronInListToCopy, bool frontOrBack)
{
	Neuron * currentNeuronToCopy = firstNeuronInListToCopy;

	for(vector<NeuronConnection*>::iterator neuronConnectionListToUpdateIter = neuronConnectionListToUpdate->begin(); neuronConnectionListToUpdateIter != neuronConnectionListToUpdate->end(); neuronConnectionListToUpdateIter++)
	{
		//average
		if(frontOrBack)
		{
			averageNeuronKeyProperties((*neuronConnectionListToUpdateIter)->frontNeuron, currentNeuronToCopy);
		}
		else
		{
			averageNeuronKeyProperties((*neuronConnectionListToUpdateIter)->backNeuron, currentNeuronToCopy);
		}
		currentNeuronToCopy = currentNeuronToCopy->nextNeuron;
	}
}

void copyNeuronConnectionContainerListToNeuronContainerList(Neuron * firstNeuronInListToUpdate, vector<NeuronConnection*> * neuronConnectionListToCopy, bool frontOrBack)
{
	Neuron * currentNeuronToUpdate = firstNeuronInListToUpdate;
	vector<NeuronConnection*>::iterator neuronConnectionListToCopyIter = neuronConnectionListToCopy->begin();

	while(currentNeuronToUpdate->nextNeuron != NULL)		//?this will fail; it appears as the neuron connection containers have less neuron connections than neurons in the layer - this is a mistake
	//while(currentNeuronConnectionContainerToCopy -> nextNeuronConnectionContainer != NULL)
	{
		//average
		if(frontOrBack)
		{
			averageNeuronKeyProperties(currentNeuronToUpdate, (*neuronConnectionListToCopyIter)->frontNeuron);
		}
		else
		{
			averageNeuronKeyProperties(currentNeuronToUpdate, (*neuronConnectionListToCopyIter)->backNeuron);
		}
		currentNeuronToUpdate = currentNeuronToUpdate->nextNeuron;
		neuronConnectionListToCopyIter++;
	}
}

#endif


/************************************************************ Forward Pass Step Routines ******************************************************/

void forwardPassStep(Neuron * neuronBeingAccessed)
{
	#ifdef ANN_DEBUG
	cout << "\nstatic void forwardPassStep(Neuron * neuronBeingAccessed)" << endl;
	cout << "NeuronContainer ID = " << neuronBeingAccessed->id << endl;
	cout << "NeuronContainer ID Order = " << neuronBeingAccessed->orderID << endl;
	cout << "NeuronContainer ID Layer = " << neuronBeingAccessed->layerID << endl;
	cout << "NeuronContainer ID Subnet = " << neuronBeingAccessed->subnetID << endl;
	#endif

	Neuron * currentNeuron = neuronBeingAccessed->firstNeuronInFrontLayer;

	while(currentNeuron->nextNeuron != NULL)
	{
		#ifdef ANN_DEBUG
		cout << "\nA Hidden level NeuronContainer has been selected for Learning" << endl;
		cout << "NeuronContainer ID = " << currentNeuron->id << endl;
		cout << "NeuronContainer ID Order = " << currentNeuron->orderID << endl;
		cout << "NeuronContainer ID Layer = " << currentNeuron->layerID << endl;
		cout << "NeuronContainer ID Subnet = " << currentNeuron->subnetID << endl;
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
			cout << "currentNeuron NeuronContainer ID = " << currentNeuron->id << endl;
			cout << "currentNeuron NeuronContainer ID Order = " << currentNeuron->orderID << endl;
			cout << "currentNeuron NeuronContainer ID Layer = " << currentNeuron->layerID << endl;
			cout << "currentNeuron NeuronContainer ID Subnet = " << currentNeuron->subnetID << endl;
			//cout << "numberOfInputNeurons = " << currentNeuron->numNeuronsInBackLayerOfSubnet << endl;
			//cout << "numberOfOutputNeurons = " <<  currentNeuron->numNeuronsInFrontLayerOfSubnet << endl;
			#endif

			if(!(currentNeuron->isInputSubnet))
			{
				copyNeuronConnectionContainerListToNeuronContainerList(currentNeuron->firstNeuronInBackLayerOfSubnet, &(currentNeuron->backNeuronConnectionList), false);
			}

			copyNeuronConnectionContainerListToNeuronContainerList(currentNeuron->firstNeuronInFrontLayerOfSubnet, &(currentNeuron->frontNeuronConnectionList), true);
			forwardPassStep(currentNeuron->firstNeuronInBackLayerOfSubnet);	//?ISSUE HERE

			if(!(currentNeuron->isInputSubnet))
			{
				copyNeuronContainerListToNeuronConnectionContainerList(&(currentNeuron->backNeuronConnectionList), currentNeuron->firstNeuronInBackLayerOfSubnet, false);
			}

			copyNeuronContainerListToNeuronConnectionContainerList(&(currentNeuron->frontNeuronConnectionList), currentNeuron->firstNeuronInFrontLayerOfSubnet, true);
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
void adjustOutputValueOfANeuronBasedOnBackNeurons(Neuron * neuronBeingAccessed)
{
	#ifdef ANN_DEBUG
	cout << "\nstatic void adjustOutputValueOfANeuronBasedOnBackNeurons(Neuron * neuronBeingAccessed)" << endl;
	cout << "\nNeuron Output Value Adjusted" << endl;
	cout << "NeuronContainer ID = " << neuronBeingAccessed->id << endl;
	cout << "NeuronContainer ID Order = " << neuronBeingAccessed->orderID << endl;
	cout << "NeuronContainer ID Layer = " << neuronBeingAccessed->layerID << endl;
	cout << "NeuronContainer ID Subnet = " << neuronBeingAccessed->subnetID << endl;
	#endif

	//for every NeuronContainer being accessed

	float netI = 0.0;
	for(vector<NeuronConnection*>::iterator connectionIter = neuronBeingAccessed->backNeuronConnectionList.begin(); connectionIter != neuronBeingAccessed->backNeuronConnectionList.end(); connectionIter++)
	{
		NeuronConnection * currentNeuronConnection = *connectionIter;
		#ifdef ANN_DEBUG
		//cout << "currentNeuronConnection->backNeuron->output = " << currentNeuronConnection->backNeuron->output << endl;
		#endif

		netI = netI + (currentNeuronConnection->backNeuron->output * currentNeuronConnection->weight);

		#ifdef DEBUG_TRAIN_NETWORK_WITH_NON_RANDOM_VARS
		cout << "currentNeuronConnection->backNeuron->id = " << currentNeuronConnection->backNeuron->id << endl;
		#endif

		#ifdef ANN_DEBUG
		cout << "Back Neuron Output = " << currentNeuronConnection->backNeuron->output << endl;
		cout << "Back Neuron Connection Weight = " << currentNeuronConnection->weight << endl;
		cout << "NeuronContainer netI = " << netI << endl;
		/*
		cout << "currentNeuronConnection->weight > 100" << endl;
		cout << "currentNeuronConnection->weight = " << currentNeuronConnection->weight << endl;
		cout << "dataSetNum = " << dataSetNum << endl;
		cout << "\tneuronBeingAccessed ID = " << neuronBeingAccessed->id << endl;
		cout << "\tneuronBeingAccessed ID Order = " << neuronBeingAccessed->orderID << endl;
		cout << "\tneuronBeingAccessed ID Layer = " << neuronBeingAccessed->layerID << endl;
		cout << "\tneuronBeingAccessed ID Subnet = " << neuronBeingAccessed->subnetID << endl;
		cout << "\t\tcurrentNeuronConnection->backNeuron ID = " << currentNeuronConnection->backNeuron->id << endl;
		cout << "\t\tcurrentNeuronConnection->backNeuron ID Order = " << currentNeuronConnection->backNeuron->orderID << endl;
		cout << "\t\tcurrentNeuronConnection->backNeuron ID Layer = " << currentNeuronConnection->backNeuron->layerID << endl;
		cout << "\t\tcurrentNeuronConnection->backNeuron ID Subnet = " << currentNeuronConnection->backNeuron->subnetID << endl;
		*/
		#endif
	}

	netI = netI + neuronBeingAccessed->bias;

	neuronBeingAccessed->output = calculateOValue(netI);	//set ("hidden") NeuronContainer output value

	#ifdef ANN_DEBUG
	cout << "neuronBeingAccessed netI = " << netI << endl;		//this may not be necessary or meaningful
	cout << "neuronBeingAccessed Bias = " << neuronBeingAccessed->bias << endl;
	cout << "neuronBeingAccessed Output = " << neuronBeingAccessed->output << endl;
	#endif
}

/************************************************************ End Forward Pass Step Routines **************************************************/
















/************************************************************ Backward Pass Step Routines ******************************************************/

void calculateErrorOfOutputNeurons(Neuron * firstOutputNeuronInNetwork)
{
	//NB calculate error of output neurons
	Neuron * currentNeuron = firstOutputNeuronInNetwork;
	while(currentNeuron->nextNeuron != NULL)
	{
		calculateOutputErrorOfOutputNeuron(currentNeuron);
		currentNeuron = currentNeuron->nextNeuron;
	}
}


//preconditions - assumes neuronBeingAccessed has a back neuron references. (ie neuronBeingAccessed->firstBackNeuronReference != NULL)
void backwardPassStep(Neuron * neuronBeingAccessed, int isOutputLayer, bool isSubnet)
{
	#ifdef ANN_DEBUG
	cout << "\nstatic void backwardPassStep(Neuron * neuronBeingAccessed, int isOutputLayer)" << endl;
	cout << "neuronBeingAccessed ID = " << neuronBeingAccessed->id << endl;
	cout << "neuronBeingAccessed ID Order = " << neuronBeingAccessed->orderID << endl;
	cout << "neuronBeingAccessed ID Layer = " << neuronBeingAccessed->layerID << endl;
	cout << "neuronBeingAccessed ID Subnet = " << neuronBeingAccessed->subnetID << endl;
	#endif

	if((isOutputLayer) && (!isSubnet))
	{	//calculate errors of output neurons - errors2 (output errors)

		//NB calculate error of neurons at current level
		Neuron * currentNeuron = neuronBeingAccessed;
		while(currentNeuron->nextNeuron != NULL)
		{
			calculateOutputErrorOfOutputNeuron(currentNeuron);
			currentNeuron = currentNeuron->nextNeuron;
		}
	}
	else
	{//calculates the errors of the hidden neurons
		Neuron * currentNeuron = neuronBeingAccessed;
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
	Neuron * currentNeuron = neuronBeingAccessed;
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
			cout << "currentNeuron NeuronContainer ID = " << currentNeuron->id << endl;
			cout << "currentNeuron NeuronContainer ID Order = " << currentNeuron->orderID << endl;
			cout << "currentNeuron NeuronContainer ID Layer = " << currentNeuron->layerID << endl;
			cout << "currentNeuron NeuronContainer ID Subnet = " << currentNeuron->subnetID << endl;
			//cout << "numberOfInputNeurons = " << currentNeuron->numNeuronsInBackLayerOfSubnet << endl;
			//cout << "numberOfOutputNeurons = " <<  currentNeuron->numNeuronsInFrontLayerOfSubnet << endl;
			#endif

			copyNeuronConnectionContainerListToNeuronContainerList(currentNeuron->firstNeuronInBackLayerOfSubnet, &(currentNeuron->backNeuronConnectionList), false);
			if(!(currentNeuron->isOutputSubnet))
			{
				copyNeuronConnectionContainerListToNeuronContainerList(currentNeuron->firstNeuronInFrontLayerOfSubnet, &(currentNeuron->frontNeuronConnectionList), true);
			}

			backwardPassStep(currentNeuron->firstNeuronInFrontLayerOfSubnet, true, true);
				//NB CHECK ANNTHIS new requirement; each subnet must have at least 3 layers

			copyNeuronContainerListToNeuronConnectionContainerList(&(currentNeuron->backNeuronConnectionList), currentNeuron->firstNeuronInBackLayerOfSubnet, false);
			if(!(currentNeuron->isOutputSubnet))
			{
				copyNeuronContainerListToNeuronConnectionContainerList(&(currentNeuron->frontNeuronConnectionList), currentNeuron->firstNeuronInFrontLayerOfSubnet, true);
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


void calculateOutputErrorOfOutputNeuron(Neuron * neuronBeingAccessed)
{
	#ifdef ANN_DEBUG
	cout << "\nstatic void calculateOutputErrorOfOutputNeuron(Neuron * neuronBeingAccessed)" << endl;
	#endif

	double outputError = (neuronBeingAccessed->classTarget - neuronBeingAccessed->output) * neuronBeingAccessed->output * (1.0 - neuronBeingAccessed->output);
	neuronBeingAccessed->error = outputError;

	#ifdef ANN_DEBUG
	cout << "\nNeuron Output error Adjusted" << endl;
	cout << "NeuronContainer ID = " << neuronBeingAccessed->id << endl;
	cout << "NeuronContainer ID Order = " << neuronBeingAccessed->orderID << endl;
	cout << "NeuronContainer ID Layer = " << neuronBeingAccessed->layerID << endl;
	cout << "NeuronContainer ID Subnet = " << neuronBeingAccessed->subnetID << endl;
	cout << "NeuronContainer classTarget = " << neuronBeingAccessed->classTarget << endl;
	cout << "NeuronContainer Error = " << neuronBeingAccessed->error << endl;
	cout << "NeuronContainer Output = " << neuronBeingAccessed->output << endl;
	#endif
}

void calculateOutputErrorOfNonoutputNeuron(Neuron * neuronBeingAccessed)
{
	#ifdef ANN_DEBUG
	cout << "\nstatic void calculateOutputErrorOfNonoutputNeuron(Neuron * neuronBeingAccessed)" << endl;
	#endif

	#ifdef ANN_DEBUG
	cout << "\nNeuron Output error Adjusted" << endl;
	cout << "NeuronContainer ID = " << neuronBeingAccessed->id << endl;
	cout << "NeuronContainer ID Order = " << neuronBeingAccessed->orderID << endl;
	cout << "NeuronContainer ID Layer = " << neuronBeingAccessed->layerID << endl;
	cout << "NeuronContainer ID Subnet = " << neuronBeingAccessed->subnetID << endl;
	#endif

	float partOfHiddenError = 0.0;

	for(vector<NeuronConnection*>::iterator connectionIter = neuronBeingAccessed->frontNeuronConnectionList.begin(); connectionIter != neuronBeingAccessed->frontNeuronConnectionList.end(); connectionIter++)
	{
		NeuronConnection * currentNeuronConnection = *connectionIter;

		//numberOfChickens++;

		partOfHiddenError = partOfHiddenError + ((currentNeuronConnection->weight) * (currentNeuronConnection->frontNeuron->error));
		/*partOfHiddenError = partOfHiddenError + (weights2[i*numberOfOutputNeurons+j]*errors2[j]);*/

		#ifdef ANN_DEBUG
		cout << "Front Neuron Error = " << currentNeuronConnection->frontNeuron->error << endl;
		cout << "Front Neuron Connection Weight = " << currentNeuronConnection->weight << endl;
		cout << "partOfHiddenError = " << partOfHiddenError << "\n" << endl;
		#endif
	}

	neuronBeingAccessed->error = (neuronBeingAccessed->output) * (1.0 - (neuronBeingAccessed->output)) * partOfHiddenError;

	#ifdef ANN_DEBUG
	cout << "\npartOfHiddenError = " << partOfHiddenError << "\n" << endl;
	cout << "NeuronContainer output  = " << neuronBeingAccessed->output << endl;
	cout << "NeuronContainer Error = " << neuronBeingAccessed->error << endl;
	#endif
}


void calculateNewBackConnectionWeightsOfNeuron(Neuron * neuronBeingAccessed)
{
	#ifdef ANN_DEBUG
	cout << "\nstatic void calculateNewBackConnectionWeightsOfNeuron(Neuron * neuronBeingAccessed)" << endl;
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

	for(vector<NeuronConnection*>::iterator connectionIter = neuronBeingAccessed->backNeuronConnectionList.begin(); connectionIter != neuronBeingAccessed->backNeuronConnectionList.end(); connectionIter++)
	{
		NeuronConnection * currentNeuronConnection = *connectionIter;

		float weightChange;
		weightChange = MU * neuronBeingAccessed->error * currentNeuronConnection->backNeuron->output;

		currentNeuronConnection->weight = currentNeuronConnection->weight + weightChange;

		#ifdef ANN_DEBUG
		cout << "Back Neuron Output = " << currentNeuronConnection->backNeuron->output << endl;
		cout << "Back Neuron Connection Weight = " << currentNeuronConnection->weight << endl;
		#endif
	}
}

void calculateNewBiasOfNeuron(Neuron * neuronBeingAccessed)
{
	#ifdef ANN_DEBUG
	cout << "\nstatic void calculateNewBiasOfNeuron(Neuron * neuronBeingAccessed)" << endl;
	#endif

	double biasChange;
	biasChange = MU * neuronBeingAccessed->error * 1.0;	//BIASES ARE WEIGHTED WIANNTH INPUT 1
	neuronBeingAccessed->bias = neuronBeingAccessed->bias + biasChange;

	#ifdef ANN_DEBUG
	cout << "\nNeuron Bias Value Adjusted" << endl;
	cout << "neuron ID = " << neuronBeingAccessed->id << endl;
	cout << "NeuronContainer ID Order = " << neuronBeingAccessed->orderID << endl;
	cout << "NeuronContainer ID Layer = " << neuronBeingAccessed->layerID << endl;
	cout << "NeuronContainer ID Subnet = " << neuronBeingAccessed->subnetID << endl;
	cout << "MU = " << MU << endl;
	cout << "neuron Error = " << neuronBeingAccessed->error << endl;
	cout << "neuron Bias = " << neuronBeingAccessed->bias << endl;
	#endif
}



/************************************************************ End Backward Pass Step Routines **************************************************/







float calculateOValue(float netValue)
{
	#ifdef DEBUG_TH_OR_IMAGE_CATEGORISTION_NN_USE_LINEAR_COMBINATION_NETWORK
	float f = netValue;	//modified 1-6-04
	#else
	float f = (1.0/(1.0 + float(exp(-netValue))));	//modified 1-6-04
	#endif

	return f;
}

















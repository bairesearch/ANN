/*******************************************************************************
 * 
 * This file is part of BAIPROJECT.
 * 
 * BAIPROJECT is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License version 3
 * only, as published by the Free Software Foundation.
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
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2012 Baxter AI (baxterai.com)
 * Project: Artificial Neural Network (ANN)
 * Project Version: 3c3h 19-November-2012
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

double calculateErrorOfBackPropPass(NeuronContainer * firstOutputNeuronInNetwork)
{
	/*

	Error for signle training example over all output neurons:

	E = 1/2 SUM(0->i) e(i)^2 = 1/2 SUM (d(i) - o(i))^2

	therefore...

	*/

	double total2xError = 0.0;

	NeuronContainer * currentNeuron = firstOutputNeuronInNetwork;
	while(currentNeuron->nextNeuronContainer != NULL)
	{
		#ifdef DEBUG_TH_OR_IMAGE_CATEGORISTION_NN_4
		if(debugPrintNNOutputs)
		{
			cout << "currentNeuron->neuron->classTarget = " << currentNeuron->neuron->classTarget << endl;
			cout << "currentNeuron->neuron->output = " << currentNeuron->neuron->output << endl;
		}
		#endif

		total2xError = total2xError + pow((currentNeuron->neuron->classTarget - currentNeuron->neuron->output), 2.0);

		/*total2xError = total2xError + pow((classTarget[i] - outputs2[i]),2);*/

		#ifdef ANN_DEBUG
		cout << "\nNeural Network Total Error Calculations" << endl;
		cout << "NeuronContainer ID = " << currentNeuron->neuron->id << endl;
		cout << "NeuronContainer ID Order = " << currentNeuron->neuron->orderID << endl;
		cout << "NeuronContainer ID Layer = " << currentNeuron->neuron->layerID << endl;
		cout << "NeuronContainer ID Subnet = " << currentNeuron->neuron->subnetID << endl;
		cout << "NeuronContainer classTarget = " << currentNeuron->neuron->classTarget << endl;
		cout << "NeuronContainer Output = " << currentNeuron->neuron->output << endl;
		cout << "total2xError = " << total2xError << "\n" << endl;
		#endif

		currentNeuron = currentNeuron->nextNeuronContainer;
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

double ANNBackPropogationPass(NeuronContainer * firstInputNeuronInNetwork, NeuronContainer * firstOutputNeuronInNetwork)
{
	#ifdef ANN_DEBUG
	cout << "\ndouble ANNBackPropogationPass(NeuronContainer * firstInputNeuronInNetwork, NeuronContainer * firstOutputNeuronInNetwork)" << endl;
	#endif

	forwardPassStep(firstInputNeuronInNetwork);

	backwardPassStep(firstOutputNeuronInNetwork, true, false);

	return calculateErrorOfBackPropPass(firstOutputNeuronInNetwork);
}
/************************************************************ End Main Neural Network Learning Routines with a class Target set ***************************************************/





#ifdef ANN_ADVANCED

void copyNeuronContainerListToNeuronConnectionContainerList(NeuronConnectionContainer * firstNeuronConnectionInListToUpdate, NeuronContainer * firstNeuronInListToCopy)
{
	NeuronConnectionContainer * currentNeuronConnectionContainerToUpdate = firstNeuronConnectionInListToUpdate;
	NeuronContainer * currentNeuronContainerToCopy = firstNeuronInListToCopy;

	while(currentNeuronConnectionContainerToUpdate -> nextNeuronConnectionContainer != NULL)
	{
		//average
		//copy
		averageNeuronKeyProperties(currentNeuronConnectionContainerToUpdate->neuron, currentNeuronContainerToCopy->neuron);
		currentNeuronConnectionContainerToUpdate = currentNeuronConnectionContainerToUpdate->nextNeuronConnectionContainer;
		currentNeuronContainerToCopy = currentNeuronContainerToCopy->nextNeuronContainer;
	}
}

void copyNeuronConnectionContainerListToNeuronContainerList(NeuronContainer * firstNeuronInListToUpdate, NeuronConnectionContainer * firstNeuronConnectionInListToCopy)
{
	NeuronContainer * currentNeuronContainerToUpdate = firstNeuronInListToUpdate;
	NeuronConnectionContainer * currentNeuronConnectionContainerToCopy = firstNeuronConnectionInListToCopy;

	while(currentNeuronContainerToUpdate -> nextNeuronContainer != NULL)		//?this will fail; it appears as the neuron connection containers have less neuron connections than neurons in the layer - this is a mistake
	//while(currentNeuronConnectionContainerToCopy -> nextNeuronConnectionContainer != NULL)
	{
		//average
		averageNeuronKeyProperties(currentNeuronContainerToUpdate->neuron, currentNeuronConnectionContainerToCopy->neuron);
		currentNeuronContainerToUpdate = currentNeuronContainerToUpdate->nextNeuronContainer;
		currentNeuronConnectionContainerToCopy = currentNeuronConnectionContainerToCopy->nextNeuronConnectionContainer;
	}
}

/*
void copyNeuronReferenceLists(NeuronConnectionContainer * firstNeuronInListToUpdate, NeuronConnectionContainer * firstNeuronInListToCopy)
{
	NeuronConnectionContainer * currentNeuronConnectionContainerToUpdate = firstNeuronInListToUpdate;
	NeuronConnectionContainer * currentNeuronConnectionContainerToCopy = firstNeuronInListToCopy;

	while(currentNeuronConnectionContainerToUpdate -> nextNeuronConnectionContainer != NULL)
	{
		copyNeuronKeyProperties(currentNeuronConnectionContainerToCopy->neuron, currentNeuronConnectionContainerToCopy->neuron);
		currentNeuronConnectionContainerToUpdate = currentNeuronConnectionContainerToUpdate->nextNeuronContainer;
		currentNeuronConnectionContainerToCopy = currentNeuronConnectionContainerToCopy->nextNeuronContainer;
	}
}
*/



#endif


/************************************************************ Forward Pass Step Routines ******************************************************/

void forwardPassStep(NeuronContainer * neuronBeingAccessed)
{
	#ifdef ANN_DEBUG
	cout << "\nstatic void forwardPassStep(NeuronContainer * neuronBeingAccessed)" << endl;
	cout << "NeuronContainer ID = " << neuronBeingAccessed->neuron->id << endl;
	cout << "NeuronContainer ID Order = " << neuronBeingAccessed->neuron->orderID << endl;
	cout << "NeuronContainer ID Layer = " << neuronBeingAccessed->neuron->layerID << endl;
	cout << "NeuronContainer ID Subnet = " << neuronBeingAccessed->neuron->subnetID << endl;
	#endif

	NeuronContainer * currentNeuron = neuronBeingAccessed->firstNeuronInFrontLayer;

	while(currentNeuron -> nextNeuronContainer != NULL)
	{
		#ifdef ANN_DEBUG
		cout << "\nA Hidden level NeuronContainer has been selected for Learning" << endl;
		cout << "NeuronContainer ID = " << currentNeuron->neuron->id << endl;
		cout << "NeuronContainer ID Order = " << currentNeuron->neuron->orderID << endl;
		cout << "NeuronContainer ID Layer = " << currentNeuron->neuron->layerID << endl;
		cout << "NeuronContainer ID Subnet = " << currentNeuron->neuron->subnetID << endl;
		#endif

		if(!(currentNeuron->isSubnet))
		{
			adjustOutputValueOfANeuronBasedOnBackNeurons(currentNeuron);
			//cout << "currentNeuron->neuron->output  = " << currentNeuron->neuron->output << endl;
		}

		currentNeuron = currentNeuron->nextNeuronContainer;
	}

	//ANN recursion
	#ifdef ANN_ADVANCED
	//NB if !isInputSubnet, subnets require a back layer to exist in the top level network
	currentNeuron = neuronBeingAccessed;
	while(currentNeuron -> nextNeuronContainer != NULL)
	{
		if(currentNeuron->isSubnet)
		{
			#ifdef ANN_DEBUG
			cout << "\nAbout to perform ANN recursion in forwardPassStep ... " << endl;
			cout << "currentNeuron NeuronContainer ID = " << currentNeuron->neuron->id << endl;
			cout << "currentNeuron NeuronContainer ID Order = " << currentNeuron->neuron->orderID << endl;
			cout << "currentNeuron NeuronContainer ID Layer = " << currentNeuron->neuron->layerID << endl;
			cout << "currentNeuron NeuronContainer ID Subnet = " << currentNeuron->neuron->subnetID << endl;
			//cout << "numberOfInputNeurons = " << currentNeuron->numNeuronsInBackLayerOfSubnet << endl;
			//cout << "numberOfOutputNeurons = " <<  currentNeuron->numNeuronsInFrontLayerOfSubnet << endl;
			#endif

			if(!(currentNeuron->isInputSubnet))
			{
				copyNeuronConnectionContainerListToNeuronContainerList(currentNeuron->firstNeuronContainerInBackLayerOfSubnet, currentNeuron->firstBackNeuronConnectionContainer);
			}

			copyNeuronConnectionContainerListToNeuronContainerList(currentNeuron->firstNeuronContainerInFrontLayerOfSubnet, currentNeuron->firstFrontNeuronConnectionContainer);
			forwardPassStep(currentNeuron->firstNeuronContainerInBackLayerOfSubnet);	//?ISSUE HERE

			if(!(currentNeuron->isInputSubnet))
			{
				copyNeuronContainerListToNeuronConnectionContainerList(currentNeuron->firstBackNeuronConnectionContainer, currentNeuron->firstNeuronContainerInBackLayerOfSubnet);
			}

			copyNeuronContainerListToNeuronConnectionContainerList(currentNeuron->firstFrontNeuronConnectionContainer, currentNeuron->firstNeuronContainerInFrontLayerOfSubnet);
		}
		currentNeuron = currentNeuron->nextNeuronContainer;
	}
	#endif

	//recursion
	if(neuronBeingAccessed->firstNeuronInFrontLayer->hasFrontLayer)
	{
		forwardPassStep(neuronBeingAccessed->firstNeuronInFrontLayer);
	}
}


//Only [top level]/[foward level]/[final] Output Neurons have "Class Target" values
void adjustOutputValueOfANeuronBasedOnBackNeurons(NeuronContainer * neuronBeingAccessed)
{
	#ifdef ANN_DEBUG
	cout << "\nstatic void adjustOutputValueOfANeuronBasedOnBackNeurons(NeuronContainer * neuronBeingAccessed)" << endl;
	cout << "\nNeuron Output Value Adjusted" << endl;
	cout << "NeuronContainer ID = " << neuronBeingAccessed->neuron->id << endl;
	cout << "NeuronContainer ID Order = " << neuronBeingAccessed->neuron->orderID << endl;
	cout << "NeuronContainer ID Layer = " << neuronBeingAccessed->neuron->layerID << endl;
	cout << "NeuronContainer ID Subnet = " << neuronBeingAccessed->neuron->subnetID << endl;
	#endif

	//for every NeuronContainer being accessed

	NeuronConnectionContainer * currentConnectionContainer = neuronBeingAccessed->firstBackNeuronConnectionContainer;

	float netI = 0.0;
	while(currentConnectionContainer->nextNeuronConnectionContainer != NULL)
	{
		#ifdef ANN_DEBUG
		//cout << "currentConnectionContainer->neuron->output = " << currentConnectionContainer->neuron->output << endl;
		#endif
		
		netI = netI + (currentConnectionContainer->neuron->output * currentConnectionContainer->neuronConnection->weight);

		#ifdef DEBUG_TRAIN_NETWORK_WITH_NON_RANDOM_VARS
		cout << "currentConnectionContainer->neuron->id = " << currentConnectionContainer->neuron->id << endl;
		#endif

		#ifdef ANN_DEBUG
		cout << "Back NeuronContainer Output = " << currentConnectionContainer->neuron->output << endl;
		cout << "Back NeuronContainer Connection Weight = " << currentConnectionContainer->neuronConnection->weight << endl;
		cout << "NeuronContainer netI = " << netI << endl;
		/*
		cout << "currentConnectionContainer->neuronConnection->weight > 100" << endl;
		cout << "currentConnectionContainer->neuronConnection->weight = " << currentConnectionContainer->neuronConnection->weight << endl;
		cout << "dataSetNum = " << dataSetNum << endl;
		cout << "\tNeuronContainer ID = " << neuronBeingAccessed->neuron->id << endl;
		cout << "\tNeuronContainer ID Order = " << neuronBeingAccessed->neuron->orderID << endl;
		cout << "\tNeuronContainer ID Layer = " << neuronBeingAccessed->neuron->layerID << endl;
		cout << "\tNeuronContainer ID Subnet = " << neuronBeingAccessed->neuron->subnetID << endl;
		cout << "\t\tcurrentConnectionContainer ID = " << currentConnectionContainer->neuron->id << endl;
		cout << "\t\tcurrentConnectionContainer ID Order = " << currentConnectionContainer->neuron->orderID << endl;
		cout << "\t\tcurrentConnectionContainer ID Layer = " << currentConnectionContainer->neuron->layerID << endl;
		cout << "\t\tcurrentConnectionContainer ID Subnet = " << currentConnectionContainer->neuron->subnetID << endl;
		*/
		#endif

		currentConnectionContainer = currentConnectionContainer->nextNeuronConnectionContainer;
	}

	netI = netI + neuronBeingAccessed->neuron->bias;

	neuronBeingAccessed->neuron->output = calculateOValue(netI);	//set ("hidden") NeuronContainer output value

	#ifdef ANN_DEBUG
	cout << "NeuronContainer netI = " << netI << endl;		//this may not be necessary or meaningful
	cout << "NeuronContainer Bias = " << neuronBeingAccessed->neuron->bias << endl;
	cout << "NeuronContainer Output = " << neuronBeingAccessed->neuron->output << endl;
	#endif
}

/************************************************************ End Forward Pass Step Routines **************************************************/
















/************************************************************ Backward Pass Step Routines ******************************************************/

void calculateErrorOfOutputNeurons(NeuronContainer * firstOutputNeuronInNetwork)
{
	//NB calculate error of output neurons
	NeuronContainer * currentNeuron = firstOutputNeuronInNetwork;
	while(currentNeuron -> nextNeuronContainer != NULL)
	{
		calculateOutputErrorOfOutputNeuron(currentNeuron);
		currentNeuron = currentNeuron->nextNeuronContainer;
	}
}


//preconditions - assumes neuronBeingAccessed has a back neuron references. (ie neuronBeingAccessed->firstBackNeuronReference != NULL)
void backwardPassStep(NeuronContainer * neuronBeingAccessed, int isOutputLayer, bool isSubnet)
{
	#ifdef ANN_DEBUG
	cout << "\nstatic void backwardPassStep(NeuronContainer * neuronBeingAccessed, int isOutputLayer)" << endl;
	cout << "NeuronContainer ID = " << neuronBeingAccessed->neuron->id << endl;
	cout << "NeuronContainer ID Order = " << neuronBeingAccessed->neuron->orderID << endl;
	cout << "NeuronContainer ID Layer = " << neuronBeingAccessed->neuron->layerID << endl;
	cout << "NeuronContainer ID Subnet = " << neuronBeingAccessed->neuron->subnetID << endl;
	#endif

	if((isOutputLayer) && (!isSubnet))
	{	//calculate errors of output neurons - errors2 (output errors)

		//NB calculate error of neurons at current level
		NeuronContainer * currentNeuron = neuronBeingAccessed;
		while(currentNeuron -> nextNeuronContainer != NULL)
		{
			calculateOutputErrorOfOutputNeuron(currentNeuron);
			currentNeuron = currentNeuron->nextNeuronContainer;
		}
	}
	else
	{//calculates the errors of the hidden neurons
		NeuronContainer * currentNeuron = neuronBeingAccessed;
		while(currentNeuron -> nextNeuronContainer != NULL)
		{
			if(!(currentNeuron->isSubnet))
			{
				calculateOutputErrorOfNonoutputNeuron(currentNeuron);
			}
			currentNeuron = currentNeuron->nextNeuronContainer;
		}
	}

	//NB calculate back Weight of neurons at current level {NB if at the back layer of a subnet, nothing will happen}
	NeuronContainer * currentNeuron = neuronBeingAccessed;
	while(currentNeuron -> nextNeuronContainer != NULL)
	{
		if(!(currentNeuron->isSubnet))
		{
			calculateNewBackConnectionWeightsOfNeuron(currentNeuron);
		}
		currentNeuron = currentNeuron->nextNeuronContainer;
	}

	//NB calculate bias for neurons at current level
	currentNeuron = neuronBeingAccessed;
	while(currentNeuron -> nextNeuronContainer != NULL)
	{
		if(!(currentNeuron->isSubnet))
		{
			calculateNewBiasOfNeuron(currentNeuron);
		}
		currentNeuron = currentNeuron->nextNeuronContainer;
	}

	//ANN recursion
#ifdef ANN_ADVANCED
	//NBOLD if !isOutputSubnet, subnets require a front layer to exist in the top level network
	currentNeuron = neuronBeingAccessed;
	while(currentNeuron -> nextNeuronContainer != NULL)
	{
		if(currentNeuron->isSubnet)
		{
			#ifdef ANN_DEBUG
			cout << "\nAbout to perform ANN recursion in backwardPassStep ... " << endl;
			cout << "currentNeuron NeuronContainer ID = " << currentNeuron->neuron->id << endl;
			cout << "currentNeuron NeuronContainer ID Order = " << currentNeuron->neuron->orderID << endl;
			cout << "currentNeuron NeuronContainer ID Layer = " << currentNeuron->neuron->layerID << endl;
			cout << "currentNeuron NeuronContainer ID Subnet = " << currentNeuron->neuron->subnetID << endl;
			//cout << "numberOfInputNeurons = " << currentNeuron->numNeuronsInBackLayerOfSubnet << endl;
			//cout << "numberOfOutputNeurons = " <<  currentNeuron->numNeuronsInFrontLayerOfSubnet << endl;
			#endif

			copyNeuronConnectionContainerListToNeuronContainerList(currentNeuron->firstNeuronContainerInBackLayerOfSubnet, currentNeuron->firstBackNeuronConnectionContainer);
			if(!(currentNeuron->isOutputSubnet))
			{
				copyNeuronConnectionContainerListToNeuronContainerList(currentNeuron->firstNeuronContainerInFrontLayerOfSubnet, currentNeuron->firstFrontNeuronConnectionContainer);
			}

			backwardPassStep(currentNeuron->firstNeuronContainerInFrontLayerOfSubnet, true, true);
				//NB CHECK ANNTHIS new requirement; each subnet must have at least 3 layers

			copyNeuronContainerListToNeuronConnectionContainerList(currentNeuron->firstBackNeuronConnectionContainer, currentNeuron->firstNeuronContainerInBackLayerOfSubnet);
			if(!(currentNeuron->isOutputSubnet))
			{
				copyNeuronContainerListToNeuronConnectionContainerList(currentNeuron->firstFrontNeuronConnectionContainer, currentNeuron->firstNeuronContainerInFrontLayerOfSubnet);
			}
		}
		currentNeuron = currentNeuron->nextNeuronContainer;
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


void calculateOutputErrorOfOutputNeuron(NeuronContainer * neuronBeingAccessed)
{
	#ifdef ANN_DEBUG
	cout << "\nstatic void calculateOutputErrorOfOutputNeuron(NeuronContainer * neuronBeingAccessed)" << endl;
	#endif

	double outputError = (neuronBeingAccessed->neuron->classTarget - neuronBeingAccessed->neuron->output) * neuronBeingAccessed->neuron->output * (1.0 - neuronBeingAccessed->neuron->output);
	neuronBeingAccessed->neuron->error = outputError;

	#ifdef ANN_DEBUG
	cout << "\nNeuron Output error Adjusted" << endl;
	cout << "NeuronContainer ID = " << neuronBeingAccessed->neuron->id << endl;
	cout << "NeuronContainer ID Order = " << neuronBeingAccessed->neuron->orderID << endl;
	cout << "NeuronContainer ID Layer = " << neuronBeingAccessed->neuron->layerID << endl;
	cout << "NeuronContainer ID Subnet = " << neuronBeingAccessed->neuron->subnetID << endl;
	cout << "NeuronContainer classTarget = " << neuronBeingAccessed->neuron->classTarget << endl;
	cout << "NeuronContainer Error = " << neuronBeingAccessed->neuron->error << endl;
	cout << "NeuronContainer Output = " << neuronBeingAccessed->neuron->output << endl;
	#endif
}

void calculateOutputErrorOfNonoutputNeuron(NeuronContainer * neuronBeingAccessed)
{
	#ifdef ANN_DEBUG
	cout << "\nstatic void calculateOutputErrorOfNonoutputNeuron(NeuronContainer * neuronBeingAccessed)" << endl;
	#endif

	#ifdef ANN_DEBUG
	cout << "\nNeuron Output error Adjusted" << endl;
	cout << "NeuronContainer ID = " << neuronBeingAccessed->neuron->id << endl;
	cout << "NeuronContainer ID Order = " << neuronBeingAccessed->neuron->orderID << endl;
	cout << "NeuronContainer ID Layer = " << neuronBeingAccessed->neuron->layerID << endl;
	cout << "NeuronContainer ID Subnet = " << neuronBeingAccessed->neuron->subnetID << endl;
	#endif

	NeuronConnectionContainer * currentConnectionContainer = neuronBeingAccessed->firstFrontNeuronConnectionContainer;

	float partOfHiddenError = 0.0;
	while(currentConnectionContainer->nextNeuronConnectionContainer != NULL)
	{
		//numberOfChickens++;

		partOfHiddenError = partOfHiddenError + ((currentConnectionContainer->neuronConnection->weight) * (currentConnectionContainer->neuron->error));
		/*partOfHiddenError = partOfHiddenError + (weights2[i*numberOfOutputNeurons+j]*errors2[j]);*/

		#ifdef ANN_DEBUG
		cout << "Front NeuronContainer Error = " << currentConnectionContainer->neuron->error << endl;
		cout << "Front NeuronContainer Connection Weight = " << currentConnectionContainer->neuronConnection->weight << endl;
		cout << "partOfHiddenError = " << partOfHiddenError << "\n" << endl;
		#endif

		currentConnectionContainer = currentConnectionContainer->nextNeuronConnectionContainer;


	}

	neuronBeingAccessed->neuron->error = (neuronBeingAccessed->neuron->output) * (1.0 - (neuronBeingAccessed->neuron->output)) * partOfHiddenError;

	#ifdef ANN_DEBUG
	cout << "\npartOfHiddenError = " << partOfHiddenError << "\n" << endl;
	cout << "NeuronContainer output  = " << neuronBeingAccessed->neuron->output << endl;
	cout << "NeuronContainer Error = " << neuronBeingAccessed->neuron->error << endl;
	#endif
}


void calculateNewBackConnectionWeightsOfNeuron(NeuronContainer * neuronBeingAccessed)
{
	#ifdef ANN_DEBUG
	cout << "\nstatic void calculateNewBackConnectionWeightsOfNeuron(NeuronContainer * neuronBeingAccessed)" << endl;
	#endif

	#ifdef ANN_DEBUG
	cout << "\nNeuron Back Weight Value Adjusted" << endl;
	cout << "NeuronContainer ID = " << neuronBeingAccessed->neuron->id << endl;
	cout << "NeuronContainer ID Order = " << neuronBeingAccessed->neuron->orderID << endl;
	cout << "NeuronContainer ID Layer = " << neuronBeingAccessed->neuron->layerID << endl;
	cout << "NeuronContainer ID Subnet = " << neuronBeingAccessed->neuron->subnetID << endl;
	cout << "MU = " << MU << endl;
	cout << "NeuronContainer Error = " << neuronBeingAccessed->neuron->error << endl;
	#endif

	NeuronConnectionContainer * currentConnectionContainer = neuronBeingAccessed->firstBackNeuronConnectionContainer;

	while(currentConnectionContainer->nextNeuronConnectionContainer != NULL)
	{
		float weightChange;
		weightChange = MU * neuronBeingAccessed->neuron->error * currentConnectionContainer->neuron->output;

		currentConnectionContainer->neuronConnection->weight = currentConnectionContainer->neuronConnection->weight + weightChange;

		#ifdef ANN_DEBUG
		cout << "Back NeuronContainer Output = " << currentConnectionContainer->neuron->output << endl;
		cout << "Back NeuronContainer Connection Weight = " << currentConnectionContainer->neuronConnection->weight << endl;
		#endif

		currentConnectionContainer = currentConnectionContainer->nextNeuronConnectionContainer;
	}
}

void calculateNewBiasOfNeuron(NeuronContainer * neuronBeingAccessed)
{
	#ifdef ANN_DEBUG
	cout << "\nstatic void calculateNewBiasOfNeuron(NeuronContainer * neuronBeingAccessed)" << endl;
	#endif

	double biasChange;
	biasChange = MU * neuronBeingAccessed->neuron->error * 1.0;	//BIASES ARE WEIGHTED WIANNTH INPUT 1
	neuronBeingAccessed->neuron->bias = neuronBeingAccessed->neuron->bias + biasChange;

	#ifdef ANN_DEBUG
	cout << "\nNeuron Bias Value Adjusted" << endl;
	cout << "neuron ID = " << neuronBeingAccessed->neuron->id << endl;
	cout << "NeuronContainer ID Order = " << neuronBeingAccessed->neuron->orderID << endl;
	cout << "NeuronContainer ID Layer = " << neuronBeingAccessed->neuron->layerID << endl;
	cout << "NeuronContainer ID Subnet = " << neuronBeingAccessed->neuron->subnetID << endl;
	cout << "MU = " << MU << endl;
	cout << "neuron Error = " << neuronBeingAccessed->neuron->error << endl;
	cout << "neuron Bias = " << neuronBeingAccessed->neuron->bias << endl;
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

















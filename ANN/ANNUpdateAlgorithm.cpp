/*******************************************************************************
 *
 * File Name: ANNUpdateAlgorithm.cpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2012 Baxter AI (baxterai.com)
 * Project: Advanced Neural Network (ANN)
 * Project Version: 3a7a 06-June-2012
 * Comments:
 *
 *******************************************************************************/

#include "ANNUpdateAlgorithm.h"
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






//DEBUG
int dataSetNum;

//long numberOfChickens;

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

		#ifdef DEBUG
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

	#ifdef DEBUG
	cout << "total error = " << totalError << "\n" << endl;
	//error checking exit
	//exit(0);
	#endif


	//basic total error calculation in C
	/*
	float total2xError = 0;
	for(i = 0; i < numberOfOutputNeurons; i++)
	{

		total2xError = total2xError + pow((classTarget[i] - outputs2[i]),2);
		#ifdef DEBUG
		cout << "classTarget[" << i << "] = " << classTarget[i] << "\n" << endl;
		cout << "outputs2[" << i << "] = " << outputs2[i] << "\n" << endl;
		cout << "total2xError = " << total2xError << "\n" << endl;
		#endif
	}

	float totalError = 0.5F * total2xError;

	#ifdef DEBUG
	cout << "total error = " << totalError << "\n" << endl;
	//error checking exit
	//exit(0);
	#endif
	*/

	return total2xError;
}

double ANNBackPropogationPass(NeuronContainer * firstInputNeuronInNetwork, NeuronContainer * firstOutputNeuronInNetwork)
{
	//numberOfChickens = 0;

	#ifdef DEBUG
	cout << "\ndouble ANNBackPropogationPass(NeuronContainer * firstInputNeuronInNetwork, NeuronContainer * firstOutputNeuronInNetwork)" << endl;
	#endif

	forwardPassStep(firstInputNeuronInNetwork);

	backwardPassStep(firstOutputNeuronInNetwork, true, false);

	#ifdef DEBUG_TRAIN_NETWORK_WITH_NON_RANDOM_VARS
	//cout << "numberOfChickens = " << numberOfChickens << endl;
	cout << "DEBUG_TRAIN_NETWORK_WITH_NON_RANDOM_VARS halting program" << endl;
	exit(0);
	#endif

	return calculateErrorOfBackPropPass(firstOutputNeuronInNetwork);
}
/************************************************************ End Main Neural Network Learning Routines with a class Target set ***************************************************/





#ifdef ANN

void copyNeuronContainerListToNeuronConnectionContainerList(NeuronConnectionContainer * firstNeuronConnectionInListToUpdate, NeuronContainer * firstNeuronInListToCopy)
{
	NeuronConnectionContainer * currentNeuronConnectionContainerToUpdate = firstNeuronConnectionInListToUpdate;
	NeuronContainer * currentNeuronContainerToCopy = firstNeuronInListToCopy;

	while(currentNeuronConnectionContainerToUpdate -> nextNeuronConnectionContainer != NULL)
	{
		//cout << "h3c" << endl;

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

	while(currentNeuronContainerToUpdate -> nextNeuronContainer != NULL)		//this will fail; it appears as the neuron connection containers have less neuron connections than neurons in the layer - this is a mistake
	//while(currentNeuronConnectionContainerToCopy -> nextNeuronConnectionContainer != NULL)
	{
		//cout << "h3b" << endl;
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
	#ifdef DEBUG
	cout << "\nstatic void forwardPassStep(NeuronContainer * neuronBeingAccessed)" << endl;
	cout << "NeuronContainer ID = " << neuronBeingAccessed->neuron->id << endl;
	cout << "NeuronContainer ID Order = " << neuronBeingAccessed->neuron->orderID << endl;
	cout << "NeuronContainer ID Layer = " << neuronBeingAccessed->neuron->layerID << endl;
	cout << "NeuronContainer ID Subnet = " << neuronBeingAccessed->neuron->subnetID << endl;
	#endif

	NeuronContainer * currentNeuron = neuronBeingAccessed->firstNeuronInFrontLayer;


	while(currentNeuron -> nextNeuronContainer != NULL)
	{
		#ifdef DEBUG
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
	//exit(0);
	
	//ANN recursion
	#ifdef ANN



	//NB if !isInputSubnet, subnets require a back layer to exist in the top level network
	currentNeuron = neuronBeingAccessed;
	while(currentNeuron -> nextNeuronContainer != NULL)
	{
		if(currentNeuron->isSubnet)
		{
			//cout << "h1" << endl;

			#ifdef DEBUG
			cout << "\nAbout to perform ANN recursion in forwardPassStep ... " << endl;
			cout << "currentNeuron NeuronContainer ID = " << currentNeuron->neuron->id << endl;
			cout << "currentNeuron NeuronContainer ID Order = " << currentNeuron->neuron->orderID << endl;
			cout << "currentNeuron NeuronContainer ID Layer = " << currentNeuron->neuron->layerID << endl;
			cout << "currentNeuron NeuronContainer ID Subnet = " << currentNeuron->neuron->subnetID << endl;
			//cout << "numberOfInputNeurons = " << currentNeuron->numNeuronsInBackLayerOfSubnet << endl;
			//cout << "numberOfOutputNeurons = " <<  currentNeuron->numNeuronsInFrontLayerOfSubnet << endl;
			#endif

			//cout << "h1a" << endl;
			if(!(currentNeuron->isInputSubnet))
			{
				//cout << "h1b" << endl;
				copyNeuronConnectionContainerListToNeuronContainerList(currentNeuron->firstNeuronContainerInBackLayerOfSubnet, currentNeuron->firstBackNeuronConnectionContainer);
			}

			//cout << "h1c" << endl;
			copyNeuronConnectionContainerListToNeuronContainerList(currentNeuron->firstNeuronContainerInFrontLayerOfSubnet, currentNeuron->firstFrontNeuronConnectionContainer);
			//cout << "h1d" << endl;
			forwardPassStep(currentNeuron->firstNeuronContainerInBackLayerOfSubnet);	//ISSUE HERE
			//cout << "h1e" << endl;

			if(!(currentNeuron->isInputSubnet))
			{
				copyNeuronContainerListToNeuronConnectionContainerList(currentNeuron->firstBackNeuronConnectionContainer, currentNeuron->firstNeuronContainerInBackLayerOfSubnet);
			}

			//cout << "h1f" << endl;
			copyNeuronContainerListToNeuronConnectionContainerList(currentNeuron->firstFrontNeuronConnectionContainer, currentNeuron->firstNeuronContainerInFrontLayerOfSubnet);

			//cout << "h2" << endl;
		}
		currentNeuron = currentNeuron->nextNeuronContainer;


	}



	#endif


	//recursion

	if(neuronBeingAccessed->firstNeuronInFrontLayer->hasFrontLayer)
	{
		forwardPassStep(neuronBeingAccessed->firstNeuronInFrontLayer);
	}




	//basic forwards pass step algorithm in C
	/*
	//forward pass

	//calaculates outputs1 (hidden NeuronContainer outputs);
	for(i = 0; i < numberOfHiddenNeurons; i++)
	{
		float netI = 0.0F;
		for(j=0;j<numberOfInputNeurons;j++)
		{
			#ifdef DEBUG
			cout << "outputs0[" << j << "] = " << outputs0[j] << "\n" << endl;
			cout << "weights1[" << j*numberOfHiddenNeurons+i << "] = " << weights1[j*numberOfHiddenNeurons+i] << "\n" << endl;
			#endif

			netI = netI + (outputs0[j]*weights1[j*numberOfHiddenNeurons+i]);

		}

		netI = netI + biases1[i];
		outputs1[i] = calculateOValue(netI);	//set hidden neurons output

		#ifdef DEBUG
		cout << "netI " << i << " = " << netI << "\n" << endl;
		cout << "biases1[" << i << "] = " << biases1[i] << "\n" << endl;
		cout << "outputs1[" << i << "] = " << outputs1[i] << "\n" << endl;
		//error checking exit
		//exit(0);
		#endif

	}
	*/



}


void DEBUGsetDataSetNum(int newdataSetNum)
{
	dataSetNum = newdataSetNum;
}

//Only [top level]/[foward level]/[final] Output Neurons have "Class Target" values
void adjustOutputValueOfANeuronBasedOnBackNeurons(NeuronContainer * neuronBeingAccessed)
{

	#ifdef DEBUG
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
		//numberOfChickens++;

		//cout << "currentConnectionContainer->neuron->output = " << currentConnectionContainer->neuron->output << endl;
		
		netI = netI + (currentConnectionContainer->neuron->output * currentConnectionContainer->neuronConnection->weight);
		/*netI = netI + (outputs0[j]*weights1[j*numberOfHiddenNeurons+i]);*/

		#ifdef DEBUG_TRAIN_NETWORK_WITH_NON_RANDOM_VARS
		cout << "currentConnectionContainer->neuron->id = " << currentConnectionContainer->neuron->id << endl;
		#endif


		#ifdef DEBUG
		cout << "Back NeuronContainer Output = " << currentConnectionContainer->neuron->output << endl;
		cout << "Back NeuronContainer Connection Weight = " << currentConnectionContainer->neuronConnection->weight << endl;
		cout << "NeuronContainer netI = " << netI << endl;
		#endif

		/*DEBUG:
		if(currentConnectionContainer->neuron->output > 100)
		{//issue not here
			cout << "currentConnectionContainer->neuron->output > 100" << endl;
		}
		*/

		/*
			//only ever neurons 11 and 12 in this network have unusually high weighted connections to their previous layer - probably just an artefact of the particular trainign set. this error probably only occurs when subnets are used
		if(currentConnectionContainer->neuronConnection->weight > 100)		//&& ((neuronBeingAccessed->neuron->id < 11) || (neuronBeingAccessed->neuron->id > 12))
		{//found issue here

			cout << "DEBUG: ISSUE FOUND" << endl;
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
			exit(0);

		}
		*/


		currentConnectionContainer = currentConnectionContainer->nextNeuronConnectionContainer;
	}

	netI = netI + neuronBeingAccessed->neuron->bias;
	/*netI = netI + biases1[i];*/

	//cout << "netI = " << netI << endl;
	
	/*DEBUG:
	if(neuronBeingAccessed->neuron->bias > 100)
	{
		cout << "neuronBeingAccessed->neuron->bias > 100" << endl;
	}
	*/
	/*DEBUG:
	if(netI > 999.99)
	{
		cout << "error: netValue > 999.99";
		//string temp;
		//cin >> temp;
		//exit(0);
	}
	*/

	
	
	neuronBeingAccessed->neuron->output = calculateOValue(netI);	//set ("hidden") NeuronContainer output value

	#ifdef DEBUG
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
	#ifdef DEBUG
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
#ifdef ANN



	//NBOLD if !isOutputSubnet, subnets require a front layer to exist in the top level network
	currentNeuron = neuronBeingAccessed;
	while(currentNeuron -> nextNeuronContainer != NULL)
	{
		if(currentNeuron->isSubnet)
		{
			//cout << "h3" << endl;

			#ifdef DEBUG
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

			//OLD: backwardPassStep(currentNeuron->firstNeuronContainerInFrontLayerOfSubnet->firstNeuronInBackLayer, false);
			backwardPassStep(currentNeuron->firstNeuronContainerInFrontLayerOfSubnet, true, true);
					//NB CHECK ANNTHIS new requirement; each subnet must have at least 3 layers

			copyNeuronContainerListToNeuronConnectionContainerList(currentNeuron->firstBackNeuronConnectionContainer, currentNeuron->firstNeuronContainerInBackLayerOfSubnet);
			if(!(currentNeuron->isOutputSubnet))
			{
				copyNeuronContainerListToNeuronConnectionContainerList(currentNeuron->firstFrontNeuronConnectionContainer, currentNeuron->firstNeuronContainerInFrontLayerOfSubnet);
			}

			//	cout << "h4" << endl;
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




	//basic backward pass step algorithm in C
	/*
	if(isOutputLayer)
	{
		//calculate errors of output neurons - errors2 (output errors)

		for(i = 0; i < numberOfOutputNeurons; i++)
		{

			float outputError = (classTarget[i] - outputs2[i]) * outputs2[i] * (1 - outputs2[i]);
			errors2[i] = outputError;

			#ifdef DEBUG
			cout << "classTarget[" << i << "] = " << classTarget[i] << "\n" << endl;
			cout << "outputs2[" << i << "] = " << outputs2[i] << "\n" << endl;
			cout << "errors2[" << i << "] = " << errors2[i] << "\n" << endl;
			#endif
		}
	}
	else
	{
		for(i = 0; i < numberOfHiddenNeurons; i++)
		{
			float hiddenError;

			float partOfHiddenError = 0;	//eg error3 = o3*(1-03)*partOfHiddenError where partOfHiddenError = (w36*o6 +w37*o7)
			for(j = 0; j < numberOfOutputNeurons; j++)
			{
				#ifdef DEBUG
				cout << "weights2[" << i*numberOfOutputNeurons+j << "] = " << weights2[i*numberOfOutputNeurons+j] << "\n" << endl;
				cout << "errors2[" << j << "] = " << errors2[j] << "\n" << endl;
				cout << "partOfHiddenError = " << partOfHiddenError << "\n" << endl;
				#endif
				partOfHiddenError = partOfHiddenError + (weights2[i*numberOfOutputNeurons+j]*errors2[j]);
			}

			errors1[i] = outputs1[i]*(1-outputs1[i])*partOfHiddenError;

			#ifdef DEBUG
			cout << "partOfHiddenError = " << partOfHiddenError << "\n" << endl;
			cout << "outputs1[" << i << "] = " << outputs1[i] << "\n" << endl;
			cout << "errors1[" << i << "] = " << errors1[i] << "\n" << endl;
			#endif
		}
	}

	float mu = MU;

	//calculates new weights between the hidden and output neurons

	for(i = 0; i < numberOfHiddenNeurons; i++)
	{
		for(j = 0; j < numberOfOutputNeurons; j++)
		{
			float weightChange;
			weightChange = mu * errors2[j] * outputs1[i];

			weights2[i* numberOfOutputNeurons+j] = weights2[i* numberOfOutputNeurons+j] + weightChange;

			#ifdef DEBUG
			cout << "mu = " << mu << "\n" << endl;
			cout << "errors2[" << j << "] = " << errors2[j] << "\n" << endl;
			cout << "outputs1[" << i << "] = " << outputs1[i] << "\n" << endl;
			cout << "weights2[" << i*numberOfOutputNeurons+j << "] = " << weights2[i*numberOfOutputNeurons+j] << "\n" << endl;
			#endif

		}
	}

	//calculates new biases of the output neurons

	for(i = 0; i < numberOfOutputNeurons; i++)
	{
		float biasChange;
		biasChange = mu * errors2[i] * 1.0F;	//BIASES ARE WEIGHTED WIANNTH INPUT 1
		biases2[i] = biases2[i] + biasChange;

		#ifdef DEBUG
		cout << "errors2[" << i << "] = " << errors2[i] << "\n" << endl;
		cout << "biases2[" << i << "] = " << biases2[i] << "\n" << endl;
		#endif
	}
	*/

}



void calculateOutputErrorOfOutputNeuron(NeuronContainer * neuronBeingAccessed)
{
	//numberOfChickens++;

	#ifdef DEBUG
	cout << "\nstatic void calculateOutputErrorOfOutputNeuron(NeuronContainer * neuronBeingAccessed)" << endl;
	#endif

	double outputError = (neuronBeingAccessed->neuron->classTarget - neuronBeingAccessed->neuron->output) * neuronBeingAccessed->neuron->output * (1.0 - neuronBeingAccessed->neuron->output);
	neuronBeingAccessed->neuron->error = outputError;

	#ifdef DEBUG
	cout << "\nNeuron Output error Adjusted" << endl;
	cout << "NeuronContainer ID = " << neuronBeingAccessed->neuron->id << endl;
	cout << "NeuronContainer ID Order = " << neuronBeingAccessed->neuron->orderID << endl;
	cout << "NeuronContainer ID Layer = " << neuronBeingAccessed->neuron->layerID << endl;
	cout << "NeuronContainer ID Subnet = " << neuronBeingAccessed->neuron->subnetID << endl;
	cout << "NeuronContainer classTarget = " << neuronBeingAccessed->neuron->classTarget << endl;
	cout << "NeuronContainer Error = " << neuronBeingAccessed->neuron->error << endl;
	cout << "NeuronContainer Output = " << neuronBeingAccessed->neuron->output << endl;
	#endif

	/*
	float outputError = (classTarget[i] - outputs2[i]) * outputs2[i] * (1 - outputs2[i]);
	errors2[i] = outputError;
	*/

}

void calculateOutputErrorOfNonoutputNeuron(NeuronContainer * neuronBeingAccessed)
{
	#ifdef DEBUG
	cout << "\nstatic void calculateOutputErrorOfNonoutputNeuron(NeuronContainer * neuronBeingAccessed)" << endl;
	#endif

	#ifdef DEBUG
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

		#ifdef DEBUG
		cout << "Front NeuronContainer Error = " << currentConnectionContainer->neuron->error << endl;
		cout << "Front NeuronContainer Connection Weight = " << currentConnectionContainer->neuronConnection->weight << endl;
		cout << "partOfHiddenError = " << partOfHiddenError << "\n" << endl;
		#endif

		currentConnectionContainer = currentConnectionContainer->nextNeuronConnectionContainer;


	}

	neuronBeingAccessed->neuron->error = (neuronBeingAccessed->neuron->output) * (1.0 - (neuronBeingAccessed->neuron->output)) * partOfHiddenError;
	/*errors1[i] = outputs1[i]*(1-outputs1[i])*partOfHiddenError;*/

	#ifdef DEBUG
	cout << "\npartOfHiddenError = " << partOfHiddenError << "\n" << endl;
	cout << "NeuronContainer output  = " << neuronBeingAccessed->neuron->output << endl;
	cout << "NeuronContainer Error = " << neuronBeingAccessed->neuron->error << endl;
	#endif
}


void calculateNewBackConnectionWeightsOfNeuron(NeuronContainer * neuronBeingAccessed)
{
	#ifdef DEBUG
	cout << "\nstatic void calculateNewBackConnectionWeightsOfNeuron(NeuronContainer * neuronBeingAccessed)" << endl;
	#endif

	#ifdef DEBUG
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
		//numberOfChickens++;

		float weightChange;
		weightChange = MU * neuronBeingAccessed->neuron->error * currentConnectionContainer->neuron->output;


		/*
		if(weightChange > 3)
		{//issue not here;
			cout << "weightChange > 3" << endl;
		}
		*/
		/*
		if(currentConnectionContainer->neuronConnection->weight > 100)
		{//issue is here; therefore issue must relates to collective addition weights of neurons - CORRECT
			cout << currentConnectionContainer->neuronConnection->weight << endl;
		}
		*/

		currentConnectionContainer->neuronConnection->weight = currentConnectionContainer->neuronConnection->weight + weightChange;

		#ifdef DEBUG
		cout << "Back NeuronContainer Output = " << currentConnectionContainer->neuron->output << endl;
		cout << "Back NeuronContainer Connection Weight = " << currentConnectionContainer->neuronConnection->weight << endl;
		#endif

		currentConnectionContainer = currentConnectionContainer->nextNeuronConnectionContainer;

	}

	/*
	float weightChange;
	weightChange = mu * errors2[j] * outputs1[i];

	weights2[i* numberOfOutputNeurons+j] = weights2[i* numberOfOutputNeurons+j] + weightChange;
	*/

}

void calculateNewBiasOfNeuron(NeuronContainer * neuronBeingAccessed)
{
	//numberOfChickens++;
	#ifdef DEBUG
	cout << "\nstatic void calculateNewBiasOfNeuron(NeuronContainer * neuronBeingAccessed)" << endl;
	#endif

	double biasChange;
	biasChange = MU * neuronBeingAccessed->neuron->error * 1.0;	//BIASES ARE WEIGHTED WIANNTH INPUT 1
	neuronBeingAccessed->neuron->bias = neuronBeingAccessed->neuron->bias + biasChange;

	#ifdef DEBUG
	cout << "\nNeuron Bias Value Adjusted" << endl;
	cout << "neuron ID = " << neuronBeingAccessed->neuron->id << endl;
	cout << "NeuronContainer ID Order = " << neuronBeingAccessed->neuron->orderID << endl;
	cout << "NeuronContainer ID Layer = " << neuronBeingAccessed->neuron->layerID << endl;
	cout << "NeuronContainer ID Subnet = " << neuronBeingAccessed->neuron->subnetID << endl;
	cout << "MU = " << MU << endl;
	cout << "neuron Error = " << neuronBeingAccessed->neuron->error << endl;
	cout << "neuron Bias = " << neuronBeingAccessed->neuron->bias << endl;
	#endif

	/*
	float biasChange;
	biasChange = mu * errors2[i] * 1.0F;	//BIASES ARE WEIGHTED WIANNTH INPUT 1
	biases2[i] = biases2[i] + biasChange;
	*/
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

















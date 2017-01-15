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
 * File Name: ANNformation.cpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2012 Baxter AI (baxterai.com)
 * Project: Artificial Neural Network (ANN)
 * Project Version: 3c3a 16-November-2012
 * Comments:
 *
 *******************************************************************************/


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

#include "ANNformation.h"
#include "ANNglobalDefs.h"



static long IDCounter;
static long orderIDcounter;
static long layerIDcounter;
static long subnetIDcounter;

/************************************************************ Form Neural Network Routines ******************************************************/

/*forms a 3 layered net similar to that used in original c project*/
//assumes firstInputNeuron and firstOutputNeuron have already been filled
void formSimpleNeuralNet(NeuronContainer * firstInputNeuron, NeuronContainer * firstOutputNeuron, long numberOfInputNeurons, long numberOfHiddenNeurons, long numberOfOutputNeurons)
{
	/*NB simple neural network heursitc;
	number of hidden neurons = average of input and output neurons
	*/

	/*Create all Neurons in Simply Layered Net*/

	NeuronContainer * firstHiddenNeuron = new NeuronContainer();


	//initiate Input Layer in Neural Network

	IDCounter = 1;
	orderIDcounter = 1;
	layerIDcounter = 1;
	subnetIDcounter = 1;
	NeuronContainer * currentNeuron = firstInputNeuron;
	for(long i = 0; i < numberOfInputNeurons; i++)
	{
		fillInNeuronIDProperties(currentNeuron->neuron, IDCounter, orderIDcounter, layerIDcounter, subnetIDcounter);

		currentNeuron->firstNeuronInFrontLayer = firstHiddenNeuron;
		currentNeuron->hasFrontLayer = true;

		NeuronContainer * newNeuron = new NeuronContainer();
		currentNeuron->nextNeuronContainer = newNeuron;
		currentNeuron = currentNeuron->nextNeuronContainer;

		IDCounter++;
		orderIDcounter++;
	}

	//initiate Hidden Layer in Neural Network
	orderIDcounter = 1;
	layerIDcounter++;
	currentNeuron = firstHiddenNeuron;
	for(long i = 0; i < numberOfHiddenNeurons; i++)
	{
		fillInNeuronIDProperties(currentNeuron->neuron, IDCounter, orderIDcounter, layerIDcounter, subnetIDcounter);

		currentNeuron->firstNeuronInFrontLayer = firstOutputNeuron;
		currentNeuron->firstNeuronInBackLayer = firstInputNeuron;
		currentNeuron->hasFrontLayer = true;
		currentNeuron->hasBackLayer = true;

		NeuronContainer * newNeuron = new NeuronContainer();
		currentNeuron->nextNeuronContainer = newNeuron;
		currentNeuron = currentNeuron->nextNeuronContainer;

		IDCounter++;
		orderIDcounter++;
	}

	//initiate Output Layer in Neural Network
	orderIDcounter = 1;
	layerIDcounter++;
	currentNeuron = firstOutputNeuron;
	for(long i = 0; i < numberOfOutputNeurons; i++)
	{
		fillInNeuronIDProperties(currentNeuron->neuron, IDCounter, orderIDcounter, layerIDcounter, subnetIDcounter);
		currentNeuron->firstNeuronInBackLayer = firstHiddenNeuron;
		currentNeuron->hasBackLayer = true;

		NeuronContainer * newNeuron = new NeuronContainer();
		currentNeuron->nextNeuronContainer = newNeuron;
		currentNeuron = currentNeuron->nextNeuronContainer;

		IDCounter++;
		orderIDcounter++;
	}

	/*Link all Neurons in Simply Layered Net - fill firstFrontNeuronConnectionContainer and BackNeuronReferences for all neurons*/


	NeuronContainer * currentNeuronL1;
	NeuronContainer * currentNeuronL2;


	//Link ALL input and hidden layer neurons neurons (fill in firstFrontNeuronConnectionContainer/BackNeuronReferences list for all relevant neurons)

	currentNeuronL1 = firstInputNeuron;
	currentNeuronL2 = firstHiddenNeuron;
	currentNeuronL1->currentFrontNeuronConnectionContainer = currentNeuronL1->firstFrontNeuronConnectionContainer;		//not required here
	currentNeuronL2->currentBackNeuronConnectionContainer = currentNeuronL2->firstBackNeuronConnectionContainer;		//not required here

	for(long i = 0; i < numberOfInputNeurons; i++)
	{
		currentNeuronL2 = firstHiddenNeuron;

		for(long j = 0; j < numberOfHiddenNeurons; j++)
		{
			NeuronConnection * newNeuronConnection = new NeuronConnection();

			NeuronConnectionContainer * newNeuronConnectionContainerL1 = new NeuronConnectionContainer();
			//currentNeuronL1->numFrontNeuronConnections = numberOfHiddenNeurons;	//this variable is filled in more than once
			currentNeuronL1->currentFrontNeuronConnectionContainer->neuron = currentNeuronL2->neuron;
			//currentNeuronL1->currentFrontNeuronConnectionContainer->neuronID = currentNeuronL2->neuron->id;
			currentNeuronL1->currentFrontNeuronConnectionContainer->neuronConnection = newNeuronConnection;
			currentNeuronL1->currentFrontNeuronConnectionContainer->nextNeuronConnectionContainer = newNeuronConnectionContainerL1;
			currentNeuronL1->currentFrontNeuronConnectionContainer = currentNeuronL1->currentFrontNeuronConnectionContainer->nextNeuronConnectionContainer;

			NeuronConnectionContainer * newNeuronConnectionContainerL2 = new NeuronConnectionContainer();
			//currentNeuronL2->numBackNeuronConnections = numberOfInputNeurons;	//this variable is filled in more than once
			currentNeuronL2->currentBackNeuronConnectionContainer->neuron = currentNeuronL1->neuron;
			//currentNeuronL2->currentBackNeuronConnectionContainer->neuronID = currentNeuronL1->neuron->id;
			currentNeuronL2->currentBackNeuronConnectionContainer->neuronConnection = newNeuronConnection;
			currentNeuronL2->currentBackNeuronConnectionContainer->nextNeuronConnectionContainer = newNeuronConnectionContainerL2;
			currentNeuronL2->currentBackNeuronConnectionContainer = currentNeuronL2->currentBackNeuronConnectionContainer->nextNeuronConnectionContainer;

			currentNeuronL2 = currentNeuronL2->nextNeuronContainer;
		}
		currentNeuronL1 = currentNeuronL1->nextNeuronContainer;
	}


	//Link ALL hidden and output layer neurons neurons (fill in firstFrontNeuronConnectionContainer/BackNeuronReferences list for all relevant neurons)

	currentNeuronL1 = firstHiddenNeuron;
	currentNeuronL2 = firstOutputNeuron;
	currentNeuronL1->currentFrontNeuronConnectionContainer = currentNeuronL1->firstFrontNeuronConnectionContainer;
	currentNeuronL2->currentBackNeuronConnectionContainer = currentNeuronL2->firstBackNeuronConnectionContainer;

	for(long i = 0; i < numberOfHiddenNeurons; i++)
	{
		currentNeuronL2 = firstOutputNeuron;

		for(long j = 0; j < numberOfOutputNeurons; j++)
		{
			NeuronConnection * newNeuronConnection = new NeuronConnection();

			NeuronConnectionContainer * newNeuronConnectionContainerL1 = new NeuronConnectionContainer();
			//currentNeuronL1->numFrontNeuronConnections = numberOfOutputNeurons;	//this variable is filled in more than once
			currentNeuronL1->currentFrontNeuronConnectionContainer->neuron = currentNeuronL2->neuron;
			currentNeuronL1->currentFrontNeuronConnectionContainer->neuronID = currentNeuronL2->neuron->id;
			currentNeuronL1->currentFrontNeuronConnectionContainer->neuronConnection = newNeuronConnection;
			currentNeuronL1->currentFrontNeuronConnectionContainer->nextNeuronConnectionContainer = newNeuronConnectionContainerL1;
			currentNeuronL1->currentFrontNeuronConnectionContainer = currentNeuronL1->currentFrontNeuronConnectionContainer->nextNeuronConnectionContainer;

			NeuronConnectionContainer * newNeuronConnectionContainerL2 = new NeuronConnectionContainer();
			//currentNeuronL2->numBackNeuronConnections = numberOfHiddenNeurons;	//this variable is filled in more than once
			currentNeuronL2->currentBackNeuronConnectionContainer->neuron = currentNeuronL1->neuron;
			//currentNeuronL2->currentBackNeuronConnectionContainer->neuronID = currentNeuronL1->neuron->id;
			currentNeuronL2->currentBackNeuronConnectionContainer->neuronConnection = newNeuronConnection;
			currentNeuronL2->currentBackNeuronConnectionContainer->nextNeuronConnectionContainer = newNeuronConnectionContainerL2;
			currentNeuronL2->currentBackNeuronConnectionContainer = currentNeuronL2->currentBackNeuronConnectionContainer->nextNeuronConnectionContainer;

			currentNeuronL2 = currentNeuronL2->nextNeuronContainer;
		}
		currentNeuronL1 = currentNeuronL1->nextNeuronContainer;
	}
}



















/************************************************************ End Form Neural Network Routines ******************************************************/




















/************************************************************ Form Advanced Neural Network Routines ******************************************************/


NeuronContainer * formNeuralNetWithOptimisedProperties(NeuronContainer * firstInputNeuronInNetwork, long numberOfInputNeurons, long numberOfOutputNeurons, long numberOfLayers)
{
	NeuronContainer * firstOutputNeuronInNetwork;

	int layerDivergenceType;
	double meanLayerDivergenceFactor;
	double probabilityNeuronConnectionWithPreviousLayerNeuron;
	double probabilityOfSubnetCreation;
	bool useSubnetDependentNumberOfLayers;
	long maxNumRecursiveSubnets;
	double probabilityNeuronConnectionWithAllPreviousLayersNeurons;

	//set defaults of advanced network structure properties

#ifdef TEST_LAYER_DIVERGENCE_TYPE_LINEAR_DIVERGING_THEN_CONVERGING
	layerDivergenceType = LAYER_DIVERGENCE_TYPE_LINEAR_DIVERGING_THEN_CONVERGING; //DEFAULT = LAYER_DIVERGENCE_TYPE_LINEAR_CONVERGING;
	meanLayerDivergenceFactor = 1.2;	//DEFAULT = DEFAULT_MEAN_LAYER_DIVERGENCE_FACTOR
#else
	layerDivergenceType = LAYER_DIVERGENCE_TYPE_LINEAR_CONVERGING;
	meanLayerDivergenceFactor = DEFAULT_MEAN_LAYER_DIVERGENCE_FACTOR;
#endif
	probabilityNeuronConnectionWithPreviousLayerNeuron = DEFAULT_PROBABILITY_NEURON_CONNECTION_WITH_PREVIOUS_LAYER_NEURON_ANNTH;
	probabilityOfSubnetCreation = DEFAULT_PROB_OF_SUBNET_CREATION_ANNTH;
	useSubnetDependentNumberOfLayers = DEFAULT_USE_SUBNET_DEPENDENT_NUMLAYERS_ANNTH;
	maxNumRecursiveSubnets = DEFAULT_ANN_MAX_NUM_RECURSIVE_SUBNETS_ANNTH;
	probabilityNeuronConnectionWithAllPreviousLayersNeurons = DEFAULT_PROBABILITY_NEURON_CONNECTION_WITH_ALL_PREVIOUS_LAYERS_NEURONS_ANNTH;

	firstOutputNeuronInNetwork = formNeuralNet(firstInputNeuronInNetwork, numberOfInputNeurons, numberOfOutputNeurons, numberOfLayers, layerDivergenceType, meanLayerDivergenceFactor, probabilityNeuronConnectionWithPreviousLayerNeuron, probabilityNeuronConnectionWithAllPreviousLayersNeurons);

	return firstOutputNeuronInNetwork;
}

NeuronContainer * formNeuralNet(NeuronContainer * firstInputNeuron, long numberOfInputNeurons, long numberOfOutputNeurons, long numberOfLayers, int layerDivergenceType, double meanLayerDivergenceFactor, double probabilityNeuronConnectionWithPreviousLayerNeuron, double probabilityNeuronConnectionWithAllPreviousLayersNeurons)
{
	#ifdef ANN_DEBUG
	cout << "\nNeuronContainer * formNeuralNet()\n" << endl;
	#endif

	NeuronContainer * firstOutputNeuronInNetwork;

	double probabilityOfSubnetCreation = 0.0;	/*no divergence = no ANN*/
	long maxNumberOfRecursiveSubnets = 0;		/*no recursive subnets = no ANN*/
	double subnetNumberOfLayersModifier = 0.0;
	bool useSubnetDependentNumberOfLayers = false;

	firstOutputNeuronInNetwork = formNonDistinctLayeredNeuralNetwork(firstInputNeuron, numberOfInputNeurons, numberOfOutputNeurons, numberOfLayers, probabilityOfSubnetCreation, maxNumberOfRecursiveSubnets, useSubnetDependentNumberOfLayers, subnetNumberOfLayersModifier, layerDivergenceType, meanLayerDivergenceFactor, probabilityNeuronConnectionWithPreviousLayerNeuron, probabilityNeuronConnectionWithAllPreviousLayersNeurons);

	return firstOutputNeuronInNetwork;
}


#ifdef ANN_ADVANCED
NeuronContainer * formAdvancedNeuralNetwork(NeuronContainer * firstInputNeuron, long numberOfInputNeurons, long numberOfOutputNeurons, bool useSubnetDependentNumberOfLayers, double probabilityOfSubnetCreation, long maxNumberOfRecursiveSubnets, long numberOfLayers, int layerDivergenceType, double meanLayerDivergenceFactor, double probabilityNeuronConnectionWithPreviousLayerNeuron, double probabilityNeuronConnectionWithAllPreviousLayersNeurons)
{
	#ifdef ANN_DEBUG
	cout << "\nNeuronContainer * formAdvancedNeuralNetwork()\n" << endl;
	#endif

	double subnetNumberOfLayersModifier = ((numberOfInputNeurons+numberOfOutputNeurons)/2) * numberOfLayers;		//or numberOfLayers = subnetNumberOfLayersModifier / averageNumberOfInputOutputNeurons

	NeuronContainer * firstOutputNeuronInNetwork;

	firstOutputNeuronInNetwork = formNonDistinctLayeredNeuralNetwork(firstInputNeuron, numberOfInputNeurons, numberOfOutputNeurons, numberOfLayers, probabilityOfSubnetCreation, maxNumberOfRecursiveSubnets, useSubnetDependentNumberOfLayers, subnetNumberOfLayersModifier, layerDivergenceType, meanLayerDivergenceFactor, probabilityNeuronConnectionWithPreviousLayerNeuron, probabilityNeuronConnectionWithAllPreviousLayersNeurons);

	return firstOutputNeuronInNetwork;
}
#endif

NeuronContainer * formNonDistinctLayeredNeuralNetwork(NeuronContainer * firstInputNeuronInNetwork, long numberOfInputNeurons, long numberOfOutputNeurons, long numberOfLayers, double probabilityOfSubnetCreation, long maxNumberOfRecursiveSubnets, bool useSubnetDependentNumberOfLayers, double subnetNumberOfLayersModifier, int layerDivergenceType, double meanLayerDivergenceFactor, double probabilityNeuronConnectionWithPreviousLayerNeuron, double probabilityNeuronConnectionWithAllPreviousLayersNeurons)
{
	#ifdef ANN_DEBUG
	cout << "\nNeuronContainer * formNonDistinctLayeredNeuralNetwork()\n" << endl;
	#endif

	NeuronContainer * firstOutputNeuronInNetwork;

	if((layerDivergenceType == LAYER_DIVERGENCE_TYPE_LINEAR_DIVERGING_SQUARE2D) || (layerDivergenceType == LAYER_DIVERGENCE_TYPE_LINEAR_DIVERGING_SQUARE2D_RADIALBIAS) || (layerDivergenceType == LAYER_DIVERGENCE_TYPE_LINEAR_CONVERGING_SQUARE2D))
	{
 		createInputLayerInNeuralNetwork2D(firstInputNeuronInNetwork, numberOfInputNeurons);
	}
	else
	{
 		createInputLayerInNeuralNetwork(firstInputNeuronInNetwork, numberOfInputNeurons);
	}

	firstOutputNeuronInNetwork = fillNonDistinctHiddenLayer(firstInputNeuronInNetwork, numberOfInputNeurons, numberOfOutputNeurons, numberOfInputNeurons, 1, numberOfLayers, probabilityOfSubnetCreation, maxNumberOfRecursiveSubnets, 1, useSubnetDependentNumberOfLayers, subnetNumberOfLayersModifier, layerDivergenceType, meanLayerDivergenceFactor, probabilityNeuronConnectionWithPreviousLayerNeuron, firstInputNeuronInNetwork, probabilityNeuronConnectionWithAllPreviousLayersNeurons);

	return firstOutputNeuronInNetwork;
}



























void createInputLayerInNeuralNetwork(NeuronContainer * firstInputNeuronInNetwork, long numberOfInputNeurons)
{
	IDCounter = 1;	//	IDCounter = DEFAULT_FIRST_INPUT_NEURON_ID+1;
	orderIDcounter = 1;
	layerIDcounter = 1;
	subnetIDcounter = 1;

	//initiate Input Layer in Neural Network
	NeuronContainer * currentNeuron = firstInputNeuronInNetwork;

	for(long i = 0; i < numberOfInputNeurons; i++)
	{
		fillInNeuronIDProperties(currentNeuron->neuron, IDCounter, orderIDcounter, layerIDcounter, subnetIDcounter);

		NeuronContainer * newNeuron = new NeuronContainer();
		currentNeuron->nextNeuronContainer = newNeuron;
		currentNeuron = currentNeuron->nextNeuronContainer;

		IDCounter++;
		orderIDcounter++;
	}
}

void createInputLayerInNeuralNetwork2D(NeuronContainer * firstInputNeuronInNetwork, long numberOfInputNeurons)
{
	IDCounter = 1;	//	IDCounter = DEFAULT_FIRST_INPUT_NEURON_ID+1;
	orderIDcounter = 1;
	layerIDcounter = 1;
	subnetIDcounter = 1;

	//initiate Input Layer in Neural Network
	NeuronContainer * currentNeuron = firstInputNeuronInNetwork;

	long xPosRel = 0;
	long yPosRel = 0;

	for(long i = 0; i < numberOfInputNeurons; i++)
	{
		fillInNeuronIDProperties(currentNeuron->neuron, IDCounter, orderIDcounter, layerIDcounter, subnetIDcounter);

		currentNeuron->neuron->spatialCoordinatesSet = true;
		currentNeuron->neuron->xPosRel = xPosRel;
		currentNeuron->neuron->yPosRel = yPosRel;

		NeuronContainer * newNeuron = new NeuronContainer();
		currentNeuron->nextNeuronContainer = newNeuron;
		currentNeuron = currentNeuron->nextNeuronContainer;

		IDCounter++;
		orderIDcounter++;

		xPosRel++;
		if(xPosRel >= (sqrt(double(numberOfInputNeurons))))
		{
			xPosRel = 0;
			yPosRel++;
		}
	}
}


void createNewFrontLayer(NeuronContainer * firstNeuronInCurrentLayer, NeuronContainer * firstNeuronInNewFrontLayer, long numberOfNeuronsInNewFrontLayer, long currentNumberOfLayers, long numberOfLayers)
{
	layerIDcounter = currentNumberOfLayers+1;
	orderIDcounter=1;

	NeuronContainer * currentNeuron = firstNeuronInNewFrontLayer;
	//fill in network description variables for new front layer (CHECK ANNTHIS - this is incomplete)
	for(long i=0; i<numberOfNeuronsInNewFrontLayer; i++)
	{
		fillInNeuronIDProperties(currentNeuron->neuron, IDCounter, orderIDcounter, layerIDcounter, subnetIDcounter);
		currentNeuron->firstNeuronInBackLayer = firstNeuronInCurrentLayer;
		currentNeuron->hasBackLayer = true;
		if(currentNumberOfLayers != (numberOfLayers-1))
		{
			currentNeuron->hasFrontLayer = true;	//CHECK ANNTHIS - need to update ANNupdateAlgorithm.cpp accordingly
		}

		NeuronContainer * newNeuron = new NeuronContainer();
		currentNeuron->nextNeuronContainer = newNeuron;
		currentNeuron = currentNeuron->nextNeuronContainer;

		IDCounter++;
		orderIDcounter++;
	}

	currentNeuron = firstNeuronInCurrentLayer;
	long numberOfNeuronsOnCurrentLayer = 0;
	//fill in network description variables for current layer (CHECK ANNTHIS - this is incomplete)
	while(currentNeuron -> nextNeuronContainer != NULL)
	{
		currentNeuron->hasFrontLayer = true;
		currentNeuron->firstNeuronInFrontLayer = firstNeuronInNewFrontLayer;

		currentNeuron = currentNeuron->nextNeuronContainer;
		numberOfNeuronsOnCurrentLayer++;
	}
}

void linkNewFrontLayerWithPreviousLayers(NeuronContainer * firstNeuronInCurrentLayer, NeuronContainer * firstNeuronInNewFrontLayer, double probabilityNeuronConnectionWithPreviousLayerNeuron, NeuronContainer * firstInputNeuronInSubnet, double probabilityNeuronConnectionWithAllPreviousLayersNeurons, long numberOfInputNeurons, long numberOfOutputNeurons, long currentNumberOfLayers)
{
	//Modify associations/links based upon layers of intermediate/hidden neurons
	//if probabilityNeuronConnectionWithPreviousLayerNeuron is 1.0, link ALL current/back and front layer neurons neurons (fill in firstFrontNeuronConnectionContainer/BackNeuronReferences list for all relevant neurons)

	NeuronContainer * currentNeuronL1;
	NeuronContainer * currentNeuronL2;

	currentNeuronL1 = firstNeuronInCurrentLayer;
	currentNeuronL2 = firstNeuronInNewFrontLayer;
	currentNeuronL1->currentFrontNeuronConnectionContainer = currentNeuronL1->firstFrontNeuronConnectionContainer;
	currentNeuronL2->currentBackNeuronConnectionContainer = currentNeuronL2->firstBackNeuronConnectionContainer;

	while(currentNeuronL1->nextNeuronContainer != NULL)
	{
		currentNeuronL2 = firstNeuronInNewFrontLayer;

		//long numConnectionsWithFrontLayerNeurons = 0;	//No longer required/used...

		while(currentNeuronL2->nextNeuronContainer != NULL)
		{
			double randomNumberBetween0And1 = ((double(abs((short)rand())))/(ABS_OF_SHORT));
			#ifdef ANN_DEBUG
			//cout << "randomNumberBetween0And1 = " << randomNumberBetween0And1 << endl;
			#endif

			if(randomNumberBetween0And1 < probabilityNeuronConnectionWithPreviousLayerNeuron)
			{
				NeuronConnection * newNeuronConnection = new NeuronConnection();

				NeuronConnectionContainer * newNeuronConnectionContainerL1 = new NeuronConnectionContainer();
				currentNeuronL1->currentFrontNeuronConnectionContainer->neuron = currentNeuronL2->neuron;
				currentNeuronL1->currentFrontNeuronConnectionContainer->neuronConnection = newNeuronConnection;
				currentNeuronL1->currentFrontNeuronConnectionContainer->nextNeuronConnectionContainer = newNeuronConnectionContainerL1;
				currentNeuronL1->currentFrontNeuronConnectionContainer = currentNeuronL1->currentFrontNeuronConnectionContainer->nextNeuronConnectionContainer;

				NeuronConnectionContainer * newNeuronConnectionContainerL2 = new NeuronConnectionContainer();
				currentNeuronL2->currentBackNeuronConnectionContainer->neuron = currentNeuronL1->neuron;
				currentNeuronL2->currentBackNeuronConnectionContainer->neuronConnection = newNeuronConnection;
				currentNeuronL2->currentBackNeuronConnectionContainer->nextNeuronConnectionContainer = newNeuronConnectionContainerL2;
				currentNeuronL2->currentBackNeuronConnectionContainer = currentNeuronL2->currentBackNeuronConnectionContainer->nextNeuronConnectionContainer;

				//numConnectionsWithFrontLayerNeurons++	//No longer required/used...
			}

			currentNeuronL2 = currentNeuronL2->nextNeuronContainer;
		}
		//currentNeuronL1->numFrontNeuronConnections = numConnectionsWithFrontLayerNeurons;		//No longer required/used...

		currentNeuronL1 = currentNeuronL1->nextNeuronContainer;
	}


	NeuronContainer * firstNeuronInPreviousLayerX = firstInputNeuronInSubnet;
		//firstNeuronInPreviousLayerX = currentNeuronL1

	long tempCurrentEffectiveNeuronsOnLayer = numberOfInputNeurons;
	long tempCurrentNumNeuronsOnLayer = numberOfInputNeurons;
	for(long tempLayerIndex = 0; tempLayerIndex < currentNumberOfLayers; tempLayerIndex++)
	{
		//tempCurrentNumNeuronsOnNewLayer = (tempCurrentEffectiveNeuronsOnLayer*meanLayerDivergenceFactor);
		//tempCurrentEffectiveNeuronsOnNewLayer = tempCurrentEffectiveNeuronsOnNewLayer + tempCurrentNumNeuronsOnNewLayer;

		NeuronContainer * currentNeuronL1 = firstNeuronInPreviousLayerX;
		NeuronContainer * currentNeuronL2 = firstNeuronInNewFrontLayer;

		while(currentNeuronL1->nextNeuronContainer != NULL)
		{
			currentNeuronL2 = firstNeuronInNewFrontLayer;

			while(currentNeuronL2->nextNeuronContainer != NULL)
			{
				double randomNumberBetween0And1 = ((double(abs((short)rand())))/(ABS_OF_SHORT));
				#ifdef ANN_DEBUG
				//cout << "randomNumberBetween0And1 = " << randomNumberBetween0And1 << endl;
				#endif
				
				if(randomNumberBetween0And1 < probabilityNeuronConnectionWithAllPreviousLayersNeurons)
				{//add connections to previous layers

					NeuronConnection * newNeuronConnection = new NeuronConnection();

					NeuronConnectionContainer * newNeuronConnectionContainerL1 = new NeuronConnectionContainer();
					currentNeuronL1->currentFrontNeuronConnectionContainer->neuron = currentNeuronL2->neuron;
					currentNeuronL1->currentFrontNeuronConnectionContainer->neuronConnection = newNeuronConnection;
					currentNeuronL1->currentFrontNeuronConnectionContainer->nextNeuronConnectionContainer = newNeuronConnectionContainerL1;
					currentNeuronL1->currentFrontNeuronConnectionContainer = currentNeuronL1->currentFrontNeuronConnectionContainer->nextNeuronConnectionContainer;

					NeuronConnectionContainer * newNeuronConnectionContainerL2 = new NeuronConnectionContainer();
					currentNeuronL2->currentBackNeuronConnectionContainer->neuron = currentNeuronL1->neuron;
					currentNeuronL2->currentBackNeuronConnectionContainer->neuronConnection = newNeuronConnection;
					currentNeuronL2->currentBackNeuronConnectionContainer->nextNeuronConnectionContainer = newNeuronConnectionContainerL2;
					currentNeuronL2->currentBackNeuronConnectionContainer = currentNeuronL2->currentBackNeuronConnectionContainer->nextNeuronConnectionContainer;

				}

				currentNeuronL2 = currentNeuronL2->nextNeuronContainer;
			}

			currentNeuronL1 = currentNeuronL1->nextNeuronContainer;
		}

		firstNeuronInPreviousLayerX=firstNeuronInPreviousLayerX->firstNeuronInFrontLayer;

	}
}

double calculateDistanceBetween2Points(double xPositionOfUnit1, double xPositionOfUnit2, double yPositionOfUnit1, double yPositionOfUnit2)
{
	double xDistanceBetweenTheTwoUnits = fabs(xPositionOfUnit1 -  xPositionOfUnit2);
	double yDistanceBetweenTheTwoUnits = fabs(yPositionOfUnit1 -  yPositionOfUnit2);
	double distanceBetweenTheTwoUnits = sqrt(pow(xDistanceBetweenTheTwoUnits, 2) + pow(yDistanceBetweenTheTwoUnits, 2));

	return distanceBetweenTheTwoUnits;
}



void linkNewFrontLayerWithPreviousLayers2D(NeuronContainer * firstNeuronInCurrentLayer, NeuronContainer * firstNeuronInNewFrontLayer, double probabilityNeuronConnectionWithPreviousLayerNeuron, NeuronContainer * firstInputNeuronInSubnet, double probabilityNeuronConnectionWithAllPreviousLayersNeurons, long numberOfInputNeurons, long numberOfOutputNeurons, long currentNumberOfLayers, long numberOfLayers, long numberOfNeuronsInCurrentLayer, long numberOfNeuronsInNewFrontLayer, int layerDivergenceType)
{
	//Modify associations/links based upon layers of intermediate/hidden neurons
	//if probabilityNeuronConnectionWithPreviousLayerNeuron is 1.0, link ALL current/back and front layer neurons neurons (fill in firstFrontNeuronConnectionContainer/BackNeuronReferences list for all relevant neurons)

	NeuronContainer * currentNeuronL1;
	NeuronContainer * currentNeuronL2;

	currentNeuronL1 = firstNeuronInCurrentLayer;
	currentNeuronL2 = firstNeuronInNewFrontLayer;
	currentNeuronL1->currentFrontNeuronConnectionContainer = currentNeuronL1->firstFrontNeuronConnectionContainer;
	currentNeuronL2->currentBackNeuronConnectionContainer = currentNeuronL2->firstBackNeuronConnectionContainer;

	while(currentNeuronL1->nextNeuronContainer != NULL)
	{
		currentNeuronL2 = firstNeuronInNewFrontLayer;

		//long numConnectionsWithFrontLayerNeurons = 0;	//No longer required/used...

		long xPosRelL1;
		long yPosRelL1;
		long xPosRelL2;
		long yPosRelL2;

		#ifdef LAYER_DIVERGENCE_TYPE_LINEAR_DIVERGING_SQUARE2D_USE_CENTRED
		int numberOfNeuronsInCurrentLayerRow = sqrt(double(numberOfNeuronsInCurrentLayer));
		int numberOfNeuronsInNewFrontLayerRow = sqrt(double(numberOfNeuronsInNewFrontLayer));
		int newFrontLayerXOffset = firstNeuronInCurrentLayer->neuron->xPosRel + (numberOfNeuronsInCurrentLayerRow-numberOfNeuronsInNewFrontLayerRow)/2;
		int newFrontLayerYOffset = firstNeuronInCurrentLayer->neuron->yPosRel + (numberOfNeuronsInCurrentLayerRow-numberOfNeuronsInNewFrontLayerRow)/2;
		#ifdef ANN_DEBUG
		//cout  << "newFrontLayerXOffset = " << newFrontLayerXOffset << endl;
		//cout  << "newFrontLayerYOffset = " << newFrontLayerYOffset << endl;
		#endif
		#else
		int newFrontLayerXOffset = 0;
		int newFrontLayerYOffset = 0;
		#endif

		if((layerDivergenceType == LAYER_DIVERGENCE_TYPE_LINEAR_DIVERGING_SQUARE2D) || (layerDivergenceType == LAYER_DIVERGENCE_TYPE_LINEAR_DIVERGING_SQUARE2D_RADIALBIAS))
		{
			xPosRelL1 = currentNeuronL1->neuron->xPosRel;
			yPosRelL1 = currentNeuronL1->neuron->yPosRel;

			#ifdef LAYER_DIVERGENCE_TYPE_LINEAR_DIVERGING_SQUARE2D_USE_CENTRED
			xPosRelL2 = 0 + newFrontLayerXOffset;
			yPosRelL2 = 0 + newFrontLayerYOffset;
			#else
			xPosRelL2 = 0;
			yPosRelL2 = 0;
			#endif
		}

		while(currentNeuronL2->nextNeuronContainer != NULL)
		{
			bool spatialCondition2D = false;
			if((layerDivergenceType == LAYER_DIVERGENCE_TYPE_LINEAR_DIVERGING_SQUARE2D) || (layerDivergenceType == LAYER_DIVERGENCE_TYPE_LINEAR_DIVERGING_SQUARE2D_RADIALBIAS))
			{
				currentNeuronL2->neuron->spatialCoordinatesSet = true;
				currentNeuronL2->neuron->xPosRel = xPosRelL2;
				currentNeuronL2->neuron->yPosRel = yPosRelL2;

				double xPosRelL1CorrespondingToL2 = xPosRelL2 * (sqrt((double)numberOfNeuronsInCurrentLayer)/sqrt((double)numberOfNeuronsInNewFrontLayer));
				double yPosRelL1CorrespondingToL2 = yPosRelL2 * (sqrt((double)numberOfNeuronsInCurrentLayer)/sqrt((double)numberOfNeuronsInNewFrontLayer));
				double maxDistanceBetweenTwoPointsIn2DMapL1 = sqrt(pow(sqrt(double(numberOfNeuronsInCurrentLayer)), 2) + pow(sqrt(double(numberOfNeuronsInCurrentLayer)), 2));

				if(layerDivergenceType == LAYER_DIVERGENCE_TYPE_LINEAR_DIVERGING_SQUARE2D)
				{
					#ifdef LAYER_DIVERGENCE_TYPE_LINEAR_DIVERGING_SQUARE2D_USE_CENTRED
					double distanceBetweenPosRelL1AndPosRelL1CorrespondingToL2 = calculateDistanceBetween2Points(xPosRelL1, xPosRelL2, yPosRelL1, yPosRelL2);
					#else
					double distanceBetweenPosRelL1AndPosRelL1CorrespondingToL2 = calculateDistanceBetween2Points(xPosRelL1, xPosRelL1CorrespondingToL2, yPosRelL1, yPosRelL1CorrespondingToL2);
					#endif

					if(distanceBetweenPosRelL1AndPosRelL1CorrespondingToL2 < (((double)currentNumberOfLayers/(double)numberOfLayers)*maxDistanceBetweenTwoPointsIn2DMapL1))
					{
						spatialCondition2D = true;
					}

					#ifdef ANN_DEBUG
					/*
					cout << " " << endl;
					cout << "\t xPosRelL1CorrespondingToL2 = " << xPosRelL1CorrespondingToL2 << endl;
					cout << "\t yPosRelL1CorrespondingToL2 = " << yPosRelL1CorrespondingToL2 << endl;
					cout << "\t distanceBetweenPosRelL1AndPosRelL1CorrespondingToL2 = " << distanceBetweenPosRelL1AndPosRelL1CorrespondingToL2 << endl;
					cout << "\t maxDistanceBetweenTwoPointsIn2DMapL1 = " << maxDistanceBetweenTwoPointsIn2DMapL1 << endl;
					cout << "\t\t xPosRelL2 = " << xPosRelL2 << endl;
					cout << "\t\t yPosRelL2 = " << yPosRelL2 << endl;
					cout << "\t\t xPosRelL1 = " << xPosRelL1 << endl;
					cout << "\t\t yPosRelL1 = " << yPosRelL1 << endl;
					cout << "\t\t numberOfNeuronsInCurrentLayer = " << numberOfNeuronsInCurrentLayer << endl;
					cout << "\t\t numberOfNeuronsInNewFrontLayer = " << numberOfNeuronsInNewFrontLayer << endl;
					cout << "\t\t currentNumberOfLayers = " << currentNumberOfLayers << endl;
					cout << "\t\t numberOfLayers = " << numberOfLayers << endl;
					*/
					#endif
				}
				else if(layerDivergenceType == LAYER_DIVERGENCE_TYPE_LINEAR_DIVERGING_SQUARE2D_RADIALBIAS)
				{

				#ifdef IF_LAYER_DIVERGENCE_TYPE_LINEAR_DIVERGING_SQUARE2D_RADIALBIAS_USE_NEARBY_POINTS_INSTEAD
					#ifdef LAYER_DIVERGENCE_TYPE_LINEAR_DIVERGING_SQUARE2D_USE_CENTRED
					long distanceBetweenNeuronL1AndL1CorrespondingToL2 = calculateDistanceBetween2Points(xPosRelL1, xPosRelL2, yPosRelL1, yPosRelL2);
					#else
					long distanceBetweenNeuronL1AndL1CorrespondingToL2 = calculateDistanceBetween2Points(xPosRelL1, xPosRelL1CorrespondingToL2, yPosRelL1, yPosRelL1CorrespondingToL2);
					#endif
					#ifdef ANN_DEBUG
					//cout << "distanceBetweenNeuronL1AndL1CorrespondingToL2 = " << distanceBetweenNeuronL1AndL1CorrespondingToL2 << endl;
					#endif
					
					//if(abs(distanceBetweenNeuronL1AndL1CorrespondingToL2*IF_LAYER_DIVERGENCE_TYPE_LINEAR_DIVERGING_SQUARE2D_RADIALBIAS_BIAS) < (((double)currentNumberOfLayers/(double)numberOfLayers)*maxDistanceBetweenTwoPointsIn2DMapL1))
					if(distanceBetweenNeuronL1AndL1CorrespondingToL2 < IF_LAYER_DIVERGENCE_TYPE_LINEAR_DIVERGING_SQUARE2D_RADIALBIAS_BIAS)
					{
						spatialCondition2D = true;
					}

				#else
					long distanceBetweenNeuronL1AndCentreOfMap = calculateDistanceBetween2Points(xPosRelL1, (sqrt(double(numberOfNeuronsInCurrentLayer))/2.0), yPosRelL1, (sqrt(double(numberOfNeuronsInCurrentLayer))/2.0));
					#ifdef LAYER_DIVERGENCE_TYPE_LINEAR_DIVERGING_SQUARE2D_USE_CENTRED
					long distanceBetweenNeuronL1CorrespondingToL2AndCentreOfMap = calculateDistanceBetween2Points(xPosRelL2, (sqrt(double(numberOfNeuronsInCurrentLayer))/2.0), yPosRelL2, (sqrt(double(numberOfNeuronsInCurrentLayer))/2.0));
					#else
					long distanceBetweenNeuronL1CorrespondingToL2AndCentreOfMap = calculateDistanceBetween2Points(xPosRelL1CorrespondingToL2, (sqrt(double(numberOfNeuronsInCurrentLayer))/2.0), yPosRelL1CorrespondingToL2, (sqrt(double(numberOfNeuronsInCurrentLayer))/2.0));
					#endif

					if(abs((distanceBetweenNeuronL1AndCentreOfMap-distanceBetweenNeuronL1CorrespondingToL2AndCentreOfMap)*IF_LAYER_DIVERGENCE_TYPE_LINEAR_DIVERGING_SQUARE2D_RADIALBIAS_BIAS) < (((double)currentNumberOfLayers/(double)numberOfLayers)*maxDistanceBetweenTwoPointsIn2DMapL1))
					{
						spatialCondition2D = true;
					}
				#endif
				}
			}
			else
			{
				spatialCondition2D = true;
			}

			if(spatialCondition2D)
			{
				double randomNumberBetween0And1 = ((double(abs((short)rand())))/(ABS_OF_SHORT));
				#ifdef ANN_DEBUG
				//cout << "randomNumberBetween0And1 = " << randomNumberBetween0And1 << endl;
				#endif
				
				if(randomNumberBetween0And1 < probabilityNeuronConnectionWithPreviousLayerNeuron)
				{
					NeuronConnection * newNeuronConnection = new NeuronConnection();

					NeuronConnectionContainer * newNeuronConnectionContainerL1 = new NeuronConnectionContainer();
					currentNeuronL1->currentFrontNeuronConnectionContainer->neuron = currentNeuronL2->neuron;
					currentNeuronL1->currentFrontNeuronConnectionContainer->neuronConnection = newNeuronConnection;
					currentNeuronL1->currentFrontNeuronConnectionContainer->nextNeuronConnectionContainer = newNeuronConnectionContainerL1;
					currentNeuronL1->currentFrontNeuronConnectionContainer = currentNeuronL1->currentFrontNeuronConnectionContainer->nextNeuronConnectionContainer;

					NeuronConnectionContainer * newNeuronConnectionContainerL2 = new NeuronConnectionContainer();
					currentNeuronL2->currentBackNeuronConnectionContainer->neuron = currentNeuronL1->neuron;
					currentNeuronL2->currentBackNeuronConnectionContainer->neuronConnection = newNeuronConnection;
					currentNeuronL2->currentBackNeuronConnectionContainer->nextNeuronConnectionContainer = newNeuronConnectionContainerL2;
					currentNeuronL2->currentBackNeuronConnectionContainer = currentNeuronL2->currentBackNeuronConnectionContainer->nextNeuronConnectionContainer;
				}
			}

			if((layerDivergenceType == LAYER_DIVERGENCE_TYPE_LINEAR_DIVERGING_SQUARE2D) || (layerDivergenceType == LAYER_DIVERGENCE_TYPE_LINEAR_DIVERGING_SQUARE2D_RADIALBIAS))
			{
				xPosRelL2++;
				if(xPosRelL2 >= (sqrt(double(numberOfNeuronsInNewFrontLayer))+newFrontLayerXOffset))
				{
					#ifdef LAYER_DIVERGENCE_TYPE_LINEAR_DIVERGING_SQUARE2D_USE_CENTRED
					xPosRelL2 = 0 + newFrontLayerXOffset;
					#else
					xPosRelL2 = 0;
					#endif
					yPosRelL2++;
				}
			}

			currentNeuronL2 = currentNeuronL2->nextNeuronContainer;
		}
		
		currentNeuronL1 = currentNeuronL1->nextNeuronContainer;
	}


	NeuronContainer * firstNeuronInPreviousLayerX = firstInputNeuronInSubnet;

	long tempCurrentEffectiveNeuronsOnLayer = numberOfInputNeurons;
	long tempCurrentNumNeuronsOnLayer = numberOfInputNeurons;
	for(long tempLayerIndex = 0; tempLayerIndex < currentNumberOfLayers; tempLayerIndex++)
	{
		//count num neurons in layer L1;
		NeuronContainer * currentNeuronL1 = firstNeuronInPreviousLayerX;
		long numberOfNeuronsInL1 = 0;
		while(currentNeuronL1->nextNeuronContainer != NULL)
		{
			numberOfNeuronsInL1++;
			currentNeuronL1 = currentNeuronL1->nextNeuronContainer;
		}

		currentNeuronL1 = firstNeuronInPreviousLayerX;
		NeuronContainer * currentNeuronL2 = firstNeuronInNewFrontLayer;
		while(currentNeuronL1->nextNeuronContainer != NULL)
		{
			currentNeuronL2 = firstNeuronInNewFrontLayer;

			while(currentNeuronL2->nextNeuronContainer != NULL)
			{
				bool spatialCondition2D = false;
				if((layerDivergenceType == LAYER_DIVERGENCE_TYPE_LINEAR_DIVERGING_SQUARE2D) || (layerDivergenceType == LAYER_DIVERGENCE_TYPE_LINEAR_DIVERGING_SQUARE2D_RADIALBIAS))
				{

					long xPosRelL1 = currentNeuronL1->neuron->xPosRel;
					long yPosRelL1 = currentNeuronL1->neuron->yPosRel;
					long xPosRelL2 = currentNeuronL2->neuron->xPosRel;
					long yPosRelL2 = currentNeuronL2->neuron->yPosRel;
					double xPosRelL1CorrespondingToL2 = xPosRelL2 * (sqrt(double(numberOfNeuronsInL1))/sqrt(double(numberOfNeuronsInNewFrontLayer)));
					double yPosRelL1CorrespondingToL2 = yPosRelL2 * (sqrt(double(numberOfNeuronsInL1))/sqrt(double(numberOfNeuronsInNewFrontLayer)));
					double maxDistanceBetweenTwoPointsIn2DMapL1 = sqrt(pow(sqrt(double(numberOfNeuronsInL1)), 2) + pow(sqrt(double(numberOfNeuronsInL1)), 2));

					if(layerDivergenceType == LAYER_DIVERGENCE_TYPE_LINEAR_DIVERGING_SQUARE2D)
					{
						#ifdef LAYER_DIVERGENCE_TYPE_LINEAR_DIVERGING_SQUARE2D_USE_CENTRED
						double distanceBetweenPosRelL1AndPosRelL1CorrespondingToL2 = calculateDistanceBetween2Points(xPosRelL1, xPosRelL2, yPosRelL1, yPosRelL2);
						#else
						double distanceBetweenPosRelL1AndPosRelL1CorrespondingToL2 = calculateDistanceBetween2Points(xPosRelL1, xPosRelL1CorrespondingToL2, yPosRelL1, yPosRelL1CorrespondingToL2);
						#endif

						#ifdef ANN_DEBUG
						/*
						cout << " " << endl;
						cout << "\t xPosRelL1CorrespondingToL2 = " << xPosRelL1CorrespondingToL2 << endl;
						cout << "\t yPosRelL1CorrespondingToL2 = " << yPosRelL1CorrespondingToL2 << endl;
						cout << "\t distanceBetweenPosRelL1AndPosRelL1CorrespondingToL2 = " << distanceBetweenPosRelL1AndPosRelL1CorrespondingToL2 << endl;
						cout << "\t maxDistanceBetweenTwoPointsIn2DMapL1 = " << maxDistanceBetweenTwoPointsIn2DMapL1 << endl;
						cout << "\t\t xPosRelL2 = " << xPosRelL2 << endl;
						cout << "\t\t yPosRelL2 = " << yPosRelL2 << endl;
						cout << "\t\t xPosRelL1 = " << xPosRelL1 << endl;
						cout << "\t\t yPosRelL1 = " << yPosRelL1 << endl;
						cout << "\t\t numberOfNeuronsInCurrentLayer = " << numberOfNeuronsInCurrentLayer << endl;
						cout << "\t\t numberOfNeuronsInNewFrontLayer = " << numberOfNeuronsInNewFrontLayer << endl;
						cout << "\t\t currentNumberOfLayers = " << currentNumberOfLayers << endl;
						cout << "\t\t numberOfLayers = " << numberOfLayers << endl;
						cout << "\t\t tempLayerIndex = " << tempLayerIndex << endl;
						cout << "\t\t numberOfNeuronsInL1 = " << numberOfNeuronsInL1 << endl;
						*/
						#endif

						if(distanceBetweenPosRelL1AndPosRelL1CorrespondingToL2 < (((double)currentNumberOfLayers/(double)numberOfLayers)*maxDistanceBetweenTwoPointsIn2DMapL1))			//(tempLayerIndex/currentNumberOfLayers)
						{
							spatialCondition2D = true;
						}
					}
					else if(layerDivergenceType == LAYER_DIVERGENCE_TYPE_LINEAR_DIVERGING_SQUARE2D_RADIALBIAS)
					{
					#ifdef IF_LAYER_DIVERGENCE_TYPE_LINEAR_DIVERGING_SQUARE2D_RADIALBIAS_USE_NEARBY_POINTS_INSTEAD
						#ifdef LAYER_DIVERGENCE_TYPE_LINEAR_DIVERGING_SQUARE2D_USE_CENTRED
						long distanceBetweenNeuronL1AndL1CorrespondingToL2 = calculateDistanceBetween2Points(xPosRelL1, xPosRelL2, yPosRelL1, yPosRelL2);
						#else
						long distanceBetweenNeuronL1AndL1CorrespondingToL2 = calculateDistanceBetween2Points(xPosRelL1, xPosRelL1CorrespondingToL2, yPosRelL1, yPosRelL1CorrespondingToL2);
						#endif

						//if(abs(distanceBetweenNeuronL1AndL1CorrespondingToL2*IF_LAYER_DIVERGENCE_TYPE_LINEAR_DIVERGING_SQUARE2D_RADIALBIAS_BIAS) < (((double)currentNumberOfLayers/(double)numberOfLayers)*maxDistanceBetweenTwoPointsIn2DMapL1))
						if(distanceBetweenNeuronL1AndL1CorrespondingToL2 < IF_LAYER_DIVERGENCE_TYPE_LINEAR_DIVERGING_SQUARE2D_RADIALBIAS_BIAS)
						{
							spatialCondition2D = true;
						}

					#else
						long distanceBetweenNeuronL1AndCentreOfMap = calculateDistanceBetween2Points(xPosRelL1, (sqrt(double(numberOfNeuronsInCurrentLayer))/2.0), yPosRelL1, (sqrt(double(numberOfNeuronsInCurrentLayer))/2.0));
						#ifdef LAYER_DIVERGENCE_TYPE_LINEAR_DIVERGING_SQUARE2D_USE_CENTRED
						long distanceBetweenNeuronL1CorrespondingToL2AndCentreOfMap = calculateDistanceBetween2Points(xPosRelL2, (sqrt(double(numberOfNeuronsInCurrentLayer))/2.0), yPosRelL2, (sqrt(double(numberOfNeuronsInCurrentLayer))/2.0));
						#else
						long distanceBetweenNeuronL1CorrespondingToL2AndCentreOfMap = calculateDistanceBetween2Points(xPosRelL1CorrespondingToL2, (sqrt(double(numberOfNeuronsInCurrentLayer))/2.0), yPosRelL1CorrespondingToL2, (sqrt(double(numberOfNeuronsInCurrentLayer))/2.0));
						#endif

						if(abs((distanceBetweenNeuronL1AndCentreOfMap-distanceBetweenNeuronL1CorrespondingToL2AndCentreOfMap)*IF_LAYER_DIVERGENCE_TYPE_LINEAR_DIVERGING_SQUARE2D_RADIALBIAS_BIAS) < (((double)currentNumberOfLayers/(double)numberOfLayers)*maxDistanceBetweenTwoPointsIn2DMapL1))
						{
							spatialCondition2D = true;
						}
					#endif
					}
				}
				else
				{
					spatialCondition2D = true;
				}

				if(spatialCondition2D)
				{
					double randomNumberBetween0And1 = ((double(abs((short)rand())))/(ABS_OF_SHORT));
					#ifdef ANN_DEBUG
					//cout << "randomNumberBetween0And1 = " << randomNumberBetween0And1 << endl;
					#endif
					
					if(randomNumberBetween0And1 < probabilityNeuronConnectionWithAllPreviousLayersNeurons)
					{//add connections to previous layers

						NeuronConnection * newNeuronConnection = new NeuronConnection();

						NeuronConnectionContainer * newNeuronConnectionContainerL1 = new NeuronConnectionContainer();
						currentNeuronL1->currentFrontNeuronConnectionContainer->neuron = currentNeuronL2->neuron;
						currentNeuronL1->currentFrontNeuronConnectionContainer->neuronConnection = newNeuronConnection;
						currentNeuronL1->currentFrontNeuronConnectionContainer->nextNeuronConnectionContainer = newNeuronConnectionContainerL1;
						currentNeuronL1->currentFrontNeuronConnectionContainer = currentNeuronL1->currentFrontNeuronConnectionContainer->nextNeuronConnectionContainer;

						NeuronConnectionContainer * newNeuronConnectionContainerL2 = new NeuronConnectionContainer();
						currentNeuronL2->currentBackNeuronConnectionContainer->neuron = currentNeuronL1->neuron;
						currentNeuronL2->currentBackNeuronConnectionContainer->neuronConnection = newNeuronConnection;
						currentNeuronL2->currentBackNeuronConnectionContainer->nextNeuronConnectionContainer = newNeuronConnectionContainerL2;
						currentNeuronL2->currentBackNeuronConnectionContainer = currentNeuronL2->currentBackNeuronConnectionContainer->nextNeuronConnectionContainer;

					}
				}

				currentNeuronL2 = currentNeuronL2->nextNeuronContainer;
			}

			currentNeuronL1 = currentNeuronL1->nextNeuronContainer;
		}

		firstNeuronInPreviousLayerX=firstNeuronInPreviousLayerX->firstNeuronInFrontLayer;
	}
}




















NeuronContainer * fillNonDistinctHiddenLayer(NeuronContainer * firstNeuronInCurrentLayer, long numberOfInputNeurons, long numberOfOutputNeurons, long numberOfNeuronsInCurrentLayer, long currentNumberOfLayers, long numberOfLayers, double probabilityOfSubnetCreation, long maxNumberOfRecursiveSubnets, long currentNumberOfRecursiveSubnets, bool useSubnetDependentNumberOfLayers, double subnetNumberOfLayersModifier, int layerDivergenceType, double meanLayerDivergenceFactor, double probabilityNeuronConnectionWithPreviousLayerNeuron, NeuronContainer * firstInputNeuronInSubnet, double probabilityNeuronConnectionWithAllPreviousLayersNeurons)
{
	#ifdef ANN_DEBUG
	cout << "\nNeuronContainer * fillNonDistinctHiddenLayer()\n" << endl;
	#endif

	subnetIDcounter = currentNumberOfRecursiveSubnets;

	NeuronContainer * firstOutputNeuronInNetwork;

	//Create neurons for the new front layer

	long numberOfNeuronsInNewFrontLayer;
	numberOfNeuronsInNewFrontLayer = calculateNumberOfNeuronsInNewFrontLayer(numberOfInputNeurons, numberOfOutputNeurons, numberOfNeuronsInCurrentLayer, currentNumberOfLayers, numberOfLayers, meanLayerDivergenceFactor, layerDivergenceType);
	#ifdef ANN_DEBUG
	cout << "numberOfNeuronsInNewFrontLayer = " << numberOfNeuronsInNewFrontLayer << endl;
	#endif

	NeuronContainer * firstNeuronInNewFrontLayer = new NeuronContainer();

	createNewFrontLayer(firstNeuronInCurrentLayer, firstNeuronInNewFrontLayer, numberOfNeuronsInNewFrontLayer, currentNumberOfLayers, numberOfLayers);

	if((layerDivergenceType == LAYER_DIVERGENCE_TYPE_LINEAR_DIVERGING_SQUARE2D) || (layerDivergenceType == LAYER_DIVERGENCE_TYPE_LINEAR_DIVERGING_SQUARE2D_RADIALBIAS) || (layerDivergenceType == LAYER_DIVERGENCE_TYPE_LINEAR_CONVERGING_SQUARE2D))
	{
		linkNewFrontLayerWithPreviousLayers2D(firstNeuronInCurrentLayer, firstNeuronInNewFrontLayer, probabilityNeuronConnectionWithPreviousLayerNeuron, firstInputNeuronInSubnet, probabilityNeuronConnectionWithAllPreviousLayersNeurons, numberOfInputNeurons, numberOfOutputNeurons, currentNumberOfLayers, numberOfLayers, numberOfNeuronsInCurrentLayer, numberOfNeuronsInNewFrontLayer, layerDivergenceType);
	}
	else
	{
		linkNewFrontLayerWithPreviousLayers(firstNeuronInCurrentLayer, firstNeuronInNewFrontLayer, probabilityNeuronConnectionWithPreviousLayerNeuron, firstInputNeuronInSubnet, probabilityNeuronConnectionWithAllPreviousLayersNeurons, numberOfInputNeurons, numberOfOutputNeurons, currentNumberOfLayers);
	}


#ifdef ANN_ADVANCED

	NeuronContainer * currentNeuronL1 = firstNeuronInCurrentLayer;
	while(currentNeuronL1->nextNeuronContainer != NULL)
	{
		//create a subnet if necessary
		//NeuronContainer * currentNeuronL1 = currentNeuronL1;		//define the neuron which will become a subnet


		if(currentNeuronL1->hasBackLayer)	//can only create a subnet if the neuron candidate has a back layer
		{
			#ifdef ANN_DEBUG
			//cout << "\ncurrentNumberOfRecursiveSubnets = " << currentNumberOfRecursiveSubnets << endl;
			//cout << "maxNumberOfRecursiveSubnets = " << maxNumberOfRecursiveSubnets << endl;
			#endif
			
			if(currentNumberOfRecursiveSubnets < maxNumberOfRecursiveSubnets)
			{
			#ifdef DEBUG_TRAIN_NETWORK_WITH_NON_RANDOM_VARS
				double randomNumberBetween0And1 = 0.3;
			#else
				double randomNumberBetween0And1 = ((double(abs((short)rand())))/(ABS_OF_SHORT));
			#endif
				#ifdef ANN_DEBUG
				//cout << "randomNumberBetween0And1 = " << randomNumberBetween0And1 << endl;
				#endif
				
				if(randomNumberBetween0And1 < probabilityOfSubnetCreation)
				{
					#ifdef ANN_DEBUG
					//cout << "creating subnet..." << endl;
					#endif
					
					#ifdef PROPER_LARGE_NUMBER_OF_NN_INPUTS
					if(abs(currentNeuronL1->numBackNeuronConnections - currentNeuronL1->numFrontNeuronConnections) >= 2)
					{//only create a subnet if the difference in the number of neurons between the front and the back layers is at least 2
					#endif

						NeuronContainer * tempNeuron;

						long numNeuronsInBackLayerOfSubnet = 0;
						long numNeuronsInFrontLayerOfSubnet = 0;
						tempNeuron = currentNeuronL1->firstNeuronInFrontLayer;
						while(tempNeuron->nextNeuronContainer != NULL)
						{
							numNeuronsInFrontLayerOfSubnet++;
							tempNeuron = tempNeuron->nextNeuronContainer;
						}
						tempNeuron = currentNeuronL1->firstNeuronInBackLayer;
						while(tempNeuron->nextNeuronContainer != NULL)
						{
							numNeuronsInBackLayerOfSubnet++;
							tempNeuron = tempNeuron->nextNeuronContainer;
						}

						orderIDcounter=1;
						layerIDcounter=1;
						subnetIDcounter = currentNumberOfRecursiveSubnets+1;
						NeuronContainer * firstNeuronContainerInBackLayerOfSubnet = new NeuronContainer();
						NeuronContainer * currentNeuronInBackLayerOfSubnet = firstNeuronContainerInBackLayerOfSubnet;
						//currentNeuronL1->numNeuronsInBackLayerOfSubnet = currentNeuronL1->numBackNeuronConnections;
						//currentNeuronL1->numNeuronsInFrontLayerOfSubnet =  currentNeuronL1->numFrontNeuronConnections;

						tempNeuron = currentNeuronL1->firstNeuronInBackLayer;
						while(tempNeuron->nextNeuronContainer != NULL)
						{
							fillInNeuronIDProperties(currentNeuronInBackLayerOfSubnet->neuron, IDCounter, orderIDcounter, layerIDcounter, subnetIDcounter);

							#ifdef ANN_DEBUG
							//cout << "layerIDcounter=" << layerIDcounter << endl;
							#endif
							currentNeuronInBackLayerOfSubnet->hasBackLayer = false;	//CHECK ANNTHIS - need to update ANNupdateAlgorithm.cpp accordingly
							currentNeuronInBackLayerOfSubnet->hasFrontLayer = true;

							NeuronContainer * newNeuron = new NeuronContainer();
							currentNeuronInBackLayerOfSubnet->nextNeuronContainer = newNeuron;
							currentNeuronInBackLayerOfSubnet = currentNeuronInBackLayerOfSubnet->nextNeuronContainer;

							IDCounter++;
							orderIDcounter++;

							tempNeuron = tempNeuron->nextNeuronContainer;
						}

						currentNeuronL1->isSubnet = true;
						currentNeuronL1->firstNeuronContainerInBackLayerOfSubnet = firstNeuronContainerInBackLayerOfSubnet;

						NeuronContainer * firstNeuronContainerInFrontLayerOfSubnet;
						long numberOfHiddenLayersInSubnet = calcNumberOfLayersInSubnet(numNeuronsInBackLayerOfSubnet, numNeuronsInFrontLayerOfSubnet, currentNumberOfRecursiveSubnets, maxNumberOfRecursiveSubnets, useSubnetDependentNumberOfLayers, subnetNumberOfLayersModifier);
						#define NONLINEARDIVERGENCEFACTOR (1)

						#ifdef ANN_DEBUG
						cout << "\nAbout to perform ANN recursion in fillNonDistinctHiddenLayer ... " << endl;
						cout << "currentNeuronL1 NeuronContainer ID = " << currentNeuronL1->neuron->id << endl;
						cout << "currentNeuronL1 NeuronContainer ID Order = " << currentNeuronL1->neuron->orderID << endl;
						cout << "currentNeuronL1 NeuronContainer ID Layer = " << currentNeuronL1->neuron->layerID << endl;
						cout << "currentNeuronL1 NeuronContainer ID Subnet = " << currentNeuronL1->neuron->subnetID << endl;
						cout << "firstInputNeuronInNetwork = firstNeuronContainerInBackLayerOfSubnet = ... " << endl;
						//cout << "numberOfInputNeurons = " << currentNeuronL1->neuron->numNeuronsInBackLayerOfSubnet << endl;
						//cout << "numberOfOutputNeurons = " <<  currentNeuronL1->neuron->numNeuronsInFrontLayerOfSubnet << endl;
						//cout << "numberOfNeuronsInCurrentLayer = " << currentNeuronL1->neuron->numNeuronsInBackLayerOfSubnet << endl;
						cout << "currentNumberOfLayers = " << 1 << endl;
						cout << "numberOfHiddenLayersInSubnet = " << numberOfHiddenLayersInSubnet << endl;
						cout << "probabilityOfSubnetCreation*NONLINEARDIVERGENCEFACTOR = " << probabilityOfSubnetCreation*NONLINEARDIVERGENCEFACTOR << endl;
						cout << "maxNumberOfRecursiveSubnets = " << maxNumberOfRecursiveSubnets << endl;
						cout << "currentNumberOfRecursiveSubnets+1 = " << currentNumberOfRecursiveSubnets+1 << endl;
						#endif

						firstNeuronContainerInFrontLayerOfSubnet = fillNonDistinctHiddenLayer(firstNeuronContainerInBackLayerOfSubnet, numNeuronsInBackLayerOfSubnet, numNeuronsInFrontLayerOfSubnet, numNeuronsInBackLayerOfSubnet, 1, numberOfHiddenLayersInSubnet, probabilityOfSubnetCreation*NONLINEARDIVERGENCEFACTOR, maxNumberOfRecursiveSubnets, currentNumberOfRecursiveSubnets+1, useSubnetDependentNumberOfLayers, subnetNumberOfLayersModifier, layerDivergenceType, meanLayerDivergenceFactor, probabilityNeuronConnectionWithPreviousLayerNeuron, firstNeuronContainerInBackLayerOfSubnet, probabilityNeuronConnectionWithAllPreviousLayersNeurons);

						currentNeuronL1->firstNeuronContainerInFrontLayerOfSubnet = firstNeuronContainerInFrontLayerOfSubnet;
							//now replace output layer of subnet with frontLayerOfSubnet [this is not necessary as 1-1 mapping will be assumed here during the neural network training / update algorithm]
							//[1-1] map forward layer neurons of currentNeuronL1 with output neurons of subnet	[CHECK ANNTHIS - nb this 1-1 mapping must be maintained in the neuronNetworkUpdateAlgorithm, not just in the ANNformation]
											//reset layer/subnet IDs after ANN recursion;	//this is not currently required as layerIDcounter/subnetIDcounter are not used again in the current execution of this function
						layerIDcounter = currentNumberOfLayers+1;			//reset layer to new front layer
						subnetIDcounter = currentNumberOfRecursiveSubnets;

					#ifdef PROPER_LARGE_NUMBER_OF_NN_INPUTS
					}
					#endif
				}
			}
		}
		currentNeuronL1 = currentNeuronL1->nextNeuronContainer;
	}
#endif

	if(currentNumberOfLayers == (numberOfLayers-1))
	{//CHECK THIS - nb this needs to imply that numberOfOutputNeurons = number of neurons in current layer
		if(numberOfOutputNeurons != numberOfNeuronsInNewFrontLayer)
		{
			cout << "Error: error with NN heuristic; (currentNumberOfLayers == (numberOfLayers-1)) && (numberOfNeuronsInNewFrontLayer != numberOfOutputNeurons)" << endl;
			exit(0);
		}

		//reached final output layer
		firstOutputNeuronInNetwork = firstNeuronInNewFrontLayer;
	}
	else
	{
		//now recurse
		firstOutputNeuronInNetwork = fillNonDistinctHiddenLayer(firstNeuronInNewFrontLayer, numberOfInputNeurons, numberOfOutputNeurons, numberOfNeuronsInNewFrontLayer, (currentNumberOfLayers+1), numberOfLayers, probabilityOfSubnetCreation, maxNumberOfRecursiveSubnets, currentNumberOfRecursiveSubnets, useSubnetDependentNumberOfLayers, subnetNumberOfLayersModifier, layerDivergenceType, meanLayerDivergenceFactor, probabilityNeuronConnectionWithPreviousLayerNeuron, firstInputNeuronInSubnet, probabilityNeuronConnectionWithAllPreviousLayersNeurons);
	}

	return firstOutputNeuronInNetwork;
}


/************************************************************ End Form Advanced Neural Network Routines ******************************************************/




















long calcNumberOfLayersInSubnet(long numberOfInputNeurons, long numberOfOutputNeurons, long currentNumberOfRecursiveSubnets, long maxNumberOfRecursiveSubnets, bool useSubnetDependentNumberOfLayers, double subnetNumberOfLayersModifier)
{
	long numberOfLayersInSubnet;

#ifdef USE_OLD_NUM_HIDDEN_LAYERS_HEURISTIC
	if(useSubnetDependentNumberOfLayers == false)
	{
		if(numberOfInputNeurons > numberOfOutputNeurons)
		{
			numberOfLayersInSubnet = (long)((double)(numberOfInputNeurons - numberOfOutputNeurons) * subnetNumberOfLayersModifier) + 1;
		}
		else
		{
			numberOfLayersInSubnet = (long)((double)(numberOfOutputNeurons - numberOfInputNeurons) * subnetNumberOfLayersModifier) + 1;
		}
	}
	else
	{
		if(numberOfInputNeurons > numberOfOutputNeurons)
		{
			numberOfLayersInSubnet = (long)((double)(numberOfInputNeurons - numberOfOutputNeurons) * subnetNumberOfLayersModifier * ((double)currentNumberOfRecursiveSubnets/(double)maxNumberOfRecursiveSubnets)) + 1;
		}
		else
		{
			numberOfLayersInSubnet = (long)((double)(numberOfOutputNeurons - numberOfInputNeurons) * subnetNumberOfLayersModifier * ((double)currentNumberOfRecursiveSubnets/(double)maxNumberOfRecursiveSubnets)) + 1;
		}
	}
#else

	if(useSubnetDependentNumberOfLayers == true)
	{
		numberOfLayersInSubnet = subnetNumberOfLayersModifier/((numberOfInputNeurons+numberOfOutputNeurons)/2) * (currentNumberOfRecursiveSubnets/maxNumberOfRecursiveSubnets);
	}
	else
	{
		numberOfLayersInSubnet = subnetNumberOfLayersModifier/((numberOfInputNeurons+numberOfOutputNeurons)/2);
	}
#endif

	if(numberOfLayersInSubnet < 3)
	{
		//enforce min number of layers
		numberOfLayersInSubnet = 3;
	}

	#ifdef ANN_DEBUG
	cout << "\nlong calcNumberOfLayersInSubnet()\n" << endl;
	cout << "numberOfInputNeurons = " << numberOfInputNeurons << endl;
	cout << "numberOfOutputNeurons = " << numberOfOutputNeurons << endl;
	cout << "currentNumberOfRecursiveSubnets = " << currentNumberOfRecursiveSubnets << endl;
	cout << "maxNumberOfRecursiveSubnets = " << maxNumberOfRecursiveSubnets << endl;
	cout << "useSubnetDependentNumberOfLayers = " << useSubnetDependentNumberOfLayers << endl;
	cout << "subnetNumberOfLayersModifier = " << subnetNumberOfLayersModifier << endl;
	#endif

	return numberOfLayersInSubnet;
}



long calculateNumberOfNeuronsInNewFrontLayer(long numberOfInputNeurons, long numberOfOutputNeurons, long numberOfNeuronsInCurrentLayer, long currentNumberOfLayers, long numberOfLayers, double meanLayerDivergenceFactor, int layerDivergenceType)
{
	long numberOfNeuronsInNewFrontLayer;

	#ifdef ANN_DEBUG
	cout << "\nlong calculateNumberOfNeuronsInNewFrontLayer()\n" << endl;
	cout << "currentNumberOfLayers = " << currentNumberOfLayers << endl;
	cout << "numberOfLayers = " << numberOfLayers << endl;
	cout << "numberOfInputNeurons = " << numberOfInputNeurons << endl;
	cout << "numberOfOutputNeurons = " << numberOfOutputNeurons << endl;
	cout << "((currentNumberOfLayers+1)/(numberOfLayers+1)) = " << (double(currentNumberOfLayers)/double(numberOfLayers-1)) << endl;
	#endif

	if((layerDivergenceType == LAYER_DIVERGENCE_TYPE_LINEAR_CONVERGING) || (layerDivergenceType == LAYER_DIVERGENCE_TYPE_LINEAR_DIVERGING_THEN_CONVERGING) || (layerDivergenceType == LAYER_DIVERGENCE_TYPE_NONLINEAR_DIVERGING_THEN_CONVERGING))
	{
		long currentNumberBasicNeuronVariation;
		long currentNumberDivergenceNeurons;

		if(numberOfInputNeurons > numberOfOutputNeurons)
		{
			currentNumberBasicNeuronVariation = -long(double(numberOfInputNeurons - numberOfOutputNeurons) * (double(currentNumberOfLayers)/double(numberOfLayers-1)));

		}
		else
		{
			currentNumberBasicNeuronVariation = long(double(numberOfOutputNeurons - numberOfInputNeurons) * (double(currentNumberOfLayers)/double(numberOfLayers-1)));

		}

		if(layerDivergenceType == LAYER_DIVERGENCE_TYPE_LINEAR_CONVERGING)
		{
			currentNumberDivergenceNeurons = 0;
		}
		else if((layerDivergenceType == LAYER_DIVERGENCE_TYPE_LINEAR_DIVERGING_THEN_CONVERGING) || (layerDivergenceType == LAYER_DIVERGENCE_TYPE_NONLINEAR_DIVERGING_THEN_CONVERGING))
		{

			if(layerDivergenceType == LAYER_DIVERGENCE_TYPE_LINEAR_DIVERGING_THEN_CONVERGING)
			{
				if(currentNumberOfLayers < (numberOfLayers/2))
				{
					currentNumberDivergenceNeurons = numberOfInputNeurons * meanLayerDivergenceFactor * currentNumberOfLayers;
				}
				else
				{
					currentNumberDivergenceNeurons = (numberOfInputNeurons * meanLayerDivergenceFactor * (numberOfLayers/2)) - (numberOfInputNeurons * meanLayerDivergenceFactor * (currentNumberOfLayers-(numberOfLayers/2)));
				}
			}
			else if(layerDivergenceType == LAYER_DIVERGENCE_TYPE_NONLINEAR_DIVERGING_THEN_CONVERGING)
			{

				if(currentNumberOfLayers < (numberOfLayers/2))
				{
					currentNumberDivergenceNeurons = numberOfInputNeurons * meanLayerDivergenceFactor * advancedDivergenceFactor(currentNumberOfLayers, numberOfLayers) * currentNumberOfLayers;
				}
				else
				{
					currentNumberDivergenceNeurons = numberOfInputNeurons * meanLayerDivergenceFactor * advancedDivergenceFactor((numberOfLayers - currentNumberOfLayers), numberOfLayers) * currentNumberOfLayers;
				}
			}

		}
		else
		{
			cout << "illegal layerDivergenceType detected" << endl;
			exit(0);
		}

		numberOfNeuronsInNewFrontLayer = numberOfInputNeurons + currentNumberBasicNeuronVariation + currentNumberDivergenceNeurons;
	}
	else if((layerDivergenceType == LAYER_DIVERGENCE_TYPE_LINEAR_DIVERGING) || (layerDivergenceType == LAYER_DIVERGENCE_TYPE_LINEAR_DIVERGING_SQUARE2D) || (layerDivergenceType == LAYER_DIVERGENCE_TYPE_LINEAR_DIVERGING_SQUARE2D_RADIALBIAS))
	{
		if((currentNumberOfLayers + 1) == numberOfLayers)
		{
			numberOfNeuronsInNewFrontLayer = numberOfOutputNeurons;
		}
		else
		{
			if(layerDivergenceType == LAYER_DIVERGENCE_TYPE_LINEAR_DIVERGING)
			{
				numberOfNeuronsInNewFrontLayer = numberOfNeuronsInCurrentLayer*meanLayerDivergenceFactor;
			}
			else if((layerDivergenceType == LAYER_DIVERGENCE_TYPE_LINEAR_DIVERGING_SQUARE2D) || (layerDivergenceType == LAYER_DIVERGENCE_TYPE_LINEAR_DIVERGING_SQUARE2D_RADIALBIAS))
			{
				numberOfNeuronsInNewFrontLayer = pow((sqrt(double(numberOfNeuronsInCurrentLayer)) * meanLayerDivergenceFactor), 2.0);
						//assumes square 2D array
			}
			else
			{
				cout << "illegal layerDivergenceType detected" << endl;
				exit(0);
			}
		}
	}
	else if(layerDivergenceType == LAYER_DIVERGENCE_TYPE_LINEAR_CONVERGING_SQUARE2D)
	{
		if((currentNumberOfLayers + 1) == numberOfLayers)
		{
			numberOfNeuronsInNewFrontLayer = numberOfOutputNeurons;
		}
		else
		{
			long currentNumberBasicNeuronVariation;

			if(numberOfInputNeurons > numberOfOutputNeurons)
			{
				currentNumberBasicNeuronVariation = -long(double(numberOfInputNeurons - numberOfOutputNeurons) * (double(currentNumberOfLayers)/double(numberOfLayers-1)));
			}
			else
			{
				currentNumberBasicNeuronVariation = long(double(numberOfOutputNeurons - numberOfInputNeurons) * (double(currentNumberOfLayers)/double(numberOfLayers-1)));
			}

			numberOfNeuronsInNewFrontLayer = numberOfInputNeurons + currentNumberBasicNeuronVariation;
			numberOfNeuronsInNewFrontLayer = pow(double((long)sqrt(double(numberOfNeuronsInNewFrontLayer))), 2.0);
		}
	}
	else
	{
		cout << "illegal layerDivergenceType detected" << endl;
		exit(0);
	}

	#ifdef ANN_DEBUG
	cout << "numberOfNeuronsInNewFrontLayer = " << numberOfNeuronsInNewFrontLayer << endl;
	#endif

		//LAYER_DIVERGENCE_TYPE_LINEAR_CONVERGING NB static Divergence Rate: assuming convergence not divergence, each hidden layer has one less neuron, numberOfNeuronsInNewFrontLayer = numberOfInputNeurons - long(double(numberOfInputNeurons - numberOfOutputNeurons) * (double(currentNumberOfLayers)/double(numberOfLayers-1)));
		//LAYER_DIVERGENCE_TYPE_LINEAR_DIVERGING_THEN_CONVERGING NB meanLayerDivergenceFactor is less than 1 always; Eg 0.1 or 10% more neurons than input layer per layer

	return numberOfNeuronsInNewFrontLayer;

	//numberOfNeuronsInNewFrontLayer is calculated based upon the description of divergence for any given subnet

	/*Basic Subnet divergence	/	USE_BASIC_SUBNET_DIVERGENCE

		/   On On On  \
	   /   Hn Hn Hn Hn \
	  /  Hn Hn Hn Hn Hn  \
	 /  In In In In In In  \

	using simple layer hidden NeuronContainer heuristic; nb  numberOfLayers = (numberOfInputNeurons + numberOfOutputNeurons)/2

	*/


	/*Intermediate Subnet divergence	/	USE_INTERMEDIATE_SUBNET_DIVERGENCE

					/\
				  /    \
				/        \
			  /	           \
			/                \
		  /                    \
		/                        \

	Input 1                        Output 1
	Input 2                        Output 2
	Input 3                        Output 3
	Input 4                        Output 4
	Input N                        Output N

	   \                         /
		 \                     /
		   \                 /
			 \             /
			   \         /
				 \     /
				   \ /



	NB divergence = meanLayerDivergenceFactor until 1/2 numberOfLayers is reached,  afterwards meanLayerDivergenceFactor = -meanLayerDivergenceFactor  (until number of outputNeurons is reached).
	this implies a ~ diamond shapped (linearily increasing/decreasing) resulting neural net*/


	/* Advanced Subnet divergence	/	USE_ADVANCED_SUBNET_DIVERGENCE

						/\
					   /  \
					  /    \
					/	     \
				  /            \
			   /                  \
			/                        \

		Input 1                        Output 1
		Input 2                        Output 2
		Input 3                        Output 3
		Input 4                        Output 4
		Input N                        Output N

		 \	                         /
			\                     /
			   \               /
				  \          /
					\      /
					 \    /
					  \  /
					   \/

	NB divergence = meanLayerDivergenceFactor*f(currentNumberOfLayers)  until 1/2 numberOfLayers is reached,  afterwards meanLayerDivergenceFactor = -meanLayerDivergenceFactor*f(currentNumberOfLayers)  (until number of outputNeurons is reached)
	depending upon function f this could implies a ~ diamond shapped (non-linearily increasing/decreasing) resulting neural net
	another possibility is that f(currentNumberOfLayers) could be rand()

   */
}



double advancedDivergenceFactor(long currentNumberOfLayersFromNearestEndPoint, long numberOfLayers)
{
	double advancedDivergenceFactor;

#ifdef USE_ADVANCED_RANDOM_SUBNET_DIVERGENCE
	//seed random number generator
	advancedDivergenceFactor = rand();
#else
	advancedDivergenceFactor = currentNumberOfLayersFromNearestEndPoint/numberOfLayers;
#endif

	return advancedDivergenceFactor;
}





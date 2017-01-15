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
 * File Name: ANNneuronClass.cpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2012 Baxter AI (baxterai.com)
 * Project: Advanced Neural Network (ANN)
 * Project Version: 3a12a 31-July-2012
 * Comments:
 *
 *******************************************************************************/

#include "ANNneuronClass.h"

/************************************************************ Neural Network Class Definitions ******************************************************/

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







NeuronConnection::NeuronConnection(void)
{
	//weight = 0;			//this value must be initialised explicity
	//storedWeight = 0; //this value must be initialised explicity
}

NeuronConnection::~NeuronConnection(void)
{

}








NeuronConnectionContainer::NeuronConnectionContainer(void)
{
	neuronID = 0;
	neuron = NULL;
	neuronConnection = NULL;
	nextNeuronConnectionContainer = NULL;
}

NeuronConnectionContainer::NeuronConnectionContainer(Neuron * initialNeuron)
{
	neuronID = initialNeuron->id;
	neuron = initialNeuron;
	neuronConnection = NULL;
	nextNeuronConnectionContainer = NULL;
}

NeuronConnectionContainer::NeuronConnectionContainer(Neuron * initialNeuron, NeuronConnection * initialConnection)
{
	neuronID = initialNeuron->id;
	neuron = initialNeuron;
	neuronConnection = initialConnection;
	nextNeuronConnectionContainer = NULL;
}

NeuronConnectionContainer::~NeuronConnectionContainer(void)
{

	if(nextNeuronConnectionContainer != NULL)
	{
		delete nextNeuronConnectionContainer;
	}

	if(neuronConnection != NULL)
	{
		delete neuronConnection;
	}


	//do not delete neurons in this destructor
}










NeuronContainer::NeuronContainer(void)
{
	neuron = new Neuron();

	nextNeuronContainer = NULL;
	//previousNeuronContainer = NULL;
	hasFrontLayer = false;
	hasBackLayer = false;
	firstNeuronInFrontLayer = NULL;
	firstNeuronInBackLayer = NULL;

	//numFrontNeuronConnections = 0;
	//numBackNeuronConnections = 0;

	firstFrontNeuronConnectionContainer = new NeuronConnectionContainer();		//OLD: firstFrontNeuronConnectionContainer = NULL;
	firstBackNeuronConnectionContainer = new NeuronConnectionContainer();		//OLD: firstBackNeuronConnectionContainer = NULL;
	currentFrontNeuronConnectionContainer = firstFrontNeuronConnectionContainer;	//this variable is temporary and is used for neural network Formation only
	currentBackNeuronConnectionContainer = firstBackNeuronConnectionContainer;	//this variable is temporary and is used for neural network Formation only

#ifdef ANN
	isSubnet = false;

	//numNeuronsInBackLayerOfSubnet = 0;
	//numNeuronsInFrontLayerOfSubnet = 0;
	firstNeuronContainerInBackLayerOfSubnet = NULL;
	firstNeuronContainerInFrontLayerOfSubnet = NULL;

	isInputSubnet = false;
	isOutputSubnet = false;
#endif

}

NeuronContainer::NeuronContainer(long IDInit, long orderIDInit, long layerIDInit, long subnetIDInit)
{
	neuron = new Neuron(IDInit, orderIDInit, layerIDInit, subnetIDInit);

	nextNeuronContainer = NULL;
	//previousNeuronContainer = NULL;
	hasFrontLayer = false;
	hasBackLayer = false;
	firstNeuronInFrontLayer = NULL;
	firstNeuronInBackLayer = NULL;

	//numFrontNeuronConnections = 0;
	//numBackNeuronConnections = 0;
	firstFrontNeuronConnectionContainer = new NeuronConnectionContainer();			//OLD: firstFrontNeuronConnectionContainer = NULL;
	firstBackNeuronConnectionContainer = new NeuronConnectionContainer();			//OLD: firstBackNeuronConnectionContainer = NULL;
	currentFrontNeuronConnectionContainer = firstFrontNeuronConnectionContainer;	//this variable is temporary and is used for neural network Formation only
	currentBackNeuronConnectionContainer = firstBackNeuronConnectionContainer;		//this variable is temporary and is used for neural network Formation only


#ifdef ANN
	isSubnet = false;

	//numNeuronsInBackLayerOfSubnet = 0;
	//numNeuronsInFrontLayerOfSubnet = 0;
	firstNeuronContainerInBackLayerOfSubnet = NULL;
	firstNeuronContainerInFrontLayerOfSubnet = NULL;

	isInputSubnet = false;
	isOutputSubnet = false;
#endif

}



NeuronContainer::~NeuronContainer(void)
{

	if(neuron != NULL)
	{
		delete neuron;
	}

	if(firstFrontNeuronConnectionContainer != NULL)
	{
		delete firstFrontNeuronConnectionContainer;
	}

	if(firstBackNeuronConnectionContainer != NULL)
	{
		delete firstBackNeuronConnectionContainer;
	}

	if(firstNeuronContainerInBackLayerOfSubnet != NULL)
	{
		delete firstNeuronContainerInBackLayerOfSubnet;
	}

	if(firstNeuronContainerInFrontLayerOfSubnet != NULL)
	{
		delete firstNeuronContainerInFrontLayerOfSubnet;
	}

	if(nextNeuronContainer != NULL)
	{
		delete nextNeuronContainer;
	}
	else
	{
		if(firstNeuronInFrontLayer != NULL)
		{
			delete firstNeuronInFrontLayer;
		}
	}




	/*OLD:::

	ExperienceInput * currentNeuronConnectionContainer;

	currentNeuronConnectionContainer = firstFrontNeuronConnectionContainer;
	while(currentNeuronConnectionContainer->next != NULL)
	{
		previousNeuronConnectionContainer = currentNeuronConnectionContainer;
		currentNeuronConnectionContainer = currentNeuronConnectionContainer->next;
		delete previousNeuronConnectionContainer;
	}

	currentNeuronConnectionContainer = firstBackNeuronConnectionContainer;
	while(currentNeuronConnectionContainer->next != NULL)
	{
		previousNeuronConnectionContainer = currentNeuronConnectionContainer;
		currentNeuronConnectionContainer = currentNeuronConnectionContainer->next;
		delete previousNeuronConnectionContainer;
	}

	currentNeuronConnectionContainer = firstNeuronContainerInBackLayerOfSubnet;
	while(currentNeuronConnectionContainer->next != NULL)
	{
		previousNeuronConnectionContainer = currentNeuronConnectionContainer;
		currentNeuronConnectionContainer = currentNeuronConnectionContainer->next;
		delete previousNeuronConnectionContainer;
	}

	currentNeuronConnectionContainer = firstNeuronContainerInFrontLayerOfSubnet;
	while(currentNeuronConnectionContainer->next != NULL)
	{
		previousNeuronConnectionContainer = currentNeuronConnectionContainer;
		currentNeuronConnectionContainer = currentNeuronConnectionContainer->next;
		delete previousNeuronConnectionContainer;
	}
	*/

}














Neuron::Neuron(void)
{
	id = 0;			//none = 0
	orderID = 0;	//none = 0
	layerID = 0;	//none = 0
	subnetID = 0;	//none = 0

	//bias = 0;		 //this value must be initialised explicity
	//storedBias = 0; 	//this value must be initialised explicity
	//output = 0; 		//this value must be initialised explicity
	//classTarget = 0; 	//this value must be initialised explicity
	//error = 0; 		//this value must be initialised explicity

	spatialCoordinatesSet = 0;
	//xPosRel = 0;		//this value must be initialised explicity
	//yPosRel = 0;		//this value must be initialised explicity
	//zPosRel = 0;		//this value must be initialised explicity
}

Neuron::Neuron(long IDInit, long orderIDInit, long layerIDInit, long subnetIDInit)
{
	id = IDInit;
	orderID = orderIDInit;
	layerID = layerIDInit;
	subnetID = subnetIDInit;

	//bias = 0;		 //this value must be initialised explicity
	//storedBias = 0; 	//this value must be initialised explicity
	//output = 0; 		//this value must be initialised explicity
	//classTarget = 0; 	//this value must be initialised explicity
	//error = 0; 		//this value must be initialised explicity

	spatialCoordinatesSet = 0;
	//xPosRel = 0;		//this value must be initialised explicity
	//yPosRel = 0;		//this value must be initialised explicity
	//zPosRel = 0;		//this value must be initialised explicity
}

Neuron::~Neuron(void)
{

}





void fillInNeuronIDProperties(Neuron * neuronToUpdate, long IDInit, long orderIDInit, long layerIDInit, long subnetIDInit)
{
	neuronToUpdate->id = IDInit;
	neuronToUpdate->orderID = orderIDInit;
	neuronToUpdate->layerID = layerIDInit;
	neuronToUpdate->subnetID = subnetIDInit;
}


void copyNeuronKeyProperties(Neuron * neuronToUpdate, Neuron * neuronToCopy)
{
	neuronToUpdate->bias = neuronToCopy->bias;
	neuronToUpdate->storedBias = neuronToCopy->storedBias;
	neuronToUpdate->output = neuronToCopy->output;
	neuronToUpdate->classTarget = neuronToCopy->classTarget;
	neuronToUpdate->error = neuronToCopy->error;
}

void averageNeuronKeyProperties(Neuron * neuronToUpdate, Neuron * neuronToCopy)
{
	neuronToUpdate->bias = (neuronToUpdate->bias + neuronToCopy->bias)/2.0;
	neuronToUpdate->storedBias = (neuronToUpdate->storedBias + neuronToCopy->storedBias)/2.0;
	neuronToUpdate->output = (neuronToUpdate->output + neuronToCopy->output)/2.0;
	neuronToUpdate->classTarget = (neuronToUpdate->classTarget + neuronToCopy->classTarget)/2.0;
	neuronToUpdate->error = (neuronToUpdate->error + neuronToCopy->error)/2.0;
}


/************************************************************ End Neural Network Class Definitions **************************************************/



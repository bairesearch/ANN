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
 * File Name: ANNneuronClass.cpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2017 Baxter AI (baxterai.com)
 * Project: Artificial Neural Network (ANN)
 * Project Version: 3l2a 12-June-2017
 * Comments:
 *
 *******************************************************************************/

#include "ANNneuronClass.hpp"

/************************************************************ Neural Network Class Definitions* *****************************************************/


ANNneuron::ANNneuron(void)
{
	id = 0;		//none = 0
	orderID = 0;	//none = 0
	layerID = 0;	//none = 0
	subnetID = 1;

	//incase network is never trained, but output for visualisation purposes, set these values to dummy values
	bias = 0;		//this value must be initialised explicity
	storedBias = 0; 	//this value must be initialised explicity
	output = 0;		//this value must be initialised explicity
	classTarget = 0;	//this value must be initialised explicity
	error = 0;		//this value must be initialised explicity
	#ifdef ANN_ALGORITHM_CLASSIFICATION_NETWORK
	backNeuronMatchTemp = false;
	memoryTrace = 1;
	intermediaryNeuronCreatedThisRound = false;
	neuronTypeInput = false;
	neuronTypeTopLevelCategory = false;
	neuronTypeIntermediary = false;
	printed = false;
	topLevelCategoryAlreadyExistsDeleteThisNewCopy = false;
	#endif
	printedXML = false;

	spatialCoordinatesSet2D = false;
	xPosRel = 0;
	yPosRel = 0;
	zPosRel = 0;
	#ifdef ANN_ALGORITHM_CLASSIFICATION_NETWORK
	spatialCoordinatesSetClassification = true;
	xPosRelFrac = 0.0;
	yPosRelFrac = 0.0;
	zPosRelFrac = 0.0;
	#endif

	nextNeuron = NULL;

	hasFrontLayer = false;
	hasBackLayer = false;
	firstNeuronInFrontLayer = NULL;
	firstNeuronInBackLayer = NULL;

	#ifdef ANN_SUBNETS
	isSubnet = false;

	firstNeuronInBackLayerOfSubnet = NULL;
	firstNeuronInFrontLayerOfSubnet = NULL;

	isInputSubnet = false;
	isOutputSubnet = false;
	#endif
}

ANNneuron::ANNneuron(long IDinit, long orderIDinit, long layerIDinit, long subnetIDinit)
{
	id = IDinit;
	orderID = orderIDinit;
	layerID = layerIDinit;
	subnetID = subnetIDinit;

	bias = 0;		//this value must be initialised explicity
	storedBias = 0;		//this value must be initialised explicity
	output = 0;		//this value must be initialised explicity
	classTarget = 0;	//this value must be initialised explicity
	error = 0;		//this value must be initialised explicity
	#ifdef ANN_ALGORITHM_CLASSIFICATION_NETWORK
	backNeuronMatchTemp = false;
	memoryTrace = 1;
	intermediaryNeuronCreatedThisRound = false;
	neuronTypeInput = false;
	neuronTypeTopLevelCategory = false;
	neuronTypeIntermediary = false;
	printed = false;
	printedXML = false;
	topLevelCategoryAlreadyExistsDeleteThisNewCopy = false;
	#endif

	spatialCoordinatesSet2D = false;
	xPosRel = 0;	      //this value must be initialised explicity
	yPosRel = 0;	      //this value must be initialised explicity
	zPosRel = 0;	      //this value must be initialised explicity
	#ifdef ANN_ALGORITHM_CLASSIFICATION_NETWORK
	spatialCoordinatesSetClassification = false;
	xPosRelFrac = 0.0;
	yPosRelFrac = 0.0;
	zPosRelFrac = 0.0;
	#endif

	nextNeuron = NULL;

	hasFrontLayer = false;
	hasBackLayer = false;
	firstNeuronInFrontLayer = NULL;
	firstNeuronInBackLayer = NULL;

	#ifdef ANN_SUBNETS
	isSubnet = false;

	firstNeuronInBackLayerOfSubnet = NULL;
	firstNeuronInFrontLayerOfSubnet = NULL;

	isInputSubnet = false;
	isOutputSubnet = false;
	#endif
}

ANNneuron::~ANNneuron(void)
{
	for(vector<ANNneuronConnection*>::iterator connectionIter = frontANNneuronConnectionList.begin(); connectionIter != frontANNneuronConnectionList.end(); connectionIter++)
	{
		if(*connectionIter != NULL)
		{
			delete *connectionIter;
		}
	}

	/*
	for(vector<ANNneuronConnection*>::iterator connectionIter = backANNneuronConnectionList.begin(); connectionIter != backANNneuronConnectionList.end(); connectionIter++)
	{
		if(*connectionIter != NULL)
		{
			#ifdef GIA_FREE_MEMORY_DEBUG
			//cout << "deleting: backANNneuronConnectionList connection: " << (*connectionIter)->frontNeuronID << endl;
			#endif
			delete* connectionIter;
		}
	}
	*/

	#ifndef ANN_ALGORITHM_CLASSIFICATION_NETWORK

	if(firstNeuronInBackLayerOfSubnet != NULL)
	{
		delete firstNeuronInBackLayerOfSubnet;
	}

	if(firstNeuronInFrontLayerOfSubnet != NULL)
	{
		delete firstNeuronInFrontLayerOfSubnet;
	}

	if(nextNeuron != NULL)
	{
		delete nextNeuron;
	}
	else
	{
		if(firstNeuronInFrontLayer != NULL)
		{
			delete firstNeuronInFrontLayer;
		}
	}

	#endif
}





void ANNneuronClassClass::fillInNeuronIDProperties(ANNneuron* neuronToUpdate, long IDinit, long orderIDinit, long layerIDinit, long subnetIDinit)
{
	neuronToUpdate->id = IDinit;
	neuronToUpdate->orderID = orderIDinit;
	neuronToUpdate->layerID = layerIDinit;
	neuronToUpdate->subnetID = subnetIDinit;
}


void ANNneuronClassClass::copyNeuronKeyProperties(ANNneuron* neuronToUpdate, ANNneuron* neuronToCopy)
{
	neuronToUpdate->bias = neuronToCopy->bias;
	neuronToUpdate->storedBias = neuronToCopy->storedBias;
	neuronToUpdate->output = neuronToCopy->output;
	neuronToUpdate->classTarget = neuronToCopy->classTarget;
	neuronToUpdate->error = neuronToCopy->error;
}

void ANNneuronClassClass::averageNeuronKeyProperties(ANNneuron* neuronToUpdate, ANNneuron* neuronToCopy)
{
	neuronToUpdate->bias = (neuronToUpdate->bias + neuronToCopy->bias)/2.0;
	neuronToUpdate->storedBias = (neuronToUpdate->storedBias + neuronToCopy->storedBias)/2.0;
	neuronToUpdate->output = (neuronToUpdate->output + neuronToCopy->output)/2.0;
	neuronToUpdate->classTarget = (neuronToUpdate->classTarget + neuronToCopy->classTarget)/2.0;
	neuronToUpdate->error = (neuronToUpdate->error + neuronToCopy->error)/2.0;
}


/************************************************************ End Neural Network Class Definitions* *************************************************/



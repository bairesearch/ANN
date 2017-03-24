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
 * File Name: ANNxmlConversion.cpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2017 Baxter AI (baxterai.com)
 * Project: Artificial Neural Network (ANN)
 * Project Version: 3k2c 21-March-2017
 * Comments
 *
 *******************************************************************************/



#include "ANNxmlConversion.hpp"

//this function works and no longer needs to be tested

ANNneuron* ANNxmlConversionClass::readNetXMLfileAndRecordFormationVariables(string xmlFileName, ANNneuron* firstInputNeuronInNetwork, long* numberOfInputNeurons, long* numberOfOutputNeurons)
{
	ANNneuron* firstOutputNeuronInNetwork;

	this->readNetXMLfile(xmlFileName, firstInputNeuronInNetwork);

	firstOutputNeuronInNetwork = this->recordOutputNeuronAndNumInputAndOutputNeuronsInNetwork(firstInputNeuronInNetwork, numberOfInputNeurons, numberOfOutputNeurons);
	return firstOutputNeuronInNetwork;
}

ANNneuron* ANNxmlConversionClass::recordOutputNeuronAndNumInputAndOutputNeuronsInNetwork(ANNneuron* firstInputNeuronInNetwork, long* numberOfInputNeurons, long* numberOfOutputNeurons)
{
	ANNneuron* firstOutputNeuronInNetwork;

	long networkLayer = 1;

	ANNneuron* currentNeuron = firstInputNeuronInNetwork;

	bool stillMoreLayers = true;
	while(stillMoreLayers)
	{
		bool currentLayerHasFrontLayer = false;

		ANNneuron* firstNeuronInLayer = currentNeuron;

		bool numNeuronsInCurrentLayer = 0;
		while(currentNeuron->nextNeuron != NULL)
		{
			numNeuronsInCurrentLayer++;
			currentNeuron = currentNeuron->nextNeuron;
		}

		if(networkLayer == 1)
		{
			*numberOfInputNeurons = numNeuronsInCurrentLayer;
		}

		if(firstNeuronInLayer->hasFrontLayer)
		{
			currentNeuron=firstNeuronInLayer->firstNeuronInFrontLayer;
			networkLayer++;
		}
		else
		{
			*numberOfOutputNeurons = numNeuronsInCurrentLayer;
			firstOutputNeuronInNetwork = firstNeuronInLayer;
			stillMoreLayers = false;
		}
	}

	return firstOutputNeuronInNetwork;
}


bool ANNxmlConversionClass::readNetXMLfile(string xmlFileName, ANNneuron* firstInputNeuronInNetwork)
{
	bool result = true;

 	XMLparserTag* firstTagInXMLFile = new XMLparserTag();	//the firstTagInXMLFile object must be initialised here (in ANNxmlConversion.cpp scope). if it is initialised in XMLparserClass.cpp else it will be come corrupted,

 	if(!XMLparserClass.readXMLfile(xmlFileName, firstTagInXMLFile))
 	{
		result = false;
	}

	if(!this->parseNetTag(firstTagInXMLFile, firstInputNeuronInNetwork))
	{
		result = false;
	}

	if(!this->linkLayerXNeuronsBasedUponFrontANNneuronConnectionListNeuronIDs(firstInputNeuronInNetwork, firstInputNeuronInNetwork, false, NULL))
	{
		result = false;
	}
	return result;
}

bool ANNxmlConversionClass::writeNetXMLfile(string xmlFileName, ANNneuron* firstInputNeuronInNetwork)
{
	bool result = true;

 	XMLparserTag* firstTagInXMLFile = new XMLparserTag();	//the firstTagInXMLFile object must be initialised here (in ANNxmlConversion.cpp scope). if it is initialised in XMLparserClass.cpp else it will be come corrupted,

	XMLparserTag* currentTagL0 = firstTagInXMLFile;
	currentTagL0->name = NET_XML_TAG_network;
	XMLparserTag* newTag0 = new XMLparserTag();	//had to add a null tag
	currentTagL0->nextTag = newTag0;

	XMLparserTag* firstTagL1 = new XMLparserTag();
	currentTagL0->firstLowerLevelTag = firstTagL1;
	XMLparserTag* currentTagL1 = currentTagL0->firstLowerLevelTag;
	XMLparserTag* newTag1 = new XMLparserTag();	//had to add a null tag
	currentTagL1->nextTag = newTag1;

	currentTagL1->name = NET_XML_TAG_subnet;
	XMLparserTag* firstTagL2 = new XMLparserTag();
	currentTagL1->firstLowerLevelTag = firstTagL2;
	XMLparserTag* currentTagL2 = currentTagL1->firstLowerLevelTag;
	/*
	XMLparserTag* newTag2 = new XMLparserTag();	//had to add a null tag	//removed ANN4a3j
	currentTagL2->nextTag = newTag2;
	*/

	if(!this->generateXMLtagListBasedUponSubnet(currentTagL1->firstLowerLevelTag, firstInputNeuronInNetwork))
	{
		result = false;
	}

 	if(!XMLparserClass.writeXMLfile(xmlFileName, firstTagInXMLFile))
 	{
		result = false;
	}

	delete firstTagInXMLFile;

	return result;
}


bool ANNxmlConversionClass::generateXMLtagListBasedUponSubnet(XMLparserTag* firstTagInSubnet, ANNneuron* firstNeuronInSubnet)
{
	bool result = true;

	XMLparserTag* currentTagL0 = firstTagInSubnet;
	if(!this->generateXMLtagListBasedUponLayer(currentTagL0, firstNeuronInSubnet))
	{
		result = false;
	}

	return result;
}


bool ANNxmlConversionClass::generateXMLtagListBasedUponLayer(XMLparserTag* firstTagInSubnet, ANNneuron* firstNeuronInLayer)
{
	bool result = true;

	bool currentLayerHasFrontLayer = false;

	ANNneuron* currentNeuron = firstNeuronInLayer;
	XMLparserTag* currentTagL0 = firstTagInSubnet;
	while(currentTagL0->nextTag != NULL)
	{
		currentTagL0 = currentTagL0->nextTag;	//skip already created layers
	}

	XMLparserTag* layerTag = currentTagL0;

	currentTagL0->name = NET_XML_TAG_layer;
	XMLparserTag* newTag0 = new XMLparserTag();	//had to add a null tag
	currentTagL0->nextTag = newTag0;

	/*
	XMLparserTag* newTag = new XMLparserTag();
	currentTagL0->nextTag = newTag;
	currentTagL0 = currentTagL0->nextTag;
	*/

	XMLparserTag* firstTagL1 = new XMLparserTag();
	currentTagL0->firstLowerLevelTag = firstTagL1;
	XMLparserTag* currentTagL1 = currentTagL0->firstLowerLevelTag;
	XMLparserTag* newTag1 = new XMLparserTag();	//had to add a null tag
	currentTagL1->nextTag = newTag1;

	bool foundNonPrintedNeuronOnLayer = false;

	while(currentNeuron->nextNeuron != NULL)
	{
		if(!(currentNeuron->printedXML))
		{
			foundNonPrintedNeuronOnLayer = true;
			currentNeuron->printedXML = true;

			//generate neuron container tag
			currentTagL1->name = NET_XML_TAG_neuronContainer;
			XMLparserTag* firstTagL2 = new XMLparserTag();
			currentTagL1->firstLowerLevelTag = firstTagL2;
			XMLparserTag* currentTagL2 = currentTagL1->firstLowerLevelTag;

			//generate neuron tag
			currentTagL2->name = NET_XML_TAG_neuron;

			XMLparserAttribute* currentAttribute = currentTagL2->firstAttribute;

			currentAttribute->name = NET_XML_ATTRIBUTE_id;
			currentAttribute->value = SHAREDvars.convertIntToString((currentNeuron->id));
			currentAttribute = XMLparserClass.createNewAttribute(currentAttribute);

			#ifndef DO_NOT_STORE_NET_XML_NEURON_ID_PARAMETERS
			currentAttribute->name = NET_XML_ATTRIBUTE_layerID;
			currentAttribute->value = SHAREDvars.convertIntToString((currentNeuron->layerID));
			currentAttribute = XMLparserClass.createNewAttribute(currentAttribute);

			currentAttribute->name = NET_XML_ATTRIBUTE_orderID;
			currentAttribute->value = SHAREDvars.convertIntToString((currentNeuron->orderID));
			currentAttribute = XMLparserClass.createNewAttribute(currentAttribute);

			currentAttribute->name = NET_XML_ATTRIBUTE_subnetID;
			currentAttribute->value = SHAREDvars.convertIntToString((currentNeuron->subnetID));
			currentAttribute = XMLparserClass.createNewAttribute(currentAttribute);
			#endif

			#ifndef DO_NOT_STORE_NET_XML_NEURON_KEYPROPERTIES_PARAMETERS
			currentAttribute->name = NET_XML_ATTRIBUTE_bias;
			currentAttribute->value = SHAREDvars.convertDoubleToString((currentNeuron->bias), "%0.6f");
			currentAttribute = XMLparserClass.createNewAttribute(currentAttribute);

			currentAttribute->name = NET_XML_ATTRIBUTE_output;
			currentAttribute->value = SHAREDvars.convertDoubleToString((currentNeuron->output), "%0.6f");
			currentAttribute = XMLparserClass.createNewAttribute(currentAttribute);

			currentAttribute->name = NET_XML_ATTRIBUTE_classTarget;
			currentAttribute->value = SHAREDvars.convertDoubleToString((currentNeuron->classTarget), "%0.6f");
			currentAttribute = XMLparserClass.createNewAttribute(currentAttribute);

			currentAttribute->name = NET_XML_ATTRIBUTE_error;
			currentAttribute->value = SHAREDvars.convertDoubleToString((currentNeuron->error), "%0.6f");
			currentAttribute = XMLparserClass.createNewAttribute(currentAttribute);
			#endif

			#ifdef ANN_ALGORITHM_CLASSIFICATION_NETWORK
			currentAttribute->name = NET_XML_ATTRIBUTE_memoryTrace;
			currentAttribute->value = SHAREDvars.convertIntToString((currentNeuron->memoryTrace));
			currentAttribute = XMLparserClass.createNewAttribute(currentAttribute);
			#endif

			#ifndef DO_NOT_STORE_NET_XML_NEURON_SPATIAL_COORD_PARAMETERS
			if(currentNeuron->spatialCoordinatesSet2D)
			{
				currentAttribute->name = NET_XML_ATTRIBUTE_xPosRel;
				currentAttribute->value = SHAREDvars.convertIntToString((currentNeuron->xPosRel));
				currentAttribute = XMLparserClass.createNewAttribute(currentAttribute);

				currentAttribute->name = NET_XML_ATTRIBUTE_yPosRel;
				currentAttribute->value = SHAREDvars.convertIntToString((currentNeuron->yPosRel));
				currentAttribute = XMLparserClass.createNewAttribute(currentAttribute);

				currentAttribute->name = NET_XML_ATTRIBUTE_zPosRel;
				currentAttribute->value = SHAREDvars.convertIntToString((currentNeuron->zPosRel));
				currentAttribute = XMLparserClass.createNewAttribute(currentAttribute);
			}
			#endif
			#ifdef ANN_ALGORITHM_CLASSIFICATION_NETWORK
			else if(currentNeuron->spatialCoordinatesSetClassification)
			{

				currentAttribute->name = NET_XML_ATTRIBUTE_xPosRelFrac;
				currentAttribute->value = SHAREDvars.convertDoubleToString((currentNeuron->xPosRelFrac), "%0.6f");
				currentAttribute = XMLparserClass.createNewAttribute(currentAttribute);

				currentAttribute->name = NET_XML_ATTRIBUTE_yPosRelFrac;
				currentAttribute->value = SHAREDvars.convertDoubleToString((currentNeuron->yPosRelFrac), "%0.6f");
				currentAttribute = XMLparserClass.createNewAttribute(currentAttribute);

				currentAttribute->name = NET_XML_ATTRIBUTE_zPosRelFrac;
				currentAttribute->value = SHAREDvars.convertDoubleToString((currentNeuron->zPosRelFrac), "%0.6f");
				currentAttribute = XMLparserClass.createNewAttribute(currentAttribute);
			}
			#endif

			XMLparserTag* newTag = new XMLparserTag();
			currentTagL2->nextTag = newTag;
			currentTagL2 = currentTagL2->nextTag;

			//generate NET_XML_TAG_forwardANNneuronConnectionsList tag
			if(!(currentNeuron->frontANNneuronConnectionList.empty()))	//if(currentNeuron -> firstFrontANNneuronConnectionContainer -> nextANNneuronConnectionContainer != NULL)
			{
				currentTagL2->name = NET_XML_TAG_forwardANNneuronConnectionsList;
				XMLparserTag* firstTagL3 = new XMLparserTag();
				currentTagL2->firstLowerLevelTag = firstTagL3;
				XMLparserTag* currentTagL3 = currentTagL2->firstLowerLevelTag;

				for(vector<ANNneuronConnection*>::iterator connectionIter = currentNeuron->frontANNneuronConnectionList.begin(); connectionIter != currentNeuron->frontANNneuronConnectionList.end(); connectionIter++)
				{
					ANNneuronConnection* currentANNneuronConnection = *connectionIter;

					//generate NET_XML_TAG_forwardANNneuronConnection tag and attributes
					currentTagL3->name = NET_XML_TAG_forwardANNneuronConnection;

					XMLparserAttribute* currentAttribute = currentTagL3->firstAttribute;

					currentAttribute->name = NET_XML_ATTRIBUTE_neuronID;
					currentAttribute->value = SHAREDvars.convertIntToString((currentANNneuronConnection->frontNeuron->id));
					currentAttribute = XMLparserClass.createNewAttribute(currentAttribute);

					#ifndef DO_NOT_STORE_NET_XML_NEURON_KEYPROPERTIES_PARAMETERS
					currentAttribute->name = NET_XML_ATTRIBUTE_weight;
					currentAttribute->value = SHAREDvars.convertDoubleToString((currentANNneuronConnection->weight), "%0.6f");
					currentAttribute = XMLparserClass.createNewAttribute(currentAttribute);
					#endif

					#ifdef ANN_ALGORITHM_CLASSIFICATION_NETWORK
					currentAttribute->name = NET_XML_ATTRIBUTE_idealValue;
					currentAttribute->value = SHAREDvars.convertDoubleToString((currentANNneuronConnection->idealValue), "%0.6f");
					currentAttribute = XMLparserClass.createNewAttribute(currentAttribute);
					#endif

					XMLparserTag* newTag = new XMLparserTag();
					currentTagL3->nextTag = newTag;
					currentTagL3 = currentTagL3->nextTag;
				}
				XMLparserTag* newTag = new XMLparserTag();
				currentTagL2->nextTag = newTag;
			}

			#ifdef ANN_SUBNETS
			currentTagL2 = currentTagL2->nextTag;

			//generate subtag tag
			if(currentNeuron->isSubnet)
			{
				currentTagL2->name = NET_XML_TAG_subnet;

				XMLparserTag* newTag1 = new XMLparserTag();
				currentTagL2->firstLowerLevelTag = newTag1;
				if(!this->generateXMLtagListBasedUponSubnet(currentTagL2->firstLowerLevelTag, currentNeuron->firstNeuronInBackLayerOfSubnet))
				{
					result = false;
				}

				XMLparserTag* newTag = new XMLparserTag();
				currentTagL2->nextTag = newTag;
				//currentTagL2 = currentTagL2->nextTag;
			}
			#endif

			#ifdef ANN_ALGORITHM_CLASSIFICATION_NETWORK
			if(currentNeuron->hasFrontLayer)
			{
				for(vector<ANNneuronConnection*>::iterator connectionIter = currentNeuron->frontANNneuronConnectionList.begin(); connectionIter != currentNeuron->frontANNneuronConnectionList.end(); connectionIter++)
				{
					ANNneuronConnection* currentANNneuronConnection = *connectionIter;
					if(!this->generateXMLtagListBasedUponLayer(firstTagInSubnet, currentANNneuronConnection->frontNeuron))
					{
						result = false;
					}
				}
			}
			#endif

			XMLparserTag* newTag2 = new XMLparserTag();
			currentTagL1->nextTag = newTag2;
			currentTagL1 = currentTagL1->nextTag;
		}

		currentNeuron = currentNeuron->nextNeuron;
	}

	if(!foundNonPrintedNeuronOnLayer)
	{
		this->clearTag(layerTag);
	}

	#ifndef ANN_ALGORITHM_CLASSIFICATION_NETWORK
	if(firstNeuronInLayer->hasFrontLayer)
	{
		if(!this->generateXMLtagListBasedUponLayer(firstTagInSubnet, firstNeuronInLayer->firstNeuronInFrontLayer))
		{
			result = false;
		}
	}
	#endif

	return result;
}

bool ANNxmlConversionClass::linkLayerXNeuronsBasedUponFrontANNneuronConnectionListNeuronIDs(ANNneuron* firstNeuronInLayer, ANNneuron* firstInputNeuronInNetwork, bool hasBackLayer, ANNneuron* firstNeuronInBackLayer)
{
	bool result = true;

	ANNneuron* currentNeuron = firstNeuronInLayer;
	while(currentNeuron->nextNeuron != NULL)
	{
		for(vector<ANNneuronConnection*>::iterator connectionIter = currentNeuron->frontANNneuronConnectionList.begin(); connectionIter != currentNeuron->frontANNneuronConnectionList.end(); connectionIter++)
		{
			ANNneuronConnection* currentANNneuronConnection = *connectionIter;

			bool tempResult = false;

			ANNneuron* neuronToConnect = this->findNeuron(firstInputNeuronInNetwork, currentANNneuronConnection->frontNeuronID, &tempResult);
			if(tempResult)
			{
				//link neurons
				currentANNneuronConnection->backNeuron = currentNeuron;
				currentANNneuronConnection->frontNeuron = neuronToConnect;
				neuronToConnect->backANNneuronConnectionList.push_back(currentANNneuronConnection);
			}
			else
			{
				cout << "linkNeuronsBasedUponConnectionIDs error: neuron in neuron connection list not found. neuron id being searched for = " << currentANNneuronConnection->frontNeuronID << endl;
				result = false;
			}
		}

		if(firstNeuronInLayer->firstNeuronInFrontLayer != NULL)
		{
			currentNeuron->hasFrontLayer = true;
			currentNeuron->firstNeuronInFrontLayer = firstNeuronInLayer->firstNeuronInFrontLayer;
		}
		if(hasBackLayer)
		{
			currentNeuron->hasBackLayer = true;
			currentNeuron->firstNeuronInBackLayer = firstNeuronInBackLayer;
		}

		#ifdef ANN_SUBNETS
		if(currentNeuron->firstNeuronInBackLayerOfSubnet != NULL)
		{
			currentNeuron->isSubnet = true;
			if(currentNeuron->hasFrontLayer != true)
			{
				currentNeuron->isInputSubnet = true;
			}
			if(currentNeuron->hasFrontLayer != true)
			{
				currentNeuron->isOutputSubnet = true;
			}

			if(!this->linkLayerXNeuronsBasedUponFrontANNneuronConnectionListNeuronIDs(currentNeuron->firstNeuronInBackLayerOfSubnet, firstInputNeuronInNetwork, false, NULL))
			{
				result = false;
			}

			ANNneuron* firstOutputNeuronInSubnet;
			long temp;
			firstOutputNeuronInSubnet = this->recordOutputNeuronAndNumInputAndOutputNeuronsInNetwork(currentNeuron->firstNeuronInBackLayerOfSubnet, &temp, &temp);
			currentNeuron->firstNeuronInFrontLayerOfSubnet	= firstOutputNeuronInSubnet;
		}
		#endif

		currentNeuron = currentNeuron->nextNeuron;
	}
	if(firstNeuronInLayer->hasFrontLayer)
	{
		if(!this->linkLayerXNeuronsBasedUponFrontANNneuronConnectionListNeuronIDs(firstNeuronInLayer->firstNeuronInFrontLayer, firstInputNeuronInNetwork, true, firstNeuronInLayer))
		{
			result = false;
		}
	}

	return result;
}


ANNneuron* ANNxmlConversionClass::findNeuron(ANNneuron* firstNeuronInLayer, long neuronIDtoFind, bool* result)
{
	ANNneuron* foundNeuron = NULL;

	ANNneuron* currentNeuron = firstNeuronInLayer;
	while(currentNeuron->nextNeuron != NULL)
	{
		if(currentNeuron->id == neuronIDtoFind)
		{
			*result = true;
			foundNeuron = currentNeuron;
		}
		#ifdef ANN_SUBNETS
		if(currentNeuron->firstNeuronInBackLayerOfSubnet != NULL)
		{
			bool tempResult = false;
			ANNneuron* tempFoundNeuron;
			tempFoundNeuron = this->findNeuron(currentNeuron->firstNeuronInBackLayerOfSubnet, neuronIDtoFind, &tempResult);
			if(tempResult)
			{
				*result = true;
				foundNeuron = tempFoundNeuron;
			}
		}
		#endif
		currentNeuron = currentNeuron->nextNeuron;
	}

	if(firstNeuronInLayer->firstNeuronInFrontLayer != NULL)
	{
		bool tempResult = false;
		ANNneuron* tempFoundNeuron;
		tempFoundNeuron = this->findNeuron(firstNeuronInLayer->firstNeuronInFrontLayer, neuronIDtoFind, &tempResult);
		if(tempResult)
		{
			*result = true;
			foundNeuron = tempFoundNeuron;
		}
	}

	return foundNeuron;
}


//Top Level
bool ANNxmlConversionClass::parseNetTag(XMLparserTag* firstTagInNetwork, ANNneuron* currentNeuron)
{
	bool result = true;

	XMLparserTag* currentTagUpdatedL1 = firstTagInNetwork;

	if(currentTagUpdatedL1->name == NET_XML_TAG_network)
	{
		currentTagUpdatedL1 = currentTagUpdatedL1->firstLowerLevelTag;
	}
	else
	{
		cout << "parseNetTag error: network tag not detected" << endl;
		result = false;
	}

	if(result)
	{
		if(currentTagUpdatedL1->name == NET_XML_TAG_iodescriptions)
		{
			XMLparserTag* currentTagUpdatedL2 = currentTagUpdatedL1->firstLowerLevelTag;
			if(currentTagUpdatedL2->name == NET_XML_TAG_inputNeurons)
			{
				XMLparserTag* currentTagUpdatedL3 = currentTagUpdatedL2->firstLowerLevelTag;
				while(currentTagUpdatedL3->nextTag != NULL)
				{
					if(currentTagUpdatedL3->name == NET_XML_TAG_description)
					{
						cout << "Input ANNneuron Description: " << currentTagUpdatedL3->value << endl;
					}
					else
					{
						cout << "parseInputNeuronsDescriptionTag error: description tag not detected" << endl;
					}
					currentTagUpdatedL3=currentTagUpdatedL3->nextTag;
				}

			}
			else
			{
				cout << "parseIODescriptionsTag error: no inputNeurons tag detected" << endl;
			}
			currentTagUpdatedL2=currentTagUpdatedL2->nextTag;
			if(currentTagUpdatedL2->name == NET_XML_TAG_outputNeurons)
			{
				XMLparserTag* currentTagUpdatedL3 = currentTagUpdatedL2->firstLowerLevelTag;
				while(currentTagUpdatedL3->nextTag != NULL)
				{
					if(currentTagUpdatedL3->name == NET_XML_TAG_description)
					{
						cout << "Output ANNneuron Description: " << currentTagUpdatedL3->value << endl;
					}
					else
					{
						cout << "parseOutputNeuronsDescriptionTag error: description tag not detected" << endl;
					}
					currentTagUpdatedL3=currentTagUpdatedL3->nextTag;
				}
			}
			else
			{
				cout << "parseIODescriptionsTag error: no outputNeurons tag detected" << endl;
			}
			currentTagUpdatedL1=currentTagUpdatedL1->nextTag;
		}
		if(currentTagUpdatedL1->name == NET_XML_TAG_subnet)
		{
			long wrongAndNotUsedIDcounter = 1;
			if(!this->parseSubnetTag(currentTagUpdatedL1->firstLowerLevelTag, currentNeuron, 1, &wrongAndNotUsedIDcounter, 1))
			{
				result = false;
			}
		}
		else
		{
			cout << "parseNetTag error: no subnet tag detected" << endl;
		}
	}

	return result;
}

bool ANNxmlConversionClass::parseSubnetTag(XMLparserTag* firstTagInSubnet, ANNneuron* firstNeuronInSubnet, long layerIDcounter, long* wrongAndNotUsedIDcounter, long subnetIDcounter)
{
	bool result = true;

	ANNneuron* currentNeuron = firstNeuronInSubnet;

	XMLparserTag* currentTag = firstTagInSubnet;

	ANNneuron* recordOfFirstNeuronInPreviousLayer;	//to stop an additional layer being added

	while(currentTag->nextTag != NULL)
	{
		if(currentTag->name == NET_XML_TAG_layer)
		{
			if(!this->parseLayerTag(currentTag->firstLowerLevelTag, currentNeuron, layerIDcounter++, 1, wrongAndNotUsedIDcounter, subnetIDcounter))
			{
				result = false;
			}
			else
			{
				recordOfFirstNeuronInPreviousLayer = currentNeuron;	//to stop an additional layer being added
				ANNneuron* newNeuron = new ANNneuron();
				currentNeuron->firstNeuronInFrontLayer = newNeuron;
				currentNeuron = currentNeuron->firstNeuronInFrontLayer;
			}
		}
		else
		{
			cout << "parseSubnetTag error: layer tag not detected" << endl;
		}
		currentTag=currentTag->nextTag;
	}
	recordOfFirstNeuronInPreviousLayer ->firstNeuronInFrontLayer=NULL;	//to stop an additional layer being added

	return result;
}

bool ANNxmlConversionClass::parseLayerTag(XMLparserTag* firstTagInLayer, ANNneuron* firstNeuronInLayer, long layerIDcounter, long orderIDcounter, long* wrongAndNotUsedIDcounter, long subnetIDcounter)
{
	bool result = true;

	XMLparserTag* currentTag = firstTagInLayer;
	ANNneuron* currentNeuron = firstNeuronInLayer;

	while(currentTag->nextTag != NULL)
	{
		if(currentTag->name == NET_XML_TAG_neuronContainer)
		{
			if(!this->parseNeuronContainerTag(currentTag->firstLowerLevelTag, currentNeuron, layerIDcounter, orderIDcounter++, wrongAndNotUsedIDcounter, subnetIDcounter))
			{
				result = false;
			}
			*wrongAndNotUsedIDcounter = *wrongAndNotUsedIDcounter + 1;

			ANNneuron* newNeuron = new ANNneuron();
			currentNeuron->nextNeuron = newNeuron;
			currentNeuron = currentNeuron->nextNeuron;
		}
		else
		{
			cout << "parseLayerTag error: neuron container tag not detected" << endl;
		}
		currentTag=currentTag->nextTag;
	}
	return result;
}


bool ANNxmlConversionClass::parseNeuronContainerTag(XMLparserTag* firstTagInNeuronContainer, ANNneuron* currentNeuron, long layerIDcounter, long orderIDcounter, long* wrongAndNotUsedIDcounter, long subnetIDcounter)
{
	bool result = true;

	XMLparserTag* currentTag = firstTagInNeuronContainer;

	if(currentTag->name == NET_XML_TAG_neuron)
	{
		if(!this->parseNeuronTag(currentTag, currentNeuron, layerIDcounter, orderIDcounter, wrongAndNotUsedIDcounter, subnetIDcounter))
		{
			result = false;
		}
		currentTag=currentTag->nextTag;
	}
	else
	{
		cout << "parseNeuronContainerTag error: no neuron tag detected" << endl;
	}

	if(currentTag->name == NET_XML_TAG_forwardANNneuronConnectionsList)
	{
		if(!this->parseForwardANNneuronConnectionsListTag(currentTag->firstLowerLevelTag, currentNeuron))
		{
			result = false;
		}
		currentTag=currentTag->nextTag;
	}
	else
	{
		//NB there may not be a forwardANNneuronConnectionsList
	}

	#ifdef ANN_SUBNETS
	if(currentTag->name == NET_XML_TAG_subnet)
	{
		ANNneuron* newNeuron = new ANNneuron();
		currentNeuron->firstNeuronInBackLayerOfSubnet = newNeuron;
		if(!this->parseSubnetTag(currentTag->firstLowerLevelTag, currentNeuron->firstNeuronInBackLayerOfSubnet, 1, wrongAndNotUsedIDcounter, (subnetIDcounter+1)))
		{
			result = false;
		}
		currentTag=currentTag->nextTag;
	}
	#endif

	if(currentTag->nextTag != NULL)
	{
		cout << "parseNeuronContainerTag error: end of content not detected" << endl;
	}

	return result;
}



bool ANNxmlConversionClass::parseForwardANNneuronConnectionsListTag(XMLparserTag* firstTagInForwardANNneuronConnectionsList, ANNneuron* currentNeuron)
{
	bool result = true;

	XMLparserTag* currentTag = firstTagInForwardANNneuronConnectionsList;

	while(currentTag->nextTag != NULL)
	{
		//create a new neuron connection
		ANNneuronConnection* newANNneuronConnection = new ANNneuronConnection();

		if(!this->parseForwardANNneuronConnectionTag(currentTag, newANNneuronConnection))
		{
			result = false;
		}

		//newANNneuronConnection->backNeuron = currentNeuron;	//redundant; will be added later
		currentNeuron->frontANNneuronConnectionList.push_back(newANNneuronConnection);

		currentTag = currentTag->nextTag;
	}

	return result;

}


bool ANNxmlConversionClass::parseForwardANNneuronConnectionTag(XMLparserTag* currentTag, ANNneuronConnection* currentANNneuronConnection)
{
	bool result = true;

	XMLparserAttribute* currentAttribute = currentTag->firstAttribute;

	bool neuronIDFound = false;
	bool weightFound = false;
	bool idealValueFound = false;

	while(currentAttribute->nextAttribute != NULL)
	{
		if(currentAttribute->name == NET_XML_ATTRIBUTE_neuronID)
		{
			currentANNneuronConnection->frontNeuronID = SHAREDvars.convertStringToDouble(currentAttribute->value);		//temporary variable used to link neuron connections at a later stage
			neuronIDFound = true;
		}
		else if(currentAttribute->name == NET_XML_ATTRIBUTE_weight)
		{
			currentANNneuronConnection->weight = SHAREDvars.convertStringToDouble(currentAttribute->value);
			weightFound = true;
		}
		#ifdef ANN_ALGORITHM_CLASSIFICATION_NETWORK
		else if(currentAttribute->name == NET_XML_ATTRIBUTE_idealValue)
		{
			currentANNneuronConnection->idealValue = SHAREDvars.convertStringToDouble(currentAttribute->value);
			idealValueFound = true;
		}
		#endif

		currentAttribute = currentAttribute->nextAttribute;
	}

	if(!neuronIDFound)
	{
		cout << "parseForwardANNneuronConnectionsListTag error: frontNeuronID attribute not detected" << endl;
		result = false;
	}
	#ifdef ENFORCE_EXPLICIT_NET_XML_NEURON_KEYPROPERTIES_PARAMETERS
	if(!weightFound)
	{
		cout << "parseForwardANNneuronConnectionsListTag error: weight attribute not detected" << endl;
		result = false;
	}
	#endif

	return result;

}





bool ANNxmlConversionClass::parseNeuronTag(XMLparserTag* currentTag, ANNneuron* currentNeuron, long layerIDcounter, long orderIDcounter, long* wrongAndNotUsedIDcounter, long subnetIDcounter)
{
	bool result = true;

	XMLparserAttribute* currentAttribute = currentTag->firstAttribute;

	bool idFound = false;
	bool orderIDFound = false;
	bool layerIDFound = false;
	bool subnetIDFound = false;
	bool biasFound = false;
	bool outputFound = false;
	bool classTargetFound = false;
	bool errorFound = false;

	//incase network is never trained, but output for visualisation purposes, set these values to dummy values
	currentNeuron->bias = 0;
	currentNeuron->output = 0;
	currentNeuron->classTarget = 0;
	currentNeuron->error = 0;

	while(currentAttribute->nextAttribute != NULL)
	{
		if(currentAttribute->name == NET_XML_ATTRIBUTE_id)
		{
			long attributeValue = SHAREDvars.convertStringToDouble(currentAttribute->value);
			//always use XML file's neuron id as this has been generated in a different order than wrongAndNotUsedIDcounter
			//#ifndef DO_NOT_REGENERATE_NEURON_IDS_WHEN_PARSING_NET_XML

			currentNeuron->id = attributeValue;

			idFound = true;
		}
		else if(currentAttribute->name == NET_XML_ATTRIBUTE_orderID)
		{
			long attributeValue = SHAREDvars.convertStringToDouble(currentAttribute->value);
			if(attributeValue != orderIDcounter)
			{
				cout << "parseNeuronTag error: attributeValue != orderIDcounter, attributeValue = " << attributeValue << ", orderIDcounter = " << subnetIDcounter << endl;
				result = false;
			}
			currentNeuron->orderID = attributeValue;
			orderIDFound = true;
		}
		else if(currentAttribute->name == NET_XML_ATTRIBUTE_layerID)
		{
			long attributeValue = SHAREDvars.convertStringToDouble(currentAttribute->value);
			if(attributeValue != layerIDcounter)
			{
				cout << "parseNeuronTag error: attributeValue != layerIDcounter, attributeValue = " << attributeValue << ", layerIDcounter = " << subnetIDcounter << endl;
				result = false;
			}
			currentNeuron->layerID = attributeValue;
			layerIDFound = true;
		}
		else if(currentAttribute->name == NET_XML_ATTRIBUTE_subnetID)
		{
			long attributeValue = SHAREDvars.convertStringToDouble(currentAttribute->value);
			if(attributeValue != subnetIDcounter)
			{
				cout << "parseNeuronTag error: attributeValue != subnetIDcounter, attributeValue = " << attributeValue << ", subnetIDcounter = " << subnetIDcounter << endl;
				result = false;
			}
			currentNeuron->subnetID = attributeValue;
			subnetIDFound = true;
		}
		else if(currentAttribute->name == NET_XML_ATTRIBUTE_bias)
		{
			double attributeValue = SHAREDvars.convertStringToDouble(currentAttribute->value);
			currentNeuron->bias = attributeValue;
			biasFound = true;
		}
		else if(currentAttribute->name == NET_XML_ATTRIBUTE_output)
		{
			double attributeValue = SHAREDvars.convertStringToDouble(currentAttribute->value);
			currentNeuron->output = attributeValue;
			outputFound = true;
		}
		else if(currentAttribute->name == NET_XML_ATTRIBUTE_classTarget)
		{
			double attributeValue = SHAREDvars.convertStringToDouble(currentAttribute->value);
			currentNeuron->classTarget = attributeValue;
			classTargetFound = true;
		}
		else if(currentAttribute->name == NET_XML_ATTRIBUTE_error)
		{
			double attributeValue = SHAREDvars.convertStringToDouble(currentAttribute->value);
			currentNeuron->error = attributeValue;
			errorFound = true;
		}
		#ifdef ANN_ALGORITHM_CLASSIFICATION_NETWORK
		else if(currentAttribute->name == NET_XML_ATTRIBUTE_memoryTrace)
		{
			double attributeValue = SHAREDvars.convertStringToDouble(currentAttribute->value);
			currentNeuron->memoryTrace = attributeValue;
			errorFound = true;
		}
		#endif
		else if(currentAttribute->name == NET_XML_ATTRIBUTE_xPosRel)
		{
			currentNeuron->spatialCoordinatesSet2D = true;
			long attributeValue = SHAREDvars.convertStringToDouble(currentAttribute->value);
			currentNeuron->xPosRel = attributeValue;
		}
		else if(currentAttribute->name == NET_XML_ATTRIBUTE_yPosRel)
		{
			currentNeuron->spatialCoordinatesSet2D = true;
			long attributeValue = SHAREDvars.convertStringToDouble(currentAttribute->value);
			currentNeuron->yPosRel = attributeValue;
		}
		else if(currentAttribute->name == NET_XML_ATTRIBUTE_zPosRel)
		{
			currentNeuron->spatialCoordinatesSet2D = true;
			long attributeValue = SHAREDvars.convertStringToDouble(currentAttribute->value);
			currentNeuron->zPosRel = attributeValue;
		}
		#ifdef ANN_ALGORITHM_CLASSIFICATION_NETWORK
		else if(currentAttribute->name == NET_XML_ATTRIBUTE_xPosRelFrac)
		{
			currentNeuron->spatialCoordinatesSetClassification = true;
			double attributeValue = SHAREDvars.convertStringToDouble(currentAttribute->value);
			currentNeuron->xPosRelFrac = attributeValue;
		}
		else if(currentAttribute->name == NET_XML_ATTRIBUTE_yPosRelFrac)
		{
			currentNeuron->spatialCoordinatesSetClassification = true;
			double attributeValue = SHAREDvars.convertStringToDouble(currentAttribute->value);
			currentNeuron->yPosRelFrac = attributeValue;
		}
		else if(currentAttribute->name == NET_XML_ATTRIBUTE_zPosRelFrac)
		{
			currentNeuron->spatialCoordinatesSetClassification = true;
			double attributeValue = SHAREDvars.convertStringToDouble(currentAttribute->value);
			currentNeuron->zPosRelFrac = attributeValue;
		}
		#endif
		else
		{
			cout << "parseNeuronTag error: illegal attribute name detected";
		}

		currentAttribute = currentAttribute->nextAttribute;
	}

	if(!idFound)
	{
		cout << "parseNeuronTag error: neuron attribute 'id' not detected";
		result = false;
	}

	#ifdef ENFORCE_EXPLICIT_NET_XML_NEURON_ID_PARAMETERS
	if(!orderIDFound || !layerIDFound || !subnetIDFound)
	{
		cout << "parseNeuronTag error: NEURON_ID_PARAMETERS not detected (ENFORCE_EXPLICIT_NET_XML_NEURON_ID_PARAMETERS)";
		result = false;
	}
	#else
	if(!orderIDFound)
	{
		currentNeuron->orderID = orderIDcounter;
	}
	if(!layerIDFound)
	{
		currentNeuron->layerID = layerIDcounter;
	}
	if(!subnetIDFound)
	{
		currentNeuron->subnetID = subnetIDcounter;
	}
	#endif

	#ifdef ENFORCE_EXPLICIT_NET_XML_NEURON_KEYPROPERTIES_PARAMETERS
	if(!biasFound || !outputFound || !classTargetFound || !errorFound)
	{
		cout << "parseNeuronTag error: NEURON_KEYPROPERTIES_PARAMETERS not detected (ENFORCE_EXPLICIT_NET_XML_NEURON_KEYPROPERTIES_PARAMETERS)";
		result = false;
	}
	#endif
	return result;
}

void ANNxmlConversionClass::clearTag(XMLparserTag* tag)
{
	delete (tag->nextTag);
	tag->nextTag = NULL;
	delete (tag->firstLowerLevelTag);
	tag->firstLowerLevelTag = NULL;
}



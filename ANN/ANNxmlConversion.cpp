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
 * File Name: ANNxmlConversion.c
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2012 Baxter AI (baxterai.com)
 * Project: Artificial Neural Network (ANN)
 * Project Version: 3c3h 19-November-2012
 * Comments
 *
 *******************************************************************************/



#include "ANNxmlConversion.h"

//this function works and no longer needs to be tested
#ifdef NN_XML_DEBUG
bool testReadNetXMLFile()
{
	bool result = true;

	NeuronContainer * firstInputNeuronInNetwork = new NeuronContainer();	//the firstInputNeuronInNetwork object must be initialised here (in ANNxmlConversion.cpp scope). if it is initialised in another .cpp it will be come corrupted,

	if(!readNetXMLfile(NET_XML_FILE_NAME, firstInputNeuronInNetwork))
	{
		result = false;
	}

	#define TEMP_NET_XML_FILE_NAME1 "tempnet1.xml"
	if(!writeNetXMLfile(TEMP_NET_XML_FILE_NAME1, firstInputNeuronInNetwork))
	{
		result = false;
	}
	return result;
}

bool testReadNetXMLFile2(NeuronContainer * firstInputNeuronInNetwork)
{
	bool result = true;

	if(!writeNetXMLfile(NET_XML_FILE_NAME, firstInputNeuronInNetwork))
	{
		result = false;
	}

	NeuronContainer * tempFirstInputNeuronInNetwork = new NeuronContainer();
	if(!readNetXMLfile(NET_XML_FILE_NAME, tempFirstInputNeuronInNetwork))
	{
		result = false;
	}

	#define TEMP_NET_XML_FILE_NAME1 "tempnet1.xml"
	if(!writeNetXMLfile(TEMP_NET_XML_FILE_NAME1, tempFirstInputNeuronInNetwork))
	{
		result = false;
	}
	return result;
}

#endif

NeuronContainer * readNetXMLfileAndRecordFormationVariables(string xmlFileName, NeuronContainer * firstInputNeuronInNetwork, long * numberOfInputNeurons, long * numberOfOutputNeurons)
{
	NeuronContainer * firstOutputNeuronInNetwork;

	readNetXMLfile(xmlFileName, firstInputNeuronInNetwork);

	firstOutputNeuronInNetwork = recordOutputNeuronAndNumInputAndOutputNeuronsInNetwork(firstInputNeuronInNetwork, numberOfInputNeurons, numberOfOutputNeurons);

	return firstOutputNeuronInNetwork;
}

NeuronContainer * recordOutputNeuronAndNumInputAndOutputNeuronsInNetwork(NeuronContainer * firstInputNeuronInNetwork, long * numberOfInputNeurons, long * numberOfOutputNeurons)
{
	NeuronContainer * firstOutputNeuronInNetwork;

	long networkLayer = 1;

	NeuronContainer * currentNeuron = firstInputNeuronInNetwork;

	bool stillMoreLayers = true;
	while(stillMoreLayers)
	{
		bool currentLayerHasFrontLayer = false;

		NeuronContainer * firstNeuronInLayer = currentNeuron;

		bool numNeuronsInCurrentLayer = 0;
		while(currentNeuron->nextNeuronContainer != NULL)
		{
			numNeuronsInCurrentLayer++;
			currentNeuron = currentNeuron->nextNeuronContainer;
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
			//cout << "firstOutputNeuronInNetwork ID = " << firstOutputNeuronInNetwork->neuron->id << endl;
			stillMoreLayers = false;
		}
	}

	return firstOutputNeuronInNetwork;
}


bool readNetXMLfile(string xmlFileName, NeuronContainer * firstInputNeuronInNetwork)
{
	bool result = true;

 	XMLparserTag * firstTagInXMLFile = new XMLparserTag();	//the firstTagInXMLFile object must be initialised here (in ANNxmlConversion.cpp scope). if it is initialised in XMLparserClass.cpp else it will be come corrupted,

 	if(!readXMLfile(xmlFileName, firstTagInXMLFile))
 	{
		result = false;
	}

	if(!parseNetTag(firstTagInXMLFile, firstInputNeuronInNetwork))
	{
		result = false;
	}

	if(!linkLayerXNeuronsBasedUponFrontNeuronConnectionContainerListNeuronIDs(firstInputNeuronInNetwork, firstInputNeuronInNetwork, false, NULL))
	{
		result = false;
	}
	return result;
}

bool writeNetXMLfile(string xmlFileName, NeuronContainer * firstInputNeuronInNetwork)
{
	bool result = true;

 	XMLparserTag * firstTagInXMLFile = new XMLparserTag();	//the firstTagInXMLFile object must be initialised here (in ANNxmlConversion.cpp scope). if it is initialised in XMLparserClass.cpp else it will be come corrupted,

	XMLparserTag * currentTagL0 = firstTagInXMLFile;
	currentTagL0->name = NET_XML_TAG_network;
	XMLparserTag * newTag0 = new XMLparserTag();	//had to add a null tag
	currentTagL0->nextTag = newTag0;

	XMLparserTag * firstTagL1 = new XMLparserTag();
	currentTagL0->firstLowerLevelTag = firstTagL1;
	XMLparserTag * currentTagL1 = currentTagL0->firstLowerLevelTag;
	XMLparserTag * newTag1 = new XMLparserTag();	//had to add a null tag
	currentTagL1->nextTag = newTag1;

	currentTagL1->name = NET_XML_TAG_subnet;
	XMLparserTag * firstTagL2 = new XMLparserTag();
	currentTagL1->firstLowerLevelTag = firstTagL2;
	XMLparserTag * currentTagL2 = currentTagL1->firstLowerLevelTag;
	XMLparserTag * newTag2 = new XMLparserTag();	//had to add a null tag
	currentTagL2->nextTag = newTag2;

	if(!generateXMLtagListBasedUponSubnet(currentTagL1->firstLowerLevelTag, firstInputNeuronInNetwork))
	{
		result = false;
	}

 	if(!writeXMLfile(xmlFileName, firstTagInXMLFile))
 	{
		result = false;
	}

	delete firstTagInXMLFile;

	return result;
}


bool generateXMLtagListBasedUponSubnet(XMLparserTag * firstTagInSubnet, NeuronContainer * firstNeuronContainerInSubnet)
{
	bool result = true;

	XMLparserTag * currentTagL0 = firstTagInSubnet;
	NeuronContainer * currentNeuron = firstNeuronContainerInSubnet;

	bool stillMoreLayers = true;
	while(stillMoreLayers)
	{
		bool currentLayerHasFrontLayer = false;

		NeuronContainer * firstNeuronInLayer = currentNeuron;

		currentTagL0->name = NET_XML_TAG_layer;
		XMLparserTag * newTag0 = new XMLparserTag();	//had to add a null tag
		currentTagL0->nextTag = newTag0;

		XMLparserTag * firstTagL1 = new XMLparserTag();
		currentTagL0->firstLowerLevelTag = firstTagL1;
		XMLparserTag * currentTagL1 = currentTagL0->firstLowerLevelTag;
		XMLparserTag * newTag1 = new XMLparserTag();	//had to add a null tag
		currentTagL1->nextTag = newTag1;

		while(currentNeuron->nextNeuronContainer != NULL)
		{
			char tempString[MAX_ATTRIBUTE_VALUE_SIZE];

			//generate neuron connection tag
			currentTagL1->name = NET_XML_TAG_neuronContainer;
			XMLparserTag * firstTagL2 = new XMLparserTag();
			currentTagL1->firstLowerLevelTag = firstTagL2;
			XMLparserTag * currentTagL2 = currentTagL1->firstLowerLevelTag;

			//generate neuron tag
			currentTagL2->name = NET_XML_TAG_neuron;

			XMLParserAttribute * currentAttribute = currentTagL2->firstAttribute;

			currentAttribute->name = NET_XML_ATTRIBUTE_id;
			sprintf(tempString, "%d", (currentNeuron->neuron->id));
			currentAttribute->value = tempString;

			XMLParserAttribute * newAttribute1 = new XMLParserAttribute();
			currentAttribute->nextAttribute = newAttribute1;
			currentAttribute = currentAttribute->nextAttribute;

		#ifndef DO_NOT_STORE_NET_XML_NEURON_ID_PARAMETERS

			currentAttribute->name = NET_XML_ATTRIBUTE_layerID;
			sprintf(tempString, "%d", (currentNeuron->neuron->layerID));
			currentAttribute->value = tempString;

			XMLParserAttribute * newAttribute2 = new XMLParserAttribute();
			currentAttribute->nextAttribute = newAttribute2;
			currentAttribute = currentAttribute->nextAttribute;

			currentAttribute->name = NET_XML_ATTRIBUTE_orderID;
			sprintf(tempString, "%d", (currentNeuron->neuron->orderID));
			currentAttribute->value = tempString;

			XMLParserAttribute * newAttribute3 = new XMLParserAttribute();
			currentAttribute->nextAttribute = newAttribute3;
			currentAttribute = currentAttribute->nextAttribute;

			currentAttribute->name = NET_XML_ATTRIBUTE_subnetID;
			sprintf(tempString, "%d", (currentNeuron->neuron->subnetID));
			currentAttribute->value = tempString;

			XMLParserAttribute * newAttribute4 = new XMLParserAttribute();
			currentAttribute->nextAttribute = newAttribute4;
			currentAttribute = currentAttribute->nextAttribute;
		#endif

		#ifndef DO_NOT_STORE_NET_XML_NEURON_KEYPROPERTIES_PARAMETERS

			currentAttribute->name = NET_XML_ATTRIBUTE_bias;
			sprintf(tempString, "%0.6f", (currentNeuron->neuron->bias));
			currentAttribute->value = tempString;

			XMLParserAttribute * newAttribute5 = new XMLParserAttribute();
			currentAttribute->nextAttribute = newAttribute5;
			currentAttribute = currentAttribute->nextAttribute;

			currentAttribute->name = NET_XML_ATTRIBUTE_output;
			sprintf(tempString, "%0.6f", (currentNeuron->neuron->output));
			currentAttribute->value = tempString;

			XMLParserAttribute * newAttribute6 = new XMLParserAttribute();
			currentAttribute->nextAttribute = newAttribute6;
			currentAttribute = currentAttribute->nextAttribute;

			currentAttribute->name = NET_XML_ATTRIBUTE_classTarget;
			sprintf(tempString, "%0.6f", (currentNeuron->neuron->classTarget));
			currentAttribute->value = tempString;

			XMLParserAttribute * newAttribute7 = new XMLParserAttribute();
			currentAttribute->nextAttribute = newAttribute7;
			currentAttribute = currentAttribute->nextAttribute;

			currentAttribute->name = NET_XML_ATTRIBUTE_error;
			sprintf(tempString, "%0.6f", (currentNeuron->neuron->error));
			currentAttribute->value = tempString;

			XMLParserAttribute * newAttribute8 = new XMLParserAttribute();
			currentAttribute->nextAttribute = newAttribute8;
			currentAttribute = currentAttribute->nextAttribute;
		#endif

		#ifndef DO_NOT_STORE_NET_XML_NEURON_SPATIAL_COORD_PARAMETERS

			if(currentNeuron->neuron->spatialCoordinatesSet)
			{

				currentAttribute->name = NET_XML_ATTRIBUTE_xPosRel;
				sprintf(tempString, "%d", (currentNeuron->neuron->xPosRel));
				currentAttribute->value = tempString;

				XMLParserAttribute * newAttribute9 = new XMLParserAttribute();
				currentAttribute->nextAttribute = newAttribute9;
				currentAttribute = currentAttribute->nextAttribute;

				currentAttribute->name = NET_XML_ATTRIBUTE_yPosRel;
				sprintf(tempString, "%d", (currentNeuron->neuron->yPosRel));
				currentAttribute->value = tempString;

				XMLParserAttribute * newAttribute10 = new XMLParserAttribute();
				currentAttribute->nextAttribute = newAttribute10;
				currentAttribute = currentAttribute->nextAttribute;

				currentAttribute->name = NET_XML_ATTRIBUTE_zPosRel;
				sprintf(tempString, "%d", (currentNeuron->neuron->zPosRel));
				currentAttribute->value = tempString;

				XMLParserAttribute * newAttribute11 = new XMLParserAttribute();
				currentAttribute->nextAttribute = newAttribute11;
				//currentAttribute = currentAttribute->nextAttribute;
			}
		#endif

			XMLparserTag * newTag = new XMLparserTag();
			currentTagL2->nextTag = newTag;
			currentTagL2 = currentTagL2->nextTag;

			//generate NET_XML_TAG_forwardNeuronConnectionsList tag
			if(currentNeuron -> firstFrontNeuronConnectionContainer -> nextNeuronConnectionContainer != NULL)
			{
				currentTagL2->name = NET_XML_TAG_forwardNeuronConnectionsList;
				XMLparserTag * firstTagL3 = new XMLparserTag();
				currentTagL2->firstLowerLevelTag = firstTagL3;
				XMLparserTag * currentTagL3 = currentTagL2->firstLowerLevelTag;
				NeuronConnectionContainer * currentNeuronConnectionContainer = currentNeuron->firstFrontNeuronConnectionContainer;
				while(currentNeuronConnectionContainer -> nextNeuronConnectionContainer != NULL)
				{
					//generate NET_XML_TAG_forwardNeuronConnection tag and attributes
					currentTagL3->name = NET_XML_TAG_forwardNeuronConnection;

					XMLParserAttribute * currentAttribute = currentTagL3->firstAttribute;

					currentAttribute->name = "neuronID";
					sprintf(tempString, "%d", (currentNeuronConnectionContainer->neuron->id));
					currentAttribute->value = tempString;

					XMLParserAttribute * newAttribute1 = new XMLParserAttribute();
					currentAttribute->nextAttribute = newAttribute1;
					currentAttribute = currentAttribute->nextAttribute;

				#ifndef DO_NOT_STORE_NET_XML_NEURON_KEYPROPERTIES_PARAMETERS
					currentAttribute->name = "weight";
					sprintf(tempString, "%0.6f", (currentNeuronConnectionContainer->neuronConnection->weight));
					currentAttribute->value = tempString;

					XMLParserAttribute * newAttribute2 = new XMLParserAttribute();
					currentAttribute->nextAttribute = newAttribute2;
					//currentAttribute = currentAttribute->nextAttribute;
				#endif

					XMLparserTag * newTag = new XMLparserTag();
					currentTagL3->nextTag = newTag;
					currentTagL3 = currentTagL3->nextTag;

					currentNeuronConnectionContainer = currentNeuronConnectionContainer -> nextNeuronConnectionContainer;
				}
				XMLparserTag * newTag = new XMLparserTag();
				currentTagL2->nextTag = newTag;
			}

		#ifdef ANN_ADVANCED
			currentTagL2 = currentTagL2->nextTag;

			//generate subtag tag
			if(currentNeuron -> isSubnet)
			{
				currentTagL2->name = NET_XML_TAG_subnet;

				XMLparserTag * newTag1 = new XMLparserTag();
				currentTagL2->firstLowerLevelTag = newTag1;
				if(!generateXMLtagListBasedUponSubnet(currentTagL2->firstLowerLevelTag, currentNeuron->firstNeuronContainerInBackLayerOfSubnet))
				{
					result = false;
				}

				XMLparserTag * newTag = new XMLparserTag();
				currentTagL2->nextTag = newTag;
				//currentTagL2 = currentTagL2->nextTag;
			}
		#endif

			XMLparserTag * newTag2 = new XMLparserTag();
			currentTagL1->nextTag = newTag2;
			currentTagL1 = currentTagL1->nextTag;

			currentNeuron = currentNeuron->nextNeuronContainer;
		}

		if(firstNeuronInLayer->hasFrontLayer)
		{
			currentNeuron=firstNeuronInLayer->firstNeuronInFrontLayer;
		}
		else
		{
			stillMoreLayers = false;
		}

		XMLparserTag * newTag = new XMLparserTag();
		currentTagL0->nextTag = newTag;
		currentTagL0 = currentTagL0->nextTag;
	}

	return result;
}


bool linkLayerXNeuronsBasedUponFrontNeuronConnectionContainerListNeuronIDs(NeuronContainer * firstNeuronContainerInLayer, NeuronContainer * firstInputNeuronInNetwork, bool hasBackLayer, NeuronContainer * firstNeuronContainerInBackLayer)
{
	bool result = true;

	NeuronContainer * currentNeuron = firstNeuronContainerInLayer;
	while(currentNeuron->nextNeuronContainer != NULL)
	{
		NeuronConnectionContainer * currentNeuronConnectionContainer = currentNeuron->firstFrontNeuronConnectionContainer;
		while(currentNeuronConnectionContainer -> nextNeuronConnectionContainer != NULL)
		{
			bool tempResult = false;

			NeuronContainer * neuronToConnect = findNeuronContainer(firstInputNeuronInNetwork, currentNeuronConnectionContainer->neuronID, &tempResult);
			if(tempResult)
			{
				//link neurons
				currentNeuronConnectionContainer->neuron = neuronToConnect->neuron;
				neuronToConnect->currentBackNeuronConnectionContainer->neuron = currentNeuron->neuron;		//OLD: 				neuronToConnect->currentBackNeuronConnectionContainer->neuron = currentNeuronConnectionContainer->neuron;
				neuronToConnect->currentBackNeuronConnectionContainer->neuronConnection = currentNeuronConnectionContainer->neuronConnection;

				NeuronConnectionContainer * newNeuronConnectionContainer = new NeuronConnectionContainer();
				neuronToConnect->currentBackNeuronConnectionContainer->nextNeuronConnectionContainer = newNeuronConnectionContainer;
				neuronToConnect->currentBackNeuronConnectionContainer = neuronToConnect->currentBackNeuronConnectionContainer->nextNeuronConnectionContainer;
			}
			else
			{
				cout << "linkNeuronsBasedUponConnectionIDs error: neuron in neuron connection list not found. neuron id being searched for = " << currentNeuronConnectionContainer->neuronID <<  endl;
				result = false;
			}

			currentNeuronConnectionContainer = currentNeuronConnectionContainer -> nextNeuronConnectionContainer;
		}

		if(firstNeuronContainerInLayer->firstNeuronInFrontLayer != NULL)
		{
			currentNeuron->hasFrontLayer = true;
			currentNeuron->firstNeuronInFrontLayer = firstNeuronContainerInLayer->firstNeuronInFrontLayer;
		}
		if(hasBackLayer)
		{
			currentNeuron->hasBackLayer = true;
			currentNeuron->firstNeuronInBackLayer = firstNeuronContainerInBackLayer;
		}

	#ifdef ANN_ADVANCED
		if(currentNeuron->firstNeuronContainerInBackLayerOfSubnet != NULL)
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

			if(!linkLayerXNeuronsBasedUponFrontNeuronConnectionContainerListNeuronIDs(currentNeuron->firstNeuronContainerInBackLayerOfSubnet, firstInputNeuronInNetwork, false, NULL))
			{
				result = false;
			}

			NeuronContainer * firstOutputNeuronInSubnet;
			long temp;
			firstOutputNeuronInSubnet = recordOutputNeuronAndNumInputAndOutputNeuronsInNetwork(currentNeuron->firstNeuronContainerInBackLayerOfSubnet, &temp, &temp);
			currentNeuron->firstNeuronContainerInFrontLayerOfSubnet	= firstOutputNeuronInSubnet;
		}
	#endif

		currentNeuron = currentNeuron -> nextNeuronContainer;
	}
	if(firstNeuronContainerInLayer->hasFrontLayer)
	{
		if(!linkLayerXNeuronsBasedUponFrontNeuronConnectionContainerListNeuronIDs(firstNeuronContainerInLayer->firstNeuronInFrontLayer, firstInputNeuronInNetwork, true, firstNeuronContainerInLayer))
		{
			result = false;
		}
	}

	return result;
}


NeuronContainer * findNeuronContainer(NeuronContainer * firstNeuronContainerInLayer, long neuronIDtoFind, bool * result)
{
	NeuronContainer * foundNeuronContainer = NULL;

	NeuronContainer * currentNeuron = firstNeuronContainerInLayer;
	while(currentNeuron->nextNeuronContainer != NULL)
	{
		if(currentNeuron->neuron->id == neuronIDtoFind)
		{
			//cout << "currentNeuron->neuron->id  = " << currentNeuron->neuron->id << endl;
			*result = true;
			foundNeuronContainer = currentNeuron;
		}
	#ifdef ANN_ADVANCED
		if(currentNeuron->firstNeuronContainerInBackLayerOfSubnet != NULL)
		{
			bool tempResult = false;
			NeuronContainer * tempFoundNeuronContainer;
			tempFoundNeuronContainer = findNeuronContainer(currentNeuron->firstNeuronContainerInBackLayerOfSubnet, neuronIDtoFind, &tempResult);
			if(tempResult)
			{
				*result = true;
				foundNeuronContainer = tempFoundNeuronContainer;
			}
		}
	#endif
		currentNeuron = currentNeuron -> nextNeuronContainer;
	}

	if(firstNeuronContainerInLayer->firstNeuronInFrontLayer != NULL)
	{
		bool tempResult = false;
		NeuronContainer * tempFoundNeuronContainer;
		tempFoundNeuronContainer = findNeuronContainer(firstNeuronContainerInLayer->firstNeuronInFrontLayer, neuronIDtoFind, &tempResult);
		if(tempResult)
		{
			*result = true;
			foundNeuronContainer = tempFoundNeuronContainer;
		}
	}

	return foundNeuronContainer;
}


//Top Level
bool parseNetTag(XMLparserTag * firstTagInNetwork, NeuronContainer * currentNeuron)
{
	bool result = true;

	XMLparserTag * currentTagUpdatedL1 = firstTagInNetwork;

	if(currentTagUpdatedL1->name == NET_XML_TAG_network)
	{
		currentTagUpdatedL1 = currentTagUpdatedL1->firstLowerLevelTag;
	}
	else
	{
		cout << "parseNetTag error: network tag not detected";
		result = false;
	}

	if(result)
	{
		if(currentTagUpdatedL1->name == NET_XML_TAG_iodescriptions)
		{
			XMLparserTag * currentTagUpdatedL2 = currentTagUpdatedL1->firstLowerLevelTag;
			if(currentTagUpdatedL2->name == NET_XML_TAG_inputNeurons)
			{
				XMLparserTag * currentTagUpdatedL3 = currentTagUpdatedL2->firstLowerLevelTag;
				while(currentTagUpdatedL3->nextTag != NULL)
				{
					if(currentTagUpdatedL3->name == NET_XML_TAG_description)
					{
						cout << "Input Neuron Description: " << currentTagUpdatedL3->value << endl;
					}
					else
					{
						cout << "parseInputNeuronsDescriptionTag error: description tag not detected";
					}
					currentTagUpdatedL3=currentTagUpdatedL3->nextTag;
				}

			}
			else
			{
				cout << "parseIODescriptionsTag error: no inputNeurons tag detected";
			}
			currentTagUpdatedL2=currentTagUpdatedL2->nextTag;
			if(currentTagUpdatedL2->name == NET_XML_TAG_outputNeurons)
			{
				XMLparserTag * currentTagUpdatedL3 = currentTagUpdatedL2->firstLowerLevelTag;
				while(currentTagUpdatedL3->nextTag != NULL)
				{
					if(currentTagUpdatedL3->name == NET_XML_TAG_description)
					{
						cout << "Output Neuron Description: " << currentTagUpdatedL3->value << endl;
					}
					else
					{
						cout << "parseOutputNeuronsDescriptionTag error: description tag not detected";
					}
					currentTagUpdatedL3=currentTagUpdatedL3->nextTag;
				}
			}
			else
			{
				cout << "parseIODescriptionsTag error: no outputNeurons tag detected";
			}
			currentTagUpdatedL1=currentTagUpdatedL1->nextTag;
		}
		if(currentTagUpdatedL1->name == NET_XML_TAG_subnet)
		{
			long wrongAndNotUsedIDcounter = 1;
			if(!parseSubnetTag(currentTagUpdatedL1->firstLowerLevelTag, currentNeuron, 1, &wrongAndNotUsedIDcounter, 1))
			{
				result = false;
			}
		}
		else
		{
			cout << "parseNetTag error: no subnet tag detected";
		}
	}

	return result;
}

bool parseSubnetTag(XMLparserTag * firstTagInSubnet, NeuronContainer * firstNeuronInSubnet, long layerIDcounter, long * wrongAndNotUsedIDcounter, long subnetIDcounter)
{
	bool result = true;

	NeuronContainer * currentNeuron = firstNeuronInSubnet;

	XMLparserTag * currentTag = firstTagInSubnet;

	NeuronContainer * recordOfFirstNeuronInPreviousLayer;	//to stop an additional layer being added

	while(currentTag->nextTag != NULL)
	{
		if(currentTag->name == NET_XML_TAG_layer)
		{
			if(!parseLayerTag(currentTag->firstLowerLevelTag, currentNeuron, layerIDcounter++, 1, wrongAndNotUsedIDcounter, subnetIDcounter))
			{
				result = false;
			}
			else
			{
				recordOfFirstNeuronInPreviousLayer = currentNeuron;	//to stop an additional layer being added
				NeuronContainer * newNeuron = new NeuronContainer();
				currentNeuron->firstNeuronInFrontLayer = newNeuron;
				currentNeuron = currentNeuron->firstNeuronInFrontLayer;
			}
		}
		else
		{
			cout << "parseSubnetTag error: layer tag not detected";
		}
		currentTag=currentTag->nextTag;
	}
	recordOfFirstNeuronInPreviousLayer ->firstNeuronInFrontLayer=NULL;	//to stop an additional layer being added

	return result;
}

bool parseLayerTag(XMLparserTag * firstTagInLayer, NeuronContainer * firstNeuronInLayer, long layerIDcounter, long orderIDcounter, long * wrongAndNotUsedIDcounter, long subnetIDcounter)
{
	bool result = true;

	XMLparserTag * currentTag = firstTagInLayer;
	NeuronContainer * currentNeuron = firstNeuronInLayer;

	while(currentTag->nextTag != NULL)
	{
		if(currentTag->name == NET_XML_TAG_neuronContainer)
		{
			if(!parseNeuronContainerTag(currentTag->firstLowerLevelTag, currentNeuron, layerIDcounter, orderIDcounter++, wrongAndNotUsedIDcounter, subnetIDcounter))
			{
				result = false;
			}
			*wrongAndNotUsedIDcounter = *wrongAndNotUsedIDcounter + 1;

			NeuronContainer * newNeuron = new NeuronContainer();
			currentNeuron->nextNeuronContainer = newNeuron;
			currentNeuron = currentNeuron->nextNeuronContainer;
		}
		else
		{
			cout << "parseLayerTag error: neuron container tag not detected";
		}
		currentTag=currentTag->nextTag;
	}
	return result;
}


bool parseNeuronContainerTag(XMLparserTag * firstTagInNeuronContainer, NeuronContainer * currentNeuron, long layerIDcounter, long orderIDcounter, long * wrongAndNotUsedIDcounter, long subnetIDcounter)
{
	bool result = true;

	XMLparserTag * currentTag = firstTagInNeuronContainer;

	if(currentTag->name == NET_XML_TAG_neuron)
	{
		if(!parseNeuronTag(currentTag, currentNeuron, layerIDcounter, orderIDcounter, wrongAndNotUsedIDcounter, subnetIDcounter))
		{
			result = false;
		}
		currentTag=currentTag->nextTag;
	}
	else
	{
		cout << "parseNeuronContainerTag error: no neuron tag detected";
	}

	if(currentTag->name == NET_XML_TAG_forwardNeuronConnectionsList)
	{
		if(!parseForwardNeuronConnectionsListTag(currentTag->firstLowerLevelTag, currentNeuron))
		{
			result = false;
		}
		currentTag=currentTag->nextTag;
	}
	else
	{
		//NB there may not be a forwardNeuronConnectionsList
		//cout << "parseNeuronContainerTag error: no forwardNeuronConnectionsList tag detected";
	}

#ifdef ANN_ADVANCED
	if(currentTag->name == NET_XML_TAG_subnet)
	{
		NeuronContainer * newNeuron = new NeuronContainer();
		currentNeuron->firstNeuronContainerInBackLayerOfSubnet = newNeuron;
		if(!parseSubnetTag(currentTag->firstLowerLevelTag, currentNeuron->firstNeuronContainerInBackLayerOfSubnet, 1, wrongAndNotUsedIDcounter, (subnetIDcounter+1)))
		{
			result = false;
		}
		currentTag=currentTag->nextTag;
	}
#endif

	if(currentTag->nextTag != NULL)
	{
		cout << "parseNeuronContainerTag error: end of content not detected";
	}

	return result;
}



bool parseForwardNeuronConnectionsListTag(XMLparserTag * firstTagInForwardNeuronConnectionsList, NeuronContainer * currentNeuron)
{
	bool result = true;

	XMLparserTag * currentTag = firstTagInForwardNeuronConnectionsList;
	NeuronConnectionContainer * currentNeuronConnectionContainer = currentNeuron->firstFrontNeuronConnectionContainer;

	while(currentTag->nextTag != NULL)
	{
		//create a new neuron connection for currentNeuronConnectionContainer
		NeuronConnection * newNeuronConnection = new NeuronConnection();
		currentNeuronConnectionContainer->neuronConnection = newNeuronConnection;

		if(!parseForwardNeuronConnectionTag(currentTag, currentNeuronConnectionContainer))
		{
			result = false;
		}

		//create a new currentNeuronConnectionContainer
		NeuronConnectionContainer * newNeuronConnectionContainer = new NeuronConnectionContainer();
		currentNeuronConnectionContainer->nextNeuronConnectionContainer = newNeuronConnectionContainer;
		currentNeuronConnectionContainer = currentNeuronConnectionContainer->nextNeuronConnectionContainer;

		currentTag = currentTag->nextTag;
	}

	return result;

}


bool parseForwardNeuronConnectionTag(XMLparserTag * currentTag, NeuronConnectionContainer * currentNeuronConnectionContainer)
{
	bool result = true;

	XMLParserAttribute * currentAttribute = currentTag->firstAttribute;

	bool neuronIDFound = false;
	bool weightFound = false;

	while(currentAttribute->nextAttribute != NULL)
	{
		if(currentAttribute->name == NET_XML_ATTRIBUTE_neuronID)
		{
			currentNeuronConnectionContainer->neuronID = atof(currentAttribute->value.c_str());		//temporary variable used to link neuron connections at a later stage
			neuronIDFound = true;
		}
		else if(currentAttribute->name == NET_XML_ATTRIBUTE_weight)
		{
			currentNeuronConnectionContainer->neuronConnection->weight = atof(currentAttribute->value.c_str());
			weightFound = true;
		}

		currentAttribute = currentAttribute->nextAttribute;
	}

	if(!neuronIDFound)
	{
		cout << "parseForwardNeuronConnectionsListTag error: neuronID attribute not detected";
		result = false;
	}
#ifdef ENFORCE_EXPLICIT_NET_XML_NEURON_KEYPROPERTIES_PARAMETERS
	if(!weightFound)
	{
		cout << "parseForwardNeuronConnectionsListTag error: weight attribute not detected";
		result = false;
	}
#endif

	return result;

}





bool parseNeuronTag(XMLparserTag * currentTag, NeuronContainer * currentNeuron, long layerIDcounter, long orderIDcounter, long * wrongAndNotUsedIDcounter, long subnetIDcounter)
{
	bool result = true;

	XMLParserAttribute * currentAttribute = currentTag->firstAttribute;

	bool idFound = false;
	bool orderIDFound = false;
	bool layerIDFound = false;
	bool subnetIDFound = false;
	bool biasFound = false;
	bool outputFound = false;
	bool classTargetFound = false;
	bool errorFound = false;

	//incase network is never trained, but output for visualisation purposes, set these values to dummy values
	currentNeuron->neuron->bias = 0;
	currentNeuron->neuron->output = 0;
	currentNeuron->neuron->classTarget = 0;
	currentNeuron->neuron->error = 0;

	while(currentAttribute->nextAttribute != NULL)
	{
		if(currentAttribute->name == NET_XML_ATTRIBUTE_id)
		{
			long attributeValue = atof(currentAttribute->value.c_str());
			//always use XML file's neuron id as this has been generated in a different order than wrongAndNotUsedIDcounter
			//#ifndef DO_NOT_REGENERATE_NEURON_IDS_WHEN_PARSING_NET_XML

			currentNeuron->neuron->id = attributeValue;

			idFound = true;
		}
		else if(currentAttribute->name == NET_XML_ATTRIBUTE_orderID)
		{
			long attributeValue = atof(currentAttribute->value.c_str());
			if(attributeValue != orderIDcounter)
			{
				cout << "parseNeuronTag error: attributeValue != orderIDcounter, attributeValue = " << attributeValue << ", orderIDcounter = " << subnetIDcounter << endl;
				result = false;
			}
			currentNeuron->neuron->orderID = attributeValue;
			orderIDFound = true;
		}
		else if(currentAttribute->name == NET_XML_ATTRIBUTE_layerID)
		{
			long attributeValue = atof(currentAttribute->value.c_str());
			if(attributeValue != layerIDcounter)
			{
				cout << "parseNeuronTag error: attributeValue != layerIDcounter, attributeValue = " << attributeValue << ", layerIDcounter = " << subnetIDcounter << endl;
				result = false;
			}
			currentNeuron->neuron->layerID = attributeValue;
			layerIDFound = true;
		}
		else if(currentAttribute->name == NET_XML_ATTRIBUTE_subnetID)
		{
			long attributeValue = atof(currentAttribute->value.c_str());
			if(attributeValue != subnetIDcounter)
			{
				cout << "parseNeuronTag error: attributeValue != subnetIDcounter, attributeValue = " << attributeValue << ", subnetIDcounter = " << subnetIDcounter << endl;
				result = false;
			}
			currentNeuron->neuron->subnetID = attributeValue;
			subnetIDFound = true;
		}
		else if(currentAttribute->name == NET_XML_ATTRIBUTE_bias)
		{
			double attributeValue = atof(currentAttribute->value.c_str());
			currentNeuron->neuron->bias = attributeValue;
			biasFound = true;
		}
		else if(currentAttribute->name == NET_XML_ATTRIBUTE_output)
		{
			double attributeValue = atof(currentAttribute->value.c_str());
			currentNeuron->neuron->output = attributeValue;
			outputFound = true;
		}
		else if(currentAttribute->name == NET_XML_ATTRIBUTE_classTarget)
		{
			double attributeValue = atof(currentAttribute->value.c_str());
			currentNeuron->neuron->classTarget = attributeValue;
			classTargetFound = true;
		}
		else if(currentAttribute->name == NET_XML_ATTRIBUTE_error)
		{
			double attributeValue = atof(currentAttribute->value.c_str());
			currentNeuron->neuron->error = attributeValue;
			errorFound = true;
		}
		else if(currentAttribute->name == NET_XML_ATTRIBUTE_xPosRel)
		{
			currentNeuron->neuron->spatialCoordinatesSet = true;
			long attributeValue = atof(currentAttribute->value.c_str());
			currentNeuron->neuron->xPosRel = attributeValue;
		}
		else if(currentAttribute->name == NET_XML_ATTRIBUTE_yPosRel)
		{
			currentNeuron->neuron->spatialCoordinatesSet = true;
			long attributeValue = atof(currentAttribute->value.c_str());
			currentNeuron->neuron->yPosRel = attributeValue;
		}
		else if(currentAttribute->name == NET_XML_ATTRIBUTE_zPosRel)
		{
			currentNeuron->neuron->spatialCoordinatesSet = true;
			long attributeValue = atof(currentAttribute->value.c_str());
			currentNeuron->neuron->zPosRel = attributeValue;
		}
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
		currentNeuron->neuron->orderID = orderIDcounter;
	}
	if(!layerIDFound)
	{
		currentNeuron->neuron->layerID = layerIDcounter;
	}
	if(!subnetIDFound)
	{
		currentNeuron->neuron->subnetID = subnetIDcounter;
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


/*******************************************************************************
 *
 * File Name: ANNXMLconversion.c
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2012 Baxter AI (baxterai.com)
 * Project: Advanced Neural Network (ANN)
 * Project Version: 3a7a 06-June-2012
 * Comments
 *
 *******************************************************************************/



#include "ANNXMLconversion.h"

//this function works and no longer needs to be tested
#ifdef NN_XML_DEBUG
bool testReadNetXMLFile()
{
	bool result = true;

	NeuronContainer * firstInputNeuronInNetwork = new NeuronContainer();	//the firstInputNeuronInNetwork object must be initialised here (in ANNXMLconversion.cpp scope). if it is initialised in another .cpp it will be come corrupted,

	if(!readNetXMLFile(NET_XML_FILE_NAME, firstInputNeuronInNetwork))
	{
		result = false;
	}

	#define TEMP_NET_XML_FILE_NAME1 "tempnet1.xml"
	if(!writeNetXMLFile(TEMP_NET_XML_FILE_NAME1, firstInputNeuronInNetwork))
	{
		result = false;
	}
	return result;
}

bool testReadNetXMLFile2(NeuronContainer * firstInputNeuronInNetwork)
{
	bool result = true;

	if(!writeNetXMLFile(NET_XML_FILE_NAME, firstInputNeuronInNetwork))
	{
		result = false;
	}

	NeuronContainer * tempFirstInputNeuronInNetwork = new NeuronContainer();
	if(!readNetXMLFile(NET_XML_FILE_NAME, tempFirstInputNeuronInNetwork))
	{
		result = false;
	}

	//cout << "here3" << endl;
	#define TEMP_NET_XML_FILE_NAME1 "tempnet1.xml"
	if(!writeNetXMLFile(TEMP_NET_XML_FILE_NAME1, tempFirstInputNeuronInNetwork))
	{
		result = false;
	}
	return result;
}

#endif

NeuronContainer * readNetXMLFileAndRecordFormationVariables(string xmlFileName, NeuronContainer * firstInputNeuronInNetwork, long * numberOfInputNeurons, long * numberOfOutputNeurons)
{
	NeuronContainer * firstOutputNeuronInNetwork;

	readNetXMLFile(xmlFileName, firstInputNeuronInNetwork);

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


bool readNetXMLFile(string xmlFileName, NeuronContainer * firstInputNeuronInNetwork)
{
	bool result = true;

 	XMLParserTag * firstTagInXMLFile = new XMLParserTag();	//the firstTagInXMLFile object must be initialised here (in ANNXMLconversion.cpp scope). if it is initialised in XMLParserClass.cpp else it will be come corrupted,

		//cout << "a" << endl;

 	if(!readXMLFile(xmlFileName, firstTagInXMLFile))
 	{
		result = false;
	}

		//cout << "b" << endl;

	if(!parseNetTag(firstTagInXMLFile, firstInputNeuronInNetwork))
	{
		result = false;
	}

		//cout << "c" << endl;

	if(!linkLayerXNeuronsBasedUponFrontNeuronConnectionContainerListNeuronIDs(firstInputNeuronInNetwork, firstInputNeuronInNetwork, false, NULL))
	{
		result = false;
	}
	return result;
}

bool writeNetXMLFile(string xmlFileName, NeuronContainer * firstInputNeuronInNetwork)
{
	bool result = true;

 	XMLParserTag * firstTagInXMLFile = new XMLParserTag();	//the firstTagInXMLFile object must be initialised here (in ANNXMLconversion.cpp scope). if it is initialised in XMLParserClass.cpp else it will be come corrupted,

	XMLParserTag * currentTagL0 = firstTagInXMLFile;
	currentTagL0->name = NET_XML_TAG_network;
	XMLParserTag * newTag0 = new XMLParserTag();	//had to add a null tag
	currentTagL0->nextTag = newTag0;

	XMLParserTag * firstTagL1 = new XMLParserTag();
	currentTagL0->firstLowerLevelTag = firstTagL1;
	XMLParserTag * currentTagL1 = currentTagL0->firstLowerLevelTag;
	XMLParserTag * newTag1 = new XMLParserTag();	//had to add a null tag
	currentTagL1->nextTag = newTag1;

	currentTagL1->name = NET_XML_TAG_subnet;
	XMLParserTag * firstTagL2 = new XMLParserTag();
	currentTagL1->firstLowerLevelTag = firstTagL2;
	XMLParserTag * currentTagL2 = currentTagL1->firstLowerLevelTag;
	XMLParserTag * newTag2 = new XMLParserTag();	//had to add a null tag
	currentTagL2->nextTag = newTag2;

	if(!generateXMLTagListBasedUponSubnet(currentTagL1->firstLowerLevelTag, firstInputNeuronInNetwork))
	{
		result = false;
	}

	//cout << "dEBUG generateXMLTagListBasedUponSubnet done " << endl;

 	if(!writeXMLFile(xmlFileName, firstTagInXMLFile))
 	{
		result = false;
	}

	delete firstTagInXMLFile;
	//cout << "dEBUG writeXMLFile done " << endl;


	return result;
}


bool generateXMLTagListBasedUponSubnet(XMLParserTag * firstTagInSubnet, NeuronContainer * firstNeuronContainerInSubnet)
{
	//cout << "h3" << endl;

	bool result = true;

	XMLParserTag * currentTagL0 = firstTagInSubnet;
	NeuronContainer * currentNeuron = firstNeuronContainerInSubnet;

	bool stillMoreLayers = true;
	while(stillMoreLayers)
	{
		bool currentLayerHasFrontLayer = false;

		NeuronContainer * firstNeuronInLayer = currentNeuron;

		currentTagL0->name = NET_XML_TAG_layer;
		XMLParserTag * newTag0 = new XMLParserTag();	//had to add a null tag
		currentTagL0->nextTag = newTag0;

		XMLParserTag * firstTagL1 = new XMLParserTag();
		currentTagL0->firstLowerLevelTag = firstTagL1;
		XMLParserTag * currentTagL1 = currentTagL0->firstLowerLevelTag;
		XMLParserTag * newTag1 = new XMLParserTag();	//had to add a null tag
		currentTagL1->nextTag = newTag1;

		//cout << "h4" << endl;

		while(currentNeuron->nextNeuronContainer != NULL)
		{
			//cout << "h5" << endl;

			char tempString[MAX_ATTRIBUTE_VALUE_SIZE];

			//generate neuron connection tag
			currentTagL1->name = NET_XML_TAG_neuronContainer;
			XMLParserTag * firstTagL2 = new XMLParserTag();
			currentTagL1->firstLowerLevelTag = firstTagL2;
			XMLParserTag * currentTagL2 = currentTagL1->firstLowerLevelTag;

			//generate neuron tag
			currentTagL2->name = NET_XML_TAG_neuron;

			XMLParserAttribute * currentAttribute = currentTagL2->firstAttribute;

			currentAttribute->name = NET_XML_ATTRIBUTE_id;
			sprintf(tempString, "%d", (currentNeuron->neuron->id));
			currentAttribute->value = tempString;

			XMLParserAttribute * newAttribute1 = new XMLParserAttribute();
			currentAttribute->nextAttribute = newAttribute1;
			currentAttribute = currentAttribute->nextAttribute;

			//cout << "h5a" << endl;

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
			//cout << "h5b" << endl;

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

			//cout << "h5cB1" << endl;

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
			//cout << "h5d" << endl;


			XMLParserTag * newTag = new XMLParserTag();
			currentTagL2->nextTag = newTag;
			currentTagL2 = currentTagL2->nextTag;

			//cout << "h5e" << endl;

			//generate NET_XML_TAG_forwardNeuronConnectionsList tag
			if(currentNeuron -> firstFrontNeuronConnectionContainer -> nextNeuronConnectionContainer != NULL)
			{
				//cout << "h6" << endl;
				currentTagL2->name = NET_XML_TAG_forwardNeuronConnectionsList;
				XMLParserTag * firstTagL3 = new XMLParserTag();
				currentTagL2->firstLowerLevelTag = firstTagL3;
				XMLParserTag * currentTagL3 = currentTagL2->firstLowerLevelTag;
				NeuronConnectionContainer * currentNeuronConnectionContainer = currentNeuron->firstFrontNeuronConnectionContainer;
				while(currentNeuronConnectionContainer -> nextNeuronConnectionContainer != NULL)
				{
					//cout << "h7" << endl;

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

					XMLParserTag * newTag = new XMLParserTag();
					currentTagL3->nextTag = newTag;
					currentTagL3 = currentTagL3->nextTag;

					currentNeuronConnectionContainer = currentNeuronConnectionContainer -> nextNeuronConnectionContainer;

					//cout << "h7b" << endl;

				}
				XMLParserTag * newTag = new XMLParserTag();
				currentTagL2->nextTag = newTag;

				//cout << "h6b" << endl;


			}

		#ifdef ANN
			currentTagL2 = currentTagL2->nextTag;

			//generate subtag tag
			if(currentNeuron -> isSubnet)
			{
				currentTagL2->name = NET_XML_TAG_subnet;

				XMLParserTag * newTag1 = new XMLParserTag();
				currentTagL2->firstLowerLevelTag = newTag1;
				if(!generateXMLTagListBasedUponSubnet(currentTagL2->firstLowerLevelTag, currentNeuron->firstNeuronContainerInBackLayerOfSubnet))
				{
					result = false;
				}

				XMLParserTag * newTag = new XMLParserTag();
				currentTagL2->nextTag = newTag;
				//currentTagL2 = currentTagL2->nextTag;
			}
		#endif

			XMLParserTag * newTag2 = new XMLParserTag();
			currentTagL1->nextTag = newTag2;
			currentTagL1 = currentTagL1->nextTag;

			currentNeuron = currentNeuron->nextNeuronContainer;
		}

		//cout << "h8" << endl;

		if(firstNeuronInLayer->hasFrontLayer)
		{
			currentNeuron=firstNeuronInLayer->firstNeuronInFrontLayer;
		}
		else
		{
			stillMoreLayers = false;
		}

		XMLParserTag * newTag = new XMLParserTag();
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

	#ifdef ANN
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


NeuronContainer * findNeuronContainer(NeuronContainer * firstNeuronContainerInLayer, long neuronIDToFind, bool * result)
{
	NeuronContainer * foundNeuronContainer = NULL;


	NeuronContainer * currentNeuron = firstNeuronContainerInLayer;
	while(currentNeuron->nextNeuronContainer != NULL)
	{
		if(currentNeuron->neuron->id == neuronIDToFind)
		{
			//cout << "currentNeuron->neuron->id  = " << currentNeuron->neuron->id << endl;
			*result = true;
			foundNeuronContainer = currentNeuron;
		}
	#ifdef ANN
		if(currentNeuron->firstNeuronContainerInBackLayerOfSubnet != NULL)
		{
			bool tempResult = false;
			NeuronContainer * tempFoundNeuronContainer;
			tempFoundNeuronContainer = findNeuronContainer(currentNeuron->firstNeuronContainerInBackLayerOfSubnet, neuronIDToFind, &tempResult);
			if(tempResult)
			{
				*result = true;
				foundNeuronContainer = tempFoundNeuronContainer;
			}
		}
	#endif
		currentNeuron = currentNeuron -> nextNeuronContainer;
	}

	//cout << "firstNeuronContainerInLayer ID = " << firstNeuronContainerInLayer->neuron->id << endl;
	if(firstNeuronContainerInLayer->firstNeuronInFrontLayer != NULL)
	{
		//cout << "trace1" << endl;
		bool tempResult = false;
		NeuronContainer * tempFoundNeuronContainer;
		tempFoundNeuronContainer = findNeuronContainer(firstNeuronContainerInLayer->firstNeuronInFrontLayer, neuronIDToFind, &tempResult);
		if(tempResult)
		{
			*result = true;
			foundNeuronContainer = tempFoundNeuronContainer;
		}
	}

	return foundNeuronContainer;
}


//Top Level
bool parseNetTag(XMLParserTag * firstTagInNetwork, NeuronContainer * currentNeuron)
{
	bool result = true;

	XMLParserTag * currentTagUpdatedL1 = firstTagInNetwork;

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
			XMLParserTag * currentTagUpdatedL2 = currentTagUpdatedL1->firstLowerLevelTag;
			if(currentTagUpdatedL2->name == NET_XML_TAG_inputNeurons)
			{
				XMLParserTag * currentTagUpdatedL3 = currentTagUpdatedL2->firstLowerLevelTag;
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
				XMLParserTag * currentTagUpdatedL3 = currentTagUpdatedL2->firstLowerLevelTag;
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
			long wrongAndNotUsedIDCounter = 1;
			if(!parseSubnetTag(currentTagUpdatedL1->firstLowerLevelTag, currentNeuron, 1, &wrongAndNotUsedIDCounter, 1))
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

bool parseSubnetTag(XMLParserTag * firstTagInSubnet, NeuronContainer * firstNeuronInSubnet, long layerIDCounter, long * wrongAndNotUsedIDCounter, long subnetIDCounter)
{
	bool result = true;

	NeuronContainer * currentNeuron = firstNeuronInSubnet;

	XMLParserTag * currentTag = firstTagInSubnet;

	NeuronContainer * recordOfFirstNeuronInPreviousLayer;	//to stop an additional layer being added

	while(currentTag->nextTag != NULL)
	{
		if(currentTag->name == NET_XML_TAG_layer)
		{
			if(!parseLayerTag(currentTag->firstLowerLevelTag, currentNeuron, layerIDCounter++, 1, wrongAndNotUsedIDCounter, subnetIDCounter))
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

bool parseLayerTag(XMLParserTag * firstTagInLayer, NeuronContainer * firstNeuronInLayer, long layerIDCounter, long orderIDCounter, long * wrongAndNotUsedIDCounter, long subnetIDCounter)
{
	bool result = true;

	XMLParserTag * currentTag = firstTagInLayer;
	NeuronContainer * currentNeuron = firstNeuronInLayer;

	while(currentTag->nextTag != NULL)
	{
		if(currentTag->name == NET_XML_TAG_neuronContainer)
		{
			/*
			if(subnetIDCounter > 1)
			{
				cout << "here1" << endl;
			}
			*/

			if(!parseNeuronContainerTag(currentTag->firstLowerLevelTag, currentNeuron, layerIDCounter, orderIDCounter++, wrongAndNotUsedIDCounter, subnetIDCounter))
			{
				result = false;
			}
			*wrongAndNotUsedIDCounter = *wrongAndNotUsedIDCounter + 1;

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


bool parseNeuronContainerTag(XMLParserTag * firstTagInNeuronContainer, NeuronContainer * currentNeuron, long layerIDCounter, long orderIDCounter, long * wrongAndNotUsedIDCounter, long subnetIDCounter)
{
	bool result = true;

	XMLParserTag * currentTag = firstTagInNeuronContainer;

	if(currentTag->name == NET_XML_TAG_neuron)
	{
		if(!parseNeuronTag(currentTag, currentNeuron, layerIDCounter, orderIDCounter, wrongAndNotUsedIDCounter, subnetIDCounter))
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

#ifdef ANN
	if(currentTag->name == NET_XML_TAG_subnet)
	{
		NeuronContainer * newNeuron = new NeuronContainer();
		currentNeuron->firstNeuronContainerInBackLayerOfSubnet = newNeuron;
		if(!parseSubnetTag(currentTag->firstLowerLevelTag, currentNeuron->firstNeuronContainerInBackLayerOfSubnet, 1, wrongAndNotUsedIDCounter, (subnetIDCounter+1)))
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



bool parseForwardNeuronConnectionsListTag(XMLParserTag * firstTagInForwardNeuronConnectionsList, NeuronContainer * currentNeuron)
{
	bool result = true;

	XMLParserTag * currentTag = firstTagInForwardNeuronConnectionsList;
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


bool parseForwardNeuronConnectionTag(XMLParserTag * currentTag, NeuronConnectionContainer * currentNeuronConnectionContainer)
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





bool parseNeuronTag(XMLParserTag * currentTag, NeuronContainer * currentNeuron, long layerIDCounter, long orderIDCounter, long * wrongAndNotUsedIDCounter, long subnetIDCounter)
{
	bool result = true;

	//cout << "g1 error detected here" << endl;

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
			//always use XML file's neuron id as this has been generated in a different order than wrongAndNotUsedIDCounter
			//#ifndef DO_NOT_REGENERATE_NEURON_IDS_WHEN_PARSING_NET_XML

			currentNeuron->neuron->id = attributeValue;

			idFound = true;
		}
		else if(currentAttribute->name == NET_XML_ATTRIBUTE_orderID)
		{
			long attributeValue = atof(currentAttribute->value.c_str());
			if(attributeValue != orderIDCounter)
			{
				cout << "parseNeuronTag error: attributeValue != orderIDCounter, attributeValue = " << attributeValue << ", orderIDCounter = " << subnetIDCounter << endl;
				result = false;
			}
			currentNeuron->neuron->orderID = attributeValue;
			orderIDFound = true;
		}
		else if(currentAttribute->name == NET_XML_ATTRIBUTE_layerID)
		{
			long attributeValue = atof(currentAttribute->value.c_str());
			if(attributeValue != layerIDCounter)
			{
				cout << "parseNeuronTag error: attributeValue != layerIDCounter, attributeValue = " << attributeValue << ", layerIDCounter = " << subnetIDCounter << endl;
				result = false;
			}
			currentNeuron->neuron->layerID = attributeValue;
			layerIDFound = true;
		}
		else if(currentAttribute->name == NET_XML_ATTRIBUTE_subnetID)
		{
			long attributeValue = atof(currentAttribute->value.c_str());
			if(attributeValue != subnetIDCounter)
			{
				cout << "parseNeuronTag error: attributeValue != subnetIDCounter, attributeValue = " << attributeValue << ", subnetIDCounter = " << subnetIDCounter << endl;
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
		currentNeuron->neuron->orderID = orderIDCounter;
	}
	if(!layerIDFound)
	{
		currentNeuron->neuron->layerID = layerIDCounter;
	}
	if(!subnetIDFound)
	{
		currentNeuron->neuron->subnetID = subnetIDCounter;
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



/*
XMLParserTag * parseTagDownALevel(XMLParserTag * currentTag, string sectionTagName, bool * result)
{
	*result = true;

	XMLParserTag * updatedCurrentTag;

	if(currentTag->name != sectionTagName)
	{
		*result = false;
		cout << "parse error; lower level sectionTagName expected = " <<  sectionTagName << ". section tag name found currentTag->name = " << currentTag->name << endl;
		updatedCurrentTag = currentTag;
	}
	else
	{
		updatedCurrentTag = currentTag->firstLowerLevelTag;
	}

	return updatedCurrentTag;

}
*/


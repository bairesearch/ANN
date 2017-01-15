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
 * File Name: ANNxmlConversion.h
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2012 Baxter AI (baxterai.com)
 * Project: Artificial Neural Network (ANN)
 * Project Version: 3c5b 15-December-2012
 * Comments
 *
 *******************************************************************************/


#ifndef HEADER_ANN_XML_CONVERSION
#define HEADER_ANN_XML_CONVERSION


#include "XMLparserClass.h"
#include "ANNneuronClass.h"

//#define ENFORCE_EXPLICIT_NET_XML_NEURON_ID_PARAMETERS
//#define ENFORCE_EXPLICIT_NET_XML_NEURON_KEYPROPERTIES_PARAMETERS

//#define DO_NOT_STORE_NET_XML_NEURON_ID_PARAMETERS
//#define DO_NOT_STORE_NET_XML_NEURON_KEYPROPERTIES_PARAMETERS

#define MAX_ATTRIBUTE_VALUE_SIZE 200




#define NET_XML_TAG_network ((string)"network")
#define NET_XML_TAG_iodescriptions ((string)"iodescriptions")
#define NET_XML_TAG_inputNeurons ((string)"inputNeurons")
#define NET_XML_TAG_outputNeurons ((string)"outputNeurons")
#define NET_XML_TAG_description ((string)"description")
#define NET_XML_TAG_subnet ((string)"subnet")
#define NET_XML_TAG_layer ((string)"layer")
#define NET_XML_TAG_neuronContainer ((string)"neuronContainer")
#define NET_XML_TAG_neuron ((string)"neuron")
#define NET_XML_TAG_forwardNeuronConnectionsList ((string)"forwardNeuronConnectionsList")
#define NET_XML_TAG_forwardNeuronConnection ((string)"forwardNeuronConnection")
#define NET_XML_TAG_subnet ((string)"subnet")

#define NET_XML_ATTRIBUTE_id ((string)"id")
#define NET_XML_ATTRIBUTE_orderID ((string)"orderID")
#define NET_XML_ATTRIBUTE_layerID ((string)"layerID")
#define NET_XML_ATTRIBUTE_subnetID ((string)"subnetID")
#define NET_XML_ATTRIBUTE_bias ((string)"bias")
#define NET_XML_ATTRIBUTE_output ((string)"output")
#define NET_XML_ATTRIBUTE_classTarget ((string)"classTarget")
#define NET_XML_ATTRIBUTE_error ((string)"error")
#define NET_XML_ATTRIBUTE_xPosRel ((string)"xPosRel")
#define NET_XML_ATTRIBUTE_yPosRel ((string)"yPosRel")
#define NET_XML_ATTRIBUTE_zPosRel ((string)"zPosRel")
#define NET_XML_ATTRIBUTE_neuronID ((string)"neuronID")
#define NET_XML_ATTRIBUTE_weight ((string)"weight")


//#define NN_XML_DEBUG
#ifdef NN_XML_DEBUG
bool testReadNetXMLFile();
bool testReadNetXMLFile2(NeuronContainer * firstInputNeuronInNetwork);
#endif

bool writeNetXMLfile(string xmlFileName, NeuronContainer * firstInputNeuronInNetwork);
	bool generateXMLtagListBasedUponSubnet(XMLparserTag * firstTagInSubnet, NeuronContainer * firstNeuronContainerInSubnet);

NeuronContainer * readNetXMLfileAndRecordFormationVariables(string xmlFileName, NeuronContainer * firstInputNeuronInNetwork, long * numberOfInputNeurons, long * numberOfOutputNeurons);
	bool readNetXMLfile(string xmlFileName, NeuronContainer * firstInputNeuronInNetwork);
		bool parseNetTag(XMLparserTag * firstTagInNetwork, NeuronContainer * currentNeuron);
			bool parseSubnetTag(XMLparserTag * firstTagInSubnet, NeuronContainer * firstNeuronInSubnet, long layerIDcounter, long * wrongAndNotUsedIDcounter, long subnetIDcounter);
				bool parseLayerTag(XMLparserTag * firstTagInLayer, NeuronContainer * firstNeuronInLayer, long layerIDcounter, long orderIDcounter, long * wrongAndNotUsedIDcounter, long subnetIDcounter);
					bool parseNeuronContainerTag(XMLparserTag * firstTagInNeuronContainer, NeuronContainer * currentNeuron, long layerIDcounter, long orderIDcounter, long * wrongAndNotUsedIDcounter, long subnetIDcounter);
						bool parseNeuronTag(XMLparserTag * currentTag, NeuronContainer * currentNeuron, long layerIDcounter, long orderIDcounter, long * wrongAndNotUsedIDcounter, long subnetIDcounter);
						bool parseForwardNeuronConnectionsListTag(XMLparserTag * firstTagInForwardNeuronConnectionsList, NeuronContainer * currentNeuron);
								bool parseForwardNeuronConnectionTag(XMLparserTag * currentTag, NeuronConnectionContainer * currentNeuronConnectionContainer);
		bool linkLayerXNeuronsBasedUponFrontNeuronConnectionContainerListNeuronIDs(NeuronContainer * firstNeuronContainerInLayer, NeuronContainer * firstInputNeuronInNetwork, bool hasBackLayer, NeuronContainer * firstNeuronContainerInBackLayer);
			NeuronContainer * findNeuronContainer(NeuronContainer * firstNeuronContainerInLayer, long neuronIDtoFind, bool * result);
	NeuronContainer * recordOutputNeuronAndNumInputAndOutputNeuronsInNetwork(NeuronContainer * firstInputNeuronInNetwork, long * numberOfInputNeurons, long * numberOfOutputNeurons);


#endif

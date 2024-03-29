 /*******************************************************************************
 *
 * License: GNU Affero General Public License version 3 (AGPLv3)
 *
 *******************************************************************************/

/*******************************************************************************
 *
 * File Name: ANNxmlConversion.hpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2022 Baxter AI (baxterai.com)
 * Project: Artificial Neural Network (ANN)
 * Project Version: 3q1a 05-June-2022
 * Comments
 * /
 *******************************************************************************/


#ifndef HEADER_ANN_XML_CONVERSION
#define HEADER_ANN_XML_CONVERSION


#include "XMLparserClass.hpp"
#include "ANNneuronClass.hpp"
#include "ANNneuronConnectionClass.hpp"
#include "SHAREDvars.hpp"


//#define ENFORCE_EXPLICIT_NET_XML_NEURON_ID_PARAMETERS
//#define ENFORCE_EXPLICIT_NET_XML_NEURON_KEYPROPERTIES_PARAMETERS
//#define DO_NOT_STORE_NET_XML_NEURON_SPATIAL_COORD_PARAMETERS

#ifdef ANN_ALGORITHM_CLASSIFICATION_NETWORK
	#define DO_NOT_STORE_NET_XML_NEURON_ID_PARAMETERS
	#define DO_NOT_STORE_NET_XML_NEURON_KEYPROPERTIES_PARAMETERS
#endif

#define MAX_ATTRIBUTE_VALUE_SIZE 200

#define ANN_XML_CONVERSION_VALUE_RECORD_PRECISION "%0.15f"	//DBL_DIG	//"%0.6f"



#define NET_XML_TAG_network ((string)"network")
#define NET_XML_TAG_iodescriptions ((string)"iodescriptions")
#define NET_XML_TAG_inputNeurons ((string)"inputNeurons")
#define NET_XML_TAG_outputNeurons ((string)"outputNeurons")
#define NET_XML_TAG_description ((string)"description")
#define NET_XML_TAG_subnet ((string)"subnet")
#define NET_XML_TAG_layer ((string)"layer")
#define NET_XML_TAG_neuronContainer ((string)"neuronContainer")
#define NET_XML_TAG_neuron ((string)"neuron")
#define NET_XML_TAG_forwardANNneuronConnectionsList ((string)"forwardANNneuronConnectionsList")
#define NET_XML_TAG_forwardANNneuronConnection ((string)"forwardANNneuronConnection")
#define NET_XML_TAG_subnet ((string)"subnet")

#define NET_XML_ATTRIBUTE_id ((string)"id")
#define NET_XML_ATTRIBUTE_orderID ((string)"orderID")
#define NET_XML_ATTRIBUTE_layerID ((string)"layerID")
#define NET_XML_ATTRIBUTE_subnetID ((string)"subnetID")
#define NET_XML_ATTRIBUTE_bias ((string)"bias")
#define NET_XML_ATTRIBUTE_output ((string)"output")
#define NET_XML_ATTRIBUTE_classTarget ((string)"classTarget")
#define NET_XML_ATTRIBUTE_error ((string)"error")
#ifdef ANN_ALGORITHM_CLASSIFICATION_NETWORK
#define NET_XML_ATTRIBUTE_memoryTrace ((string)"memoryTrace")
#endif
#define NET_XML_ATTRIBUTE_xPosRel ((string)"xPosRel")
#define NET_XML_ATTRIBUTE_yPosRel ((string)"yPosRel")
#define NET_XML_ATTRIBUTE_zPosRel ((string)"zPosRel")
#ifdef ANN_ALGORITHM_CLASSIFICATION_NETWORK
#define NET_XML_ATTRIBUTE_xPosRelFrac ((string)"xPosRelFrac")
#define NET_XML_ATTRIBUTE_yPosRelFrac ((string)"yPosRelFrac")
#define NET_XML_ATTRIBUTE_zPosRelFrac ((string)"zPosRelFrac")
#endif
#define NET_XML_ATTRIBUTE_neuronID ((string)"neuronID")
#define NET_XML_ATTRIBUTE_weight ((string)"weight")
#ifdef ANN_ALGORITHM_CLASSIFICATION_NETWORK
#define NET_XML_ATTRIBUTE_idealValue ((string)"idealValue")
#endif
#ifdef ANN_ALGORITHM_GIA_NEURAL_NETWORK
#define NET_XML_ATTRIBUTE_entityName ((string)"entityName")
#endif


//#define NN_XML_DEBUG
class ANNxmlConversionClass
{
	private: XMLparserClassClass XMLparserClass;
	private: SHAREDvarsClass SHAREDvars;

	public: bool writeNetXMLfile(string xmlFileName, ANNneuron* firstInputNeuronInNetwork);
		private: bool generateXMLtagListBasedUponSubnet(XMLparserTag* firstTagInSubnet, ANNneuron* firstNeuronInSubnet);
			private: bool generateXMLtagListBasedUponLayer(XMLparserTag* firstTagInSubnet, ANNneuron* firstNeuronInLayer);
			private: bool resetPrintedXMLbasedUponLayer(ANNneuron* firstNeuronInLayer);

	public: ANNneuron* readNetXMLfileAndRecordFormationVariables(string xmlFileName, ANNneuron* firstInputNeuronInNetwork, int64_t* numberOfInputNeurons, int64_t* numberOfOutputNeurons, int64_t* numberOfLayers);
		private: bool readNetXMLfile(string xmlFileName, ANNneuron* firstInputNeuronInNetwork);
			private: bool parseNetTag(XMLparserTag* firstTagInNetwork, ANNneuron* currentNeuron);
				private: bool parseSubnetTag(XMLparserTag* firstTagInSubnet, ANNneuron* firstNeuronInSubnet, int64_t layerIDcounter, int64_t* wrongAndNotUsedIDcounter, int64_t subnetIDcounter);
					private: bool parseLayerTag(XMLparserTag* firstTagInLayer, ANNneuron* firstNeuronInLayer, int64_t layerIDcounter, int64_t orderIDcounter, int64_t* wrongAndNotUsedIDcounter, int64_t subnetIDcounter);
						private: bool parseNeuronContainerTag(XMLparserTag* firstTagInNeuronContainer, ANNneuron* currentNeuron, int64_t layerIDcounter, int64_t orderIDcounter, int64_t* wrongAndNotUsedIDcounter, int64_t subnetIDcounter);
							private: bool parseNeuronTag(XMLparserTag* currentTag, ANNneuron* currentNeuron, int64_t layerIDcounter, int64_t orderIDcounter, int64_t* wrongAndNotUsedIDcounter, int64_t subnetIDcounter);
							private: bool parseForwardANNneuronConnectionsListTag(XMLparserTag* firstTagInForwardANNneuronConnectionsList, ANNneuron* currentNeuron);
									private: bool parseForwardANNneuronConnectionTag(XMLparserTag* currentTag, ANNneuronConnection* currentANNneuronConnection);
			private: bool linkLayerXNeuronsBasedUponFrontANNneuronConnectionListNeuronIDs(ANNneuron* firstNeuronInLayer, ANNneuron* firstInputNeuronInNetwork, bool hasBackLayer, ANNneuron* firstNeuronInBackLayer);
				private: ANNneuron* findNeuron(ANNneuron* firstNeuronInLayer, int64_t neuronIDtoFind, bool* result);
		private: ANNneuron* recordOutputNeuronAndNumInputAndOutputNeuronsInNetwork(ANNneuron* firstInputNeuronInNetwork, int64_t* numberOfInputNeurons, int64_t* numberOfOutputNeurons, int64_t* numberOfLayers);

	private: void clearTag(XMLparserTag* tag);
};

#endif

/*******************************************************************************
 *
 * No License
 * 
 * This work is under exclusive copyright (c) Baxter AI (baxterai.com). 
 * Nobody else can use, copy, distribute, or modify this work without being 
 * at risk of take-downs, shake-downs, or litigation. 
 * 
 * By publishing this source code in a public repository on GitHub, Terms of 
 * Service have been accepted by which Baxter AI have allowed others to view 
 * and fork their repository.
 * 
 * If you find software that doesn't have a license, that generally means you 
 * have no permission from the creators of the software to use, modify, or 
 * share the software. Although a code host such as GitHub may allow you to 
 * view and fork the code, this does not imply that you are permitted to use, 
 * modify, or share the software for any purpose.
 *
 * This notice has been derived from https://choosealicense.com/no-permission 
 * (https://web.archive.org/web/20180312144938/https://choosealicense.com/no-permission)
 *
 *******************************************************************************/

/*******************************************************************************
 *
 * File Name: ANNformation.hpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2018 Baxter AI (baxterai.com)
 * Project: Artificial Neural Network (ANN)
 * Project Version: 3m13c 22-February-2018
 * Comments:
 * /
 *******************************************************************************/


#ifndef HEADER_ANN_FORMATION
#define HEADER_ANN_FORMATION

#include "ANNneuronClass.hpp"
#include "ANNneuronConnectionClass.hpp"
#include "ANNglobalDefs.hpp"

#ifdef ANN_ALGORITHM_MEMORY_NETWORK
	#define DEFAULT_PROBABILITY_NEURON_CONNECTION_WITH_SIDE_LAYER_NEURONS_ANNTH (1.0)		//100% probability that a neuron will be connected to a neuron on the same layer (memory trace connections only)
#endif

	//probability of subnet creation parameters
#define DEFAULT_PROB_OF_SUBNET_CREATION_ANNTH (0.6)				//60% probability of a neuron object being a subnet (assuming the neuron object can be made into a subnet - it has a front layer and a back layer)
#define DEFAULT_ANN_MAX_NUM_RECURSIVE_SUBNETS_ANNTH (2)				//number of recursive subnets including the top level neural network
#define DEFAULT_NOANN_MAX_NUM_RECURSIVE_SUBNETS_ANNTH (1)				//number of recursive subnets including the top level neural network

	//subnet excusivity of neuron connections parameters
#define DEFAULT_PROBABILITY_NEURON_CONNECTION_WITH_PREVIOUS_LAYER_NEURON_ANNTH (1.0)					//100% probability of a neuron haveing a neuron connection with a neuron on the previous layer
#define DEFAULT_PROBABILITY_NEURON_CONNECTION_WITH_ALL_PREVIOUS_LAYERS_NEURONS_ANNTH (0.0)		//100% probability that a neuron will not be provided a direct link with a previous layer neuron
				//probabilityOfConnectionInNewFrontLayer

	//subnet number of layers parameters
#define DEFAULT_NUM_LAYERS_MODIFIER_ANNTH (1.0)						//a fractional value which effects the number of layers generated for a subnet of level x with a given number of input and output neurons
#define DEFAULT_USE_SUBNET_DEPENDENT_NUMLAYERS_ANNTH (false)					//a boolean value which determines whether or not the number of layers generated for a subnet of level x with a given number of input and output neurons is dependent upon the subnet level x

	//subnet divergence parameters
#define LAYER_DIVERGENCE_TYPE_LINEAR_CONVERGING (1)									//see ANNformation.cpp for diagramatic description
#define LAYER_DIVERGENCE_TYPE_LINEAR_DIVERGING_THEN_CONVERGING (2)					//see ANNformation.cpp for diagramatic description
#define LAYER_DIVERGENCE_TYPE_NONLINEAR_DIVERGING_THEN_CONVERGING (3)				//see ANNformation.cpp for diagramatic description
#define LAYER_DIVERGENCE_TYPE_LINEAR_DIVERGING (4)
#define LAYER_DIVERGENCE_TYPE_LINEAR_DIVERGING_SQUARE2D (5)
#define LAYER_DIVERGENCE_TYPE_LINEAR_DIVERGING_SQUARE2D_RADIALBIAS (6)
#define LAYER_DIVERGENCE_TYPE_LINEAR_CONVERGING_SQUARE2D (7)

	//USE_ADVANCED_RANDOM_SUBNET_DIVERGENCE
#define DEFAULT_MEAN_LAYER_DIVERGENCE_FACTOR (1.5)

#define ABS_OF_SHORT 32767.0

#define BASIC_NUMBER_OF_HIDDEN_NEURONS_HEURISTIC_ANNTH (1.0F)	//only used for backwards compatability

class ANNformationClass
{
	private: ANNneuronClassClass ANNneuronClass;
	#ifdef ANN_ALGORITHM_CLASSIFICATION_NETWORK
	public: void formNeuralNetworkInputLayer(ANNneuron* firstInputNeuronInNetwork, const int numberOfInputNeurons);
	#else
	private: void formSimpleNeuralNet(ANNneuron* firstInputNeuron, ANNneuron* firstOutputNeuron, const long numberOfInputNeurons, const long numberOfHiddenNeurons, const long numberOfOutputNeurons);
	public: ANNneuron* formNeuralNetWithOptimisedProperties(ANNneuron* firstInputNeuronInNetwork, const long numberOfInputNeurons, long numberOfOutputNeurons, const long numberOfLayers);
		public: ANNneuron* formNeuralNet(ANNneuron* firstInputNeuron, const long numberOfInputNeurons, long numberOfOutputNeurons, const long numberOfLayers, const int layerDivergenceType, const double meanLayerDivergenceFactor, const double probabilityANNneuronConnectionWithPreviousLayerNeuron, const double probabilityANNneuronConnectionWithAllPreviousLayersNeurons);
		#ifdef ANN_SUBNETS
		public: ANNneuron* formAdvancedNeuralNetwork(ANNneuron* firstInputNeuron, const long numberOfInputNeurons, long numberOfOutputNeurons, const bool useSubnetDependentNumberOfLayers, const double probabilityOfSubnetCreation, const long maxNumberOfRecursiveSubnets, const long numberOfLayers, const int layerDivergenceType, const double meanLayerDivergenceFactor, const double probabilityANNneuronConnectionWithPreviousLayerNeuron, const double probabilityANNneuronConnectionWithAllPreviousLayersNeurons);
		#endif
			private: ANNneuron* formNonDistinctLayeredNeuralNetwork(ANNneuron* firstInputNeuronInNetwork, const long numberOfInputNeurons, long numberOfOutputNeurons, const long numberOfLayers, const double probabilityOfSubnetCreation, const long maxNumberOfRecursiveSubnets, const bool useSubnetDependentNumberOfLayers, const double subnetNumberOfLayersModifier, const int layerDivergenceType, const double meanLayerDivergenceFactor, const double probabilityANNneuronConnectionWithPreviousLayerNeuron, const double probabilityANNneuronConnectionWithAllPreviousLayersNeurons);
				private: void createInputLayerInNeuralNetwork(ANNneuron* firstInputNeuronInNetwork, const long numberOfInputNeurons);
				private: void createInputLayerInNeuralNetwork2D(ANNneuron* firstInputNeuronInNetwork, const long numberOfInputNeurons);
				public: ANNneuron* fillNonDistinctHiddenLayer(ANNneuron* firstNeuronInCurrentLayer, const long numberOfInputNeurons, long numberOfOutputNeurons, const long numberOfNeuronsInCurrentLayer, const long currentNumberOfLayers, const long numberOfLayers, const double probabilityOfSubnetCreation, const long maxNumberOfRecursiveSubnets, const long currentNumberOfRecursiveSubnets, const bool useSubnetDependentNumberOfLayers, const double subnetNumberOfLayersModifier, const int layerDivergenceType, const double meanLayerDivergenceFactor, const double probabilityANNneuronConnectionWithPreviousLayerNeuron, ANNneuron* firstInputNeuronInSubnet, const double probabilityANNneuronConnectionWithAllPreviousLayersNeurons, const bool onlyLinkWithPreviousAndFirstLayer);
					private: void createNewFrontLayer(ANNneuron* firstNeuronInCurrentLayer, ANNneuron* firstNeuronInNewFrontLayer, const long numberOfNeuronsInNewFrontLayer, const long currentNumberOfLayers, const long numberOfLayers);
					private: void linkNewFrontLayerWithPreviousLayers(ANNneuron* firstNeuronInCurrentLayer, ANNneuron* firstNeuronInNewFrontLayer, const double probabilityANNneuronConnectionWithPreviousLayerNeuron, ANNneuron* firstInputNeuronInSubnet, const double probabilityANNneuronConnectionWithAllPreviousLayersNeurons, const long numberOfInputNeurons, const long numberOfOutputNeurons, const long currentNumberOfLayers, const bool onlyLinkWithPreviousAndFirstLayer);
					private: void linkNewFrontLayerWithPreviousLayers2D(ANNneuron* firstNeuronInCurrentLayer, ANNneuron* firstNeuronInNewFrontLayer, const double probabilityANNneuronConnectionWithPreviousLayerNeuron, ANNneuron* firstInputNeuronInSubnet, const double probabilityANNneuronConnectionWithAllPreviousLayersNeurons, const long numberOfInputNeurons, const long numberOfOutputNeurons, const long currentNumberOfLayers, const long numberOfLayers, const long numberOfNeuronsInCurrentLayer, const long numberOfNeuronsInNewFrontLayer, const int layerDivergenceType, const bool onlyLinkWithPreviousAndFirstLayer);
					private: long calcNumberOfLayersInSubnet(const long numberOfInputNeurons, const long numberOfOutputNeurons, const long currentNumberOfRecursiveSubnets, const long maxNumberOfRecursiveSubnets, const bool useSubnetDependentNumberOfLayers, const double subnetNumberOfLayersModifier);
					private: double advancedDivergenceFactor(const long currentNumberOfLayersFromNearestEndPoint, const long numberOfLayers);


	private: long calculateNumberOfNeuronsInNewFrontLayer(const long numberOfInputNeurons, long numberOfOutputNeurons, const long numberOfNeuronsInCurrentLayer, const long currentNumberOfLayers, const long numberOfLayers, const double meanLayerDivergenceFactor, const int layerDivergenceType);


	private: double calculateDistanceBetween2Points(const double xPositionOfUnit1, const double xPositionOfUnit2, const double yPositionOfUnit1, const double yPositionOfUnit2);

	#ifdef ANN_ALGORITHM_MEMORY_NETWORK
	private: void addSideConnectionsForLayer(ANNneuron* firstNeuronInLayer);
	private: void addSideConnectionsForLayer2D(ANNneuron* firstNeuronInLayer, const int layerDivergenceType, const long currentNumberOfLayers, const long numberOfLayers);
	private: void addSideConnectionIfNotAlreadyAdded(ANNneuron* currentNeuronL1, ANNneuron* currentNeuronL2);
	#endif
	#endif
	
	public: int countNumberOfLayersInSubnet(ANNneuron* firstInputNeuronInSubnet);
};


#endif

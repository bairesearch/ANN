/*******************************************************************************
 *
 * File Name: ANNFormation.h
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2010 Baxter AI (baxterai.com)
 * Project: Advanced Neural Network (ANN)
 * Project Version: 3a6b 30-Apr-2012
 * Comments:
 *
 *******************************************************************************/

#ifndef HEADER_ANN_FORMATION
#define HEADER_ANN_FORMATION

#include "ANNneuronClass.h"

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
#define LAYER_DIVERGENCE_TYPE_LINEAR_CONVERGING (1)									//see ANNFormation.cpp for diagramatic description
#define LAYER_DIVERGENCE_TYPE_LINEAR_DIVERGING_THEN_CONVERGING (2)					//see ANNFormation.cpp for diagramatic description
#define LAYER_DIVERGENCE_TYPE_NONLINEAR_DIVERGING_THEN_CONVERGING (3)				//see ANNFormation.cpp for diagramatic description
#define LAYER_DIVERGENCE_TYPE_LINEAR_DIVERGING (4)
#define LAYER_DIVERGENCE_TYPE_LINEAR_DIVERGING_SQUARE2D (5)
#define LAYER_DIVERGENCE_TYPE_LINEAR_DIVERGING_SQUARE2D_RADIALBIAS (6)
#define LAYER_DIVERGENCE_TYPE_LINEAR_CONVERGING_SQUARE2D (7)

	//USE_ADVANCED_RANDOM_SUBNET_DIVERGENCE
#define DEFAULT_MEAN_LAYER_DIVERGENCE_FACTOR (1.5)

#define ABS_OF_SHORT 32767.0

#define BASIC_NUMBER_OF_HIDDEN_NEURONS_HEURISTIC_ANNTH (1.0F)	//only used for backwards compatability

void formSimpleNeuralNet(NeuronContainer * firstInputNeuron, NeuronContainer * firstOutputNeuron, long numberOfInputNeurons, long numberOfHiddenNeurons, long numberOfOutputNeurons);
NeuronContainer * formNeuralNetWithOptimisedProperties(NeuronContainer * firstInputNeuronInNetwork, long numberOfInputNeurons, long numberOfOutputNeurons, long numberOfLayers);
	NeuronContainer * formNeuralNet(NeuronContainer * firstInputNeuron, long numberOfInputNeurons, long numberOfOutputNeurons, long numberOfLayers, int layerDivergenceType, double meanLayerDivergenceFactor, double probabilityNeuronConnectionWithPreviousLayerNeuron, double probabilityNeuronConnectionWithAllPreviousLayersNeurons);
	#ifdef ANN
	NeuronContainer * formAdvancedNeuralNetwork(NeuronContainer * firstInputNeuron, long numberOfInputNeurons, long numberOfOutputNeurons, bool useSubnetDependentNumberOfLayers, double probabilityOfSubnetCreation, long maxNumberOfRecursiveSubnets, long numberOfLayers, int layerDivergenceType, double meanLayerDivergenceFactor, double probabilityNeuronConnectionWithPreviousLayerNeuron, double probabilityNeuronConnectionWithAllPreviousLayersNeurons);
	#endif
		NeuronContainer * formNondistinctLayeredNeuralNetwork(NeuronContainer * firstInputNeuronInNetwork, long numberOfInputNeurons, long numberOfOutputNeurons, long numberOfLayers, double probabilityOfSubnetCreation, long maxNumberOfRecursiveSubnets, bool useSubnetDependentNumberOfLayers, double subnetNumberOfLayersModifier, int layerDivergenceType, double meanLayerDivergenceFactor, double probabilityNeuronConnectionWithPreviousLayerNeuron, double probabilityNeuronConnectionWithAllPreviousLayersNeurons);
			void createInputLayerInNeuralNetwork(NeuronContainer * firstInputNeuronInNetwork, long numberOfInputNeurons);
			void createInputLayerInNeuralNetwork2D(NeuronContainer * firstInputNeuronInNetwork, long numberOfInputNeurons);
			NeuronContainer * fillNondistinctHiddenLayer(NeuronContainer * firstNeuronInCurrentLayer, long numberOfInputNeurons, long numberOfOutputNeurons, long numberOfNeuronsInCurrentLayer, long currentNumberOfLayers, long numberOfLayers, double probabilityOfSubnetCreation, long maxNumberOfRecursiveSubnets, long currentNumberOfRecursiveSubnets, bool useSubnetDependentNumberOfLayers, double subnetNumberOfLayersModifier, int layerDivergenceType, double meanLayerDivergenceFactor, double probabilityNeuronConnectionWithPreviousLayerNeuron, NeuronContainer * firstInputNeuronInSubnet, double probabilityNeuronConnectionWithAllPreviousLayersNeurons);
				void createNewFrontLayer(NeuronContainer * firstNeuronInCurrentLayer, NeuronContainer * firstNeuronInNewFrontLayer, long numberOfNeuronsInNewFrontLayer, long currentNumberOfLayers, long numberOfLayers);
				void linkNewFrontLayerWithPreviousLayers(NeuronContainer * firstNeuronInCurrentLayer, NeuronContainer * firstNeuronInNewFrontLayer, double probabilityNeuronConnectionWithPreviousLayerNeuron, NeuronContainer * firstInputNeuronInSubnet, double probabilityNeuronConnectionWithAllPreviousLayersNeurons, long numberOfInputNeurons, long numberOfOutputNeurons, long currentNumberOfLayers);
				void linkNewFrontLayerWithPreviousLayers2D(NeuronContainer * firstNeuronInCurrentLayer, NeuronContainer * firstNeuronInNewFrontLayer, double probabilityNeuronConnectionWithPreviousLayerNeuron, NeuronContainer * firstInputNeuronInSubnet, double probabilityNeuronConnectionWithAllPreviousLayersNeurons, long numberOfInputNeurons, long numberOfOutputNeurons, long currentNumberOfLayers, long numberOfLayers, long numberOfNeuronsInCurrentLayer, long numberOfNeuronsInNewFrontLayer, int layerDivergenceType);
				long calcNumberOfLayersInSubnet(long numberOfInputNeurons, long numberOfOutputNeurons, long currentNumberOfRecursiveSubnets, long maxNumberOfRecursiveSubnets, bool useSubnetDependentNumberOfLayers, double subnetNumberOfLayersModifier);
				double advancedDivergenceFactor(long currentNumberOfLayersFromNearestEndPoint, long numberOfLayers);


long calculateNumberOfNeuronsInNewFrontLayer(long numberOfInputNeurons, long numberOfOutputNeurons, long numberOfNeuronsInCurrentLayer, long currentNumberOfLayers, long numberOfLayers, double meanLayerDivergenceFactor, int layerDivergenceType);


double calculateDistanceBetween2Points(double positionXOfUnit1, double positionXOfUnit2, double positionYOfUnit1, double positionYOfUnit2);





#endif

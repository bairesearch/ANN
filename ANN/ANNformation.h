/*******************************************************************************
 *
 * This file is part of BAIPROJECT.
 *
 * BAIPROJECT is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License version 3
 * only, as published by the Free Software Foundation. The use of
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
 * File Name: ANNformation.h
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2016 Baxter AI (baxterai.com)
 * Project: Artificial Neural Network (ANN)
 * Project Version: 3i19e 15-December-2016
 * Comments:
 *
 *******************************************************************************/


#ifndef HEADER_ANN_FORMATION
#define HEADER_ANN_FORMATION

#include "ANNneuronClass.h"
#include "ANNneuronConnectionClass.h"

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

#ifdef ANN_ALGORITHM_CLASSIFICATION_NETWORK
void formNeuralNetworkInputLayer(ANNneuron* firstInputNeuronInNetwork, int numberOfInputNeurons);
#else
void formSimpleNeuralNet(ANNneuron* firstInputNeuron, ANNneuron* firstOutputNeuron, long numberOfInputNeurons, long numberOfHiddenNeurons, long numberOfOutputNeurons);
ANNneuron* formNeuralNetWithOptimisedProperties(ANNneuron* firstInputNeuronInNetwork, long numberOfInputNeurons, long numberOfOutputNeurons, long numberOfLayers);
	ANNneuron* formNeuralNet(ANNneuron* firstInputNeuron, long numberOfInputNeurons, long numberOfOutputNeurons, long numberOfLayers, int layerDivergenceType, double meanLayerDivergenceFactor, double probabilityANNneuronConnectionWithPreviousLayerNeuron, double probabilityANNneuronConnectionWithAllPreviousLayersNeurons);
	#ifdef ANN_SUBNETS
	ANNneuron* formAdvancedNeuralNetwork(ANNneuron* firstInputNeuron, long numberOfInputNeurons, long numberOfOutputNeurons, bool useSubnetDependentNumberOfLayers, double probabilityOfSubnetCreation, long maxNumberOfRecursiveSubnets, long numberOfLayers, int layerDivergenceType, double meanLayerDivergenceFactor, double probabilityANNneuronConnectionWithPreviousLayerNeuron, double probabilityANNneuronConnectionWithAllPreviousLayersNeurons);
	#endif
		ANNneuron* formNonDistinctLayeredNeuralNetwork(ANNneuron* firstInputNeuronInNetwork, long numberOfInputNeurons, long numberOfOutputNeurons, long numberOfLayers, double probabilityOfSubnetCreation, long maxNumberOfRecursiveSubnets, bool useSubnetDependentNumberOfLayers, double subnetNumberOfLayersModifier, int layerDivergenceType, double meanLayerDivergenceFactor, double probabilityANNneuronConnectionWithPreviousLayerNeuron, double probabilityANNneuronConnectionWithAllPreviousLayersNeurons);
			void createInputLayerInNeuralNetwork(ANNneuron* firstInputNeuronInNetwork, long numberOfInputNeurons);
			void createInputLayerInNeuralNetwork2D(ANNneuron* firstInputNeuronInNetwork, long numberOfInputNeurons);
			ANNneuron* fillNonDistinctHiddenLayer(ANNneuron* firstNeuronInCurrentLayer, long numberOfInputNeurons, long numberOfOutputNeurons, long numberOfNeuronsInCurrentLayer, long currentNumberOfLayers, long numberOfLayers, double probabilityOfSubnetCreation, long maxNumberOfRecursiveSubnets, long currentNumberOfRecursiveSubnets, bool useSubnetDependentNumberOfLayers, double subnetNumberOfLayersModifier, int layerDivergenceType, double meanLayerDivergenceFactor, double probabilityANNneuronConnectionWithPreviousLayerNeuron, ANNneuron* firstInputNeuronInSubnet, double probabilityANNneuronConnectionWithAllPreviousLayersNeurons);
				void createNewFrontLayer(ANNneuron* firstNeuronInCurrentLayer, ANNneuron* firstNeuronInNewFrontLayer, long numberOfNeuronsInNewFrontLayer, long currentNumberOfLayers, long numberOfLayers);
				void linkNewFrontLayerWithPreviousLayers(ANNneuron* firstNeuronInCurrentLayer, ANNneuron* firstNeuronInNewFrontLayer, double probabilityANNneuronConnectionWithPreviousLayerNeuron, ANNneuron* firstInputNeuronInSubnet, double probabilityANNneuronConnectionWithAllPreviousLayersNeurons, long numberOfInputNeurons, long numberOfOutputNeurons, long currentNumberOfLayers);
				void linkNewFrontLayerWithPreviousLayers2D(ANNneuron* firstNeuronInCurrentLayer, ANNneuron* firstNeuronInNewFrontLayer, double probabilityANNneuronConnectionWithPreviousLayerNeuron, ANNneuron* firstInputNeuronInSubnet, double probabilityANNneuronConnectionWithAllPreviousLayersNeurons, long numberOfInputNeurons, long numberOfOutputNeurons, long currentNumberOfLayers, long numberOfLayers, long numberOfNeuronsInCurrentLayer, long numberOfNeuronsInNewFrontLayer, int layerDivergenceType);
				long calcNumberOfLayersInSubnet(long numberOfInputNeurons, long numberOfOutputNeurons, long currentNumberOfRecursiveSubnets, long maxNumberOfRecursiveSubnets, bool useSubnetDependentNumberOfLayers, double subnetNumberOfLayersModifier);
				double advancedDivergenceFactor(long currentNumberOfLayersFromNearestEndPoint, long numberOfLayers);


long calculateNumberOfNeuronsInNewFrontLayer(long numberOfInputNeurons, long numberOfOutputNeurons, long numberOfNeuronsInCurrentLayer, long currentNumberOfLayers, long numberOfLayers, double meanLayerDivergenceFactor, int layerDivergenceType);


double calculateDistanceBetween2Points(double xPositionOfUnit1, double xPositionOfUnit2, double yPositionOfUnit1, double yPositionOfUnit2);

#ifdef ANN_ALGORITHM_MEMORY_NETWORK
void addSideConnectionsForLayer(ANNneuron* firstNeuronInLayer);
void addSideConnectionsForLayer2D(ANNneuron* firstNeuronInLayer, int layerDivergenceType, long currentNumberOfLayers, long numberOfLayers);
void addSideConnectionIfNotAlreadyAdded(ANNneuron* currentNeuronL1, ANNneuron* currentNeuronL2);
#endif
#endif

#endif

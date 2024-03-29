 /*******************************************************************************
 *
 * License: GNU Affero General Public License version 3 (AGPLv3)
 *
 *******************************************************************************/

/*******************************************************************************
 *
 * File Name: ANNalgorithmBreakawayNetwork.cpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2022 Baxter AI (baxterai.com)
 * Project: Artificial Neural Network (ANN)
 * Project Version: 3q1a 05-June-2022
 * Comments:
 * /
 *******************************************************************************/


#include "ANNalgorithmBreakawayNetwork.hpp"

#ifdef ANN_ALGORITHM_BREAKAWAY_NETWORK

bool ANNalgorithmBreakawayNetworkClass::trainNeuralNetworkBreakaway(ANNneuron* firstInputNeuronInNetwork, ANNneuron** firstOutputNeuronInNetwork, const int numberOfInputNeurons, const int numberOfOutputNeurons, const bool simple, const int numEpochs, const int maxFolds, const int maxEpochs, ANNexperience* firstExperienceInDataSet, const int64_t numberOfExperiences, const int sizeOfSupernet, int numberOfLayersOriginal)
{
	bool result = true;
	
	#ifdef ANN_ALGORITHM_BACKPROPAGATION
	int layerDivergenceType = LAYER_DIVERGENCE_TYPE_LINEAR_CONVERGING;
	double meanLayerDivergenceFactor = DEFAULT_MEAN_LAYER_DIVERGENCE_FACTOR;
	double probabilityANNneuronConnectionWithPreviousLayerNeuron = DEFAULT_PROBABILITY_NEURON_CONNECTION_WITH_PREVIOUS_LAYER_NEURON_ANNTH;
	#ifdef ANN_ALGORITHM_BREAKAWAY_NETWORK_LINK_WITH_ALL_PREVIOUS_LAYERS
	bool onlyLinkWithPreviousAndFirstLayer = false;
	double probabilityANNneuronConnectionWithAllPreviousLayersNeurons = 0.25;	//this should be a) dynamic or b) optimised?	
	#else
	bool onlyLinkWithPreviousAndFirstLayer = true;
	double probabilityANNneuronConnectionWithAllPreviousLayersNeurons = 1.0;
	#endif
	//#ifdef ANN_ALGORITHM_BACKPROPAGATION_NETWORK
	double probabilityOfSubnetCreation = 0.0;	/*no divergence = no ANN*/
	int64_t maxNumberOfRecursiveSubnets = 0;		/*no recursive subnets = no ANN*/
	double subnetNumberOfLayersModifier = 0.0;
	bool useSubnetDependentNumberOfLayers = false;
	//#endif
	#endif
	
	int currentNumberOfLayers = numberOfLayersOriginal;
	
	for(int i=0; i<sizeOfSupernet; i++)
	{
		if(i != 0)
		{
			//assume network has already been initialised by ANNmain
		
			#ifdef ANN_ALGORITHM_BREAKAWAY_NETWORK_ADD_LAYERS
			int numberOfNewLayers = 1;		
			//addLayer(firstInputNeuronInNetwork, firstOutputNeuronInNetwork, numberOfInputNeurons, numberOfOutputNeurons);		
			#endif
			#ifdef ANN_ALGORITHM_BREAKAWAY_NETWORK_ADD_SUPERNETS
			int numberOfNewLayers = ANN_ALGORITHM_BREAKAWAY_NETWORK_ADD_SUPERNETS_NUMBER_OF_NEW_LAYERS;	
			//addSubnet(firstInputNeuronInNetwork, firstOutputNeuronInNetwork, numberOfInputNeurons, numberOfOutputNeurons);	
			#endif
			ANNneuron* firstInputNeuronInNewLayerOrNetworkPreviousLayer = *firstOutputNeuronInNetwork;
			*firstOutputNeuronInNetwork = ANNformation.fillNonDistinctHiddenLayer(*firstOutputNeuronInNetwork, numberOfInputNeurons, numberOfOutputNeurons, numberOfInputNeurons, currentNumberOfLayers, currentNumberOfLayers+numberOfNewLayers, probabilityOfSubnetCreation, maxNumberOfRecursiveSubnets, 1, useSubnetDependentNumberOfLayers, subnetNumberOfLayersModifier, layerDivergenceType, meanLayerDivergenceFactor, probabilityANNneuronConnectionWithPreviousLayerNeuron, firstInputNeuronInNetwork, probabilityANNneuronConnectionWithAllPreviousLayersNeurons, onlyLinkWithPreviousAndFirstLayer);
			currentNumberOfLayers = currentNumberOfLayers+numberOfNewLayers;		
		
			ANNalgorithmBackpropagation.resetNeuralNetworkWithRandomBiasAndWeights(firstInputNeuronInNewLayerOrNetworkPreviousLayer);	//only reset biases/weights for the new layer (verify that biases can be reset for the neurons on the previous output layer; as this will be done)		
		}
		else
		{
			ANNalgorithmBackpropagation.resetNeuralNetworkWithRandomBiasAndWeights(firstInputNeuronInNetwork);
		}
					
		if(simple)
		{
			ANNalgorithmBackpropagation.trainNeuralNetworkBackpropagationSimpleNoReset(firstInputNeuronInNetwork, *firstOutputNeuronInNetwork, numberOfInputNeurons, numberOfOutputNeurons, numEpochs, firstExperienceInDataSet, numExperiences);
		}
		else
		{	
			cout << "trainNeuralNetworkBreakaway{} error: !simple training (ie with folds;) has not yet been coded; user must train with preset number of folds" << endl;
			exit(EXIT_ERROR);
			//ANNalgorithmBackpropagation.trainNeuralNetworkBackpropagationNoReset(firstInputNeuronInNetwork, *firstOutputNeuronInNetwork, numberOfInputNeurons, numberOfOutputNeurons, maxFolds, firstExperienceInDataSet, numExperiences, maxEpochs);
		}
	}
	
	return result;
}

#endif


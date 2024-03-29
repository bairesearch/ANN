 /*******************************************************************************
 *
 * License: GNU Affero General Public License version 3 (AGPLv3)
 *
 *******************************************************************************/

/*******************************************************************************
 *
 * File Name: ANNbiologicalConstructs.cpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2022 Baxter AI (baxterai.com)
 * Project: Artificial Neural Network (ANN)
 * Project Version: 3q1a 05-June-2022
 * Comments: CONTAINS EXAMPLE CODE AND IS NOT COMPLETE
 *			how to record movement information? (Eg recurrent network required) how to get synthetic network to modify its focus; left, right, in, out towards the objects of most interest?
 * /
 *******************************************************************************/


#include "ANNformation.hpp"
#include "ANNalgorithmBackpropagation.hpp"

#include "ANNbiologicalConstructs.hpp"


//pseudo code only
void formNeuralNetworkBiologicalConstructs()
{
	//this function is useless without some real data, however it should be pretty clear as to its usage once a data feed can be established (Eg with a robot)

	ANNneuron* firstNeuronInVisualCortexSubnet = new ANNneuron();
	formVisualNetwork(firstNeuronInVisualCortex);

	ANNneuron* firstNeuronInAudialCentreSubnet = new ANNneuron();
	formAudialCentreSubnet(firstNeuronInAudialCentreSubnet);

	ANNneuron* firstNeuronInSyntheticBrainSubnet = new ANNneuron();
	formSyntheticBrainSubnet(firstNeuronInSyntheticBrainSubnet, firstNeuronInVisualCortexSubnet, firstNeuronInAudialCentreSubnet);

}

void formVisualNetwork(ANNneuron* firstNeuronInVisualCortexSubnet)
{
	ANNneuron* firstNeuronInLeftEyeVisualFieldSubnet = new ANNneuron();
	ANNneuron* firstNeuronInRightEyeVisualFieldSubnet = new ANNneuron();
	ANNneuron* firstNeuronInFocusDistanceSubnet = new ANNneuron();			//single input neuron identifying the current focus distance of the eyes (effects 1-1 mapping of L/R eye data)
	ANNneuron* firstNeuronInParallaxRecognitionSubnet = new ANNneuron();
	ANNneuron* firstNeuronInObjectRecognitionSubnet = new ANNneuron();

	form2dVisualFieldSubnet(firstNeuronInLeftEyeVisualFieldSubnet, NUMBER_OF_HORIZONTAL_PIXELS_ANNTH, NUMBER_OF_VERTICAL_PIXELS_ANNTH);
	form2dVisualFieldSubnet(firstNeuronInRightEyeVisualFieldSubnet, NUMBER_OF_HORIZONTAL_PIXELS_ANNTH, NUMBER_OF_VERTICAL_PIXELS_ANNTH);

	formParallaxRecognitionSubnet(firstNeuronInParallaxRecognitionSubnet, firstNeuronInLeftEyeVisualFieldSubnet, firstNeuronInRightEyeVisualFieldSubnet, firstNeuronInFocusDistanceSubnet);
	formObjectRecognitionSubnet(firstNeuronInLeftEyeVisualFieldSubnet, NUMBER_OF_OBJECT_RECOGNITION_HORIZONTAL_PIXELS_ANNTH, NUMBER_OF_OBJECT_RECOGNITION_VERTICAL_PIXELS_ANNTH);


	//NOW.. to form firstNeuronInVisualCortexSubnet, add the formParallaxRecognitionSubnet and the firstNeuronInObjectRecognitionSubnet in parallel (on a single layer) and have their outputs mixed on the next layer?
}

void formParallaxRecognitionSubnet(ANNneuron* firstNeuronInParallaxRecognitionSubnet, ANNneuron* firstNeuronInLeftEyeVisualFieldSubnet, ANNneuron* firstNeuronInRightEyeVisualFieldSubnet, ANNneuron* firstNeuronInFocusDistanceSubnet)
{
	//NOW.. to formParallaxRecognitionSubnet, connect the low level neurons of each eye, if the neural network recognises the same information in all kernels (Eg 10x10 pixel groups) of each eye, it is assumed an object has been focused, and its distance can be determined based upon the current focal distance of the eyes
}

void formSyntheticBrainSubnet(ANNneuron* firstNeuronInSyntheticBrainSubnet, ANNneuron* firstNeuronInVisualCortexSubnet, ANNneuron* firstNeuronInAudialCentreSubnet)
{
	//NOW.. to formfirstNeuronInSyntheticBrainSubnet, add the firstNeuronInVisualCortexSubnet and firstNeuronInAudialCentreSubnet in parallel (on a single layer) and have their outputs mixed on the next layer
}

void form2dVisualFieldSubnet(ANNneuron* firstNeuronInVisualFieldSubnet, int64_t visualArrayWidth, int64_t visualArrayHeight)
{
	//NOW.. to form2dVisualFieldSubnet, form a network such that each input neuron (cone/rod/pixel) is first connected to neighboring input neurons in the 2D field, enabling contrast recognition and object outline angles to be determined.

	numOutputNeurons >> numInputNeurons
}

void formObjectRecognitionSubnet(ANNneuron* firstNeuronInEyeVisualFieldSubnet, int64_t objectRecognitionVisualArrayWidth, int64_t objectRecognitionVisualArrayHeight)
{
	//NOW.. to formObjectRecognitionSubnet, take the central section of the visual field, and form a dense complex network
}


		//in higher layers in the visual cortex, this low level information is then brought together in higher layers to store memory of certain shapes/objects in the network.
			//object recognition should  be performed primarily on the central visual field neurons. movement detection should be performed primarily on the outer visual field neurons

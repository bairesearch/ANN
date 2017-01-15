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
 * File Name: ANNbiologicalConstructs.cpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2012 Baxter AI (baxterai.com)
 * Project: Artificial Neural Network (ANN)
 * Project Version: 3c4b 01-December-2012
 * Comments: CONTAINS EXAMPLE CODE AND IS NOT COMPLETE
 *			how to record movement information? (Eg recurrent network required) how to get synthetic network to modify its focus; left, right, in, out towards the objects of most interest?
 *******************************************************************************/

#include <ctime>
#include <cstdlib>	//for random number generation
#include <cmath>

#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <string.h>
#include <iostream>
#include <fstream>
using namespace std;


#include "ANNformation.h"
#include "ANNtraining.h"

#include "ANNbiologicalConstructs.h"


//pseudo code only
void formNeuralNetworkBiologicalConstructs()
{
	//this function is useless without some real data, however it should be pretty clear as to its usage once a data feed can be established (Eg with a robot)

	NeuronContainer * firstNeuronInVisualCortexSubnet = new NeuronContainer();
	formVisualNetwork(firstNeuronInVisualCortex);

	NeuronContainer * firstNeuronInAudialCentreSubnet = new NeuronContainer();
	formAudialCentreSubnet(firstNeuronInAudialCentreSubnet);

	NeuronContainer * firstNeuronInSyntheticBrainSubnet = new NeuronContainer();
	formSyntheticBrainSubnet(firstNeuronInSyntheticBrainSubnet, firstNeuronInVisualCortexSubnet, firstNeuronInAudialCentreSubnet);

}

void formVisualNetwork(NeuronContainer * firstNeuronInVisualCortexSubnet)
{
	NeuronContainer * firstNeuronInLeftEyeVisualFieldSubnet = new NeuronContainer();
	NeuronContainer * firstNeuronInRightEyeVisualFieldSubnet = new NeuronContainer();
	NeuronContainer * firstNeuronInFocusDistanceSubnet = new NeuronContainer();			//single input neuron identifying the current focus distance of the eyes (effects 1-1 mapping of L/R eye data)
	NeuronContainer * firstNeuronInParallaxRecognitionSubnet = new NeuronContainer();
	NeuronContainer * firstNeuronInObjectRecognitionSubnet = new NeuronContainer();

	form2dVisualFieldSubnet(firstNeuronInLeftEyeVisualFieldSubnet, NUMBER_OF_HORIZONTAL_PIXELS_ANNTH, NUMBER_OF_VERTICAL_PIXELS_ANNTH);
	form2dVisualFieldSubnet(firstNeuronInRightEyeVisualFieldSubnet, NUMBER_OF_HORIZONTAL_PIXELS_ANNTH, NUMBER_OF_VERTICAL_PIXELS_ANNTH);

	formParallaxRecognitionSubnet(firstNeuronInParallaxRecognitionSubnet, firstNeuronInLeftEyeVisualFieldSubnet, firstNeuronInRightEyeVisualFieldSubnet, firstNeuronInFocusDistanceSubnet);
	formObjectRecognitionSubnet(firstNeuronInLeftEyeVisualFieldSubnet, NUMBER_OF_OBJECT_RECOGNITION_HORIZONTAL_PIXELS_ANNTH, NUMBER_OF_OBJECT_RECOGNITION_VERTICAL_PIXELS_ANNTH);


	//NOW.. to form firstNeuronInVisualCortexSubnet, add the formParallaxRecognitionSubnet and the firstNeuronInObjectRecognitionSubnet in parallel (on a single layer) and have their outputs mixed on the next layer?
}

void formParallaxRecognitionSubnet(NeuronContainer * firstNeuronInParallaxRecognitionSubnet, NeuronContainer * firstNeuronInLeftEyeVisualFieldSubnet, NeuronContainer * firstNeuronInRightEyeVisualFieldSubnet, NeuronContainer * firstNeuronInFocusDistanceSubnet)
{
	//NOW.. to formParallaxRecognitionSubnet, connect the low level neurons of each eye, if the neural network recognises the same information in all kernels (Eg 10x10 pixel groups) of each eye, it is assumed an object has been focused, and its distance can be determined based upon the current focal distance of the eyes
}

void formSyntheticBrainSubnet(NeuronContainer * firstNeuronInSyntheticBrainSubnet, NeuronContainer * firstNeuronInVisualCortexSubnet, NeuronContainer * firstNeuronInAudialCentreSubnet)
{
	//NOW.. to formfirstNeuronInSyntheticBrainSubnet, add the firstNeuronInVisualCortexSubnet and firstNeuronInAudialCentreSubnet in parallel (on a single layer) and have their outputs mixed on the next layer
}

void form2dVisualFieldSubnet(NeuronContainer * firstNeuronInVisualFieldSubnet, long visualArrayWidth, long visualArrayHeight)
{
	//NOW.. to form2dVisualFieldSubnet, form a network such that each input neuron (cone/rod/pixel) is first connected to neighboring input neurons in the 2D field, enabling contrast recognition and object outline angles to be determined.

	numOutputNeurons >> numInputNeurons
}

void formObjectRecognitionSubnet(NeuronContainer * firstNeuronInEyeVisualFieldSubnet, long objectRecognitionVisualArrayWidth, long objectRecognitionVisualArrayHeight)
{
	//NOW.. to formObjectRecognitionSubnet, take the central section of the visual field, and form a dense complex network
}


		//in higher layers in the visual cortex, this low level information is then brought together in higher layers to store memory of certain shapes/objects in the network.
			//object recognition should  be performed primarily on the central visual field neurons. movement detection should be performed primarily on the outer visual field neurons

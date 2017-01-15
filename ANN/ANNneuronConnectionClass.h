/*******************************************************************************
 *
 * This file is part of BAIPROJECT.
 *
 * BAIPROJECT is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License version 3
 * only, as published by the Free Software Foundation. The use of
 * intermediary programs or interfaces including file i/o is considered
 * remote network interaction.
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
 * File Name: ANNneuronConnectionClass.h
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2014 Baxter AI (baxterai.com)
 * Project: Artificial Neural Network (ANN)
 * Project Version: 3d3b 24-May-2014
 * Comments:
 *
 *******************************************************************************/

 //IMPORTANT CODING NOTE - on 10-dec-06/1b6b I have started to remove the neuronReference class as circular referencing does not appear to be allowed in C++
//NB when create NeuronList class change all referecnes to "...List->firstFrontNeuronConnectionContainer.." to "...List->neuronReferences"

/************************************************************ Neural Network Class Definitions ******************************************************/


#ifndef HEADER_ANN_NEURON_CONNECTION_CLASS
#define HEADER_ANN_NEURON_CONNECTION_CLASS

class Neuron;

class NeuronConnection
{
public:

	long frontNeuronID;		//temporary variable required for neural net creation from xml files

	NeuronConnection(void);
	~NeuronConnection(void);

	double weight;
	double storedWeight;

	Neuron * frontNeuron;
	Neuron * backNeuron;
};

#endif

/************************************************************ End Neural Network Class Definitions **************************************************/


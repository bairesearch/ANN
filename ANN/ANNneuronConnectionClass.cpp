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
 * File Name: ANNneuronConnectionClass.cpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2014 Baxter AI (baxterai.com)
 * Project: Artificial Neural Network (ANN)
 * Project Version: 3d2e 14-April-2014
 * Comments:
 *
 *******************************************************************************/

#include "ANNneuronConnectionClass.h"

/************************************************************ Neural Network Class Definitions ******************************************************/

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







NeuronConnection::NeuronConnection(void)
{
	frontNeuronID = 0;

	weight = 0.0;		//this value must be initialised explicity
	storedWeight = 0.0; 	//this value must be initialised explicity
	frontNeuron = NULL;
	backNeuron = NULL;
}

NeuronConnection::~NeuronConnection(void)
{
	if(frontNeuron != NULL)
	{
		delete frontNeuron;
	}

	/*
	if(backNeuron != NULL)
	{
		delete backNeuron;
	}
	*/

	//do not delete neurons in this destructor
}

/************************************************************ End Neural Network Class Definitions **************************************************/



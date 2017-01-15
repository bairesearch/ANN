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
 * File Name: ANNmain.h
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2016 Baxter AI (baxterai.com)
 * Project: Artificial Neural Network (ANN)
 * Project Version: 3i19a 15-December-2016
 * Comments: TH = Test Harness
 *
 *******************************************************************************/


#ifndef HEADER_ANN_MAIN
#define HEADER_ANN_MAIN

#include "ANNglobalDefs.h"

//#define LOAD_NETWORK_FROM_XML_FILE
//#define SKIP_NETWORK_TRAINING_AND_ONLY_OUTPUT_VIS
#define UNNECESSARILY_FANCY


#define EXPERIENCE_DATASET_FILE_NAME_ANNTH "new-thyroid.data"
#define NET_XML_FILE_NAME "net.xml"
#define NET_XML_FILE_NAME_TRAINED "trainedNetwork.xml"

//#define TEST_LEVEL_0_ANNTH			//emulate original C Code - nn 1aFinal with optimum number of hidden neurons
//#define TEST_LEVEL_1_ANNTH			//emulate original C Code - nn 1aFinal with range of hidden neurons
//#define TEST_LEVEL_2_ANNTH			//without subnets
//#define TEST_LEVEL_3_ANNTH			//without subnets, with range of nn layers
#define TEST_LEVEL_4_ANNTH			//with subnets
//#define TEST_LEVEL_4B_ANNTH			//with subnets, with range of nn subnets, layers
//#define TEST_LEVEL_5_ANNTH			//with subnets, with range of nn subnets, layers, and divergence
//#define TEST_LEVEL_6_ANNTH			//with subnets,	with range of nn subnets
//#define TEST_LEVEL_7_ANNTH			//with subnets, ...



int main(int argc,char* *argv);

bool mainUI();

bool loadNetworkFromXML();
bool loadExperienceDataFile();
#ifndef ANN_ALGORITHM_CLASSIFICATION_NETWORK
bool createNetwork();
#endif
bool trainNetwork(bool advancedTraining);
bool outputNetworkToXML();
bool outputNetworkAsVectorGraphics();


#endif






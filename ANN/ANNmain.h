/*******************************************************************************
 *
 * File Name: ANNmain.h
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2010 Baxter AI (baxterai.com)
 * Project: Advanced Neural Network (ANN)
 * Project Version: 3a6a 20-Mar-2012
 * Comments: TH = Test Harness
 *
 *******************************************************************************/

#ifndef HEADER_ANN_MAIN
#define HEADER_ANN_MAIN


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



int main(int argc,char **argv);

bool mainUI();

bool loadANetworkFromXML();
bool loadAnExperienceDataFile();
bool createANetwork();
bool trainANetwork();
bool outputANetworkToXML();
bool outputANetworkAsVectorGraphics();


#endif






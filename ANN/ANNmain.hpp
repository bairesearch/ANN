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
 * File Name: ANNmain.hpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2018 Baxter AI (baxterai.com)
 * Project: Artificial Neural Network (ANN)
 * Project Version: 3m12b 15-January-2018
 * Comments: TH = Test Harness
 *
 *******************************************************************************/


#ifndef HEADER_ANN_MAIN
#define HEADER_ANN_MAIN

#include "ANNglobalDefs.hpp"
#include "ANNformation.hpp"
#include "ANNalgorithmBackpropagationTraining.hpp"
#ifdef ANN_ALGORITHM_BREAKAWAY_NETWORK
#include "ANNalgorithmBreakawayNetworkTraining.hpp"
#endif
#ifdef ANN_ALGORITHM_MEMORY_NETWORK
#include "ANNalgorithmMemoryNetworkTraining.hpp"
#endif
#ifdef ANN_ALGORITHM_CLASSIFICATION_NETWORK
#include "ANNalgorithmClassificationNetworkTraining.hpp"
#endif
#include "ANNparser.hpp"
#include "ANNxmlConversion.hpp"
#include "ANNdraw.hpp"
#include "ANNdisplay.hpp"
#include "ANNdata.hpp"
#include "ANNexperienceClass.hpp"
#include "LDsvg.hpp"
#include "LDopengl.hpp"
#include "LDsprite.hpp"
#include "XMLrulesClass.hpp"

//#define LOAD_NETWORK_FROM_XML_FILE
//#define SKIP_NETWORK_TRAINING_AND_ONLY_OUTPUT_VIS
#define UNNECESSARILY_FANCY


#define EXPERIENCE_DATASET_FILE_NAME_ANNTH "new-thyroid.data"
#define NET_XML_FILE_NAME "neuralNet.xml"
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


#ifdef COMPILE_ANN
int main(const int argc,const char* *argv);
#endif

class ANNmainClass
{
	private: SHAREDvarsClass SHAREDvars;
	private: ANNxmlConversionClass ANNxmlConversion;
	private: ANNparserClass ANNparser;
	private: ANNformationClass ANNformation;
	#ifdef ANN_ALGORITHM_BACKPROPAGATION
	private: ANNalgorithmBackpropagationTrainingClass ANNalgorithmBackpropagationTraining;
	#endif
	#ifdef ANN_ALGORITHM_BREAKAWAY_NETWORK
	private: ANNalgorithmBreakawayNetworkTrainingClass ANNalgorithmBreakawayNetworkTraining;
	#endif
	#ifdef ANN_ALGORITHM_MEMORY_NETWORK
	private: ANNalgorithmMemoryNetworkTrainingClass ANNalgorithmMemoryNetworkTraining;
	#endif
	#ifdef ANN_ALGORITHM_CLASSIFICATION_NETWORK
	private: ANNalgorithmClassificationNetworkTrainingClass ANNalgorithmClassificationNetworkTraining;
	#endif
	private: ANNdisplayClass ANNdisplay;
	private: ANNexperienceClassClass ANNexperienceClass;
	private: LDopenglClass LDopengl;

	public: bool mainUI();

	private: bool loadNetworkFromXML();
	private: bool loadExperienceDataFile();
	#ifndef ANN_ALGORITHM_CLASSIFICATION_NETWORK
	private: bool createNetwork();
	#endif
	private: bool trainNetwork(const bool advancedTraining);
	private: bool outputNetworkToXML();
	private: bool outputNetworkAsVectorGraphics();
	
	public: bool trainAndOutputNeuralNetworkWithFileNames(ANNneuron* firstInputNeuronInNetwork, ANNneuron* firstOutputNeuronInNetwork, long numberOfInputNeurons, long numberOfOutputNeurons, ANNexperience* firstExperienceInList, bool addSprites, bool allowRaytrace, string* XMLNNSceneFileName, char* charstarvectorGraphicsLDRNNSceneFileName, char* charstarvectorGraphicsTALNNSceneFileName, char* charstarraytracedImagePPMNNSceneFileName, char* charstarexperienceNNSceneFileName, bool useFoldsDuringTraining, int maxOrSetNumEpochs);
};


#endif






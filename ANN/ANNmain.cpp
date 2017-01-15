/*******************************************************************************
 *
 * File Name: ANNmain.c
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2012 Baxter AI (baxterai.com)
 * Project: Advanced Neural Network (ANN)
 * Project Version: 3a8b 14-June-2012
 * Comments: TH = Test Harness
 *
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

#include "ANNmain.h"

#include "ANNFormation.h"
#include "ANNTraining.h"
#include "ANNParser.h"
#include "ANNXMLconversion.h"
#include "ANNsprite.h"
#include "ANNdisplay.h"

#include "ANNData.h"

#include "LDsvg.h"
#include "LDsprite.h"
#include "XMLrulesClass.h"

#ifdef USE_LRRC
#include "LRRCgame.h"
#endif


NeuronContainer * firstInputNeuronInNetwork;
NeuronContainer * firstOutputNeuronInNetwork;

long numberOfInputNeurons;
long numberOfOutputNeurons;

bool numberOfInputAndOutputNeuronsSelected;
bool loadedExperienceDataset;
bool formedNetwork;



static char errmessage[] = "Usage:  ANN.exe [options]"
"\n\t,where options are any of the following"
"\n"
"\n\t-idata [string]   : neural network experience data set file"
"\n"
"\n\t-layers [int]     : neural network formation number of top level layers (def: 3)"
"\n\t-ineurons [int]   : neural network number of input neurons (def: dataset dependant)"
"\n\t-oneurons [int]   : neural network number of output neurons (def: dataset dependant)"
"\n"
"\n\t-divtype [int]    : subnet divergence type (1: linear converging, "
"\n\t\t\t 2: linear diverging then converging,3: nonlinear diverging then converging, "
"\n\t\t\t 4: linear diverging, 5: diverging square 2D, 6: diverging square 2D radial bias) (def: 1)"
"\n\t-divfactor [float]: subnet divergence factor (def: 1.5)"
"\n\t-con [float]      : probability of a neuron having connection to a previous layer neuron (def: 1.0)"
"\n\t-conall [float]   : probability of a neuron having direct link with all previous layers neurons (def: 0.0)"
"\n"
"\n\t-usesubnets       : create subnets (def: no)"
"\n\t-numsubnets[int]  : maximum number of subnet layers to create (def: 2)"
"\n\t-probsubnet[float]: the probability of subnet creation (def: 0.6)"
"\n\t-subnetdepl       : use subnet dependant number of layers (def: no)"
"\n"
"\n\t-epochs [float]   : preset number of training epochs (def: auto)"
"\n\t-maxepochs [int   : maximum number of training epochs (def: 500)"
"\n\t-maxFolds [int]   : number of training folds (def: 10)"
"\n"
"\n\t-train [int]      : train neural network (1: yes, 0: no) (def: 1)"
"\n"
"\n\t-ixml [string]    : neural network definition .xml input filename (def: neuralNet.xml)"
"\n"
"\n\t-oxml [string]    : neural network definition .xml output filename (def: neuralNet.xml)"
"\n\t-oldr [string]    : neural network display .ldr output filename (def: neuralNetWithoutSprites.ldr)"
"\n\t-oldr2 [string]   : neural network display .ldr output filename with sprites (def: neuralNetWithSprites.ldr)"
"\n\t-osvg [string]    : neural network display .svg output filename (def: neuralNet.svg)"
"\n\t-oppm [string]    : neural network display .ppm opengl output filename (def: neuralNet.ppm)"
"\n\t-oppm2 [string]   : neural network display .ppm raytraced output filename (def: neuralNetRaytraced.ppm)"
"\n\t-oall [string]    : neural network display .xml/.svg/.ldr/.ppm default generic output filename (def: neuralNet)"
"\n\t-notshow          : do not display output in opengl"
"\n\t-width [int]      : raster graphics width in pixels (def: 640)"
"\n\t-height [int]     : raster graphics height in pixels (def: 480)"
"\n"
"\n\t-workingfolder [string] : working directory name for input files (def: same as exe)"
"\n\t-exefolder [string]     : exe directory name for executables ANN.exe and (def: same as exe)"
"\n\t-tempfolder [string]    : temp directory name for temporary and output files (def: same as exe)"
"\n"
"\n\t-ui               : loads text user interface upon execution (def: no)"
"\n\t-version          : print version"
"\n"
"\n\tThe program performs neural network operations - can load an experience dataset,"
"\n\tload a neural network definition xml file, form, train, test, and output neural networks"
"\n\tas .xml and .ldr "
"\n";

int main(int argc,char **argv)
{
	srand( (unsigned)time(NULL) );	//seeds randomness

	bool result = true;

	bool useInputDatasetFile = false;
	char * inputDatasetFileName = new char[100];
	strcpy(inputDatasetFileName, "dataset.data");

	long numberOfInputNeurons = 5;	//need to be generated based upon dataset or xml input file
	long numberOfOutputNeurons = 3;	//need to be generated based upon dataset or xml input file
	double numberOfLayers = 3;

	int layerDivergenceType = LAYER_DIVERGENCE_TYPE_LINEAR_CONVERGING;
	double meanLayerDivergenceFactor = DEFAULT_MEAN_LAYER_DIVERGENCE_FACTOR;
	double probabilityNeuronConnectionWithPreviousLayerNeuron = DEFAULT_PROBABILITY_NEURON_CONNECTION_WITH_PREVIOUS_LAYER_NEURON_ANNTH;
	double probabilityNeuronConnectionWithAllPreviousLayersNeurons = DEFAULT_PROBABILITY_NEURON_CONNECTION_WITH_ALL_PREVIOUS_LAYERS_NEURONS_ANNTH;
	bool useSubnets = false;
	long maxNumRecursiveSubnets = DEFAULT_ANN_MAX_NUM_RECURSIVE_SUBNETS_ANNTH;
	double probabilityOfSubnetCreation = DEFAULT_PROB_OF_SUBNET_CREATION_ANNTH;
	bool useSubnetDependentNumberOfLayers = true;

	int maxFolds = MAX_NUM_FOLDS_ANNTH;
	int maxNumEpochs = ANN_DEFAULT_MAX_NUMBER_OF_EPOCHS;
	int numEpochs = ANN_DEFAULT_SIMPLE_TRAIN_DEFAULT_NUM_OF_TRAINING_EPOCHS;
	bool usePresetNumberOfEpochs = false;

	bool useInputXMLFile = false;
	string inputXMLFileName = "neuralNet.xml";
	
	bool useOutputXMLFile = false;
	string outputXMLFileName = "neuralNet.xml";
	
	bool useOutputLDRFileWithoutSprites = false;
	string outputLDRFileNameWithoutSprites = "neuralNetWithoutSprites.ldr";

	bool useOutputLDRFileWithSprites = false;
	string outputLDRFileNameWithSprites = "neuralNetWithSprites.ldr";

	bool useOutputSVGFile = false;
	string outputSVGFileName = "neuralNet.svg";
		
	bool useOutputPPMFile = false;
	string outputPPMFileName = "neuralNet.ppm";
			
	bool useOutputPPMFileRaytraced = false;
	string outputPPMFileNameRaytraced = "neuralNetRaytraced.ppm";
	string outputTALFileName = "neuralNet.tal";
	
	bool useOutputAllFile = false;
	string outputAllFileName = "neuralNet";
			
	bool displayInOpenGLAndOutputScreenshot = true;
	bool printOutput = false;

	int rasterImageWidth = 1024;
	int rasterImageHeight = 768; 
		
	bool useTextUI = false;

	int version;

	bool trainIfUseInputDatasetFile = true;
	//bool form = true;

	//basic execution flow outline; if no dataset or xml input file is specified, just form network - do not train network

	if ((exists_argument(argc,argv,"-idata")) || (exists_argument(argc,argv,"-ixml")) || (exists_argument(argc,argv,"-oxml")) || (exists_argument(argc,argv,"-oldr")) || (exists_argument(argc,argv,"-oldr2")) || (exists_argument(argc,argv,"-osvg")) || (exists_argument(argc,argv,"-oppm")) || (exists_argument(argc,argv,"-oppm2")) || (exists_argument(argc,argv,"-oall")) || (exists_argument(argc,argv,"-ui")))
	{
		if(exists_argument(argc,argv,"-idata"))
		{
			inputDatasetFileName=get_char_argument(argc,argv,"-idata");
			//train = true;
			useInputDatasetFile = true;
		}

		if (exists_argument(argc,argv,"-layers"))
		numberOfLayers=get_float_argument(argc,argv,"-layers");

		if (exists_argument(argc,argv,"-ineurons"))
		numberOfInputNeurons=get_float_argument(argc,argv,"-ineurons");

		if (exists_argument(argc,argv,"-oneurons"))
		numberOfOutputNeurons=get_float_argument(argc,argv,"-oneurons");

		if (exists_argument(argc,argv,"-divtype"))
		layerDivergenceType=get_float_argument(argc,argv,"-divtype");

		if (exists_argument(argc,argv,"-divfactor"))
		meanLayerDivergenceFactor=get_float_argument(argc,argv,"-divfactor");

		if (exists_argument(argc,argv,"-con"))
		probabilityNeuronConnectionWithPreviousLayerNeuron=get_float_argument(argc,argv,"-con");

		if (exists_argument(argc,argv,"-conall"))
		probabilityNeuronConnectionWithAllPreviousLayersNeurons=get_float_argument(argc,argv,"-conall");

		if (exists_argument(argc,argv,"-usesubnets"))
		useSubnets = true;

		if (exists_argument(argc,argv,"-numsubnets"))
		maxNumRecursiveSubnets=get_float_argument(argc,argv,"-numsubnets");

		if (exists_argument(argc,argv,"-probsubnet"))
		probabilityOfSubnetCreation=get_float_argument(argc,argv,"-probsubnet");

		if (exists_argument(argc,argv,"-subnetdepl"))
		useSubnetDependentNumberOfLayers = true;

		if (exists_argument(argc,argv,"-epochs"))
		{
			numEpochs=get_float_argument(argc,argv,"-epochs");
			usePresetNumberOfEpochs = true;
		}

		if (exists_argument(argc,argv,"-maxepochs"))
		maxNumEpochs=get_float_argument(argc,argv,"-maxepochs");

		if (exists_argument(argc,argv,"-maxFolds"))
		maxFolds=get_float_argument(argc,argv,"-maxFolds");



		if (exists_argument(argc,argv,"-numsubnets"))
		maxNumRecursiveSubnets=get_float_argument(argc,argv,"-numsubnets");

		if (exists_argument(argc,argv,"-numsubnets"))
		maxNumRecursiveSubnets=get_float_argument(argc,argv,"-numsubnets");


		if(exists_argument(argc,argv,"-ixml"))
		{
			inputXMLFileName=get_char_argument(argc,argv,"-ixml");
			//train = true;
			useInputXMLFile = true;
		}

		if(exists_argument(argc,argv,"-oxml"))
		{
			outputXMLFileName=get_char_argument(argc,argv,"-oxml");
			useOutputXMLFile = true;
		}

		if(exists_argument(argc,argv,"-oldr"))
		{
			outputLDRFileNameWithoutSprites=get_char_argument(argc,argv,"-oldr");
			useOutputLDRFileWithoutSprites = true;
			printOutput = true;
		}

		if(exists_argument(argc,argv,"-oldr2"))
		{
			outputLDRFileNameWithSprites=get_char_argument(argc,argv,"-oldr2");
			useOutputLDRFileWithSprites = true;
			printOutput = true;
		}

		if(exists_argument(argc,argv,"-osvg"))
		{
			outputSVGFileName=get_char_argument(argc,argv,"-osvg");
			useOutputSVGFile = true;
			printOutput = true;
		}

		if(exists_argument(argc,argv,"-oppm"))
		{
			outputPPMFileName=get_char_argument(argc,argv,"-oppm");
			useOutputPPMFile = true;
			printOutput = true;
		}
		
		if(exists_argument(argc,argv,"-oppm2"))
		{
			outputPPMFileNameRaytraced=get_char_argument(argc,argv,"-oppm2");
			useOutputPPMFileRaytraced = true;
			printOutput = true;
		}
		
		if(exists_argument(argc,argv,"-oall"))
		{
			outputAllFileName=get_char_argument(argc,argv,"-oall");
			useOutputAllFile = true;
			printOutput = true;
		}
				
		if (exists_argument(argc,argv,"-notshow"))
		{
			displayInOpenGLAndOutputScreenshot = false;
		}
		
		if (exists_argument(argc,argv,"-width"))
		rasterImageWidth=get_float_argument(argc,argv,"-width");

		if (exists_argument(argc,argv,"-height"))
		rasterImageHeight=get_float_argument(argc,argv,"-height");
				
		if(exists_argument(argc,argv,"-train"))
		{
			int trainInt;
			trainInt=get_float_argument(argc,argv,"-train");
			trainIfUseInputDatasetFile = (bool)trainInt;
		}
	
		char currentFolder[EXE_FOLDER_PATH_MAX_LENGTH];	
		#ifdef LINUX
		getcwd(currentFolder, EXE_FOLDER_PATH_MAX_LENGTH);					
		#else
		::GetCurrentDirectory(EXE_FOLDER_PATH_MAX_LENGTH, currentFolder);
		#endif
		
		if (exists_argument(argc,argv,"-workingfolder"))
		{
			workingFolderCharStar=get_char_argument(argc,argv,"-workingfolder");
		}
		else
		{
			workingFolderCharStar = currentFolder;		
		}
		if (exists_argument(argc,argv,"-exefolder"))
		{
			exeFolderCharStar=get_char_argument(argc,argv,"-exefolder");
		}
		else
		{
			exeFolderCharStar = currentFolder;
		}
		if (exists_argument(argc,argv,"-tempfolder"))
		{
			tempFolderCharStar=get_char_argument(argc,argv,"-tempfolder");
		}
		else
		{
			tempFolderCharStar = currentFolder;
		}

		#ifdef LINUX
		chdir(workingFolderCharStar);						
		#else
		::SetCurrentDirectory(workingFolderCharStar);
		#endif					

		if (exists_argument(argc,argv,"-ui"))
		{
			mainUI();
		}
		if (exists_argument(argc,argv,"-version"))
		{
			cout << "rt.exe version: 1pXy" << endl;
			exit(1);
		}
	}
	else
	{
		printf(errmessage);
		exit(1);
	}

	if(printOutput)
	{
		if(!useOutputXMLFile)
		{	
			if(useOutputAllFile)
			{	
				useOutputXMLFile = true;		
				outputXMLFileName = outputAllFileName + ".xml";
			}
		}		
		if(!useOutputLDRFileWithSprites)
		{		
			if(useOutputAllFile || displayInOpenGLAndOutputScreenshot)		//LDR output is always required when displaying neural network in OpenGL and outputing screenshot
			{
				//LDR output is always required when displaying neural network in OpenGL and outputing screenshot
				useOutputLDRFileWithSprites = true;			
				outputLDRFileNameWithSprites = outputAllFileName + "WithSprites.ldr";
			}
		}
		if(!useOutputLDRFileWithoutSprites)		//LDR output is always required when printing neural network
		{		
			useOutputLDRFileWithoutSprites = true;			
			outputLDRFileNameWithoutSprites = outputAllFileName + "WithoutSprites.ldr";
		}		
		if(!useOutputSVGFile)
		{	
			if(useOutputAllFile)
			{
				useOutputSVGFile = true;	//SVG output is not always required when printing/drawing neural network
				outputSVGFileName = outputAllFileName + ".svg";
			}
		}
		if(!useOutputPPMFile)
		{
			if(useOutputAllFile)
			{
				useOutputPPMFile = true;		
				outputPPMFileName = outputAllFileName + ".ppm";
			}
		}	
		/* disable raytrace output by default	
		if(!useOutputPPMFileRaytraced)
		{
			if(useOutputAllFile)
			{
				useOutputPPMFileRaytraced = true;		
				outputPPMFileNameRaytraced = outputAllFileName + "Raytraced.ppm";
			}
		}
		*/	
	}
	
	
	
	if(useInputXMLFile)
	{//create a network and load input dataset file
		//Neural Network initialisations
		firstInputNeuronInNetwork = new NeuronContainer();

		long numberOfInputNeuronsLoaded;
		long numberOfOutputNeuronsLoaded;

		string xmlFileName = inputXMLFileName;

		firstOutputNeuronInNetwork = readNetXMLFileAndRecordFormationVariables(xmlFileName, firstInputNeuronInNetwork, &numberOfInputNeuronsLoaded, &numberOfOutputNeuronsLoaded);
		//check  number of input and number of output neurons

		numberOfInputNeurons = numberOfInputNeuronsLoaded;
		numberOfOutputNeurons = numberOfOutputNeuronsLoaded;

		formedNetwork = true;

	}
	else
	{//create a neural network

		if(useInputDatasetFile)
		{//create a network and load experience dataset
			bool result = true;

			//parse test harness Data set

			string nameOfExperiencesDataSetFile = inputDatasetFileName;
			cout << "nameOfExperiencesDataSetFile = " << nameOfExperiencesDataSetFile << endl;

			//doSomething();
			ANNTHparseTestDataFile(nameOfExperiencesDataSetFile);
			//cout << "h1" << endl;

			numberOfInputNeurons = numInputNeurons;
			numberOfOutputNeurons = numOutputNeurons;

		}
		else
		{
			//just form a neural network, dont train
		}



		//now create a network;

		bool result = true;

		//Neural Network initialisations
		firstInputNeuronInNetwork = new NeuronContainer();

		if(useSubnets)
		{
			firstOutputNeuronInNetwork = formAdvancedNeuralNetwork(firstInputNeuronInNetwork, numberOfInputNeurons, numberOfOutputNeurons, useSubnetDependentNumberOfLayers, probabilityOfSubnetCreation, maxNumRecursiveSubnets, numberOfLayers, layerDivergenceType, meanLayerDivergenceFactor, probabilityNeuronConnectionWithPreviousLayerNeuron, probabilityNeuronConnectionWithAllPreviousLayersNeurons);

		}
		else
		{
			firstOutputNeuronInNetwork = formNeuralNet(firstInputNeuronInNetwork, numberOfInputNeurons, numberOfOutputNeurons, numberOfLayers, layerDivergenceType, meanLayerDivergenceFactor, probabilityNeuronConnectionWithPreviousLayerNeuron, probabilityNeuronConnectionWithAllPreviousLayersNeurons);
		}


		cout << "subnet Divergence Type = " << layerDivergenceType << endl;
		cout << "mean Subnet Divergence Factor = " << meanLayerDivergenceFactor << endl;
		cout << "probability Of Neuron Connection Exclusivity = " << probabilityNeuronConnectionWithPreviousLayerNeuron <<endl;
		cout << "Probability Of Subnet Creation = " << probabilityOfSubnetCreation <<endl;
		cout << "use Subnet Dependent Number Of Layers = " << useSubnetDependentNumberOfLayers << endl;
		cout << "Max Number Recursive Subnets = " << maxNumRecursiveSubnets <<endl;
		cout << "Number Of top level Layers = " << numberOfLayers << endl;

	}


	if(useInputDatasetFile)
	{//train the network

		if(trainIfUseInputDatasetFile)
		{
			if(!usePresetNumberOfEpochs)
			{

				trainNeuralNetwork(firstInputNeuronInNetwork, firstOutputNeuronInNetwork, numberOfInputNeurons, numberOfOutputNeurons, maxFolds, firstExperienceInDataSet, numExperiences, maxNumEpochs);
			}
			else
			{
				trainNeuralNetworkSimple(firstInputNeuronInNetwork, firstOutputNeuronInNetwork, numberOfInputNeurons, numberOfOutputNeurons, numEpochs, firstExperienceInDataSet, numExperiences);

			}
		}

	}


	if(!parseANNRulesXMLFile())
	{
		result = false;
	}
	fillInANNSpriteExternVariables();
	#ifdef LINUX
	chdir(tempFolderCharStar);						
	#else
	::SetCurrentDirectory(tempFolderCharStar);
	#endif
	//cout << "tempFolderCharStar = " << tempFolderCharStar << endl;


	if(printOutput)
	{
		cout << "vector graphics file name without sprites = " << outputLDRFileNameWithoutSprites << endl;

		if(useOutputLDRFileWithSprites)
		{
			cout << "vector graphics file name with sprites = " << outputLDRFileNameWithSprites << endl;
		}

		//ANNcreateNeuralNetworkSceneFilesWithAndWithoutSprites(outputLDRFileNameWithoutSprites, outputLDRFileNameWithSprites, firstInputNeuronInNetwork, useOutputLDRFileWithSprites);

		outputNeuralNetworkToVectorGraphicsAndRaytrace(firstInputNeuronInNetwork, useOutputLDRFileWithSprites, useOutputPPMFileRaytraced, displayInOpenGLAndOutputScreenshot, useOutputLDRFileWithoutSprites, useOutputPPMFile, useOutputSVGFile, outputLDRFileNameWithoutSprites, outputLDRFileNameWithSprites, outputSVGFileName, outputPPMFileName, outputPPMFileNameRaytraced, outputTALFileName, rasterImageWidth, rasterImageHeight);

	}

	if(useOutputXMLFile)
	{
		cout << "xml file name = " << outputXMLFileName << endl;

		if(!writeNetXMLFile(outputXMLFileName, firstInputNeuronInNetwork))
		{
			result = false;
		}

	}

}



bool loadANetworkFromXML()
{
	bool result = true;

	//Neural Network initialisations
	firstInputNeuronInNetwork = new NeuronContainer();

	long numberOfInputNeuronsLoaded;
	long numberOfOutputNeuronsLoaded;

	string xmlFileName = NET_XML_FILE_NAME;

	firstOutputNeuronInNetwork = readNetXMLFileAndRecordFormationVariables(xmlFileName, firstInputNeuronInNetwork, &numberOfInputNeuronsLoaded, &numberOfOutputNeuronsLoaded);
	//check  number of input and number of output neurons

	numberOfInputNeurons = numberOfInputNeuronsLoaded;
	numberOfOutputNeurons = numberOfOutputNeuronsLoaded;

	formedNetwork = true;

	return result;
}

bool createANetwork()
{
	bool result = true;

	//Neural Network initialisations
	firstInputNeuronInNetwork = new NeuronContainer();

	#ifdef DEBUG
	cout << "************************************************************************************" << endl;
	cout << "******** DEBUG preprocessor definition enabled... tracing execution path...  *******" << endl;
	cout << "************************************************************************************" << endl;
	#endif

	int layerDivergenceType = 0;
	double meanLayerDivergenceFactor = 0;
	double probabilityNeuronConnectionWithPreviousLayerNeuron = 0;
	double probabilityOfSubnetCreation = 0;
	bool useSubnetDependentNumberOfLayers = 0;
	long maxNumRecursiveSubnets = 0;
	double numberOfLayers = 0;
	double probabilityNeuronConnectionWithAllPreviousLayersNeurons = 0;


	double answerAsDouble;
	long answerAsInt;
	char answerAsChar;
	string answerAsString;


	if(!loadedExperienceDataset)
	{
		cout <<	"\nEnter the number of input neurons (2, 3, 4, 5 [Default], ... etc):\n\n>> ";
		cin >> answerAsString;
		answerAsInt = long(atof(answerAsString.c_str()));
		numberOfInputNeurons = answerAsInt;

		cout <<	"\nEnter the number of output neurons (2, 3 [Default], 4, 5, ... etc):\n\n>> ";
		cin >> answerAsString;
		answerAsInt = long(atof(answerAsString.c_str()));
		numberOfOutputNeurons = answerAsInt;

		numberOfInputAndOutputNeuronsSelected = true;
	}


	cout <<	"\nEnter the number of top level Layers (2, 3 [Default], ... etc):\n\n>> ";
	cin >> answerAsString;
	answerAsInt = long(atof(answerAsString.c_str()));
	numberOfLayers = answerAsInt;
	if(numberOfLayers < 2)
	{
		numberOfLayers = 2;
	}


	cout <<	"\nDo you wish to specify advanced network structure properties? (y/n)\n\n>> ";
	cin >> answerAsString;
	answerAsChar = answerAsString[0];

	if(answerAsChar == 'y')
	{
		//non subnet/ANN specific parameters....
		cout <<	"\nEnter layer divergence type [Default = " << LAYER_DIVERGENCE_TYPE_LINEAR_CONVERGING << "]:\n\t1 (LINEAR_CONVERGING), \n\t2 (LAYER_DIVERGENCE_TYPE_LINEAR_DIVERGING_THEN_CONVERGING),\n\t3 (LAYER_DIVERGENCE_TYPE_NONLINEAR_DIVERGING_THEN_CONVERGING),\n\t4 (LAYER_DIVERGENCE_TYPE_LINEAR_DIVERGING),\n\t5 (LAYER_DIVERGENCE_TYPE_LINEAR_DIVERGING_SQUARE2D),\n\t6 (LAYER_DIVERGENCE_TYPE_LINEAR_DIVERGING_SQUARE2D_RADIALBIAS). \n\n>> ";
		cin >> answerAsString;
		answerAsInt = long(atof(answerAsString.c_str()));
		layerDivergenceType = answerAsInt;

		//TEST!
		/*
		if((layerDivergenceType == LAYER_DIVERGENCE_TYPE_LINEAR_DIVERGING_SQUARE2D) || (layerDivergenceType == LAYER_DIVERGENCE_TYPE_LINEAR_DIVERGING_SQUARE2D_RADIALBIAS))
		{
			numberOfInputNeurons = 16;
			numberOfOutputNeurons = 16;
		}
		*/

		cout <<	"\nEnter the mean layer divergence factor [Default = " << DEFAULT_MEAN_LAYER_DIVERGENCE_FACTOR << "]:\n\n>> ";
		cin >> answerAsString;
		answerAsDouble = atof(answerAsString.c_str());
		meanLayerDivergenceFactor = answerAsDouble;

		//cout << "DEBUG:" << meanLayerDivergenceFactor  << endl;

		cout <<	"\nEnter the probability of neuron having a connection with a previous layer neuron [Default = " << DEFAULT_PROBABILITY_NEURON_CONNECTION_WITH_PREVIOUS_LAYER_NEURON_ANNTH << "]:\n\n>> ";
		cin >> answerAsString;
		answerAsDouble = atof(answerAsString.c_str());
		probabilityNeuronConnectionWithPreviousLayerNeuron = answerAsDouble;


		cout <<	"\nEnter the probability of a neuron having a direct link with all previous layers neurons [Default = " << DEFAULT_PROBABILITY_NEURON_CONNECTION_WITH_ALL_PREVIOUS_LAYERS_NEURONS_ANNTH << "]:\n\n>> ";
		cin >> answerAsString;
		answerAsDouble = atof(answerAsString.c_str());
		probabilityNeuronConnectionWithAllPreviousLayersNeurons = answerAsDouble;


		//subnet/ANN specific parameters....
		cout <<	"\nDo you wish to create Subnets? (y/n) [Default = n] \n\n>> ";
		cin >> answerAsString;
		answerAsChar = answerAsString[0];

		if(answerAsChar == 'y')
		{
			cout <<	"\nEnter the maximum number of subnet layers you wish to create (1,2,3,4,5,... etc) [Default = " << DEFAULT_ANN_MAX_NUM_RECURSIVE_SUBNETS_ANNTH << "]:\n\n>> ";
			cin >> answerAsString;
			answerAsInt = long(atof(answerAsString.c_str()));
			maxNumRecursiveSubnets = answerAsInt;

			cout <<	"\nEnter the probability of subnet creation [Default = " << DEFAULT_PROB_OF_SUBNET_CREATION_ANNTH << "]:\n\n>> ";
			cin >> answerAsString;
			answerAsDouble = atof(answerAsString.c_str());
			probabilityOfSubnetCreation = answerAsDouble;

			cout <<	"\nDo you wish to use subnet dependant number of layers? (y/n). [Default = n]:\n\n>> ";
			cin >> answerAsString;
			answerAsChar = answerAsString[0];
			if(answerAsChar == 'y')
			{
				useSubnetDependentNumberOfLayers = true;
			}
			else
			{
				useSubnetDependentNumberOfLayers = false;
			}

			firstOutputNeuronInNetwork = formAdvancedNeuralNetwork(firstInputNeuronInNetwork, numberOfInputNeurons, numberOfOutputNeurons, useSubnetDependentNumberOfLayers, probabilityOfSubnetCreation, maxNumRecursiveSubnets, numberOfLayers, layerDivergenceType, meanLayerDivergenceFactor, probabilityNeuronConnectionWithPreviousLayerNeuron, probabilityNeuronConnectionWithAllPreviousLayersNeurons);

		}
		else
		{
			firstOutputNeuronInNetwork = formNeuralNet(firstInputNeuronInNetwork, numberOfInputNeurons, numberOfOutputNeurons, numberOfLayers, layerDivergenceType, meanLayerDivergenceFactor, probabilityNeuronConnectionWithPreviousLayerNeuron, probabilityNeuronConnectionWithAllPreviousLayersNeurons);
		}
	}
	else
	{
		firstOutputNeuronInNetwork = formNeuralNetWithOptimisedProperties(firstInputNeuronInNetwork, numberOfInputNeurons, numberOfOutputNeurons, numberOfLayers);
	}




	cout << "subnet Divergence Type = " << layerDivergenceType << endl;
	cout << "mean Subnet Divergence Factor = " << meanLayerDivergenceFactor << endl;
	cout << "probability Of Neuron Connection Exclusivity = " << probabilityNeuronConnectionWithPreviousLayerNeuron <<endl;
	cout << "Probability Of Subnet Creation = " << probabilityOfSubnetCreation <<endl;
	cout << "use Subnet Dependent Number Of Layers = " << useSubnetDependentNumberOfLayers << endl;
	cout << "Max Number Recursive Subnets = " << maxNumRecursiveSubnets <<endl;
	cout << "Number Of top level Layers = " << numberOfLayers << endl;


	formedNetwork = true;

	return result;
}






bool loadAnExperienceDataFile()
{
	bool result = true;

	//parse test harness Data set

	string nameOfExperiencesDataSetFile;
	char answerAsChar;
	string answerAsString;

	cout <<	"\nDo you wish to use the default experience dataset name? (y/n) [Default = y] \n\n>> ";
	cin >> answerAsString;
	answerAsChar = answerAsString[0];

	if(answerAsChar == 'y')
	{
		nameOfExperiencesDataSetFile = EXPERIENCE_DATASET_FILE_NAME_ANNTH;
	}
	else
	{
		cout <<	"\nEnter the name of the experience dataset:\n\n>> ";
		cin >> answerAsString;
		nameOfExperiencesDataSetFile = answerAsString;
	}

	cout << "nameOfExperiencesDataSetFile = " << nameOfExperiencesDataSetFile << endl;

	//doSomething();
	ANNTHparseTestDataFile(nameOfExperiencesDataSetFile);
	//cout << "h1" << endl;

	if(numberOfInputAndOutputNeuronsSelected)
	{
		if(numInputNeurons != numberOfInputNeurons)
		{
			cout << "Warning: number of input neurons previously selected do not match with data file. Please recreate neural network with correct number of output neurons" << endl;
		}
		if(numOutputNeurons != numberOfOutputNeurons)
		{
			cout << "Warning: number of output neurons previously selected do not match with data file. Please recreate neural network with correct number of output neurons" << endl;
		}

	}
	numberOfInputNeurons = numInputNeurons;
	numberOfOutputNeurons = numOutputNeurons;

	numberOfInputAndOutputNeuronsSelected = true;
	loadedExperienceDataset = true;

	return result;
}


bool trainANetwork(bool advancedTraining)
{
	bool result = true;


	if(!loadedExperienceDataset)
	{
		if(!loadAnExperienceDataFile())
		{
			result = false;
		}
	}

	if(formedNetwork)
	{
		if(advancedTraining)
		{
			int maxFolds = MAX_NUM_FOLDS_ANNTH;		//maximum number of folds are same regardless of TEST_LEVEL_X_ANNT
			int maxNumEpochs = ANN_DEFAULT_MAX_NUMBER_OF_EPOCHS;
			trainNeuralNetwork(firstInputNeuronInNetwork, firstOutputNeuronInNetwork, numberOfInputNeurons, numberOfOutputNeurons, maxFolds, firstExperienceInDataSet, numExperiences, maxNumEpochs);
		}
		else
		{
			int numEpochs = ANN_DEFAULT_SIMPLE_TRAIN_DEFAULT_NUM_OF_TRAINING_EPOCHS;
			trainNeuralNetworkSimple(firstInputNeuronInNetwork, firstOutputNeuronInNetwork, numberOfInputNeurons, numberOfOutputNeurons, numEpochs, firstExperienceInDataSet, numExperiences);

		}
	}
	else
	{
		cout << "Warning: cannot train network - no network has been formed" << endl;
	}
	return result;

}

bool outputANetworkToXML()
{
	bool result = true;

	cout << "xml file name = " << NET_XML_FILE_NAME << endl;

	if(formedNetwork)
	{
		if(!writeNetXMLFile(NET_XML_FILE_NAME, firstInputNeuronInNetwork))
		{
			result = false;
		}
	}
	else
	{
		cout << "Warning: cannot output network to xml - no network has been formed" << endl;
	}
	return result;
}



bool outputANetworkAsVectorGraphics()
{
	bool result = true;

	if(formedNetwork)
	{
		cout << "vector graphics file name without sprites = " << NEURAL_NETWORK_VISUALISATION_DAT_FILE_NAME_WITHOUT_SPRITES << endl;
		cout << "vector graphics file name with sprites = " << NEURAL_NETWORK_VISUALISATION_DAT_FILE_NAME_WITH_SPRITES << endl;
		cout << "vector graphics file svg = " << NEURAL_NETWORK_VISUALISATION_SVG_FILE_NAME << endl;
		
		ofstream writeFileObject(NEURAL_NETWORK_VISUALISATION_SVG_FILE_NAME);
		writeSVGHeader(&writeFileObject);
					
		ANNcreateNeuralNetworkSceneFilesWithAndWithoutSprites(NEURAL_NETWORK_VISUALISATION_DAT_FILE_NAME_WITHOUT_SPRITES, NEURAL_NETWORK_VISUALISATION_DAT_FILE_NAME_WITH_SPRITES, firstInputNeuronInNetwork, true, true, &writeFileObject);
	
		writeSVGFooter(&writeFileObject);
		writeFileObject.close();	
	}
	else
	{
		cout << "Warning: cannot output network as vector graphics - no network has been formed" << endl;
	}

	return result;
}


bool mainUI()
{
	bool result = true;

	bool UIstatus = true;
	numberOfInputAndOutputNeuronsSelected = false;
	loadedExperienceDataset = false;
	formedNetwork = false;

	while(UIstatus == true)
	{
		int answerAsInt;
		char answerAsChar;
		string answerAsString;

		cout << "\n ---\n";
		cout << "Welcome To ANN Command Console (Version 1e1a or higher)\n";
		cout << " ---\n";
		cout << "Do you wish to \n";
		cout << " ---\n";
		cout << "1. Load a network from xml \n";
		cout << "2. Create a new network \n";
		cout << " --- \n";
		cout << "3. Load an experience dataset \n";
		cout << " --- \n";
		cout << "4. Train the network \n";
		cout << "8. Train the network using preset number of epochs \n";
		cout << " --- \n";
		cout << "5. Output the network as xml \n";
		cout << "6. Output the network as vector graphics (.ldr) \n";
		cout << " --- \n";
#ifdef USE_LRRC
		cout << "7. Generate Experience Data Using LRRC \n";
		cout << " ---\n";
#endif
		cout << "0. Exit\n";
		cout << " ---\n";
		cout <<	"Enter Answer (0,1,2,3... etc):\n\n>> ";

		cin >> answerAsString;
		answerAsInt = long(atof(answerAsString.c_str()));
		if(answerAsInt == 1)
		{
			loadANetworkFromXML();
		}
		else if(answerAsInt == 2)
		{
			createANetwork();
		}
		else if(answerAsInt == 3)
		{
			loadAnExperienceDataFile();
		}
		else if(answerAsInt == 4)
		{
			trainANetwork(true);
		}
		else if(answerAsInt == 8)
		{
			trainANetwork(false);
		}
		else if(answerAsInt == 5)
		{
			outputANetworkToXML();
		}
		else if(answerAsInt == 6)
		{
			outputANetworkAsVectorGraphics();
		}
		#ifdef USE_LRRC
		else if(answerAsInt == 7)
		{
			executeLRRCfunctionsWithAI();
		}
		#endif
		else if(answerAsInt == 0)
		{
			UIstatus = false;
		}
		else
		{
			cout << "\nInvalid answerAsInt, please try another command\n" << endl;
		}
	}

	return result;
}





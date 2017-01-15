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
 * File Name: ANNmain.c
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2015 Baxter AI (baxterai.com)
 * Project: Artificial Neural Network (ANN)
 * Project Version: 3f4a 11-July-2015
 * Comments: TH = Test Harness
 *
 *******************************************************************************/


#include "ANNmain.h"

#include "ANNformation.h"
#include "ANNtraining.h"
#include "ANNparser.h"
#include "ANNxmlConversion.h"
#include "ANNsprite.h"
#include "ANNdisplay.h"

#include "ANNdata.h"

#include "LDsvg.h"
#include "LDsprite.h"
#include "XMLrulesClass.h"

#ifdef USE_LRRC
#include "LRRCgame.h"
#endif

#ifndef LINUX
	#include <windows.h>
#endif


ANNneuron* firstInputNeuronInNetwork;
ANNneuron* firstOutputNeuronInNetwork;

long numberOfInputNeurons;
long numberOfOutputNeurons;

bool numberOfInputAndOutputNeuronsSelected;
bool loadedExperienceDataset;
bool formedNetwork;



static char errmessage[] = "Usage:  OpenANN.exe [options]"
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
"\n\t-exefolder [string]     : exe directory name for executables OpenANN.exe and (def: same as exe)"
"\n\t-tempfolder [string]    : temp directory name for temporary and output files (def: same as exe)"
"\n"
"\n\t-ui               : loads text user interface upon execution (def: no)"
"\n\t-version          : print version"
"\n"
"\n\tThe program performs neural network operations - can load an experience dataset,"
"\n\tload a neural network definition xml file, form, train, test, and output neural networks"
"\n\tas .xml and .ldr "
"\n";

int main(int argc,char* *argv)
{
	srand( (unsigned)time(NULL) );	//seeds randomness

	bool result = true;

	bool useInputDatasetFile = false;
	string inputDatasetFileName = "dataset.data";

	long numberOfInputNeurons = 5;	//need to be generated based upon dataset or xml input file
	long numberOfOutputNeurons = 3;	//need to be generated based upon dataset or xml input file
	double numberOfLayers = 3;

	int layerDivergenceType = LAYER_DIVERGENCE_TYPE_LINEAR_CONVERGING;
	double meanLayerDivergenceFactor = DEFAULT_MEAN_LAYER_DIVERGENCE_FACTOR;
	double probabilityANNneuronConnectionWithPreviousLayerNeuron = DEFAULT_PROBABILITY_NEURON_CONNECTION_WITH_PREVIOUS_LAYER_NEURON_ANNTH;
	double probabilityANNneuronConnectionWithAllPreviousLayersNeurons = DEFAULT_PROBABILITY_NEURON_CONNECTION_WITH_ALL_PREVIOUS_LAYERS_NEURONS_ANNTH;
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

	if((argumentExists(argc,argv,"-idata")) || (argumentExists(argc,argv,"-ixml")) || (argumentExists(argc,argv,"-oxml")) || (argumentExists(argc,argv,"-oldr")) || (argumentExists(argc,argv,"-oldr2")) || (argumentExists(argc,argv,"-osvg")) || (argumentExists(argc,argv,"-oppm")) || (argumentExists(argc,argv,"-oppm2")) || (argumentExists(argc,argv,"-oall")) || (argumentExists(argc,argv,"-ui")))
	{
		if(argumentExists(argc,argv,"-idata"))
		{
			inputDatasetFileName = getStringArgument(argc,argv,"-idata");
			//train = true;
			useInputDatasetFile = true;
		}

		if(argumentExists(argc,argv,"-layers"))
		numberOfLayers = getFloatArgument(argc,argv,"-layers");

		if(argumentExists(argc,argv,"-ineurons"))
		numberOfInputNeurons = getFloatArgument(argc,argv,"-ineurons");

		if(argumentExists(argc,argv,"-oneurons"))
		numberOfOutputNeurons = getFloatArgument(argc,argv,"-oneurons");

		if(argumentExists(argc,argv,"-divtype"))
		layerDivergenceType = getFloatArgument(argc,argv,"-divtype");

		if(argumentExists(argc,argv,"-divfactor"))
		meanLayerDivergenceFactor = getFloatArgument(argc,argv,"-divfactor");

		if(argumentExists(argc,argv,"-con"))
		probabilityANNneuronConnectionWithPreviousLayerNeuron = getFloatArgument(argc,argv,"-con");

		if(argumentExists(argc,argv,"-conall"))
		probabilityANNneuronConnectionWithAllPreviousLayersNeurons = getFloatArgument(argc,argv,"-conall");

		if(argumentExists(argc,argv,"-usesubnets"))
		useSubnets = true;

		if(argumentExists(argc,argv,"-numsubnets"))
		maxNumRecursiveSubnets = getFloatArgument(argc,argv,"-numsubnets");

		if(argumentExists(argc,argv,"-probsubnet"))
		probabilityOfSubnetCreation = getFloatArgument(argc,argv,"-probsubnet");

		if(argumentExists(argc,argv,"-subnetdepl"))
		useSubnetDependentNumberOfLayers = true;

		if(argumentExists(argc,argv,"-epochs"))
		{
			numEpochs=getFloatArgument(argc,argv,"-epochs");
			usePresetNumberOfEpochs = true;
		}

		if(argumentExists(argc,argv,"-maxepochs"))
		maxNumEpochs = getFloatArgument(argc,argv,"-maxepochs");

		if(argumentExists(argc,argv,"-maxFolds"))
		maxFolds = getFloatArgument(argc,argv,"-maxFolds");



		if(argumentExists(argc,argv,"-numsubnets"))
		maxNumRecursiveSubnets = getFloatArgument(argc,argv,"-numsubnets");

		if(argumentExists(argc,argv,"-numsubnets"))
		maxNumRecursiveSubnets = getFloatArgument(argc,argv,"-numsubnets");


		if(argumentExists(argc,argv,"-ixml"))
		{
			inputXMLFileName = getStringArgument(argc,argv,"-ixml");
			//train = true;
			useInputXMLFile = true;
		}

		if(argumentExists(argc,argv,"-oxml"))
		{
			outputXMLFileName = getStringArgument(argc,argv,"-oxml");
			useOutputXMLFile = true;
		}

		if(argumentExists(argc,argv,"-oldr"))
		{
			outputLDRFileNameWithoutSprites = getStringArgument(argc,argv,"-oldr");
			useOutputLDRFileWithoutSprites = true;
			printOutput = true;
		}

		if(argumentExists(argc,argv,"-oldr2"))
		{
			outputLDRFileNameWithSprites = getStringArgument(argc,argv,"-oldr2");
			useOutputLDRFileWithSprites = true;
			printOutput = true;
		}

		if(argumentExists(argc,argv,"-osvg"))
		{
			outputSVGFileName = getStringArgument(argc,argv,"-osvg");
			useOutputSVGFile = true;
			printOutput = true;
		}

		if(argumentExists(argc,argv,"-oppm"))
		{
			outputPPMFileName = getStringArgument(argc,argv,"-oppm");
			useOutputPPMFile = true;
			printOutput = true;
		}

		if(argumentExists(argc,argv,"-oppm2"))
		{
			outputPPMFileNameRaytraced = getStringArgument(argc,argv,"-oppm2");
			useOutputPPMFileRaytraced = true;
			printOutput = true;
		}

		if(argumentExists(argc,argv,"-oall"))
		{
			outputAllFileName = getStringArgument(argc,argv,"-oall");
			useOutputAllFile = true;
			printOutput = true;
		}

		if(argumentExists(argc,argv,"-notshow"))
		{
			displayInOpenGLAndOutputScreenshot = false;
		}

		if (argumentExists(argc,argv,"-width"))
		rasterImageWidth = getFloatArgument(argc,argv,"-width");

		if (argumentExists(argc,argv,"-height"))
		rasterImageHeight = getFloatArgument(argc,argv,"-height");

		if(argumentExists(argc,argv,"-train"))
		{
			int trainInt;
			trainInt = getFloatArgument(argc,argv,"-train");
			trainIfUseInputDatasetFile = (bool)trainInt;
		}

		string currentFolder = getCurrentDirectory();

		if (argumentExists(argc,argv,"-workingfolder"))
		{
			workingFolder = getStringArgument(argc,argv,"-workingfolder");
		}
		else
		{
			workingFolder = currentFolder;
		}
		if(argumentExists(argc,argv,"-exefolder"))
		{
			exeFolder = getStringArgument(argc,argv,"-exefolder");
		}
		else
		{
			exeFolder = currentFolder;
		}
		if(argumentExists(argc,argv,"-tempfolder"))
		{
			tempFolder = getStringArgument(argc,argv,"-tempfolder");
		}
		else
		{
			tempFolder = currentFolder;
		}

		setCurrentDirectory(workingFolder);

		if (argumentExists(argc,argv,"-ui"))
		{
			mainUI();
		}
		if (argumentExists(argc,argv,"-version"))
		{
			cout << "Project Version: 3c4b 01-December-2012" << endl;
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
		firstInputNeuronInNetwork = new ANNneuron();

		long numberOfInputNeuronsLoaded;
		long numberOfOutputNeuronsLoaded;

		string xmlFileName = inputXMLFileName;

		firstOutputNeuronInNetwork = readNetXMLfileAndRecordFormationVariables(xmlFileName, firstInputNeuronInNetwork, &numberOfInputNeuronsLoaded, &numberOfOutputNeuronsLoaded);
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

			ANNTHparseTestDataFile(nameOfExperiencesDataSetFile);

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
		firstInputNeuronInNetwork = new ANNneuron();

		if(useSubnets)
		{
			firstOutputNeuronInNetwork = formAdvancedNeuralNetwork(firstInputNeuronInNetwork, numberOfInputNeurons, numberOfOutputNeurons, useSubnetDependentNumberOfLayers, probabilityOfSubnetCreation, maxNumRecursiveSubnets, numberOfLayers, layerDivergenceType, meanLayerDivergenceFactor, probabilityANNneuronConnectionWithPreviousLayerNeuron, probabilityANNneuronConnectionWithAllPreviousLayersNeurons);

		}
		else
		{
			firstOutputNeuronInNetwork = formNeuralNet(firstInputNeuronInNetwork, numberOfInputNeurons, numberOfOutputNeurons, numberOfLayers, layerDivergenceType, meanLayerDivergenceFactor, probabilityANNneuronConnectionWithPreviousLayerNeuron, probabilityANNneuronConnectionWithAllPreviousLayersNeurons);
		}

		cout << "subnet Divergence Type = " << layerDivergenceType << endl;
		cout << "mean Subnet Divergence Factor = " << meanLayerDivergenceFactor << endl;
		cout << "probability Of ANNneuron Connection Exclusivity = " << probabilityANNneuronConnectionWithPreviousLayerNeuron <<endl;
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


	if(!parseANNrulesXMLfile())
	{
		result = false;
	}
	fillInANNSpriteExternVariables();
	setCurrentDirectory(tempFolder);

	if(printOutput)
	{
		cout << "vector graphics file name without sprites = " << outputLDRFileNameWithoutSprites << endl;

		if(useOutputLDRFileWithSprites)
		{
			cout << "vector graphics file name with sprites = " << outputLDRFileNameWithSprites << endl;
		}

		//cout << "q1" << endl;
		outputNeuralNetworkToVectorGraphicsAndRaytrace(firstInputNeuronInNetwork, useOutputLDRFileWithSprites, useOutputPPMFileRaytraced, displayInOpenGLAndOutputScreenshot, useOutputLDRFileWithoutSprites, useOutputPPMFile, useOutputSVGFile, outputLDRFileNameWithoutSprites, outputLDRFileNameWithSprites, outputSVGFileName, outputPPMFileName, outputPPMFileNameRaytraced, outputTALFileName, rasterImageWidth, rasterImageHeight);
		//cout << "q2" << endl;
	}

	if(useOutputXMLFile)
	{
		cout << "XML file name = " << outputXMLFileName << endl;

		if(!writeNetXMLfile(outputXMLFileName, firstInputNeuronInNetwork))
		{
			result = false;
		}

	}

}



bool loadNetworkFromXML()
{
	bool result = true;

	//Neural Network initialisations
	firstInputNeuronInNetwork = new ANNneuron();

	long numberOfInputNeuronsLoaded;
	long numberOfOutputNeuronsLoaded;

	string xmlFileName = NET_XML_FILE_NAME;

	firstOutputNeuronInNetwork = readNetXMLfileAndRecordFormationVariables(xmlFileName, firstInputNeuronInNetwork, &numberOfInputNeuronsLoaded, &numberOfOutputNeuronsLoaded);
	//check  number of input and number of output neurons

	numberOfInputNeurons = numberOfInputNeuronsLoaded;
	numberOfOutputNeurons = numberOfOutputNeuronsLoaded;

	formedNetwork = true;

	return result;
}

bool createNetwork()
{
	bool result = true;

	//Neural Network initialisations
	firstInputNeuronInNetwork = new ANNneuron();

	#ifdef ANN_DEBUG
	cout << "************************************************************************************" << endl;
	cout << "******** DEBUG preprocessor definition enabled... tracing execution path... * ******" << endl;
	cout << "************************************************************************************" << endl;
	#endif

	int layerDivergenceType = 0;
	double meanLayerDivergenceFactor = 0;
	double probabilityANNneuronConnectionWithPreviousLayerNeuron = 0;
	double probabilityOfSubnetCreation = 0;
	bool useSubnetDependentNumberOfLayers = 0;
	long maxNumRecursiveSubnets = 0;
	double numberOfLayers = 0;
	double probabilityANNneuronConnectionWithAllPreviousLayersNeurons = 0;


	double answerAsDouble;
	long answerAsInt;
	char answerAsChar;
	string answerAsString;


	if(!loadedExperienceDataset)
	{
		cout <<	"\nEnter the number of input neurons (2, 3, 4, 5 [Default], ... etc):\n\n>> ";
		cin >> answerAsString;
		answerAsInt = long(convertStringToDouble(answerAsString));
		numberOfInputNeurons = answerAsInt;

		cout <<	"\nEnter the number of output neurons (2, 3 [Default], 4, 5, ... etc):\n\n>> ";
		cin >> answerAsString;
		answerAsInt = long(convertStringToDouble(answerAsString));
		numberOfOutputNeurons = answerAsInt;

		numberOfInputAndOutputNeuronsSelected = true;
	}


	cout <<	"\nEnter the number of top level Layers (2, 3 [Default], ... etc):\n\n>> ";
	cin >> answerAsString;
	answerAsInt = long(convertStringToDouble(answerAsString));
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
		answerAsInt = long(convertStringToDouble(answerAsString));
		layerDivergenceType = answerAsInt;

		cout <<	"\nEnter the mean layer divergence factor [Default = " << DEFAULT_MEAN_LAYER_DIVERGENCE_FACTOR << "]:\n\n>> ";
		cin >> answerAsString;
		answerAsDouble = convertStringToDouble(answerAsString);
		meanLayerDivergenceFactor = answerAsDouble;

		#ifdef ANN_DEBUG
		//cout << "meanLayerDivergenceFactor:" << meanLayerDivergenceFactor << endl;
		#endif

		cout <<	"\nEnter the probability of neuron having a connection with a previous layer neuron [Default = " << DEFAULT_PROBABILITY_NEURON_CONNECTION_WITH_PREVIOUS_LAYER_NEURON_ANNTH << "]:\n\n>> ";
		cin >> answerAsString;
		answerAsDouble = convertStringToDouble(answerAsString);
		probabilityANNneuronConnectionWithPreviousLayerNeuron = answerAsDouble;


		cout <<	"\nEnter the probability of a neuron having a direct link with all previous layers neurons [Default = " << DEFAULT_PROBABILITY_NEURON_CONNECTION_WITH_ALL_PREVIOUS_LAYERS_NEURONS_ANNTH << "]:\n\n>> ";
		cin >> answerAsString;
		answerAsDouble = convertStringToDouble(answerAsString);
		probabilityANNneuronConnectionWithAllPreviousLayersNeurons = answerAsDouble;


		//subnet/ANN specific parameters....
		cout <<	"\nDo you wish to create Subnets? (y/n) [Default = n] \n\n>> ";
		cin >> answerAsString;
		answerAsChar = answerAsString[0];

		if(answerAsChar == 'y')
		{
			cout <<	"\nEnter the maximum number of subnet layers you wish to create (1,2,3,4,5,... etc) [Default = " << DEFAULT_ANN_MAX_NUM_RECURSIVE_SUBNETS_ANNTH << "]:\n\n>> ";
			cin >> answerAsString;
			answerAsInt = long(convertStringToDouble(answerAsString));
			maxNumRecursiveSubnets = answerAsInt;

			cout <<	"\nEnter the probability of subnet creation [Default = " << DEFAULT_PROB_OF_SUBNET_CREATION_ANNTH << "]:\n\n>> ";
			cin >> answerAsString;
			answerAsDouble = convertStringToDouble(answerAsString);
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

			firstOutputNeuronInNetwork = formAdvancedNeuralNetwork(firstInputNeuronInNetwork, numberOfInputNeurons, numberOfOutputNeurons, useSubnetDependentNumberOfLayers, probabilityOfSubnetCreation, maxNumRecursiveSubnets, numberOfLayers, layerDivergenceType, meanLayerDivergenceFactor, probabilityANNneuronConnectionWithPreviousLayerNeuron, probabilityANNneuronConnectionWithAllPreviousLayersNeurons);

		}
		else
		{
			firstOutputNeuronInNetwork = formNeuralNet(firstInputNeuronInNetwork, numberOfInputNeurons, numberOfOutputNeurons, numberOfLayers, layerDivergenceType, meanLayerDivergenceFactor, probabilityANNneuronConnectionWithPreviousLayerNeuron, probabilityANNneuronConnectionWithAllPreviousLayersNeurons);
		}
	}
	else
	{
		firstOutputNeuronInNetwork = formNeuralNetWithOptimisedProperties(firstInputNeuronInNetwork, numberOfInputNeurons, numberOfOutputNeurons, numberOfLayers);
	}

	cout << "subnet Divergence Type = " << layerDivergenceType << endl;
	cout << "mean Subnet Divergence Factor = " << meanLayerDivergenceFactor << endl;
	cout << "probability Of ANNneuron Connection Exclusivity = " << probabilityANNneuronConnectionWithPreviousLayerNeuron <<endl;
	cout << "Probability Of Subnet Creation = " << probabilityOfSubnetCreation <<endl;
	cout << "use Subnet Dependent Number Of Layers = " << useSubnetDependentNumberOfLayers << endl;
	cout << "Max Number Recursive Subnets = " << maxNumRecursiveSubnets <<endl;
	cout << "Number Of top level Layers = " << numberOfLayers << endl;


	formedNetwork = true;

	return result;
}






bool loadExperienceDataFile()
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

	ANNTHparseTestDataFile(nameOfExperiencesDataSetFile);

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


bool trainNetwork(bool advancedTraining)
{
	bool result = true;


	if(!loadedExperienceDataset)
	{
		if(!loadExperienceDataFile())
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

bool outputNetworkToXML()
{
	setCurrentDirectory(tempFolder);
	bool result = true;

	cout << "XML file name = " << NET_XML_FILE_NAME << endl;

	if(formedNetwork)
	{
		if(!writeNetXMLfile(NET_XML_FILE_NAME, firstInputNeuronInNetwork))
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



bool outputNetworkAsVectorGraphics()
{
	setCurrentDirectory(tempFolder);
	bool result = true;

	if(formedNetwork)
	{
		cout << "vector graphics file name without sprites = " << NEURAL_NETWORK_VISUALISATION_DAT_FILE_NAME_WITHOUT_SPRITES << endl;
		cout << "vector graphics file name with sprites = " << NEURAL_NETWORK_VISUALISATION_DAT_FILE_NAME_WITH_SPRITES << endl;
		cout << "vector graphics file svg = " << NEURAL_NETWORK_VISUALISATION_SVG_FILE_NAME << endl;
		cout << "vector graphics file ppm = " << NEURAL_NETWORK_VISUALISATION_PPM_FILE_NAME << endl;

		bool display = false;
		bool useOutputPPMFile = false;
		bool allowRaytrace = false;
		string outputPPMFileNameRaytraced = "";
		string outputTALFileName = "";
		int rasterImageWidth = 0;
		int rasterImageHeight = 0;
		outputNeuralNetworkToVectorGraphicsAndRaytrace(firstInputNeuronInNetwork, true, allowRaytrace, display, true, useOutputPPMFile, true, NEURAL_NETWORK_VISUALISATION_DAT_FILE_NAME_WITHOUT_SPRITES, NEURAL_NETWORK_VISUALISATION_DAT_FILE_NAME_WITH_SPRITES, NEURAL_NETWORK_VISUALISATION_SVG_FILE_NAME, NEURAL_NETWORK_VISUALISATION_PPM_FILE_NAME, outputPPMFileNameRaytraced, outputTALFileName, rasterImageWidth, rasterImageHeight);
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

	if(!parseANNrulesXMLfile())
	{
		result = false;
	}
	fillInANNSpriteExternVariables();

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
		cout << "7. Generate ANNexperience Data Using LRRC \n";
		cout << " ---\n";
#endif
		cout << "0. Exit\n";
		cout << " ---\n";
		cout <<	"Enter Answer (0,1,2,3... etc):\n\n>> ";

		cin >> answerAsString;
		answerAsInt = long(convertStringToDouble(answerAsString));
		if(answerAsInt == 1)
		{
			loadNetworkFromXML();
		}
		else if(answerAsInt == 2)
		{
			createNetwork();
		}
		else if(answerAsInt == 3)
		{
			loadExperienceDataFile();
		}
		else if(answerAsInt == 4)
		{
			trainNetwork(true);
		}
		else if(answerAsInt == 8)
		{
			trainNetwork(false);
		}
		else if(answerAsInt == 5)
		{
			outputNetworkToXML();
		}
		else if(answerAsInt == 6)
		{
			outputNetworkAsVectorGraphics();
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





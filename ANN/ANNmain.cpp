/*******************************************************************************
 *
 * This file is part of BAIPROJECT.
 *
 * BAIPROJECT is licensed under the GNU Affero General Public License
 * version 3, as published by the Free Software Foundation. The use of
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
 * File Name: ANNmain.cpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2017 Baxter AI (baxterai.com)
 * Project: Artificial Neural Network (ANN)
 * Project Version: 3m2a 10-July-2017
 * Comments: TH = Test Harness
 *
 *******************************************************************************/


#include "ANNmain.hpp"

#ifdef ANN_ALGORITHM_MEMORY_NETWORK
#endif
#ifdef ANN_ALGORITHM_CLASSIFICATION_NETWORK
#endif



#ifdef USE_LRRC
//#include "LRRCgame.hpp"
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



static char errmessage[] = "Usage:  ANN.exe [options]"
"\n\t,where options are any of the following"
"\n"
"\n\t-idata [string]   : neural network experience data set file"
"\n"
"\n\t-layers [int]     : neural network formation number of top level layers (def: 3)"
"\n\t-ineurons [int]   : neural network number of input neurons (def: dataset dependant)"
"\n\t-oneurons [int]   : neural network number of output neurons (def: dataset dependant)"
"\n"
#ifdef ANN_ALGORITHM_BACKPROPAGATION
"\n\t-divtype [int]    : subnet divergence type (1: linear converging, "
"\n\t\t\t 2: linear diverging then converging,3: nonlinear diverging then converging, "
"\n\t\t\t 4: linear diverging, 5: diverging square 2D, 6: diverging square 2D radial bias) (def: 1)"
"\n\t-divfactor [float]: subnet divergence factor (def: 1.5)"
"\n\t-con [float]      : probability of a neuron having connection to a previous layer neuron (def: 1.0)"
"\n\t-conall [float]   : probability of a neuron having direct link with all previous layers neurons (def: 0.0)"
"\n"
#ifdef ANN_ALGORITHM_BACKPROPAGATION_NETWORK
"\n\t-usesubnets       : create subnets (def: no)"
"\n\t-numsubnets[int]  : maximum number of subnet layers to create (def: 2)"
"\n\t-probsubnet[float]: the probability of subnet creation (def: 0.6)"
"\n\t-subnetdepl       : use subnet dependant number of layers (def: no)"
"\n"
#endif
#endif
"\n\t-epochs [float]   : preset number of training epochs (def: auto)"
"\n\t-maxepochs [int   : maximum number of training epochs (def: 500)"
"\n\t-maxFolds [int]   : number of training folds (def: 10)"
"\n"
"\n\t-train [int]      : train neural network (1: yes, 0: no) (def: 1)"
"\n"
"\n\t-ixml [string]    : neural network definition .xml input filename (def: neuralNet.xml)"
"\n"
"\n\t-oxml [string]    : neural network definition .xml output filename (def: neuralNet.xml)"
"\n\t-oldr [string]    : neural network display .ldr output filename (def: neuralNet.ldr)"
"\n\t-osvg [string]    : neural network display .svg output filename (def: neuralNet.svg)"
"\n\t-oppm [string]    : neural network display .ppm opengl output filename (def: neuralNet.ppm)"
"\n\t-oppm2 [string]   : neural network display .ppm raytraced output filename (def: neuralNetRaytraced.ppm)"
"\n\t-oall [string]    : neural network display .xml/.svg/.ldr/.ppm default generic output filename (def: neuralNet)"
#ifndef ANN_DISPLAY_DISABLE_SPRITES
"\n\t-sprites [int]    : neural network display output sprites (1: yes, 0: no) (def: 1)"
#endif
"\n\t-show             : display output in opengl"
"\n\t-width [int]      : raster graphics width in pixels (def: 640)"
"\n\t-height [int]     : raster graphics height in pixels (def: 480)"
"\n"
"\n\t-inputfolder [string]   : input directory name for input files (def: same as exe)"
"\n\t-exefolder [string]     : exe directory name for executables; ANN.exe (def: same as exe)"
"\n\t-outputfolder [string]  : output directory name for temporary and output files (def: same as exe)"
"\n"
"\n\t-ui               : loads text user interface upon execution (def: no)"
"\n\t-version          : print version"
"\n"
"\n\tThe program performs neural network operations - can load an experience dataset,"
"\n\tload a neural network definition xml file, form, train, test, and output neural networks"
"\n\tas .xml and .ldr "
"\n";

#ifdef COMPILE_ANN
int main(const int argc,const char* *argv)
{
	srand( (unsigned)time(NULL) );	//seeds randomness

	bool result = true;

	bool useInputDatasetFile = false;
	string inputDatasetFileName = "dataset.data";

	#ifdef ANN_ALGORITHM_CLASSIFICATION_NETWORK
	long numberOfInputNeurons = 0;	//need to be generated based upon dataset or xml input file
	long numberOfOutputNeurons = 0;	//dynamically generated while creating classification net
	int numberOfLayers = 0;	//NOTUSED
	#else
	long numberOfInputNeurons = 5;	//need to be generated based upon dataset or xml input file
	long numberOfOutputNeurons = 3;	//need to be generated based upon dataset or xml input file
	int numberOfLayers = 3;
	#endif

	#ifdef ANN_ALGORITHM_BACKPROPAGATION
	int layerDivergenceType = LAYER_DIVERGENCE_TYPE_LINEAR_CONVERGING;
	double meanLayerDivergenceFactor = DEFAULT_MEAN_LAYER_DIVERGENCE_FACTOR;
	double probabilityANNneuronConnectionWithPreviousLayerNeuron = DEFAULT_PROBABILITY_NEURON_CONNECTION_WITH_PREVIOUS_LAYER_NEURON_ANNTH;
	double probabilityANNneuronConnectionWithAllPreviousLayersNeurons = DEFAULT_PROBABILITY_NEURON_CONNECTION_WITH_ALL_PREVIOUS_LAYERS_NEURONS_ANNTH;
	#ifdef ANN_ALGORITHM_BACKPROPAGATION_NETWORK
	bool useSubnets = false;
	long maxNumRecursiveSubnets = DEFAULT_ANN_MAX_NUM_RECURSIVE_SUBNETS_ANNTH;
	double probabilityOfSubnetCreation = DEFAULT_PROB_OF_SUBNET_CREATION_ANNTH;
	bool useSubnetDependentNumberOfLayers = true;
	#endif
	#endif

	int maxFolds = MAX_NUM_FOLDS_ANNTH;
	int maxNumEpochs = ANN_DEFAULT_MAX_NUMBER_OF_EPOCHS;
	int numEpochs = ANN_DEFAULT_SIMPLE_TRAIN_DEFAULT_NUM_OF_TRAINING_EPOCHS;
	bool usePresetNumberOfEpochs = false;

	bool useInputXMLFile = false;
	string inputXMLFileName = string(NEURAL_NETWORK_VISUALISATION_BASE_FILE_NAME) + NEURAL_NETWORK_VISUALISATION_XML_FILE_EXTENSION;
	bool useOutputXMLFile = false;
	string outputXMLFileName = string(NEURAL_NETWORK_VISUALISATION_BASE_FILE_NAME) + NEURAL_NETWORK_VISUALISATION_XML_FILE_EXTENSION;
	bool useOutputLDRFile = false;
	string outputLDRFileName = string(NEURAL_NETWORK_VISUALISATION_BASE_FILE_NAME) + NEURAL_NETWORK_VISUALISATION_LDR_FILE_EXTENSION;
	bool useOutputSVGFile = false;
	string outputSVGFileName = string(NEURAL_NETWORK_VISUALISATION_BASE_FILE_NAME) + NEURAL_NETWORK_VISUALISATION_SVG_FILE_EXTENSION;
	bool useOutputPPMFile = false;
	string outputPPMFileName = string(NEURAL_NETWORK_VISUALISATION_BASE_FILE_NAME) + NEURAL_NETWORK_VISUALISATION_PPM_FILE_EXTENSION;
	bool useOutputPPMFileRaytraced = false;
	string outputPPMFileNameRaytraced = string(NEURAL_NETWORK_VISUALISATION_BASE_FILE_NAME) + NEURAL_NETWORK_VISUALISATION_PPM_RAYTRACED_FILE_EXTENSION;
	string outputTALFileName = string(NEURAL_NETWORK_VISUALISATION_BASE_FILE_NAME) + NEURAL_NETWORK_VISUALISATION_TAL_FILE_EXTENSION;
	bool useOutputAllFile = false;
	string outputAllFileName = NEURAL_NETWORK_VISUALISATION_BASE_FILE_NAME;
	
	bool useSprites = true;	//default true

	bool displayInOpenGL = false;
	bool drawOutput = false;

	int rasterImageWidth = 1024;
	int rasterImageHeight = 768;

	bool useTextUI = false;

	int version;

	bool trainIfUseInputDatasetFile = true;
	//bool form = true;

	//basic execution flow outline; if no dataset or xml input file is specified, just form network - do not train network

	if((SHAREDvarsClass().argumentExists(argc, argv, "-idata")) || (SHAREDvarsClass().argumentExists(argc, argv, "-ixml")) || (SHAREDvarsClass().argumentExists(argc, argv, "-oxml")) || (SHAREDvarsClass().argumentExists(argc, argv, "-oldr")) || (SHAREDvarsClass().argumentExists(argc, argv, "-osvg")) || (SHAREDvarsClass().argumentExists(argc, argv, "-oppm")) || (SHAREDvarsClass().argumentExists(argc, argv, "-oppm2")) || (SHAREDvarsClass().argumentExists(argc, argv, "-oall")) || (SHAREDvarsClass().argumentExists(argc, argv, "-ui")))
	{
		if(SHAREDvarsClass().argumentExists(argc, argv, "-idata"))
		{
			inputDatasetFileName = SHAREDvarsClass().getStringArgument(argc, argv, "-idata");
			//train = true;
			useInputDatasetFile = true;
		}

		if(SHAREDvarsClass().argumentExists(argc, argv, "-layers"))
		{
			numberOfLayers = SHAREDvarsClass().getFloatArgument(argc, argv, "-layers");
		}
		if(SHAREDvarsClass().argumentExists(argc, argv, "-ineurons"))
		{
			numberOfInputNeurons = SHAREDvarsClass().getFloatArgument(argc, argv, "-ineurons");
		}
		#ifndef ANN_ALGORITHM_CLASSIFICATION_NETWORK
		if(SHAREDvarsClass().argumentExists(argc, argv, "-oneurons"))
		{
			numberOfOutputNeurons = SHAREDvarsClass().getFloatArgument(argc, argv, "-oneurons");
		}
		#endif

		#ifdef ANN_ALGORITHM_BACKPROPAGATION
		if(SHAREDvarsClass().argumentExists(argc, argv, "-divtype"))
		{
			layerDivergenceType = SHAREDvarsClass().getFloatArgument(argc, argv, "-divtype");
		}
		if(SHAREDvarsClass().argumentExists(argc, argv, "-divfactor"))
		{
			meanLayerDivergenceFactor = SHAREDvarsClass().getFloatArgument(argc, argv, "-divfactor");
		}
		if(SHAREDvarsClass().argumentExists(argc, argv, "-con"))
		{
			probabilityANNneuronConnectionWithPreviousLayerNeuron = SHAREDvarsClass().getFloatArgument(argc, argv, "-con");
		}
		if(SHAREDvarsClass().argumentExists(argc, argv, "-conall"))
		{
			probabilityANNneuronConnectionWithAllPreviousLayersNeurons = SHAREDvarsClass().getFloatArgument(argc, argv, "-conall");
		}
		#ifdef ANN_ALGORITHM_BACKPROPAGATION_NETWORK
		if(SHAREDvarsClass().argumentExists(argc, argv, "-usesubnets"))
		{
			useSubnets = true;
		}
		if(SHAREDvarsClass().argumentExists(argc, argv, "-numsubnets"))
		{
			maxNumRecursiveSubnets = SHAREDvarsClass().getFloatArgument(argc, argv, "-numsubnets");
		}
		if(SHAREDvarsClass().argumentExists(argc, argv, "-probsubnet"))
		{
			probabilityOfSubnetCreation = SHAREDvarsClass().getFloatArgument(argc, argv, "-probsubnet");
		}
		if(SHAREDvarsClass().argumentExists(argc, argv, "-subnetdepl"))
		{
			useSubnetDependentNumberOfLayers = true;
		}
		#endif
		#endif

		if(SHAREDvarsClass().argumentExists(argc, argv, "-epochs"))
		{
			numEpochs=SHAREDvarsClass().getFloatArgument(argc, argv, "-epochs");
			usePresetNumberOfEpochs = true;
		}
		if(SHAREDvarsClass().argumentExists(argc, argv, "-maxepochs"))
		{
			maxNumEpochs = SHAREDvarsClass().getFloatArgument(argc, argv, "-maxepochs");
		}
		if(SHAREDvarsClass().argumentExists(argc, argv, "-maxFolds"))
		{
			maxFolds = SHAREDvarsClass().getFloatArgument(argc, argv, "-maxFolds");
		}

		if(SHAREDvarsClass().argumentExists(argc, argv, "-ixml"))
		{
			inputXMLFileName = SHAREDvarsClass().getStringArgument(argc, argv, "-ixml");
			//train = true;
			useInputXMLFile = true;
		}
		if(SHAREDvarsClass().argumentExists(argc, argv, "-oxml"))
		{
			outputXMLFileName = SHAREDvarsClass().getStringArgument(argc, argv, "-oxml");
			useOutputXMLFile = true;
		}
		if(SHAREDvarsClass().argumentExists(argc, argv, "-oldr"))
		{
			outputLDRFileName = SHAREDvarsClass().getStringArgument(argc, argv, "-oldr");
			useOutputLDRFile = true;
		}
		if(SHAREDvarsClass().argumentExists(argc, argv, "-osvg"))
		{
			outputSVGFileName = SHAREDvarsClass().getStringArgument(argc, argv, "-osvg");
			useOutputSVGFile = true;
		}
		if(SHAREDvarsClass().argumentExists(argc, argv, "-oppm"))
		{
			outputPPMFileName = SHAREDvarsClass().getStringArgument(argc, argv, "-oppm");
			useOutputPPMFile = true;
			useOutputLDRFile = true;	//required for OpenGL image generation
		}
		if(SHAREDvarsClass().argumentExists(argc, argv, "-oppm2"))
		{
			outputPPMFileNameRaytraced = SHAREDvarsClass().getStringArgument(argc, argv, "-oppm2");
			useOutputPPMFileRaytraced = true;
			useOutputLDRFile = true;	//required for raytrace image generation
		}
		if(SHAREDvarsClass().argumentExists(argc, argv, "-oall"))
		{
			outputAllFileName = SHAREDvarsClass().getStringArgument(argc, argv, "-oall");
			useOutputAllFile = true;
		}
		
		#ifndef ANN_DISPLAY_DISABLE_SPRITES
		if(SHAREDvarsClass().argumentExists(argc, argv, "-sprites"))
		{
			useSprites = true;
		}
		#endif

		if(SHAREDvarsClass().argumentExists(argc, argv, "-show"))
		{
			displayInOpenGL = true;
		}
		if(SHAREDvarsClass().argumentExists(argc, argv, "-width"))
		{
			rasterImageWidth = SHAREDvarsClass().getFloatArgument(argc, argv, "-width");
		}
		if(SHAREDvarsClass().argumentExists(argc, argv, "-height"))
		{
			rasterImageHeight = SHAREDvarsClass().getFloatArgument(argc, argv, "-height");
		}

		if(SHAREDvarsClass().argumentExists(argc, argv, "-train"))
		{
			int trainInt;
			trainInt = SHAREDvarsClass().getFloatArgument(argc, argv, "-train");
			trainIfUseInputDatasetFile = (bool)trainInt;
		}

		string currentFolder = SHAREDvarsClass().getCurrentDirectory();

		if(SHAREDvarsClass().argumentExists(argc, argv, "-inputfolder"))
		{
			inputFolder = SHAREDvarsClass().getStringArgument(argc, argv, "-inputfolder");
		}
		else
		{
			inputFolder = currentFolder;
		}
		if(SHAREDvarsClass().argumentExists(argc, argv, "-exefolder"))
		{
			exeFolder = SHAREDvarsClass().getStringArgument(argc, argv, "-exefolder");
		}
		else
		{
			exeFolder = currentFolder;
		}
		if(SHAREDvarsClass().argumentExists(argc, argv, "-outputfolder"))
		{
			outputFolder = SHAREDvarsClass().getStringArgument(argc, argv, "-outputfolder");
		}
		else
		{
			outputFolder = currentFolder;
		}

		SHAREDvarsClass().setCurrentDirectory(inputFolder);

		if(SHAREDvarsClass().argumentExists(argc, argv, "-ui"))
		{
			useTextUI = true;
		}
		if(SHAREDvarsClass().argumentExists(argc, argv, "-version"))
		{
			cout << "Project Version: 3m2a 10-July-2017" << endl;
			exit(EXIT_OK);
		}
	}
	else
	{
		cerr << errmessage << endl;
		exit(EXIT_ERROR);
	}

	if(useOutputPPMFile)
	{
		displayInOpenGL = true;
	}
	if(useOutputLDRFile)
	{
		drawOutput = true;
	}
	if(useOutputSVGFile)
	{
		drawOutput = true;
	}
	if(useOutputPPMFile)
	{
		drawOutput = true;
	}
	if(useOutputPPMFileRaytraced)
	{
		drawOutput = true;
	}
	if(useOutputAllFile)
	{
		drawOutput = true;
	}	
	
	if(!XMLrulesClassClass().parseANNrulesXMLfile())
	{
		result = false;
	}
	ANNdrawClass().fillInANNSpriteExternVariables();

	if(useTextUI)
	{
		ANNmainClass().mainUI();
	}

	if(displayInOpenGL)
	{
		drawOutput = true;
		useOutputLDRFile = true;	//required for OpenGL image generation
		
		LDopenglClass().initiateOpenGL(rasterImageWidth, rasterImageHeight, 0, 0, false);
	}

	if(drawOutput)
	{
		if(useOutputAllFile)
		{
			if(!useOutputLDRFile)
			{
				useOutputLDRFile = true;
				outputLDRFileName = outputAllFileName + NEURAL_NETWORK_VISUALISATION_LDR_FILE_EXTENSION;
			}

			if(!useOutputXMLFile)
			{
				useOutputXMLFile = true;
				outputXMLFileName = outputAllFileName + NEURAL_NETWORK_VISUALISATION_XML_FILE_EXTENSION;
			}

			if(!useOutputSVGFile)
			{
				useOutputSVGFile = true;	//SVG output is not always required when printing/drawing neural network
				outputSVGFileName = outputAllFileName + NEURAL_NETWORK_VISUALISATION_SVG_FILE_EXTENSION;
			}
			if(!useOutputPPMFile)
			{
				useOutputPPMFile = true;
				outputPPMFileName = outputAllFileName + NEURAL_NETWORK_VISUALISATION_PPM_FILE_EXTENSION;
			}
			/* disable raytrace output by default
			if(!useOutputPPMFileRaytraced)
			{
				useOutputPPMFileRaytraced = true;
				outputPPMFileNameRaytraced = outputAllFileName + NEURAL_NETWORK_VISUALISATION_PPM_RAYTRACED_FILE_EXTENSION;
			}
			*/
		}
	}

	if(useInputXMLFile)
	{//create a network and load input dataset file
		//Neural Network initialisations
		firstInputNeuronInNetwork = new ANNneuron();

		long numberOfInputNeuronsLoaded;
		long numberOfOutputNeuronsLoaded;

		string xmlFileName = inputXMLFileName;

		firstOutputNeuronInNetwork = ANNxmlConversionClass().readNetXMLfileAndRecordFormationVariables(xmlFileName, firstInputNeuronInNetwork, &numberOfInputNeuronsLoaded, &numberOfOutputNeuronsLoaded);

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

			ANNparserClass().ANNparseDataFile(nameOfExperiencesDataSetFile);

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

		#ifdef ANN_ALGORITHM_CLASSIFICATION_NETWORK
		ANNformationClass().formNeuralNetworkInputLayer(firstInputNeuronInNetwork, numberOfInputNeurons);
		#elif defined ANN_ALGORITHM_BACKPROPAGATION_NETWORK
		if(useSubnets)
		{
			firstOutputNeuronInNetwork = ANNformationClass().formAdvancedNeuralNetwork(firstInputNeuronInNetwork, numberOfInputNeurons, numberOfOutputNeurons, useSubnetDependentNumberOfLayers, probabilityOfSubnetCreation, maxNumRecursiveSubnets, numberOfLayers, layerDivergenceType, meanLayerDivergenceFactor, probabilityANNneuronConnectionWithPreviousLayerNeuron, probabilityANNneuronConnectionWithAllPreviousLayersNeurons);

		}
		else
		{
			firstOutputNeuronInNetwork = ANNformationClass().formNeuralNet(firstInputNeuronInNetwork, numberOfInputNeurons, numberOfOutputNeurons, numberOfLayers, layerDivergenceType, meanLayerDivergenceFactor, probabilityANNneuronConnectionWithPreviousLayerNeuron, probabilityANNneuronConnectionWithAllPreviousLayersNeurons);
		}
		#elif defined ANN_ALGORITHM_BREAKAWAY_NETWORK
		firstOutputNeuronInNetwork = ANNformationClass().formNeuralNet(firstInputNeuronInNetwork, numberOfInputNeurons, numberOfOutputNeurons, numberOfLayers, layerDivergenceType, meanLayerDivergenceFactor, probabilityANNneuronConnectionWithPreviousLayerNeuron, probabilityANNneuronConnectionWithAllPreviousLayersNeurons);		
		#endif

		#ifdef ANN_ALGORITHM_BACKPROPAGATION
		cout << "subnet Divergence Type = " << layerDivergenceType << endl;
		cout << "mean Subnet Divergence Factor = " << meanLayerDivergenceFactor << endl;
		cout << "probability Of ANNneuron Connection Exclusivity = " << probabilityANNneuronConnectionWithPreviousLayerNeuron <<endl;
		#ifdef ANN_ALGORITHM_BACKPROPAGATION_NETWORK
		cout << "Probability Of Subnet Creation = " << probabilityOfSubnetCreation <<endl;
		cout << "use Subnet Dependent Number Of Layers = " << useSubnetDependentNumberOfLayers << endl;
		cout << "Max Number Recursive Subnets = " << maxNumRecursiveSubnets <<endl;
		#endif
		#endif
		cout << "Number Of top level Layers = " << numberOfLayers << endl;
		cout << "number Of Input Neurons = " << numberOfInputNeurons << endl;
		cout << "number Of Output Neurons = " << numberOfOutputNeurons << endl;
	}

	if(useInputDatasetFile)
	{//train the network

		if(trainIfUseInputDatasetFile)
		{
			if(!usePresetNumberOfEpochs)
			{
				#ifdef ANN_ALGORITHM_BACKPROPAGATION_NETWORK
				ANNalgorithmBackpropagationTrainingClass().trainNeuralNetworkBackpropagation(firstInputNeuronInNetwork, firstOutputNeuronInNetwork, numberOfInputNeurons, numberOfOutputNeurons, maxFolds, firstExperienceInDataSet, numExperiences, maxNumEpochs);
				#endif
				#ifdef ANN_ALGORITHM_BREAKAWAY_NETWORK
				int sizeOfSupernet = ANN_ALGORITHM_BREAKAWAY_NETWORK_ADD_SIZE_SUPERNET;
				ANNalgorithmBreakawayNetworkTrainingClass().trainNeuralNetworkBreakaway(firstInputNeuronInNetwork, &firstOutputNeuronInNetwork, numberOfInputNeurons, numberOfOutputNeurons, false, numEpochs, maxFolds, maxNumEpochs, firstExperienceInDataSet, numExperiences, sizeOfSupernet, numberOfLayers);
				#endif
				#ifdef ANN_ALGORITHM_MEMORY_NETWORK
				ANNalgorithmMemoryNetworkTrainingClass().trainNeuralNetworkMemory(firstInputNeuronInNetwork, firstOutputNeuronInNetwork, numberOfInputNeurons, numberOfOutputNeurons, maxFolds, firstExperienceInDataSet, numExperiences);
				#endif
				#ifdef ANN_ALGORITHM_CLASSIFICATION_NETWORK
				ANNalgorithmClassificationNetworkTrainingClass().trainNeuralNetworkClassificationSimple(firstInputNeuronInNetwork, &firstOutputNeuronInNetwork, numberOfInputNeurons, &numberOfOutputNeurons, firstExperienceInDataSet, numExperiences);
				#endif
			}
			else
			{
				#ifdef ANN_ALGORITHM_BACKPROPAGATION_NETWORK
				ANNalgorithmBackpropagationTrainingClass().trainNeuralNetworkBackpropagationSimple(firstInputNeuronInNetwork, firstOutputNeuronInNetwork, numberOfInputNeurons, numberOfOutputNeurons, numEpochs, firstExperienceInDataSet, numExperiences);
				#endif
				#ifdef ANN_ALGORITHM_BREAKAWAY_NETWORK
				int sizeOfSupernet = ANN_ALGORITHM_BREAKAWAY_NETWORK_ADD_SIZE_SUPERNET;
				ANNalgorithmBreakawayNetworkTrainingClass().trainNeuralNetworkBreakaway(firstInputNeuronInNetwork, &firstOutputNeuronInNetwork, numberOfInputNeurons, numberOfOutputNeurons, true, numEpochs, maxFolds, maxNumEpochs, firstExperienceInDataSet, numExperiences, sizeOfSupernet, numberOfLayers);
				#endif
				#ifdef ANN_ALGORITHM_MEMORY_NETWORK
				ANNalgorithmMemoryNetworkTrainingClass().trainNeuralNetworkMemorySimple(firstInputNeuronInNetwork, firstOutputNeuronInNetwork, numberOfInputNeurons, numberOfOutputNeurons, firstExperienceInDataSet, numExperiences);
				#endif
				#ifdef ANN_ALGORITHM_CLASSIFICATION_NETWORK
				ANNalgorithmClassificationNetworkTrainingClass().trainNeuralNetworkClassificationSimple(firstInputNeuronInNetwork, &firstOutputNeuronInNetwork, numberOfInputNeurons, &numberOfOutputNeurons, firstExperienceInDataSet, numExperiences);
				#endif
			}
		}
	}

	SHAREDvarsClass().setCurrentDirectory(outputFolder);

	if(drawOutput)
	{
		ANNdisplayClass().outputNeuralNetworkToVectorGraphicsAndRaytrace(firstInputNeuronInNetwork, useSprites, useOutputPPMFileRaytraced, displayInOpenGL, useOutputLDRFile, useOutputSVGFile, useOutputPPMFile, outputLDRFileName, outputSVGFileName, outputPPMFileName, outputPPMFileNameRaytraced, outputTALFileName, rasterImageWidth, rasterImageHeight);
	}

	if(useOutputXMLFile)
	{
		cout << "XML file name = " << outputXMLFileName << endl;

		if(!ANNxmlConversionClass().writeNetXMLfile(outputXMLFileName, firstInputNeuronInNetwork))
		{
			result = false;
		}
	}

}
#endif


bool ANNmainClass::loadNetworkFromXML()
{
	bool result = true;

	//Neural Network initialisations
	firstInputNeuronInNetwork = new ANNneuron();

	long numberOfInputNeuronsLoaded;
	long numberOfOutputNeuronsLoaded;

	string xmlFileName = NET_XML_FILE_NAME;

	firstOutputNeuronInNetwork = ANNxmlConversion.readNetXMLfileAndRecordFormationVariables(xmlFileName, firstInputNeuronInNetwork, &numberOfInputNeuronsLoaded, &numberOfOutputNeuronsLoaded);

	numberOfInputNeurons = numberOfInputNeuronsLoaded;
	numberOfOutputNeurons = numberOfOutputNeuronsLoaded;

	formedNetwork = true;

	return result;
}

#ifndef ANN_ALGORITHM_CLASSIFICATION_NETWORK
bool ANNmainClass::createNetwork()
{
	bool result = true;

	//Neural Network initialisations
	firstInputNeuronInNetwork = new ANNneuron();


	int layerDivergenceType = 0;
	double meanLayerDivergenceFactor = 0;
	double probabilityANNneuronConnectionWithPreviousLayerNeuron = 0;
	double probabilityOfSubnetCreation = 0;
	bool useSubnetDependentNumberOfLayers = 0;
	long maxNumRecursiveSubnets = 0;
	int numberOfLayers = 0;
	double probabilityANNneuronConnectionWithAllPreviousLayersNeurons = 0;


	double answerAsDouble;
	long answerAsInt;
	char answerAsChar;
	string answerAsString;


	if(!loadedExperienceDataset)
	{
		cout <<	"\nEnter the number of input neurons (2, 3, 4, 5 [Default], ... etc):\n\n>> ";
		cin >> answerAsString;
		answerAsInt = long(SHAREDvars.convertStringToDouble(answerAsString));
		numberOfInputNeurons = answerAsInt;

		cout <<	"\nEnter the number of output neurons (2, 3 [Default], 4, 5, ... etc):\n\n>> ";
		cin >> answerAsString;
		answerAsInt = long(SHAREDvars.convertStringToDouble(answerAsString));
		numberOfOutputNeurons = answerAsInt;

		numberOfInputAndOutputNeuronsSelected = true;
	}


	cout <<	"\nEnter the number of top level Layers (2, 3 [Default], ... etc):\n\n>> ";
	cin >> answerAsString;
	answerAsInt = long(SHAREDvars.convertStringToDouble(answerAsString));
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
		answerAsInt = long(SHAREDvars.convertStringToDouble(answerAsString));
		layerDivergenceType = answerAsInt;

		cout <<	"\nEnter the mean layer divergence factor [Default = " << DEFAULT_MEAN_LAYER_DIVERGENCE_FACTOR << "]:\n\n>> ";
		cin >> answerAsString;
		answerAsDouble = SHAREDvars.convertStringToDouble(answerAsString);
		meanLayerDivergenceFactor = answerAsDouble;


		cout <<	"\nEnter the probability of neuron having a connection with a previous layer neuron [Default = " << DEFAULT_PROBABILITY_NEURON_CONNECTION_WITH_PREVIOUS_LAYER_NEURON_ANNTH << "]:\n\n>> ";
		cin >> answerAsString;
		answerAsDouble = SHAREDvars.convertStringToDouble(answerAsString);
		probabilityANNneuronConnectionWithPreviousLayerNeuron = answerAsDouble;


		cout <<	"\nEnter the probability of a neuron having a direct link with all previous layers neurons [Default = " << DEFAULT_PROBABILITY_NEURON_CONNECTION_WITH_ALL_PREVIOUS_LAYERS_NEURONS_ANNTH << "]:\n\n>> ";
		cin >> answerAsString;
		answerAsDouble = SHAREDvars.convertStringToDouble(answerAsString);
		probabilityANNneuronConnectionWithAllPreviousLayersNeurons = answerAsDouble;


		//subnet/ANN specific parameters....
		cout <<	"\nDo you wish to create Subnets? (y/n) [Default = n] \n\n>> ";
		cin >> answerAsString;
		answerAsChar = answerAsString[0];

		if(answerAsChar == 'y')
		{
			cout <<	"\nEnter the maximum number of subnet layers you wish to create (1,2,3,4,5,... etc) [Default = " << DEFAULT_ANN_MAX_NUM_RECURSIVE_SUBNETS_ANNTH << "]:\n\n>> ";
			cin >> answerAsString;
			answerAsInt = long(SHAREDvars.convertStringToDouble(answerAsString));
			maxNumRecursiveSubnets = answerAsInt;

			cout <<	"\nEnter the probability of subnet creation [Default = " << DEFAULT_PROB_OF_SUBNET_CREATION_ANNTH << "]:\n\n>> ";
			cin >> answerAsString;
			answerAsDouble = SHAREDvars.convertStringToDouble(answerAsString);
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

			firstOutputNeuronInNetwork = ANNformation.formAdvancedNeuralNetwork(firstInputNeuronInNetwork, numberOfInputNeurons, numberOfOutputNeurons, useSubnetDependentNumberOfLayers, probabilityOfSubnetCreation, maxNumRecursiveSubnets, numberOfLayers, layerDivergenceType, meanLayerDivergenceFactor, probabilityANNneuronConnectionWithPreviousLayerNeuron, probabilityANNneuronConnectionWithAllPreviousLayersNeurons);

		}
		else
		{
			firstOutputNeuronInNetwork = ANNformation.formNeuralNet(firstInputNeuronInNetwork, numberOfInputNeurons, numberOfOutputNeurons, numberOfLayers, layerDivergenceType, meanLayerDivergenceFactor, probabilityANNneuronConnectionWithPreviousLayerNeuron, probabilityANNneuronConnectionWithAllPreviousLayersNeurons);
		}
	}
	else
	{
		firstOutputNeuronInNetwork = ANNformation.formNeuralNetWithOptimisedProperties(firstInputNeuronInNetwork, numberOfInputNeurons, numberOfOutputNeurons, numberOfLayers);
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
#endif





bool ANNmainClass::loadExperienceDataFile()
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

	ANNparser.ANNparseDataFile(nameOfExperiencesDataSetFile);

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


bool ANNmainClass::trainNetwork(const bool advancedTraining)
{
	bool result = true;


	if(!loadedExperienceDataset)
	{
		if(!this->loadExperienceDataFile())
		{
			result = false;
		}
	}

	if(formedNetwork)
	{
		int numberOfLayers = ANNformation.countNumberOfLayersInSubnet(firstInputNeuronInNetwork);
		cout << "numberOfLayers = " << numberOfLayers << endl;
		int numEpochs = ANN_DEFAULT_SIMPLE_TRAIN_DEFAULT_NUM_OF_TRAINING_EPOCHS;
		int maxFolds = MAX_NUM_FOLDS_ANNTH;		//maximum number of folds are same regardless of TEST_LEVEL_X_ANNT
		int maxNumEpochs = ANN_DEFAULT_MAX_NUMBER_OF_EPOCHS;

		if(advancedTraining)
		{
			#ifdef ANN_ALGORITHM_BACKPROPAGATION_NETWORK
			ANNalgorithmBackpropagationTraining.trainNeuralNetworkBackpropagation(firstInputNeuronInNetwork, firstOutputNeuronInNetwork, numberOfInputNeurons, numberOfOutputNeurons, maxFolds, firstExperienceInDataSet, numExperiences, maxNumEpochs);
			#endif
			#ifdef ANN_ALGORITHM_BREAKAWAY_NETWORK
			int sizeOfSupernet = ANN_ALGORITHM_BREAKAWAY_NETWORK_ADD_SIZE_SUPERNET;
			ANNalgorithmBreakawayNetworkTraining.trainNeuralNetworkBreakaway(firstInputNeuronInNetwork, &firstOutputNeuronInNetwork, numberOfInputNeurons, numberOfOutputNeurons, false, numEpochs, maxFolds, maxNumEpochs, firstExperienceInDataSet, numExperiences, sizeOfSupernet, numberOfLayers);			
			#endif
			#ifdef ANN_ALGORITHM_MEMORY_NETWORK
			ANNalgorithmMemoryNetworkTraining.trainNeuralNetworkMemory(firstInputNeuronInNetwork, firstOutputNeuronInNetwork, numberOfInputNeurons, numberOfOutputNeurons, maxFolds, firstExperienceInDataSet, numExperiences);
			#endif
			#ifdef ANN_ALGORITHM_CLASSIFICATION_NETWORK
			ANNalgorithmClassificationNetworkTraining.trainNeuralNetworkClassificationSimple(firstInputNeuronInNetwork, &firstOutputNeuronInNetwork, numberOfInputNeurons, &numberOfOutputNeurons, firstExperienceInDataSet, numExperiences);
			#endif
		}
		else
		{			
			#ifdef ANN_ALGORITHM_BACKPROPAGATION_NETWORK
			ANNalgorithmBackpropagationTraining.trainNeuralNetworkBackpropagationSimple(firstInputNeuronInNetwork, firstOutputNeuronInNetwork, numberOfInputNeurons, numberOfOutputNeurons, numEpochs, firstExperienceInDataSet, numExperiences);
			#endif
			#ifdef ANN_ALGORITHM_BREAKAWAY_NETWORK
			int sizeOfSupernet = ANN_ALGORITHM_BREAKAWAY_NETWORK_ADD_SIZE_SUPERNET;
			ANNalgorithmBreakawayNetworkTraining.trainNeuralNetworkBreakaway(firstInputNeuronInNetwork, &firstOutputNeuronInNetwork, numberOfInputNeurons, numberOfOutputNeurons, true, numEpochs, maxFolds, maxNumEpochs, firstExperienceInDataSet, numExperiences, sizeOfSupernet, numberOfLayers);			
			#endif
			#ifdef ANN_ALGORITHM_MEMORY_NETWORK
			ANNalgorithmMemoryNetworkTraining.trainNeuralNetworkMemorySimple(firstInputNeuronInNetwork, firstOutputNeuronInNetwork, numberOfInputNeurons, numberOfOutputNeurons, firstExperienceInDataSet, numExperiences);
			#endif
			#ifdef ANN_ALGORITHM_CLASSIFICATION_NETWORK
			ANNalgorithmClassificationNetworkTraining.trainNeuralNetworkClassificationSimple(firstInputNeuronInNetwork, &firstOutputNeuronInNetwork, numberOfInputNeurons, &numberOfOutputNeurons, firstExperienceInDataSet, numExperiences);
			#endif
		}
	}
	else
	{
		cout << "Warning: cannot train network - no network has been formed" << endl;
	}
	return result;

}

bool ANNmainClass::outputNetworkToXML()
{
	SHAREDvars.setCurrentDirectory(outputFolder);
	bool result = true;

	cout << "XML file name = " << NET_XML_FILE_NAME << endl;

	if(formedNetwork)
	{
		if(!ANNxmlConversion.writeNetXMLfile(NET_XML_FILE_NAME, firstInputNeuronInNetwork))
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



bool ANNmainClass::outputNetworkAsVectorGraphics()
{
	SHAREDvars.setCurrentDirectory(outputFolder);
	bool result = true;

	if(formedNetwork)
	{
		bool useOutputLDRFile = true;
		string outputLDRFileName = string(NEURAL_NETWORK_VISUALISATION_BASE_FILE_NAME) + NEURAL_NETWORK_VISUALISATION_LDR_FILE_EXTENSION;
		bool useOutputSVGFile = true;
		string outputSVGFileName = string(NEURAL_NETWORK_VISUALISATION_BASE_FILE_NAME) + NEURAL_NETWORK_VISUALISATION_SVG_FILE_EXTENSION;
		bool useOutputPPMFile = false;
		string outputPPMFileName = string(NEURAL_NETWORK_VISUALISATION_BASE_FILE_NAME) + NEURAL_NETWORK_VISUALISATION_PPM_FILE_EXTENSION;
		bool useOutputPPMFileRaytraced = false;
		string outputPPMFileNameRaytraced = string(NEURAL_NETWORK_VISUALISATION_BASE_FILE_NAME) + NEURAL_NETWORK_VISUALISATION_PPM_RAYTRACED_FILE_EXTENSION;
		string outputTALFileName = string(NEURAL_NETWORK_VISUALISATION_BASE_FILE_NAME) + NEURAL_NETWORK_VISUALISATION_TAL_FILE_EXTENSION;

		bool addSprites = true;
		bool displayInOpenGL = false;
		bool allowRaytrace = false;
		int rasterImageWidth = 0;
		int rasterImageHeight = 0;
		
		cout << "vector graphics file name = " << outputLDRFileName << endl;
		cout << "vector graphics file svg = " << outputSVGFileName << endl;
		cout << "vector graphics file ppm = " << useOutputPPMFile << endl;
		
		if(displayInOpenGL)
		{
			LDopengl.initiateOpenGL(rasterImageWidth, rasterImageHeight, 0, 0, false);
		}
		
		ANNdisplay.outputNeuralNetworkToVectorGraphicsAndRaytrace(firstInputNeuronInNetwork, addSprites, allowRaytrace, displayInOpenGL, useOutputLDRFile, useOutputSVGFile, useOutputPPMFile, outputLDRFileName, outputSVGFileName, outputPPMFileName, outputPPMFileNameRaytraced, outputTALFileName, rasterImageWidth, rasterImageHeight);
	}
	else
	{
		cout << "Warning: cannot output network as vector graphics - no network has been formed" << endl;
	}

	return result;
}


bool ANNmainClass::mainUI()
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
		#ifndef ANN_ALGORITHM_CLASSIFICATION_NETWORK
		cout << "2. Create a new network \n";
		#endif
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
		answerAsInt = long(SHAREDvars.convertStringToDouble(answerAsString));
		if(answerAsInt == 1)
		{
			this->loadNetworkFromXML();
		}
		#ifndef ANN_ALGORITHM_CLASSIFICATION_NETWORK
		else if(answerAsInt == 2)
		{
			this->createNetwork();
		}
		#endif
		else if(answerAsInt == 3)
		{
			this->loadExperienceDataFile();
		}
		else if(answerAsInt == 4)
		{
			this->trainNetwork(true);
		}
		else if(answerAsInt == 8)
		{
			this->trainNetwork(false);
		}
		else if(answerAsInt == 5)
		{
			this->outputNetworkToXML();
		}
		else if(answerAsInt == 6)
		{
			this->outputNetworkAsVectorGraphics();
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





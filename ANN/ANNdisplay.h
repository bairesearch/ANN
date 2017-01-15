/*******************************************************************************
 *
 * File Name: ANNdisplay.h
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2012 Baxter AI (baxterai.com)
 * Project: Advanced Neural Network (ANN)
 * Project Version: 3a7d 11-June-2012
 * Comments: TH = Test Harness
 *
 *******************************************************************************/

#ifndef HEADER_ANN_DISPLAY
#define HEADER_ANN_DISPLAY

#include "ANNexperienceClass.h"
#include "ANNneuronClass.h"
#include "ANNglobalDefs.h"


#ifdef USE_RT
void generateExperienceWith2DRGBMap(unsigned char * rgbMap, int imageWidth, int imageHeight, Experience * currentExperience, long objectDecision);
void generateExperienceWith2DMap(double * lumOrContrastOrDepthMap, int imageWidth, int imageHeight, double mapMaxValue, Experience * currentExperience, long objectDecision);
void generateExperienceWith2DBooleanMap(bool * booleanMap, int imageWidth, int imageHeight, Experience * currentExperience, long objectDecision);
#endif


bool trainAndOutputNeuralNetworkWithFileNames(NeuronContainer * firstInputNeuronInNetwork, NeuronContainer * firstOutputNeuronInNetwork, int numberOfInputNeurons, int numberOfOutputNeurons, Experience * firstExperienceInList, bool addSprites, bool allowRaytrace, string * XMLNNSceneFileName, char * charstarvectorGraphicsLDRNNSceneFileName, char * charstarvectorGraphicsLDRNNSceneFileNameWithSprites, char * charstarvectorGraphicsTALNNSceneFileName, char * charstarraytracedImagePPMNNSceneFileName, char * charstarexperienceNNSceneFileName, bool useFoldsDuringTraining, int maxOrSetNumEpochs);	//OLD wrapper function: this does not support full set of ANN features (SVG, opengl output etc)
	void outputNeuralNetworkToVectorGraphicsAndRaytrace(NeuronContainer * firstInputNeuronInNetwork, bool addSprites, bool allowRaytrace, bool display, bool useOutputLDRFile, bool useOutputPPMFile, bool useOutputSVGFile, string outputLDRFileNameWithoutSprites, string outputLDRFileNameWithSprites, string outputSVGFileName, string outputPPMFileName, string outputPPMFileNameRaytraced, string outputTALFileName, int width, int height);

void writeExperienceListToFile(char * fileName, Experience * firstExperienceInList);

#endif

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
 * File Name: ANNdisplay.hpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2020 Baxter AI (baxterai.com)
 * Project: Artificial Neural Network (ANN)
 * Project Version: 3n7b 17-August-2020
 * Comments: TH = Test Harness
 * /
 *******************************************************************************/


#ifndef HEADER_ANN_DISPLAY
#define HEADER_ANN_DISPLAY

#include "ANNexperienceClass.hpp"
#include "ANNneuronClass.hpp"
#include "ANNglobalDefs.hpp"
#include "ANNformation.hpp"
#include "ANNxmlConversion.hpp"
#include "ANNdraw.hpp"
//#include "ANNalgorithmBackpropagationUpdate.hpp"
#include "LDparser.hpp"
#include "LDreferenceManipulation.hpp"
#include "LDsvg.hpp"
#include "LDopengl.hpp"
#include "RTscene.hpp"
#include "RTreferenceManipulation.hpp"
#include "RTpixelMaps.hpp"
#include "RTviewinfo.hpp"


class ANNdisplayClass
{
	private: ANNexperienceClassClass ANNexperienceClass;
	private: RTpixelMapsClass RTpixelMaps;
	private: ANNxmlConversionClass ANNxmlConversion;
	private: ANNdrawClass ANNdraw;
	private: LDreferenceManipulationClass LDreferenceManipulation;
	private: LDsvgClass LDsvg;
	private: LDparserClass LDparser;
	private: LDopenglClass LDopengl;
	private: RTsceneClass RTscene;
	private: RTreferenceManipulationClass RTreferenceManipulation;
	#ifdef USE_RT
	public: void generateExperienceWith2DrgbMap(unsigned char* rgbMap, int imageWidth, int imageHeight, ANNexperience* currentExperience, int64_t objectDecision);
	public: void generateExperienceWith2Dmap(double* lumOrContrastOrDepthMap, int imageWidth, int imageHeight, double mapMaxValue, ANNexperience* currentExperience, int64_t objectDecision);
	public: void generateExperienceWith2DbooleanMap(bool* booleanMap, int imageWidth, int imageHeight, ANNexperience* currentExperience, int64_t objectDecision);
	#endif

	public: void outputNeuralNetworkToVectorGraphicsAndRaytrace(ANNneuron* firstInputNeuronInNetwork, const bool addSprites, const bool allowRaytrace, const bool displayInOpenGL, const bool useOutputLDRFile, const bool useOutputSVGFile, const bool useOutputPPMFile, const string outputLDRFileName, const string outputSVGFileName, const string outputPPMFileName, const string outputPPMFileNameRaytraced, const string outputTALFileName, const int width, const int height);
		public: void outputNeuralNetworkToVectorGraphicsAndRaytrace(ANNneuron* firstInputNeuronInNetwork, const bool addSprites, const bool allowRaytrace, const bool displayInOpenGL, const bool useOutputLDRFile, const bool useOutputSVGFile, const bool useOutputPPMFile, const string outputLDRFileName, const string outputSVGFileName, const string outputPPMFileName, const string outputPPMFileNameRaytraced, const string outputTALFileName, const int width, const int height, const int widthSVG, const int heightSVG);

	public: void writeExperienceListToFile(char* fileName, ANNexperience* firstExperienceInList);
};

#endif

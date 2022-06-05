 /*******************************************************************************
 *
 * License: GNU Affero General Public License version 3 (AGPLv3)
 *
 *******************************************************************************/

/*******************************************************************************
 *
 * File Name: ANNdisplay.hpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2022 Baxter AI (baxterai.com)
 * Project: Artificial Neural Network (ANN)
 * Project Version: 3q1a 05-June-2022
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
	public: void generateExperienceWith2DrgbMap(uchar* rgbMap, int imageWidth, int imageHeight, ANNexperience* currentExperience, int64_t objectDecision);
	public: void generateExperienceWith2Dmap(double* lumOrContrastOrDepthMap, int imageWidth, int imageHeight, double mapMaxValue, ANNexperience* currentExperience, int64_t objectDecision);
	public: void generateExperienceWith2DbooleanMap(bool* booleanMap, int imageWidth, int imageHeight, ANNexperience* currentExperience, int64_t objectDecision);
	#endif

	public: void outputNeuralNetworkToVectorGraphicsAndRaytrace(ANNneuron* firstInputNeuronInNetwork, const bool addSprites, const bool allowRaytrace, const bool displayInOpenGL, const bool useOutputLDRFile, const bool useOutputSVGFile, const bool useOutputPPMFile, const string outputLDRFileName, const string outputSVGFileName, const string outputPPMFileName, const string outputPPMFileNameRaytraced, const string outputTALFileName, const int width, const int height);
		public: void outputNeuralNetworkToVectorGraphicsAndRaytrace(ANNneuron* firstInputNeuronInNetwork, const bool addSprites, const bool allowRaytrace, const bool displayInOpenGL, const bool useOutputLDRFile, const bool useOutputSVGFile, const bool useOutputPPMFile, const string outputLDRFileName, const string outputSVGFileName, const string outputPPMFileName, const string outputPPMFileNameRaytraced, const string outputTALFileName, const int width, const int height, const int widthSVG, const int heightSVG);

	public: void writeExperienceListToFile(char* fileName, ANNexperience* firstExperienceInList);
};

#endif

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
 * File Name: ANNdisplay.h
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2017 Baxter AI (baxterai.com)
 * Project: Artificial Neural Network (ANN)
 * Project Version: 3j1c 14-January-2017
 * Comments: TH = Test Harness
 *
 *******************************************************************************/

#ifndef HEADER_ANN_DISPLAY
#define HEADER_ANN_DISPLAY

#include "ANNexperienceClass.h"
#include "ANNneuronClass.h"
#include "ANNglobalDefs.h"
#include "ANNformation.h"
#include "ANNalgorithmBackpropagationTraining.h"
#include "ANNalgorithmMemoryNetworkTraining.h"
#include "ANNalgorithmClassificationNetworkTraining.h"
#include "ANNxmlConversion.h"
#include "ANNdraw.h"
#include "ANNalgorithmBackpropagationUpdate.h"
#include "LDparser.h"
#include "LDreferenceManipulation.h"
#include "LDsvg.h"
#include "LDopengl.h"
#include "RTscene.h"
#include "RTreferenceManipulation.h"
#include "RTpixelMaps.h"
#include "RTviewinfo.h"


class ANNdisplayClass
{
	private: ANNexperienceClassClass ANNexperienceClass;
	private: RTpixelMapsClass RTpixelMaps;
	#ifdef ANN_ALGORITHM_BACKPROPAGATION
	private: ANNalgorithmBackpropagationTrainingClass ANNalgorithmBackpropagationTraining;
	#endif
	#ifdef ANN_ALGORITHM_MEMORY_NETWORK
	private: ANNalgorithmMemoryNetworkTrainingClass ANNalgorithmMemoryNetworkTraining;
	#endif
	#ifdef ANN_ALGORITHM_CLASSIFICATION_NETWORK
	private: ANNalgorithmClassificationNetworkTrainingClass ANNalgorithmClassificationNetworkTraining;
	#endif
	private: ANNxmlConversionClass ANNxmlConversion;
	private: ANNdrawClass ANNdraw;
	private: LDreferenceManipulationClass LDreferenceManipulation;
	private: LDsvgClass LDsvg;
	private: LDparserClass LDparser;
	private: LDopenglClass LDopengl;
	private: RTsceneClass RTscene;
	private: RTreferenceManipulationClass RTreferenceManipulation;
#ifdef USE_RT
	private: void generateExperienceWith2DrgbMap(unsigned char* rgbMap, int imageWidth, int imageHeight, ANNexperience* currentExperience, long objectDecision);
	private: void generateExperienceWith2Dmap(double* lumOrContrastOrDepthMap, int imageWidth, int imageHeight, double mapMaxValue, ANNexperience* currentExperience, long objectDecision);
	private: void generateExperienceWith2DbooleanMap(bool* booleanMap, int imageWidth, int imageHeight, ANNexperience* currentExperience, long objectDecision);
#endif


	private: bool trainAndOutputNeuralNetworkWithFileNames(ANNneuron* firstInputNeuronInNetwork, ANNneuron* firstOutputNeuronInNetwork, long numberOfInputNeurons, long numberOfOutputNeurons, ANNexperience* firstExperienceInList, bool addSprites, bool allowRaytrace, string* XMLNNSceneFileName, char* charstarvectorGraphicsLDRNNSceneFileName, char* charstarvectorGraphicsTALNNSceneFileName, char* charstarraytracedImagePPMNNSceneFileName, char* charstarexperienceNNSceneFileName, bool useFoldsDuringTraining, int maxOrSetNumEpochs);	//OLD wrapper function: this does not support full set of ANN features (SVG, opengl output etc)
		public: void outputNeuralNetworkToVectorGraphicsAndRaytrace(ANNneuron* firstInputNeuronInNetwork, bool addSprites, bool allowRaytrace, bool displayInOpenGL, bool useOutputLDRFile, bool useOutputPPMFile, bool useOutputSVGFile, string outputLDRFileName, string outputSVGFileName, string outputPPMFileName, string outputPPMFileNameRaytraced, string outputTALFileName, int width, int height);

	private: void writeExperienceListToFile(char* fileName, ANNexperience* firstExperienceInList);
};

#endif

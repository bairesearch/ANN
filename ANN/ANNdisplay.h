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
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2016 Baxter AI (baxterai.com)
 * Project: Artificial Neural Network (ANN)
 * Project Version: 4a3u 02-May-2016
 * Comments: TH = Test Harness
 *
 *******************************************************************************/

#ifndef HEADER_ANN_DISPLAY
#define HEADER_ANN_DISPLAY

#include "ANNexperienceClass.h"
#include "ANNneuronClass.h"
#include "ANNglobalDefs.h"


#ifdef USE_RT
void generateExperienceWith2DrgbMap(unsigned char* rgbMap, int imageWidth, int imageHeight, ANNexperience* currentExperience, long objectDecision);
void generateExperienceWith2Dmap(double* lumOrContrastOrDepthMap, int imageWidth, int imageHeight, double mapMaxValue, ANNexperience* currentExperience, long objectDecision);
void generateExperienceWith2DbooleanMap(bool* booleanMap, int imageWidth, int imageHeight, ANNexperience* currentExperience, long objectDecision);
#endif


bool trainAndOutputNeuralNetworkWithFileNames(ANNneuron* firstInputNeuronInNetwork, ANNneuron* firstOutputNeuronInNetwork, long numberOfInputNeurons, long numberOfOutputNeurons, ANNexperience* firstExperienceInList, bool addSprites, bool allowRaytrace, string* XMLNNSceneFileName, char* charstarvectorGraphicsLDRNNSceneFileName, char* charstarvectorGraphicsLDRNNSceneFileNameWithSprites, char* charstarvectorGraphicsTALNNSceneFileName, char* charstarraytracedImagePPMNNSceneFileName, char* charstarexperienceNNSceneFileName, bool useFoldsDuringTraining, int maxOrSetNumEpochs);	//OLD wrapper function: this does not support full set of ANN features (SVG, opengl output etc)
	void outputNeuralNetworkToVectorGraphicsAndRaytrace(ANNneuron* firstInputNeuronInNetwork, bool addSprites, bool allowRaytrace, bool display, bool useOutputLDRFile, bool useOutputPPMFile, bool useOutputSVGFile, string outputLDRFileNameWithoutSprites, string outputLDRFileNameWithSprites, string outputSVGFileName, string outputPPMFileName, string outputPPMFileNameRaytraced, string outputTALFileName, int width, int height);

void writeExperienceListToFile(char* fileName, ANNexperience* firstExperienceInList);

#endif

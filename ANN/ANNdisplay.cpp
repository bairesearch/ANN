/*******************************************************************************
 * 
 * This file is part of BAIPROJECT.
 * 
 * BAIPROJECT is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License version 3
 * only, as published by the Free Software Foundation.
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
 * File Name: ANNdisplay.cpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2012 Baxter AI (baxterai.com)
 * Project: Artificial Neural Network (ANN)
 * Project Version: 3c3g 18-November-2012
 * Comments: TH = Test Harness
 *
 *******************************************************************************/

#include "ANNdisplay.h"
#include "ANNformation.h"
#include "ANNtraining.h"
#include "ANNxmlConversion.h"
#include "ANNsprite.h"
#include "ANNupdateAlgorithm.h"
#include "LDparser.h"
#include "LDreferenceManipulation.h"
#include "LDsvg.h"
#include "LDopengl.h"


#ifdef USE_RT
#include "RTscene.h"
#include "RTreferenceManipulation.h"
#include "RTpixelMaps.h"
#include "RTviewinfo.h"
#endif


#ifdef USE_RT

void generateExperienceWith2DrgbMap(unsigned char * rgbMap, int imageWidth, int imageHeight, Experience * currentExperience, long objectDecision)
{
	currentExperience->classTargetValue = objectDecision;
	ExperienceInput * currentExperienceInput = currentExperience->firstExperienceInput;

	//fill experience inputs
	for(int y=0; y<imageHeight; y++)
	{
		for(int x=0; x<imageWidth; x++)
		{
			for(int rgb=0; rgb<RGB_NUM; rgb++)
			{
				unsigned char col = getRGBMapValue(x, y, imageWidth, rgb, rgbMap);
				double normalisedMapValue =  normaliseExperienceInput((double)col, MAX_RGB_VAL);
				currentExperienceInput->inputValue = normalisedMapValue;
				ExperienceInput * newExperienceInput = new ExperienceInput();
				currentExperienceInput->next = newExperienceInput;
				currentExperienceInput = currentExperienceInput->next;
			}
		}
	}
}


void generateExperienceWith2Dmap(double * lumOrContrastOrDepthMap, int imageWidth, int imageHeight, double mapMaxValue, Experience * currentExperience, long objectDecision)
{
	currentExperience->classTargetValue = objectDecision;
	ExperienceInput * currentExperienceInput = currentExperience->firstExperienceInput;

	//fill experience inputs
	for(int y=0; y<imageHeight; y++)
	{
		for(int x=0; x<imageWidth; x++)
		{
			double mapValue = getLumOrContrastOrDepthMapValue(x, y, imageWidth,  lumOrContrastOrDepthMap);

			double normalisedMapValue =  normaliseExperienceInput(mapValue, mapMaxValue);
			currentExperienceInput->inputValue = normalisedMapValue;
			ExperienceInput * newExperienceInput = new ExperienceInput();
			currentExperienceInput->next = newExperienceInput;
			currentExperienceInput = currentExperienceInput->next;
		}
	}
}

void generateExperienceWith2DbooleanMap(bool * booleanMap, int imageWidth, int imageHeight, Experience * currentExperience, long objectDecision)
{
	currentExperience->classTargetValue = objectDecision;
	ExperienceInput * currentExperienceInput = currentExperience->firstExperienceInput;

	//fill experience inputs
	for(int y=0; y<imageHeight; y++)
	{
		for(int x=0; x<imageWidth; x++)
		{
			bool mapValue = getBooleanMapValue(x, y, imageWidth,  booleanMap);

			double normalisedMapValue =  normaliseExperienceInput((double)mapValue, 1.0);
			currentExperienceInput->inputValue = normalisedMapValue;
			ExperienceInput * newExperienceInput = new ExperienceInput();
			currentExperienceInput->next = newExperienceInput;
			currentExperienceInput = currentExperienceInput->next;
		}
	}
}

#endif





bool trainAndOutputNeuralNetworkWithFileNames(NeuronContainer * firstInputNeuronInNetwork, NeuronContainer * firstOutputNeuronInNetwork, int numberOfInputNeurons, int numberOfOutputNeurons, Experience * firstExperienceInList, bool addSprites, bool allowRaytrace, string * XMLNNSceneFileName, char * charstarvectorGraphicsLDRNNSceneFileName, char * charstarvectorGraphicsLDRNNSceneFileNameWithSprites, char * charstarvectorGraphicsTALNNSceneFileName, char * charstarraytracedImagePPMNNSceneFileName, char * charstarexperienceNNSceneFileName, bool useFoldsDuringTraining, int maxOrSetNumEpochs)
{
	bool result = true;

	cout << "num experiences in list = " << countNumberOfExperiences(firstExperienceInList) << endl;

	long numberOfExperiences = countNumberOfExperiences(firstExperienceInList);

	if(useFoldsDuringTraining)
	{
		int maxFolds = MAX_NUM_FOLDS_ANNTH;
		int maxNumEpochs = maxOrSetNumEpochs;
		trainNeuralNetwork(firstInputNeuronInNetwork, firstOutputNeuronInNetwork, numberOfInputNeurons, numberOfOutputNeurons, maxFolds, firstExperienceInList, numberOfExperiences, maxNumEpochs);
			//this is done dynamically if do not have heaps of RAM
	}
	else
	{
		int setNumEpochs = maxOrSetNumEpochs; //ANN_DEFAULT_SIMPLE_TRAIN_DEFAULT_NUM_OF_TRAINING_EPOCHS;
		trainNeuralNetworkSimple(firstInputNeuronInNetwork, firstOutputNeuronInNetwork, numberOfInputNeurons, numberOfOutputNeurons, setNumEpochs, firstExperienceInList, numberOfExperiences);
	}


#ifndef USE_OR
	if(!writeNetXMLfile(*XMLNNSceneFileName, firstInputNeuronInNetwork))
	{
		result = false;
	}

	string vectorGraphicsLDRNNSceneFileName = charstarvectorGraphicsLDRNNSceneFileName;
	string vectorGraphicsLDRNNSceneFileNameWithSprites = charstarvectorGraphicsLDRNNSceneFileNameWithSprites;
	string vectorGraphicsTALNNSceneFileName = charstarvectorGraphicsTALNNSceneFileName;
	string raytracedImagePPMNNSceneFileName = charstarraytracedImagePPMNNSceneFileName;
	string outputSVGFileName = "";
	string outputPPMFileName = "";
	outputNeuralNetworkToVectorGraphicsAndRaytrace(firstInputNeuronInNetwork, addSprites, allowRaytrace, false, true, true, false, vectorGraphicsLDRNNSceneFileName, vectorGraphicsLDRNNSceneFileNameWithSprites, NULL, NULL, raytracedImagePPMNNSceneFileName, vectorGraphicsTALNNSceneFileName, NULL, NULL);


	writeExperienceListToFile(charstarexperienceNNSceneFileName, firstExperienceInList);

#endif

	return result;
}



void outputNeuralNetworkToVectorGraphicsAndRaytrace(NeuronContainer * firstInputNeuronInNetwork, bool addSprites, bool allowRaytrace, bool display, bool useOutputLDRFile, bool useOutputPPMFile, bool useOutputSVGFile, string outputLDRFileNameWithoutSprites, string outputLDRFileNameWithSprites, string outputSVGFileName, string outputPPMFileName, string outputPPMFileNameRaytraced, string outputTALFileName, int width, int height)
{
	bool result = true;

	if(display)
	{
		initiateOpenGL(width, height, 0, 0, false);
	}

	char * outputFileNameLDRwithoutSpritescharstar = const_cast<char*>(outputLDRFileNameWithoutSprites.c_str());
	char * outputFileNameLDRwithSpritescharstar = const_cast<char*>(outputLDRFileNameWithSprites.c_str());
	char * outputFileNameSVGcharstar = const_cast<char*>(outputSVGFileName.c_str());
	char * displayFileNamePPMcharstar = const_cast<char*>(outputPPMFileName.c_str());
	char * outputFileNamePPMrayTracedcharstar = const_cast<char*>(outputPPMFileNameRaytraced.c_str());
	char * outputFileNameTALcharstar = const_cast<char*>(outputTALFileName.c_str());

	//now output the network to vector graphics file
	if(useOutputLDRFile || display || allowRaytrace)
	{
		//now output the vector graphics file to image file via ray tracer

		XMLparserTag * firstTagInSVGFile = new XMLparserTag();
		XMLparserTag * currentTagInSVGFile = firstTagInSVGFile;
		
		//ANNcreateNeuralNetworkSceneFilesWithAndWithoutSprites(outputFileNameLDRwithoutSpritescharstar, outputFileNameLDRwithSpritescharstar, firstInputNeuronInNetwork, addSprites, writeSVG, &currentTagInSVGFile);

		Reference * nonSpriteListInitialReference = new Reference();
		Reference * spriteListInitialReference = new Reference();

		int numSpritesAdded = 0;

		if(!ANNcreateNeuralNetworkReferenceListsWithAndWithoutSprites(outputFileNameLDRwithSpritescharstar, nonSpriteListInitialReference, spriteListInitialReference, firstInputNeuronInNetwork, addSprites, &numSpritesAdded, useOutputSVGFile, &currentTagInSVGFile))
		{
			result = false;
		}

		if(useOutputLDRFile)
		{
			if(!ANNcreateNeuralNetworkSceneFilesWithAndWithoutSpritesFromReferenceLists(outputFileNameLDRwithoutSpritescharstar, outputFileNameLDRwithSpritescharstar, addSprites, nonSpriteListInitialReference, spriteListInitialReference, numSpritesAdded))
			{
				result = false;
			}
		}

		if(useOutputSVGFile)
		{
			if(!writeSVGfile(outputFileNameSVGcharstar, firstTagInSVGFile))
			{
				result = false;
			}
			delete firstTagInSVGFile;
		}

		char * charstarsceneFileNameForRayTracing;

		if(addSprites)
		{
			charstarsceneFileNameForRayTracing = outputFileNameLDRwithSpritescharstar;
		}
		else
		{
			charstarsceneFileNameForRayTracing = outputFileNameLDRwithoutSpritescharstar;
		}

		if(display || allowRaytrace)
		{
			// OLD; if using RT, do not ray trace sprites as the RT raytracer is not optimised - use povray instead
			// NEW; use ANNrules.xml to remove sprites for RT speed

			//reparse scenefilewithandwithout sprites - to build absolute position information
			Reference * initialReferenceInSceneFileForRayTracing = new Reference();
			Reference * topLevelReferenceInSceneFileForRayTracing = new Reference(charstarsceneFileNameForRayTracing, 1, true);	//The information in this object is not required or meaningful, but needs to be passed into the parseFile/parseReferenceList recursive function
			if(!parseFile(charstarsceneFileNameForRayTracing, initialReferenceInSceneFileForRayTracing, topLevelReferenceInSceneFileForRayTracing, true))
			{//file does not exist
				cout << "The file: " << charstarsceneFileNameForRayTracing << " does not exist in the directory" << endl;
				result = false;
			}

			if(allowRaytrace)
			{


				#ifdef TH_USE_RT_FOR_NEURAL_NETWORK_VEC_GRAPHICS

					ViewInfo vi;
					vi.imageWidth = width;
					vi.imageHeight = height;
					vi.eye.x = TAL_FILE_HEADER_DEFAULT_EYE_X;
					vi.eye.y = TAL_FILE_HEADER_DEFAULT_EYE_Y;
					vi.eye.z = TAL_FILE_HEADER_DEFAULT_EYE_Z;
					vi.viewAt.x = TAL_FILE_HEADER_DEFAULT_VIEWAT_X;
					vi.viewAt.y = TAL_FILE_HEADER_DEFAULT_VIEWAT_Y;
					vi.viewAt.z = TAL_FILE_HEADER_DEFAULT_VIEWAT_Z;
					vi.viewUp.x = TAL_FILE_HEADER_DEFAULT_VIEWUP_X;
					vi.viewUp.y = TAL_FILE_HEADER_DEFAULT_VIEWUP_Y;
					vi.viewUp.z = TAL_FILE_HEADER_DEFAULT_VIEWUP_Z;
					vi.viewWidth = TAL_FILE_HEADER_DEFAULT_VIEWSIZE_WIDTH;
					vi.viewHeight = TAL_FILE_HEADER_DEFAULT_VIEWSIZE_HEIGHT;

					vi.focalLength = TAL_FILE_HEADER_DEFAULT_FOCAL;

					if(!write2DReferenceListCollapsedTo1DToFileRayTraceFormat(outputFileNameTALcharstar, initialReferenceInSceneFileForRayTracing, true, &vi, false, NULL, NULL))
					{
						result = false;
					}

					if(!rayTraceScene(outputFileNameTALcharstar, outputFileNamePPMrayTracedcharstar, true, false, NULL, NULL, NULL, NULL))
					{
						result = false;
					}
				#else
					#ifdef TH_USE_POVRAY_FOR_NEURAL_NETWORK_VEC_GRAPHICS	//NOT YET TEST;


						Reference * nonSpriteListInitialReference = new Reference();
						Reference * spriteListInitialReference = new Reference();

						int numSpritesAdded = 0;
						if(!ANNcreateNeuralNetworkReferenceListsWithAndWithoutSprites(outputFileNameLDRwithSpritescharstar, nonSpriteListInitialReference, spriteListInitialReference, firstInputNeuronInNetwork, addSprites, &numSpritesAdded, useOutputSVGFile, &currentTagInSVGFile))
						{
							result = false;
						}

						if(!ANNcreateNeuralNetworkSceneFilesWithAndWithoutSpritesFromReferenceLists(outputFileNameLDRwithoutSpritescharstar, outputFileNameLDRwithSpritescharstar, addSprites, nonSpriteListInitialReference, spriteListInitialReference, numSpritesAdded))
						{
							result = false;
						}

						char * charstarsceneFileNameForRayTracing;
						if(addSprites)
						{
							if(!joinReferenceLists(nonSpriteListInitialReference, spriteListInitialReference))
							{
								result = false;
							}

							charstarsceneFileNameForRayTracing = outputFileNameLDRwithSpritescharstar;
						}
						else
						{
							charstarsceneFileNameForRayTracing = outputFileNameLDRwithoutSpritescharstar;
						}


						//use third party raytracer, povray;

						string temp = "";
						string l3pCommand = temp + OBJECT_EXPERIENCE_GEN_L3P_EXCCMD_PART_1 + OBJECT_EXPERIENCE_GEN_L3P_EXCCMD_PART_2 + charstarsceneFileNameForRayTracing + OBJECT_EXPERIENCE_GEN_L3P_EXCCMD_PART_5 + OBJECT_EXPERIENCE_GEN_L3P_EXCCMD_PART_6 + OBJECT_EXPERIENCE_GEN_L3P_EXCCMD_PART_7;
						string povrayCommand = temp + OBJECT_EXPERIENCE_GEN_POVRAY_EXCCMD_PART_1 + OBJECT_EXPERIENCE_GEN_POVRAY_EXCCMD_PART_2 + OBJECT_EXPERIENCE_GEN_POVRAY_EXCCMD_PART_3 + OBJECT_EXPERIENCE_GEN_POVRAY_EXCCMD_PART_4 + OBJECT_EXPERIENCE_GEN_POVRAY_EXCCMD_PART_5 + OBJECT_EXPERIENCE_GEN_POVRAY_EXCCMD_PART_6 + outputFileNamePPMrayTracedcharstar + OBJECT_EXPERIENCE_GEN_L3P_EXCCMD_PART_9 + OBJECT_EXPERIENCE_GEN_POVRAY_EXCCMD_PART_10 + OBJECT_EXPERIENCE_GEN_POVRAY_EXCCMD_PART_11 + OBJECT_EXPERIENCE_GEN_POVRAY_EXCCMD_PART_12 + OBJECT_EXPERIENCE_GEN_POVRAY_EXCCMD_PART_13 + OBJECT_EXPERIENCE_GEN_POVRAY_EXCCMD_PART_14;

						//Eg system("env LDRAWDIR=/usr/share/local/LDRAW l3p /usr/share/local/LDRAW/MODELS/car.dat object -cg30,45 -b -f");
						//Eg system("povray object.pov");
						cout << l3pCommand <<endl;
						system(l3pCommand.c_str());
						cout << povrayCommand <<endl;
						system(povrayCommand.c_str());


						delete nonSpriteListInitialReference;
						delete spriteListInitialReference;

					#else
						cout << "Error: no ray tracer enabled" << endl;
						exit(0);
					#endif
				#endif
			}

			if(display)
			{
				char * topLevelSceneFileNameCollapsed = "sceneCollapsedForOpenGLDisplay.ldr";
				write2DreferenceListCollapsedTo1DtoFile(topLevelSceneFileNameCollapsed, initialReferenceInSceneFileForRayTracing);

				unsigned char * rgbMap = new unsigned char[width*height*RGB_NUM];

				//setViewPort2Dortho(-100.0, 2000.0, -100.0, 2000.0);
				setViewPort3Dortho(0.0, 5.0, 5.0, 0.0, 2.0, -2.0);

				//now reparse file
				Reference * initialReferenceInCollapsedSceneFile = new Reference();
				Reference * topLevelReferenceInCollapsedSceneFile = new Reference(topLevelSceneFileNameCollapsed, 1, true);	//The information in this object is not required or meaningful, but needs to be passed into the parseFile/parseReferenceList recursive function
				if(!parseFile(topLevelSceneFileNameCollapsed, initialReferenceInCollapsedSceneFile, topLevelReferenceInCollapsedSceneFile, true))
				{//file does not exist
					cout << "The file: " << topLevelSceneFileNameCollapsed << " does not exist in the directory" << endl;
					exit(0);
				}

				drawPrimitivesReferenceListToOpenGLandCreateRGBmapBasic(initialReferenceInCollapsedSceneFile, width, height, rgbMap);
				drawPrimitivesReferenceListToOpenGLandCreateRGBmapBasic(initialReferenceInCollapsedSceneFile, width, height, rgbMap);
					//due to opengl code bug, need to execute this function twice.

				#ifdef TH_USE_RT_FOR_NEURAL_NETWORK_VEC_GRAPHICS
				if(useOutputPPMFile)
				{
					generatePixmapFromRGBmap(displayFileNamePPMcharstar, width, height, rgbMap);
				}
				#endif
			}
		}
	}
}

void writeExperienceListToFile(char * fileName, Experience * firstExperienceInList)
{
	ofstream experienceDataSetOfStreamObject(fileName);

	Experience * currentExperience = firstExperienceInList;
	while(currentExperience->next != NULL)
	{
		addExperienceToOFStream(&experienceDataSetOfStreamObject, currentExperience);
		currentExperience = currentExperience->next;
	}

	experienceDataSetOfStreamObject.close();
}



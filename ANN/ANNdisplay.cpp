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
 * File Name: ANNdisplay.cpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2022 Baxter AI (baxterai.com)
 * Project: Artificial Neural Network (ANN)
 * Project Version: 3p5a 14-March-2022
 * Comments: TH = Test Harness
 * /
 *******************************************************************************/


#include "ANNdisplay.hpp"
#ifdef ANN_ALGORITHM_MEMORY_NETWORK
#endif
#ifdef ANN_ALGORITHM_CLASSIFICATION_NETWORK
#endif


#ifdef USE_RT
#endif


#ifdef USE_RT

void ANNdisplayClass::generateExperienceWith2DrgbMap(uchar* rgbMap, int imageWidth, int imageHeight, ANNexperience* currentExperience, int64_t objectDecision)
{
	currentExperience->classTargetValue = objectDecision;
	ANNexperienceInput* currentExperienceInput = currentExperience->firstExperienceInput;

	//fill experience inputs
	for(int y=0; y<imageHeight; y++)
	{
		for(int x=0; x<imageWidth; x++)
		{
			for(int rgb=0; rgb<RGB_NUM; rgb++)
			{
				uchar col = RTpixelMaps.getRGBMapValue(x, y, imageWidth, rgb, rgbMap);
				double normalisedMapValue =  ANNexperienceClass.normaliseExperienceInput((double)col, MAX_RGB_VAL);
				currentExperienceInput->inputValue = normalisedMapValue;
				ANNexperienceInput* newExperienceInput = new ANNexperienceInput();
				currentExperienceInput->next = newExperienceInput;
				currentExperienceInput = currentExperienceInput->next;
			}
		}
	}
}


void ANNdisplayClass::generateExperienceWith2Dmap(double* lumOrContrastOrDepthMap, int imageWidth, int imageHeight, double mapMaxValue, ANNexperience* currentExperience, int64_t objectDecision)
{
	currentExperience->classTargetValue = objectDecision;
	ANNexperienceInput* currentExperienceInput = currentExperience->firstExperienceInput;

	//fill experience inputs
	for(int y=0; y<imageHeight; y++)
	{
		for(int x=0; x<imageWidth; x++)
		{
			double mapValue = RTpixelMaps.getLumOrContrastOrDepthMapValue(x, y, imageWidth,  lumOrContrastOrDepthMap);

			double normalisedMapValue =  ANNexperienceClass.normaliseExperienceInput(mapValue, mapMaxValue);
			currentExperienceInput->inputValue = normalisedMapValue;
			ANNexperienceInput* newExperienceInput = new ANNexperienceInput();
			currentExperienceInput->next = newExperienceInput;
			currentExperienceInput = currentExperienceInput->next;
		}
	}
}

void ANNdisplayClass::generateExperienceWith2DbooleanMap(bool* booleanMap, int imageWidth, int imageHeight, ANNexperience* currentExperience, int64_t objectDecision)
{
	currentExperience->classTargetValue = objectDecision;
	ANNexperienceInput* currentExperienceInput = currentExperience->firstExperienceInput;

	//fill experience inputs
	for(int y=0; y<imageHeight; y++)
	{
		for(int x=0; x<imageWidth; x++)
		{
			bool mapValue = RTpixelMaps.getBooleanMapValue(x, y, imageWidth,  booleanMap);

			double normalisedMapValue =  ANNexperienceClass.normaliseExperienceInput((double)mapValue, 1.0);
			currentExperienceInput->inputValue = normalisedMapValue;
			ANNexperienceInput* newExperienceInput = new ANNexperienceInput();
			currentExperienceInput->next = newExperienceInput;
			currentExperienceInput = currentExperienceInput->next;
		}
	}
}

#endif






void ANNdisplayClass::outputNeuralNetworkToVectorGraphicsAndRaytrace(ANNneuron* firstInputNeuronInNetwork, const bool addSprites, const bool allowRaytrace, const bool displayInOpenGL, const bool useOutputLDRFile, const bool useOutputSVGFile, const bool useOutputPPMFile, const string outputLDRFileName, const string outputSVGFileName, const string outputPPMFileName, const string outputPPMFileNameRaytraced, const string outputTALFileName, const int width, const int height)
{
	int widthSVG = ANN_DISPLAY_DEFAULT_SVG_WIDTH;
	int heightSVG = ANN_DISPLAY_DEFAULT_SVG_HEIGHT;
	outputNeuralNetworkToVectorGraphicsAndRaytrace(firstInputNeuronInNetwork, addSprites, allowRaytrace, displayInOpenGL, useOutputLDRFile, useOutputSVGFile, useOutputPPMFile, outputLDRFileName, outputSVGFileName, outputPPMFileName, outputPPMFileNameRaytraced, outputTALFileName, width, height, widthSVG, heightSVG);
}
void ANNdisplayClass::outputNeuralNetworkToVectorGraphicsAndRaytrace(ANNneuron* firstInputNeuronInNetwork, const bool addSprites, const bool allowRaytrace, const bool displayInOpenGL, const bool useOutputLDRFile, const bool useOutputSVGFile, const bool useOutputPPMFile, const string outputLDRFileName, const string outputSVGFileName, const string outputPPMFileName, const string outputPPMFileNameRaytraced, const string outputTALFileName, const int width, const int height, const int widthSVG, const int heightSVG)
{
	bool result = true;

	char* outputFileNameLDRcharstar = const_cast<char*>(outputLDRFileName.c_str());
	char* outputFileNameSVGcharstar = const_cast<char*>(outputSVGFileName.c_str());
	char* displayFileNamePPMcharstar = const_cast<char*>(outputPPMFileName.c_str());
	char* outputFileNamePPMrayTracedcharstar = const_cast<char*>(outputPPMFileNameRaytraced.c_str());
	char* outputFileNameTALcharstar = const_cast<char*>(outputTALFileName.c_str());

	//now output the network to vector graphics file
	if(useOutputLDRFile || useOutputSVGFile || displayInOpenGL || allowRaytrace)
	{
		//now output the vector graphics file to image file via ray tracer

		#ifndef ANN_DRAW_DISABLE_FILE_OUTPUT_NOTIFICATIONS
		cout << "referenceFileName = " << outputLDRFileName << endl;
		#endif

		XMLparserTag* firstTagInSVGFile = new XMLparserTag();
		XMLparserTag* currentTagInSVGFile = firstTagInSVGFile;

		LDreference* initialReference = new LDreference();

		int numSpritesAdded = 0;

		//cout << "outputNeuralNetworkToVectorGraphicsAndRaytrace1" << endl;
		
		if(!ANNdraw.ANNcreateNeuralNetworkReferenceLists(outputFileNameLDRcharstar, initialReference, firstInputNeuronInNetwork, addSprites, &numSpritesAdded, useOutputSVGFile, &currentTagInSVGFile, useOutputLDRFile))
		{
			result = false;
		}

		//cout << "outputNeuralNetworkToVectorGraphicsAndRaytrace2" << endl;
		
		if(useOutputLDRFile)
		{
			#ifndef ANN_DRAW_DISABLE_FILE_OUTPUT_NOTIFICATIONS
			cout << "LDR graphics file name = " << outputLDRFileName << endl;
			#endif
			if(!ANNdraw.ANNcreateNeuralNetworkSceneFilesFromReferenceLists(outputFileNameLDRcharstar, addSprites, initialReference, numSpritesAdded))
			{
				result = false;
			}
		}

		//cout << "outputNeuralNetworkToVectorGraphicsAndRaytrace3" << endl;
		
		if(useOutputSVGFile)
		{
			#ifndef ANN_DRAW_DISABLE_FILE_OUTPUT_NOTIFICATIONS
			cout << "SVG graphics file name = " << outputSVGFileName << endl;
			#endif
			int viewBoxMinX = -ANN_DISPLAY_DEFAULT_SVG_WIDTH_VIEWBOX_OFFSET;
			int viewBoxMaxX = widthSVG-ANN_DISPLAY_DEFAULT_SVG_WIDTH_VIEWBOX_OFFSET;
			int viewBoxMinY = -ANN_DISPLAY_DEFAULT_SVG_WIDTH_VIEWBOX_OFFSET;
			int viewBoxMaxY = heightSVG-ANN_DISPLAY_DEFAULT_SVG_WIDTH_VIEWBOX_OFFSET;
			
			if(!LDsvg.writeSVGfile(outputFileNameSVGcharstar, firstTagInSVGFile, viewBoxMinX, viewBoxMaxX, viewBoxMinY, viewBoxMaxY))
			{
				result = false;
			}
			delete firstTagInSVGFile;
		}

		//cout << "outputNeuralNetworkToVectorGraphicsAndRaytrace4" << endl;
		
		char* charstarsceneFileNameForRayTracing;
		charstarsceneFileNameForRayTracing = outputFileNameLDRcharstar;

		if(displayInOpenGL || allowRaytrace)
		{
			// OLD; if using RT, do not ray trace sprites as the RT raytracer is not optimised - use povray instead
			// NEW; use ANNrules.xml to remove sprites for RT speed

			//reparse scenefilewithandwithout sprites - to build absolute position information
			LDreference* initialReferenceInSceneFileForRayTracing = new LDreference();
			LDreference* topLevelReferenceInSceneFileForRayTracing = new LDreference(charstarsceneFileNameForRayTracing, 1, true);	//The information in this object is not required or meaningful, but needs to be passed into the parseFile/parseReferenceList recursive function
			if(!LDparser.parseFile(charstarsceneFileNameForRayTracing, initialReferenceInSceneFileForRayTracing, topLevelReferenceInSceneFileForRayTracing, true))
			{//file does not exist
				cout << "The file: " << charstarsceneFileNameForRayTracing << " does not exist in the directory" << endl;
				result = false;
			}

			if(allowRaytrace)
			{
				#ifdef TH_USE_RT_FOR_NEURAL_NETWORK_VEC_GRAPHICS

					RTviewInfo vi;
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

					if(!RTreferenceManipulation.write2DReferenceListCollapsedTo1DToFileRayTraceFormat(outputFileNameTALcharstar, initialReferenceInSceneFileForRayTracing, true, &vi, false, NULL, NULL))
					{
						result = false;
					}

					if(!RTscene.rayTraceScene(outputFileNameTALcharstar, outputFileNamePPMrayTracedcharstar, true, false, NULL, NULL, NULL, NULL))
					{
						result = false;
					}
				#else
					#ifdef TH_USE_POVRAY_FOR_NEURAL_NETWORK_VEC_GRAPHICS	//NOT YET TESTED [CHECKTHIS];

						/*
						LDreference* initialReference = new LDreference();

						int numSpritesAdded = 0;
						if(!ANNdraw.ANNcreateNeuralNetworkReferenceLists(outputFileNameLDRcharstar, initialReference, firstInputNeuronInNetwork, addSprites, &numSpritesAdded, useOutputSVGFile, &currentTagInSVGFile, useOutputLDRFile))
						{
							result = false;
						}

						if(!ANNdraw.ANNcreateNeuralNetworkSceneFilesFromReferenceLists(outputFileNameLDRcharstar, addSprites, initialReference, numSpritesAdded))
						{
							result = false;
						}
						*/

						char* charstarsceneFileNameForRayTracing = outputFileNameLDRcharstar;

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

					#else
						cerr << "Error: no ray tracer enabled" << endl;
						exit(EXIT_ERROR);
					#endif
				#endif
			}

			if(displayInOpenGL)
			{
				char* topLevelSceneFileNameCollapsed = "sceneCollapsedForOpenGLDisplay.ldr";
				LDreferenceManipulation.write2DreferenceListCollapsedTo1DtoFile(topLevelSceneFileNameCollapsed, initialReferenceInSceneFileForRayTracing);

				uchar* rgbMap = new uchar[width*height*RGB_NUM];

				//setViewPort2Dortho(-100.0, 2000.0, -100.0, 2000.0);
				LDopengl.setViewPort3Dortho(0.0, 5.0, 5.0, 0.0, 2.0, -2.0);

				//now reparse file
				LDreference* initialReferenceInCollapsedSceneFile = new LDreference();
				LDreference* topLevelReferenceInCollapsedSceneFile = new LDreference(topLevelSceneFileNameCollapsed, 1, true);	//The information in this object is not required or meaningful, but needs to be passed into the parseFile/parseReferenceList recursive function
				if(!LDparser.parseFile(topLevelSceneFileNameCollapsed, initialReferenceInCollapsedSceneFile, topLevelReferenceInCollapsedSceneFile, true))
				{//file does not exist
					cerr << "The file: " << topLevelSceneFileNameCollapsed << " does not exist in the directory" << endl;
					exit(EXIT_ERROR);
				}

				LDopengl.drawPrimitivesReferenceListToOpenGLandCreateRGBmapBasic(initialReferenceInCollapsedSceneFile, width, height, rgbMap);
				LDopengl.drawPrimitivesReferenceListToOpenGLandCreateRGBmapBasic(initialReferenceInCollapsedSceneFile, width, height, rgbMap);
					//due to opengl code bug, need to execute this function twice.

				#ifdef TH_USE_RT_FOR_NEURAL_NETWORK_VEC_GRAPHICS
				if(useOutputPPMFile)
				{
					#ifndef ANN_DRAW_DISABLE_FILE_OUTPUT_NOTIFICATIONS
					cout << "PPM graphics file name = " << outputPPMFileName << endl;
					#endif
					RTpixelMaps.generatePixmapFromRGBmap(displayFileNamePPMcharstar, width, height, rgbMap);
				}
				#endif
			}
		}
	}

	if(displayInOpenGL)
	{
		LDopengl.exitOpenGL();
	}
}

void ANNdisplayClass::writeExperienceListToFile(char* fileName, ANNexperience* firstExperienceInList)
{
	ofstream experienceDataSetOfStreamObject(fileName);

	ANNexperience* currentExperience = firstExperienceInList;
	while(currentExperience->next != NULL)
	{
		ANNexperienceClass.addExperienceToOFStream(&experienceDataSetOfStreamObject, currentExperience);
		currentExperience = currentExperience->next;
	}

	experienceDataSetOfStreamObject.close();
}



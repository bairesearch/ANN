/*******************************************************************************
 *
 * File Name: ANNdisplay.cpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2012 Baxter AI (baxterai.com)
 * Project: Advanced Neural Network (ANN)
 * Project Version: 3a8b 14-June-2012
 * Comments: TH = Test Harness
 *
 *******************************************************************************/

#include "ANNdisplay.h"
#include "ANNFormation.h"
#include "ANNTraining.h"
#include "ANNXMLconversion.h"
#include "ANNsprite.h"
#include "ANNUpdateAlgorithm.h"
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

void generateExperienceWith2DRGBMap(unsigned char * rgbMap, int imageWidth, int imageHeight, Experience * currentExperience, long objectDecision)
{
	currentExperience->classTargetValue = objectDecision;
	ExperienceInput * currentExperienceInput = currentExperience->firstExperienceInput;

	//fill experience inputs
	for(int y=0; y<imageHeight; y++)
	{
		for(int x=0; x<imageWidth; x++)
		{
			//cout << "x = " << x << endl;
			//cout << "y = " << y << endl;

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


void generateExperienceWith2DMap(double * lumOrContrastOrDepthMap, int imageWidth, int imageHeight, double mapMaxValue, Experience * currentExperience, long objectDecision)
{
	currentExperience->classTargetValue = objectDecision;
	ExperienceInput * currentExperienceInput = currentExperience->firstExperienceInput;

	//fill experience inputs
	for(int y=0; y<imageHeight; y++)
	{
		for(int x=0; x<imageWidth; x++)
		{
			//cout << "x = " << x << endl;
			//cout << "y = " << y << endl;

			double mapValue = getLumOrContrastOrDepthMapValue(x, y, imageWidth,  lumOrContrastOrDepthMap);

			double normalisedMapValue =  normaliseExperienceInput(mapValue, mapMaxValue);
			currentExperienceInput->inputValue = normalisedMapValue;
			ExperienceInput * newExperienceInput = new ExperienceInput();
			currentExperienceInput->next = newExperienceInput;
			currentExperienceInput = currentExperienceInput->next;
		}
	}
}

void generateExperienceWith2DBooleanMap(bool * booleanMap, int imageWidth, int imageHeight, Experience * currentExperience, long objectDecision)
{
	currentExperience->classTargetValue = objectDecision;
	ExperienceInput * currentExperienceInput = currentExperience->firstExperienceInput;

	//fill experience inputs
	for(int y=0; y<imageHeight; y++)
	{
		for(int x=0; x<imageWidth; x++)
		{
			//cout << "x = " << x << endl;
			//cout << "y = " << y << endl;

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
	if(!writeNetXMLFile(*XMLNNSceneFileName, firstInputNeuronInNetwork))
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


	//cout << "H6" << endl;


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

		ofstream * writeFileObject;
		if(useOutputSVGFile)
		{
			writeFileObject = new ofstream(outputFileNameSVGcharstar);
			writeSVGHeader(writeFileObject);
		}
		
		//ANNcreateNeuralNetworkSceneFilesWithAndWithoutSprites(outputFileNameLDRwithoutSpritescharstar, outputFileNameLDRwithSpritescharstar, firstInputNeuronInNetwork, addSprites, writeSVG, writeFileObject);

		Reference * nonspriteListInitialReference = new Reference();
		Reference * spriteListInitialReference = new Reference();

		int numSpritesAdded = 0;

		if(!ANNcreateNeuralNetworkReferenceListsWithAndWithoutSprites(outputFileNameLDRwithSpritescharstar, nonspriteListInitialReference, spriteListInitialReference, firstInputNeuronInNetwork, addSprites, &numSpritesAdded, useOutputSVGFile, writeFileObject))
		{
			result = false;
		}
	
		if(useOutputLDRFile)
		{
			if(!ANNcreateNeuralNetworkSceneFilesWithAndWithoutSpritesFromReferenceLists(outputFileNameLDRwithoutSpritescharstar, outputFileNameLDRwithSpritescharstar, addSprites, nonspriteListInitialReference, spriteListInitialReference, numSpritesAdded))
			{
				result = false;
			}		
		}
		
		if(useOutputSVGFile)
		{
			writeSVGFooter(writeFileObject);
			writeFileObject->close();
			delete writeFileObject;
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
			//charstarsceneFileNameForRayTracing = outputFileNameLDRwithoutSpritescharstar;
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

					view_info vi;
					vi.imgwidth = width;
					vi.imgheight = height;
					vi.eye.x = TAL_FILE_HEADER_DEFAULT_EYE_X;		
					vi.eye.y = TAL_FILE_HEADER_DEFAULT_EYE_Y;		
					vi.eye.z = TAL_FILE_HEADER_DEFAULT_EYE_Z;		
					vi.viewat.x = TAL_FILE_HEADER_DEFAULT_VIEWAT_X;		
					vi.viewat.y = TAL_FILE_HEADER_DEFAULT_VIEWAT_Y;		
					vi.viewat.z = TAL_FILE_HEADER_DEFAULT_VIEWAT_Z;		
					vi.viewup.x = TAL_FILE_HEADER_DEFAULT_VIEWUP_X;		
					vi.viewup.y = TAL_FILE_HEADER_DEFAULT_VIEWUP_Y;		
					vi.viewup.z = TAL_FILE_HEADER_DEFAULT_VIEWUP_Z;		
					vi.viewwidth = TAL_FILE_HEADER_DEFAULT_VIEWSIZE_WIDTH;		
					vi.viewheight = TAL_FILE_HEADER_DEFAULT_VIEWSIZE_HEIGHT;		     								 

					vi.focal_length = TAL_FILE_HEADER_DEFAULT_FOCAL;

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


						Reference * nonspriteListInitialReference = new Reference();
						Reference * spriteListInitialReference = new Reference();

						int numSpritesAdded = 0;
						if(!ANNcreateNeuralNetworkReferenceListsWithAndWithoutSprites(outputFileNameLDRwithSpritescharstar, nonspriteListInitialReference, spriteListInitialReference, firstInputNeuronInNetwork, addSprites, &numSpritesAdded, useOutputSVGFile, writeFileObject))
						{
							result = false;
						}

						if(!ANNcreateNeuralNetworkSceneFilesWithAndWithoutSpritesFromReferenceLists(outputFileNameLDRwithoutSpritescharstar, outputFileNameLDRwithSpritescharstar, addSprites, nonspriteListInitialReference, spriteListInitialReference, numSpritesAdded))
						{
							result = false;
						}

						char * charstarsceneFileNameForRayTracing;
						if(addSprites)
						{
							if(!joinReferenceLists(nonspriteListInitialReference, spriteListInitialReference))
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


						delete nonspriteListInitialReference;
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
				write2DReferenceListCollapsedTo1DToFile(topLevelSceneFileNameCollapsed, initialReferenceInSceneFileForRayTracing);
				/* method2: why doesnt this work - "invalid dat file for conversion to rgb"
				char * topLevelSceneFileNameCollapsed = "sceneCollapsedForRaytracing.ldr";
				write2DReferenceListCollapsedTo1DToFile(topLevelSceneFileNameCollapsed, firstReferenceInPrintList);
				*/

				//cout << "has" << endl;

				unsigned char * rgbMap = new unsigned char[width*height*RGB_NUM];

				//setViewPort2DOrtho(-100.0, 2000.0, -100.0, 2000.0);
				setViewPort3DOrtho(0.0, 5.0, 5.0, 0.0, 2.0, -2.0);

				//now reparse file
				Reference * initialReferenceInCollapsedSceneFile = new Reference();
				Reference * topLevelReferenceInCollapsedSceneFile = new Reference(topLevelSceneFileNameCollapsed, 1, true);	//The information in this object is not required or meaningful, but needs to be passed into the parseFile/parseReferenceList recursive function
				if(!parseFile(topLevelSceneFileNameCollapsed, initialReferenceInCollapsedSceneFile, topLevelReferenceInCollapsedSceneFile, true))
				{//file does not exist
					cout << "The file: " << topLevelSceneFileNameCollapsed << " does not exist in the directory" << endl;
					exit(0);
				}
					
				drawPrimitivesReferenceListToOpenGLAndCreateRGBMapBasic(initialReferenceInCollapsedSceneFile, width, height, rgbMap);
				drawPrimitivesReferenceListToOpenGLAndCreateRGBMapBasic(initialReferenceInCollapsedSceneFile, width, height, rgbMap);
					//due to opengl code bug, need to execute this function twice.

				#ifdef TH_USE_RT_FOR_NEURAL_NETWORK_VEC_GRAPHICS
				if(useOutputPPMFile)
				{
					generatePixmapFromRGBMap(displayFileNamePPMcharstar, width, height, rgbMap);
				}
				#endif
			}
		}
				
	}
}

void writeExperienceListToFile(char * fileName, Experience * firstExperienceInList)
{
	ofstream experienceDataSetOFStreamObject(fileName);

	Experience * currentExperience = firstExperienceInList;
	while(currentExperience->next != NULL)
	{
		addExperienceToOFStream(&experienceDataSetOFStreamObject, currentExperience);
		currentExperience = currentExperience->next;
	}

	experienceDataSetOFStreamObject.close();
}



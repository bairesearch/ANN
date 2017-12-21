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
 * File Name: ANNdisplay.cpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2017 Baxter AI (baxterai.com)
 * Project: Artificial Neural Network (ANN)
 * Project Version: 3m10a 16-December-2017
 * Comments: TH = Test Harness
 *
 *******************************************************************************/


#include "ANNdisplay.hpp"
#ifdef ANN_ALGORITHM_MEMORY_NETWORK
#endif
#ifdef ANN_ALGORITHM_CLASSIFICATION_NETWORK
#endif


#ifdef USE_RT
#endif


#ifdef USE_RT

void ANNdisplayClass::generateExperienceWith2DrgbMap(unsigned char* rgbMap, int imageWidth, int imageHeight, ANNexperience* currentExperience, long objectDecision)
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
				unsigned char col = RTpixelMaps.getRGBMapValue(x, y, imageWidth, rgb, rgbMap);
				double normalisedMapValue =  ANNexperienceClass.normaliseExperienceInput((double)col, MAX_RGB_VAL);
				currentExperienceInput->inputValue = normalisedMapValue;
				ANNexperienceInput* newExperienceInput = new ANNexperienceInput();
				currentExperienceInput->next = newExperienceInput;
				currentExperienceInput = currentExperienceInput->next;
			}
		}
	}
}


void ANNdisplayClass::generateExperienceWith2Dmap(double* lumOrContrastOrDepthMap, int imageWidth, int imageHeight, double mapMaxValue, ANNexperience* currentExperience, long objectDecision)
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

void ANNdisplayClass::generateExperienceWith2DbooleanMap(bool* booleanMap, int imageWidth, int imageHeight, ANNexperience* currentExperience, long objectDecision)
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





bool ANNdisplayClass::trainAndOutputNeuralNetworkWithFileNames(ANNneuron* firstInputNeuronInNetwork, ANNneuron* firstOutputNeuronInNetwork, long numberOfInputNeurons, long numberOfOutputNeurons, ANNexperience* firstExperienceInList, bool addSprites, bool allowRaytrace, string* XMLNNSceneFileName, char* charstarvectorGraphicsLDRNNSceneFileName, char* charstarvectorGraphicsTALNNSceneFileName, char* charstarraytracedImagePPMNNSceneFileName, char* charstarexperienceNNSceneFileName, bool useFoldsDuringTraining, int maxOrSetNumEpochs)
{
	bool result = true;

	cout << "num experiences in list = " << ANNexperienceClass.countNumberOfExperiences(firstExperienceInList) << endl;

	long numberOfExperiences = ANNexperienceClass.countNumberOfExperiences(firstExperienceInList);

	if(useFoldsDuringTraining)
	{
		int maxFolds = MAX_NUM_FOLDS_ANNTH;
		#ifdef ANN_ALGORITHM_BACKPROPAGATION
		int maxNumEpochs = maxOrSetNumEpochs;
		ANNalgorithmBackpropagationTraining.trainNeuralNetworkBackpropagation(firstInputNeuronInNetwork, firstOutputNeuronInNetwork, numberOfInputNeurons, numberOfOutputNeurons, maxFolds, firstExperienceInList, numberOfExperiences, maxNumEpochs);
		#endif
		#ifdef ANN_ALGORITHM_MEMORY_NETWORK
		ANNalgorithmMemoryNetworkTraining.trainNeuralNetworkMemory(firstInputNeuronInNetwork, firstOutputNeuronInNetwork, numberOfInputNeurons, numberOfOutputNeurons, maxFolds, firstExperienceInList, numberOfExperiences);
		#endif
		#ifdef ANN_ALGORITHM_CLASSIFICATION_NETWORK
		ANNalgorithmClassificationNetworkTraining.trainNeuralNetworkClassificationSimple(firstInputNeuronInNetwork, &firstOutputNeuronInNetwork, numberOfInputNeurons, &numberOfOutputNeurons, firstExperienceInList, numberOfExperiences);
		#endif
		//this is done dynamically if do not have heaps of RAM
	}
	else
	{
		#ifdef ANN_ALGORITHM_BACKPROPAGATION
		int setNumEpochs = maxOrSetNumEpochs; //ANN_DEFAULT_SIMPLE_TRAIN_DEFAULT_NUM_OF_TRAINING_EPOCHS;
		ANNalgorithmBackpropagationTraining.trainNeuralNetworkBackpropagationSimple(firstInputNeuronInNetwork, firstOutputNeuronInNetwork, numberOfInputNeurons, numberOfOutputNeurons, setNumEpochs, firstExperienceInList, numberOfExperiences);
		#endif
		#ifdef ANN_ALGORITHM_MEMORY_NETWORK
		ANNalgorithmMemoryNetworkTraining.trainNeuralNetworkMemorySimple(firstInputNeuronInNetwork, firstOutputNeuronInNetwork, numberOfInputNeurons, numberOfOutputNeurons, firstExperienceInList, numberOfExperiences);
		#endif
		#ifdef ANN_ALGORITHM_CLASSIFICATION_NETWORK
		ANNalgorithmClassificationNetworkTraining.trainNeuralNetworkClassificationSimple(firstInputNeuronInNetwork, &firstOutputNeuronInNetwork, numberOfInputNeurons, &numberOfOutputNeurons, firstExperienceInList, numberOfExperiences);
		#endif
	}


#ifndef USE_OR
	if(!ANNxmlConversion.writeNetXMLfile(*XMLNNSceneFileName, firstInputNeuronInNetwork))
	{
		result = false;
	}

	string vectorGraphicsLDRNNSceneFileName = charstarvectorGraphicsLDRNNSceneFileName;
	string vectorGraphicsTALNNSceneFileName = charstarvectorGraphicsTALNNSceneFileName;
	string raytracedImagePPMNNSceneFileName = charstarraytracedImagePPMNNSceneFileName;
	string outputSVGFileName = "";
	string outputPPMFileName = "";
	outputNeuralNetworkToVectorGraphicsAndRaytrace(firstInputNeuronInNetwork, addSprites, allowRaytrace, false, true, false, true, vectorGraphicsLDRNNSceneFileName, NULL, NULL, raytracedImagePPMNNSceneFileName, vectorGraphicsTALNNSceneFileName, NULL, NULL);


	writeExperienceListToFile(charstarexperienceNNSceneFileName, firstExperienceInList);

#endif

	return result;
}



void ANNdisplayClass::outputNeuralNetworkToVectorGraphicsAndRaytrace(ANNneuron* firstInputNeuronInNetwork, bool addSprites, bool allowRaytrace, bool displayInOpenGL, bool useOutputLDRFile, bool useOutputSVGFile, bool useOutputPPMFile, string outputLDRFileName, string outputSVGFileName, string outputPPMFileName, string outputPPMFileNameRaytraced, string outputTALFileName, int width, int height)
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

		if(!ANNdraw.ANNcreateNeuralNetworkReferenceLists(outputFileNameLDRcharstar, initialReference, firstInputNeuronInNetwork, addSprites, &numSpritesAdded, useOutputSVGFile, &currentTagInSVGFile, useOutputLDRFile))
		{
			result = false;
		}

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

		if(useOutputSVGFile)
		{
			#ifndef ANN_DRAW_DISABLE_FILE_OUTPUT_NOTIFICATIONS
			cout << "SVG graphics file name = " << outputSVGFileName << endl;
			#endif
			if(!LDsvg.writeSVGfile(outputFileNameSVGcharstar, firstTagInSVGFile))
			{
				result = false;
			}
			delete firstTagInSVGFile;
		}

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

				unsigned char* rgbMap = new unsigned char[width*height*RGB_NUM];

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



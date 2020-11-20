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
 * File Name: ANNdraw.hpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2020 Baxter AI (baxterai.com)
 * Project: Generic Construct Functions
 * Project Version: 3o4b 17-November-2020
 * Description: This code allows the addition of a sprite into a given scene file where a sprite is a paragraph of text. [The text is to be rendered in 3D, and point towards the user POV]
 * /
 *******************************************************************************/


#ifndef HEADER_ANN_DRAW
#define HEADER_ANN_DRAW

#include "LDreferenceClass.hpp"
#include "ANNneuronClass.hpp"
#include "ANNneuronConnectionClass.hpp"
#include "XMLparserClass.hpp"
#include "LDsprite.hpp"
#include "math.h"
#include "LDreferenceManipulation.hpp"
#include "SHAREDvector.hpp"
#include "XMLrulesClass.hpp"
#include "LDsvg.hpp"

#define NEURON_SPRITE_SCALE_FACTOR 0.005	//0.005

#define CPLUSPLUSERRORCORRECTION1 (10000)
//#define CPLUSPLUSERRORCORRECTION3

#define ANN_OUTPUT_Z_POSITION_CONNECTIONS (0.6)
#define ANN_OUTPUT_Z_POSITION_NODES (0.4)
#define ANN_OUTPUT_Z_POSITION_TEXT (0.2)
#define ANN_SVG_SCALE_FACTOR (100)
#define ANN_SVG_TEXT_SCALE_FACTOR (5)
#define ANN_SVG_NEURON_SIZE (10)
//#define ANN_SVG_SPRITE_TEXT_MAX_NUMBER_LINES (10)
#define ANN_SVG_SPRITE_TEXT_OFFSET_PER_LINE (10)

#define ANN_SPRITE_HEADER_NAME "\n0 Start ANN.exe Sprites\n"
#define ANN_SPRITE_TRAILER_NAME "0 End ANN.exe Sprites\n"
#define ANN_SPRITE_HEADER_NUM_LINES (2)
#define ANN_SPRITE_TRAILER_NUM_LINES (1)
#define ANN_SPRITE_HEADER_LENGTH 25
#define ANN_SPRITE_TRAILER_LENGTH 22


#define NEURAL_NETWORK_VISUALISATION_BASE_FILE_NAME "neuralNet"
#define NEURAL_NETWORK_VISUALISATION_XML_FILE_EXTENSION ".xml"
#define NEURAL_NETWORK_VISUALISATION_LDR_FILE_EXTENSION ".ldr"
#define NEURAL_NETWORK_VISUALISATION_SVG_FILE_EXTENSION ".svg"
#define NEURAL_NETWORK_VISUALISATION_PPM_FILE_EXTENSION ".ppm"
#define NEURAL_NETWORK_VISUALISATION_PPM_RAYTRACED_FILE_EXTENSION "Raytraced.ppm"
#define NEURAL_NETWORK_VISUALISATION_TAL_FILE_EXTENSION ".tal"


#define LDRAW_UNITS_PER_CM 25.189
	//create xml rules for above parameters


//#define ANN_SPRITE_TYPE_NEURON (1)			//kindOfNNSprite
//#define ANN_SPRITE_TYPE_CONNECTION (2)		//kindOfNNSprite

#define SPRITE_SUBMODEL_NEURON_FILE_NAME_NAME "SPRITE_SUBMODEL_NEURON_FILE_NAME"		//CHECKTHIS this can be changed between circle and ring depending upon High Resolution renderer; Ray Tracer versus LDVIEW [and taking into account other factors Eg LGEO parts library]
#define SPRITE_TEXTUAL_INCLUDE_NEURON_ID_COLOUR_NAME "SPRITE_TEXTUAL_INCLUDE_NEURON_ID_COLOUR"
#define SPRITE_TEXTUAL_INCLUDE_NEURON_ORDERID_COLOUR_NAME "SPRITE_TEXTUAL_INCLUDE_NEURON_ORDERID_COLOUR"
#define SPRITE_TEXTUAL_INCLUDE_NEURON_LAYERID_COLOUR_NAME "SPRITE_TEXTUAL_INCLUDE_NEURON_LAYERID_COLOUR"
#define SPRITE_TEXTUAL_INCLUDE_NEURON_SUBNETID_COLOUR_NAME "SPRITE_TEXTUAL_INCLUDE_NEURON_SUBNETID_COLOUR"
#define SPRITE_TEXTUAL_INCLUDE_NEURON_BIAS_COLOUR_NAME "SPRITE_TEXTUAL_INCLUDE_NEURON_BIAS_COLOUR"
#define SPRITE_TEXTUAL_INCLUDE_NEURON_OUTPUT_COLOUR_NAME "SPRITE_TEXTUAL_INCLUDE_NEURON_OUTPUT_COLOUR"
#define SPRITE_TEXTUAL_INCLUDE_NEURON_CLASSTARGET_COLOUR_NAME "SPRITE_TEXTUAL_INCLUDE_NEURON_CLASSTARGET_COLOUR"
#define SPRITE_TEXTUAL_INCLUDE_NEURON_ERROR_COLOUR_NAME "SPRITE_TEXTUAL_INCLUDE_NEURON_ERROR_COLOUR"
#define SPRITE_TEXTUAL_INCLUDE_NEURONCONNECTION_WEIGHT_COLOUR_NAME "SPRITE_TEXTUAL_INCLUDE_NEURONCONNECTION_WEIGHT_COLOUR"
#define SPRITE_SUBMODEL_RANGE_SCALE_FACTOR_NAME "SPRITE_SUBMODEL_RANGE_SCALE_FACTOR"		//this value is now multiplied by LDRAW_UNITS_PER_CM

#define SPRITE_TEXTUAL_INCLUDE_DESCRIPTION_TEXT
#define SPRITE_TEXTUAL_MAX_NUM_OF_LINES (8)

#define SPRITE_TEXTUAL_INCLUDE_NEURON_ID_INFO_NAME "SPRITE_TEXTUAL_INCLUDE_NEURON_ID_INFO"
#define SPRITE_TEXTUAL_INCLUDE_NEURON_ORDERID_INFO_NAME "SPRITE_TEXTUAL_INCLUDE_NEURON_ORDERID_INFO"
#define SPRITE_TEXTUAL_INCLUDE_NEURON_LAYERID_INFO_NAME "SPRITE_TEXTUAL_INCLUDE_NEURON_LAYERID_INFO"
#define SPRITE_TEXTUAL_INCLUDE_NEURON_SUBNETID_INFO_NAME "SPRITE_TEXTUAL_INCLUDE_NEURON_SUBNETID_INFO"
#define SPRITE_TEXTUAL_INCLUDE_NEURON_BIAS_INFO_NAME "SPRITE_TEXTUAL_INCLUDE_NEURON_BIAS_INFO"
#define SPRITE_TEXTUAL_INCLUDE_NEURON_OUTPUT_INFO_NAME "SPRITE_TEXTUAL_INCLUDE_NEURON_OUTPUT_INFO"
#define SPRITE_TEXTUAL_INCLUDE_NEURON_CLASSTARGET_INFO_NAME "SPRITE_TEXTUAL_INCLUDE_NEURON_CLASSTARGET_INFO"
#define SPRITE_TEXTUAL_INCLUDE_NEURON_ERROR_INFO_NAME "SPRITE_TEXTUAL_INCLUDE_NEURON_ERROR_INFO"
#define SPRITE_TEXTUAL_INCLUDE_NEURONCONNECTION_WEIGHT_INFO_NAME "SPRITE_TEXTUAL_INCLUDE_NEURONCONNECTION_WEIGHT_INFO"
/*
#define SPRITE_TEXTUAL_INCLUDE_NEURON_ID_INFO
#define SPRITE_TEXTUAL_INCLUDE_NEURON_EXTRA_SECONDARY_INFO
#ifdef SPRITE_TEXTUAL_INCLUDE_NEURON_EXTRA_SECONDARY_INFO
#define SPRITE_TEXTUAL_INCLUDE_NEURON_ORDERID_INFO
#define SPRITE_TEXTUAL_INCLUDE_NEURON_LAYERID_INFO
#define SPRITE_TEXTUAL_INCLUDE_NEURON_SUBNETID_INFO
#define SPRITE_TEXTUAL_INCLUDE_NEURON_BIAS_INFO
#define SPRITE_TEXTUAL_INCLUDE_NEURON_OUTPUT_INFO
#define SPRITE_TEXTUAL_INCLUDE_NEURON_CLASSTARGET_INFO
#define SPRITE_TEXTUAL_INCLUDE_NEURON_ERROR_INFO
#define SPRITE_TEXTUAL_INCLUDE_NEURONCONNECTION_WEIGHT_INFO
#endif
*/





class ANNdrawClass
{
	private: LDspriteClass LDsprite;
	private: LDreferenceManipulationClass LDreferenceManipulation;
	private: LDreferenceClassClass LDreferenceClassObject;
	private: SHAREDvectorClass SHAREDvector;
	private: LDsvgClass LDsvg;
	private: SHAREDvarsClass SHAREDvars;
	public: void fillInANNSpriteExternVariables();

/*top level sprite routine*/

//void checkThis();

	private: bool ANNcreateNeuralNetworkSceneFiles(string sceneFileName, ANNneuron* firstNeuronInNetwork, bool addSprites, bool writeSVG, XMLparserTag** currentTagSVG);
		public: bool ANNcreateNeuralNetworkReferenceLists(string sceneFileName, LDreference* initialReference, ANNneuron* firstNeuronInNetwork, bool addSprites, int* numSpritesAdded, bool writeSVG, XMLparserTag** currentTagSVG, bool writeLDR);
		public: bool ANNcreateNeuralNetworkSceneFilesFromReferenceLists(string sceneFileName, bool addSprites, LDreference* initialReference, int numSpritesAdded);
			private: LDreference* ANNsearchNeuralNetworkAndCreateReferences(ANNneuron* firstNeuronInLayer, LDreference* currentListReference, vec* eyeCoords, int* numSpritesAdded, string sceneFileName, bool isSubnet, vec* positionOfSubnetNeuron, bool addSprites, bool writeSVG, XMLparserTag** currentTagSVG, bool writeLDR);
				#ifdef ANN_DRAW_PREVENT_REPRINT
				private: void ANNsearchNeuralNetworkAndCreateReferencesReset(ANNneuron* firstNeuronInLayer);
				#endif
				private: bool ANNfillNeuronDisplayReference(LDreference* currentNeuronDispayReference, ANNneuron* neuron, bool hasSubnetNeuron, vec* positionOfsubnetNeuron, bool writeSVG, XMLparserTag** currentTagSVG);
				private: bool ANNfillANNneuronConnectionDisplayReference(LDreference* currentNeuronDispayReference, LDreference* backNeuronReference, LDreference* forwardNeuronReference, ANNneuronConnection* ANNneuronConnection, bool writeSVG, XMLparserTag** currentTagSVG);

				private: bool ANNdetermineSpriteInfoForNeuronAndAddSpriteToSpriteRefList(ANNneuron* neuron, LDreference** currentListReference, vec* eyeCoords, int* numSpritesAdded, string sceneFileName, vec* neuronReferenceRelativePosition, bool writeSVG, XMLparserTag** currentTagSVG, bool writeLDR);
				private: bool ANNdetermineSpriteInfoForANNneuronConnectionAndAddSpriteToSpriteRefList(ANNneuronConnection* ANNneuronConnection, LDreference** currentListReference, vec* eyeCoords, int* numSpritesAdded, string sceneFileName, vec* backNeuronReferenceRelativePosition, vec* forwardNeuronReferenceRelativePosition, bool writeSVG, XMLparserTag** currentTagSVG, bool writeLDR);
					private: bool ANNaddSpriteToSpriteReferenceList(vec* spriteSceneCoords, vec* eyeCoords, LDreference** currentListReference, string spriteReferenceFileName, int spriteDefaultColour, double spriteScaleFactor);
					private: void writeSpriteTextToSVG(string* spriteTextString, bool writeSVG, XMLparserTag** currentTagSVG, vec* referencePosition);

				private: void ANNgenerateTextualNeuronSpriteInfoString(ANNneuron* neuron, string* spriteTextString, int spriteColourArray[]);
				private: void ANNgenerateTextualANNneuronConnectionSpriteInfoString(ANNneuronConnection* ANNneuronConnection, string* spriteTextString, int spriteColourArray[]);

	private: colour convertDoubleToRainbow(const double colourDouble, const double doubleMaxValue);
		private: colour convertDoubleToRainbow(const double f);

};


#endif


/*******************************************************************************
 *
 * File Name: ANNsprite.h
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2012 Baxter AI (baxterai.com)
 * Project: Generic Construct Functions
 * Project Version: 3a8a 14-June-2012
 *
 *******************************************************************************/


/*spriteCode.cpp - 10-Sept-06 12:30pm - for integration into legoRulesRoundsChecker. This code allows the addition of a sprite into a given scene file where a sprite is a paragraph of text. [The text is to be rendered in 3D, and point towards the user POV - see CHECK THIS for missing code]*/

#ifndef HEADER_ANN_SPRITE
#define HEADER_ANN_SPRITE

#include "LDreferenceClass.h"
#include "ANNneuronClass.h"

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

#define ANN_SPRITE_HEADER_NAME "\n0 Start ann.exe Sprites\n"
#define ANN_SPRITE_TRAILER_NAME "0 End ann.exe Sprites\n"
#define ANN_SPRITE_HEADER_NUM_LINES (2)
#define ANN_SPRITE_TRAILER_NUM_LINES (1)
#define ANN_SPRITE_HEADER_LENGTH 25
#define ANN_SPRITE_TRAILER_LENGTH 22



#define NEURAL_NETWORK_VISUALISATION_DAT_FILE_NAME_WITHOUT_SPRITES "netWithoutSprites.ldr"
#define NEURAL_NETWORK_VISUALISATION_DAT_FILE_NAME_WITH_SPRITES "netWithSprites.ldr"
#define NEURAL_NETWORK_VISUALISATION_SVG_FILE_NAME "net.svg"

#define LDRAW_UNITS_PER_CM 25.189
	//create xml rules for above parameters


//#define ANN_SPRITE_TYPE_NEURON (1)			//kindOfNNSprite
//#define ANN_SPRITE_TYPE_CONNECTION (2)		//kindOfNNSprite

#define SPRITE_SUBMODEL_NEURON_FILE_NAME_NAME 				"SPRITE_SUBMODEL_NEURON_FILE_NAME"		//CHECKTHIS this can be changed between circle and ring depending upon High Resolution renderer; Ray Tracer versus LDVIEW [and taking into account other factors Eg LGEO parts library]
#define SPRITE_TEXTUAL_INCLUDE_NEURON_ID_COLOUR_NAME 		"SPRITE_TEXTUAL_INCLUDE_NEURON_ID_COLOUR"
#define SPRITE_TEXTUAL_INCLUDE_NEURON_ORDERID_COLOUR_NAME 	"SPRITE_TEXTUAL_INCLUDE_NEURON_ORDERID_COLOUR"
#define SPRITE_TEXTUAL_INCLUDE_NEURON_LAYERID_COLOUR_NAME 	"SPRITE_TEXTUAL_INCLUDE_NEURON_LAYERID_COLOUR"
#define SPRITE_TEXTUAL_INCLUDE_NEURON_SUBNETID_COLOUR_NAME 	"SPRITE_TEXTUAL_INCLUDE_NEURON_SUBNETID_COLOUR"
#define SPRITE_TEXTUAL_INCLUDE_NEURON_BIAS_COLOUR_NAME 		"SPRITE_TEXTUAL_INCLUDE_NEURON_BIAS_COLOUR"
#define SPRITE_TEXTUAL_INCLUDE_NEURON_OUTPUT_COLOUR_NAME 	"SPRITE_TEXTUAL_INCLUDE_NEURON_OUTPUT_COLOUR"
#define SPRITE_TEXTUAL_INCLUDE_NEURON_CLASSTARGET_COLOUR_NAME 	"SPRITE_TEXTUAL_INCLUDE_NEURON_CLASSTARGET_COLOUR"
#define SPRITE_TEXTUAL_INCLUDE_NEURON_ERROR_COLOUR_NAME 	"SPRITE_TEXTUAL_INCLUDE_NEURON_ERROR_COLOUR"
#define SPRITE_TEXTUAL_INCLUDE_NEURONCONNECTION_WEIGHT_COLOUR_NAME 	"SPRITE_TEXTUAL_INCLUDE_NEURONCONNECTION_WEIGHT_COLOUR"
#define SPRITE_SUBMODEL_RANGE_SCALE_FACTOR_NAME 			"SPRITE_SUBMODEL_RANGE_SCALE_FACTOR"		//this value is now multiplied by LDRAW_UNITS_PER_CM


#define SPRITE_TEXTUAL_INCLUDE_DESCRIPTION_TEXT
#define SPRITE_TEXTUAL_MAX_NUM_OF_LINES (8) /*defence level, LD attack level, CC attack level, movement level*/


#define SPRITE_TEXTUAL_INCLUDE_NEURON_ID_INFO_NAME					"SPRITE_TEXTUAL_INCLUDE_NEURON_ID_INFO"
#define SPRITE_TEXTUAL_INCLUDE_NEURON_ORDERID_INFO_NAME				"SPRITE_TEXTUAL_INCLUDE_NEURON_ORDERID_INFO"
#define SPRITE_TEXTUAL_INCLUDE_NEURON_LAYERID_INFO_NAME				"SPRITE_TEXTUAL_INCLUDE_NEURON_LAYERID_INFO"
#define SPRITE_TEXTUAL_INCLUDE_NEURON_SUBNETID_INFO_NAME			"SPRITE_TEXTUAL_INCLUDE_NEURON_SUBNETID_INFO"
#define SPRITE_TEXTUAL_INCLUDE_NEURON_BIAS_INFO_NAME				"SPRITE_TEXTUAL_INCLUDE_NEURON_BIAS_INFO"
#define SPRITE_TEXTUAL_INCLUDE_NEURON_OUTPUT_INFO_NAME				"SPRITE_TEXTUAL_INCLUDE_NEURON_OUTPUT_INFO"
#define SPRITE_TEXTUAL_INCLUDE_NEURON_CLASSTARGET_INFO_NAME			"SPRITE_TEXTUAL_INCLUDE_NEURON_CLASSTARGET_INFO"
#define SPRITE_TEXTUAL_INCLUDE_NEURON_ERROR_INFO_NAME				"SPRITE_TEXTUAL_INCLUDE_NEURON_ERROR_INFO"
#define SPRITE_TEXTUAL_INCLUDE_NEURONCONNECTION_WEIGHT_INFO_NAME	"SPRITE_TEXTUAL_INCLUDE_NEURONCONNECTION_WEIGHT_INFO"
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





void fillInANNSpriteExternVariables();

/*top level sprite routine*/

//void checkThis();

bool ANNcreateNeuralNetworkSceneFilesWithAndWithoutSprites(char* sceneFileNameWithoutSprites, char* sceneFileNameWithSprites, NeuronContainer * firstNeuronInNetwork, bool addSprites, bool writeSVG, ofstream * writeFileObject);
	bool ANNcreateNeuralNetworkReferenceListsWithAndWithoutSprites(char* sceneFileNameWithSprites, Reference * nonspriteListInitialReference, Reference * spriteListInitialReference, NeuronContainer * firstNeuronInNetwork, bool addSprites, int * numSpritesAdded, bool writeSVG, ofstream * writeFileObject);
	bool ANNcreateNeuralNetworkSceneFilesWithAndWithoutSpritesFromReferenceLists(char* sceneFileNameWithoutSprites, char* sceneFileNameWithSprites, bool addSprites, Reference * nonspriteListInitialReference, Reference * spriteListInitialReference, int numSpritesAdded);
		Reference * ANNsearchNeuralNetworkAndCreateSpriteAndNonSpriteReferences(NeuronContainer * firstNeuronContainerInLayer, Reference * spriteListInitialReference, Reference * currentNonSpriteListReference, vec * eyeCoords, int * numSpritesAdded, int * numnonSpritesAdded, char* sceneFileNameWithSprites, bool isSubnet, vec * positionOfSubnetNeuron, bool addSprites, bool writeSVG, ofstream * writeFileObject);
			bool ANNfillNeuronDisplayReference(Reference * currentNeuronDispayReference, Neuron * neuron, bool hasSubnetNeuron, vec * positionOfsubnetNeuron, bool writeSVG, ofstream * writeFileObject);
			bool ANNfillNeuronConnectionDisplayReference(Reference * currentNeuronDispayReference, Reference * backNeuronReference, Reference * forwardNeuronReference, NeuronConnection * neuronConnection, bool writeSVG, ofstream * writeFileObject);

			bool ANNdetermineSpriteInfoForNeuronConnectionAndAddSpriteToSpriteRefList(Reference * neuronReference, NeuronConnectionContainer * neuronConnectionContainer, Reference * spriteListInitialReference, vec * eyeCoords, int * numSpritesAdded, char * sceneFileName, Reference * backNeuronReference, Reference * forwardNeuronReference, bool writeSVG, ofstream * writeFileObject);
			bool ANNdetermineSpriteInfoForNeuronAndAddSpriteToSpriteRefList(Reference * neuronReference, NeuronContainer * neuron, Reference * spriteListInitialReference, vec * eyeCoords, int * numSpritesAdded, char * sceneFileName, bool writeSVG, ofstream * writeFileObject);


			void ANNgenerateTextualNeuronSpriteInfoString(NeuronContainer * neuronContainer, string * spriteTextString, int spriteColourArray[]);
			void ANNgenerateTextualNeuronConnectionSpriteInfoString(NeuronConnectionContainer * neuronConnectionContainer, string * spriteTextString, int spriteColourArray[]);


#endif


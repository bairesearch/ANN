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
 * File Name: ANNsprite.cpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2012 Baxter AI (baxterai.com)
 * Project: Generic Construct Functions
 * Project Version: 3c3a 16-November-2012
 * Description: This code allows the addition of a sprite into a given scene file where a sprite is a paragraph of text. [The text is to be rendered in 3D, and point towards the user POV]
 *
 *******************************************************************************/




#include "ANNsprite.h"
#include "LDsprite.h"
#include "math.h"
#include "LDreferenceManipulation.h"
#include "SHAREDvector.h"
#include "XMLrulesClass.h"
#include "LDsvg.h"





static string SPRITE_SUBMODEL_NEURON_FILE_NAME;
static int SPRITE_TEXTUAL_INCLUDE_NEURON_ID_COLOUR;
static int SPRITE_TEXTUAL_INCLUDE_NEURON_ORDERID_COLOUR;
static int SPRITE_TEXTUAL_INCLUDE_NEURON_LAYERID_COLOUR;
static int SPRITE_TEXTUAL_INCLUDE_NEURON_SUBNETID_COLOUR;
static int SPRITE_TEXTUAL_INCLUDE_NEURON_BIAS_COLOUR;
static int SPRITE_TEXTUAL_INCLUDE_NEURON_OUTPUT_COLOUR;
static int SPRITE_TEXTUAL_INCLUDE_NEURON_CLASSTARGET_COLOUR;
static int SPRITE_TEXTUAL_INCLUDE_NEURON_ERROR_COLOUR;
static int SPRITE_TEXTUAL_INCLUDE_NEURONCONNECTION_WEIGHT_COLOUR;
static double SPRITE_SUBMODEL_RANGE_SCALE_FACTOR;

bool SPRITE_TEXTUAL_INCLUDE_NEURON_ID_INFO;
bool SPRITE_TEXTUAL_INCLUDE_NEURON_ORDERID_INFO;
bool SPRITE_TEXTUAL_INCLUDE_NEURON_LAYERID_INFO;
bool SPRITE_TEXTUAL_INCLUDE_NEURON_SUBNETID_INFO;
bool SPRITE_TEXTUAL_INCLUDE_NEURON_BIAS_INFO;
bool SPRITE_TEXTUAL_INCLUDE_NEURON_OUTPUT_INFO;
bool SPRITE_TEXTUAL_INCLUDE_NEURON_CLASSTARGET_INFO;
bool SPRITE_TEXTUAL_INCLUDE_NEURON_ERROR_INFO;
bool SPRITE_TEXTUAL_INCLUDE_NEURONCONNECTION_WEIGHT_INFO;

bool SPRITE_TEXTUAL_INCLUDE_NEURON_EXTRA_SECONDARY_INFO;
bool SPRITE_TEXTUAL_INCLUDE_NEURONCONNECTION_INFO;
bool SPRITE_TEXTUAL_INCLUDE_NEURON_INFO;
bool SPRITE_TEXTUAL_INCLUDE_INFO;

void fillInANNSpriteExternVariables()
{
	RulesClass * currentReferenceRulesClass = ANNrulesSprite;

	while(currentReferenceRulesClass->next != NULL)
	{
		if(currentReferenceRulesClass->name == SPRITE_SUBMODEL_NEURON_FILE_NAME_NAME)
		{
			SPRITE_SUBMODEL_NEURON_FILE_NAME = currentReferenceRulesClass->stringValue;
		}
		else if(currentReferenceRulesClass->name == SPRITE_TEXTUAL_INCLUDE_NEURON_ID_COLOUR_NAME)
		{
			SPRITE_TEXTUAL_INCLUDE_NEURON_ID_COLOUR = currentReferenceRulesClass->fractionalValue;
		}
		else if(currentReferenceRulesClass->name == SPRITE_TEXTUAL_INCLUDE_NEURON_ORDERID_COLOUR_NAME)
		{
			SPRITE_TEXTUAL_INCLUDE_NEURON_ORDERID_COLOUR = currentReferenceRulesClass->fractionalValue;
		}
		else if(currentReferenceRulesClass->name == SPRITE_TEXTUAL_INCLUDE_NEURON_LAYERID_COLOUR_NAME)
		{
			SPRITE_TEXTUAL_INCLUDE_NEURON_LAYERID_COLOUR = currentReferenceRulesClass->fractionalValue;
		}
		else if(currentReferenceRulesClass->name == SPRITE_TEXTUAL_INCLUDE_NEURON_SUBNETID_COLOUR_NAME)
		{
			SPRITE_TEXTUAL_INCLUDE_NEURON_SUBNETID_COLOUR = currentReferenceRulesClass->fractionalValue;
		}
		else if(currentReferenceRulesClass->name == SPRITE_TEXTUAL_INCLUDE_NEURON_BIAS_COLOUR_NAME)
		{
			SPRITE_TEXTUAL_INCLUDE_NEURON_BIAS_COLOUR = currentReferenceRulesClass->fractionalValue;
		}
		else if(currentReferenceRulesClass->name == SPRITE_TEXTUAL_INCLUDE_NEURON_OUTPUT_COLOUR_NAME)
		{
			SPRITE_TEXTUAL_INCLUDE_NEURON_OUTPUT_COLOUR = currentReferenceRulesClass->fractionalValue;
		}
		else if(currentReferenceRulesClass->name == SPRITE_TEXTUAL_INCLUDE_NEURON_CLASSTARGET_COLOUR_NAME)
		{
			SPRITE_TEXTUAL_INCLUDE_NEURON_CLASSTARGET_COLOUR = currentReferenceRulesClass->fractionalValue;
		}
		else if(currentReferenceRulesClass->name == SPRITE_TEXTUAL_INCLUDE_NEURON_ERROR_COLOUR_NAME)
		{
			SPRITE_TEXTUAL_INCLUDE_NEURON_ERROR_COLOUR = currentReferenceRulesClass->fractionalValue;
		}
		else if(currentReferenceRulesClass->name == SPRITE_TEXTUAL_INCLUDE_NEURONCONNECTION_WEIGHT_COLOUR_NAME)
		{
			SPRITE_TEXTUAL_INCLUDE_NEURONCONNECTION_WEIGHT_COLOUR = currentReferenceRulesClass->fractionalValue;
		}
		else if(currentReferenceRulesClass->name == SPRITE_SUBMODEL_RANGE_SCALE_FACTOR_NAME)
		{
			SPRITE_SUBMODEL_RANGE_SCALE_FACTOR = currentReferenceRulesClass->fractionalValue;
		}

		else if(currentReferenceRulesClass->name == SPRITE_TEXTUAL_INCLUDE_NEURON_ID_INFO_NAME)
		{
			SPRITE_TEXTUAL_INCLUDE_NEURON_ID_INFO = (bool)((int)(currentReferenceRulesClass->fractionalValue));
		}
		else if(currentReferenceRulesClass->name == SPRITE_TEXTUAL_INCLUDE_NEURON_ORDERID_INFO_NAME)
		{
			SPRITE_TEXTUAL_INCLUDE_NEURON_ORDERID_INFO = (bool)((int)(currentReferenceRulesClass->fractionalValue));
		}
		else if(currentReferenceRulesClass->name == SPRITE_TEXTUAL_INCLUDE_NEURON_LAYERID_INFO_NAME)
		{
			SPRITE_TEXTUAL_INCLUDE_NEURON_LAYERID_INFO = (bool)((int)(currentReferenceRulesClass->fractionalValue));
		}
		else if(currentReferenceRulesClass->name == SPRITE_TEXTUAL_INCLUDE_NEURON_SUBNETID_INFO_NAME)
		{
			SPRITE_TEXTUAL_INCLUDE_NEURON_SUBNETID_INFO = (bool)((int)(currentReferenceRulesClass->fractionalValue));
		}
		else if(currentReferenceRulesClass->name == SPRITE_TEXTUAL_INCLUDE_NEURON_BIAS_INFO_NAME)
		{
			SPRITE_TEXTUAL_INCLUDE_NEURON_BIAS_INFO = (bool)((int)(currentReferenceRulesClass->fractionalValue));
		}
		else if(currentReferenceRulesClass->name == SPRITE_TEXTUAL_INCLUDE_NEURON_OUTPUT_INFO_NAME)
		{
			SPRITE_TEXTUAL_INCLUDE_NEURON_OUTPUT_INFO = (bool)((int)(currentReferenceRulesClass->fractionalValue));
		}
		else if(currentReferenceRulesClass->name == SPRITE_TEXTUAL_INCLUDE_NEURON_CLASSTARGET_INFO_NAME)
		{
			SPRITE_TEXTUAL_INCLUDE_NEURON_CLASSTARGET_INFO = (bool)((int)(currentReferenceRulesClass->fractionalValue));
		}
		else if(currentReferenceRulesClass->name == SPRITE_TEXTUAL_INCLUDE_NEURON_ERROR_INFO_NAME)
		{
			SPRITE_TEXTUAL_INCLUDE_NEURON_ERROR_INFO = (bool)((int)(currentReferenceRulesClass->fractionalValue));
		}
		else if(currentReferenceRulesClass->name == SPRITE_TEXTUAL_INCLUDE_NEURONCONNECTION_WEIGHT_INFO_NAME)
		{
			SPRITE_TEXTUAL_INCLUDE_NEURONCONNECTION_WEIGHT_INFO = (bool)((int)(currentReferenceRulesClass->fractionalValue));
		}
		else
		{

		}

		currentReferenceRulesClass = currentReferenceRulesClass->next;
	}

	SPRITE_TEXTUAL_INCLUDE_NEURON_EXTRA_SECONDARY_INFO = false;

	if(SPRITE_TEXTUAL_INCLUDE_NEURON_ORDERID_INFO || SPRITE_TEXTUAL_INCLUDE_NEURON_LAYERID_INFO || SPRITE_TEXTUAL_INCLUDE_NEURON_SUBNETID_INFO || SPRITE_TEXTUAL_INCLUDE_NEURON_BIAS_INFO || SPRITE_TEXTUAL_INCLUDE_NEURON_OUTPUT_INFO || SPRITE_TEXTUAL_INCLUDE_NEURON_CLASSTARGET_INFO || SPRITE_TEXTUAL_INCLUDE_NEURON_ERROR_INFO)
	{
		SPRITE_TEXTUAL_INCLUDE_NEURON_EXTRA_SECONDARY_INFO = true;
	}

	if(SPRITE_TEXTUAL_INCLUDE_NEURON_ID_INFO || SPRITE_TEXTUAL_INCLUDE_NEURON_EXTRA_SECONDARY_INFO)
	{
		SPRITE_TEXTUAL_INCLUDE_NEURON_INFO = true;
	}
	else
	{
		SPRITE_TEXTUAL_INCLUDE_NEURON_INFO = false;
	}

	if(SPRITE_TEXTUAL_INCLUDE_NEURONCONNECTION_WEIGHT_INFO)
	{
		SPRITE_TEXTUAL_INCLUDE_NEURONCONNECTION_INFO = true;
	}
	else
	{
		SPRITE_TEXTUAL_INCLUDE_NEURONCONNECTION_INFO = false;
	}
	if(SPRITE_TEXTUAL_INCLUDE_NEURONCONNECTION_INFO || SPRITE_TEXTUAL_INCLUDE_NEURON_INFO)
	{
		SPRITE_TEXTUAL_INCLUDE_INFO = true;
	}
	else
	{
		SPRITE_TEXTUAL_INCLUDE_INFO = false;
	}

	fillInLDspriteExternVariables();
}


/*top level sprite routines - required for independent LRRCsprite.cpp calculations*/


bool ANNcreateNeuralNetworkSceneFilesWithAndWithoutSprites(char* sceneFileNameWithoutSprites, char* sceneFileNameWithSprites, NeuronContainer * firstNeuronInNetwork, bool addSprites, bool writeSVG, XMLparserTag ** currentTag)
{
	bool result = true;

	Reference * nonSpriteListInitialReference = new Reference();
	Reference * spriteListInitialReference = new Reference();

	int numSpritesAdded = 0;

	if(!ANNcreateNeuralNetworkReferenceListsWithAndWithoutSprites(sceneFileNameWithSprites, nonSpriteListInitialReference, spriteListInitialReference, firstNeuronInNetwork, addSprites, &numSpritesAdded, writeSVG, currentTag))
	{
		result = false;
	}

	if(!ANNcreateNeuralNetworkSceneFilesWithAndWithoutSpritesFromReferenceLists(sceneFileNameWithoutSprites, sceneFileNameWithSprites, addSprites, nonSpriteListInitialReference, spriteListInitialReference, numSpritesAdded))
	{
		result = false;
	}

	delete nonSpriteListInitialReference;
	delete spriteListInitialReference;

	return result;
}



bool ANNcreateNeuralNetworkReferenceListsWithAndWithoutSprites(char* sceneFileNameWithSprites, Reference * nonSpriteListInitialReference, Reference * spriteListInitialReference, NeuronContainer * firstNeuronInNetwork, bool addSprites, int * numSpritesAdded, bool writeSVG, XMLparserTag ** currentTag)
{
	bool result = true;

	int numnonSpritesAdded = 0;
	vec eyeCoords;
	eyeCoords.x = 0.0;
	eyeCoords.y = 0.0;
	eyeCoords.z = 0.0;

	ANNsearchNeuralNetworkAndCreateSpriteAndNonSpriteReferences(firstNeuronInNetwork, spriteListInitialReference, nonSpriteListInitialReference, &eyeCoords, numSpritesAdded, &numnonSpritesAdded, sceneFileNameWithSprites, false, NULL, addSprites, writeSVG, currentTag);

	return result;
}


bool ANNcreateNeuralNetworkSceneFilesWithAndWithoutSpritesFromReferenceLists(char* sceneFileNameWithoutSprites, char* sceneFileNameWithSprites, bool addSprites, Reference * nonSpriteListInitialReference, Reference * spriteListInitialReference, int numSpritesAdded)
{
	bool result = true;

	writeReferencesToFile(sceneFileNameWithoutSprites, nonSpriteListInitialReference);

	if(addSprites)
	{
		if(!addSpriteReferenceListToSceneFile(sceneFileNameWithoutSprites, sceneFileNameWithSprites, spriteListInitialReference, numSpritesAdded))
		{
			result = false;
		}
	}

	return result;
}




Reference * ANNsearchNeuralNetworkAndCreateSpriteAndNonSpriteReferences(NeuronContainer * firstNeuronContainerInLayer, Reference * spriteListInitialReference, Reference * currentNonSpriteListReference, vec * eyeCoords, int * numSpritesAdded, int * numnonSpritesAdded, char* sceneFileNameWithSprites, bool isSubnet, vec * positionOfSubnetNeuron, bool addSprites, bool writeSVG, XMLparserTag ** currentTag)
{
	Reference * updatedNonSpriteListReference = currentNonSpriteListReference;

	bool result = true;

	NeuronContainer * currentNeuron = firstNeuronContainerInLayer;
	while(currentNeuron->nextNeuronContainer != NULL)
	{
		ANNfillNeuronDisplayReference(updatedNonSpriteListReference, currentNeuron->neuron, isSubnet, positionOfSubnetNeuron, writeSVG, currentTag);
		Reference * currentNeuronReferenceOnLayer = updatedNonSpriteListReference;

		if(addSprites)
		{
			if(SPRITE_TEXTUAL_INCLUDE_NEURON_INFO)
			{
				if(!ANNdetermineSpriteInfoForNeuronAndAddSpriteToSpriteRefList(updatedNonSpriteListReference, currentNeuron, spriteListInitialReference, eyeCoords, numSpritesAdded, sceneFileNameWithSprites, writeSVG, currentTag))
				{
					result = false;
				}
			}
		}

		Reference * newNeuronDispayReference = new Reference();
		updatedNonSpriteListReference->next = newNeuronDispayReference;
		updatedNonSpriteListReference = updatedNonSpriteListReference->next;


		if(currentNeuron->hasBackLayer == true)
		{

			NeuronConnectionContainer * currentNeuronConnectionContainerInBackLayer = currentNeuron->firstBackNeuronConnectionContainer;
			while(currentNeuronConnectionContainerInBackLayer -> nextNeuronConnectionContainer != NULL)
			{
				//regenerate reference properties for a neuron display reference in the previous layer (which is connected to the current neuron)

				Reference currentNeuronReferenceOnPreviousLayer;
				ANNfillNeuronDisplayReference(&currentNeuronReferenceOnPreviousLayer, currentNeuronConnectionContainerInBackLayer->neuron, isSubnet, positionOfSubnetNeuron, writeSVG, currentTag);

				if(!ANNfillNeuronConnectionDisplayReference(updatedNonSpriteListReference, &currentNeuronReferenceOnPreviousLayer, currentNeuronReferenceOnLayer, currentNeuronConnectionContainerInBackLayer->neuronConnection, writeSVG, currentTag))
				{
					result = false;
				}

				if(addSprites)
				{
					if(SPRITE_TEXTUAL_INCLUDE_NEURONCONNECTION_INFO)
					{
						if(!ANNdetermineSpriteInfoForNeuronConnectionAndAddSpriteToSpriteRefList(updatedNonSpriteListReference, currentNeuronConnectionContainerInBackLayer, spriteListInitialReference, eyeCoords, numSpritesAdded, sceneFileNameWithSprites, &currentNeuronReferenceOnPreviousLayer, currentNeuronReferenceOnLayer, writeSVG, currentTag))
						{
							result = false;
						}
					}
				}

				Reference * newNeuronDispayReference = new Reference();
				updatedNonSpriteListReference->next = newNeuronDispayReference;
				updatedNonSpriteListReference = updatedNonSpriteListReference->next;


				currentNeuronConnectionContainerInBackLayer = currentNeuronConnectionContainerInBackLayer -> nextNeuronConnectionContainer;
			}
		}



	#ifdef ANN_ADVANCED
		if(currentNeuron->isSubnet)
		{
			//create back layer of subnet 1-1 connections for display only
			NeuronContainer * currentNeuronContainerInBackLayerOfSubnet = currentNeuron->firstNeuronContainerInBackLayerOfSubnet;
			NeuronConnectionContainer * currentNeuronConnectionContainerInBackLayer = currentNeuron->firstBackNeuronConnectionContainer;
			while(currentNeuronConnectionContainerInBackLayer -> nextNeuronConnectionContainer != NULL)
			{
				//regenerate reference properties for a neuron display reference in the previous layer (which is connected to the current neuron)

				Reference currentNeuronReferenceOnPreviousLayer;
				ANNfillNeuronDisplayReference(&currentNeuronReferenceOnPreviousLayer, currentNeuronConnectionContainerInBackLayer->neuron, isSubnet, positionOfSubnetNeuron, writeSVG, currentTag);

				Reference currentNeuronReferenceOnBackLayerOfSubnet;
				ANNfillNeuronDisplayReference(&currentNeuronReferenceOnBackLayerOfSubnet, currentNeuronContainerInBackLayerOfSubnet->neuron, true, &(currentNeuronReferenceOnLayer->relativePosition), writeSVG, currentTag);

				NeuronConnection blankOneToOneNeuronConnection;
				blankOneToOneNeuronConnection.weight = 0.0;
				if(!ANNfillNeuronConnectionDisplayReference(updatedNonSpriteListReference, &currentNeuronReferenceOnBackLayerOfSubnet, &currentNeuronReferenceOnPreviousLayer, &blankOneToOneNeuronConnection, writeSVG, currentTag))
				{
					result = false;
				}

				Reference * newNeuronDispayReference = new Reference();
				updatedNonSpriteListReference->next = newNeuronDispayReference;
				updatedNonSpriteListReference = updatedNonSpriteListReference->next;

				currentNeuronContainerInBackLayerOfSubnet=currentNeuronContainerInBackLayerOfSubnet->nextNeuronContainer;
				currentNeuronConnectionContainerInBackLayer = currentNeuronConnectionContainerInBackLayer -> nextNeuronConnectionContainer;
			}

			//create front layer of subnet 1-1 connections for display only
			NeuronContainer * currentNeuronContainerInFrontLayerOfSubnet = currentNeuron->firstNeuronContainerInFrontLayerOfSubnet;
			NeuronConnectionContainer * currentNeuronConnectionContainerInFrontLayer = currentNeuron->firstFrontNeuronConnectionContainer;
			while(currentNeuronConnectionContainerInFrontLayer -> nextNeuronConnectionContainer != NULL)
			{
				//regenerate reference properties for a neuron display reference in the previous layer (which is connected to the current neuron)

				Reference currentNeuronReferenceOnNextLayer;
				ANNfillNeuronDisplayReference(&currentNeuronReferenceOnNextLayer, currentNeuronConnectionContainerInFrontLayer->neuron, isSubnet, positionOfSubnetNeuron, writeSVG, currentTag);

				Reference currentNeuronReferenceOnFrontLayerOfSubnet;
				ANNfillNeuronDisplayReference(&currentNeuronReferenceOnFrontLayerOfSubnet, currentNeuronContainerInFrontLayerOfSubnet->neuron, true, &(currentNeuronReferenceOnLayer->relativePosition), writeSVG, currentTag);

				NeuronConnection blankOneToOneNeuronConnection;
				blankOneToOneNeuronConnection.weight = 0.0;
				if(!ANNfillNeuronConnectionDisplayReference(updatedNonSpriteListReference, &currentNeuronReferenceOnFrontLayerOfSubnet, &currentNeuronReferenceOnNextLayer, &blankOneToOneNeuronConnection, writeSVG, currentTag))
				{
					result = false;
				}

				Reference * newNeuronDispayReference = new Reference();
				updatedNonSpriteListReference->next = newNeuronDispayReference;
				updatedNonSpriteListReference = updatedNonSpriteListReference->next;

				currentNeuronContainerInFrontLayerOfSubnet=currentNeuronContainerInFrontLayerOfSubnet->nextNeuronContainer;
				currentNeuronConnectionContainerInFrontLayer = currentNeuronConnectionContainerInFrontLayer -> nextNeuronConnectionContainer;
			}

			updatedNonSpriteListReference = ANNsearchNeuralNetworkAndCreateSpriteAndNonSpriteReferences(currentNeuron->firstNeuronContainerInBackLayerOfSubnet, spriteListInitialReference, updatedNonSpriteListReference, eyeCoords, numSpritesAdded, numnonSpritesAdded, sceneFileNameWithSprites, (true|isSubnet), &(currentNeuronReferenceOnLayer->relativePosition), addSprites, writeSVG, currentTag);
		}
	#endif


		currentNeuron = currentNeuron -> nextNeuronContainer;
	}
	if(firstNeuronContainerInLayer->hasFrontLayer)
	{
		updatedNonSpriteListReference = ANNsearchNeuralNetworkAndCreateSpriteAndNonSpriteReferences(firstNeuronContainerInLayer->firstNeuronInFrontLayer, spriteListInitialReference, updatedNonSpriteListReference, eyeCoords, numSpritesAdded, numnonSpritesAdded, sceneFileNameWithSprites, isSubnet, positionOfSubnetNeuron, addSprites, writeSVG, currentTag);
	}

	return updatedNonSpriteListReference;

}













































/*medium level sprite routines - these can be used by LRRCsprite.cpp top level routines or by LRRCgame.cpp routines*/

bool ANNfillNeuronDisplayReference(Reference * currentNeuronDispayReference, Neuron * neuron, bool hasSubnetNeuron, vec * positionOfsubnetNeuron, bool writeSVG, XMLparserTag ** currentTag)
{
	bool result = true;

	mat currentDeformationMatrix;

	//add neuron range info [green ring/sphere/cylinder]

	createIdentityMatrix(&currentDeformationMatrix);
	scaleMatrix(&currentDeformationMatrix, (NEURON_SPRITE_SCALE_FACTOR/((neuron->subnetID)*2.0)*(SPRITE_SUBMODEL_RANGE_SCALE_FACTOR*LDRAW_UNITS_PER_CM)));		//	scaleMatrix(&currentDeformationMatrix, ((neuronContainer->neuron->bias)*(SPRITE_SUBMODEL_RANGE_SCALE_FACTOR*LDRAW_UNITS_PER_CM)));
	copyMatrixTwoIntoMatrixOne(&(currentNeuronDispayReference->deformationMatrix), &(currentDeformationMatrix));

	currentNeuronDispayReference->type = REFERENCE_TYPE_SUBMODEL;

	if(neuron->bias < (-3.0))
	{
		currentNeuronDispayReference->colour = DAT_FILE_COLOUR_MAGENTA;
	}
	else if(neuron->bias < (-0.8))
	{
		currentNeuronDispayReference->colour = DAT_FILE_COLOUR_RED;
	}
	else if(neuron->bias < -0.4)
	{
		currentNeuronDispayReference->colour = DAT_FILE_COLOUR_ORANGE;
	}
	else if(neuron->bias < -0.01)
	{
		currentNeuronDispayReference->colour = DAT_FILE_COLOUR_YELLOW;
	}
	else if(neuron->bias < 0.01)
	{
		currentNeuronDispayReference->colour = DAT_FILE_COLOUR_DARKGREY;
	}
	else if(neuron->bias < 0.4)
	{
		currentNeuronDispayReference->colour = DAT_FILE_COLOUR_GREEN;
	}
	else if(neuron->bias < 0.8)
	{
		currentNeuronDispayReference->colour = DAT_FILE_COLOUR_CYAN;
	}
	else if(neuron->bias < 3.0)
	{
		currentNeuronDispayReference->colour = DAT_FILE_COLOUR_BLUE;
	}
	else
	{
		currentNeuronDispayReference->colour = DAT_FILE_COLOUR_PURPLE;
	}

	if(hasSubnetNeuron)
	{
		currentNeuronDispayReference->relativePosition.x = (positionOfsubnetNeuron->x) + (double)(neuron->orderID)/((double)(neuron->subnetID)*3.0);
		currentNeuronDispayReference->relativePosition.y = (positionOfsubnetNeuron->y) + (double)(neuron->layerID)/((double)(neuron->subnetID)*3.0);
		currentNeuronDispayReference->relativePosition.z = (positionOfsubnetNeuron->z) + (double)(neuron->subnetID)/(4.0);
	}
	else
	{
		if(neuron->spatialCoordinatesSet)
		{
			currentNeuronDispayReference->relativePosition.x = (double)(neuron->xPosRel);
			currentNeuronDispayReference->relativePosition.y = (double)(neuron->layerID);
			currentNeuronDispayReference->relativePosition.z = (double)(neuron->yPosRel);
		}
		else
		{
			currentNeuronDispayReference->relativePosition.x = (double)(neuron->orderID);
			currentNeuronDispayReference->relativePosition.y = (double)(neuron->layerID);
			currentNeuronDispayReference->relativePosition.z = (double)(neuron->subnetID);
		}
	}

	currentNeuronDispayReference->name = SPRITE_SUBMODEL_NEURON_FILE_NAME;

	if(writeSVG)
	{
		//double scaleFactor = (NEURON_SPRITE_SCALE_FACTOR/((neuron->subnetID)*2.0)*(SPRITE_SUBMODEL_RANGE_SCALE_FACTOR*LDRAW_UNITS_PER_CM));
		//double width = scaleFactor;
		//double height = scaleFactor;
		double width = ANN_SVG_NEURON_SIZE;	//scaleFactor * ANN_SVG_SCALE_FACTOR
		double height = ANN_SVG_NEURON_SIZE;	//scaleFactor * ANN_SVG_SCALE_FACTOR

		vec positionSVG;
		positionSVG.x = currentNeuronDispayReference->relativePosition.x*ANN_SVG_SCALE_FACTOR + ANN_SVG_NEURON_SIZE/2;		//scaleFactor	//ANN_DRAW_BASICENTITY_NODE_WIDTH/2
		positionSVG.y = currentNeuronDispayReference->relativePosition.y*ANN_SVG_SCALE_FACTOR + ANN_SVG_NEURON_SIZE/2;
		positionSVG.z = ANN_OUTPUT_Z_POSITION_NODES;
		writeSVGbox(currentTag, &positionSVG, width, height, currentNeuronDispayReference->colour, 0.0, true);

	}

	return result;
}

//currently this function just adds a plain line between neuron1 and neuron2, in the future it could be more complex
bool ANNfillNeuronConnectionDisplayReference(Reference * currentNeuronDispayReference, Reference * backNeuronReference, Reference * forwardNeuronReference, NeuronConnection * neuronConnection, bool writeSVG, XMLparserTag ** currentTag)
{
	bool result = true;

	//add neuron connection sprite info
	currentNeuronDispayReference->type = REFERENCE_TYPE_LINE;


	if(neuronConnection->weight < (-3.0))
	{
		currentNeuronDispayReference->colour = DAT_FILE_COLOUR_MAGENTA;
	}
	else if(neuronConnection->weight < (-0.8))
	{
		currentNeuronDispayReference->colour = DAT_FILE_COLOUR_RED;
	}
	else if(neuronConnection->weight < -0.4)
	{
		currentNeuronDispayReference->colour = DAT_FILE_COLOUR_ORANGE;
	}
	else if(neuronConnection->weight < -0.01)
	{
		currentNeuronDispayReference->colour = DAT_FILE_COLOUR_YELLOW;
	}
	else if(neuronConnection->weight < 0.01)
	{
		currentNeuronDispayReference->colour = DAT_FILE_COLOUR_DARKGREY;
	}
	else if(neuronConnection->weight < 0.4)
	{
		currentNeuronDispayReference->colour = DAT_FILE_COLOUR_GREEN;
	}
	else if(neuronConnection->weight < 0.8)
	{
		currentNeuronDispayReference->colour = DAT_FILE_COLOUR_CYAN;
	}
	else if(neuronConnection->weight < 3.0)
	{
		currentNeuronDispayReference->colour = DAT_FILE_COLOUR_BLUE;
	}
	else
	{
		currentNeuronDispayReference->colour = DAT_FILE_COLOUR_PURPLE;
	}
	//currentNeuronDispayReference->colour = DAT_FILE_DEFAULT_COLOUR_EDGELINE;


	currentNeuronDispayReference->vertex1relativePosition = backNeuronReference->relativePosition;
	currentNeuronDispayReference->vertex2relativePosition = forwardNeuronReference->relativePosition;

	if(writeSVG)
	{
		vec position1SVG;
		vec position2SVG;
		position1SVG.x = currentNeuronDispayReference->vertex1relativePosition.x * ANN_SVG_SCALE_FACTOR;
		position1SVG.y = currentNeuronDispayReference->vertex1relativePosition.y * ANN_SVG_SCALE_FACTOR;
		position1SVG.z = ANN_OUTPUT_Z_POSITION_CONNECTIONS;
		position2SVG.x = currentNeuronDispayReference->vertex2relativePosition.x * ANN_SVG_SCALE_FACTOR;
		position2SVG.y = currentNeuronDispayReference->vertex2relativePosition.y * ANN_SVG_SCALE_FACTOR;
		position2SVG.z = ANN_OUTPUT_Z_POSITION_CONNECTIONS;

		writeSVGline(currentTag, &position1SVG, &position2SVG, currentNeuronDispayReference->colour);
	}

	return result;
}





bool ANNdetermineSpriteInfoForNeuronConnectionAndAddSpriteToSpriteRefList(Reference * neuronReference, NeuronConnectionContainer * neuronConnectionContainer, Reference * spriteListInitialReference, vec * eyeCoords, int * numSpritesAdded, char * sceneFileName, Reference * backNeuronReference, Reference * forwardNeuronReference, bool writeSVG, XMLparserTag ** currentTag)
{
	bool result = true;

	//add sprite info [textual + range info] to reference list:
	Reference * spriteSubmodelInitialReference = new Reference();
	Reference * spriteSubmodelCurrentReference = spriteSubmodelInitialReference;

	//create textual sprite info:
	string * spriteTextString = new string();

	int spriteNumberOfLines;
	spriteNumberOfLines = SPRITE_TEXTUAL_MAX_NUM_OF_LINES;
	int * spriteColourArray = new int[spriteNumberOfLines];
	ANNgenerateTextualNeuronConnectionSpriteInfoString(neuronConnectionContainer, spriteTextString, spriteColourArray);

	int numSpritesAddedTemp = 0;
	spriteSubmodelCurrentReference = LDaddTextualSpriteInfoStringToReferenceList(NULL, spriteTextString, spriteColourArray, spriteSubmodelCurrentReference, spriteNumberOfLines, &numSpritesAddedTemp, true);
	//CHECK THIS: Add svg text here
	delete spriteTextString;
	delete spriteColourArray;

	*numSpritesAdded = (*numSpritesAdded + 1);

	//generate sprite reference name
	char * spriteReferenceFileName = new char[DAT_FILE_REF_SUBMODEL_NAME_LENGTH_MAX];

	LDcreateSpriteReferenceName(spriteReferenceFileName, *numSpritesAdded, sceneFileName);
	cout << "spriteReferenceFileName = " << spriteReferenceFileName << endl;

#ifdef CPLUSPLUSERRORCORRECTION3
	char * CHICKENDEBUG = new char[DAT_FILE_REF_MAX_SIZE*DAT_FILE_MAX_NUM_OF_REFERENCES];
#endif

	//writeReferencesToFile
	if(!writeReferencesToFile(spriteReferenceFileName, spriteSubmodelInitialReference))
	{
		result = false;
	}

	delete spriteSubmodelInitialReference;

	int spriteDefaultColour = SPRITE_DEFAULT_COLOUR;

	vec centralPositionOfConnection;
	centralPositionOfConnection.x = (backNeuronReference->relativePosition.x + forwardNeuronReference->relativePosition.x)/2.0;
	centralPositionOfConnection.y = (backNeuronReference->relativePosition.y + forwardNeuronReference->relativePosition.y)/2.0;
	centralPositionOfConnection.z = (backNeuronReference->relativePosition.z + forwardNeuronReference->relativePosition.z)/2.0;

	double spriteScaleFactor = (0.00008*(SPRITE_SUBMODEL_RANGE_SCALE_FACTOR*LDRAW_UNITS_PER_CM));
	if(!LDaddSpriteToSpriteReferenceList(&centralPositionOfConnection, eyeCoords, spriteListInitialReference, spriteReferenceFileName, spriteDefaultColour, spriteScaleFactor))
	{
		result = false;
	}

	delete spriteReferenceFileName;

	return result;
}


bool ANNdetermineSpriteInfoForNeuronAndAddSpriteToSpriteRefList(Reference * neuronReference, NeuronContainer * neuron, Reference * spriteListInitialReference, vec * eyeCoords, int * numSpritesAdded, char * sceneFileName, bool writeSVG, XMLparserTag ** currentTag)
{
	bool result = true;

	//add sprite info [textual + range info] to reference list:
	Reference * spriteSubmodelInitialReference = new Reference();
	Reference * spriteSubmodelCurrentReference = spriteSubmodelInitialReference;

	//create textual sprite info:
	string * spriteTextString = new string();

	int spriteNumberOfLines;
	spriteNumberOfLines = SPRITE_TEXTUAL_MAX_NUM_OF_LINES;
	int * spriteColourArray = new int[spriteNumberOfLines];
	ANNgenerateTextualNeuronSpriteInfoString(neuron, spriteTextString, spriteColourArray);

	if(writeSVG)
	{
		int numberOfLines = 0;
		for(int i=0; i < spriteTextString->length(); i++)
		{
			if(spriteTextString->at(i) == CHAR_NEWLINE)
			{
				numberOfLines++;
			}
		}

		string stringCurrentLine = "";
		int stringCurrentLineIndex = 0;
		int lineIndex = 0;

		for(int i=0; i < spriteTextString->length(); i++)
		{
			if(spriteTextString->at(i) == CHAR_NEWLINE)
			{
				string stringCurrentLine = spriteTextString->substr(i-stringCurrentLineIndex, stringCurrentLineIndex);

				vec positionSVG;
				positionSVG.x = neuronReference->relativePosition.x*ANN_SVG_SCALE_FACTOR - ANN_SVG_NEURON_SIZE/2;
				positionSVG.y = neuronReference->relativePosition.y*ANN_SVG_SCALE_FACTOR - (numberOfLines+1)*ANN_SVG_SPRITE_TEXT_OFFSET_PER_LINE + lineIndex*ANN_SVG_SPRITE_TEXT_OFFSET_PER_LINE;
				positionSVG.z = ANN_OUTPUT_Z_POSITION_TEXT;
				writeSVGtext(currentTag, stringCurrentLine, &positionSVG, ANN_SVG_TEXT_SCALE_FACTOR, DAT_FILE_COLOUR_BLACK);

				stringCurrentLine = "";
				stringCurrentLineIndex = 0;
				lineIndex++;
			}
			else
			{
				stringCurrentLineIndex++;
			}
		}
	}


	int numSpritesAddedTemp = 0;
	spriteSubmodelCurrentReference = LDaddTextualSpriteInfoStringToReferenceList(NULL, spriteTextString, spriteColourArray, spriteSubmodelCurrentReference, spriteNumberOfLines, &numSpritesAddedTemp, true);

	*numSpritesAdded = (*numSpritesAdded + 1);

	delete spriteTextString;
	delete spriteColourArray;

	//generate sprite reference name
	char * spriteReferenceFileName = new char[DAT_FILE_REF_SUBMODEL_NAME_LENGTH_MAX];

	LDcreateSpriteReferenceName(spriteReferenceFileName, *numSpritesAdded, sceneFileName);
	cout << "spriteReferenceFileName = " << spriteReferenceFileName << endl;

#ifdef CPLUSPLUSERRORCORRECTION3
	char * CHICKENDEBUG = new char[DAT_FILE_REF_MAX_SIZE*DAT_FILE_MAX_NUM_OF_REFERENCES];
#endif

	//writeReferencesToFile
	if(!writeReferencesToFile(spriteReferenceFileName, spriteSubmodelInitialReference))
	{
		result = false;
	}

	delete spriteSubmodelInitialReference;

	int spriteDefaultColour = SPRITE_DEFAULT_COLOUR;

	double spriteScaleFactor = (0.00008*(SPRITE_SUBMODEL_RANGE_SCALE_FACTOR*LDRAW_UNITS_PER_CM));
	if(!LDaddSpriteToSpriteReferenceList(&(neuronReference->relativePosition), eyeCoords, spriteListInitialReference, spriteReferenceFileName, spriteDefaultColour, spriteScaleFactor))
	{
		result = false;
	}

	delete spriteReferenceFileName;

	return result;
}










void ANNgenerateTextualNeuronSpriteInfoString(NeuronContainer * neuronContainer, string * spriteTextString, int spriteColourArray[])
{
	/*Start Sprite Text Creation*/

	spriteColourArray[0] = SPRITE_TEXTUAL_INCLUDE_NEURON_ID_COLOUR;
	spriteColourArray[1] = SPRITE_TEXTUAL_INCLUDE_NEURON_ORDERID_COLOUR;
	spriteColourArray[2] = SPRITE_TEXTUAL_INCLUDE_NEURON_LAYERID_COLOUR;
	spriteColourArray[3] = SPRITE_TEXTUAL_INCLUDE_NEURON_SUBNETID_COLOUR;
	spriteColourArray[4] = SPRITE_TEXTUAL_INCLUDE_NEURON_BIAS_COLOUR;
	spriteColourArray[5] = SPRITE_TEXTUAL_INCLUDE_NEURON_OUTPUT_COLOUR;
	spriteColourArray[6] = SPRITE_TEXTUAL_INCLUDE_NEURON_CLASSTARGET_COLOUR;
	spriteColourArray[7] = SPRITE_TEXTUAL_INCLUDE_NEURON_ERROR_COLOUR;


	char * tempString = new char[DAT_FILE_DATA_VALUE_MAX_LENGTH*CPLUSPLUSERRORCORRECTION1];		//for some reason tempString must be very large or else the heap will get corrupted
	//char * tempString = new char[DAT_FILE_DATA_VALUE_MAX_LENGTH];

	/*Unit Name Information*/

	if(SPRITE_TEXTUAL_INCLUDE_NEURON_ID_INFO)
	{
		tempString[0] = '\0';
		sprintf(tempString, "%d", neuronContainer->neuron->id);
		if(SPRITE_TEXTUAL_INCLUDE_NEURON_EXTRA_SECONDARY_INFO)
		{
			#ifdef ANN_DEBUG
			/*
			//text kernel checking;
			if(neuronContainer->neuron->id == 1)
			{
				//test spritetext kernelling;
				*spriteTextString = *spriteTextString + "0123456789 10ABCDEFGHIJKLMNOPQRSTUVQXYZ .=-+ ID = " + tempString;
			}
			else
			{
			*/
			#endif
			*spriteTextString = *spriteTextString + "ID = " + tempString;

			#ifdef ANN_DEBUG
			/*
			//text kernel checking;
			}
			*/
			#endif
		}
		else
		{
			*spriteTextString = *spriteTextString + "        " + tempString;
		}
	}


	if(SPRITE_TEXTUAL_INCLUDE_NEURON_ORDERID_INFO)
	{
		*spriteTextString = *spriteTextString + '\n';
		tempString[0] = '\0';
		sprintf(tempString, "%d", neuronContainer->neuron->orderID);
		*spriteTextString = *spriteTextString + "orderID = " + tempString;
	}


	if(SPRITE_TEXTUAL_INCLUDE_NEURON_LAYERID_INFO)
	{
		*spriteTextString = *spriteTextString + '\n';
		tempString[0] = '\0';
		sprintf(tempString, "%d", neuronContainer->neuron->layerID);
		*spriteTextString = *spriteTextString + "layerID = " + tempString;
	}


	if(SPRITE_TEXTUAL_INCLUDE_NEURON_SUBNETID_INFO)
	{
		*spriteTextString = *spriteTextString + '\n';
		tempString[0] = '\0';
		sprintf(tempString, "%d", neuronContainer->neuron->subnetID);
		*spriteTextString = *spriteTextString + "subnetID = " + tempString;
	}


	if(SPRITE_TEXTUAL_INCLUDE_NEURON_BIAS_INFO)
	{
		*spriteTextString = *spriteTextString + '\n';
		tempString[0] = '\0';
		sprintf(tempString, "%0.2f", neuronContainer->neuron->bias);
		*spriteTextString = *spriteTextString + "bias = " + tempString;
	}


	if(SPRITE_TEXTUAL_INCLUDE_NEURON_OUTPUT_INFO)
	{
		*spriteTextString = *spriteTextString + '\n';
		tempString[0] = '\0';
		sprintf(tempString, "%0.2f", neuronContainer->neuron->output);
		*spriteTextString = *spriteTextString + "output = " + tempString;
	}

	if(SPRITE_TEXTUAL_INCLUDE_NEURON_CLASSTARGET_INFO)
	{
		*spriteTextString = *spriteTextString + '\n';
		tempString[0] = '\0';
		sprintf(tempString, "%0.2f", neuronContainer->neuron->classTarget);
		*spriteTextString = *spriteTextString + "classTarget = " + tempString;
	}

	if(SPRITE_TEXTUAL_INCLUDE_NEURON_ERROR_INFO)
	{
		*spriteTextString = *spriteTextString + '\n';
		tempString[0] = '\0';
		sprintf(tempString, "%0.2f", neuronContainer->neuron->error);
		*spriteTextString = *spriteTextString + "error = " + tempString;
	}

	delete tempString;

	#ifdef ANN_DEBUG
	//cout << "*spriteTextString = \n" << *spriteTextString << "\n\n" << endl;
	#endif
	
	/*End Start Sprite Text Creation*/
}


void ANNgenerateTextualNeuronConnectionSpriteInfoString(NeuronConnectionContainer * neuronConnectionContainer, string * spriteTextString, int spriteColourArray[])
{
	/*Start Sprite Text Creation*/

	spriteColourArray[0] = SPRITE_TEXTUAL_INCLUDE_NEURONCONNECTION_WEIGHT_COLOUR;
	spriteColourArray[1] = SPRITE_TEXTUAL_INCLUDE_NEURONCONNECTION_WEIGHT_COLOUR;
	spriteColourArray[2] = SPRITE_TEXTUAL_INCLUDE_NEURONCONNECTION_WEIGHT_COLOUR;
	spriteColourArray[3] = SPRITE_TEXTUAL_INCLUDE_NEURONCONNECTION_WEIGHT_COLOUR;

	char * tempString = new char[DAT_FILE_DATA_VALUE_MAX_LENGTH*CPLUSPLUSERRORCORRECTION1];		//for some reason tempString must be very large or else the heap will get corrupted
	//char * tempString = new char[DAT_FILE_DATA_VALUE_MAX_LENGTH];

	/*Unit Name Information*/

	tempString[0] = '\0';
	sprintf(tempString, "%0.2f", neuronConnectionContainer->neuronConnection->weight);
	*spriteTextString = "\n\n" + *spriteTextString + "w = " + tempString;

	delete tempString;

	#ifdef ANN_DEBUG
	//cout << "*spriteTextString = \n" << *spriteTextString << "\n\n" << endl;
	#endif
	
	/*End Start Sprite Text Creation*/
}




 /*******************************************************************************
 *
 * License: GNU Affero General Public License version 3 (AGPLv3)
 *
 *******************************************************************************/

/*******************************************************************************
 *
 * File Name: ANNdraw.cpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2022 Baxter AI (baxterai.com)
 * Project: Generic Construct Functions
 * Project Version: 3q1a 05-June-2022
 * Description: This code allows the addition of a sprite into a given scene file where a sprite is a paragraph of text. [The text is to be rendered in 3D, and point towards the user POV]
 * /
 *******************************************************************************/




#include "ANNdraw.hpp"



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
bool SPRITE_TEXTUAL_INCLUDE_ANNCONNECTION_INFO;
bool SPRITE_TEXTUAL_INCLUDE_NEURON_INFO;
bool SPRITE_TEXTUAL_INCLUDE_INFO;

void ANNdrawClass::fillInANNSpriteExternVariables()
{
	XMLrulesClass* currentReferenceRulesClass = ANNrulesSprite;

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
		SPRITE_TEXTUAL_INCLUDE_ANNCONNECTION_INFO = true;
	}
	else
	{
		SPRITE_TEXTUAL_INCLUDE_ANNCONNECTION_INFO = false;
	}
	if(SPRITE_TEXTUAL_INCLUDE_ANNCONNECTION_INFO || SPRITE_TEXTUAL_INCLUDE_NEURON_INFO)
	{
		SPRITE_TEXTUAL_INCLUDE_INFO = true;
	}
	else
	{
		SPRITE_TEXTUAL_INCLUDE_INFO = false;
	}

	LDsprite.fillInLDspriteExternVariables();
}


/*top level sprite routines - required for independent LRRCsprite.cpp calculations*/


bool ANNdrawClass::ANNcreateNeuralNetworkSceneFiles(string sceneFileName, ANNneuron* firstNeuronInNetwork, bool addSprites, bool writeSVG, XMLparserTag** currentTagSVG)
{
	bool result = true;
	
	LDreference* initialReference = new LDreference();

	int numSpritesAdded = 0;
	bool writeLDR = true;
	if(!ANNcreateNeuralNetworkReferenceLists(sceneFileName, initialReference, firstNeuronInNetwork, addSprites, &numSpritesAdded, writeSVG, currentTagSVG, writeLDR))
	{
		result = false;
	}
	if(writeLDR)
	{
		if(!ANNcreateNeuralNetworkSceneFilesFromReferenceLists(sceneFileName, addSprites, initialReference, numSpritesAdded))
		{
			result = false;
		}
	}
		
	delete initialReference;

	return result;
}



bool ANNdrawClass::ANNcreateNeuralNetworkReferenceLists(string sceneFileName, LDreference* initialReference, ANNneuron* firstNeuronInNetwork, bool addSprites, int* numSpritesAdded, bool writeSVG, XMLparserTag** currentTagSVG, bool writeLDR)
{
	bool result = true;

	vec eyeCoords;
	eyeCoords.x = 0.0;
	eyeCoords.y = 0.0;
	eyeCoords.z = 0.0;

	//cout << "ANNsearchNeuralNetworkAndCreateReferences" << endl;

	ANNsearchNeuralNetworkAndCreateReferences(firstNeuronInNetwork, initialReference, &eyeCoords, numSpritesAdded, sceneFileName, false, NULL, addSprites, writeSVG, currentTagSVG, writeLDR);

	#ifdef ANN_DRAW_PREVENT_REPRINT
	ANNsearchNeuralNetworkAndCreateReferencesReset(firstNeuronInNetwork);
	#endif

	return result;
}


bool ANNdrawClass::ANNcreateNeuralNetworkSceneFilesFromReferenceLists(string sceneFileName, bool addSprites, LDreference* initialReference, int numSpritesAdded)
{
	bool result = true;

	LDreferenceManipulation.writeReferencesToFile(sceneFileName, initialReference);

	return result;
}



LDreference* ANNdrawClass::ANNsearchNeuralNetworkAndCreateReferences(ANNneuron* firstNeuronInLayer, LDreference* currentListReference, vec* eyeCoords, int* numSpritesAdded, string sceneFileName, bool isSubnet, vec* positionOfSubnetNeuron, bool addSprites, bool writeSVG, XMLparserTag** currentTagSVG, bool writeLDR)
{
	bool result = true;

	//cout << "ANNsearchNeuralNetworkAndCreateReferences" << endl;
	
	ANNneuron* currentNeuron = firstNeuronInLayer;
	while(currentNeuron->nextNeuron != NULL)
	{	
		//cout << "currentNeuron->id = " << currentNeuron->id << ", currentNeuron->GIAentityName " << currentNeuron->GIAentityName << endl;
		
		#ifdef ANN_DRAW_PREVENT_REPRINT
		if(!(currentNeuron->printed))
		{
			currentNeuron->printed = true;
		#endif
		
			//cout << "currentNeuron->id = " << currentNeuron->id << endl;
			//cout << "currentNeuron->GIAentityName = " << currentNeuron->GIAentityName << endl;

			ANNfillNeuronDisplayReference(currentListReference, currentNeuron, isSubnet, positionOfSubnetNeuron, writeSVG, currentTagSVG);
			LDreference* currentNeuronReferenceOnLayer = currentListReference;

			LDreference* newNeuronDispayReference = new LDreference();
			currentListReference->next = newNeuronDispayReference;
			currentListReference = currentListReference->next;

			#ifndef ANN_DISPLAY_DISABLE_SPRITES
			if(addSprites)
			{
				if(SPRITE_TEXTUAL_INCLUDE_NEURON_INFO)
				{
					if(!ANNdetermineSpriteInfoForNeuronAndAddSpriteToSpriteRefList(currentNeuron, &currentListReference, eyeCoords, numSpritesAdded, sceneFileName, &(currentNeuronReferenceOnLayer->relativePosition), writeSVG, currentTagSVG, writeLDR))
					{
						result = false;
					}
				}
			}
			#endif

			if(currentNeuron->hasBackLayer == true)
			{
				for(vector<ANNneuronConnection*>::iterator connectionIter = currentNeuron->backANNneuronConnectionList.begin(); connectionIter != currentNeuron->backANNneuronConnectionList.end(); connectionIter++)
				{
					ANNneuronConnection* currentANNneuronConnection = *connectionIter;

					//regenerate reference properties for a neuron display reference in the previous layer (which is connected to the current neuron)
					
					LDreference currentNeuronReferenceOnPreviousLayer;
					ANNfillNeuronDisplayReference(&currentNeuronReferenceOnPreviousLayer, currentANNneuronConnection->backNeuron, isSubnet, positionOfSubnetNeuron, false, currentTagSVG);	//do not writeSVG for back layer neurons (as this will duplicate them)
					
					if(!ANNfillANNneuronConnectionDisplayReference(currentListReference, &currentNeuronReferenceOnPreviousLayer, currentNeuronReferenceOnLayer, currentANNneuronConnection, writeSVG, currentTagSVG))
					{
						result = false;
					}

					LDreference* newNeuronDispayReference = new LDreference();
					currentListReference->next = newNeuronDispayReference;
					currentListReference = currentListReference->next;

					#ifndef ANN_DISPLAY_DISABLE_SPRITES
					if(addSprites)
					{
						if(SPRITE_TEXTUAL_INCLUDE_ANNCONNECTION_INFO)
						{
							if(!ANNdetermineSpriteInfoForANNneuronConnectionAndAddSpriteToSpriteRefList(currentANNneuronConnection, &currentListReference, eyeCoords, numSpritesAdded, sceneFileName, &(currentNeuronReferenceOnPreviousLayer.relativePosition), &(currentNeuronReferenceOnLayer->relativePosition), writeSVG, currentTagSVG, writeLDR))
							{
								result = false;
							}
						}
					}
					#endif
				}
			}

			#ifdef ANN_SUBNETS
			if(currentNeuron->isSubnet)
			{
				//create back layer of subnet 1-1 connections for display only

				ANNneuron* currentNeuronInBackLayerOfSubnet = currentNeuron->firstNeuronInBackLayerOfSubnet;

				for(vector<ANNneuronConnection*>::iterator connectionIter = currentNeuron->backANNneuronConnectionList.begin(); connectionIter != currentNeuron->backANNneuronConnectionList.end(); connectionIter++)
				{
					ANNneuronConnection* currentANNneuronConnection = *connectionIter;

					//regenerate reference properties for a neuron display reference in the previous layer (which is connected to the current neuron)

					LDreference currentNeuronReferenceOnPreviousLayer;
					ANNfillNeuronDisplayReference(&currentNeuronReferenceOnPreviousLayer, currentANNneuronConnection->backNeuron, isSubnet, positionOfSubnetNeuron, writeSVG, currentTagSVG);

					LDreference currentNeuronReferenceOnBackLayerOfSubnet;
					ANNfillNeuronDisplayReference(&currentNeuronReferenceOnBackLayerOfSubnet, currentNeuronInBackLayerOfSubnet, true, &(currentNeuronReferenceOnLayer->relativePosition), writeSVG, currentTagSVG);

					ANNneuronConnection blankOneToOneANNneuronConnection;
					blankOneToOneANNneuronConnection.weight = 0.0;
					if(!ANNfillANNneuronConnectionDisplayReference(currentListReference, &currentNeuronReferenceOnBackLayerOfSubnet, &currentNeuronReferenceOnPreviousLayer, &blankOneToOneANNneuronConnection, writeSVG, currentTagSVG))
					{
						result = false;
					}

					LDreference* newNeuronDispayReference = new LDreference();
					currentListReference->next = newNeuronDispayReference;
					currentListReference = currentListReference->next;

					currentNeuronInBackLayerOfSubnet=currentNeuronInBackLayerOfSubnet->nextNeuron;
				}

				//create front layer of subnet 1-1 connections for display only
				ANNneuron* currentNeuronInFrontLayerOfSubnet = currentNeuron->firstNeuronInFrontLayerOfSubnet;

				for(vector<ANNneuronConnection*>::iterator connectionIter = currentNeuron->frontANNneuronConnectionList.begin(); connectionIter != currentNeuron->frontANNneuronConnectionList.end(); connectionIter++)
				{
					ANNneuronConnection* currentANNneuronConnection = *connectionIter;
					//regenerate reference properties for a neuron display reference in the previous layer (which is connected to the current neuron)

					LDreference currentNeuronReferenceOnNextLayer;
					ANNfillNeuronDisplayReference(&currentNeuronReferenceOnNextLayer, currentANNneuronConnection->frontNeuron, isSubnet, positionOfSubnetNeuron, writeSVG, currentTagSVG);

					LDreference currentNeuronReferenceOnFrontLayerOfSubnet;
					ANNfillNeuronDisplayReference(&currentNeuronReferenceOnFrontLayerOfSubnet, currentNeuronInFrontLayerOfSubnet, true, &(currentNeuronReferenceOnLayer->relativePosition), writeSVG, currentTagSVG);

					ANNneuronConnection blankOneToOneANNneuronConnection;
					blankOneToOneANNneuronConnection.weight = 0.0;
					if(!ANNfillANNneuronConnectionDisplayReference(currentListReference, &currentNeuronReferenceOnFrontLayerOfSubnet, &currentNeuronReferenceOnNextLayer, &blankOneToOneANNneuronConnection, writeSVG, currentTagSVG))
					{
						result = false;
					}

					LDreference* newNeuronDispayReference = new LDreference();
					currentListReference->next = newNeuronDispayReference;
					currentListReference = currentListReference->next;

					currentNeuronInFrontLayerOfSubnet=currentNeuronInFrontLayerOfSubnet->nextNeuron;
				}

				currentListReference = ANNsearchNeuralNetworkAndCreateReferences(currentNeuron->firstNeuronInBackLayerOfSubnet, currentListReference, eyeCoords, numSpritesAdded, sceneFileName, (true|isSubnet), &(currentNeuronReferenceOnLayer->relativePosition), addSprites, writeSVG, currentTagSVG, writeLDR);
			}
			#endif

			#ifdef ANN_DRAW_DYNAMIC
			if(currentNeuron->hasFrontLayer)
			{
				for(vector<ANNneuronConnection*>::iterator connectionIter = currentNeuron->frontANNneuronConnectionList.begin(); connectionIter != currentNeuron->frontANNneuronConnectionList.end(); connectionIter++)
				{
					ANNneuronConnection* currentANNneuronConnection = *connectionIter;
					currentListReference = ANNsearchNeuralNetworkAndCreateReferences(currentANNneuronConnection->frontNeuron, currentListReference, eyeCoords, numSpritesAdded, sceneFileName, isSubnet, positionOfSubnetNeuron, addSprites, writeSVG, currentTagSVG, writeLDR);
				}
			}
			#endif

		#ifdef ANN_DRAW_PREVENT_REPRINT
		}
		#endif

		currentNeuron = currentNeuron->nextNeuron;
	}
	
	#ifndef ANN_DRAW_DYNAMIC
	if(firstNeuronInLayer->hasFrontLayer)
	{
		currentListReference = ANNsearchNeuralNetworkAndCreateReferences(firstNeuronInLayer->firstNeuronInFrontLayer, currentListReference, eyeCoords, numSpritesAdded, sceneFileName, isSubnet, positionOfSubnetNeuron, addSprites, writeSVG, currentTagSVG, writeLDR);
	}
	#endif
	
	//cout << "ANNsearchNeuralNetworkAndCreateReferences end" << endl;

	return currentListReference;

}


#ifdef ANN_DRAW_PREVENT_REPRINT
void ANNdrawClass::ANNsearchNeuralNetworkAndCreateReferencesReset(ANNneuron* firstNeuronInLayer)
{
	ANNneuron* currentNeuron = firstNeuronInLayer;
	while(currentNeuron->nextNeuron != NULL)
	{
		if(currentNeuron->printed)
		{
			currentNeuron->printed = false;
			#ifdef ANN_DRAW_DYNAMIC
			if(currentNeuron->hasFrontLayer)
			{
				for(vector<ANNneuronConnection*>::iterator connectionIter = currentNeuron->frontANNneuronConnectionList.begin(); connectionIter != currentNeuron->frontANNneuronConnectionList.end(); connectionIter++)
				{
					ANNneuronConnection* currentANNneuronConnection = *connectionIter;
					ANNsearchNeuralNetworkAndCreateReferencesReset(currentANNneuronConnection->frontNeuron);
				}
			}
			#endif
		}

		currentNeuron = currentNeuron->nextNeuron;
	}
	
	#ifndef ANN_DRAW_DYNAMIC
	if(firstNeuronInLayer->hasFrontLayer)
	{
		ANNsearchNeuralNetworkAndCreateReferencesReset(firstNeuronInLayer->firstNeuronInFrontLayer);
	}
	#endif

}
#endif














































/*medium level sprite routines - these can be used by LRRCsprite.cpp top level routines or by LRRCgame.cpp routines*/

bool ANNdrawClass::ANNfillNeuronDisplayReference(LDreference* currentNeuronDispayReference, ANNneuron* neuron, bool hasSubnetNeuron, vec* positionOfsubnetNeuron, bool writeSVG, XMLparserTag** currentTagSVG)
{
	bool result = true;

	mat currentDeformationMatrix;

	//add neuron range info [green ring/sphere/cylinder]

	SHAREDvector.createIdentityMatrix(&currentDeformationMatrix);
	SHAREDvector.scaleMatrix(&currentDeformationMatrix, (NEURON_SPRITE_SCALE_FACTOR/((neuron->subnetID)*2.0)*(SPRITE_SUBMODEL_RANGE_SCALE_FACTOR*LDRAW_UNITS_PER_CM)));		//	SHAREDvector.scaleMatrix(&currentDeformationMatrix, ((neuron->bias)*(SPRITE_SUBMODEL_RANGE_SCALE_FACTOR*LDRAW_UNITS_PER_CM)));
	SHAREDvector.copyMatrixTwoIntoMatrixOne(&(currentNeuronDispayReference->deformationMatrix), &(currentDeformationMatrix));

	currentNeuronDispayReference->type = REFERENCE_TYPE_SUBMODEL;

	#ifdef ANN_ALGORITHM_CLASSIFICATION_NETWORK
	if(neuron->memoryTrace > 1000)
	{
		currentNeuronDispayReference->colour = SHARED_COLOUR_RED;
	}
	else if(neuron->memoryTrace > 100)
	{
		currentNeuronDispayReference->colour = SHARED_COLOUR_ORANGE;
	}
	else if(neuron->memoryTrace > 25)
	{
		currentNeuronDispayReference->colour = SHARED_COLOUR_YELLOW;
	}
	else if(neuron->memoryTrace > 10)
	{
		currentNeuronDispayReference->colour = SHARED_COLOUR_GREEN;
	}
	else if(neuron->memoryTrace > 4)
	{
		currentNeuronDispayReference->colour = SHARED_COLOUR_CYAN;
	}
	else if(neuron->memoryTrace > 2)
	{
		currentNeuronDispayReference->colour = SHARED_COLOUR_BLUE;
	}
	else if(neuron->memoryTrace > 1)
	{
		currentNeuronDispayReference->colour = SHARED_COLOUR_PURPLE;
	}
	else
	{
		currentNeuronDispayReference->colour = SHARED_COLOUR_MAGENTA;
	}
	#else
	if(neuron->bias < (-3.0))
	{
		currentNeuronDispayReference->colour = SHARED_COLOUR_MAGENTA;
	}
	else if(neuron->bias < (-0.8))
	{
		currentNeuronDispayReference->colour = SHARED_COLOUR_RED;
	}
	else if(neuron->bias < -0.4)
	{
		currentNeuronDispayReference->colour = SHARED_COLOUR_ORANGE;
	}
	else if(neuron->bias < -0.01)
	{
		currentNeuronDispayReference->colour = SHARED_COLOUR_YELLOW;
	}
	else if(neuron->bias < 0.01)
	{
		currentNeuronDispayReference->colour = SHARED_COLOUR_DARKGREY;
	}
	else if(neuron->bias < 0.4)
	{
		currentNeuronDispayReference->colour = SHARED_COLOUR_GREEN;
	}
	else if(neuron->bias < 0.8)
	{
		currentNeuronDispayReference->colour = SHARED_COLOUR_CYAN;
	}
	else if(neuron->bias < 3.0)
	{
		currentNeuronDispayReference->colour = SHARED_COLOUR_BLUE;
	}
	else
	{
		currentNeuronDispayReference->colour = SHARED_COLOUR_PURPLE;
	}
	#endif

	if(hasSubnetNeuron)
	{
		currentNeuronDispayReference->relativePosition.x = (positionOfsubnetNeuron->x) + (double)(neuron->orderID)/((double)(neuron->subnetID)*3.0);
		currentNeuronDispayReference->relativePosition.y = (positionOfsubnetNeuron->y) + (double)(neuron->layerID)/((double)(neuron->subnetID)*3.0);
		currentNeuronDispayReference->relativePosition.z = (positionOfsubnetNeuron->z) + (double)(neuron->subnetID)/(4.0);
	}
	else
	{
		if(neuron->spatialCoordinatesSet2D)
		{
			currentNeuronDispayReference->relativePosition.x = (double)(neuron->xPosRel);
			currentNeuronDispayReference->relativePosition.y = (double)(neuron->layerID);
			currentNeuronDispayReference->relativePosition.z = (double)(neuron->yPosRel);
		}
		else if(neuron->spatialCoordinatesSet3D)
		{
			currentNeuronDispayReference->relativePosition.x = (double)(neuron->xPosRel);
			currentNeuronDispayReference->relativePosition.y = (double)(neuron->yPosRel);
			currentNeuronDispayReference->relativePosition.z = (double)(neuron->zPosRel);		
		}
		#ifdef ANN_ALGORITHM_CLASSIFICATION_NETWORK
		else if(neuron->spatialCoordinatesSetClassification)
		{
			currentNeuronDispayReference->relativePosition.x = (neuron->xPosRelFrac);
			currentNeuronDispayReference->relativePosition.y = (neuron->yPosRelFrac);
			currentNeuronDispayReference->relativePosition.z = (neuron->zPosRelFrac);
		}
		#endif
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
		double width = ANN_SVG_NEURON_SIZE;	//scaleFactor* ANN_SVG_SCALE_FACTOR
		double height = ANN_SVG_NEURON_SIZE;	//scaleFactor* ANN_SVG_SCALE_FACTOR

		vec positionSVG;
		positionSVG.x = currentNeuronDispayReference->relativePosition.x*ANN_SVG_SCALE_FACTOR + ANN_SVG_NEURON_SIZE/2;		//scaleFactor	//ANN_DRAW_BASICENTITY_NODE_WIDTH/2
		positionSVG.y = currentNeuronDispayReference->relativePosition.y*ANN_SVG_SCALE_FACTOR + ANN_SVG_NEURON_SIZE/2;
		positionSVG.z = ANN_OUTPUT_Z_POSITION_NODES;
		
		colour colourrgb;
		
		#ifdef ANN_ALGORITHM_SANI_SEQUENCE_GRAMMAR_NETWORK_PRINT_COLOURS_POS
		int col = SHARED_COLOUR_BLUE;
		if(neuron->SANIentityType == LRP_SHARED_ENTITY_TYPE_UNDEFINED)
		{
			col = LRP_SHARED_ENTITY_TYPE_UNDEFINED_COLOUR;
		}
		else if(neuron->SANIentityType == LRP_SHARED_ENTITY_TYPE_NETWORK_INDEX)
		{
			col = LRP_SHARED_ENTITY_TYPE_NETWORK_INDEX_COLOUR;
		}
		else if(neuron->SANIentityType == LRP_SHARED_ENTITY_TYPE_SUBSTANCE)
		{
			col = LRP_SHARED_ENTITY_TYPE_SUBSTANCE_COLOUR;
		}
		else if(neuron->SANIentityType == LRP_SHARED_ENTITY_TYPE_CONCEPT)
		{
			col = LRP_SHARED_ENTITY_TYPE_CONCEPT_COLOUR;
		}
		else if(neuron->SANIentityType == LRP_SHARED_ENTITY_TYPE_ACTION)
		{
			col = LRP_SHARED_ENTITY_TYPE_ACTION_COLOUR;
		}
		else if(neuron->SANIentityType == LRP_SHARED_ENTITY_TYPE_CONDITION)
		{
			col = LRP_SHARED_ENTITY_TYPE_CONDITION_COLOUR;
		}
		else if(neuron->SANIentityType == LRP_SHARED_ENTITY_TYPE_PROPERTY)
		{
			col = LRP_SHARED_ENTITY_TYPE_PROPERTY_COLOUR;
		}
		else if(neuron->SANIentityType == LRP_SHARED_ENTITY_TYPE_DEFINITION)
		{
			col = LRP_SHARED_ENTITY_TYPE_DEFINITION_COLOUR;
		}
		else if(neuron->SANIentityType == LRP_SHARED_ENTITY_TYPE_QUALITY)
		{
			col = LRP_SHARED_ENTITY_TYPE_QUALITY_COLOUR;
		}
		else if(neuron->SANIentityType == LRP_SHARED_ENTITY_TYPE_LOGIC_REFERENCE)
		{
			col = LRP_SHARED_ENTITY_TYPE_LOGIC_REFERENCE_COLOUR;
		}
 		LDreferenceClassObject.convertLdrawColourToDatFileRGB(col, &colourrgb);
		#else
		#ifdef ANN_ALGORITHM_SANI_SEQUENCE_GRAMMAR_NETWORK_PRINT_COLOURS_LAST_GENERATED_SENTENCE
		if(neuron->SANIgeneratedForLastSentence)
		{
			LDreferenceClassObject.convertLdrawColourToDatFileRGB(ANN_ALGORITHM_SANI_SEQUENCE_GRAMMAR_NETWORK_PRINT_COLOURS_LAST_GENERATED_SENTENCE_COLOR, &colourrgb);
		}
		else
		{
			LDreferenceClassObject.convertLdrawColourToDatFileRGB(ANN_ALGORITHM_SANI_SEQUENCE_GRAMMAR_NETWORK_PRINT_COLOURS_ACTIVE_LEVEL_INACTIVE_COLOR, &colourrgb);
		}
		#else
		#ifdef ANN_ALGORITHM_SANI_SEQUENCE_GRAMMAR_NETWORK_PRINT_COLOURS_ACTIVE
		int col = SHARED_COLOUR_BLUE;
		if(neuron->activationLevel == ANN_ALGORITHM_SANI_SEQUENCE_GRAMMAR_NETWORK_PRINT_COLOURS_ACTIVE_LEVEL_INACTIVE)
		{
			col = ANN_ALGORITHM_SANI_SEQUENCE_GRAMMAR_NETWORK_PRINT_COLOURS_ACTIVE_LEVEL_INACTIVE_COLOR;
		}
		else if(neuron->activationLevel == ANN_ALGORITHM_SANI_SEQUENCE_GRAMMAR_NETWORK_PRINT_COLOURS_ACTIVE_LEVEL_PARTIAL)
		{
			col = ANN_ALGORITHM_SANI_SEQUENCE_GRAMMAR_NETWORK_PRINT_COLOURS_ACTIVE_LEVEL_PARTIAL_COLOR;
		}
		else if(neuron->activationLevel == ANN_ALGORITHM_SANI_SEQUENCE_GRAMMAR_NETWORK_PRINT_COLOURS_ACTIVE_LEVEL_FULL)
		{
			col = ANN_ALGORITHM_SANI_SEQUENCE_GRAMMAR_NETWORK_PRINT_COLOURS_ACTIVE_LEVEL_FULL_COLOR;
		}
 		LDreferenceClassObject.convertLdrawColourToDatFileRGB(col, &colourrgb);
		#else
		#ifdef ANN_ALGORITHM_SANI_SEQUENCE_GRAMMAR_NETWORK_WEIGHTS_PRINT_COLOURS
		colourrgb = convertDoubleToRainbow(neuron->SANIneuronStrength, ANN_ALGORITHM_SANI_SEQUENCE_GRAMMAR_NETWORK_WEIGHTS_PRINT_COLOURS_NEURON_MAX_WEIGHT);
		#else
		LDreferenceClassObject.convertLdrawColourToDatFileRGB(currentNeuronDispayReference->colour, &colourrgb);
		#endif
		#endif
		#endif
		#endif

		
		bool transparent = false;
		double fillOpacityNOTUSED;
		LDsvg.writeSVGbox3(currentTagSVG, &positionSVG, width, height, colourrgb, 0.0, true, transparent, fillOpacityNOTUSED);
	}

	return result;
}



//currently this function just adds a plain line between neuron1 and neuron2, in the future it could be more complex
bool ANNdrawClass::ANNfillANNneuronConnectionDisplayReference(LDreference* currentNeuronDispayReference, LDreference* backNeuronReference, LDreference* forwardNeuronReference, ANNneuronConnection* ANNneuronConnection, bool writeSVG, XMLparserTag** currentTagSVG)
{
	bool result = true;

	//add neuron connection sprite info
	currentNeuronDispayReference->type = REFERENCE_TYPE_LINE;

	#ifdef ANN_ALGORITHM_CLASSIFICATION_NETWORK
	if(ANNneuronConnection->idealValue > 1.0)
	{
		currentNeuronDispayReference->colour = SHARED_COLOUR_RED;
	}
	if(ANNneuronConnection->idealValue > 0.8)
	{
		currentNeuronDispayReference->colour = SHARED_COLOUR_ORANGE;
	}
	else if(ANNneuronConnection->idealValue > 0.6)
	{
		currentNeuronDispayReference->colour = SHARED_COLOUR_YELLOW;
	}
	else if(ANNneuronConnection->idealValue > 0.4)
	{
		currentNeuronDispayReference->colour = SHARED_COLOUR_GREEN;
	}
	else if(ANNneuronConnection->idealValue > 0.3)
	{
		currentNeuronDispayReference->colour = SHARED_COLOUR_CYAN;
	}
	else if(ANNneuronConnection->idealValue > 0.2)
	{
		currentNeuronDispayReference->colour = SHARED_COLOUR_BLUE;
	}
	else if(ANNneuronConnection->idealValue > 0.1)
	{
		currentNeuronDispayReference->colour = SHARED_COLOUR_PURPLE;
	}
	else
	{
		currentNeuronDispayReference->colour = SHARED_COLOUR_MAGENTA;
	}
	#else
	if(ANNneuronConnection->weight < (-3.0))
	{
		currentNeuronDispayReference->colour = SHARED_COLOUR_MAGENTA;
	}
	else if(ANNneuronConnection->weight < (-0.8))
	{
		currentNeuronDispayReference->colour = SHARED_COLOUR_RED;
	}
	else if(ANNneuronConnection->weight < -0.4)
	{
		currentNeuronDispayReference->colour = SHARED_COLOUR_ORANGE;
	}
	else if(ANNneuronConnection->weight < -0.01)
	{
		currentNeuronDispayReference->colour = SHARED_COLOUR_YELLOW;
	}
	else if(ANNneuronConnection->weight < 0.01)
	{
		currentNeuronDispayReference->colour = SHARED_COLOUR_DARKGREY;
	}
	else if(ANNneuronConnection->weight < 0.4)
	{
		currentNeuronDispayReference->colour = SHARED_COLOUR_GREEN;
	}
	else if(ANNneuronConnection->weight < 0.8)
	{
		currentNeuronDispayReference->colour = SHARED_COLOUR_CYAN;
	}
	else if(ANNneuronConnection->weight < 3.0)
	{
		currentNeuronDispayReference->colour = SHARED_COLOUR_BLUE;
	}
	else
	{
		currentNeuronDispayReference->colour = SHARED_COLOUR_PURPLE;
	}
	#endif
	//currentNeuronDispayReference->colour = DAT_FILE_DEFAULT_COLOUR_EDGELINE;


	currentNeuronDispayReference->vertex1relativePosition = backNeuronReference->relativePosition;
	currentNeuronDispayReference->vertex2relativePosition = forwardNeuronReference->relativePosition;

	if(writeSVG)
	{
		vec position1SVG;
		vec position2SVG;
		position1SVG.x = currentNeuronDispayReference->vertex1relativePosition.x* ANN_SVG_SCALE_FACTOR;
		position1SVG.y = currentNeuronDispayReference->vertex1relativePosition.y* ANN_SVG_SCALE_FACTOR;
		position1SVG.z = ANN_OUTPUT_Z_POSITION_CONNECTIONS;
		position2SVG.x = currentNeuronDispayReference->vertex2relativePosition.x* ANN_SVG_SCALE_FACTOR;
		position2SVG.y = currentNeuronDispayReference->vertex2relativePosition.y* ANN_SVG_SCALE_FACTOR;
		position2SVG.z = ANN_OUTPUT_Z_POSITION_CONNECTIONS;

		#ifdef ANN_ALGORITHM_SANI_SEQUENCE_GRAMMAR_NETWORK_PRINT_COLOURS	
		#ifdef ANN_ALGORITHM_SANI_SEQUENCE_GRAMMAR_NETWORK_PRINT_COLOURS_POS
		int colorIndex;
		if(ANNneuronConnection->SANIrefSetConnectionType == LRP_SHARED_ENTITY_CONNECTION_TYPE_REFERENCE)	//ANN_ALGORITHM_SANI_SEQUENCE_GRAMMAR_NETWORK_PRINT_COLOURS_POS_CONNECTION_IDENTITY_REFSET
		{
			colorIndex = LRP_SHARED_ENTITY_CONNECTION_TYPE_REFERENCE_COLOUR;	//ANN_ALGORITHM_SANI_SEQUENCE_GRAMMAR_NETWORK_PRINT_COLOURS_POS_CONNECTION_IDENTITY_REFSET_COLOR;
		}
		if(ANNneuronConnection->SANIrefSetConnectionType == LRP_SHARED_ENTITY_CONNECTION_TYPE_SAMEREFERENCESET)	//ANN_ALGORITHM_SANI_SEQUENCE_GRAMMAR_NETWORK_PRINT_COLOURS_POS_CONNECTION_REFSET_NORMAL
		{
			colorIndex = LRP_SHARED_ENTITY_CONNECTION_TYPE_SAMEREFERENCESET_COLOUR;	//ANN_ALGORITHM_SANI_SEQUENCE_GRAMMAR_NETWORK_PRINT_COLOURS_POS_CONNECTION_REFSET_NORMAL_COLOR;
		}
		if(ANNneuronConnection->SANIrefSetConnectionType == LRP_SHARED_ENTITY_CONNECTION_TYPE_DIFFREFERENCESET)	//ANN_ALGORITHM_SANI_SEQUENCE_GRAMMAR_NETWORK_PRINT_COLOURS_POS_CONNECTION_REFSET_DELIMITER
		{
			colorIndex = LRP_SHARED_ENTITY_CONNECTION_TYPE_DIFFREFERENCESET_COLOUR;	//ANN_ALGORITHM_SANI_SEQUENCE_GRAMMAR_NETWORK_PRINT_COLOURS_POS_CONNECTION_REFSET_DELIMITER_COLOR;
		}
		else if(ANNneuronConnection->SANIrefSetConnectionType == LRP_SHARED_ENTITY_CONNECTION_TYPE_UNDEFINED)	//ANN_ALGORITHM_SANI_SEQUENCE_GRAMMAR_NETWORK_PRINT_COLOURS_POS_UNKNOWN
		{
			colorIndex = LRP_SHARED_ENTITY_CONNECTION_TYPE_UNDEFINED_COLOUR;	//ANN_ALGORITHM_SANI_SEQUENCE_GRAMMAR_NETWORK_PRINT_COLOURS_POS_CONNECTION_UNKNOWN_COLOR;
		}		
		LDsvg.writeSVGline(currentTagSVG, &position1SVG, &position2SVG, colorIndex);		
		#else
		#ifdef ANN_ALGORITHM_SANI_SEQUENCE_GRAMMAR_NETWORK_PRINT_COLOURS_COMPONENT_ORDER
		#ifdef ANN_ALGORITHM_SANI_SEQUENCE_GRAMMAR_NETWORK_PRINT_COLOURS_COMPONENT_ORDER_EXACT
		colour colourrgb;
		if(ANNneuronConnection->SANIcomponentIndexFirst)
		{
			//for white background;
			//col = ANN_ALGORITHM_SANI_SEQUENCE_GRAMMAR_NETWORK_PRINT_COLOURS_COMPONENT_ORDER_EXACT_FIRST_COLOR;
			
			//for black background (magenta [bright purple/violet]);
			colourrgb.r = ANN_ALGORITHM_SANI_SEQUENCE_GRAMMAR_NETWORK_PRINT_COLOURS_COMPONENT_ORDER_EXACT_FIRST_COLOR_R;
			colourrgb.g = ANN_ALGORITHM_SANI_SEQUENCE_GRAMMAR_NETWORK_PRINT_COLOURS_COMPONENT_ORDER_EXACT_FIRST_COLOR_G;
			colourrgb.b = ANN_ALGORITHM_SANI_SEQUENCE_GRAMMAR_NETWORK_PRINT_COLOURS_COMPONENT_ORDER_EXACT_FIRST_COLOR_B;
		}
		else
		{
			//for white background;
			//col = ANN_ALGORITHM_SANI_SEQUENCE_GRAMMAR_NETWORK_PRINT_COLOURS_COMPONENT_ORDER_EXACT_LAST_COLOR;
			
			//for black background (dark purple/indigo);
			colourrgb.r = ANN_ALGORITHM_SANI_SEQUENCE_GRAMMAR_NETWORK_PRINT_COLOURS_COMPONENT_ORDER_EXACT_LAST_COLOR_R;
			colourrgb.g = ANN_ALGORITHM_SANI_SEQUENCE_GRAMMAR_NETWORK_PRINT_COLOURS_COMPONENT_ORDER_EXACT_LAST_COLOR_G;
			colourrgb.b = ANN_ALGORITHM_SANI_SEQUENCE_GRAMMAR_NETWORK_PRINT_COLOURS_COMPONENT_ORDER_EXACT_LAST_COLOR_B;
		}
		LDsvg.writeSVGline(currentTagSVG, &position1SVG, &position2SVG, colourrgb);
		#else
		colour colourrgb = convertDoubleToRainbow(ANNneuronConnection->SANIcomponentIndex, ANN_ALGORITHM_SANI_SEQUENCE_GRAMMAR_NETWORK_PRINT_COLOURS_COMPONENT_ORDER_MAX_NUM_COMPONENTS);
		LDsvg.writeSVGline(currentTagSVG, &position1SVG, &position2SVG, colourrgb);
		#endif
		#else
		#ifdef ANN_ALGORITHM_SANI_SEQUENCE_GRAMMAR_NETWORK_WEIGHTS_PRINT_COLOURS		
		colour colourrgb = convertDoubleToRainbow(ANNneuronConnection->SANIconnectionStrength, ANN_ALGORITHM_SANI_SEQUENCE_GRAMMAR_NETWORK_WEIGHTS_PRINT_COLOURS_CONNECTION_MAX_WEIGHT);
		LDsvg.writeSVGline(currentTagSVG, &position1SVG, &position2SVG, colourrgb);
		#endif
		#endif
		#endif
		#else
		LDsvg.writeSVGline(currentTagSVG, &position1SVG, &position2SVG, currentNeuronDispayReference->colour);
		#endif
	}

	return result;
}



bool ANNdrawClass::ANNdetermineSpriteInfoForNeuronAndAddSpriteToSpriteRefList(ANNneuron* neuron, LDreference** currentListReference, vec* eyeCoords, int* numSpritesAdded, string sceneFileName, vec* neuronReferenceRelativePosition, bool writeSVG, XMLparserTag** currentTagSVG, bool writeLDR)
{
	bool result = true;

	//add sprite info [textual + range info] to reference list:
	LDreference* spriteSubmodelInitialReference = new LDreference();
	LDreference* spriteSubmodelCurrentReference = spriteSubmodelInitialReference;

	//create textual sprite info:
	string spriteTextString = "";

	int spriteNumberOfLines;
	spriteNumberOfLines = SPRITE_TEXTUAL_MAX_NUM_OF_LINES;
	int spriteColourArray[SPRITE_TEXTUAL_MAX_NUM_OF_LINES];
	ANNgenerateTextualNeuronSpriteInfoString(neuron, &spriteTextString, spriteColourArray);

	writeSpriteTextToSVG(&spriteTextString, writeSVG, currentTagSVG, neuronReferenceRelativePosition);
	int numSpritesAddedTemp = 0;
	spriteSubmodelCurrentReference = LDsprite.LDaddTextualSpriteInfoStringToReferenceList(NULL, spriteTextString, spriteColourArray, spriteSubmodelCurrentReference, spriteNumberOfLines, &numSpritesAddedTemp, true);

	*numSpritesAdded = (*numSpritesAdded + 1);

	//generate sprite reference name
	string spriteReferenceFileName = LDsprite.LDcreateSpriteReferenceName(*numSpritesAdded, sceneFileName);

	if(writeLDR)
	{
		#ifndef ANN_DRAW_DISABLE_FILE_OUTPUT_NOTIFICATIONS
		cout << "spriteReferenceFileName = " << spriteReferenceFileName << endl;
		#endif
		if(!LDreferenceManipulation.writeReferencesToFile(spriteReferenceFileName, spriteSubmodelInitialReference))
		{
			result = false;
		}
	}

	delete spriteSubmodelInitialReference;

	int spriteDefaultColour = SPRITE_DEFAULT_COLOUR;

	double spriteScaleFactor = (0.00008*(SPRITE_SUBMODEL_RANGE_SCALE_FACTOR*LDRAW_UNITS_PER_CM));
	if(!ANNaddSpriteToSpriteReferenceList(neuronReferenceRelativePosition, eyeCoords, currentListReference, spriteReferenceFileName, spriteDefaultColour, spriteScaleFactor))
	{
		result = false;
	}

	return result;
}

bool ANNdrawClass::ANNdetermineSpriteInfoForANNneuronConnectionAndAddSpriteToSpriteRefList(ANNneuronConnection* ANNneuronConnection, LDreference** currentListReference, vec* eyeCoords, int* numSpritesAdded, string sceneFileName, vec* backNeuronReferenceRelativePosition, vec* forwardNeuronReferenceRelativePosition, bool writeSVG, XMLparserTag** currentTagSVG, bool writeLDR)
{
	bool result = true;

	//add sprite info [textual + range info] to reference list:
	LDreference* spriteSubmodelInitialReference = new LDreference();
	LDreference* spriteSubmodelCurrentReference = spriteSubmodelInitialReference;

	vec centralPositionOfConnection;
	centralPositionOfConnection.x = (backNeuronReferenceRelativePosition->x + forwardNeuronReferenceRelativePosition->x)/2.0;
	centralPositionOfConnection.y = (backNeuronReferenceRelativePosition->y + forwardNeuronReferenceRelativePosition->y)/2.0;
	centralPositionOfConnection.z = (backNeuronReferenceRelativePosition->z + forwardNeuronReferenceRelativePosition->z)/2.0;

	//create textual sprite info:
	string spriteTextString = "";

	int spriteNumberOfLines;
	spriteNumberOfLines = SPRITE_TEXTUAL_MAX_NUM_OF_LINES;
	int spriteColourArray[SPRITE_TEXTUAL_MAX_NUM_OF_LINES];
	ANNgenerateTextualANNneuronConnectionSpriteInfoString(ANNneuronConnection, &spriteTextString, spriteColourArray);

	writeSpriteTextToSVG(&spriteTextString, writeSVG, currentTagSVG, &centralPositionOfConnection);
	int numSpritesAddedTemp = 0;
	spriteSubmodelCurrentReference = LDsprite.LDaddTextualSpriteInfoStringToReferenceList(NULL, spriteTextString, spriteColourArray, spriteSubmodelCurrentReference, spriteNumberOfLines, &numSpritesAddedTemp, true);

	*numSpritesAdded = (*numSpritesAdded + 1);

	//generate sprite reference name
	string spriteReferenceFileName = LDsprite.LDcreateSpriteReferenceName(*numSpritesAdded, sceneFileName);

	if(writeLDR)
	{
		#ifndef ANN_DRAW_DISABLE_FILE_OUTPUT_NOTIFICATIONS
		cout << "spriteReferenceFileName = " << spriteReferenceFileName << endl;
		#endif
		//writeReferencesToFile
		if(!LDreferenceManipulation.writeReferencesToFile(spriteReferenceFileName, spriteSubmodelInitialReference))
		{
			result = false;
		}
	}

	delete spriteSubmodelInitialReference;

	int spriteDefaultColour = SPRITE_DEFAULT_COLOUR;

	double spriteScaleFactor = (0.00008*(SPRITE_SUBMODEL_RANGE_SCALE_FACTOR*LDRAW_UNITS_PER_CM));
	if(!ANNaddSpriteToSpriteReferenceList(&centralPositionOfConnection, eyeCoords, currentListReference, spriteReferenceFileName, spriteDefaultColour, spriteScaleFactor))
	{
		result = false;
	}

	return result;
}


//added ANN3i4b
bool ANNdrawClass::ANNaddSpriteToSpriteReferenceList(vec* spriteSceneCoords, vec* eyeCoords, LDreference** currentListReference, string spriteReferenceFileName, int spriteDefaultColour, double spriteScaleFactor)
{
	//add sprite to spriteByteArray (replace sprite of sprite index, spriteIndex, if it already exists)

	bool result = true;

	LDreference spriteReference;

	spriteReference.colour = spriteDefaultColour;
	spriteReference.type = REFERENCE_TYPE_SUBMODEL;
	mat spriteRotationMatrix;

	SHAREDvector.createIdentityMatrix(&spriteRotationMatrix);
	SHAREDvector.scaleMatrix(&spriteRotationMatrix, spriteScaleFactor);

	SHAREDvector.copyMatrixTwoIntoMatrixOne(&(spriteReference.deformationMatrix), &(spriteRotationMatrix));

	spriteReference.relativePosition.x = spriteSceneCoords->x;
	spriteReference.relativePosition.y = spriteSceneCoords->y;
	spriteReference.relativePosition.z = spriteSceneCoords->z;
	spriteReference.name = spriteReferenceFileName;

	LDreferenceManipulation.copyReferences(*currentListReference, &spriteReference, spriteReference.type);
	LDreference* newReference = new LDreference();
	(*currentListReference)->next = newReference;
	(*currentListReference) = (*currentListReference)->next;

	return result;
}


void ANNdrawClass::writeSpriteTextToSVG(string* spriteTextString, bool writeSVG, XMLparserTag** currentTagSVG, vec* referencePosition)
{
	if(writeSVG)
	{
		int numberOfLines = 1;
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
			if((spriteTextString->at(i) == CHAR_NEWLINE) || (i == spriteTextString->length()-1))
			{
				string stringCurrentLine = "";
				if(spriteTextString->at(i) == CHAR_NEWLINE)
				{
					stringCurrentLine = spriteTextString->substr(i-stringCurrentLineIndex, stringCurrentLineIndex);
				}
				else
				{
					stringCurrentLine = spriteTextString->substr(i-stringCurrentLineIndex, stringCurrentLineIndex+1);
				}

				vec positionSVG;
				positionSVG.x = referencePosition->x*ANN_SVG_SCALE_FACTOR - ANN_SVG_NEURON_SIZE/2;
				positionSVG.y = referencePosition->y*ANN_SVG_SCALE_FACTOR - (numberOfLines)*ANN_SVG_SPRITE_TEXT_OFFSET_PER_LINE + lineIndex*ANN_SVG_SPRITE_TEXT_OFFSET_PER_LINE;
				positionSVG.z = ANN_OUTPUT_Z_POSITION_TEXT;
				LDsvg.writeSVGtext(currentTagSVG, stringCurrentLine, &positionSVG, ANN_SVG_TEXT_SCALE_FACTOR, SHARED_COLOUR_BLACK);

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
}









void ANNdrawClass::ANNgenerateTextualNeuronSpriteInfoString(ANNneuron* neuron, string* spriteTextString, int spriteColourArray[])
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


	string tempString = "";
	//char* tempString = new char[DAT_FILE_DATA_VALUE_MAX_LENGTH];

	/*Unit Name Information*/

	if(SPRITE_TEXTUAL_INCLUDE_NEURON_ID_INFO)
	{
		tempString = SHAREDvars.convertIntToString(neuron->id);
		if(SPRITE_TEXTUAL_INCLUDE_NEURON_EXTRA_SECONDARY_INFO)
		{
			*spriteTextString = *spriteTextString + "ID = " + tempString;

		}
		else
		{
			*spriteTextString = *spriteTextString + "        " + tempString;
		}
	}


	if(SPRITE_TEXTUAL_INCLUDE_NEURON_ORDERID_INFO)
	{
		*spriteTextString = *spriteTextString + '\n';
		tempString = SHAREDvars.convertIntToString(neuron->orderID);
		*spriteTextString = *spriteTextString + "orderID = " + tempString;
	}


	if(SPRITE_TEXTUAL_INCLUDE_NEURON_LAYERID_INFO)
	{
		*spriteTextString = *spriteTextString + '\n';
		tempString = SHAREDvars.convertIntToString(neuron->layerID);
		*spriteTextString = *spriteTextString + "layerID = " + tempString;
	}


	if(SPRITE_TEXTUAL_INCLUDE_NEURON_SUBNETID_INFO)
	{
		*spriteTextString = *spriteTextString + '\n';
		tempString = SHAREDvars.convertIntToString(neuron->subnetID);
		*spriteTextString = *spriteTextString + "subnetID = " + tempString;
	}

	#ifndef ANN_DRAW_DONT_DISPLAY_BIAS
	if(SPRITE_TEXTUAL_INCLUDE_NEURON_BIAS_INFO)
	{
		*spriteTextString = *spriteTextString + '\n';
		#ifdef ANN_ALGORITHM_CLASSIFICATION_NETWORK
		tempString = SHAREDvars.convertLongToString(neuron->memoryTrace);
		*spriteTextString = *spriteTextString + "memoryTrace = " + tempString;
		#else
		tempString = SHAREDvars.convertDoubleToString(neuron->bias, "%0.2f");
		*spriteTextString = *spriteTextString + "bias = " + tempString;
		#endif
	}
	#endif


	if(SPRITE_TEXTUAL_INCLUDE_NEURON_OUTPUT_INFO)
	{
		*spriteTextString = *spriteTextString + '\n';
		tempString = SHAREDvars.convertDoubleToString(neuron->output, "%0.2f");
		*spriteTextString = *spriteTextString + "output = " + tempString;
	}

	if(SPRITE_TEXTUAL_INCLUDE_NEURON_CLASSTARGET_INFO)
	{
		*spriteTextString = *spriteTextString + '\n';
		tempString = SHAREDvars.convertDoubleToString(neuron->classTarget, "%0.2f");
		*spriteTextString = *spriteTextString + "classTarget = " + tempString;
	}

	if(SPRITE_TEXTUAL_INCLUDE_NEURON_ERROR_INFO)
	{
		*spriteTextString = *spriteTextString + '\n';
		tempString = SHAREDvars.convertDoubleToString(neuron->error, "%0.2f");
		*spriteTextString = *spriteTextString + "error = " + tempString;
	}

	#ifdef ANN_ALGORITHM_GIA_NEURAL_NETWORK
	string entityTypeString = "";
	if(neuron->GIAisConceptEntity)
	{
		entityTypeString = "concept";
	}
	else
	{
		entityTypeString = "instance";
	}
	*spriteTextString = *spriteTextString + '\n';
	tempString = neuron->GIAentityName;
	*spriteTextString = *spriteTextString + entityTypeString + " = " + tempString;
	#endif
	#ifdef ANN_ALGORITHM_SANI_SEQUENCE_GRAMMAR
	tempString = neuron->SANIneuronName;
	*spriteTextString = *spriteTextString + tempString;	
	#endif

	/*End Start Sprite Text Creation*/
}


void ANNdrawClass::ANNgenerateTextualANNneuronConnectionSpriteInfoString(ANNneuronConnection* ANNneuronConnection, string* spriteTextString, int spriteColourArray[])
{
	/*Start Sprite Text Creation*/

	spriteColourArray[0] = SPRITE_TEXTUAL_INCLUDE_NEURONCONNECTION_WEIGHT_COLOUR;
	spriteColourArray[1] = SPRITE_TEXTUAL_INCLUDE_NEURONCONNECTION_WEIGHT_COLOUR;
	spriteColourArray[2] = SPRITE_TEXTUAL_INCLUDE_NEURONCONNECTION_WEIGHT_COLOUR;
	spriteColourArray[3] = SPRITE_TEXTUAL_INCLUDE_NEURONCONNECTION_WEIGHT_COLOUR;

	string tempString = "";

	/*Unit Name Information*/

	#ifdef ANN_ALGORITHM_CLASSIFICATION_NETWORK
	tempString = SHAREDvars.convertDoubleToString(ANNneuronConnection->idealValue, "%0.2f");
	*spriteTextString = "\n\n" + *spriteTextString + "IV = " + tempString;
	#else
	#ifdef ANN_ALGORITHM_GIA_POS_REL_TRANSLATOR_NEURAL_NETWORK
	*spriteTextString = "";
	#else
	tempString = SHAREDvars.convertDoubleToString(ANNneuronConnection->weight, "%0.2f");
	*spriteTextString = "\n\n" + *spriteTextString + "weight = " + tempString;
	#endif
	#endif


	/*End Start Sprite Text Creation*/
}


/*
c++ convert number to rainbow
https://www.particleincell.com/2014/colormap/
*/
colour ANNdrawClass::convertDoubleToRainbow(const double colourDouble, const double doubleMaxValue)
{
	double f = colourDouble/doubleMaxValue;
	return convertDoubleToRainbow(f);
}
colour ANNdrawClass::convertDoubleToRainbow(const double f)
{
	colour colrgb;

	//double a=(1-f)/0.2;	//with magenta
	double a=(1-f)/0.25;
	int X=floor(a);
	int Y=floor(255*(a-X));
	uchar r;
	uchar g;
	uchar b;
	switch(X)
	{
		case 0: r=255;g=Y;b=0;break;
		case 1: r=255-Y;g=255;b=0;break;
		case 2: r=0;g=255;b=Y;break;
		case 3: r=0;g=255-Y;b=255;break;
		case 4: r=0;g=0;b=255;break;
		/*
		//with magenta
		case 4: r=Y;g=0;b=255;break;
		case 5: r=255;g=0;b=255;break;
		*/
	}
	colrgb.r = r;
	colrgb.g = g;
	colrgb.b = b;
	
	return colrgb;
}





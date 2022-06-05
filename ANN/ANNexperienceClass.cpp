 /*******************************************************************************
 *
 * License: GNU Affero General Public License version 3 (AGPLv3)
 *
 *******************************************************************************/

/*******************************************************************************
 *
 * File Name: ANNexperienceClass.cpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2022 Baxter AI (baxterai.com)
 * Project: Artificial Neural Network (ANN)
 * Project Version: 3q1a 05-June-2022
 * Comments:
 * /
 *******************************************************************************/


#include "ANNexperienceClass.hpp"

ANNexperience::ANNexperience(void)
{
	classTargetValue = 0;
	#ifdef ANN_EXPERIENCE_CLASS_PREDICTION_ERROR
	predictionError = 0.0;
	#endif
	firstExperienceInput = new ANNexperienceInput();
	next = NULL;
}

ANNexperience::~ANNexperience(void)
{
	if(firstExperienceInput != NULL)
	{
		delete firstExperienceInput;
	}

	if(next != NULL)
	{
		delete next;
	}

}



ANNexperienceInput::ANNexperienceInput(void)
{
	inputValue = 0;
	next = NULL;
}

ANNexperienceInput::~ANNexperienceInput(void)
{
	if(next != NULL)
	{
		delete next;
	}
}




void ANNexperienceClassClass::copyExperiences(ANNexperience* newExperience, ANNexperience* experienceToCopy)
{
	newExperience->classTargetValue = experienceToCopy->classTargetValue;

	ANNexperienceInput* currentExperienceToCopyInput = experienceToCopy->firstExperienceInput;
	ANNexperienceInput* currentNewExperienceInput = newExperience->firstExperienceInput;

	while(currentExperienceToCopyInput->next != NULL)
	{
		currentNewExperienceInput->inputValue = currentExperienceToCopyInput->inputValue;
		ANNexperienceInput* newExperienceInput = new ANNexperienceInput();
		currentNewExperienceInput->next = newExperienceInput;
		currentNewExperienceInput = currentNewExperienceInput->next;
		currentExperienceToCopyInput = currentExperienceToCopyInput->next;
	}
}

ANNexperience* ANNexperienceClassClass::findExperience(ANNexperience* firstExperienceInDataSet, const int64_t experienceNumToFind)
{
	ANNexperience* foundExperience;
	ANNexperience* currentExperience = firstExperienceInDataSet;
	for(int64_t i = 0; i < experienceNumToFind; i++)
	{
		currentExperience = currentExperience->next;
	}
	foundExperience = currentExperience;
	return foundExperience;
}

int64_t ANNexperienceClassClass::countNumberOfExperienceInputs(const ANNexperience* experience)
{
	int64_t numberOfExperienceInputs = 0;

	const ANNexperienceInput* currentExperienceInput = experience->firstExperienceInput;
	while(currentExperienceInput->next != NULL)
	{
		numberOfExperienceInputs++;
		currentExperienceInput = currentExperienceInput->next;
	}

	return numberOfExperienceInputs;
}

int64_t ANNexperienceClassClass::countNumberOfExperiences(const ANNexperience* firstExperienceInList)
{
	int64_t numberOfExperiences = 0;

	const ANNexperience* currentExperience = firstExperienceInList;
	while(currentExperience->next != NULL)
	{
		countNumberOfExperienceInputs(currentExperience);

		numberOfExperiences++;
		currentExperience = currentExperience->next;
	}

	return numberOfExperiences;
}


void ANNexperienceClassClass::printExperienceInputs(const ANNexperience* experience)
{
	const ANNexperienceInput* currentExperienceInput = experience->firstExperienceInput;
	while(currentExperienceInput->next != NULL)
	{
		cout << "\tInput Value = " << currentExperienceInput->inputValue << endl;

		currentExperienceInput = currentExperienceInput->next;
	}
}

void ANNexperienceClassClass::printExperiences(const ANNexperience* firstExperienceInList)
{
	const ANNexperience* currentExperience = firstExperienceInList;
	while(currentExperience->next != NULL)
	{
		cout << "ANNexperience:" << endl;
		cout << "class Target = " << currentExperience->classTargetValue << endl;

		printExperienceInputs(currentExperience);

		currentExperience = currentExperience->next;
	}
}

void ANNexperienceClassClass::addExperiencesToOFStream(ofstream* experienceDataSetOfStreamObject, const ANNexperience* firstExperienceInList)
{
	const ANNexperience* currentExperience = firstExperienceInList;
	while(currentExperience->next != NULL)
	{
		addExperienceToOFStream(experienceDataSetOfStreamObject, currentExperience);
		currentExperience = currentExperience->next;
	}
}

void ANNexperienceClassClass::addExperienceToOFStream(ofstream* experienceDataSetOfStreamObject, const ANNexperience* experience)
{
	string tempString = SHAREDvars.convertIntToString(experience->classTargetValue);
	//experienceDataSetOfStreamObject->write(tempString);
	for(int i = 0; i<(int)(tempString.length());i++)
	{
		experienceDataSetOfStreamObject->put(tempString[i]);
	}
	experienceDataSetOfStreamObject->put(CHAR_COMMA);

	const ANNexperienceInput* currentExperienceInput = experience->firstExperienceInput;
	while(currentExperienceInput->next != NULL)
	{
		string tempString2 = SHAREDvars.convertDoubleToString(currentExperienceInput->inputValue, "%0.6f");
		//experienceDataSetOfStreamObject->write(tempString2);
		for(int i = 0; i<(int)(tempString2.length());i++)
		{
			experienceDataSetOfStreamObject->put(tempString2[i]);
		}
		experienceDataSetOfStreamObject->put(CHAR_COMMA);
		currentExperienceInput = currentExperienceInput->next;
	}

	experienceDataSetOfStreamObject->put(CHAR_NEWLINE);
}




double ANNexperienceClassClass::normaliseExperienceInput(double currentInputValue, double maxInputValue)
{
	double minInputValue = 0.0;
	if(currentInputValue > maxInputValue)
	{//deal with redundant situations, eg a single unit has two swords - nb only one part type can be used per unit,  and so the following is a valid correction;
	//also deals with raytraced no hit illegal depth values from depth map experience lists
		currentInputValue = maxInputValue;
	}

	double normalisedExperienceInput;

	if(SHAREDvars.compareDoubles(maxInputValue, minInputValue))
	{
		normalisedExperienceInput = currentInputValue;
	}
	else
	{
		normalisedExperienceInput = ((currentInputValue - minInputValue) / (maxInputValue - minInputValue));
	}

	return normalisedExperienceInput;
}



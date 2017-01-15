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
 * File Name: ANNexperienceClass.cpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2016 Baxter AI (baxterai.com)
 * Project: Artificial Neural Network (ANN)
 * Project Version: 4a12b 10-June-2016
 * Comments:
 *
 *******************************************************************************/


#include "ANNexperienceClass.h"
#include "SHAREDvars.h"

ANNexperience::ANNexperience(void)
{
	classTargetValue = 0;
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




void copyExperiences(ANNexperience* newExperience, ANNexperience* experienceToCopy)
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

ANNexperience* findExperience(ANNexperience* firstExperienceInDataSet, long experienceNumToFind)
{
	ANNexperience* foundExperience;
	ANNexperience* currentExperience = firstExperienceInDataSet;
	for(long i = 0; i < experienceNumToFind; i++)
	{
		currentExperience = currentExperience->next;
	}
	foundExperience = currentExperience;
	return foundExperience;
}

long countNumberOfExperienceInputs(ANNexperience* experience)
{
	long numberOfExperienceInputs = 0;

	ANNexperienceInput* currentExperienceInput = experience->firstExperienceInput;
	while(currentExperienceInput->next != NULL)
	{
		numberOfExperienceInputs++;
		currentExperienceInput = currentExperienceInput->next;
	}

	return numberOfExperienceInputs;
}

long countNumberOfExperiences(ANNexperience* firstExperienceInList)
{
	long numberOfExperiences = 0;

	ANNexperience* currentExperience = firstExperienceInList;
	while(currentExperience->next != NULL)
	{
		countNumberOfExperienceInputs(currentExperience);

		numberOfExperiences++;
		currentExperience = currentExperience->next;
	}

	return numberOfExperiences;
}


void printExperienceInputs(ANNexperience* experience)
{
	ANNexperienceInput* currentExperienceInput = experience->firstExperienceInput;
	while(currentExperienceInput->next != NULL)
	{
		cout << "\tInput Value = " << currentExperienceInput->inputValue << endl;

		currentExperienceInput = currentExperienceInput->next;
	}
}

void printExperiences(ANNexperience* firstExperienceInList)
{
	ANNexperience* currentExperience = firstExperienceInList;
	while(currentExperience->next != NULL)
	{
		cout << "ANNexperience:" << endl;
		cout << "class Target = " << currentExperience->classTargetValue << endl;

		printExperienceInputs(currentExperience);

		currentExperience = currentExperience->next;
	}
}

void addExperiencesToOFStream(ofstream* experienceDataSetOfStreamObject, ANNexperience* firstExperienceInList)
{
	ANNexperience* currentExperience = firstExperienceInList;
	while(currentExperience->next != NULL)
	{
		addExperienceToOFStream(experienceDataSetOfStreamObject, currentExperience);
		currentExperience = currentExperience->next;
	}
}

void addExperienceToOFStream(ofstream* experienceDataSetOfStreamObject, ANNexperience* experience)
{
	string tempString = convertIntToString(experience->classTargetValue);
	//experienceDataSetOfStreamObject->write(tempString);
	for(int i = 0; i<(int)(tempString.length());i++)
	{
		experienceDataSetOfStreamObject->put(tempString[i]);
	}
	experienceDataSetOfStreamObject->put(CHAR_COMMA);

	ANNexperienceInput* currentExperienceInput = experience->firstExperienceInput;
	while(currentExperienceInput->next != NULL)
	{
		string tempString2 = convertDoubleToString(currentExperienceInput->inputValue, "%0.6f");
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




double normaliseExperienceInput(double currentInputValue, double maxInputValue)
{
	double minInputValue = 0.0;
	if(currentInputValue > maxInputValue)
	{//deal with redundant situations, eg a single unit has two swords - nb only one part type can be used per unit,  and so the following is a valid correction;
	//also deals with raytraced no hit illegal depth values from depth map experience lists
		currentInputValue = maxInputValue;
	}

	double normalisedExperienceInput;

	if(compareDoubles(maxInputValue, minInputValue))
	{
		normalisedExperienceInput = currentInputValue;
	}
	else
	{
		normalisedExperienceInput = ((currentInputValue - minInputValue) / (maxInputValue - minInputValue));
	}

	return normalisedExperienceInput;
}



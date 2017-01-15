/*******************************************************************************
 *
 * This file is part of BAIPROJECT.
 *
 * BAIPROJECT is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License version 3
 * only, as published by the Free Software Foundation. The use of
 * intermediary programs or interfaces including file i/o is considered
 * remote network interaction.
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
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2014 Baxter AI (baxterai.com)
 * Project: Artificial Neural Network (ANN)
 * Project Version: 3d3a 24-May-2014
 * Comments:
 *
 *******************************************************************************/

#include "ANNexperienceClass.h"
#include "SHAREDvars.h"

#include <ctime>
#include <cstdlib>	//for random number generation
#include <cmath>

#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <string.h>
#include <iostream>
#include <fstream>
using namespace std;








Experience::Experience(void)
{
	classTargetValue = 0;
	firstExperienceInput = new ExperienceInput();
	next = NULL;
}

Experience::~Experience(void)
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



ExperienceInput::ExperienceInput(void)
{
	inputValue = 0;
	next = NULL;
}

ExperienceInput::~ExperienceInput(void)
{
	if(next != NULL)
	{
		delete next;
	}
}




void copyExperiences(Experience * newExperience, Experience * experienceToCopy)
{
	newExperience->classTargetValue = experienceToCopy->classTargetValue;

	ExperienceInput * currentExperienceToCopyInput = experienceToCopy->firstExperienceInput;
	ExperienceInput * currentNewExperienceInput = newExperience->firstExperienceInput;

	while(currentExperienceToCopyInput->next != NULL)
	{
		currentNewExperienceInput->inputValue = currentExperienceToCopyInput->inputValue;
		ExperienceInput * newExperienceInput = new ExperienceInput();
		currentNewExperienceInput->next = newExperienceInput;
		currentNewExperienceInput = currentNewExperienceInput->next;
		currentExperienceToCopyInput = currentExperienceToCopyInput->next;
	}
}

Experience * findExperience(Experience * firstExperienceInDataSet, long experienceNumToFind)
{
	Experience * foundExperience;
	Experience * currentExperience = firstExperienceInDataSet;
	for(long i = 0; i < experienceNumToFind; i++)
	{
		currentExperience = currentExperience->next;
	}
	foundExperience = currentExperience;
	return foundExperience;
}

long countNumberOfExperienceInputs(Experience * experience)
{
	long numberOfExperienceInputs = 0;

	ExperienceInput * currentExperienceInput = experience->firstExperienceInput;
	while(currentExperienceInput->next != NULL)
	{
		numberOfExperienceInputs++;
		currentExperienceInput = currentExperienceInput->next;
	}

	return numberOfExperienceInputs;
}

long countNumberOfExperiences(Experience * firstExperienceInList)
{
	long numberOfExperiences = 0;

	Experience * currentExperience = firstExperienceInList;
	while(currentExperience->next != NULL)
	{
		countNumberOfExperienceInputs(currentExperience);

		numberOfExperiences++;
		currentExperience = currentExperience->next;
	}

	return numberOfExperiences;
}


void printExperienceInputs(Experience * experience)
{
	ExperienceInput * currentExperienceInput = experience->firstExperienceInput;
	while(currentExperienceInput->next != NULL)
	{
		cout << "\tInput Value = " << currentExperienceInput->inputValue << endl;

		currentExperienceInput = currentExperienceInput->next;
	}
}

void printExperiences(Experience * firstExperienceInList)
{
	Experience * currentExperience = firstExperienceInList;
	while(currentExperience->next != NULL)
	{
		cout << "Experience:" << endl;
		cout << "class Target = " << currentExperience->classTargetValue << endl;

		printExperienceInputs(currentExperience);

		currentExperience = currentExperience->next;
	}
}

void addExperiencesToOFStream(ofstream * experienceDataSetOfStreamObject, Experience * firstExperienceInList)
{
	Experience * currentExperience = firstExperienceInList;
	while(currentExperience->next != NULL)
	{
		addExperienceToOFStream(experienceDataSetOfStreamObject, currentExperience);
		currentExperience = currentExperience->next;
	}
}

void addExperienceToOFStream(ofstream * experienceDataSetOfStreamObject, Experience * experience)
{
	char tempString[MAX_EXPERIENCE_SIGNIFICANT_BITS];

	sprintf(tempString, "%d", experience->classTargetValue);
	string tempStringPlusPlus = tempString;
	for(int i = 0; i<(int)(tempStringPlusPlus.length());i++)
	{
		experienceDataSetOfStreamObject->put(tempString[i]);
	}
	experienceDataSetOfStreamObject->put(CHAR_COMMA);

	ExperienceInput * currentExperienceInput = experience->firstExperienceInput;
	while(currentExperienceInput->next != NULL)
	{
		sprintf(tempString, "%0.6f",  currentExperienceInput->inputValue);
		string tempStringPlusPlus = tempString;
		for(int i = 0; i<(int)(tempStringPlusPlus.length());i++)
		{
			experienceDataSetOfStreamObject->put(tempString[i]);
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



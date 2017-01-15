/*******************************************************************************
 *
 * File Name: ANNexperienceClass.cpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2010 Baxter AI (baxterai.com)
 * Project: Advanced Neural Network (ANN)
 * Project Version: 3a5g 01-Nov-2011
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

#define CHAR_COMMA ','
#define CHAR_NEW_LINE '\n'






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
		//TEMP;
		//cout << "currentExperienceInput->inputValue = " << currentExperienceInput->inputValue << endl;

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
		//TEMP;
		//cout << "new experience counted" << endl;

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

void addExperiencesToOFStream(ofstream * experienceDataSetOFStreamObject, Experience * firstExperienceInList)
{
	Experience * currentExperience = firstExperienceInList;
	while(currentExperience->next != NULL)
	{
		addExperienceToOFStream(experienceDataSetOFStreamObject, currentExperience);
		currentExperience = currentExperience->next;
	}
}

void addExperienceToOFStream(ofstream * experienceDataSetOFStreamObject, Experience * experience)
{
	char tempString[MAX_EXPERIENCE_SIGNIFICANT_BITS];

	sprintf(tempString, "%d", experience->classTargetValue);
	string tempStringPlusPlus = tempString;
	for(int i = 0; i<(int)(tempStringPlusPlus.length());i++)
	{
		experienceDataSetOFStreamObject->put(tempString[i]);
	}
	experienceDataSetOFStreamObject->put(CHAR_COMMA);

	ExperienceInput * currentExperienceInput = experience->firstExperienceInput;
	while(currentExperienceInput->next != NULL)
	{
		sprintf(tempString, "%0.6f",  currentExperienceInput->inputValue);
		string tempStringPlusPlus = tempString;
		for(int i = 0; i<(int)(tempStringPlusPlus.length());i++)
		{
			experienceDataSetOFStreamObject->put(tempString[i]);
		}
		experienceDataSetOFStreamObject->put(CHAR_COMMA);
		currentExperienceInput = currentExperienceInput->next;
	}

	experienceDataSetOFStreamObject->put(CHAR_NEW_LINE);
}




double normaliseExperienceInput(double currentInputValue, double maxInputValue)
{
	//cout << "44" << endl;


	double minInputValue = 0.0;
	if(currentInputValue > maxInputValue)
	{//deal with redundant situations, eg a single unit has two swords - nb only one part type can be used per unit,  and so the following is a valid correction;
	//also deals with raytraced no hit illegal depth values from depth map experience lists
		currentInputValue = maxInputValue;
	}

	double normalisedExperienceInput;

	if(compareDoubles(maxInputValue, minInputValue))
	{
		/*
		cout << "currentInputValue = " << currentInputValue << endl;
		cout << "minInputValue = " << minInputValue << endl;
		cout << "maxInputValue = " << maxInputValue << endl;
		cout << "a" << endl;
		*/
		normalisedExperienceInput = currentInputValue;
	}
	else
	{
		/*
		cout << "currentInputValue = " << currentInputValue << endl;
		cout << "minInputValue = " << minInputValue << endl;
		cout << "maxInputValue = " << maxInputValue << endl;
		cout << "b" << endl;
		*/
		normalisedExperienceInput = ((currentInputValue - minInputValue) / (maxInputValue - minInputValue));
	}
	//cout << "normalisedExperienceInput = 55 = " << normalisedExperienceInput << endl;

	return normalisedExperienceInput;
}



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
 * File Name: ANNexperienceClass.h
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2014 Baxter AI (baxterai.com)
 * Project: Artificial Neural Network (ANN)
 * Project Version: 3d2d 14-April-2014
 * Comments:
 *
 *******************************************************************************/




#ifndef HEADER_ANN_EXPERIENCE_CLASS
#define HEADER_ANN_EXPERIENCE_CLASS

#include "ANNglobalDefs.h"

#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <string.h>
#include <iostream>
#include <fstream>
#include <time.h>
#include <math.h>
using namespace std;



//Raytracing Defs;

#define DAT_EXTENSION ".dat"
#define PPM_EXTENSION ".ppm"
#define TAL_EXTENSION ".tal"
#define POVRAY_EXTENSION ".pov"
#define OBJECT_EXPERIENCE_POVRAY_FILE_NAME "object"


#ifdef USE_HI_RES_PRIMITIVE_REPLACEMENT_DURING_RENDERING
	//for hi res LGEO rendering;
	#define OBJECT_EXPERIENCE_GEN_L3P_EXCCMD_PART_1 "env LDRAWDIR=/usr/share/local/LDRAW LGEODIR=/usr/share/local/LGEO l3p "
#else
	#define OBJECT_EXPERIENCE_GEN_L3P_EXCCMD_PART_1 "env LDRAWDIR=/usr/share/local/LDRAW l3p "
#endif

#ifdef TH_USE_ANN_TEST_HARNESS
	#define OBJECT_EXPERIENCE_GEN_IMAGE_WIDTH "15"
	#define OBJECT_EXPERIENCE_GEN_IMAGE_HEIGHT "15"
	#define OBJECT_EXPERIENCE_GEN_L3P_EXCCMD_PART_2 "/usr/share/local/LDRAW/PARTS/"
#else
	#define OBJECT_EXPERIENCE_GEN_IMAGE_WIDTH "320"
	#define OBJECT_EXPERIENCE_GEN_IMAGE_HEIGHT "200"
 	#define OBJECT_EXPERIENCE_GEN_L3P_EXCCMD_PART_2 "" 	//if part/model is in current directory; ie for raytracing of custom .ldr parts
#endif

#define OBJECT_EXPERIENCE_GEN_L3P_EXCCMD_PART_3 "partName"	//this is replaced dynamically!
#define OBJECT_EXPERIENCE_GEN_L3P_EXCCMD_PART_4 (DAT_EXTENSION)	//.dat
#define OBJECT_EXPERIENCE_GEN_L3P_EXCCMD_PART_5 " "
#define OBJECT_EXPERIENCE_GEN_L3P_EXCCMD_PART_6 (OBJECT_EXPERIENCE_POVRAY_FILE_NAME)
#define OBJECT_EXPERIENCE_GEN_L3P_EXCCMD_PART_7 " -o"	//overwrite


	/*
	-b[<color>] background color, default color is 0, -b gives 0.3,0.8,1
	-c<color> default part color, default color is 7
	-bu bumps
	-sw<w> seam width, default is 0.5
	-f[<type>][<y>] floor at <y>, default y is just below model
	-cg<la>,<lo>[,<r>] camera globe position, default is 30,45,0
	-cc<x>,<y>,<z> camera coordinates (location)
	-cla<x>,<y>,<z> camera look_at coordinates
	-ca<a> camera angle, default is 67.38 degrees
	-lg<la>,<lo>,<r>[,<color>] lightsource globe position, default color is 15
	-lc<x>,<y>,<z>[,<color>] lightsource coordinates, default color is 15
	-ld always use default lightsources
	-l use positions and colors of light.dat files as lightsources
	-lgeo use LGEO parts wherever possible
	-p don't substitute any primitives in the P directory with POV equivalents
	-q<n> quality level 0..3, default is 2
	-o overwrite POV file if it already exists
	*/

#define OBJECT_EXPERIENCE_GEN_POVRAY_EXCCMD_PART_1 "povray"
#define OBJECT_EXPERIENCE_GEN_POVRAY_EXCCMD_PART_2 " +I"
#define OBJECT_EXPERIENCE_GEN_POVRAY_EXCCMD_PART_3 (OBJECT_EXPERIENCE_POVRAY_FILE_NAME)
#define OBJECT_EXPERIENCE_GEN_POVRAY_EXCCMD_PART_4 (POVRAY_EXTENSION)
#define OBJECT_EXPERIENCE_GEN_POVRAY_EXCCMD_PART_5 " +FP" //output as a PPM file
#define OBJECT_EXPERIENCE_GEN_POVRAY_EXCCMD_PART_6 " +O"
#define OBJECT_EXPERIENCE_GEN_POVRAY_EXCCMD_PART_7 "partName"	//this is replaced dynamically!
#define OBJECT_EXPERIENCE_GEN_POVRAY_EXCCMD_PART_8 ""
#ifdef USE_HI_RES_PRIMITIVE_REPLACEMENT_DURING_RENDERING
	//for hi res LGEO rendering;
	#define OBJECT_EXPERIENCE_GEN_L3P_EXCCMD_PART_9 " -b -lgeo -q3"
		//do not use the -f paramter this prevents a file from being output
#else
	#define OBJECT_EXPERIENCE_GEN_L3P_EXCCMD_PART_9 " -b"
#endif
#define OBJECT_EXPERIENCE_GEN_POVRAY_EXCCMD_PART_10 " +W"
#define OBJECT_EXPERIENCE_GEN_POVRAY_EXCCMD_PART_11 (OBJECT_EXPERIENCE_GEN_IMAGE_WIDTH)
#define OBJECT_EXPERIENCE_GEN_POVRAY_EXCCMD_PART_12 " +H"
#define OBJECT_EXPERIENCE_GEN_POVRAY_EXCCMD_PART_13 (OBJECT_EXPERIENCE_GEN_IMAGE_HEIGHT)
#define OBJECT_EXPERIENCE_GEN_POVRAY_EXCCMD_PART_14 " -d -V -WL0 -GD -GF -GR -GS -GW -GA"	//turn display off, turn off verbose output, turn warnings off, turn off all text messages, NB cannot turn off banner

	/*lighting conditions not yet varied;
	#define OBJECT_EXPERIENCE_GEN_L3P_EXCCMD_PART_14 " -cg"
	#define OBJECT_EXPERIENCE_GEN_L3P_EXCCMD_PART_15 "30,45"
	*/



#define MAX_EXPERIENCE_SIGNIFICANT_BITS (20)

class ExperienceInput
{
public:

	ExperienceInput(void);
	~ExperienceInput(void);

	double inputValue;
	ExperienceInput * next;
};



class Experience
{
public:

	Experience(void);
	~Experience(void);

	long classTargetValue;

	ExperienceInput * firstExperienceInput;

#ifdef DEBUG_ANN_PERFORM_ALTERNATE_ERROR_TOTAL
	long numberOfAlternateOptionErrors;
#endif

	Experience * next;

};

void copyExperiences(Experience * newExperience, Experience * experienceToCopy);
Experience * findExperience(Experience * firstExperienceInDataSet, long experienceNumToFind);
long countNumberOfExperienceInputs(Experience * experience);
long countNumberOfExperiences(Experience * firstExperienceInList);


void printExperienceInputs(Experience * experience);
void printExperiences(Experience * firstExperienceInList);

void addExperiencesToOFStream(ofstream * experienceDataSetOfStreamObject, Experience * firstExperienceInList);
	void addExperienceToOFStream(ofstream * experienceDataSetOfStreamObject, Experience * experience);

double normaliseExperienceInput(double currentInputValue, double maxInputValue);

#endif


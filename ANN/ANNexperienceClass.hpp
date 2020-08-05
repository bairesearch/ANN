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
 * File Name: ANNexperienceClass.hpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2020 Baxter AI (baxterai.com)
 * Project: Artificial Neural Network (ANN)
 * Project Version: 3n3a 28-May-2020
 * Comments:
 * /
 *******************************************************************************/


#ifndef HEADER_ANN_EXPERIENCE_CLASS
#define HEADER_ANN_EXPERIENCE_CLASS

#include "ANNglobalDefs.hpp"
#include "SHAREDvars.hpp"


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

class ANNexperienceInput
{
public:

	ANNexperienceInput(void);
	~ANNexperienceInput(void);

	double inputValue;
	ANNexperienceInput* next;
};



class ANNexperience
{
public:

	ANNexperience(void);
	~ANNexperience(void);

	int64_t classTargetValue;
	#ifdef ANN_EXPERIENCE_CLASS_PREDICTION_ERROR
	double predictionError;
	#endif
	
	ANNexperienceInput* firstExperienceInput;

	ANNexperience* next;

};

class ANNexperienceClassClass
{
	private: SHAREDvarsClass SHAREDvars;
	public: void copyExperiences(ANNexperience* newExperience, ANNexperience* experienceToCopy);
	public: ANNexperience* findExperience(ANNexperience* firstExperienceInDataSet, const int64_t experienceNumToFind);
	private: int64_t countNumberOfExperienceInputs(const ANNexperience* experience);
	public: int64_t countNumberOfExperiences(const ANNexperience* firstExperienceInList);


	private: void printExperienceInputs(const ANNexperience* experience);
	private: void printExperiences(const ANNexperience* firstExperienceInList);

	public: void addExperiencesToOFStream(ofstream* experienceDataSetOfStreamObject, const ANNexperience* firstExperienceInList);
		public: void addExperienceToOFStream(ofstream* experienceDataSetOfStreamObject, const ANNexperience* experience);

	public: double normaliseExperienceInput(double currentInputValue, double maxInputValue);
};

#endif


 /*******************************************************************************
 *
 * License: GNU Affero General Public License version 3 (AGPLv3)
 *
 *******************************************************************************/

/*******************************************************************************
 *
 * File Name: ANNdata.cpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2022 Baxter AI (baxterai.com)
 * Project: Artificial Neural Network (ANN)
 * Project Version: 3q1a 05-June-2022
 * Comments: TH = Test Harness
 * /
 *******************************************************************************/


#include "ANNdata.hpp"

//for specific Thyroid data {215lines, 6 columns - comma delimited}

int64_t numInputNeurons;
int64_t numOutputNeurons;
int64_t numExperiences;
ANNexperience* firstExperienceInDataSet;

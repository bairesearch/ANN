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
 * File Name: ANNparser.h
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2016 Baxter AI (baxterai.com)
 * Project: Artificial Neural Network (ANN)
 * Project Version: 4a11a 09-June-2016
 * Comments: TH = Test Harness
 *
 *******************************************************************************/


#ifndef HEADER_ANN_PARSER
#define HEADER_ANN_PARSER

#define EXPERIENCE_DATASET_MAX_NUM_ROWS_ANNTH 1000	//max num experiences in dataset
#define EXPERIENCE_DATASET_MAX_NUM_COLS_ANNTH 101 	//max num input neurons +1

#define	SOME_REALLY_SMALL_FLOAT 0.0F
#define	SOME_REALLY_LARGE_FLOAT 500.0F

void ANNparseDataFile(string nameOfExperiencesDataSetFile);

#endif

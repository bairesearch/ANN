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
 * File Name: ANNglobalsDefs.h
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2015 Baxter AI (baxterai.com)
 * Project: Artificial Neural Network (ANN)
 * Project Version: 3h1f 14-November-2015
 * Description: ANN specific global definitions
 *
 * Usage Examples:
 *
 * OpenANNwithRT.exe -idata new-thyroid.data -oxml neuralNet.xml -oldr neuralNetWithoutSprites.ldr -oldr2 neuralNetWithSprites.ldr -oppm neuralNet.ppm
 * OpenANNwithRT.exe -idata new-thyroid.data -oall neuralNet
 * OpenANNwithRT.exe -ixml neuralNet.xml -oall neuralNet
 * OpenANNwithRT.exe -idata new-thyroid.data -layers 5 -ineurons 5 -oneurons 3 -divtype 2 -oall neuralNet
 * OpenANNwithRT.exe -layers 5 -ineurons 10 oneurons 5 -oall neuralNet -notshow
 * OpenANNwithRT.exe -layers 5 -ineurons 10 oneurons 5 -oall neuralNet
 * OpenANNwithRT.exe -layers 5 -ineurons 10 -oneurons 5 -divtype 1 -oall neuralNet
 * OpenANNwithRT.exe -layers 5 -ineurons 10 -oneurons 5 -divtype 2 -oall neuralNet
 * OpenANNwithRT.exe -layers 5 -ineurons 10 -oneurons 5 -divtype 3 -oall neuralNet
 * OpenANNwithRT.exe -layers 5 -ineurons 10 -oneurons 5 -divtype 4 -oall neuralNet
 * OpenANNwithRT.exe -layers 5 -ineurons 16 -oneurons 4 -divtype 6 -oall neuralNet
 * OpenANNwithRT.exe -layers 5 -ineurons 5 -oneurons 3 -divtype 1 -usesubnets -numsubnets 2 -oall neuralNet
 * OpenANNwithRT.exe -layers 5 -ineurons 5 -oneurons 3 -divtype 1 -usesubnets -numsubnets 3 -oall neuralNet
 * OpenANNwithRT.exe -layers 5 -ineurons 8 -oneurons 4 -divtype 1 -usesubnets -numsubnets 2 -oall neuralNet
 * OpenANNwithRT.exe -layers 4 -ineurons 8 oneurons 4 -oall neuralNet
 * OpenANNwithRT.exe -ui
 *
 *******************************************************************************/


#ifndef HEADER_ANN_GLOBAL_DEFS
#define HEADER_ANN_GLOBAL_DEFS

#include "SHAREDglobalDefs.h"


#ifdef USE_RT
	#define TH_USE_RT_FOR_NEURAL_NETWORK_VEC_GRAPHICS
#else
	#define TH_USE_POVRAY_FOR_NEURAL_NETWORK_VEC_GRAPHICS
#endif

#define ANN_USE_MIN_NUM_EPOCHS_BEFORE_DECLARE_OVERFITTING_DATA
#ifdef ANN_USE_MIN_NUM_EPOCHS_BEFORE_DECLARE_OVERFITTING_DATA
	#define ANN_MIN_NUM_EPOCHS_BEFORE_DECLARE_OVERFITTING_DATA (50)	//10
#endif

#define DEBUG_ANN_VERIFY_REAL_LIFE_ACCURACY_DIAGNOSTICS_CUTOFF
#ifdef DEBUG_ANN_VERIFY_REAL_LIFE_ACCURACY_DIAGNOSTICS_CUTOFF
	#define DEBUG_ANN_PERFORM_ALTERNATE_ERROR_TOTAL
#endif

//#define TEST_LAYER_DIVERGENCE_TYPE_LINEAR_DIVERGING_THEN_CONVERGING		//Default=not yet decided upon		//ANNformation.cpp


#define IF_LAYER_DIVERGENCE_TYPE_LINEAR_DIVERGING_SQUARE2D_RADIALBIAS_USE_NEARBY_POINTS_INSTEAD
#ifdef IF_LAYER_DIVERGENCE_TYPE_LINEAR_DIVERGING_SQUARE2D_RADIALBIAS_USE_NEARBY_POINTS_INSTEAD
	/*
	#ifdef OR_IMAGE_CATEGORISTION_NN_USE_SMALL_IMAGES
		#define IF_LAYER_DIVERGENCE_TYPE_LINEAR_DIVERGING_SQUARE2D_RADIALBIAS_BIAS (2.0)
	#else
		#define IF_LAYER_DIVERGENCE_TYPE_LINEAR_DIVERGING_SQUARE2D_RADIALBIAS_BIAS (4.0)	//4.0
	#endif
	*/
	#define IF_LAYER_DIVERGENCE_TYPE_LINEAR_DIVERGING_SQUARE2D_RADIALBIAS_BIAS (4.0)
#else
	#define IF_LAYER_DIVERGENCE_TYPE_LINEAR_DIVERGING_SQUARE2D_RADIALBIAS_BIAS (1.0)
#endif
#define LAYER_DIVERGENCE_TYPE_LINEAR_DIVERGING_SQUARE2D_USE_CENTRED



#endif

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
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2016 Baxter AI (baxterai.com)
 * Project: Artificial Neural Network (ANN)
 * Project Version: 4a12b 10-June-2016
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

//#define ANN_DEBUG_DO_NOT_RANDOMISE_INPUT_DATA	//removed 4a3t for testing

#define ANN_DRAW_DISABLE_FILE_OUTPUT_NOTIFICATIONS
//#define ANN_DEBUG_GENERATE_NORMALISED_DATA_FILE

#define ANN_ALGORITHM_CLASSIFICATION_NETWORK	//ANN4a3a
//#define ANN_ALGORITHM_SEPARATE_CLASSIFICATION_AND_MEMORY_NETWORKS //ANN4a1a
#ifdef ANN_ALGORITHM_CLASSIFICATION_NETWORK
	//#define ANN_ALGORITHM_CLASSIFICATION_NETWORK_DISPLAY_EVOLUTION_OF_NETWORK	//ANN4a6a
	#define ANN_ALGORITHM_CLASSIFICATION_NETWORK_STAGE_1 (1)
	#define ANN_ALGORITHM_CLASSIFICATION_NETWORK_STAGE_2 (2)
	#define ANN_ALGORITHM_CLASSIFICATION_NETWORK_STAGE_3 (3)
	#define ANN_ALGORITHM_CLASSIFICATION_NETWORK_STAGE_12above (4)
	#define ANN_ALGORITHM_CLASSIFICATION_NETWORK_STAGE_3above (5)
	#define ANN_ALGORITHM_CLASSIFICATION_NETWORK_ALLOW_FOR_CASE_WHERE_INTERMEDIARY_NEURON_IS_CONNECTED_TO_ALL_INPUTS	//4a3w (the current algorithm doesn't prohibit this case)
	//#define ANN_ALGORITHM_CLASSIFICATION_NETWORK_CALCULATE_OUTPUT_AS_TOTAL_RATHER_THAN_AVERAGE	//disabled ANN4a3t
	//#define ANN_DEBUG_ALGORITHM_CLASSIFICATION_NETWORK_DISABLE_IDEAL_VALUE_UPDATING	//ANN4a3t temporarily enabled, disabled ANN4a11b
	//#define ANN_DEBUG_ALGORITHM_CLASSIFICATION_NETWORK_DISABLE_IDEAL_VALUE_UPDATING_FOR_PARENTS	//added ANN4a11bTEMP2
	//#define ANN_DEBUG_ALGORITHM_CLASSIFICATION_NETWORK6	//4a12b
	//#define ANN_DEBUG_ALGORITHM_CLASSIFICATION_NETWORK5	//4a3u
	//#define ANN_DEBUG_ALGORITHM_CLASSIFICATION_NETWORK4	//4a3u
	//#define ANN_DEBUG_ALGORITHM_CLASSIFICATION_NETWORK3	//4a3t
	//#define ANN_DEBUG_ALGORITHM_CLASSIFICATION_NETWORK2	//4a3t
	//#define ANN_DEBUG_ALGORITHM_CLASSIFICATION_NETWORK1	//4a3e
	#define ANN_ALGORITHM_CLASSIFICATION_NETWORK_CATEGORY_AVERAGE_DIFF_THRESHOLD (0.05)	//NEURON_SET = CATEGORY
	#define ANN_ALGORITHM_CLASSIFICATION_NETWORK_CATEGORY_INDIVIDUAL_DIFF_THRESHOLD (ANN_ALGORITHM_CLASSIFICATION_NETWORK_CATEGORY_AVERAGE_DIFF_THRESHOLD)
	#define ANN_ALGORITHM_CLASSIFICATION_NETWORK_MEMORY_TRACE_UPDATE_VALUE (0.1)
	#define ANN_ALGORITHM_CLASSIFICATION_NETWORK_MINIMUM_NUMBER_INPUTS_FOR_CATEGORY_NEURON (2)
	#define ANN_ALGORITHM_CLASSIFICATION_NETWORK_PRUNING	//ANN4a3b
	#ifdef ANN_ALGORITHM_CLASSIFICATION_NETWORK_PRUNING
		//#define ANN_ALGORITHM_CLASSIFICATION_NETWORK_PRUNING_OPTIMISE	//removed ANN 4a3d
		#define ANN_ALGORITHM_CLASSIFICATION_NETWORK_PRUNING_MINIMUM_MEMORY_TRACE_TO_RETAIN_CATEGORY_NEURON (20)	//2
	#endif
	#define ANN_ALGORITHM_CLASSIFICATION_NETWORK_STORE_OUTPUT_NEURONS	//ANN4a3b	//NOT USED (low priority)
	#define ANN_ALGORITHM_CLASSIFICATION_NETWORK_NETWORK_DISPLAY_HEIGHT (10.0)	//ANN4a3f
	//#define ANN_DISPLAY_DISABLE_SPRITES	//ANN4a3i	//disabled ANN4a4a
#elif defined ANN_ALGORITHM_SEPARATE_CLASSIFICATION_AND_MEMORY_NETWORKS
	#define ANN_ALGORITHM_MEMORY_NETWORK
	#ifdef ANN_ALGORITHM_MEMORY_NETWORK
		#define ANN_DEBUG_ALGORITHM_MEMORY_NETWORK
		#define ANN_ALGORITHM_MEMORY_NETWORK_OUTPUT_NEURON_FIRE_TRESHOLD_FOR_CLASSIFICATION_BIT (0.9)	//tweak this
		#define ANN_ALGORITHM_MEMORY_NETWORK_MEMORY_TRACE_UPDATE_VALUE (0.1)
		#define ANN_ALGORITHM_MEMORY_NETWORK_MEMORY_TRACE_TALLY_INCREMENT_VALUE (1.0)
	#endif	
	#define ANN_ALGORITHM_SEPARATE_CLASSIFICATION_AND_MEMORY_NETWORKS_OPTIMISE_CLASSIFICATION_NET	//added ANN 4a2a - executes backpropogation algorithm before creating memory trace
	#ifdef ANN_ALGORITHM_SEPARATE_CLASSIFICATION_AND_MEMORY_NETWORKS_OPTIMISE_CLASSIFICATION_NET
		#define ANN_ALGORITHM_BACKPROPAGATION	
	#endif
#else
	#define ANN_ALGORITHM_BACKPROPAGATION
#endif


//#define ANN_DEBUG_PARSER

#ifdef USE_RT
	#define TH_USE_RT_FOR_NEURAL_NETWORK_VEC_GRAPHICS
#else
	#define TH_USE_POVRAY_FOR_NEURAL_NETWORK_VEC_GRAPHICS
#endif

#define ANN_USE_MIN_NUM_EPOCHS_BEFORE_DECLARE_OVERFITTING_DATA
#ifdef ANN_USE_MIN_NUM_EPOCHS_BEFORE_DECLARE_OVERFITTING_DATA
	#define ANN_MIN_NUM_EPOCHS_BEFORE_DECLARE_OVERFITTING_DATA (50)	//10
#endif

//#define DEBUG_ANN_VERIFY_REAL_LIFE_ACCURACY_DIAGNOSTICS_CUTOFF
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

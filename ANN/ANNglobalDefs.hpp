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
 * File Name: ANNglobalsDefs.hpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2018 Baxter AI (baxterai.com)
 * Project: Artificial Neural Network (ANN)
 * Project Version: 3m12b 15-January-2018
 * Description: ANN specific global definitions
 *
 * Usage Examples:
 *
 * ANNwithRT.exe -idata new-thyroid.data -oxml neuralNet.xml -oldr neuralNetWithoutSprites.ldr -oldr2 neuralNetWithSprites.ldr -oppm neuralNet.ppm
 * ANNwithRT.exe -idata new-thyroid.data -oall neuralNet
 * ANNwithRT.exe -ixml neuralNet.xml -oall neuralNet
 * ANNwithRT.exe -idata new-thyroid.data -layers 5 -ineurons 5 -oneurons 3 -divtype 2 -oall neuralNet
 * ANNwithRT.exe -layers 5 -ineurons 10 oneurons 5 -oall neuralNet -notshow
 * ANNwithRT.exe -layers 5 -ineurons 10 oneurons 5 -oall neuralNet
 * ANNwithRT.exe -layers 5 -ineurons 10 -oneurons 5 -divtype 1 -oall neuralNet
 * ANNwithRT.exe -layers 5 -ineurons 10 -oneurons 5 -divtype 2 -oall neuralNet
 * ANNwithRT.exe -layers 5 -ineurons 10 -oneurons 5 -divtype 3 -oall neuralNet
 * ANNwithRT.exe -layers 5 -ineurons 10 -oneurons 5 -divtype 4 -oall neuralNet
 * ANNwithRT.exe -layers 5 -ineurons 16 -oneurons 4 -divtype 6 -oall neuralNet
 * ANNwithRT.exe -layers 5 -ineurons 5 -oneurons 3 -divtype 1 -usesubnets -numsubnets 2 -oall neuralNet
 * ANNwithRT.exe -layers 5 -ineurons 5 -oneurons 3 -divtype 1 -usesubnets -numsubnets 3 -oall neuralNet
 * ANNwithRT.exe -layers 5 -ineurons 8 -oneurons 4 -divtype 1 -usesubnets -numsubnets 2 -oall neuralNet
 * ANNwithRT.exe -layers 4 -ineurons 8 oneurons 4 -oall neuralNet
 * ANNwithRT.exe -ui
 *
 *******************************************************************************/


#ifndef HEADER_ANN_GLOBAL_DEFS
#define HEADER_ANN_GLOBAL_DEFS

#include "SHAREDglobalDefs.hpp"


#define ANN_DRAW_DISABLE_FILE_OUTPUT_NOTIFICATIONS


#ifdef USE_GIA
	#define ANN_EXPERIENCE_CLASS_PREDICTION_ERROR

	#define ANN_ALGORITHM_GIA_NEURAL_NETWORK
	#ifdef ANN_ALGORITHM_GIA_NEURAL_NETWORK
		//#define ANN_ALGORITHM_GIA_NEURAL_NETWORK_USE_ARTIFICIAL_INSTANCE_NEURON_SYNAPSES	//must be synced with GIA_NEURAL_NETWORK_USE_ARTIFICIAL_INSTANCE_NEURON_SYNAPSES
		#define ANN_ALGORITHM_GIA_NEURAL_NETWORK_MAX_SPECIFIC_CONCEPT_DEPTH (5)		//must be synced with GIA_NEURAL_NETWORK_MAX_SPECIFIC_CONCEPT_DEPTH
	#endif
#else
	#define ANN_XML_CONVERSION_DO_NOT_RECORD_EMPTY_LAYERS
#endif

//#define ANN_ALGORITHM_BREAKAWAY_NETWORK	//ANN3m1a
//#define ANN_ALGORITHM_CLASSIFICATION_NETWORK	//ANN3i3a
//#define ANN_ALGORITHM_SEPARATE_CLASSIFICATION_AND_MEMORY_NETWORKS //ANN3i1a
#ifdef ANN_ALGORITHM_BREAKAWAY_NETWORK
	#define ANN_ALGORITHM_BACKPROPAGATION
	//#define ANN_ALGORITHM_BREAKAWAY_NETWORK_ADD_SUPERNETS	//method 1
	#ifdef ANN_ALGORITHM_BREAKAWAY_NETWORK_ADD_SUPERNETS
		#define ANN_ALGORITHM_BREAKAWAY_NETWORK_ADD_SIZE_SUPERNET (10)	//add x new supernets
		#define ANN_ALGORITHM_BREAKAWAY_NETWORK_ADD_SUPERNETS_NUMBER_OF_NEW_LAYERS (3)	//x new layers per new supernet
	#else
		#define ANN_ALGORITHM_BREAKAWAY_NETWORK_ADD_LAYERS	//method 2
		#ifdef ANN_ALGORITHM_BREAKAWAY_NETWORK_ADD_LAYERS
			#define ANN_ALGORITHM_BREAKAWAY_NETWORK_ADD_SIZE_SUPERNET (10)	//add x new layers
		#endif
	#endif
	#define ANN_ALGORITHM_BREAKAWAY_NETWORK_LINK_WITH_ALL_PREVIOUS_LAYERS	//method 2b
#elif defined ANN_ALGORITHM_CLASSIFICATION_NETWORK
	//#define ANN_ALGORITHM_CLASSIFICATION_NETWORK_DISPLAY_EVOLUTION_OF_NETWORK	//ANN3i6a	//depreciated as requires recursive include file dependencies
	#define ANN_ALGORITHM_CLASSIFICATION_NETWORK_STAGE_1 (1)
	#define ANN_ALGORITHM_CLASSIFICATION_NETWORK_STAGE_2 (2)
	#define ANN_ALGORITHM_CLASSIFICATION_NETWORK_STAGE_3 (3)
	#define ANN_ALGORITHM_CLASSIFICATION_NETWORK_STAGE_12above (4)
	#define ANN_ALGORITHM_CLASSIFICATION_NETWORK_STAGE_3above (5)
	#define ANN_ALGORITHM_CLASSIFICATION_NETWORK_ALLOW_FOR_CASE_WHERE_INTERMEDIARY_NEURON_IS_CONNECTED_TO_ALL_INPUTS	//4a3w (the current algorithm doesn't prohibit this case)
	//#define ANN_ALGORITHM_CLASSIFICATION_NETWORK_CALCULATE_OUTPUT_AS_TOTAL_RATHER_THAN_AVERAGE	//disabled ANN3i3t
	#define ANN_VERBOSE_ALGORITHM_CLASSIFICATION_NETWORK_ENABLE_IDEAL_VALUE_UPDATING	//ANN3i3t temporarily disabled, reenabled ANN3i11b
	#ifdef ANN_VERBOSE_ALGORITHM_CLASSIFICATION_NETWORK_ENABLE_IDEAL_VALUE_UPDATING
		#define ANN_VERBOSE_ALGORITHM_CLASSIFICATION_NETWORK_ENABLE_IDEAL_VALUE_UPDATING_FOR_PARENTS	//added ANN3i11b
		#define ANN_VERBOSE_ALGORITHM_CLASSIFICATION_NETWORK_MERGE_SIMILAR_NEURONS	//added ANN3i13a
		#ifdef ANN_VERBOSE_ALGORITHM_CLASSIFICATION_NETWORK_MERGE_SIMILAR_NEURONS
			#define ANN_VERBOSE_ALGORITHM_CLASSIFICATION_NETWORK_MERGE_SIMILAR_NEURONS_THRESHOLD (0.05)
		#endif
	#endif
	#define ANN_VERBOSE_ALGORITHM_CLASSIFICATION_NETWORK_CULL_REDUNDANT_NEURONS	//ANN3i14a - remove neurons which are being connected to an intermediary neuron and its child only
	#define ANN_ALGORITHM_CLASSIFICATION_NETWORK_CATEGORY_AVERAGE_DIFF_THRESHOLD (0.05)	//NEURON_SET = CATEGORY
	#define ANN_ALGORITHM_CLASSIFICATION_NETWORK_CATEGORY_INDIVIDUAL_DIFF_THRESHOLD (ANN_ALGORITHM_CLASSIFICATION_NETWORK_CATEGORY_AVERAGE_DIFF_THRESHOLD)
	#define ANN_ALGORITHM_CLASSIFICATION_NETWORK_MEMORY_TRACE_UPDATE_VALUE (0.1)
	#define ANN_ALGORITHM_CLASSIFICATION_NETWORK_MINIMUM_NUMBER_INPUTS_FOR_CATEGORY_NEURON (2)
	#define ANN_ALGORITHM_CLASSIFICATION_NETWORK_PRUNING	//ANN3i3b
	#ifdef ANN_ALGORITHM_CLASSIFICATION_NETWORK_PRUNING
		//#define ANN_ALGORITHM_CLASSIFICATION_NETWORK_PRUNING_OPTIMISE	//removed ANN 4a3d
		#define ANN_ALGORITHM_CLASSIFICATION_NETWORK_PRUNING_MINIMUM_MEMORY_TRACE_TO_RETAIN_CATEGORY_NEURON (100)	//2
	#endif
	#define ANN_ALGORITHM_CLASSIFICATION_NETWORK_STORE_OUTPUT_NEURONS	//ANN3i3b	//NOT USED (low priority)
	#define ANN_ALGORITHM_CLASSIFICATION_NETWORK_NETWORK_DISPLAY_HEIGHT (10.0)	//ANN3i3f
	//#define ANN_DISPLAY_DISABLE_SPRITES	//ANN3i3i	//disabled ANN3i4a
#elif defined ANN_ALGORITHM_SEPARATE_CLASSIFICATION_AND_MEMORY_NETWORKS
	#define ANN_ALGORITHM_MEMORY_NETWORK
	#ifdef ANN_ALGORITHM_MEMORY_NETWORK
		#define ANN_VERBOSE_ALGORITHM_MEMORY_NETWORK
		#define ANN_ALGORITHM_MEMORY_NETWORK_OUTPUT_NEURON_FIRE_TRESHOLD_FOR_CLASSIFICATION_BIT (0.9)	//tweak this
		#define ANN_ALGORITHM_MEMORY_NETWORK_MEMORY_TRACE_UPDATE_VALUE (0.1)
		#define ANN_ALGORITHM_MEMORY_NETWORK_MEMORY_TRACE_TALLY_INCREMENT_VALUE (1.0)
	#endif
	#define ANN_ALGORITHM_SEPARATE_CLASSIFICATION_AND_MEMORY_NETWORKS_OPTIMISE_CLASSIFICATION_NET	//added ANN 4a2a - executes backpropogation algorithm before creating memory trace
	#ifdef ANN_ALGORITHM_SEPARATE_CLASSIFICATION_AND_MEMORY_NETWORKS_OPTIMISE_CLASSIFICATION_NET
		#define ANN_ALGORITHM_BACKPROPAGATION_NETWORK
	#endif
#else
	#define ANN_ALGORITHM_BACKPROPAGATION_NETWORK
#endif

#ifdef ANN_ALGORITHM_BACKPROPAGATION_NETWORK
	#define ANN_ALGORITHM_BACKPROPAGATION
#endif
#ifdef ANN_ALGORITHM_BACKPROPAGATION
	#ifdef USE_GIA
		#define	MU 0.00001F	//backpropagation learning rate
	#else
		#define	MU 0.1F		//backpropagation learning rate
	#endif
#endif



#ifdef USE_RT
	#define TH_USE_RT_FOR_NEURAL_NETWORK_VEC_GRAPHICS
#else
	#define TH_USE_POVRAY_FOR_NEURAL_NETWORK_VEC_GRAPHICS
#endif

#define ANN_USE_MIN_NUM_EPOCHS_BEFORE_DECLARE_OVERFITTING_DATA
#ifdef ANN_USE_MIN_NUM_EPOCHS_BEFORE_DECLARE_OVERFITTING_DATA
	#define ANN_MIN_NUM_EPOCHS_BEFORE_DECLARE_OVERFITTING_DATA (50)	//10
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

	//ANN, LRRC, + OR;
//Test variables (need optimisation)
#define MAX_ANN_BACK_PROPAGATION_ERROR (1000.0)



#endif

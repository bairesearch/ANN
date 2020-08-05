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
 * File Name: ANNalgorithmSequenceGrammarNetworkTraining.cpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2020 Baxter AI (baxterai.com)
 * Project: Artificial Neural Network (ANN)
 * Project Version: 3n2a 10-March-2020
 * Comments:
 * /
 *******************************************************************************/


#ifndef HEADER_ANN_ALGORITHM_SEQUENCE_GRAMMAR_NETWORK_TRAINING
#define HEADER_ANN_ALGORITHM_SEQUENCE_GRAMMAR_NETWORK_TRAINING

#include "ANNglobalDefs.hpp"

#ifdef ANN_ALGORITHM_SEQUENCE_GRAMMAR_NETWORK

#include "GIAtranslatorOperations.hpp"
#include "GIApreprocessorWordIdentification.hpp"
#include "GIApreprocessor.hpp"
#include "GIAtranslatorOperations.hpp"
#include "GIApreprocessorSentenceClass.hpp"
#include "GIAtxtRelTranslatorNeuralNetworkPropagateCompactGenerate.hpp"
#include "GIAtxtRelTranslatorNeuralNetworkFormation.hpp"
#include "GIAtxtRelTranslatorNeuralNetworkOperations.hpp"
#include "GIAtxtRelTranslatorPermutations.hpp"
#include "GIApreprocessorPOStagger.hpp"
#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_SEQUENCE_GRAMMAR_PRELIMTEST_GENERATE_CLASSIFICATION_NET_INPUT_DATASET_VIA_ANN
#include "GIApreprocessorPOStaggerDatabase.hpp"
#endif


class ANNalgorithmSequenceGrammarNetworkTrainingClass
{	 
	private: SHAREDvarsClass SHAREDvars;
	private: GIApreprocessorClass GIApreprocessor;
	private: GIApreprocessorWordIdentificationClass GIApreprocessorWordIdentification;
	private: GIApreprocessorWordClassClass GIApreprocessorWordClassObject;
	private: GIAtranslatorOperationsClass GIAtranslatorOperations;	
	private: GIApreprocessorSentenceClass GIApreprocessorSentenceClassObject;
	private: GIAtxtRelTranslatorNeuralNetworkPropagateCompactGenerateClass GIAtxtRelTranslatorNeuralNetworkPropagateCompactGenerate;
	private: GIAtxtRelTranslatorNeuralNetworkFormationClass GIAtxtRelTranslatorNeuralNetworkFormation;
	private: GIAtxtRelTranslatorNeuralNetworkOperationsClass GIAtxtRelTranslatorNeuralNetworkOperations;
	private: GIAtxtRelTranslatorPermutationsClass GIAtxtRelTranslatorPermutations;
	private: GIApreprocessorPOStaggerClass GIApreprocessorPOStagger;
	#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_SEQUENCE_GRAMMAR_PRELIMTEST_GENERATE_CLASSIFICATION_NET_INPUT_DATASET_VIA_ANN
	private: GIApreprocessorPOStaggerDatabaseClass GIApreprocessorPOStaggerDatabase;
	#endif
	
	public: bool trainNeuralNetworkSequenceGrammar(const string inputFileName);
		#ifdef GIA_TXT_REL_TRANSLATOR_NEURAL_NETWORK_SEQUENCE_GRAMMAR_PRELIMTEST_GENERATE_CLASSIFICATION_NET_INPUT_DATASET_VIA_ANN
		private: bool writeExperiencesToFile(ANNexperience* firstExperienceInList, const string dataFileName);
		#endif
		private: bool initialiseGIA(GIAtranslatorVariablesClass* translatorVariables);		
		private: bool extractSentencesAndGenerateNetwork(GIAtranslatorVariablesClass* translatorVariables, vector<GIAtxtRelTranslatorRulesGroupType*>* GIAtxtRelTranslatorRulesGroupTypes, string* lineText, ANNexperience* firstExperienceInList);
			private: bool generateNetwork(GIAtranslatorVariablesClass* translatorVariables, vector<GIAtxtRelTranslatorRulesGroupType*>* GIAtxtRelTranslatorRulesGroupTypes, GIApreprocessorSentence* firstGIApreprepreprocessorSentenceInList, ANNexperience* firstExperienceInList);
				private: bool generateNetworkSentence(GIAtranslatorVariablesClass* translatorVariables, vector<GIAtxtRelTranslatorRulesGroupType*>* GIAtxtRelTranslatorRulesGroupTypes, vector<GIApreprocessorPlainTextWord*>* sentence, vector<uint64_t>* POSambiguityInfoPermutationTemp, ANNexperience* currentExperienceInList);

};

#endif

#endif

 /*******************************************************************************
 *
 * License: GNU Affero General Public License version 3 (AGPLv3)
 *
 *******************************************************************************/

/*******************************************************************************
 *
 * File Name: ANNalgorithmSequenceGrammarNetwork.cpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2022 Baxter AI (baxterai.com)
 * Project: Artificial Neural Network (ANN)
 * Project Version: 3q1a 05-June-2022
 * Comments:
 * /
 *******************************************************************************/


#ifndef HEADER_ANN_ALGORITHM_SEQUENCE_GRAMMAR_NETWORK_TRAINING
#define HEADER_ANN_ALGORITHM_SEQUENCE_GRAMMAR_NETWORK_TRAINING

#include "ANNglobalDefs.hpp"

#ifdef ANN_ALGORITHM_SEQUENCE_GRAMMAR_NETWORK

//depreciated @ANN3n6a
/*
#include "GIAtranslatorOperations.hpp"
#include "GIApreprocessorWordIdentification.hpp"
#include "GIApreprocessor.hpp"
#include "GIAtranslatorOperations.hpp"
#include "GIApreprocessorSentenceClass.hpp"
#include "GIAposRelTranslatorSANIPropagateCompactGenerate.hpp"
#include "GIAposRelTranslatorSANIFormation.hpp"
#include "GIAposRelTranslatorSANIOperations.hpp"
#include "GIAposRelTranslatorPermutations.hpp"
#include "GIApreprocessorPOStagger.hpp"
#ifdef GIA_POS_REL_TRANSLATOR_NEURAL_NETWORK_SEQUENCE_GRAMMAR_PRELIMTEST_GENERATE_CLASSIFICATION_NET_INPUT_DATASET_VIA_ANN
#include "GIApreprocessorPOStaggerDatabase.hpp"
#endif


class ANNalgorithmSequenceGrammarNetworkClass
{	 
	private: SHAREDvarsClass SHAREDvars;
	private: GIApreprocessorClass GIApreprocessor;
	private: GIApreprocessorWordIdentificationClass GIApreprocessorWordIdentification;
	private: GIApreprocessorWordClassClass GIApreprocessorWordClassObject;
	private: GIAtranslatorOperationsClass GIAtranslatorOperations;	
	private: GIApreprocessorSentenceClass GIApreprocessorSentenceClassObject;
	private: GIAposRelTranslatorSANIPropagateCompactGenerateClass GIAposRelTranslatorSANIPropagateCompactGenerate;
	private: GIAposRelTranslatorSANIFormationClass GIAposRelTranslatorSANIFormation;
	private: GIAposRelTranslatorSANIOperationsClass GIAposRelTranslatorSANIOperations;
	private: GIAposRelTranslatorPermutationsClass GIAposRelTranslatorPermutations;
	private: GIApreprocessorPOStaggerClass GIApreprocessorPOStagger;
	#ifdef GIA_POS_REL_TRANSLATOR_NEURAL_NETWORK_SEQUENCE_GRAMMAR_PRELIMTEST_GENERATE_CLASSIFICATION_NET_INPUT_DATASET_VIA_ANN
	private: GIApreprocessorPOStaggerDatabaseClass GIApreprocessorPOStaggerDatabase;
	#endif
	
	public: bool trainNeuralNetworkSequenceGrammar(const string inputFileName);
		#ifdef GIA_POS_REL_TRANSLATOR_NEURAL_NETWORK_SEQUENCE_GRAMMAR_PRELIMTEST_GENERATE_CLASSIFICATION_NET_INPUT_DATASET_VIA_ANN
		private: bool writeExperiencesToFile(ANNexperience* firstExperienceInList, const string dataFileName);
		#endif
		private: bool initialiseGIA(GIAtranslatorVariablesClass* translatorVariables);		
		private: bool extractSentencesAndGenerateNetwork(GIAtranslatorVariablesClass* translatorVariables, vector<GIAposRelTranslatorRulesGroupType*>* GIAposRelTranslatorRulesGroupTypes, string* lineText, ANNexperience* firstExperienceInList);
			private: bool generateNetwork(GIAtranslatorVariablesClass* translatorVariables, vector<GIAposRelTranslatorRulesGroupType*>* GIAposRelTranslatorRulesGroupTypes, GIApreprocessorSentence* firstGIApreprepreprocessorSentenceInList, ANNexperience* firstExperienceInList);
				private: bool generateNetworkSentence(GIAtranslatorVariablesClass* translatorVariables, vector<GIAposRelTranslatorRulesGroupType*>* GIAposRelTranslatorRulesGroupTypes, vector<GIApreprocessorPlainTextWord*>* sentence, vector<uint64_t>* POSambiguityInfoPermutationTemp, ANNexperience* currentExperienceInList);

};
*/

#endif

#endif

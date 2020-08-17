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
 * Project Version: 3n7b 17-August-2020
 * Comments:
 * /
 *******************************************************************************/


#include "ANNalgorithmSequenceGrammarNetworkTraining.hpp"

#ifdef ANN_ALGORITHM_SEQUENCE_GRAMMAR_NETWORK

//depreciated @ANN3n6a
/*
bool ANNalgorithmSequenceGrammarNetworkTrainingClass::trainNeuralNetworkSequenceGrammar(const string inputFileName)
{
	bool result = true;
	
	string text = SHAREDvars.getFileContents(inputFileName);

	ANNexperience* firstExperienceInList = new ANNexperience();
	
	GIAtranslatorVariablesClass* translatorVariables;
	initialiseGIA(translatorVariables);
	
	//extract from GIAposRelTranslatorClass::parseTxtfileAndCreateSemanticNetworkBasedUponSemanticDependencyParsedSentences
	#ifdef GIA_POS_REL_TRANSLATOR_NEURAL_NETWORK_SEQUENCE_GRAMMAR_PRELIMTEST_GENERATE_CLASSIFICATION_NET_INPUT_DATASET_VIA_ANN
	#ifndef GIA_POS_REL_TRANSLATOR_NEURAL_NETWORK_SEQUENCE_GRAMMAR_PRELIMTEST_GENERATE_CLASSIFICATION_NET_INPUT_DATASET_VIA_ANN_EMULATE_EXACT_POS_TAGGER_DATABASE_OUTPUT
	//FUTURE GIA - upgrade GIA_POS_REL_TRANSLATOR_NEURAL_NETWORK_SEQUENCE_GRAMMAR to read/write GIAposRelTranslatorRulesGenerated.xml

	vector<GIAposRelTranslatorRulesGroupType*>* GIAposRelTranslatorRulesGroupTypes = new vector<GIAposRelTranslatorRulesGroupType*>;
	vector<XMLparserTag*>* GIAposRelTranslatorRulesTokenLayers = new vector<XMLparserTag*>;
	if(!GIAposRelTranslatorRules.extractGIAposRelTranslatorRules(GIAposRelTranslatorRulesGroupTypes, GIAposRelTranslatorRulesTokenLayers))
	{
		result = false;
	}
	if(!GIAposRelTranslatorSANIFormation.createGIAposRelTranslatorSANI(GIAposRelTranslatorRulesTokenLayers, GIAposRelTranslatorRulesGroupTypes))
	{
		result = false;
	}	
	#endif
	#endif
	
	extractSentencesAndGenerateNetwork(translatorVariables, GIAposRelTranslatorRulesGroupTypes, &text, firstExperienceInList);
	
	#ifdef GIA_POS_REL_TRANSLATOR_NEURAL_NETWORK_SEQUENCE_GRAMMAR_PRELIMTEST_GENERATE_CLASSIFICATION_NET_INPUT_DATASET_VIA_ANN
	if(!writeExperiencesToFile(firstExperienceInList, GIA_POS_REL_TRANSLATOR_NEURAL_NETWORK_SEQUENCE_GRAMMAR_PRELIMTEST_GENERATE_CLASSIFICATION_NET_ANN_DATAFILE_NAME))
	{
		result = false;
	}	
	#else
	cerr << "ANNalgorithmSequenceGrammarNetworkTrainingClass::generateNetworkSentence error: ANN_ALGORITHM_SEQUENCE_GRAMMAR_NETWORK currently requires GIA_POS_REL_TRANSLATOR_NEURAL_NETWORK_SEQUENCE_GRAMMAR_PRELIMTEST_GENERATE_CLASSIFICATION_NET_INPUT_DATASET_VIA_ANN" << endl;
	exit(EXIT_ERROR);
	#endif

	return result;
}

#ifdef GIA_POS_REL_TRANSLATOR_NEURAL_NETWORK_SEQUENCE_GRAMMAR_PRELIMTEST_GENERATE_CLASSIFICATION_NET_INPUT_DATASET_VIA_ANN
bool ANNalgorithmSequenceGrammarNetworkTrainingClass::writeExperiencesToFile(ANNexperience* firstExperienceInList, const string dataFileName)
{
	bool result = true;

	vector<string> POStaggerOutputClassificationNetExperiencesData;
		
	//generate predictions batch file
	ANNexperience* currentExperienceInList = firstExperienceInList;
	while(currentExperienceInList->next != NULL)
	{
		string experienceInputString = GIApreprocessorPOStaggerDatabase.externalANNgenerateExperienceInputStandalone(currentExperienceInList);
		POStaggerOutputClassificationNetExperiencesData.push_back(experienceInputString);
		
		currentExperienceInList = currentExperienceInList->next;
	}
	
	SHAREDvars.writeStringListToFile(dataFileName, &POStaggerOutputClassificationNetExperiencesData);
}
#endif


bool ANNalgorithmSequenceGrammarNetworkTrainingClass::initialiseGIA(GIAtranslatorVariablesClass* translatorVariables)
{
	bool result = true;
	
	translatorVariables = new GIAtranslatorVariablesClass();	//NOTUSED
	translatorVariables->firstGIApreprocessorSentenceInList = new GIApreprocessorSentence();
	
	string lrpFolder = string(GIA_LRP_FOLDER_DEFAULT) + CHAR_FOLDER_DELIMITER;
	if(!GIApreprocessorWordIdentification.initialiseLRP(lrpFolder, true))
	{
		result = false;
	}
	
	return result;
}

bool ANNalgorithmSequenceGrammarNetworkTrainingClass::extractSentencesAndGenerateNetwork(GIAtranslatorVariablesClass* translatorVariables, vector<GIAposRelTranslatorRulesGroupType*>* GIAposRelTranslatorRulesGroupTypes, string* text, ANNexperience* firstExperienceInList)
{
	bool result = true;
	
	bool interpretNewLinesAsNewSentences = false;	//NB NLC interprets new lines as new sentences
	bool splitMultisentenceLines = false;	//not currently supported by NLCI, as line detection must match
	if(!GIApreprocessor.createPreprocessSentences(*text, translatorVariables->firstGIApreprocessorSentenceInList, interpretNewLinesAsNewSentences, splitMultisentenceLines))
	{
		result = false;
	}
	
	//code replicated from GIAposRelTranslatorClass::parseTxtfileAndCreateSemanticNetworkBasedUponSemanticDependencyParsedSentences
	#ifdef GIA_POS_REL_TRANSLATOR_RULES_DEFINE_WORD_TRANSLATOR_SENTENCE_ENTITY_INDEX_AT_START
	GIApreprocessorSentence* currentGIApreprocessorSentenceInList = translatorVariables->firstGIApreprocessorSentenceInList;
	while(currentGIApreprocessorSentenceInList->next != NULL)
	{
		vector<GIApreprocessorPlainTextWord*>* sentenceContents = GIApreprocessorSentenceClassObject.getSentenceContents(currentGIApreprocessorSentenceInList);
		for(int w=0; w<sentenceContents->size(); w++)
		{	
			GIApreprocessorPlainTextWord* currentWord = sentenceContents->at(w);
			currentWord->translatorSentenceEntityIndex = GIAtranslatorOperations.convertSentenceContentsIndexToEntityIndex(w);
			#ifdef GIA_POS_REL_TRANSLATOR_NEURAL_NETWORK
			currentWord->translatorSentenceWordIndex = w;
			#endif
		}
		currentGIApreprocessorSentenceInList = currentGIApreprocessorSentenceInList->next;
	}
	#endif
	
	if(!generateNetwork(translatorVariables, GIAposRelTranslatorRulesGroupTypes, translatorVariables->firstGIApreprocessorSentenceInList, firstExperienceInList))
	{
		result = false;
	}
	
	return result;
}

//code extracted from GIAposRelTranslatorPermutations
bool ANNalgorithmSequenceGrammarNetworkTrainingClass::generateNetwork(GIAtranslatorVariablesClass* translatorVariables, vector<GIAposRelTranslatorRulesGroupType*>* GIAposRelTranslatorRulesGroupTypes, GIApreprocessorSentence* firstGIApreprepreprocessorSentenceInList, ANNexperience* firstExperienceInList)
{
	bool result = true;
	
	ANNexperience* currentExperienceInList = firstExperienceInList;

	int line = 0;
	GIApreprocessorSentence* currentGIApreprepreprocessorSentenceInList = firstGIApreprepreprocessorSentenceInList;
	while(currentGIApreprepreprocessorSentenceInList->next != NULL)
	{
		vector<uint64_t> POSambiguityInfoPermutation;
		//vector<GIApreprocessorPlainTextWord*>* sentence = &(currentGIApreprepreprocessorSentenceInList->sentenceContentsOriginal);
		vector<GIApreprocessorPlainTextWord*>* sentenceContents = GIApreprocessorSentenceClassObject.getSentenceContents(currentGIApreprepreprocessorSentenceInList);
		#ifdef GIA_POS_REL_TRANSLATOR_NEURAL_NETWORK_ADD_EXPLICIT_WORD_REFERENCES_AS_INDEPENDENT_POS_PERMUTATIONS
		vector<string>* explicitWordList = GIAposRelTranslatorSANIFormation.getExplicitWordList();
		#else
		vector<string>* explicitWordList = NULL;
		#endif
		if(!GIApreprocessorPOStagger.determinePOSambiguityInfoWrapper(sentenceContents, &POSambiguityInfoPermutation, explicitWordList))
		{
			result = false;
		}
		#ifdef GIA_POS_REL_TRANSLATOR_RULES_ITERATE_OVER_UNAMBIGUOUS_POS_PERMUTATIONS_AT_START
		int minIndexOfMatchesFoundBackupOptimum = GIAposRelTranslatorSANIOperations.calculateMinIndexOfMatchesFound(sentenceContents);
		vector<vector<uint64_t>*> POSambiguityInfoUnambiguousPermutationArray;
		vector<uint64_t>* POSambiguityInfoUnambiguousPermutationNew = new vector<uint64_t>(POSambiguityInfoPermutation.size());
		POSambiguityInfoUnambiguousPermutationArray.push_back(POSambiguityInfoUnambiguousPermutationNew);
		GIApreprocessorPOStagger.generatePOSambiguityInfoUnambiguousPermutationArray(&POSambiguityInfoUnambiguousPermutationArray, &POSambiguityInfoPermutation, POSambiguityInfoUnambiguousPermutationNew, 0);
		#endif
	
		#ifdef GIA_POS_REL_TRANSLATOR_RULES_ITERATE_OVER_UNAMBIGUOUS_POS_PERMUTATIONS_AT_START
		for(int i=0; i<POSambiguityInfoUnambiguousPermutationArray.size(); i++)
		{
			vector<uint64_t>* POSambiguityInfoPermutationTemp = (POSambiguityInfoUnambiguousPermutationArray)[i];
			GIApreprocessorPOStagger.setSentenceContentsWordsUnambiguousPOSindex(sentenceContents, POSambiguityInfoPermutationTemp);
	
			#ifdef GIA_DEBUG_POS_REL_TRANSLATOR_NEURAL_NETWORK_PROPAGATE
			cout << "POSambiguityInfoUnambiguousPermutationArray index = " << i << endl;
			//GIApreprocessorPOStagger.printPOSambiguityInfoPermutationAssumeUnambiguous(POSambiguityInfoPermutationTemp);	//printPOSambiguityInfoPermutation
			#endif
			#ifdef GIA_DEBUG_POS_REL_TRANSLATOR_RULES_PRINT_PARSE_PROCESS3
			cout << "POSambiguityInfoUnambiguousPermutationArray index = " << i << endl;
			GIApreprocessorPOStagger.printPOSambiguityInfoPermutationAssumeUnambiguous(POSambiguityInfoPermutationTemp);		
			#endif
		#else
		for(int w=0; w<sentenceContents->size(); w++)
		{
			(sentenceContents->at(w))->POSambiguityInfo = (*POSambiguityInfoPermutation)[w];
		}
		#endif
			if(!generateNetworkSentence(translatorVariables, sentenceContents, POSambiguityInfoPermutationTemp, currentExperienceInList))
			{
				result = false;
			}
		#ifdef GIA_POS_REL_TRANSLATOR_RULES_ITERATE_OVER_UNAMBIGUOUS_POS_PERMUTATIONS_AT_START
		}
		#endif			
		
		currentGIApreprepreprocessorSentenceInList = currentGIApreprepreprocessorSentenceInList->next;
		line++;
		
		currentExperienceInList = currentExperienceInList->next;
	}
	
	return result;
}

bool ANNalgorithmSequenceGrammarNetworkTrainingClass::generateNetworkSentence(GIAtranslatorVariablesClass* translatorVariables, vector<GIAposRelTranslatorRulesGroupType*>* GIAposRelTranslatorRulesGroupTypes, vector<GIApreprocessorPlainTextWord*>* sentence, vector<uint64_t>* POSambiguityInfoPermutationTemp, ANNexperience* currentExperienceInList)
{
	bool result = true;

	
	#ifdef GIA_POS_REL_TRANSLATOR_NEURAL_NETWORK_SEQUENCE_GRAMMAR_PRELIMTEST_GENERATE_CLASSIFICATION_NET_INPUT_DATASET_VIA_ANN
	#ifdef GIA_POS_REL_TRANSLATOR_NEURAL_NETWORK_SEQUENCE_GRAMMAR_PRELIMTEST_GENERATE_CLASSIFICATION_NET_INPUT_DATASET_VIA_ANN_EMULATE_EXACT_POS_TAGGER_DATABASE_OUTPUT
	uint64_t centreWordUnambiguousPOSindexTemp = GIA_PREPROCESSOR_POS_TAGGER_DATABASE_NEURAL_NETWORK_INTERNAL_CLASSIFICATION_NET_TRAIN_EXPERIENCE_OUTPUT_VALUE_NOT_USED;	//see GIA_PREPROCESSOR_POS_TAGGER_DATABASE_NEURAL_NETWORK_INTERNAL_CLASSIFICATION_NET
	GIApreprocessorPOStagger.generateANNexperienceFromPOSambiguityInfoPermutationStandalone(POSambiguityInfoPermutationTemp, centreWordUnambiguousPOSindexTemp, currentExperienceInList);
	#else
	bool createNewConnections = true; 
	if(GIAposRelTranslatorSANIPropagateCompactGenerate.executeTxtRelTranslatorNeuralNetwork(translatorVariables, &GIAposRelTranslatorRulesGroupTypes, sentenceContents, createNewConnections)
	{
		result = true;
	}

	
	//currentExperience->classTargetValue = GIA_PREPROCESSOR_POS_TAGGER_DATABASE_NEURAL_NETWORK_INTERNAL_CLASSIFICATION_NET_TRAIN_EXPERIENCE_OUTPUT_VALUE_NOT_USED;
	//ANNexperienceInput* currentExperienceInput = currentExperience->firstExperienceInput;
	//for(int firstLayerNeuronIndex=0; firstLayerNeuronIndex<firstLayer.size(); firstLayerNeuronIndex++)
	//{
	//	GIAposRelTranslatorRulesGroupNeuralNetwork* firstLayerNeuronGroup = firstLayer[firstLayerNeuronIndex]; 
	//
	//	//code extracted from GIApreprocessorPOStaggerClass::addPOStaggerDatabaseEntry/generateANNexperienceFromPOSambiguityInfoPermutation
	//	currentExperienceInput->inputValue = firstLayerNeuronGroup->wordDataTemp.wordPOStype;
	//	currentExperienceInput->next = new ANNexperienceInput();
	//	currentExperienceInput = currentExperienceInput->next;
	//}
			
	#endif
	currentExperienceInList->next = new ANNexperience();			
	#else
	cerr << "ANNalgorithmSequenceGrammarNetworkTrainingClass::generateNetworkSentence error: ANN_ALGORITHM_SEQUENCE_GRAMMAR_NETWORK currently requires GIA_POS_REL_TRANSLATOR_NEURAL_NETWORK_SEQUENCE_GRAMMAR_PRELIMTEST_GENERATE_CLASSIFICATION_NET_INPUT_DATASET_VIA_ANN" << endl;
	exit(EXIT_ERROR);
	#endif

	
	
	return result;
}
*/

#endif







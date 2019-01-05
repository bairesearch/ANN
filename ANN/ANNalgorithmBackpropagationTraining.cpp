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
 * File Name: ANNalgorithmBackpropagationTraining.cpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2018 Baxter AI (baxterai.com)
 * Project: Artificial Neural Network (ANN)
 * Project Version: 3m15a 24-April-2018
 * Comments:
 * /
 *******************************************************************************/


#include "ANNalgorithmBackpropagationTraining.hpp"
//#include "ANNformation.hpp"
//#include "ANNparser.hpp"

#ifdef ANN_ALGORITHM_BACKPROPAGATION

double ANNalgorithmBackpropagationTrainingClass::calculateExperienceErrorForHypotheticalDecisionBackpropagation(ANNneuron* firstInputNeuronInNetwork, ANNneuron* firstOutputNeuronInNetwork, const int64_t numberOfInputNeurons, const int64_t numberOfOutputNeurons, ANNexperience* experience)
{
	double experienceBackPropagationPassError;

	resetInputsAndClassTargets(firstInputNeuronInNetwork, firstOutputNeuronInNetwork, numberOfInputNeurons, numberOfOutputNeurons, experience);

	//VERIFICATION METHOD 1;
	/*
	storeNeuralNetworkBiasAndWeights(firstInputNeuronInNetwork);
	experienceBackPropagationPassError = ANNalgorithmBackpropagationUpdate.ANNbackPropogationPass(firstInputNeuronInNetwork, firstOutputNeuronInNetwork);
	restoreNeuralNetworkWithStoredBiasAndWeights(firstInputNeuronInNetwork);
	*/
	
	//VERIFICATION METHOD 2;
	ANNalgorithmBackpropagationUpdate.backPropogationForwardPassStep(firstInputNeuronInNetwork);
	experienceBackPropagationPassError = ANNalgorithmBackpropagationUpdate.calculateErrorOfBackPropPass(firstOutputNeuronInNetwork);

	/*
	if(experienceBackPropagationPassError > MAX_ANN_BACK_PROPAGATION_ERROR)
	{
		experienceBackPropagationPassError = (MAX_ANN_BACK_PROPAGATION_ERROR-1.0);
	}
	*/

	return experienceBackPropagationPassError;
}

bool ANNalgorithmBackpropagationTrainingClass::calculateIdealClassTargetOfInputExperience(ANNneuron* firstInputNeuronInNetwork, ANNneuron* firstOutputNeuronInNetwork, const int64_t numberOfInputNeurons, const int64_t numberOfOutputNeurons, ANNexperience* experience, int* idealClassTarget, double* experienceBackPropagationPassError)
{
	bool result = false;
	
	resetInputsAndClassTargets(firstInputNeuronInNetwork, firstOutputNeuronInNetwork, numberOfInputNeurons, numberOfOutputNeurons, experience);
	ANNalgorithmBackpropagationUpdate.backPropogationForwardPassStep(firstInputNeuronInNetwork);

	double maxOutput = 0.0;
	int outputNeuronIndexWithMaxOutput = INT_DEFAULT_VALUE;
	ANNneuron* currentNeuronReference = firstOutputNeuronInNetwork;
	for(int64_t i = 0; i < numberOfOutputNeurons; i++)
	{
		cout << "currentNeuronReference->output = " << currentNeuronReference->output << ", i = " << i << endl;
		if(currentNeuronReference->output > maxOutput)
		{
			outputNeuronIndexWithMaxOutput = i;
			maxOutput = currentNeuronReference->output;
		}
		currentNeuronReference = currentNeuronReference->nextNeuron;
	}
	
	currentNeuronReference = firstOutputNeuronInNetwork;
	for(int64_t i = 0; i < numberOfOutputNeurons; i++)
	{		
		currentNeuronReference->classTarget = 0.0F;
		if(i == outputNeuronIndexWithMaxOutput)
		{
			currentNeuronReference->classTarget = 1.0F;
		}
		currentNeuronReference = currentNeuronReference->nextNeuron;
	}

	*experienceBackPropagationPassError = ANNalgorithmBackpropagationUpdate.calculateErrorOfBackPropPass(firstOutputNeuronInNetwork);
	
	if(*experienceBackPropagationPassError < MAX_ANN_BACK_PROPAGATION_ERROR)
	{
		*idealClassTarget = outputNeuronIndexWithMaxOutput;
		result = true;
	}
}



//DO NOT USE THIS; must use trainNeuralNetworkBackpropagation/trainNeuralNetworkBackpropagationSimple instead!
void ANNalgorithmBackpropagationTrainingClass::feedNeuralNetworkWithASetOfExperiencesBackpropagation(ANNneuron* firstInputNeuron, ANNneuron* firstOutputNeuron, const int numberOfInputNeurons, const int numberOfOutputNeurons, ANNexperience* firstExperienceInList)
{
	ANNexperience* currentExperience = firstExperienceInList;

	while(currentExperience->next != NULL)
	{
		feedNeuralNetworkWithASetOfExperiencesBackpropagation(firstInputNeuron, firstOutputNeuron, numberOfInputNeurons, numberOfOutputNeurons, currentExperience);

		currentExperience = currentExperience->next;
	}
}

void ANNalgorithmBackpropagationTrainingClass::feedNeuralNetworkWithExperienceBackpropagation(ANNneuron* firstInputNeuron, ANNneuron* firstOutputNeuron, const int numberOfInputNeurons, const int numberOfOutputNeurons, ANNexperience* currentExperience)
{
	resetInputsAndClassTargets(firstInputNeuron, firstOutputNeuron, numberOfInputNeurons, numberOfOutputNeurons, currentExperience);

	double trainingErrorNotUsed;
	trainingErrorNotUsed = ANNalgorithmBackpropagationUpdate.ANNbackPropogationPass(firstInputNeuron, firstOutputNeuron);
}


void ANNalgorithmBackpropagationTrainingClass::trainNeuralNetworkBackpropagationSimple(ANNneuron* firstInputNeuron, ANNneuron* firstOutputNeuron, const int numberOfInputNeurons, const int numberOfOutputNeurons, const int numEpochs, ANNexperience* firstExperienceInDataSet, const int64_t numberOfExperiences)
{
	resetNeuralNetworkWithRandomBiasAndWeights(firstInputNeuron);

	trainNeuralNetworkBackpropagationSimpleNoReset(firstInputNeuron, firstOutputNeuron, numberOfInputNeurons, numberOfOutputNeurons, numEpochs, firstExperienceInDataSet, numberOfExperiences);
}

void ANNalgorithmBackpropagationTrainingClass::trainNeuralNetworkBackpropagationSimpleNoReset(ANNneuron* firstInputNeuron, ANNneuron* firstOutputNeuron, const int numberOfInputNeurons, const int numberOfOutputNeurons, const int numEpochs, ANNexperience* firstExperienceInDataSet, const int64_t numberOfExperiences)
{
	//now train the network on all examples for numEpochs
	
	cout << "\n*****************************************************" << endl;
	cout << "Number of Input Neurons = " << numberOfInputNeurons << endl;
	cout << "Number of Output Neurons = " << numberOfOutputNeurons  <<endl;
	cout << "number of training Epochs = " << numEpochs <<endl;

	float finalError;
	for(int e=0;e < numEpochs;e++)
	{
		ANNexperience* currentExperience = firstExperienceInDataSet;

		//from start of dataSet -> end of dataSet
		for(int experienceNum = 0; experienceNum < numberOfExperiences; experienceNum++)
		{
			resetInputsAndClassTargets(firstInputNeuron, firstOutputNeuron, numberOfInputNeurons, numberOfOutputNeurons, currentExperience);

			finalError = ANNalgorithmBackpropagationUpdate.ANNbackPropogationPass(firstInputNeuron, firstOutputNeuron);

			currentExperience = currentExperience->next;
		}
	}

	storeNeuralNetworkBiasAndWeights(firstInputNeuron);	//store these for future use of the network (such that they are not lost during future back props)


	//Added by RBB; now test the network across all examples and calculate the average error
	ANNexperience* currentExperience = firstExperienceInDataSet;
	float  sumingTestingError = 0.0F;
	for(int experienceNum = 0; experienceNum < numberOfExperiences; experienceNum++)
	{
		restoreNeuralNetworkWithStoredBiasAndWeights(firstInputNeuron);
		resetInputsAndClassTargets(firstInputNeuron, firstOutputNeuron, numberOfInputNeurons, numberOfOutputNeurons, currentExperience);
		float testingError = ANNalgorithmBackpropagationUpdate.ANNbackPropogationPass(firstInputNeuron, firstOutputNeuron);
		sumingTestingError = sumingTestingError + testingError;

		currentExperience = currentExperience->next;
	}

	restoreNeuralNetworkWithStoredBiasAndWeights(firstInputNeuron);


	//cout << "Final training accuracy for e epochs = " << (1.0F-finalError)*100.0F << "%" <<endl;
	cout << "Final averaged testing accuracy for e epochs = " << (1.0F-(sumingTestingError/(double)numberOfExperiences))*100.0F << "%" <<endl;
	cout << "numberOfExperiences = " << numberOfExperiences << endl;
}





void ANNalgorithmBackpropagationTrainingClass::trainNeuralNetworkBackpropagation(ANNneuron* firstInputNeuron, ANNneuron* firstOutputNeuron, const int numberOfInputNeurons, const int numberOfOutputNeurons, const int maxFolds, ANNexperience* firstExperienceInDataSet, const int64_t numberOfExperiences, const int maxEpochs)
{
	/*
	network structure

	X input neurons
	arbitary number hidden layers/subnets with arbitary number hidden neurons with
	Y output neurons
	*/

	/*
	procedure;

	Go through network 215xaverageNumberOfEpicsx215 times..

	... using each of the 215 cases as a test case, and the other 214 as training cases.

	for each of the 215 test cases;

	train on all 214 cases. then test on the test case. calculate the final error rate.

	average all these error rates over all permutations of test cases to get the final error rate.
	*/

	int foldNum; //number of folds for cross validation

	float totalTestingError = 0;
	float totalTrainingError = 0;
	float totalTrainingEpochs = 0;

	float* trainingEpochsNumberArray = new float[maxFolds];
	float* trainingErrorArray = new float[maxFolds];
	float* testingErrorArray = new float[maxFolds];

	for(foldNum=0; foldNum < maxFolds; foldNum++)
	{
		/*
		assumes Thyroid data is randomly distributed - if its not it should be -
		and takes the last 1/foldNum (*) number of data sets
		as test sets, and the first (foldNum-1)/foldNum as training sets.
		*/

		int numEpochs;
		float testingError;
		float trainingError;

		//train using all sets of data except for test set (= testPermutation)

		resetNeuralNetworkWithRandomBiasAndWeights(firstInputNeuron);

		trainingError = 1.0F;

		numEpochs = 0;

		float acceptableError = ACCEPTABLE_ERROR_RATE;


		float currentAverageTestingError;
		currentAverageTestingError = 1.0F;	//ADDED BY RBB 17 sept 08

		float recordedTestedError = 10.0F;	//this cannot be just 1.0 as errors slighly greater than 1.0F can be experienced.
			//it is initalised to a high error value so that ... so that overfitting can be identified

		bool overFittingData = false;

		int64_t indexOfFirstExperienceInFoldTrainPartA = 0;
		int64_t indexOfLastExperienceInFoldTrainPartA = int(float(numberOfExperiences)* float(foldNum)/float(maxFolds));
		ANNexperience* firstExperienceInFoldTrainPartA = firstExperienceInDataSet;

		int64_t indexOfFirstExperienceInFoldTrainPartB = int(float(numberOfExperiences)* float(foldNum)/float(maxFolds))+int(float(numberOfExperiences)/float(maxFolds));
		int64_t indexOfLastExperienceInFoldTrainPartB = numberOfExperiences;
		ANNexperience* firstExperienceInFoldTrainPartB = ANNexperienceClass.findExperience(firstExperienceInDataSet, indexOfFirstExperienceInFoldTrainPartB);

		int64_t indexOfFirstExperienceInFoldTestPart = (int(float(numberOfExperiences)* float(foldNum)/float(maxFolds)));
		int64_t indexOfLastExperienceInFoldTestPart = (int(float(numberOfExperiences)* float(foldNum)/float(maxFolds))+int(float(numberOfExperiences)/float(maxFolds)));
		ANNexperience* firstExperienceInFoldTestPart = ANNexperienceClass.findExperience(firstExperienceInDataSet, indexOfFirstExperienceInFoldTestPart);

		//training
		while((currentAverageTestingError > acceptableError) && (numEpochs < maxEpochs) && !overFittingData)	//CHANGED FROM training to averagedTesting BY RBB 17 sept 08
		{
			//from start of dataSet -> beginning of test segment
			ANNexperience* currentExperienceInFold;
			currentExperienceInFold = firstExperienceInFoldTrainPartA;
			for(int experienceNum = indexOfFirstExperienceInFoldTrainPartA; experienceNum < indexOfLastExperienceInFoldTrainPartA; experienceNum++)
			{
				//sets inputData into ANN
				//(NB normalisedInputData[experienceNum][0] is target class

				resetInputsAndClassTargets(firstInputNeuron, firstOutputNeuron, numberOfInputNeurons, numberOfOutputNeurons, currentExperienceInFold);

				trainingError = ANNalgorithmBackpropagationUpdate.ANNbackPropogationPass(firstInputNeuron, firstOutputNeuron);


				currentExperienceInFold = currentExperienceInFold->next;
			}

			//from end of test segment -> end of dataSet
			currentExperienceInFold = firstExperienceInFoldTrainPartB;
			for(int experienceNum = indexOfFirstExperienceInFoldTrainPartB; experienceNum<indexOfLastExperienceInFoldTrainPartB; experienceNum++)
			{
				resetInputsAndClassTargets(firstInputNeuron, firstOutputNeuron, numberOfInputNeurons, numberOfOutputNeurons, currentExperienceInFold);

				trainingError = ANNalgorithmBackpropagationUpdate.ANNbackPropogationPass(firstInputNeuron, firstOutputNeuron);


				currentExperienceInFold = currentExperienceInFold->next;
			}

			numEpochs++;

			//testing

			//START OF TESTING FOR TRAINING EPOCH ep

			//creates and stores copy of the trained neural network

			storeNeuralNetworkBiasAndWeights(firstInputNeuron);

			//now determine the number of epochs ep that result in best performance on the respective test partition

			float  sumingTestingError = 0.0F;

			//from start of test segment -> end of test segment
			currentExperienceInFold = firstExperienceInFoldTestPart;
			for(int experienceNum = indexOfFirstExperienceInFoldTestPart; experienceNum < indexOfLastExperienceInFoldTestPart; experienceNum++)
			{
				restoreNeuralNetworkWithStoredBiasAndWeights(firstInputNeuron);

				resetInputsAndClassTargets(firstInputNeuron, firstOutputNeuron, numberOfInputNeurons, numberOfOutputNeurons, currentExperienceInFold);

				testingError = ANNalgorithmBackpropagationUpdate.ANNbackPropogationPass(firstInputNeuron, firstOutputNeuron);

				sumingTestingError = sumingTestingError + testingError;


				currentExperienceInFold = currentExperienceInFold->next;
			}

			//resets the trained neural network to the stored copy

			restoreNeuralNetworkWithStoredBiasAndWeights(firstInputNeuron);

			currentAverageTestingError = sumingTestingError/(float(numberOfExperiences)/float(maxFolds));

			//if(testingError > (recordedTestedError+float(ERROR_VARIATION_ALLOWED)))
			if(currentAverageTestingError > (recordedTestedError+float(ERROR_VARIATION_ALLOWED)))
			{
				//Started to overfit data...

				#ifdef ANN_USE_MIN_NUM_EPOCHS_BEFORE_DECLARE_OVERFITTING_DATA
				if(numEpochs > ANN_MIN_NUM_EPOCHS_BEFORE_DECLARE_OVERFITTING_DATA)
				{
					cout << "Overfitting was detected and training was stopped" << endl;
					cout << "currentAverageTestingError = " << currentAverageTestingError << endl;
					overFittingData	= true;
				}
				#else
					cout << "Overfitting was detected and training was stopped" << endl;
					cout << "currentAverageTestingError = " << currentAverageTestingError << endl;
					overFittingData	= true;
				#endif
			}
			//else if(testingError > recordedTestedError)
			else if(currentAverageTestingError > recordedTestedError)
			{
				//cout << "Potential imminent overfitting..." << endl;
				//do not create a recordedTestedError. Wait to see if there is further testing error increase
			}
			else
			{
				//probably not overfitting the data...

				//recordedTestedError = testingError;
				recordedTestedError = currentAverageTestingError;
			}

			//END OF TESTING FOR TRAINING EPOCH ep
		}

		trainingEpochsNumberArray[foldNum] = numEpochs;
		trainingErrorArray[foldNum] = trainingError;
		//testingErrorArray[foldNum] = testingError;
		testingErrorArray[foldNum] = currentAverageTestingError;

		cout << "Results for fold number = " << foldNum << endl;
		cout << "Error on training Set = " << trainingError << endl;
		cout << "Accuracy of training Set = " << 100.0F*(1.0F-trainingError) << "%" << endl;
		//cout << "Error on test Set = " <<  testingError << endl;
		//cout << "Accuracy of test Set = " <<  100.0F*(1.0F-testingError) << "%" << endl;
		cout << "Error on Averaged test Set = " <<  currentAverageTestingError << endl;
		cout << "Accuracy of Averaged test Set = " << 100.0F*(1.0F- currentAverageTestingError) << "%" << endl;
		cout << "Number of epochs = " << numEpochs << "\n" << endl;

		totalTrainingEpochs = totalTrainingEpochs + numEpochs;
		totalTrainingError = totalTrainingError + trainingError;
		//totalTestingError = totalTestingError + testingError;
		totalTestingError = totalTestingError + currentAverageTestingError;
	}

	//calculates and prints out average error;

	//calculate averages;
	float averageTrainingError = totalTrainingError/float(maxFolds);
	float averageTestingError = totalTestingError/float(maxFolds);
	float averageTotalTrainingEpochs = totalTrainingEpochs/float(maxFolds);

	//calculate standard deviations;
	float sdTrainingError = calculateStandardDeviation(trainingErrorArray, maxFolds, averageTrainingError);
	float sdTestingError = calculateStandardDeviation(testingErrorArray, maxFolds, averageTestingError);
	float sdTotalTrainingEpochs = calculateStandardDeviation(trainingEpochsNumberArray, maxFolds, averageTotalTrainingEpochs);

	//time feedback (used to calculate the optimum number of hidden neurons
	/*
	struct tm* myTime;
	time_t myClock = time(NULL);
	char buf[BUFSIZ];
	myTime = localtime(&myClock);
	#ifdef ANN_DEBUG
	//cout << "time = " << asctime(myTime) << endl;
	#endif
	*/

	cout << "\n*****************************************************" << endl;
	cout << "Number of Input Neurons = " << numberOfInputNeurons << endl;
	cout << "Number of Output Neurons = " << numberOfOutputNeurons  <<endl;
	cout << "Average Training Error = " << averageTrainingError <<endl;
	cout << "Average Training Accuracy = " << 100.0F*(1.0F-averageTrainingError) << "%" <<endl;
	cout << "Average Testing Error = " << averageTestingError <<endl;
	cout << "Average Testing Accuracy = " << 100.0F*(1.0F-averageTestingError) << "%" <<endl;
	cout << "Average number of training Epochs = " << averageTotalTrainingEpochs <<endl;
	cout << "Standard Deviation Training Error = " << sdTrainingError <<endl;
	cout << "Standard Deviation Testing Error = " << sdTestingError <<endl;
	cout << "Standard Deviation of number of training Epochs = " << sdTotalTrainingEpochs <<endl;


	//now train the network on all examples for the mean number of epochs across all folds
	resetNeuralNetworkWithRandomBiasAndWeights(firstInputNeuron);

	float finalError;
	for(int e=0;e < averageTotalTrainingEpochs;e++)
	{
		ANNexperience* currentExperience = firstExperienceInDataSet;

		//from start of dataSet -> end of dataSet
		for(int experienceNum = 0; experienceNum < numberOfExperiences; experienceNum++)
		{
			resetInputsAndClassTargets(firstInputNeuron, firstOutputNeuron, numberOfInputNeurons, numberOfOutputNeurons, currentExperience);

			finalError = ANNalgorithmBackpropagationUpdate.ANNbackPropogationPass(firstInputNeuron, firstOutputNeuron);


			currentExperience = currentExperience->next;
		}
	}

	storeNeuralNetworkBiasAndWeights(firstInputNeuron);	//store these for future use of the network (such that they are not lost during future back props)


#ifdef TEMP_OUTPUT_HIGH_ERROR_EXPERIENCES
	ANNexperience* firstExperienceInHighErrorExperienceList = new ANNexperience();
	ANNexperience* currentExperienceInHighErrorExperienceList = firstExperienceInHighErrorExperienceList;
#endif

	int64_t experiencesIncorrectlyDiagnosed = 0;

	//Added by RBB; now test the network across all examples and calculate the average error
	ANNexperience* currentExperience = firstExperienceInDataSet;
	float  sumingTestingError = 0.0F;
	int64_t numberOfExperiencesContributingTowardsFinalTestingError = 0;


	for(int experienceNum = 0; experienceNum < numberOfExperiences; experienceNum++)
	{
		restoreNeuralNetworkWithStoredBiasAndWeights(firstInputNeuron);

		resetInputsAndClassTargets(firstInputNeuron, firstOutputNeuron, numberOfInputNeurons, numberOfOutputNeurons, currentExperience);

		float testingError = ANNalgorithmBackpropagationUpdate.ANNbackPropogationPass(firstInputNeuron, firstOutputNeuron);


	#ifndef DEBUG_DISREGARD_HIGH_ERROR_EXPERIENCES
		sumingTestingError = sumingTestingError + testingError;

		numberOfExperiencesContributingTowardsFinalTestingError++;
	#endif

		if(testingError > 0.5)
		{
			experiencesIncorrectlyDiagnosed++;
		}

		currentExperience = currentExperience->next;
	}

	restoreNeuralNetworkWithStoredBiasAndWeights(firstInputNeuron);

	delete trainingEpochsNumberArray;
	delete trainingErrorArray;
	delete testingErrorArray;


#ifdef TEMP_OUTPUT_HIGH_ERROR_EXPERIENCES
	ofstream experienceDataSetOfStreamObject("highErrorExperiences.data");
	ANNexperienceClass.addExperiencesToOFStream(&experienceDataSetOfStreamObject, firstExperienceInHighErrorExperienceList);
	experienceDataSetOfStreamObject.close();
#endif

	//cout << "Final training accuracy for e epochs (e det. from 10 fold cross validation) = " << (1.0F-finalError)*100.0F << "%" <<endl;
	cout << "Final averaged testing accuracy for e epochs (e from 10 fold cross validation) = " << (1.0F-(sumingTestingError/(double)numberOfExperiencesContributingTowardsFinalTestingError))*100.0F << "%" <<endl;
	cout << "Final Real Life accuracy (based upon number of experiences correctly diagnosed) = " << (1.0F-((double)experiencesIncorrectlyDiagnosed/(double)numberOfExperiencesContributingTowardsFinalTestingError))*100.0F << "%" <<endl;
	cout << "numberOfExperiencesContributingTowardsFinalTestingError = " << numberOfExperiencesContributingTowardsFinalTestingError << endl;
	cout << "numberOfExperiences = " << numberOfExperiences << endl;
}

































void ANNalgorithmBackpropagationTrainingClass::storeNeuralNetworkBiasAndWeights(ANNneuron* neuronBeingAccessed)
{

	ANNneuron* currentNeuronReference = neuronBeingAccessed;

	while(currentNeuronReference->nextNeuron != NULL)
	{

		if(neuronBeingAccessed->hasBackLayer)	//input layer does not have biases [CHECK ANNTHIS; NB in ANN, an input layer to a subnet may have a bias, however regardless of this, storeNeuralNetworkBiasAndWeights will take into account this, in the higher level subnet pass]
		{
			currentNeuronReference->storedBias = currentNeuronReference->bias;

		}

		if(neuronBeingAccessed->hasFrontLayer)	//output layer does not have front neuron connection weights or subnets [CHECK ANNTHIS; NB in ANN, an output layer of a subnet may have weights, however regardless of this, storeNeuralNetworkBiasAndWeights will take into account this, in the higher level subnet pass]
		{
			for(vector<ANNneuronConnection*>::iterator connectionIter = currentNeuronReference->frontANNneuronConnectionList.begin(); connectionIter != currentNeuronReference->frontANNneuronConnectionList.end(); connectionIter++)
			{
				ANNneuronConnection* currentANNneuronConnection = *connectionIter;

				currentANNneuronConnection->storedWeight = currentANNneuronConnection->weight;

			}

			#ifdef ANN_SUBNETS
			if(currentNeuronReference->isSubnet)
			{
				storeNeuralNetworkBiasAndWeights(currentNeuronReference->firstNeuronInBackLayerOfSubnet);
			}
			#endif
		}

		currentNeuronReference = currentNeuronReference->nextNeuron;
	}

	//recursion [NB there will be unnecessay redundancy here, as the same neuron will be accessed more than once]
	if(neuronBeingAccessed->hasFrontLayer)
	{
		storeNeuralNetworkBiasAndWeights(neuronBeingAccessed->firstNeuronInFrontLayer);
	}
}


void ANNalgorithmBackpropagationTrainingClass::restoreNeuralNetworkWithStoredBiasAndWeights(ANNneuron* neuronBeingAccessed)
{

	ANNneuron* currentNeuronReference = neuronBeingAccessed;

	while(currentNeuronReference->nextNeuron != NULL)
	{

		if(neuronBeingAccessed->hasBackLayer)	//input layer does not have biases[CHECK ANNTHIS; NB in ANN, an input layer to a subnet may have a bias, however regardless of this, restoreNeuralNetworkWithStoredBiasAndWeights will take into account this, in the higher level subnet pass]
		{
			currentNeuronReference->bias = currentNeuronReference->storedBias;

		}


		if(neuronBeingAccessed->hasFrontLayer)	//output layer does not have front neuron connection weights or subnets [CHECK ANNTHIS; NB in ANN, an output layer of a subnet may have weights, however regardless of this, restoreNeuralNetworkWithStoredBiasAndWeights will take into account this, in the higher level subnet pass]
		{

			for(vector<ANNneuronConnection*>::iterator connectionIter = currentNeuronReference->frontANNneuronConnectionList.begin(); connectionIter != currentNeuronReference->frontANNneuronConnectionList.end(); connectionIter++)
			{
				ANNneuronConnection* currentANNneuronConnection = *connectionIter;

				currentANNneuronConnection->weight = currentANNneuronConnection->storedWeight;

			}

			#ifdef ANN_SUBNETS
			if(currentNeuronReference->isSubnet)
			{
				restoreNeuralNetworkWithStoredBiasAndWeights(currentNeuronReference->firstNeuronInBackLayerOfSubnet);
			}
			#endif
		}

		currentNeuronReference = currentNeuronReference->nextNeuron;
	}


	//recursion [NB there will be unnecessay redundancy here, as the same neuron will be accessed more than once]
	if(neuronBeingAccessed->hasFrontLayer)
	{
		restoreNeuralNetworkWithStoredBiasAndWeights(neuronBeingAccessed->firstNeuronInFrontLayer);
	}
}

void ANNalgorithmBackpropagationTrainingClass::resetNeuralNetworkWithRandomBiasAndWeights(ANNneuron* neuronBeingAccessed)
{

	ANNneuron* currentNeuronReference = neuronBeingAccessed;

	while(currentNeuronReference->nextNeuron != NULL)
	{

		if(neuronBeingAccessed->hasBackLayer)	//input layer does not have biases [CHECK ANNTHIS; NB in ANN, an input layer to a subnet may have a bias, however regardless of this, resetNeuralNetworkWithRandomBiasAndWeights will take into account this, in the higher level subnet pass]
		{
			#ifndef DEBUG_TH_OR_IMAGE_CATEGORISTION_NN_DO_NOT_USE_NEGATIVE_BIASES_AND_WEIGHTS
			#ifdef TH_OR_IMAGE_CATEGORISTION_NN_USE_HEAVY_RANDOMISATION_OF_BIASES_AND_WEIGHTS
			currentNeuronReference->bias = ((double(rand()* 2.0F)/(double(RAND_MAX)))-1.0F)*TH_OR_IMAGE_CATEGORISTION_NN_USE_HEAVY_RANDOMISATION_OF_BIASES_AND_WEIGHTS_BIAS_MULT;
			#else
			#ifndef DEBUG_TH_OR_IMAGE_CATEGORISTION_NN_DO_NOT_RANDOMISE_BIASES
			#ifndef DEBUG_TRAIN_NETWORK_WITH_NON_RANDOM_VARS
			#ifndef DEBUG_TH_ANN_USE_ORIGINAL_RANDOMISATION
			currentNeuronReference->bias = (double(rand()* 2.0F)/(double(RAND_MAX)))-1.0F;
			#endif
			#endif
			#endif
			#endif
			#endif

		}

		if(neuronBeingAccessed->hasFrontLayer)	//output layer does not have front neuron connection weights or subnets [CHECK ANNTHIS; NB in ANN, an output layer of a subnet may have weights, however regardless of this, resetNeuralNetworkWithRandomBiasAndWeights will take into account this, in the higher level subnet pass]
		{
			for(vector<ANNneuronConnection*>::iterator connectionIter = currentNeuronReference->frontANNneuronConnectionList.begin(); connectionIter != currentNeuronReference->frontANNneuronConnectionList.end(); connectionIter++)
			{
				ANNneuronConnection* currentANNneuronConnection = *connectionIter;

				#ifndef DEBUG_TH_OR_IMAGE_CATEGORISTION_NN_DO_NOT_USE_NEGATIVE_BIASES_AND_WEIGHTS
				#ifdef TH_OR_IMAGE_CATEGORISTION_NN_USE_HEAVY_RANDOMISATION_OF_BIASES_AND_WEIGHTS
				currentANNneuronConnection->weight = ((double(rand()* 2.0F)/(double(RAND_MAX)))-1.0F)*TH_OR_IMAGE_CATEGORISTION_NN_USE_HEAVY_RANDOMISATION_OF_BIASES_AND_WEIGHTS_WEIGHT_MULT;
				#else
				#ifndef DEBUG_TH_OR_IMAGE_CATEGORISTION_NN_DO_NOT_RANDOMISE_WEIGHTS
				#ifndef DEBUG_TRAIN_NETWORK_WITH_NON_RANDOM_VARS
				#ifndef DEBUG_TH_ANN_USE_ORIGINAL_RANDOMISATION
				currentANNneuronConnection->weight = (double(rand()* 2.0F)/(double(RAND_MAX)))-1.0F;
				#endif
				#endif
				#endif
				#endif
				#endif

			}

			#ifdef ANN_SUBNETS
			if(currentNeuronReference->isSubnet)
			{
				resetNeuralNetworkWithRandomBiasAndWeights(currentNeuronReference->firstNeuronInBackLayerOfSubnet);
			}
			#endif
		}

		currentNeuronReference = currentNeuronReference->nextNeuron;
	}


	//recursion [NB there will be unnecessay redundancy here, as the same neuron will be accessed more than once]
	if(neuronBeingAccessed->hasFrontLayer)
	{
		resetNeuralNetworkWithRandomBiasAndWeights(neuronBeingAccessed->firstNeuronInFrontLayer);
	}
}

void ANNalgorithmBackpropagationTrainingClass::resetInputsAndClassTargets(ANNneuron* firstInputNeuron, ANNneuron* firstOutputNeuron, const int64_t numberOfInputNeurons, const int64_t numberOfOutputNeurons, ANNexperience* currentExperienceInDataSet)
{

	//sets inputData into ANN
	//(NB normalisedInputData[i][0] is target class

	ANNneuron* currentNeuronReference = firstInputNeuron;
	ANNexperienceInput* currentExperienceInputInExperience = currentExperienceInDataSet->firstExperienceInput;
	for(int64_t i = 0; i < numberOfInputNeurons; i++)
	{
		currentNeuronReference->output = currentExperienceInputInExperience->inputValue;	 //normalisedInputData[testSegment][(i+1)];
		currentNeuronReference = currentNeuronReference->nextNeuron;
		currentExperienceInputInExperience = currentExperienceInputInExperience->next;
	}

	currentNeuronReference = firstOutputNeuron;
	for(int64_t i = 0; i < numberOfOutputNeurons; i++)
	{
		currentNeuronReference->classTarget = 0.0F;
		if(i == currentExperienceInDataSet->classTargetValue)
		{
			currentNeuronReference->classTarget = 1.0F;
		}
		currentNeuronReference = currentNeuronReference->nextNeuron;
	}

	#ifdef ANN_SUBNETS
	/*CHECK ANNTHIS: anything required for subnet inputs/outputs?*/
	#endif

}




float ANNalgorithmBackpropagationTrainingClass::calculateStandardDeviation(const float* array, const int length, const float average)
{
	float sum = 0;

	for(int i=0;i<length;i++)
	{
		sum = sum + pow((array[i]-average),2);
	}

	float sd = sqrt(sum/float(length));

	return sd;
}

#endif

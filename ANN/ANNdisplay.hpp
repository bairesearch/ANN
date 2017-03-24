/*******************************************************************************
 *
 * This file is part of BAIPROJECT.
 *
 * BAIPROJECT is licensed under the GNU Affero General Public License
 * version 3, as published by the Free Software Foundation. The use of
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
 * File Name: ANNdisplay.hpp
 * Author: Richard Bruce Baxter - Copyright (c) 2005-2017 Baxter AI (baxterai.com)
 * Project: Artificial Neural Network (ANN)
 * Project Version: 3k2a 21-March-2017
 * Comments: TH = Test Harness
 *
 *******************************************************************************/

#ifndef HEADER_ANN_DISPLAY
#define HEADER_ANN_DISPLAY

#include "ANNexperienceClass.hpp"
#include "ANNneuronClass.hpp"
#include "ANNglobalDefs.hpp"
#include "ANNformation.hpp"
#include "ANNalgorithmBackpropagationTraining.hpp"
#include "ANNalgorithmMemoryNetworkTraining.hpp"
#include "ANNalgorithmClassificationNetworkTraining.hpp"
#include "ANNxmlConversion.hpp"
#include "ANNdraw.hpp"
#include "ANNalgorithmBackpropagationUpdate.hpp"
#include "LDparser.hpp"
#include "LDreferenceManipulation.hpp"
#include "LDsvg.hpp"
#include "LDopengl.hpp"
#include "RTscene.hpp"
#include "RTreferenceManipulation.hpp"
#include "RTpixelMaps.hpp"
#include "RTviewinfo.hpp"


class ANNdisplayClass
{
	private: ANNexperienceClassClass ANNexperienceClass;
	private: RTpixelMapsClass RTpixelMaps;
	#ifdef ANN_ALGORITHM_BACKPROPAGATION
	private: ANNalgorithmBackpropagationTrainingClass ANNalgorithmBackpropagationTraining;
	#endif
	#ifdef ANN_ALGORITHM_MEMORY_NETWORK
	private: ANNalgorithmMemoryNetworkTrainingClass ANNalgorithmMemoryNetworkTraining;
	#endif
	#ifdef ANN_ALGORITHM_CLASSIFICATION_NETWORK
	private: ANNalgorithmClassificationNetworkTrainingClass ANNalgorithmClassificationNetworkTraining;
	#endif
	private: ANNxmlConversionClass ANNxmlConversion;
	private: ANNdrawClass ANNdraw;
	private: LDreferenceManipulationClass LDreferenceManipulation;
	private: LDsvgClass LDsvg;
	private: LDparserClass LDparser;
	private: LDopenglClass LDopengl;
	private: RTsceneClass RTscene;
	private: RTreferenceManipulationClass RTreferenceManipulation;
	#ifdef USE_RT
	private: void generateExperienceWith2DrgbMap(unsigned char* rgbMap, int imageWidth, int imageHeight, ANNexperience* currentExperience, long objectDecision);
	private: void generateExperienceWith2Dmap(double* lumOrContrastOrDepthMap, int imageWidth, int imageHeight, double mapMaxValue, ANNexperience* currentExperience, long objectDecision);
	private: void generateExperienceWith2DbooleanMap(bool* booleanMap, int imageWidth, int imageHeight, ANNexperience* currentExperience, long objectDecision);
	#endif


	private: bool trainAndOutputNeuralNetworkWithFileNames(ANNneuron* firstInputNeuronInNetwork, ANNneuron* firstOutputNeuronInNetwork, long numberOfInputNeurons, long numberOfOutputNeurons, ANNexperience* firstExperienceInList, bool addSprites, bool allowRaytrace, string* XMLNNSceneFileName, char* charstarvectorGraphicsLDRNNSceneFileName, char* charstarvectorGraphicsTALNNSceneFileName, char* charstarraytracedImagePPMNNSceneFileName, char* charstarexperienceNNSceneFileName, bool useFoldsDuringTraining, int maxOrSetNumEpochs);	//OLD wrapper function: this does not support full set of ANN features (SVG, opengl output etc)
		public: void outputNeuralNetworkToVectorGraphicsAndRaytrace(ANNneuron* firstInputNeuronInNetwork, bool addSprites, bool allowRaytrace, bool displayInOpenGL, bool useOutputLDRFile, bool useOutputPPMFile, bool useOutputSVGFile, string outputLDRFileName, string outputSVGFileName, string outputPPMFileName, string outputPPMFileNameRaytraced, string outputTALFileName, int width, int height);

	private: void writeExperienceListToFile(char* fileName, ANNexperience* firstExperienceInList);
};

#endif

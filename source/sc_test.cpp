#include "sc_test_dict.h"
#include "FEX\LinearMapper.h"

void print_help(void)
{
	printf("Usage: \"sc_test.exe\" \n");	// 0
	printf("\t input_dict.dic \n");			// 1
	printf("\t input_file.txt \n");			// 2
	printf("\t output_file.txt \n");		// 3
}

int main(int argc, char *argv[])
{
	if (argc != 1) {
		print_help();
		return 0;
	}

	// Parsing Parameters
	std::string inDicFile	= "D:\\Projects\\sparse-coding\\dictionaries\\dictionary_6x16.dic";
	std::string inDatFile	= "D:\\Data\\Raw_6_sensors_examples\\S1-ADL1-1\\ex2_6-69.txt";
	size_t		nSequences  = 6;
	size_t		windowSize  = 16;
	word		windowStep  = 1;
	std::string outFeatures = "";

	// Additional constants
	const float	rangeMin = -16384;
	const float	rangeMax = 16383;
	const int	sampleLen = nSequences * windowSize;
	printf("Sample length = %d\n", sampleLen);
	
	// Loading the dictionary
	CCSTestDict sparseDictionary;
	sparseDictionary.load(inDicFile);
	
	// Reading the content of the file to vData
	FILE * pFile = fopen(inDatFile.c_str(), "r");
	std::vector<float> vData;
	double el;
	while (fscanf(pFile, "%le", &el) != EOF) 
		vData.push_back(static_cast<float>(el));
	fclose(pFile);

	DGM_ASSERT(vData.size() % nSequences == 0);
	size_t sequenceLen = vData.size() / nSequences;

	for (size_t i = 0; i < sequenceLen - windowSize; i += windowStep) {		// i = start of the window
		Mat	sample(1, sampleLen, CV_16UC1);
		for (size_t s = 0; s < nSequences; s++) 							// sequences
			for (size_t j = 0; j < windowSize; j++) {
				float el = vData[i + s * sequenceLen + j];
				sample.at<word>(0, s * windowSize + j) = dgm::fex::linear_mapper<word>(el, rangeMin, rangeMax);
			}
		sparseDictionary.get(sample);
	} // i
	
	printf("data len = %ld\n", vData.size());





	return 0;
}
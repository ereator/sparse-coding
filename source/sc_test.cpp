#include "sc_test_dict.h"
#include "FEX\LinearMapper.h"

void print_help(void)
{
	printf("Usage: \"sc_test.exe\" \n");	// 0
	printf("\t <input dictionary> \n");		// 1
	printf("\t <input file> \n");			// 2
	printf("\t <number of sequences> \n");	// 3
	printf("\t <window width> \n");			// 4
	printf("\t <window step> \n");			// 5
	printf("\t <output file> \n");			// 6
}

int main(int argc, char *argv[])
{
	if (argc != 7) {
		print_help();
		return 0;
	}

	// Parsing Parameters
	std::string inDicFile	= argv[1];			// "D:\\Projects\\sparse-coding\\dictionaries\\dictionary_6x16.dic"
	std::string inDatFile	= argv[2];			// "D:\\Data\\Raw_6_sensors_examples\\S1-ADL1-1\\ex2_6-69.txt"
	int			nSequences	= atoi(argv[3]);	// 6
	int			windowSize	= atoi(argv[4]);	// 16
	word		windowStep	= atoi(argv[5]);	// 1
	std::string outFeatures	= argv[6];			// "D:\\ex2_6-69.dat"

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
	printf("data len = %lld\n", vData.size());
	
	// Calculate and save the feature
	pFile = fopen(outFeatures.c_str(), "w");
	for (size_t i = 0; i < sequenceLen - windowSize; i += windowStep) {		// i = start of the window
		Mat	sample(1, sampleLen, CV_16UC1);
		for (int s = 0; s < nSequences; s++) 							// sequences
			for (int j = 0; j < windowSize; j++) {
				float el = vData[i + s * sequenceLen + j];
				sample.at<word>(0, s * windowSize + j) = dgm::fex::linear_mapper<word>(el, rangeMin, rangeMax);
			}
		Mat feature = sparseDictionary.getFeature(sample);
		for (int x = 0; x < feature.cols; x++)
			fprintf(pFile, "%le ", static_cast<double>(feature.at<float>(0, x)));
		fprintf(pFile, "\n");
	} // i
	fclose(pFile);

	return 0;
}
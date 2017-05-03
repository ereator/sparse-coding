#include "Windows.h"
#include "DGM.h"
#include "FEX.h"
#include "FEX\LinearMapper.h"
#include "DGM\parallel.h"

namespace dgm = DirectGraphicalModels;

void print_help(void)
{
	printf("Usage: \"sc_train.exe\" \n");		// 0
	printf("\t <input folder> \n");				// 1
	printf("\t <number of sequences> \n");		// 2
	printf("\t <window width> \n");				// 3
	printf("\t <window step> \n");				// 4
	printf("\t <num words> \n");				// 5
	printf("\t <batch> \n");					// 6
	printf("\t <number of iteration> \n");		// 7
	printf("\t <learning rate> \n");			// 8
	printf("\t <output dictionary> \n");		// 9
}

vec_string_t findFilesInDirectory(std::string &directory)
{
	HANDLE			dir;
	WIN32_FIND_DATA file_data;
	vec_string_t	res;

	if ((dir = FindFirstFile((directory + "\\*.txt").c_str(), &file_data)) == INVALID_HANDLE_VALUE)
		return res; // No files found 

	do {
		const std::string file_name = file_data.cFileName;
		const std::string full_file_name = directory + "\\" + file_name;
		const bool is_directory = (file_data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0;

		if (file_name[0] == '.') continue;
		if (is_directory) continue;

		res.push_back(full_file_name);
	} while (FindNextFile(dir, &file_data));

	FindClose(dir);
	return res;
}

int main(int argc, char *argv[])
{
	if (argc != 10) {
		print_help();
		return 0;
	}

	// Parsing Parameters
	std::string		srcFolder	= argv[1];
	size_t			nSequences	= atoi(argv[2]);			// 6
	size_t			windowSize  = atoi(argv[3]);			// 16
	word			windowStep	= atoi(argv[4]);			// 1
	word			nWords		= atoi(argv[5]);			// 512
	dword			batch		= atoi(argv[6]); 			// 2000
	int				nIt			= atoi(argv[7]);			// 1000
	float			lRate		= (float) atof(argv[8]);	// 1e-2f
	std::string		outDicFile	= argv[9];
	
	// Additional constants
	const bool		rangeShow	= true;
	const float		rangeMin	= -16384;
	const float		rangeMax	=  16383;
	const int		sampleLen	=  nSequences * windowSize;
	printf("Sample length = %d\n", sampleLen);

	// Reading the Data
	vec_string_t vFiles = findFilesInDirectory(srcFolder);
	
	Mat			X;
	Mat			sample(1, sampleLen, CV_16UC1);
	vec_float_t	min_el(nSequences, 0.0f);	
	vec_float_t max_el(nSequences, 0.0f); 
	for (auto file : vFiles) {
		printf("Processing file %s\n", file.c_str());

		// Reading the content of the file to vData
		FILE * pFile = fopen(file.c_str(), "r");
		std::vector<float> vData;
		while (!feof(pFile)) {
			double	el;
			fscanf(pFile, "%le", &el);
			vData.push_back(static_cast<float>(el));
		
		}
		fclose(pFile);

		DGM_ASSERT(vData.size() % nSequences == 0);
		size_t sequenceLen = vData.size() / nSequences;
		
		if (rangeShow) {	// Check min and max values
			for (size_t s = 0; s < nSequences; s++) {		// sequences
				size_t offset = s * sequenceLen;
				for (size_t i = 0; i < sequenceLen; i++) {	// elements
					float el = vData[offset + i];
					if (el > max_el[s]) max_el[s] = el;
					if (el < min_el[s]) min_el[s] = el;
				} // i
			} // s
		} 
		
		for (size_t i = 0; i < sequenceLen - windowSize; i += windowStep) {		// i = start of the window
			for (size_t s = 0; s < nSequences; s++) 							// sequences
				for (size_t j = 0; j < windowSize; j++) {
					float el = vData[i + s * sequenceLen + j];
					sample.at<word>(0, s * windowSize + j) = dgm::fex::linear_mapper<word>(el, rangeMin, rangeMax);
				}
			X.push_back(sample);
		} // i
	}
	if (rangeShow) {
		printf("\nThe value ranges for %ld sequences:\n", nSequences);
		for (size_t s = 0; s < nSequences; s++)
			printf("seq[%d] \\in [%.2f; %.2f]\n", s, min_el[s], max_el[s]);
	}
	
	printf("%d samples of length %d are collected\n", X.rows, X.cols);

	// Dictionary learning
	dgm::fex::CSparseDictionary sparseDictionary;
	dgm::parallel::shuffleRows(X);
	sparseDictionary.train(X, nWords, batch, nIt, lRate);
	sparseDictionary.save(outDicFile);

	return 0;
}
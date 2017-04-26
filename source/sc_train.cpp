#include "Windows.h"
#include "DGM.h"
#include "FEX.h"
#include "FEX\LinearMapper.h"
#include "DGM\parallel.h"
#include "VIS.h"

namespace dgm = DirectGraphicalModels;

void print_help(void)
{
	printf("Usage: \"sc_train.exe\" input_folder width step nWords sc_params output_dict\n");
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
	if (argc != 2) {
		print_help();
		return 0;
	}

	// Parsing Parameters
	std::string		srcFolder	= argv[1];
	const size_t	nSequences	= 6;
	size_t 			windowSize	= 16;
	word			step		= 1;
	word			nWords		= 512;
	dword			batch		= 2000;
	unsigned int	nIt			= 100;
	float			lRate		= 1e-2f;
	std::string		outDicFile	= "dictionary.dic";
	
	const int		sampleLen	= windowSize * nSequences;
	printf("sample len = %d\n", sampleLen);


	// Reading the Data
	vec_string_t vFiles = findFilesInDirectory(srcFolder);
	
	Mat		X;
	Mat		sample(1, sampleLen, CV_16UC1);
	bool	estimate_ranges = true;
	float	min_el[nSequences];	memset(min_el, 0, nSequences * sizeof(float));
	float	max_el[nSequences]; memset(max_el, 0, nSequences * sizeof(float));
	for (auto file : vFiles) {
		printf("Processing file %s\n", file.c_str());

		// Reading the content of the file to vData
		FILE * pFile = fopen(file.c_str(), "r");
		std::vector<float> vData;
		while (!feof(pFile)) {
			double	el;
			for (size_t s = 0; s < nSequences; s++) {			// sequences
				fscanf(pFile, "%le", &el);
				vData.push_back(static_cast<float>(el));
			} // s
		}
		fclose(pFile);

		DGM_ASSERT(vData.size() % nSequences == 0);
		size_t sequenceLen = vData.size() / nSequences;
		
		if (estimate_ranges) {	// Check min and max values
			for (size_t s = 0; s < nSequences; s++) {		// sequences
				size_t offset = s * sequenceLen;
				for (size_t i = 0; i < sequenceLen; i++) {	// elements
					float el = vData[offset + i];
					if (el > max_el[s]) max_el[s] = el;
					if (el < min_el[s]) min_el[s] = el;
				} // i
			} // s
		} 
		
		for (size_t i = 0; i < sequenceLen - windowSize; i++) {		// i = start of the window
			for (size_t s = 0; s < nSequences; s++) 				// sequences
				for (size_t j = 0; j < windowSize; j++) {
					float el = vData[i + s * sequenceLen + j];
					sample.at<word>(0, s * windowSize + j) = dgm::fex::linear_mapper<word>(el, -16384, 16383);
				}
			X.push_back(sample);
		} // i
	}
	if (estimate_ranges) {
		printf("\nThe value ranges for %ld sequences (if estimated):\n", nSequences);
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
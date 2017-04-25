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
	word			nSequences	= 6;
	word 			window_size = 16;
	word			step		= 1;
	word			nWords		= 512;
	dword			batch		= 2000;
	unsigned int	nIt			= 100;
	float			lRate		= 1e-2f;
	std::string		outDicFile	= "dictionary.dic";
	
	// 6-dimensional sequence in every file



	// Reading the Data
	Mat X;
	vec_string_t vFiles = findFilesInDirectory(srcFolder);
	
	double el;
	double min_el = 0;
	double max_el = 0;
	for (auto file : vFiles) {
		printf("%s\n", file.c_str());
		FILE * pFile = fopen(file.c_str(), "r");

		while (!feof(pFile)) {
			for (word s = 0; s < nSequences; s++) {		// sequences
				fscanf(pFile, "%le", &el);
				if (el > max_el) max_el = el;
				if (el < min_el) min_el = el;
				//printf("%f ", el);
			}
			//printf("\n");
			//getchar();
		}

		// Scan all files and read the data into container X

		fclose(pFile);
	}
	printf("[%f; %f]\n", min_el, max_el);
	
	getchar();
	return 0;

	// Dictionary learning
	dgm::fex::CSparseDictionary sparseDictionary;
	dgm::parallel::shuffleRows(X);
	sparseDictionary.train(X, nWords, batch, nIt, lRate);
	sparseDictionary.save(outDicFile);

	return 0;
}
#include "DGM.h"
#include "FEX.h"
#include "FEX\LinearMapper.h"
#include "VIS.h"

namespace dgm = DirectGraphicalModels;

void print_help(void)
{
	printf("Usage: \"sc_test.exe\" \n");	// 0
	printf("\t input_dict.dic \n");			// 1
	printf("\t input_file.txt \n");			// 2
	printf("\t output_file.txt \n");		// 3
}

int main(int argc, char *argv[])
{
	if (argc != 4) {
		print_help();
		return 0;
	}

	// Parsing Parameters
	std::string inDicFile	= "dictionary.dic";
	std::string inDatFile	= "";
	std::string outFeatures = "";

	dgm::fex::CSparseCoding sparseCoding(Mat());
	sparseCoding.load(inDicFile);


	return 0;
}
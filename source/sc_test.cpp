#include "DGM.h"
#include "FEX.h"
#include "FEX\LinearMapper.h"
#include "VIS.h"

namespace dgm = DirectGraphicalModels;

void print_help(void)
{
	printf("Usage: \"sc_test.exe\" input_dict.dic input_file.txt output_file.txt\n");
}

int main(int argc, char *argv[])
{
	if (argc != 4) {
		print_help();
		return 0;
	}

	// Parsing Parameters
	std::string inDicFile = "dictionary.dic";
	
	dgm::fex::CSparseCoding sparseCoding(Mat());
//	sparseCoding.


	return 0;
}
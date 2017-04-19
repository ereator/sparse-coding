#include "DGM.h"
#include "FEX.h"
#include "FEX\LinearMapper.h"
#include "VIS.h"

namespace dgm = DirectGraphicalModels;

void print_help(void)
{
	printf("Usage: \"sc_train.exe\" folder_with_train_data width step nWords sc_params output_dict\n");
}

int main(int argc, char *argv[])
{
	if (argc != 7) {
		print_help();
		return 0;
	}

	std::string srcFolder = argv[1];
	// find all txt file inside the folder

	int res = dgm::fex::linear_mapper<int>(25, 0, 100);
	printf("res = %d\n", res);
	getchar();

	dgm::CGraphExt *pgraph = new dgm::CGraphExt(5);
	pgraph->build(cvSize(10, 10));
	dgm::vis::showGraph3D(500, pgraph, [](size_t n) {return Vec3f( static_cast<float>(n / 10), static_cast<float>(n % 10), 0); });
	dgm::fex::CSparseDictionary dict;
	return 0;
}
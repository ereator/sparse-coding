#include "Windows.h"
#include "DGM.h"
#include "FEX.h"
#include "FEX\LinearMapper.h"
#include "VIS.h"

namespace dgm = DirectGraphicalModels;

void print_help(void)
{
	printf("Usage: \"sc_train.exe\" folder_with_train_data width step nWords sc_params output_dict\n");
}

vec_string_t findFilesInDirectory(std::string &directory)
{
	HANDLE			dir;
	WIN32_FIND_DATA file_data;
	vec_string_t	res;

	if ((dir = FindFirstFile((directory + "/*").c_str(), &file_data)) == INVALID_HANDLE_VALUE)
		return res; // No files found 

	do {
		const std::string file_name = file_data.cFileName;
		const std::string full_file_name = directory + "/" + file_name;
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

	std::string srcFolder = argv[1];
	FILE *pFile;
	vec_string_t vFiles = findFilesInDirectory(srcFolder);
	for (auto file : vFiles) {
		printf("%s\n", file.c_str());
		pFile = fopen(file.c_str(), "r");



		fclose(pFile);
	}
	
	
	getchar();
	return 0;




	int res = dgm::fex::linear_mapper<int>(25, 0, 100);
	printf("res = %d\n", res);
	getchar();

	dgm::CGraphExt *pgraph = new dgm::CGraphExt(5);
	pgraph->build(cvSize(10, 10));
	dgm::vis::showGraph3D(500, pgraph, [](size_t n) {return Vec3f( static_cast<float>(n / 10), static_cast<float>(n % 10), 0); });
	dgm::fex::CSparseDictionary dict;
	return 0;
}
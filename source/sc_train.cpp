#include "DGM.h"
#include "FEX.h"
#include "VIS.h"

namespace dgm = DirectGraphicalModels;

int main()
{
	dgm::CGraphExt *pgraph = new dgm::CGraphExt(5);
	pgraph->build(cvSize(10, 10));
	dgm::vis::showGraph3D(500, pgraph, [](size_t n) {return Vec3f( static_cast<float>(n / 10), static_cast<float>(n % 10), 0); });
	dgm::fex::CSparseDictionary dict;
	return 0;
}
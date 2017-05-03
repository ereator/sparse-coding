// Sparse Coding Dictionary class
// Written by Sergey Kosov for Uni Siegen in 2017
#pragma once

#include "DGM.h"
#include "FEX.h"

namespace dgm = DirectGraphicalModels;

class CCSTestDict : public dgm::fex::CSparseDictionary
{
public:
	CCSTestDict(void) : dgm::fex::CSparseDictionary() {}
	~CCSTestDict(void) {}

	void get(Mat &sample);

};
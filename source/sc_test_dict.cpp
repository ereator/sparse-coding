#include "sc_test_dict.h"
#include "FEX\LinearMapper.h"

void CCSTestDict::get(Mat &sample)
{
	Mat D = getDictionary();

	DGM_ASSERT_MSG(!D.empty(), "The dictionary must me trained or loaded before using this function");
	
	Mat _W, W;
	const word nWords = D.rows;

	sample.convertTo(sample, CV_32FC1, 1.0 / 65535);

	gemm(D, sample.t(), 1.0, Mat(), 0.0, _W);							// W = D x sample^T
	W = _W.t();
	for (int w = 0; w < W.cols; w++)
	W.col(w) /= norm(D.row(w), NORM_L2);

	calculate_W(sample, D, W, dgm::fex::SC_LAMBDA, dgm::fex::SC_EPSILON, 200, dgm::fex::SC_LRATE_W);

//	for (word w = 0; w < nWords; w++)
//		res[w] = dgm::fex::linear_mapper<byte>(W.at<float>(0, w), -1.0f, 1.0f);

}
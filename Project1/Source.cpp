#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>
#include <iostream>
#include <opencv2/flann/flann.hpp>
//cvTermCriteria�̒�`
#include <opencv2/core/types_c.h>

#define OPENCV_VERSION(a,b,c) (((a) << 16) + ((b) << 8) + (c))
#define OPENCV_VERSION_CODE OPENCV_VERSION(CV_MAJOR_VERSION, CV_MINOR_VERSION, CV_SUBMINOR_VERSION)


using namespace cv;

int main(int argc, char* argv[])
{
	// ���͉摜�̎擾
	Mat im = imread("image/lenna.png");
	Mat im2(im.size(), im.type());
	if (im.empty()) return -1;

	// �摜�z���1�����ɕϊ�
	Mat points;
	im.convertTo(points, CV_32FC3);
	points = points.reshape(3, im.rows * im.cols);

	// RGB��Ԃ�k-means�N���X�^�����O�����s
	Mat_<int> clusters(points.size(), CV_32SC1);
	Mat centers;

	// �N���X�^��
	const int cluster = 5;
	// k-meansn�N���X�^�����O�̎��s
	kmeans(points, cluster, clusters, cvTermCriteria(CV_TERMCRIT_EPS | CV_TERMCRIT_ITER, 10, 1.0), 1, KMEANS_PP_CENTERS, centers);

	// ��f�l���N���X�^���S�l�ɒu��
	MatIterator_<Vec3b> itd = im2.begin<Vec3b>(), itd_end = im2.end<Vec3b>();
	for (int i = 0; itd != itd_end; ++itd, ++i) {
		Vec3f& color = centers.at<Vec3f>(clusters(i), 0);
		(*itd)[0] = saturate_cast<uchar>(color[0]);
		(*itd)[1] = saturate_cast<uchar>(color[1]);
		(*itd)[2] = saturate_cast<uchar>(color[2]);
	}
	// ���ʕ\��
	imshow("Input", im);
	imshow("Output", im2);
	waitKey(0);
}
/*---------------------------------------------------------
�v���W�F�N�g:	�X���b�h�ł�OpenCV�摜�t�@�C���\�� 
���W���[���F	OpenCV�摜�t�@�C���\�����W���[��			
�쐬��:			Yamada Tarou
�쐬����:		2020/8/19
�@�\�F			OpenCV�ł̉摜�t�@�C���\��
�o�[�W����:		0.1
---------------------------------------------------------*/
#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgcodecs.hpp>

cv::Mat mat;

extern "C" int dsp_win()
{
	mat = cv::imread("penguin.png", cv::IMREAD_COLOR);
	cv::namedWindow("sample", cv::WINDOW_AUTOSIZE);
	cv::imshow("sample", mat);

	/*1000ms�����L�[���͑҂��A����Window�\�����b�Z�[�W�����̂���*/
	cv::waitKey(200);

	return 0;
}

extern "C" int cls_win()
{
	cv::destroyAllWindows();	//�S�Ă�Window�����

	return 0;
}

#include "opencv2/opencv.hpp"
#include <opencv2/core/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include <conio.h>
#include <threaD>
#include "opencv2/imgproc/imgproc.hpp"
#include <stdlib.h>
#include <stdio.h>

using namespace cv;
using namespace std;



void Detekcia(string cesta)
{
	Mat src = imread(cesta);
	int sirka = src.size().width;
	int vyska = src.size().height;
	int max = (sirka*vyska) / 10;
	int min = (sirka*vyska) / 1000;

	IplImage* img = cvLoadImage(cesta.c_str());
	IplImage* imgOrig = cvLoadImage(cesta.c_str());

	///////////////////Filter////////////////

	//konverzia obrazku do grayscale
	IplImage* imgGrayScale = cvCreateImage(cvGetSize(img), 8, 1);
	cvCvtColor(img, imgGrayScale, CV_BGR2GRAY);
	cvNamedWindow("grayscale", WINDOW_NORMAL);
	cvShowImage("grayscale", imgGrayScale);
	cv::moveWindow("grayscale", 0, 0);
	cvResizeWindow("grayscale", 320, 240);

	cvThreshold(imgGrayScale, imgGrayScale, 120, 255, CV_THRESH_BINARY);
	cvSmooth(imgGrayScale, imgGrayScale, CV_GAUSSIAN, 9, 9);

	cvNamedWindow("threshold", WINDOW_NORMAL);
	cvShowImage("threshold", imgGrayScale);
	cv::moveWindow("threshold", 320, 0);
	cvResizeWindow("threshold", 320, 240);

	CvSeq* contours;  //hold the pointer to a contour in the memory block
	CvSeq* result;   //hold sequence of points of a contour
					 //storage area for all contours
	CvMemStorage *storage = cvCreateMemStorage(0);

	//hladanie vsetkych kontur
	cvFindContours(imgGrayScale, storage, &contours, sizeof(CvContour), CV_RETR_LIST, CV_CHAIN_APPROX_SIMPLE, cvPoint(0, 0));

	//iteracie cez kazdu konturu
	while (contours)
	{
		result = cvApproxPoly(contours, sizeof(CvContour), storage, CV_POLY_APPROX_DP, cvContourPerimeter(contours)*0.02, 0);

		if (result->total == 3 && fabs(cvContourArea(result, CV_WHOLE_SEQ)) < max && fabs(cvContourArea(result, CV_WHOLE_SEQ)) > min)

		{
			//iterating through each point
			CvPoint *pt[3];
			for (int i = 0; i < 3; i++) {
				pt[i] = (CvPoint*)cvGetSeqElem(result, i);
			}
			//drawing lines around the triangle
			cvLine(img, *pt[0], *pt[1], cvScalar(255, 0, 0), 4);
			cvLine(img, *pt[1], *pt[2], cvScalar(255, 0, 0), 4);
			cvLine(img, *pt[2], *pt[0], cvScalar(255, 0, 0), 4);
		}

		else if (result->total == 4 && fabs(cvContourArea(result, CV_WHOLE_SEQ)) < max && fabs(cvContourArea(result, CV_WHOLE_SEQ)) > min)
		{
			//cout << fabs(cvContourArea(result, CV_WHOLE_SEQ)) << endl;
			//iterating through each point
			CvPoint *pt[4];
			for (int i = 0; i < 4; i++) {
				pt[i] = (CvPoint*)cvGetSeqElem(result, i);
			}

			//drawing lines around the quadrilateral
			cvLine(img, *pt[0], *pt[1], cvScalar(0, 255, 0), 4);
			cvLine(img, *pt[1], *pt[2], cvScalar(0, 255, 0), 4);
			cvLine(img, *pt[2], *pt[3], cvScalar(0, 255, 0), 4);
			cvLine(img, *pt[3], *pt[0], cvScalar(0, 255, 0), 4);
		}

		else if (result->total == 7 && fabs(cvContourArea(result, CV_WHOLE_SEQ))<max && fabs(cvContourArea(result, CV_WHOLE_SEQ))>min)
		{
			//iterating through each point
			CvPoint *pt[7];
			for (int i = 0; i<7; i++) {
				pt[i] = (CvPoint*)cvGetSeqElem(result, i);
			}

			//drawing lines around the quadrilateral
			cvLine(img, *pt[0], *pt[1], cvScalar(184, 138, 0), 4);
			cvLine(img, *pt[1], *pt[2], cvScalar(184, 138, 0), 4);
			cvLine(img, *pt[2], *pt[3], cvScalar(184, 138, 0), 4);
			cvLine(img, *pt[3], *pt[4], cvScalar(184, 138, 0), 4);
			cvLine(img, *pt[4], *pt[5], cvScalar(184, 138, 0), 4);
			cvLine(img, *pt[5], *pt[6], cvScalar(184, 138, 0), 4);
			cvLine(img, *pt[6], *pt[0], cvScalar(184, 138, 0), 4);
		}
		//7


		//obtain the next contour
		contours = contours->h_next;
	}

	////////////////////////////////////////////////////////// 
	//KRUH
	IplImage* gray = cvCreateImage(cvGetSize(img), 8, 1);
	cvCvtColor(img, gray, CV_BGR2GRAY);
	cvSmooth(gray, gray, CV_GAUSSIAN, 9, 9);
	CvSeq* circles = cvHoughCircles(gray, storage, CV_HOUGH_GRADIENT, 2, gray->height / 4, 200, 100);
	int i;

	cvNamedWindow("uprava pre kruhom, smooth", WINDOW_NORMAL);
	cvShowImage("uprava pre kruhom, smooth", gray);
	cv::moveWindow("uprava pre kruhom, smooth", 640, 0);
	cvResizeWindow("uprava pre kruhom, smooth", 320, 240);

	for (i = 0; i < circles->total; i++)
	{
		float* p = (float*)cvGetSeqElem(circles, i);
		cvCircle(img, cvPoint(cvRound(p[0]), cvRound(p[1])),
			3, CV_RGB(0, 255, 0), -1, 8, 0);
		cvCircle(img, cvPoint(cvRound(p[0]), cvRound(p[1])),
			cvRound(p[2]), CV_RGB(255, 0, 0), 3, 8, 0);
	}

	cvNamedWindow("Detection", WINDOW_NORMAL);
	cvShowImage("Detection", img);
	cv::moveWindow("Detection", 960, 0);
	cvResizeWindow("Detection", 320, 240);
}


///////////////
using namespace std;

int main()
{
	////////////------GALERIA----///////////
	string defaultCesta = "../data/fotkaCislo";
	string cesta;
	int pocetFotiek = 77;

	printf("\nPrehravanie galerie\n");
	printf("\nOvladanie pismenami 'a' a 'd',\nvypnutie stlacenim q\n");
	int index = 25;
	std::string si;

	IplImage* img;

	printf("\nFotka 1\n");
	si = std::to_string(index);
	cesta = defaultCesta + si + ".bmp";
	Detekcia(cesta);
	waitKey(1000);
	int p = 0;
	int timer = 0;
	char znak;
	while (1) {
		znak = _getch();
		if (znak == 'p') {
			if (p == 1) {
				p = 0;
			}
			else {
				p = 1;
				timer = 3000;
				while ((index < pocetFotiek)) {
					znak = _getch();
					if (znak == 'q') {
						return 0;
					}
					if (znak == 'p') {
						break;
					}
					waitKey(timer);
					index += 1;
					printf("\nFotka %d\n", index);
					si = std::to_string(index);
					cesta = defaultCesta + si + ".bmp";
					Detekcia(cesta);
				}
			}
		}
		if ((znak == 'd') && (index < pocetFotiek)) {

			index += 1;
			printf("\nFotka %d\n", index);
			si = std::to_string(index);
			cesta = defaultCesta + si + ".bmp";
			Detekcia(cesta);
			waitKey(30);
		}
		if ((znak == 'a') && (index > 1)) {
			index -= 1;
			printf("\nFotka %d\n", index);
			si = std::to_string(index);
			cesta = defaultCesta + si + ".bmp";
			Detekcia(cesta);
			waitKey(30);
		}
		if (znak == 'q') {
			printf("Koniec");
			//cleaning up
			cvDestroyAllWindows();
			std::this_thread::sleep_for(std::chrono::milliseconds(1000));
			break;
		}
	}


	return 0;
}
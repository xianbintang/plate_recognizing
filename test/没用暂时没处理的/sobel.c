#include "cv.h"
#include "highgui.h"
#include <math.h>

void Mysobel(IplImage * gray_img, IplImage * sobel_img, int flag);
void show_image_value_one(IplImage * img);


int main(int argc, char * argv[])
{
	IplImage  * img = cvLoadImage(argv[1]);
	IplImage  * gray = cvCreateImage(cvGetSize(img), IPL_DEPTH_8U, 1);
	IplImage * mysobel = cvCreateImage(cvGetSize(img), IPL_DEPTH_16S, 1);  
	IplImage * mysobel1 = cvCreateImage(cvGetSize(img), IPL_DEPTH_16S, 1);  

	IplImage * cvsobel = cvCreateImage(cvGetSize(img), IPL_DEPTH_16S, 1);
	IplImage * cov = cvCreateImage(cvGetSize(img), IPL_DEPTH_8U, 1);
	IplImage * cov1 = cvCreateImage(cvGetSize(img), IPL_DEPTH_8U, 1);

	cvNamedWindow("img");
	cvNamedWindow("gray");
	cvNamedWindow("mysobel");
	cvNamedWindow("cvsobel");
	cvNamedWindow("cov");
	cvNamedWindow("cov1");

	cvShowImage("img", img);

	cvCvtColor(img, gray, CV_BGR2GRAY);

	cvShowImage("gray", gray);

//	Mysobel(gray, mysobel1, 1);
	Mysobel(gray, mysobel, 1);

//	Mysobel(mysobel1, mysobel, 0);

	cvSobel(gray, cvsobel, 1, 0, 3);
	cvConvertScaleAbs(mysobel, cov1, 1, 0);
	cvConvertScaleAbs(cvsobel, cov, 1, 0);
	show_image_value_one(mysobel);

	cvShowImage("mysobel", mysobel);
	cvShowImage("cvsobel", cvsobel);
	cvShowImage("cov", cov);
	cvShowImage("cov1", cov1);

	cvWaitKey(0);
	return 0;
}




#if 1
void Mysobel(IplImage * gray_img, IplImage * sobel_img, int flag)
{
	unsigned char a00, a01, a02;
	unsigned char a10, a11, a12;
	unsigned char a20, a21, a22;
	int ux,uy;

	//边缘几个点无法求导，忽略啦,所以从1开始
	for (int i = 1; i < gray_img->height - 1; i++) {
		unsigned char * prow = (unsigned char *)(gray_img->imageData + (i - 1) * gray_img->widthStep);

		unsigned char * prow1 = (unsigned char *)(gray_img->imageData + (i) * gray_img->widthStep);

		unsigned char * prow2 = (unsigned char *)(gray_img->imageData + (i + 1) * gray_img->widthStep);

		unsigned char * srow = (unsigned char *)(sobel_img->imageData + (i) * sobel_img->widthStep);



		for (int j = 1; j < gray_img->width - 1; j++){
			a00 = prow[j - 1];
			a01 = prow[j + 0];
			a02 = prow[j + 1];

			a10 = prow1[j - 1];
			a11 = prow1[j + 0];
			a12 = prow1[j + 1];

			a20 = prow2[j - 1];
			a21 = prow2[j + 0];
			a22 = prow2[j + 1];

			if (j == 20 && i == 20)
			  printf("%d %d %d %d %d %d %d %d %d\n", a00, a01, a02,a10, a11, a12, a20, a21, a22);

			ux = a20 * (1) + a21 * (2) + a22 * (1) + (a00 * (-1) + a01 * (-2) + a02 * (-1));
			uy = a02 * (1) + a12 * (2) + a22 * (1) + a00 * (-1) + a10 * (-2) + a20 * (-1);

#if 1
			if (flag == 1)
			  srow[j] = ux;
			else
			  srow[j] = uy;
#endif
//		srow[j] = (int)sqrt(ux * ux + uy * uy);
//			printf("%d ", srow[j + 1]);
		}
//		printf("\n");
	}
}
#endif
#if 0
 void Mysobel(IplImage* gray, IplImage* gradient, int flag)
 {
     /* Sobel template
     a00 a01 a02
     a10 a11 a12
     a20 a21 a22
     */
 
     unsigned char a00, a01, a02, a20, a21, a22;
    unsigned char a10, a11, a12;
 
     for (int i=1; i<gray->height-1; ++i)
     {
         for (int j=1; j<gray->width-1; ++j)
         {
             CvScalar color = cvGet2D(gray, i, j);
     
             a00 = cvGet2D(gray, i-1, j-1).val[0];
             a01 = cvGet2D(gray, i-1, j).val[0];
             a02 = cvGet2D(gray, i-1, j+1).val[0];
 
             a10 = cvGet2D(gray, i, j-1).val[0];
             a11 = cvGet2D(gray, i, j).val[0];
             a12 = cvGet2D(gray, i, j+1).val[0];
 
             a20 = cvGet2D(gray, i+1, j-1).val[0];
             a21 = cvGet2D(gray, i+1, j).val[0];
             a22 = cvGet2D(gray, i+1, j+1).val[0];
             

			if (j == 20 && i == 20)
			  printf("%d %d %d %d %d %d %d %d %d\n", a00, a01, a02,a10, a11, a12, a20, a21, a22);
	
             // x方向上的近似导数
             double ux = a20 * (1) + a21 * (2) + a22 * (1) 
                 + (a00 * (-1) + a01 * (-2) + a02 * (-1));
 
             // y方向上的近似导数
             double uy = a02 * (1) + a12 * (2) + a22 * (1)
                 + a00 * (-1) + a10 * (-2) + a20 * (-1);
#if 0
			 if (flag == 1)
				color.val[0] = uy;
			 else
			   color.val[0] = ux;
#endif
 
			 color.val[0] = sqrt(ux * ux + uy * uy);
             cvSet2D(gradient, i, j, color);
         }
     }
 }
#endif
void show_image_value_one(IplImage * img)
{
	int i, j;
	FILE * fp_value;

	fp_value = fopen("value.txt", "w");

	for (i = 0; i < img->height; i ++) {
		unsigned char * prow = (unsigned char *)(img->imageData + i * img->widthStep);
	
		for (j = 0; j < img->width; j++) {
			fprintf(fp_value, "%d ", prow[j]);
		}
		fprintf(fp_value, "\n");

	}

}



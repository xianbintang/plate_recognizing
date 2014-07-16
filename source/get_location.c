#include "include/plate.h"
#include "include/List.h"

static void dilate_erode_x(IplImage * img_after_threshold, IplImage * img_final); 
static void dilate_erode_y(IplImage * img_final);
static void filter_rect_by_area(List src_rects, List dst_rects, int total_area);
/*
输入:预处理完成后的车辆图像
输出:车牌定位的位置
作者:唐显斌
版本:v1.2
基本思路:
1.找到膨胀腐蚀后的所有外接矩形轮廓
2.根据矩形轮廓的形状比例进行筛选
3.根据颜色以及形状比例进行筛选
4.用循环控制膨胀腐蚀的次数,找到最优矩形即为车牌位置
 */

List get_location(IplImage * img_car)
{
	/*分别为用来装预处理后的图像中所有矩形轮廓和筛选过后的轮廓的链表*/
	List rects = create_list();
	List rects_final = create_list();

	IplImage * img_after_preprocess = cvLoadImage("img_after_preprocess.bmp", -1);

	if (img_after_preprocess == NULL) {
		fprintf(stderr, "Can not load image -- img_after_preprocess.bmp!\n");
		exit(-1);
	}

	/*在检测轮廓时需要用到的两个量,storage为容器,contours为指向轮廓的指针*/
	CvMemStorage * storage = cvCreateMemStorage(0);
	CvSeq * contours = NULL;

	/*先进行膨胀腐蚀*/
	/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!*/
	/*重点改进之处,膨胀和腐蚀的次数是很关键的*/
	dilate_erode_x(img_after_preprocess, img_after_preprocess);
	dilate_erode_y(img_after_preprocess);

	/*找到所有的矩形轮廓*/
	get_contour_rect(img_after_preprocess, rects, storage, contours);/*没什么可以改进的地方*/
	assert( rects->next != NULL);

	/*按照形状进行矩形筛选*/
	/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!*/
	/*重点改进之处,如何筛选出目标矩形是重点*/
	filter_rect_by_shape(rects->next, rects_final);				
	assert(rects_final->next != NULL);

	/*注意rects有头结点,所以传进去的时候忽略掉头结点*/
	draw_contour_rect(img_after_preprocess, rects_final->next);

	return rects_final->next;
}

/*通过形状比例筛选出满足形状比例的矩形*/
void filter_rect_by_shape(List src_rects, List dst_rects)
{
	/*要保证dst_rects不为空才行!!*/
	if (src_rects == NULL) {
		fprintf(stderr, "src_rects is empty!\n");
		exit(-1);
	}
	while (src_rects != NULL) {
		double scale = 1.0 * (src_rects->item.width) / (src_rects->item.height);
		double area_of_rect = 1.0 * (src_rects->item.width) * (src_rects->item.height);

		/*车牌有固定的形状比例以及大小比例,先按这个粗略提取出车牌位置*/
		if (scale <= SCALE_MAX && scale >= SCALE_MIN && area_of_rect <= AREA_MAX && area_of_rect >= AREA_MIN) {
			push_back(dst_rects, src_rects->item);
//			print_area_of_rect(dst_rects->next->item);
			dst_rects = dst_rects->next;
		}
		src_rects = src_rects->next;
	}
}


/*自定义膨胀腐蚀操作,在循环中使用可以增加定位准确度*/
void dilate_erode_x(IplImage * img_after_threshold, IplImage * img_final) {
	/*自定义1*3的核进行X方向的膨胀腐蚀*/
	IplConvKernel* kernal = cvCreateStructuringElementEx(3,1, 1, 0, CV_SHAPE_RECT);
	cvDilate(img_after_threshold, img_final, kernal, 3);/*X方向膨胀连通数字*/
	cvErode(img_final, img_final, kernal, 1);/*X方向腐蚀去除碎片*/
	cvDilate(img_final, img_final, kernal, 3);/*X方向膨胀回复形态*/
}

void dilate_erode_y(IplImage * img_final) {
	/*自定义3*1的核进行Y方向的膨胀腐蚀*/
	IplConvKernel* kernal = cvCreateStructuringElementEx(1, 3, 0, 1, CV_SHAPE_RECT);
	cvErode(img_final, img_final, kernal, 3);/*Y方向腐蚀去除碎片*/
	cvDilate(img_final, img_final, kernal, 3);/*回复形态*/
}




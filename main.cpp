#include <iostream>
#include <cmath>
#include <fstream>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>

using namespace std;
using namespace cv;

int W=1920;
int H=1080;

IplImage *getFrame(CvSize size,int type, CvPoint point1, CvPoint point2 = cvPoint(0, 0), CvPoint point3 = cvPoint(0, 0), CvPoint point4 = cvPoint(0, 0)) {
    IplImage *image = cvCreateImage(size, IPL_DEPTH_8U, 3);
    cvSet(image, cvScalar(0, 0, 0));
	int radius = 100;
	//cvRectangle(image,point2,cvPoint(point2.x+150,point2.y+150),CV_RGB(250,255, 255),5,30);
	switch(type){
		case 0:
			cvCircle(image, point1, 5, CV_RGB(255, 255, 255), 0, 8);
			break;
		case 1:
			cvLine(image, cvPoint(point1.x-radius/2, point1.y-radius/2), cvPoint(point1.x+radius/2, point1.y+radius/2),CV_RGB(250,255, 255),8,8);
			cvLine(image, cvPoint(point1.x-radius/2, point1.y+radius/2), cvPoint(point1.x+radius/2, point1.y-radius/2),CV_RGB(250,255, 255),8,8);
			//cvLine(image, cvPoint(point2.x-radius/2, point2.y), cvPoint(point2.x+radius/2, point2.y),CV_RGB(250,255, 255),8,8);
			cvLine(image, cvPoint(point2.x, point2.y), cvPoint(point2.x+radius/2, point2.y+radius*(sqrt(3.0)/2)),CV_RGB(250,255, 255),6,8);
			cvLine(image, cvPoint(point2.x, point2.y), cvPoint(point2.x-radius/2, point2.y+radius*(sqrt(3.0)/2)),CV_RGB(250,255, 255),6,8);
			cvLine(image, cvPoint(point2.x+radius/2, point2.y+radius*(sqrt(3.0)/2)), cvPoint(point2.x-radius/2, point2.y+radius*(sqrt(3.0)/2)),CV_RGB(250,255, 255),6,8);
			cvCircle(image, point3, 50, CV_RGB(255, 255, 255), -1, 8);
		case 2:
			//cvCircle(image, point3, 50, CV_RGB(255, 255, 255), 1, 8);
			
			break;
		default:
			break;
	}
    return image;
}

CvPoint getPoint(int time,int flag){
	CvPoint cvpoint;
	int x0=W/2+200;
	int y0=H/2+100;
	double r,teta;
	switch(flag){
	case 0:
		r=100,teta=(25+time)*0.1;
		cvpoint.x=x0+r*cos(teta)+teta*sin(teta);
		cvpoint.y=y0+r*sin(teta)-teta*cos(teta);
		break;
	case 1:
		teta=(25+time)*0.01;
		r=500*sin(2*teta);
		cvpoint.x=x0+r*cos(teta);
		cvpoint.y=y0+r*sin(teta);
		break;
	case 2:
		cvpoint.x=time*0.7;
		cvpoint.y=900;
		if(time>1200){
			cvpoint.y=800;
			cvpoint.x=time-1000;
		}
		break;
	case 3:
		r=100,teta=(25+time)*0.1;
		cvpoint.x=700+2*r*cos(teta)-r*cos(2*teta);
		cvpoint.y=400+2*r*sin(teta)-r*sin(2*teta);
		break;
	case 4:
		teta=(25+time)*0.004;
		r=200;
		cvpoint.x=r*(teta-sin(teta));
		cvpoint.y=300+r*(1-cos(teta));
		break;
	case 5:
		teta=(25+time)*0.01;
		r=500*sin(5*teta);
		cvpoint.x=x0+r*cos(teta);
		cvpoint.y=y0-200+r*sin(teta);
		break;
	case 6:
		teta=(60+time)*0.01;
		r=300;
		cvpoint.x=x0+r*cos(teta)*cos(teta)*cos(teta);
		cvpoint.y=y0-200+r*sin(teta)*sin(teta)*sin(teta);
		break;
	default:
			break;
	}
	return cvpoint;
}

int main() {
    // Создаёи 8-битную, 3-канальную картинку
	
	ofstream cross;
	ofstream tiangle;
	ofstream dot;
	cross.open("cross11.txt");
	tiangle.open("tiangle11.txt");
	dot.open("dot11.txt");
	const char *filename = "capture11.avi";
    cvNamedWindow("Hello World", CV_WINDOW_NORMAL);
    cvSetWindowProperty("Hello World", CV_WND_PROP_FULLSCREEN, CV_WINDOW_FULLSCREEN);
    
    int fps = 60;
    CvVideoWriter *writer = cvCreateVideoWriter(filename, CV_FOURCC('P','I','M','1'), fps, cvSize(1920,1080), 1);
    assert(writer!=0);

    // ждём нажатия клавиши
    //cvWaitKey(27);
    for (int i = 0; i < fps*60; i++) {
		CvPoint point=getPoint(i,6);
		CvPoint point2=getPoint(i,3);
		CvPoint point3=getPoint(i,1);
        cross << point.x << "  " << point.y <<"\n";
		tiangle << point2.x << "  " << point2.y+43 <<"\n";
		dot<<point3.x<<"  "<<point3.y/*<<"  "<< i*/<<"\n";
        IplImage *image = getFrame(cvSize(W,H),1,point, point2, point3);
        cvWriteFrame(writer, image);
        cvShowImage("Hello World", image);
        cvReleaseImage(&image);
        if (cvWaitKey(1) == 27) { // если нажата ESC - выходим
            break;
        }

    }
    // освобождаем ресурсы
    cvReleaseVideoWriter(&writer);
	cross.close(); 
	tiangle.close();
	dot.close();
	
	
	//cvNamedWindow("Hello World", CV_WINDOW_NORMAL);
 //   cvSetWindowProperty("Hello World", CV_WND_PROP_FULLSCREEN, CV_WINDOW_FULLSCREEN);
	//IplImage *image = cvCreateImage(cvSize(W,H), IPL_DEPTH_8U, 3);
 //   cvSet(image, cvScalar(0, 0, 0));
	//cvLine(image, cvPoint(100, 50), cvPoint(150, 150),CV_RGB(250,255, 255),6,8);
	//cvLine(image, cvPoint(100, 50), cvPoint(50, 150),CV_RGB(250,255, 255),6,8);
	//cvLine(image, cvPoint(150, 150), cvPoint(50, 150),CV_RGB(250,255, 255),6,8);
	//cvLine(image, cvPoint(50, 50), cvPoint(150, 150),CV_RGB(250,255, 255),8,8);
	//cvLine(image, cvPoint(50, 150), cvPoint(150, 50),CV_RGB(250,255, 255),8,8);
	//cvCircle(image, cvPoint(100,100), 5, CV_RGB(255, 255, 255), -1, 8);
	//cvCircle(image, cvPoint(100,1000), 5, CV_RGB(255, 255, 255), -1, 8);
	//cvCircle(image, cvPoint(1700,1000), 5, CV_RGB(255, 255, 255), -1, 8);
	//cvCircle(image, cvPoint(1700,100), 5, CV_RGB(255, 255, 255), -1, 8);
	//while(true){
	//	cvShowImage("Hello World", image);
	//    if (cvWaitKey(1) == 27) { // если нажата ESC - выходим
	//		cvSaveImage("cal.png",image);
 //       break;
 //       }
	//}
	//
    cvDestroyWindow("Hello World");
    return 0;

}


//int main(){
//    cv::Mat src = cv::imread("calibration.jpg", 1); // Инициализация изображения
//	cv::imshow("Test", src);
//	cv::Mat gray;
//	cvtColor(src, gray, cv::COLOR_BGR2GRAY); // Перевод в чёрно-белое
//	cv::CascadeClassifier cascadeSymbol; // Объявление каскада
//	bool cascadeSymbolLoad = cascadeSymbol.load("cascadeCross3.xml"); // Загрузка каскада
// 
//	if(!cascadeSymbolLoad){
//		std::cerr << "Cascade not load. Check your directory \"haarcascade_russian_plate_number_symbol.xml\"" << std::endl;
//		return false;
//	}
//	
//	std::vector<cv::Rect> symbols;
//	cascadeSymbol.detectMultiScale(gray, symbols); // Поиск с помощью каскада
//	
//	std::vector<cv::Rect>::iterator iter = symbols.begin();
//	int i=0;
//	while (iter != symbols.end() && i<10000) {
//		cv::Rect p = *iter;
//		cv::Point symbolBegin	= cv::Point(p.x, p.y);
//		cv::Point symbolEnd		= cv::Point(p.x+p.width, p.y+p.height);
//		
//		std::cout << "X: " << p.x << " Y: " << p.y << " Width: " << p.width << " Height: " << p.height << std::endl;
//		
//		rectangle(src, symbolBegin, symbolEnd, cv::Scalar(0,255,0), 2);	
//		i++;
//	}
//	
//	cv::imshow("Test228", src);
//	
//    cv::waitKey(0);
//	
//	return 0;
//}



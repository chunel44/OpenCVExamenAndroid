#include "com_estrada_examen_MyNdk.h"
#define PKT "com/estrada/examen/MyNdk"


JNIEXPORT jstring JNICALL Java_com_estrada_examen_MyNdk_getMyString
  (JNIEnv *env, jobject){
    return (*env).NewStringUTF("Hola Mundo NDK!!!");
  }

  JNIEXPORT jint JNICALL Java_com_estrada_examen_MyNdk_suma
      (JNIEnv *env, jobject, jint num){
        return factorial(num);
      }

  jint factorial(jint n){
    if(n==0) return 1;
    else return n*factorial(n-1);
     }

JNIEXPORT jint JNICALL Java_com_estrada_examen_MyNdk_convertGray
        (JNIEnv *env, jobject, jlong addrRgba, jlong addrGray){
    Mat& mRgb = *(Mat*)addrRgba;
    Mat& mGray = *(Mat*)addrGray;
    int conv;
    jint retVal;
    conv = toGray(mRgb, mGray);

    retVal = (jint)conv;
    return retVal;
}

int toGray(Mat img, Mat& gray){
    cvtColor(img, gray, CV_RGBA2GRAY);
    if(gray.rows==img.rows && gray.cols == img.cols)
        return 1;
        return 0;
}

cv::Point2f computeIntersect(cv::Vec4i a, cv::Vec4i b) {
    int x1 = a[0], y1 = a[1], x2 = a[2], y2 = a[3];
    int x3 = b[0], y3 = b[1], x4 = b[2], y4 = b[3];

    if (float d = ((float)(x1 - x2) * (y3 - y4)) - ((y1 - y2) * (x3 - x4)))
    {
        cv::Point2f pt;
        pt.x = ((x1*y2 - y1*x2) * (x3 - x4) - (x1 - x2) * (x3*y4 - y3*x4)) / d;
        pt.y = ((x1*y2 - y1*x2) * (y3 - y4) - (y1 - y2) * (x3*y4 - y3*x4)) / d;
        return pt;
    }
    else
        return cv::Point2f(-1, -1);
}

bool comparator2(double a, double b) {
    return a<b;
}
bool comparator3(Vec3f a, Vec3f b) {
    return a[0]<b[0];
}

bool comparator(Point2f a, Point2f b) {
    return a.x<b.x;
}
void sortCorners(std::vector<cv::Point2f>& corners, cv::Point2f center)
{


    std::vector<cv::Point2f> top, bot;
    for (int i = 0; i < corners.size(); i++)
    {
        if (corners[i].y < center.y)
            top.push_back(corners[i]);
        else
            bot.push_back(corners[i]);
    }


    sort(top.begin(), top.end(), comparator);
    sort(bot.begin(), bot.end(), comparator);

    cv::Point2f tl = top[0];
    cv::Point2f tr = top[top.size() - 1];
    cv::Point2f bl = bot[0];
    cv::Point2f br = bot[bot.size() - 1];
    corners.clear();
    corners.push_back(tl);
    corners.push_back(tr);
    corners.push_back(br);
    corners.push_back(bl);
}

void ModifyString(JNIEnv *env, jobject obj, jstring texto){
jclass thisClass = env->GetObjectClass(obj);
// Get the Field ID of the instance variables "message"
jfieldID fidMessage = env->GetFieldID(thisClass, "message", "Ljava/lang/String;");
if (NULL == fidMessage) return;

// String
// Get the object given the Field ID
jstring message = (jstring) env->GetObjectField(obj, fidMessage);

// Create a C-string with the JNI String
const char *cStr = env->GetStringUTFChars(message, NULL);
if (NULL == cStr) return;

printf("In C, the string is %s\n", cStr);
__android_log_print(ANDROID_LOG_INFO, "SomeTag", "In C, the string is %s\n", cStr);
env->ReleaseStringUTFChars(message, cStr);

const char *nativeString = env->GetStringUTFChars(texto, NULL);
// Create a new C-string and assign to the JNI string
message = env->NewStringUTF(nativeString);
env->ReleaseStringUTFChars(message, nativeString);
if (NULL == message) return;

// modify the instance variables
env->SetObjectField(obj, fidMessage, message);
}

jstring int_array_to_string(int int_array[], int size_of_array, JNIEnv *env) {
    ostringstream oss("");
    jstring oss2;
    for (int temp = 0; temp < size_of_array; temp++) {
        oss << int_array[temp];
    }
    string o = oss.str();
    jstring t = env->NewStringUTF(o.c_str());
    return t;
}

JNIEXPORT void JNICALL Java_com_estrada_examen_MyNdk_detect
(JNIEnv *env, jobject obj, jlong addrRgba){
    Mat& quad = *(Mat*)addrRgba;
    detect(quad,env,obj);

    }

    void detect(Mat& quad,JNIEnv *env, jobject obj){
        cv::Size size(3, 3);
        cvtColor(quad, quad, COLOR_BGR2GRAY);
        cv::GaussianBlur(quad, quad, size, 0);
        adaptiveThreshold(quad, quad, 255, CV_ADAPTIVE_THRESH_MEAN_C, CV_THRESH_BINARY, 75, 10);
        //threshold(img,img,0,255,THRESH_OTSU);
        cv::bitwise_not(quad, quad);

        Mat cimg;
        //cvtColor(quad, cimg, CV_BGR2GRAY);
        vector<Vec3f> circles;
        HoughCircles(quad, circles, CV_HOUGH_GRADIENT, 1, quad.rows / 8, 100, 75, 0, 0);
        for (size_t i = 0; i < circles.size(); i++) {
            Point center(cvRound(circles[i][0]), cvRound(circles[i][1]));
            // circle center
            circle(quad, center, 3, Scalar(0, 255, 0), -1, 8, 0);
        }

        double averR = 0;
        vector<double> row;
        vector<double> col;

        //Find rows and columns of circles for interpolation
        for (int i = 0; i<circles.size(); i++) {
            bool found = false;
            int r = cvRound(circles[i][2]);
            averR += r;
            int x = cvRound(circles[i][0]);
            int y = cvRound(circles[i][1]);
            for (int j = 0; j<row.size(); j++) {
                double y2 = row[j];
                if (y - r < y2 && y + r > y2) {
                    found = true;
                    break;
                }
            }
            if (!found) {
                row.push_back(y);
            }
            found = false;
            for (int j = 0; j<col.size(); j++) {
                double x2 = col[j];
                if (x - r < x2 && x + r > x2) {
                    found = true;
                    break;
                }
            }
            if (!found) {
                col.push_back(x);
            }
        }
        static int arr[254];

        averR /= circles.size();

        sort(row.begin(), row.end(), comparator2);
        sort(col.begin(), col.end(), comparator2);

        for (int i = 0; i<row.size(); i++) {
            double max = 0;
            double y = row[i];
            int ind = -1;
            for (int j = 0; j<col.size(); j++) {
                double x = col[j];
                Point c(x, y);

                //Use an actual circle if it exists
                for (int k = 0; k<circles.size(); k++) {
                    double x2 = circles[k][0];
                    double y2 = circles[k][1];
                    if (abs(y2 - y)<averR && abs(x2 - x)<averR) {
                        x = x2;
                        y = y2;
                    }
                }

                // circle outline
                circle(quad, c, averR, Scalar(0, 0, 255), 3, 8, 0);
                Rect rect(x - averR, y - averR, 2 * averR, 2 * averR);
                //Mat submat = cimg(rect);
                Mat submat = quad(rect);
                double p = (double)countNonZero(submat) / (submat.size().width*submat.size().height);
                if (p >= 0.3 && p>max) {
                    max = p;
                    ind = j;
                }
            }
            //string str[50];
            if (ind == -1)printf("%d:-", i + 1);
            else {
                __android_log_print(ANDROID_LOG_INFO, "SomeTag2", "%c", 'A'+ind);
                arr[i] = ind;
            }
        }

        jstring respuestas = int_array_to_string(arr, col.size() , env);
        const char *nativeString = env->GetStringUTFChars(respuestas, NULL);
        __android_log_print(ANDROID_LOG_INFO, "SomeTag2", "%s", nativeString);
       // ModifyString(JNIEnv *env, jobject obj, jstring texto)

      // ModifyString(env,obj, respuestas);

        //env->ReleaseStringUTFChars(respuestas, nativeString);

}

JNIEXPORT void JNICALL Java_com_estrada_examen_MyNdk_black
(JNIEnv *env, jobject obj, jlong addrRgba){
    Mat& img2 = *(Mat*)addrRgba;
    black(img2);
}

void black(Mat& img){

    cv::Size size(3, 3);
    cvtColor(img, img, COLOR_BGR2GRAY);
    cv::GaussianBlur(img, img, size, 0);
    adaptiveThreshold(img, img, 255, CV_ADAPTIVE_THRESH_MEAN_C, CV_THRESH_BINARY, 75, 10);
    //threshold(img,img,0,255,THRESH_OTSU);
    cv::bitwise_not(img, img);
/*
    cv::Mat img2;
    cvtColor(img, img2, CV_GRAY2RGB);

    cv::Mat img3;
    cvtColor(img, img3, CV_GRAY2RGB);

    vector<Vec4i> lines;
    HoughLinesP(img, lines, 1, CV_PI / 180, 80, 400, 10);
    for (size_t i = 0; i < lines.size(); i++)
    {
        Vec4i l = lines[i];
        line(img, Point(l[0], l[1]), Point(l[2], l[3]), Scalar(0, 0, 255), 3, CV_AA);
    }*/
}

JNIEXPORT jintArray JNICALL Java_com_estrada_examen_MyNdk_prueba(JNIEnv *env, jobject obj, jint size){

jintArray result;
result = env->NewIntArray(size);
if (result == NULL) {
return NULL; /* out of memory error thrown */
}
int i;
// fill a temp structure to use to populate the java int array
int fill[256];
for (i = 0; i < size; i++) {
fill[i] = i+1;
}
// move from the temp structure to the java structure
env->SetIntArrayRegion(result, 0, size, fill);
return result;
}


JNIEXPORT void JNICALL Java_com_estrada_examen_MyNdk_modifyInstanceVariable(JNIEnv *env, jobject obj,jstring text){
int arr[256];
for(int i = 0; i<5; i++){
arr[i] = i;
}
jstring texto2 = int_array_to_string(arr, 5, env);
ModifyString(env, obj,texto2);
}

JNIEXPORT jintArray JNICALL Java_com_estrada_examen_MyNdk_resultado(JNIEnv *env, jobject obj, jlong addrRgba){
Mat& quad = *(Mat*)addrRgba;
jintArray result;
jint size2 = 5;
result = env->NewIntArray(size2);
if (result == NULL) {
    return NULL; /* out of memory error thrown */
}
int fill[256];
cv::Size size(3, 3);
cvtColor(quad, quad, COLOR_BGR2GRAY);
cv::GaussianBlur(quad, quad, size, 0);
adaptiveThreshold(quad, quad, 255, CV_ADAPTIVE_THRESH_MEAN_C, CV_THRESH_BINARY, 75, 10);
//threshold(img,img,0,255,THRESH_OTSU);
cv::bitwise_not(quad, quad);

Mat cimg;
//cvtColor(quad, cimg, CV_BGR2GRAY);
vector<Vec3f> circles;
HoughCircles(quad, circles, CV_HOUGH_GRADIENT, 1, quad.rows / 8, 100, 75, 0, 0);
for (size_t i = 0; i < circles.size(); i++) {
Point center(cvRound(circles[i][0]), cvRound(circles[i][1]));
// circle center
circle(quad, center, 3, Scalar(0, 255, 0), -1, 8, 0);
}

double averR = 0;
vector<double> row;
vector<double> col;

//Find rows and columns of circles for interpolation
for (int i = 0; i<circles.size(); i++) {
bool found = false;
int r = cvRound(circles[i][2]);
averR += r;
int x = cvRound(circles[i][0]);
int y = cvRound(circles[i][1]);
for (int j = 0; j<row.size(); j++) {
double y2 = row[j];
if (y - r < y2 && y + r > y2) {
found = true;
break;
}
}
if (!found) {
row.push_back(y);
}
found = false;
for (int j = 0; j<col.size(); j++) {
double x2 = col[j];
if (x - r < x2 && x + r > x2) {
found = true;
break;
}
}
if (!found) {
col.push_back(x);
}
}

averR /= circles.size();

sort(row.begin(), row.end(), comparator2);
sort(col.begin(), col.end(), comparator2);

for (int i = 0; i<row.size(); i++) {
double max = 0;
double y = row[i];
int ind = -1;
for (int j = 0; j<col.size(); j++) {
double x = col[j];
Point c(x, y);

//Use an actual circle if it exists
for (int k = 0; k<circles.size(); k++) {
double x2 = circles[k][0];
double y2 = circles[k][1];
if (abs(y2 - y)<averR && abs(x2 - x)<averR) {
x = x2;
y = y2;
}
}

// circle outline
circle(quad, c, averR, Scalar(0, 0, 255), 3, 8, 0);
Rect rect(x - averR, y - averR, 2 * averR, 2 * averR);
//Mat submat = cimg(rect);
Mat submat = quad(rect);
double p = (double)countNonZero(submat) / (submat.size().width*submat.size().height);
if (p >= 0.3 && p>max) {
max = p;
ind = j;
}
}
//string str[50];
if (ind == -1)printf("%d:-", i + 1);
else {
__android_log_print(ANDROID_LOG_INFO, "SomeTag", "%c", 'A' + ind);
fill[i] = ind;
}
}
env->SetIntArrayRegion(result, 0, size2, fill);
return result;
}
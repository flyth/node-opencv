#include "OpenCV.h"
#include <opencv2/contrib/contrib.hpp>

using namespace std;

class FaceRecognizer: public node::ObjectWrap {
	public:
	cv::Ptr<cv::FaceRecognizer> fr;

    static Persistent<FunctionTemplate> constructor;
    static void Init(Handle<Object> target);
    static Handle<Value> New(const Arguments &args);

    FaceRecognizer();      

    JSFUNC(Train)
    JSFUNC(Update)
    JSFUNC(Predict)
    JSFUNC(Save)
    JSFUNC(Load)
};


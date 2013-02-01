#include "OpenCV.h"
#include "Matrix.h"
#include "FaceRecognizer.h"

v8::Persistent<FunctionTemplate> FaceRecognizer::constructor;

cv::Scalar setColor(Local<Object> objColor);

void
FaceRecognizer::Init(Handle<Object> target) {
	HandleScope scope;

	//Class
	v8::Local<v8::FunctionTemplate> m = v8::FunctionTemplate::New(New);
	m->SetClassName(v8::String::NewSymbol("FaceRecognizer"));

	// Constructor
	constructor = Persistent<FunctionTemplate>::New(m);
	constructor->InstanceTemplate()->SetInternalFieldCount(1);
	constructor->SetClassName(String::NewSymbol("FaceRecognizer"));

	NODE_SET_PROTOTYPE_METHOD(constructor, "train", Train);
	NODE_SET_PROTOTYPE_METHOD(constructor, "update", Update);

	NODE_SET_PROTOTYPE_METHOD(constructor, "predict", Predict);
	NODE_SET_PROTOTYPE_METHOD(constructor, "save", Save);
	NODE_SET_PROTOTYPE_METHOD(constructor, "load", Load);

	target->Set(String::NewSymbol("FaceRecognizer"), m->GetFunction());
};

Handle<Value>
FaceRecognizer::New(const Arguments &args) {
	HandleScope scope;

	if (args.This()->InternalFieldCount() == 0)
		return v8::ThrowException(v8::Exception::TypeError(v8::String::New("Cannot instantiate without new")));

	FaceRecognizer *fr;

	fr = new FaceRecognizer();

	fr->Wrap(args.Holder());
	return scope.Close(args.Holder());
}

FaceRecognizer::FaceRecognizer(): ObjectWrap() {
	fr = cv::createFisherFaceRecognizer();
}

Handle<Value> 
FaceRecognizer::Train(const Arguments& args){
	SETUP_FUNCTION(FaceRecognizer)

	vector<cv::Mat> images;
	vector<int> labels;
	if (args.Length() != 2) {
		return scope.Close(Null());
	}
	if (args[0]->IsArray() && args[1]->IsArray()) {
		Local<Object> matrices = args[0]->ToObject();
		Local<Object> labelArr = args[1]->ToObject();
		int len = matrices->Get(v8::String::New("length"))->ToObject()->Uint32Value(); // Ugly?!
		for (int i = 0; i < len; i++) {
			Matrix *im = ObjectWrap::Unwrap<Matrix>(matrices->Get(i)->ToObject());
			images.push_back(im->mat);
		}
		len = labelArr->Get(v8::String::New("length"))->ToObject()->Uint32Value(); // Ugly again
		for (int i = 0; i < len; i++) {
			int label = labelArr->Get(i)->ToObject()->Uint32Value();
			labels.push_back(label);
		}
		self->fr->train(images, labels);
	}
	return scope.Close(Null());
}
Handle<Value> 
FaceRecognizer::Update(const Arguments& args){
	SETUP_FUNCTION(FaceRecognizer)

	vector<cv::Mat> images;
	vector<int> labels;
	if (args.Length() != 2) {
		return scope.Close(Null());
	}
	if (args[0]->IsArray() && args[1]->IsArray()) {
		Local<Object> matrices = args[0]->ToObject();
		Local<Object> labelArr = args[1]->ToObject();
		int len = matrices->Get(v8::String::New("length"))->ToObject()->Uint32Value(); // Ugly?!
		for (int i = 0; i < len; i++) {
			Matrix *im = ObjectWrap::Unwrap<Matrix>(matrices->Get(i)->ToObject());
			images.push_back(im->mat);
		}
		len = labelArr->Get(v8::String::New("length"))->ToObject()->Uint32Value(); // Ugly again
		for (int i = 0; i < len; i++) {
			int label = labelArr->Get(i)->ToObject()->Uint32Value();
			labels.push_back(label);
		}
		self->fr->update(images, labels);
	}
	return scope.Close(Null());
}
Handle<Value> 
FaceRecognizer::Predict(const Arguments& args){
	SETUP_FUNCTION(FaceRecognizer)
	Matrix *im = ObjectWrap::Unwrap<Matrix>(args[0]->ToObject());
	int label = -1;
	double confidence = 0.0;
	self->fr->predict(im->mat, label, confidence);
	printf("%d ; conf %f\n", label, confidence);
	return scope.Close(Null());
}
Handle<Value> 
FaceRecognizer::Save(const Arguments& args){
	SETUP_FUNCTION(FaceRecognizer)
	if (args[0]->IsString()) {
		std::string filename = std::string(*v8::String::AsciiValue(args[0]->ToString()));
		self->fr->save(filename);
	}
	return scope.Close(Null());
}
Handle<Value> 
FaceRecognizer::Load(const Arguments& args){
	SETUP_FUNCTION(FaceRecognizer)
	if (args[0]->IsString()) {
		std::string filename = std::string(*v8::String::AsciiValue(args[0]->ToString()));
		self->fr->load(filename);
	}
	return scope.Close(Null());
}


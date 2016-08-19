#include <node.h>
#include "Bmp.h"

namespace imager {
	using v8::Exception;
	using v8::FunctionCallbackInfo;
	using v8::Isolate;
	using v8::Value;
	using v8::Local;
	using v8::Object;
	using v8::String;

	// 转化为char *
	char *To_CharP(Local<Value> val) {
		String::Utf8Value utf8_value(val->ToString());
		std::string str = *utf8_value;
		char *ch = new char[str.length() + 1];
		std::strcpy(ch, str.c_str());
		return ch;
	}

	// 灰度化
	void grayScale(const FunctionCallbackInfo<Value>& args) {
		Isolate *isolate = args.GetIsolate();
		if (args.Length() < 1) {
			// 没有输入文件名
			isolate->ThrowException(Exception::TypeError(String::NewFromUtf8(isolate, "You have to input image path!")));
		};
		char *readPath = To_CharP(args[0]);
		char defaultWritePath[] = "_deal_.bmp";
		char *writePath;
		if (args.Length() >= 2) {
			writePath = To_CharP(args[1]);
		}
		else {
			writePath = defaultWritePath;
		}
		BMP bmp;
		bmp.readBmp(readPath);
		bmp.grayScale();
		bmp.saveBmp(writePath);
		args.GetReturnValue().Set(String::NewFromUtf8(isolate, "OK"));
	}

	// 初始化方法
	void init(Local<Object> exports) {
		NODE_SET_METHOD(exports, "grayScale", grayScale);
	}

	NODE_MODULE(imager, init);

}

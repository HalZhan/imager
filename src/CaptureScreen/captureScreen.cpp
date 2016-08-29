#include <node.h>
#include <iostream>
#include "CaptureScreen.h"

namespace imager {
	using v8::FunctionCallbackInfo;
	using v8::Isolate;
	using v8::Local;
	using v8::Object;
	using v8::String;
	using v8::Value;

	// string to char *
	char *To_CharP(Local<Value> val) {
		String::Utf8Value utf8_value(val->ToString());
		std::string str = *utf8_value;
		char *ch = new char[str.length() + 1];
		std::strcpy(ch, str.c_str());
		return ch;
	}

	void Method(const FunctionCallbackInfo<Value>& args) {
		Isolate* isolate = args.GetIsolate();
		char *savePath;
		char defaultSavePath[] = "_capture_screen.bmp";
		if (args.Length() < 1 || !args[0]->ToString()->Length()) {
			// no read-path
			savePath = defaultSavePath;
		}
		else {
			savePath = To_CharP(args[0]);
		}
		CaptureScreen(savePath);
		args.GetReturnValue().Set(String::NewFromUtf8(isolate, "OK"));
	}

	void init(Local<Object> exports) {
		NODE_SET_METHOD(exports, "captureScreen", Method);
	}

	NODE_MODULE(captureScreen, init)
}
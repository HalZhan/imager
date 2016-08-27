#include <node.h>
#include "../dib/cdib.h"
#include <iostream>

namespace imager {
	using v8::Exception;
	using v8::FunctionCallbackInfo;
	using v8::Isolate;
	using v8::Value;
	using v8::Local;
	using v8::Object;
	using v8::String;

	// string to char *
	char *To_CharP(Local<Value> val) {
		String::Utf8Value utf8_value(val->ToString());
		std::string str = *utf8_value;
		char *ch = new char[str.length() + 1];
		std::strcpy(ch, str.c_str());
		return ch;
	}

	// negative
	void negative(const FunctionCallbackInfo<Value>& args) {
		Isolate *isolate = args.GetIsolate();
		if (args.Length() < 1 || !args[0]->ToString()->Length()) {
			// no read-path
			isolate->ThrowException(Exception::TypeError(String::NewFromUtf8(isolate, "You have to input image path!")));
			args.GetReturnValue().Set(String::NewFromUtf8(isolate, "Error!"));
			return;
		}
		char *readPath = To_CharP(args[0]);
		char defaultWritePath[] = "_deal_.bmp";
		char *writePath;
		if (args.Length() >= 2 && args[1]->ToString()->Length()) {
			writePath = To_CharP(args[1]);
		}
		else {
			writePath = defaultWritePath;
		}
		CDib cdib;
		cdib.Load(readPath);
		cdib.Negative();
		cdib.Save(writePath);
		args.GetReturnValue().Set(String::NewFromUtf8(isolate, "OK"));
	}

	// initialize
	void init(Local<Object> exports) {
		NODE_SET_METHOD(exports, "negative", negative);
	}

	NODE_MODULE(negative, init);
}
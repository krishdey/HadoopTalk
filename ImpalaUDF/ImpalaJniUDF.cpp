/**
 * This Impala UDF written in c++ which in turn does JNI call to JAVA class ImpalaJniUDF.
 * @author krishdey
 */

#include <stdio.h>
#include <jni.h>
#include <stdlib.h>
#include <string.h>
#include <impala_udf/udf.h>
#include <cctype>
#include <cmath>
#include <string>
#include <iostream>

using namespace impala;
using namespace impala_udf;
using namespace std;

/// C linkage for helper functions in hdfsJniHelper.h
extern  "C" { extern JNIEnv* getJNIEnv(void); }

string invoke_method(FunctionContext* ctx, const char* text, const char* element,const char* user)
{
    //Add logging TODO 
    JNIEnv* env = getJNIEnv();
    
    jstring jstrBuf1 = (jstring)env->NewStringUTF(text); 
    jstring jstrBuf2 = (jstring)env->NewStringUTF(element); 
    jstring usrBuf3  = (jstring)env->NewStringUTF(user); 
   
    jclass impala_jni_class = (env->FindClass("com/krish/securedata/hadoop/impala/krishUtil"));
    if(impala_jni_class == NULL){
      ctx->SetError("java class is null\n");
      throw ("JNI ImpalaJni JAVA class not found");    
    } 
    
    jmethodID access_method = env->GetStaticMethodID(impala_voltage_class, "accessData",\
     "(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)Ljava/lang/String;");
    
    if(access_method == NULL){
     ctx->SetError("access method null\n");
     throw ("JNI Method not found");
    }
    
    jstring j_stack = (jstring) env->CallStaticObjectMethod(impala_voltage_class, access_method, jstrBuf1, jstrBuf2,usrBuf3); 
    jboolean is_copy;
    const char* c_stack = reinterpret_cast<const char*>((env)->GetStringUTFChars(j_stack, &is_copy)); 
    std::string output(c_stack);
    env->ReleaseStringUTFChars(j_stack,c_stack);
    //Free Local Reference 
    env->DeleteLocalRef(jstrBuf1);env->DeleteLocalRef(jstrBuf1);    
    return output;
}

StringVal accessdata(FunctionContext* ctx, const StringVal& arg1,const StringVal& arg2){
  char* arg3 = reinterpret_cast<char *>(arg1.ptr);
  char* arg4 = reinterpret_cast<char *>(arg2.ptr);
  string str = invoke_method(ctx,arg3,arg4,ctx->effective_user());
  StringVal result(ctx, str.size());
  memcpy(result.ptr, str.c_str(), str.size());
  //ctx->Free(arg1.ptr);ctx->Free(arg2.ptr);
  return result;
}

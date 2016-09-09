#include <jni.h>
#include <cstdio>

static void android_print(JNIEnv * env, jclass clazz)
{
    printf("helloworld\n");
}

static JNINativeMethod gMethods[] =
{
    {(char*)"printHello", (char*)"()V", (void*)android_print},
};

jint JNI_OnLoad(JavaVM* vm, void* reserved)
{
    JNIEnv* env = NULL;
    jint result = -1;
    if (vm->GetEnv((void**) &env, JNI_VERSION_1_4) != JNI_OK) {
        return -1;
    }
    char className[20] = {"test/HelloWorld"};
    jclass clazz = (env)->FindClass( (const char*)className);
    if((env)->RegisterNatives(clazz, gMethods, 1)< 0)
    {
        return -1;
    }

    jclass dpclazz = env->FindClass("test/HelloDDI");
    jmethodID method1 = env->GetMethodID(dpclazz,"javahello","()V");
    jobject dpobj= env->AllocObject(dpclazz);
    env->CallVoidMethod(dpobj,method1);

    result = JNI_VERSION_1_4;
    return result;
}

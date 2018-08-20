#include <jni.h>
#include <unistd.h>
#include <android/log.h>
#include "inc/fmod.hpp"

#define LOGI(FORMAT,...) __android_log_print(ANDROID_LOG_INFO,"jason",FORMAT,##__VA_ARGS__);
#define LOGE(FORMAT,...) __android_log_print(ANDROID_LOG_ERROR,"jason",FORMAT,##__VA_ARGS__);

using namespace FMOD;

#define MODE_NORMAL 0       //普通声音
#define MODE_LUOLI 1        //萝莉声音
#define MODE_DASHU 2        //大叔声音
#define MODE_JINGSONG 3     //惊悚声音
#define MODE_GAOGUAI 4      //搞怪声音
#define MODE_KONGLING 5     //空灵声音

extern "C"
JNIEXPORT jint JNICALL
Java_com_github_fmodtest_MainActivity_playSound(JNIEnv *env, jobject jobj, jstring path_,
                                                jint type) {
    System *system;
    Sound *sound;
    Channel *channel;
    DSP *dsp;
    FMOD_RESULT result;
    bool playing = true;
    float frequency = 0;

    const char *path = env->GetStringUTFChars(path_, 0);

    // TODO
    try {
        //首先创建一个system，然后初始化他
        result = System_Create(&system);
        LOGE("运行结果===%d",result);
        unsigned int        version;
        result = system->getVersion(&version);
        LOGE("运行结果===%d",result);
        result = system->init(32, FMOD_INIT_NORMAL, NULL);//32是指最多支持32个声音同时
        LOGE("运行结果===%d",result);
        //创建声音,返回sound
        result = system->createSound(path, FMOD_DEFAULT, 0, &sound);
        LOGE("运行结果===%d",result);

        switch (type) {
            case MODE_NORMAL:
                //返回一个channel频道
                system->playSound(sound,0,false,&channel);
                break;
            case MODE_LUOLI:
                //萝莉
                //DSP digital signal process
                //dsp -> 音效 创建fmod中预定义好的音效
                //FMOD_DSP_TYPE_PITCHSHIFT dsp，提升或者降低音调用的一种音效
                system->createDSPByType(FMOD_DSP_TYPE_PITCHSHIFT,&dsp);
                //设置音调的参数
                dsp->setParameterFloat(FMOD_DSP_PITCHSHIFT_PITCH,2.5);
                system->playSound(sound, 0, false, &channel);
                //添加到channel
                channel->addDSP(0,dsp);
                break;

            case MODE_JINGSONG:
                //惊悚
                system->createDSPByType(FMOD_DSP_TYPE_TREMOLO,&dsp);
                dsp->setParameterFloat(FMOD_DSP_TREMOLO_SKEW, 0.5);
                system->playSound(sound, 0, false, &channel);
                channel->addDSP(0,dsp);
                break;
            case MODE_DASHU:
                //大叔
                system->createDSPByType(FMOD_DSP_TYPE_PITCHSHIFT,&dsp);
                dsp->setParameterFloat(FMOD_DSP_PITCHSHIFT_PITCH,0.8);
                system->playSound(sound, 0, false, &channel);
                //添加到channel
                channel->addDSP(0,dsp);
                LOGI("%s","fix dashu");
                break;
            case MODE_GAOGUAI:
                //搞怪
                //提高说话的速度
                system->playSound(sound, 0, false, &channel);
                channel->getFrequency(&frequency);
                frequency = frequency * 1.6;
                channel->setFrequency(frequency);
                break;
            case MODE_KONGLING:
                //空灵
                system->createDSPByType(FMOD_DSP_TYPE_ECHO,&dsp);
                dsp->setParameterFloat(FMOD_DSP_ECHO_DELAY,300);
                dsp->setParameterFloat(FMOD_DSP_ECHO_FEEDBACK,20);
                system->playSound(sound, 0, false, &channel);
                channel->addDSP(0,dsp);
                break;
            default:
                break;
        }
    } catch (...) {
        LOGE("%s","发生异常");
        goto end;
    }
    system->update();   //必须加这句话才能播放声音

    //释放资源
    //单位是微秒,每秒钟判断下是否在播放
    while(playing){
        channel->isPlaying(&playing);
        usleep(1000 * 1000);
    }
    goto end;

end:
    if (sound != NULL) {
        sound->release();
    }
    if (system != NULL) {
        system->close();
        system->release();
    }
    env->ReleaseStringUTFChars(path_, path);
}
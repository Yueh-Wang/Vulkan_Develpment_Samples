#include <jni.h>
#include <errno.h>
#include <vulkan/vulkan.h>
#include <GLES/gl.h>
#include <EGL/egl.h>
#include <android_native_app_glue.h>
#include <stdlib.h>
#include "help.h"
#include "MyVulkanManager.h"

extern "C"
{
int xPre;
int yPre;
float xDis;
float yDis;
bool isClick= true;

//事件处理回调方法
static int32_t engine_handle_input(struct android_app* app, AInputEvent* event)
{
	//struct engine* engine = (struct engine*)app->userData;
	//如果是MOTION事件(包含触屏和轨迹球)
	if (AInputEvent_getType(event) == AINPUT_EVENT_TYPE_MOTION)
	{
		//如果是触屏
		if(AInputEvent_getSource(event)==AINPUT_SOURCE_TOUCHSCREEN)
		{
			//获取坐标
			int x=AMotionEvent_getRawX(event,0);
			int y=AMotionEvent_getRawY(event,0);
			//获取事件类型
			int32_t id = AMotionEvent_getAction(event);
			switch(id)
			{
				//触摸按下消息
				case AMOTION_EVENT_ACTION_DOWN:
					isClick=true;
					xPre=x;
					yPre=y;
					break;
					//触摸移动消息
				case AMOTION_EVENT_ACTION_MOVE:
					xDis=x-xPre;
					yDis=y-yPre;
					if(abs((int)xDis)>10||abs((int)yDis)>10)
					{
						isClick= false;
					}
					break;
					//触摸弹起消息
				case AMOTION_EVENT_ACTION_UP:
					if(isClick)
					{
						MyVulkanManager::topologyWay=(++MyVulkanManager::topologyWay%ShaderQueueSuit_Common::topologyCount);
					}
					isClick= true;
					break;
			}
		}
		return true;
	}
	return false;
}

//命令回调方法
static void engine_handle_cmd(struct android_app* app, int32_t cmd)
{
	struct engine* engine = (struct engine*)app->userData;
	switch (cmd)
	{
		case APP_CMD_SAVE_STATE:
			LOGI("APP_CMD_SAVE_STATE");		//background_second
			break;
		case APP_CMD_INIT_WINDOW:			//init_first
			MyVulkanManager::doVulkan();
			LOGI("APP_CMD_INIT_WINDOW");
			break;
		case APP_CMD_TERM_WINDOW:			//background_first
			LOGI("APP_CMD_TERM_WINDOW");
			break;
		case APP_CMD_GAINED_FOCUS:			//init_second
			LOGI("APP_CMD_GAINED_FOCUS");
			break;
		case APP_CMD_LOST_FOCUS:			//被遮挡时
			MyVulkanManager::loopDrawFlag=false;
			LOGI("APP_CMD_LOST_FOCUS");
			break;
		case APP_CMD_DESTROY:
			LOGI("APP_CMD_DESTROY");
			break;
	}
}


void android_main(struct android_app* app)
{
	//这一句必须写
	app_dummy();
	MyVulkanManager::Android_application=app;
	MyData md;
	//设置应用的用户数据对象
	app->userData = &md;
	//设置应用的命令回调方法
	app->onAppCmd = engine_handle_cmd;
	//设置应用的事件处理回调方法
	app->onInputEvent = engine_handle_input;
	//将应用指针设置给MyData
	md.app = app;
	//标志位
	bool beginFlag=false;
	while (true)
	{
		int events;
		struct android_poll_source* source;
		//ALooper_pollAll函数的第一个参数为0 表示获取不到输入事件则立即返回
		//为-1表示获取不到就等待
		//循环获取事件消息并处理掉
		while ((ALooper_pollAll((beginFlag?0:-1), NULL, &events,(void**)&source)) >= 0)
		{
			beginFlag=true;
			//处理事件
			if (source != NULL)
			{
				source->process(app, source);
			}
		}
		//做不断循环要做的工作，比如刷帧
	}
}
//extern "C"的右花括号
}

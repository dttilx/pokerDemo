/****************************************************************************
 Copyright (c) 2017-2018 Xiamen Yaji Software Co., Ltd.
 
 http://www.cocos2d-x.org
 
 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 ****************************************************************************/

#include "AppDelegate.h"
#include "HelloWorldScene.h"
#include "configs/GameConfig.h"

// #define USE_AUDIO_ENGINE 1
// #define USE_SIMPLE_AUDIO_ENGINE 1

#if USE_AUDIO_ENGINE && USE_SIMPLE_AUDIO_ENGINE
#error "Don't use AudioEngine and SimpleAudioEngine at the same time. Please just select one in your game!"
#endif

#if USE_AUDIO_ENGINE
#include "audio/include/AudioEngine.h"
using namespace cocos2d::experimental;
#elif USE_SIMPLE_AUDIO_ENGINE
#include "audio/include/SimpleAudioEngine.h"
using namespace CocosDenshion;
#endif

USING_NS_CC;

static cocos2d::Size kDesignResolutionSize =
    cocos2d::Size(static_cast<float>(GameConfig::getDesignWidth()),
                  static_cast<float>(GameConfig::getDesignHeight()));

AppDelegate::AppDelegate()
{
}

AppDelegate::~AppDelegate() 
{
#if USE_AUDIO_ENGINE
    AudioEngine::end();
#elif USE_SIMPLE_AUDIO_ENGINE
    SimpleAudioEngine::end();
#endif
}

// 如果需要自定义 OpenGL 上下文属性，请修改 glContextAttrs 的值
// 该设置会影响所有平台上的渲染配置
void AppDelegate::initGLContextAttrs()
{
    // set OpenGL context attributes: red,green,blue,alpha,depth,stencil,multisamplesCount
    GLContextAttrs glContextAttrs = {8, 8, 8, 8, 24, 8, 0};

    GLView::setGLContextAttrs(glContextAttrs);
}

// if you want to use the package manager to install more packages,  
// don't modify or remove this function
static int register_all_packages()
{
    return 0; //flag for packages manager
}

bool AppDelegate::applicationDidFinishLaunching() {
    // initialize director
    auto director = Director::getInstance();
    auto glview = director->getOpenGLView();
    if(!glview) {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32) || (CC_TARGET_PLATFORM == CC_PLATFORM_MAC) || (CC_TARGET_PLATFORM == CC_PLATFORM_LINUX)
        glview = GLViewImpl::createWithRect("myPoker", cocos2d::Rect(0, 0, kDesignResolutionSize.width, kDesignResolutionSize.height));
#else
        glview = GLViewImpl::create("myPoker");
#endif
        director->setOpenGLView(glview);
    }

    // 关闭左下角调试统计信息（GL verts / GL calls / FPS）
    director->setDisplayStats(false);

    // 设置帧率。若不调用则默认是 1.0 / 60 秒一帧
    director->setAnimationInterval(1.0f / 60);

    // 固定逻辑分辨率为 1080x2080，不随环境改变逻辑坐标尺寸
    glview->setDesignResolutionSize(kDesignResolutionSize.width, kDesignResolutionSize.height, ResolutionPolicy::EXACT_FIT);

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32) || (CC_TARGET_PLATFORM == CC_PLATFORM_MAC) || (CC_TARGET_PLATFORM == CC_PLATFORM_LINUX)
    // 桌面端物理窗口会受屏幕分辨率上限约束：
    // 这里固定逻辑分辨率为 1080x2080，并按屏幕尺寸自动缩放窗口，
    // 保证任何环境下都按同一逻辑尺寸渲染。
    auto* desktopView = dynamic_cast<GLViewImpl*>(glview);
    if (desktopView)
    {
        cocos2d::Size monitorSize = desktopView->getMonitorSize();
        float zoomX = (monitorSize.width * 0.92f) / kDesignResolutionSize.width;
        float zoomY = (monitorSize.height * 0.92f) / kDesignResolutionSize.height;
        float zoom = zoomX < zoomY ? zoomX : zoomY;
        if (zoom > 1.0f) zoom = 1.0f;
        if (zoom <= 0.0f) zoom = 1.0f;

        desktopView->setFrameZoomFactor(zoom);
        desktopView->setWindowed(static_cast<int>(kDesignResolutionSize.width),
                                 static_cast<int>(kDesignResolutionSize.height));
    }
#endif

    // 添加 Resources 目录到文件搜索路径
    FileUtils::getInstance()->addSearchPath("Resources");

    register_all_packages();

    // 创建首个场景（自动释放对象）
    auto scene = HelloWorld::createScene();

    // 运行游戏主循环
    director->runWithScene(scene);

    return true;
}

// This function will be called when the app is inactive. Note, when receiving a phone call it is invoked.
void AppDelegate::applicationDidEnterBackground() {
    Director::getInstance()->stopAnimation();

#if USE_AUDIO_ENGINE
    AudioEngine::pauseAll();
#elif USE_SIMPLE_AUDIO_ENGINE
    SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
    SimpleAudioEngine::getInstance()->pauseAllEffects();
#endif
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground() {
    Director::getInstance()->startAnimation();

#if USE_AUDIO_ENGINE
    AudioEngine::resumeAll();
#elif USE_SIMPLE_AUDIO_ENGINE
    SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
    SimpleAudioEngine::getInstance()->resumeAllEffects();
#endif
}

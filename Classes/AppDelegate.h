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

#ifndef  _APP_DELEGATE_H_
#define  _APP_DELEGATE_H_

#include "cocos2d.h"

/**
 * @brief 整个游戏应用的入口委托类。
 *
 * 负责初始化 OpenGL 环境、创建导演（Director）和首个场景，
 * 同时接收操作系统层面的前后台切换通知。
 * 通过私有继承 cocos2d::Application，隐藏掉部分不需要直接暴露给业务层的接口。
 */
class  AppDelegate : private cocos2d::Application
{
public:
    AppDelegate();
    virtual ~AppDelegate();

    virtual void initGLContextAttrs();

    /**
     * @brief 在应用启动完成后回调，在这里完成导演和首个场景的初始化。
     *
     * @return true  初始化成功，继续运行应用。
     * @return false 初始化失败，终止应用。
     */
    virtual bool applicationDidFinishLaunching();

    /**
     * @brief 当应用切到后台时被调用（例如被来电或切换到其他 App）。
     */
    virtual void applicationDidEnterBackground();

    /**
     * @brief 当应用从后台回到前台时被调用，用于恢复动画和音效。
     */
    virtual void applicationWillEnterForeground();
};

#endif // _APP_DELEGATE_H_


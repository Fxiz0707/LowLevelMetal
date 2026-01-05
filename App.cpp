#ifndef APP_H
#define APP_H

#include <AppKit/AppKit.hpp>
#include <Metal/Metal.hpp>
#include <MetalKit/MetalKit.hpp>

#include <chrono>

#pragma region Declarations {

class Renderer
{
public:
    Renderer( MTL::Device* pDevice );
    ~Renderer();
    void draw( MTK::View* pView );

private:
    MTL::Device* _pDevice;
    MTL::CommandQueue* _pCommandQueue;
};

class MyMTKViewDelegate : public MTK::ViewDelegate
{
public:
    MyMTKViewDelegate( MTL::Device* pDevice );
    virtual ~MyMTKViewDelegate() override;
    virtual void drawInMTKView( MTK::View* pView ) override;

private:
    Renderer* _pRenderer;
};

class MyAppDelegate : public NS::ApplicationDelegate
{
public:
    ~MyAppDelegate();

    NS::Menu* createMenuBar();

    virtual void applicationWillFinishLaunching( NS::Notification* pNotification ) override;
    virtual void applicationDidFinishLaunching( NS::Notification* pNotification ) override;
    virtual bool applicationShouldTerminateAfterLastWindowClosed( NS::Application* pSender ) override;

private:
    NS::Window* _pWindow;
    MTK::View* _pMtkView;
    MTL::Device* _pDevice;
    MyMTKViewDelegate* _pViewDelegate = nullptr;
};

#pragma endregion Declarations }

class App {
public:
    App() {}

    App(const App& other) = delete;
    App& operator=(const App& other) = delete;

     void run() {
        auto current_time = std::chrono::steady_clock::now();

        while (false) {
            auto new_time = std::chrono::steady_clock::now();
            auto elapsed_time = std::chrono::duration_cast<std::chrono::milliseconds>(new_time - current_time);
            update(elapsed_time);
            current_time = new_time;
        }
    }

    void update(std::chrono::milliseconds elapsed_time) {
    }

    void render() {
    }

    void user_input() {
    }
};

#endif //APP_H
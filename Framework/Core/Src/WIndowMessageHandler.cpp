#include"Precompiled.h"
#include"DebugUtil.h"
#include"WindowMessageHandler.h"

using namespace GGEngine::Core;

void WindowMessageHandler::Hook(HWND window,Callback cb)
{
    mWindow = window;
    mPreviousCallback = (Callback)GetWindowLongPtrA(window, GWLP_WNDPROC);
    SetWindowLongPtrA(window, GWLP_WNDPROC, (LONG_PTR)cb);
}

void WindowMessageHandler::UnHook()
{
    SetWindowLongPtrA(mWindow,GWLP_WNDPROC,(LONG_PTR)mPreviousCallback);
}

LRESULT WindowMessageHandler::ForwardMessage(HWND window, UINT message, WPARAM wParam, LPARAM lParam)
{
    ASSERT(mPreviousCallback, "WINDOW MESSAGE HANDLER -- NO CALLBACK IS HOOKED.");
    return CallWindowProcA((WNDPROC)mPreviousCallback, window, message, wParam, lParam);
}
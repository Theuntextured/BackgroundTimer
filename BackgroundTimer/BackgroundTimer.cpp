#include "Timer.h"
#include "KeybindManager.h"
#include "WindowManager.h"


int WinMain()
{
    Timer timer;
    KeybindManager kbManager(&timer);
    WindowManager wm(&timer, &kbManager);

    while (wm.window.isOpen())
    {        
        kbManager.tick();
        wm.Tick();
    }
    return 0;
}

//makes this work on debug and release modes.
int main() {
    return WinMain();
}
#ifndef ENGINE_H
#define ENGINE_H

#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include "SharedStructs.h"


class LayerMgr;
class ObjectMgr;
class PhysicsMgr;


class Engine
{
public:
    Engine();
    virtual ~Engine();

    virtual void InitScreen(uint32 sizex, uint32 sizey, uint8 bpp = 0, uint32 extraflags = 0);
    virtual bool Setup(void);
    virtual void Shutdown(void);

    virtual void OnMouseEvent(uint32 type, uint32 button, uint32 state, uint32 x, uint32 y, int32 rx, int32 ry);
    virtual void OnKeyDown(SDLKey key, SDLMod mod);
    virtual void OnKeyUp(SDLKey key, SDLMod mod);
    virtual void OnJoystickEvent(uint32 type, uint32 device, uint32 id, int32 val);
    virtual void OnWindowEvent(bool active);
    virtual void OnWindowResize(uint32 newx, uint32 newy);
    virtual bool OnRawEvent(SDL_Event& evt); // return true to pass this event to the following internal event handlers, false to proceed with next event


    inline uint32 GetResX(void) { return _screen->w; }
    inline uint32 GetResY(void) { return _screen->h; }
    inline uint8 GetBPP(void) { return _screen->format->BitsPerPixel; }
    inline Point GetCameraPos(void) { return _cameraPos; }
    inline SDL_Surface *GetSurface(void) { return _screen; }
    SDL_Rect *GetVisibleBlockRect(void);
    virtual void SetTitle(char *title);
    inline uint32 GetFPS(void) { return _fps; }
    inline static uint32 GetCurFrameTime(void) { return s_curFrameTime; }
    inline void SetSleepTime(uint32 t) { _sleeptime = t; }
    virtual void Run(void);

    inline LayerMgr *_GetLayerMgr(void) const { return _layermgr; }

    ObjectMgr *objmgr;
    PhysicsMgr *physmgr;

protected:

    LayerMgr *_layermgr;

    virtual void _ProcessEvents(void);
    virtual void _CalcFPS(void);
    virtual void _Render(void);
    virtual void _Process(uint32 ms);

    std::string _wintitle;
    SDL_Surface *_screen;
    uint32 _screenFlags; // stores surface flags set on screen creation
    static volatile uint32 s_curFrameTime, s_lastFrameTime;
    SDL_Rect _visibleBlockRect;
    uint32 _winsizex;
    uint32 _winsizey;
    uint32 _fps;
    uint32 _framecounter;
    uint32 _fpsclock;
    uint32 _sleeptime;
    Point _cameraPos; // camera / "screen anchor" position in 2D-space, top-left corner (starts with (0,0) )
    bool _quit;
    bool _paused;

private:
    void _InitJoystick(void); // this does nothing if joystick support was not explicitly initialized in SDL_Init()
    

};

#endif

//--------------------------------------------------------------------------------------
// File: DXUT.cpp
//
// Copyright (c) Microsoft Corporation. All rights reserved.
//--------------------------------------------------------------------------------------
#include "DXUT.h"
#define DXUT_MIN_WINDOW_SIZE_X 200
#define DXUT_MIN_WINDOW_SIZE_Y 200
#define DXUT_COUNTER_STAT_LENGTH 2048
#undef min // use __min instead inside this source file
#undef max // use __max instead inside this source file

#ifndef ARRAYSIZE
extern "C++" // templates cannot be declared to have 'C' linkage
template <typename T, size_t N>
char (*RtlpNumberOf( UNALIGNED T (&)[N] ))[N];

#define RTL_NUMBER_OF_V2(A) (sizeof(*RtlpNumberOf(A)))
#define ARRAYSIZE(A)    RTL_NUMBER_OF_V2(A)
#endif

//--------------------------------------------------------------------------------------
// Thread safety
//--------------------------------------------------------------------------------------
CRITICAL_SECTION g_cs;  
bool g_bThreadSafe = true;


//--------------------------------------------------------------------------------------
// Automatically enters & leaves the CS upon object creation/deletion
//--------------------------------------------------------------------------------------
class DXUTLock
{
public:
    inline DXUTLock()  { if( g_bThreadSafe ) EnterCriticalSection( &g_cs ); }
    inline ~DXUTLock() { if( g_bThreadSafe ) LeaveCriticalSection( &g_cs ); }
};



//--------------------------------------------------------------------------------------
// Helper macros to build member functions that access member variables with thread safety
//--------------------------------------------------------------------------------------
#define SET_ACCESSOR( x, y )       inline void Set##y( x t )   { DXUTLock l; m_state.m_##y = t; };
#define GET_ACCESSOR( x, y )       inline x Get##y()           { DXUTLock l; return m_state.m_##y; };
#define GET_SET_ACCESSOR( x, y )   SET_ACCESSOR( x, y ) GET_ACCESSOR( x, y )

#define SETP_ACCESSOR( x, y )      inline void Set##y( x* t )  { DXUTLock l; m_state.m_##y = *t; };
#define GETP_ACCESSOR( x, y )      inline x* Get##y()          { DXUTLock l; return &m_state.m_##y; };
#define GETP_SETP_ACCESSOR( x, y ) SETP_ACCESSOR( x, y ) GETP_ACCESSOR( x, y )


//--------------------------------------------------------------------------------------
// Stores timer callback info
//--------------------------------------------------------------------------------------
struct DXUT_TIMER
{
    LPDXUTCALLBACKTIMER pCallbackTimer;
    void* pCallbackUserContext;
    float fTimeoutInSecs;
    float fCountdown;
    bool  bEnabled;
    UINT  nID;
};

//--------------------------------------------------------------------------------------
// Stores DXUT state and data access is done with thread safety (if g_bThreadSafe==true)
//--------------------------------------------------------------------------------------
class DXUTState
{
protected:
    struct STATE
    {
        // D3D8 specific
        IDirect3D9*             m_D3D9;                    // the main D3D9 object
        IDirect3DDevice9*       m_D3D9Device;              // the D3D9 rendering device
        DXUTDeviceSettings      m_CurrentDeviceSettings;   // current device settings
        D3DSURFACE_DESC         m_BackBufferSurfaceDesc9;  // D3D9 back buffer surface description
        D3DCAPS9                m_Caps;                    // D3D caps for current device

        // General
        HWND  m_HWNDFocus;                  // the main app focus window
        HWND  m_HWNDDeviceFullScreen;       // the main app device window in fullscreen mode
        HWND  m_HWNDDeviceWindowed;         // the main app device window in windowed mode
        HMONITOR m_AdapterMonitor;          // the monitor of the adapter 
        HMENU m_Menu;                       // handle to menu

        UINT m_FullScreenBackBufferWidthAtModeChange;  // back buffer size of fullscreen mode right before switching to windowed mode.  Used to restore to same resolution when toggling back to fullscreen
        UINT m_FullScreenBackBufferHeightAtModeChange; // back buffer size of fullscreen mode right before switching to windowed mode.  Used to restore to same resolution when toggling back to fullscreen
        UINT m_WindowBackBufferWidthAtModeChange;  // back buffer size of windowed mode right before switching to fullscreen mode.  Used to restore to same resolution when toggling back to windowed mode
        UINT m_WindowBackBufferHeightAtModeChange; // back buffer size of windowed mode right before switching to fullscreen mode.  Used to restore to same resolution when toggling back to windowed mode
        DWORD m_WindowedStyleAtModeChange;  // window style
        WINDOWPLACEMENT m_WindowedPlacement;// record of windowed HWND position/show state/etc
        bool  m_TopmostWhileWindowed;       // if true, the windowed HWND is topmost 
        bool  m_Minimized;                  // if true, the HWND is minimized
        bool  m_Maximized;                  // if true, the HWND is maximized
        bool  m_MinimizedWhileFullscreen;   // if true, the HWND is minimized due to a focus switch away when fullscreen mode
        bool  m_IgnoreSizeChange;           // if true, DXUT won't reset the device upon HWND size change

        double m_Time;                      // current time in seconds
        double m_AbsoluteTime;              // absolute time in seconds
        float m_ElapsedTime;                // time elapsed since last frame

        HINSTANCE m_HInstance;              // handle to the app instance
        double m_LastStatsUpdateTime;       // last time the stats were updated
        DWORD m_LastStatsUpdateFrames;      // frames count since last time the stats were updated
        float m_FPS;                        // frames per second
        int   m_CurrentFrameNumber;         // the current frame number
        HHOOK m_KeyboardHook;               // handle to keyboard hook
        bool  m_AllowShortcutKeysWhenFullscreen; // if true, when fullscreen enable shortcut keys (Windows keys, StickyKeys shortcut, ToggleKeys shortcut, FilterKeys shortcut) 
        bool  m_AllowShortcutKeysWhenWindowed;   // if true, when windowed enable shortcut keys (Windows keys, StickyKeys shortcut, ToggleKeys shortcut, FilterKeys shortcut) 
        bool  m_AllowShortcutKeys;          // if true, then shortcut keys are currently disabled (Windows key, etc)
        bool  m_CallDefWindowProc;          // if true, DXUTStaticWndProc will call DefWindowProc for unhandled messages. Applications rendering to a dialog may need to set this to false.
        STICKYKEYS m_StartupStickyKeys;     // StickyKey settings upon startup so they can be restored later
        TOGGLEKEYS m_StartupToggleKeys;     // ToggleKey settings upon startup so they can be restored later
        FILTERKEYS m_StartupFilterKeys;     // FilterKey settings upon startup so they can be restored later

        bool  m_AppSupportsD3D9Override;    // true if app sets via DXUTSetD3DVersionSupport()
        bool  m_AppSupportsD3D10Override;   // true if app sets via DXUTSetD3DVersionSupport()
        bool  m_UseD3DVersionOverride;      // true if the app ever calls DXUTSetD3DVersionSupport()

        bool  m_HandleEscape;               // if true, then DXUT will handle escape to quit
        bool  m_HandleAltEnter;             // if true, then DXUT will handle alt-enter to toggle fullscreen
        bool  m_HandlePause;                // if true, then DXUT will handle pause to toggle time pausing
        bool  m_ShowMsgBoxOnError;          // if true, then msgboxes are displayed upon errors
        bool  m_NoStats;                    // if true, then DXUTGetFrameStats() and DXUTGetDeviceStats() will return blank strings
        bool  m_ClipCursorWhenFullScreen;   // if true, then DXUT will keep the cursor from going outside the window when full screen
        bool  m_ShowCursorWhenFullScreen;   // if true, then DXUT will show a cursor when full screen
        bool  m_ConstantFrameTime;          // if true, then elapsed frame time will always be 0.05f seconds which is good for debugging or automated capture
        float m_TimePerFrame;               // the constant time per frame in seconds, only valid if m_ConstantFrameTime==true
        bool  m_WireframeMode;              // if true, then D3DRS_FILLMODE==D3DFILL_WIREFRAME else D3DRS_FILLMODE==D3DFILL_SOLID 
        bool  m_AutoChangeAdapter;          // if true, then the adapter will automatically change if the window is different monitor
        bool  m_WindowCreatedWithDefaultPositions; // if true, then CW_USEDEFAULT was used and the window should be moved to the right adapter
        int   m_ExitCode;                   // the exit code to be returned to the command line

        bool  m_DXUTInited;                 // if true, then DXUTInit() has succeeded
        bool  m_WindowCreated;              // if true, then DXUTCreateWindow() or DXUTSetWindow() has succeeded
        bool  m_DeviceCreated;              // if true, then DXUTCreateDevice() or DXUTSetD3D*Device() has succeeded

        bool  m_DXUTInitCalled;             // if true, then DXUTInit() was called
        bool  m_WindowCreateCalled;         // if true, then DXUTCreateWindow() or DXUTSetWindow() was called
        bool  m_DeviceCreateCalled;         // if true, then DXUTCreateDevice() or DXUTSetD3D*Device() was called

        bool  m_DeviceObjectsCreated;       // if true, then DeviceCreated callback has been called (if non-NULL)
        bool  m_DeviceObjectsReset;         // if true, then DeviceReset callback has been called (if non-NULL)
        bool  m_InsideDeviceCallback;       // if true, then the framework is inside an app device callback
        bool  m_InsideMainloop;             // if true, then the framework is inside the main loop
        bool  m_Active;                     // if true, then the app is the active top level window
        bool  m_TimePaused;                 // if true, then time is paused
        bool  m_RenderingPaused;            // if true, then rendering is paused
        int   m_PauseRenderingCount;        // pause rendering ref count
        int   m_PauseTimeCount;             // pause time ref count
        bool  m_DeviceLost;                 // if true, then the device is lost and needs to be reset
        bool  m_NotifyOnMouseMove;          // if true, include WM_MOUSEMOVE in mousecallback
        bool  m_Automation;                 // if true, automation is enabled
        bool  m_InSizeMove;                 // if true, app is inside a WM_ENTERSIZEMOVE
        UINT  m_TimerLastID;               // last ID of the DXUT timer
        
        int   m_OverrideForceAPI;           // if != -1, then override to use this Direct3D API version
        int   m_OverrideAdapterOrdinal;     // if != -1, then override to use this adapter ordinal
        bool  m_OverrideWindowed;           // if true, then force to start windowed
        int   m_OverrideOutput;             // if != -1, then override to use the particular output on the adapter
        bool  m_OverrideFullScreen;         // if true, then force to start full screen
        int   m_OverrideStartX;             // if != -1, then override to this X position of the window
        int   m_OverrideStartY;             // if != -1, then override to this Y position of the window
        int   m_OverrideWidth;              // if != 0, then override to this width
        int   m_OverrideHeight;             // if != 0, then override to this height
        bool  m_OverrideForceHAL;           // if true, then force to HAL device (failing if one doesn't exist)
        bool  m_OverrideForceREF;           // if true, then force to REF device (failing if one doesn't exist)
        bool  m_OverrideForcePureHWVP;      // if true, then force to use pure HWVP (failing if device doesn't support it)
        bool  m_OverrideForceHWVP;          // if true, then force to use HWVP (failing if device doesn't support it)
        bool  m_OverrideForceSWVP;          // if true, then force to use SWVP 
        bool  m_OverrideConstantFrameTime;  // if true, then force to constant frame time
        float m_OverrideConstantTimePerFrame; // the constant time per frame in seconds if m_OverrideConstantFrameTime==true
        int   m_OverrideQuitAfterFrame;     // if != 0, then it will force the app to quit after that frame
        int   m_OverrideForceVsync;         // if == 0, then it will force the app to use D3DPRESENT_INTERVAL_IMMEDIATE, if == 1 force use of D3DPRESENT_INTERVAL_DEFAULT
        bool  m_OverrideRelaunchMCE;          // if true, then force relaunch of MCE at exit
        bool  m_ReleasingSwapChain;		  // if true, the app is releasing its swapchain

        LPDXUTCALLBACKMODIFYDEVICESETTINGS  m_ModifyDeviceSettingsFunc; // modify Direct3D device settings callback
        LPDXUTCALLBACKDEVICEREMOVED         m_DeviceRemovedFunc;        // Direct3D device removed callback
        LPDXUTCALLBACKFRAMEMOVE             m_FrameMoveFunc;            // frame move callback
        LPDXUTCALLBACKKEYBOARD              m_KeyboardFunc;             // keyboard callback
        LPDXUTCALLBACKMOUSE                 m_MouseFunc;                // mouse callback
        LPDXUTCALLBACKMSGPROC               m_WindowMsgFunc;            // window messages callback

        LPDXUTCALLBACKISD3D9DEVICEACCEPTABLE    m_IsD3D9DeviceAcceptableFunc;   // D3D9 is device acceptable callback
        LPDXUTCALLBACKD3D9DEVICECREATED         m_D3D9DeviceCreatedFunc;        // D3D9 device created callback
        LPDXUTCALLBACKD3D9DEVICERESET           m_D3D9DeviceResetFunc;          // D3D9 device reset callback
        LPDXUTCALLBACKD3D9DEVICELOST            m_D3D9DeviceLostFunc;           // D3D9 device lost callback
        LPDXUTCALLBACKD3D9DEVICEDESTROYED       m_D3D9DeviceDestroyedFunc;      // D3D9 device destroyed callback
        LPDXUTCALLBACKD3D9FRAMERENDER           m_D3D9FrameRenderFunc;          // D3D9 frame render callback

		void* m_ModifyDeviceSettingsFuncUserContext;     // user context for modify Direct3D device settings callback
        void* m_DeviceRemovedFuncUserContext;            // user context for Direct3D device removed callback
        void* m_FrameMoveFuncUserContext;                // user context for frame move callback
        void* m_KeyboardFuncUserContext;                 // user context for keyboard callback
        void* m_MouseFuncUserContext;                    // user context for mouse callback
        void* m_WindowMsgFuncUserContext;                // user context for window messages callback

        void* m_IsD3D9DeviceAcceptableFuncUserContext;   // user context for is D3D9 device acceptable callback
        void* m_D3D9DeviceCreatedFuncUserContext;        // user context for D3D9 device created callback
        void* m_D3D9DeviceResetFuncUserContext;          // user context for D3D9 device reset callback
        void* m_D3D9DeviceLostFuncUserContext;           // user context for D3D9 device lost callback
        void* m_D3D9DeviceDestroyedFuncUserContext;      // user context for D3D9 device destroyed callback
        void* m_D3D9FrameRenderFuncUserContext;          // user context for D3D9 frame render callback

        bool m_Keys[256];                                // array of key state
        bool m_MouseButtons[5];                          // array of mouse states

        CGrowableArray<DXUT_TIMER>*  m_TimerList;        // list of DXUT_TIMER structs
        WCHAR m_StaticFrameStats[256];                   // static part of frames stats 
        WCHAR m_FPSStats[64];                            // fps stats
        WCHAR m_FrameStats[256];                         // frame stats (fps, width, etc)
        WCHAR m_DeviceStats[256];                        // device stats (description, device type, etc)
        WCHAR m_WindowTitle[256];                        // window title
    };
    
    STATE m_state;

public:
    DXUTState()  { Create(); }
    ~DXUTState() { Destroy(); }

    void Create()
    {
        g_bThreadSafe = true; 
        InitializeCriticalSection( &g_cs ); 

        ZeroMemory( &m_state, sizeof(STATE) ); 
        m_state.m_OverrideStartX = -1; 
        m_state.m_OverrideStartY = -1; 
        m_state.m_OverrideForceAPI = -1; 
        m_state.m_OverrideAdapterOrdinal = -1; 
        m_state.m_OverrideOutput = -1;
        m_state.m_OverrideForceVsync = -1;
        m_state.m_AutoChangeAdapter = true; 
        m_state.m_ShowMsgBoxOnError = true;
        m_state.m_AllowShortcutKeysWhenWindowed = true;
        m_state.m_Active = true;
        m_state.m_CallDefWindowProc = true;
        m_state.m_HandleEscape = true;
        m_state.m_HandleAltEnter = true;
        m_state.m_HandlePause = true;
    }

    void Destroy()
    {
        SAFE_DELETE( m_state.m_TimerList );
        //DXUTShutdown();
        DeleteCriticalSection( &g_cs ); 
    }

    // Macros to define access functions for thread safe access into m_state 

    // D3D9 specific
    GET_SET_ACCESSOR( IDirect3D9*, D3D9 );
    GET_SET_ACCESSOR( IDirect3DDevice9*, D3D9Device );
    GET_SET_ACCESSOR( DXUTDeviceSettings, CurrentDeviceSettings );
    GETP_SETP_ACCESSOR( D3DSURFACE_DESC, BackBufferSurfaceDesc9 );
    GETP_SETP_ACCESSOR( D3DCAPS9, Caps );

    GET_SET_ACCESSOR( HWND, HWNDFocus );
    GET_SET_ACCESSOR( HWND, HWNDDeviceFullScreen );
    GET_SET_ACCESSOR( HWND, HWNDDeviceWindowed );
    GET_SET_ACCESSOR( HMONITOR, AdapterMonitor );
    GET_SET_ACCESSOR( HMENU, Menu );   

    GET_SET_ACCESSOR( UINT, FullScreenBackBufferWidthAtModeChange );
    GET_SET_ACCESSOR( UINT, FullScreenBackBufferHeightAtModeChange );
    GET_SET_ACCESSOR( UINT, WindowBackBufferWidthAtModeChange );
    GET_SET_ACCESSOR( UINT, WindowBackBufferHeightAtModeChange );
    GETP_SETP_ACCESSOR( WINDOWPLACEMENT, WindowedPlacement );
    GET_SET_ACCESSOR( DWORD, WindowedStyleAtModeChange );
    GET_SET_ACCESSOR( bool, TopmostWhileWindowed );
    GET_SET_ACCESSOR( bool, Minimized );
    GET_SET_ACCESSOR( bool, Maximized );
    GET_SET_ACCESSOR( bool, MinimizedWhileFullscreen );
    GET_SET_ACCESSOR( bool, IgnoreSizeChange );   

    GET_SET_ACCESSOR( double, Time );
    GET_SET_ACCESSOR( double, AbsoluteTime );
    GET_SET_ACCESSOR( float, ElapsedTime );

    GET_SET_ACCESSOR( HINSTANCE, HInstance );
    GET_SET_ACCESSOR( double, LastStatsUpdateTime );   
    GET_SET_ACCESSOR( DWORD, LastStatsUpdateFrames );   
    GET_SET_ACCESSOR( float, FPS );    
    GET_SET_ACCESSOR( int, CurrentFrameNumber );
    GET_SET_ACCESSOR( HHOOK, KeyboardHook );
    GET_SET_ACCESSOR( bool, AllowShortcutKeysWhenFullscreen );
    GET_SET_ACCESSOR( bool, AllowShortcutKeysWhenWindowed );
    GET_SET_ACCESSOR( bool, AllowShortcutKeys );
    GET_SET_ACCESSOR( bool, CallDefWindowProc );
    GET_SET_ACCESSOR( STICKYKEYS, StartupStickyKeys );
    GET_SET_ACCESSOR( TOGGLEKEYS, StartupToggleKeys );
    GET_SET_ACCESSOR( FILTERKEYS, StartupFilterKeys );

    GET_SET_ACCESSOR( bool, AppSupportsD3D9Override );
    GET_SET_ACCESSOR( bool, AppSupportsD3D10Override );
    GET_SET_ACCESSOR( bool, UseD3DVersionOverride );

    GET_SET_ACCESSOR( bool, HandleEscape );
    GET_SET_ACCESSOR( bool, HandleAltEnter );
    GET_SET_ACCESSOR( bool, HandlePause );
    GET_SET_ACCESSOR( bool, ShowMsgBoxOnError );
    GET_SET_ACCESSOR( bool, NoStats );
    GET_SET_ACCESSOR( bool, ClipCursorWhenFullScreen );   
    GET_SET_ACCESSOR( bool, ShowCursorWhenFullScreen );
    GET_SET_ACCESSOR( bool, ConstantFrameTime );
    GET_SET_ACCESSOR( float, TimePerFrame );
    GET_SET_ACCESSOR( bool, WireframeMode );   
    GET_SET_ACCESSOR( bool, AutoChangeAdapter );
    GET_SET_ACCESSOR( bool, WindowCreatedWithDefaultPositions );
    GET_SET_ACCESSOR( int, ExitCode );

    GET_SET_ACCESSOR( bool, DXUTInited );
    GET_SET_ACCESSOR( bool, WindowCreated );
    GET_SET_ACCESSOR( bool, DeviceCreated );
    GET_SET_ACCESSOR( bool, DXUTInitCalled );
    GET_SET_ACCESSOR( bool, WindowCreateCalled );
    GET_SET_ACCESSOR( bool, DeviceCreateCalled );
    GET_SET_ACCESSOR( bool, InsideDeviceCallback );
    GET_SET_ACCESSOR( bool, InsideMainloop );
    GET_SET_ACCESSOR( bool, DeviceObjectsCreated );
    GET_SET_ACCESSOR( bool, DeviceObjectsReset );
    GET_SET_ACCESSOR( bool, Active );
    GET_SET_ACCESSOR( bool, RenderingPaused );
    GET_SET_ACCESSOR( bool, TimePaused );
    GET_SET_ACCESSOR( int, PauseRenderingCount );
    GET_SET_ACCESSOR( int, PauseTimeCount );
    GET_SET_ACCESSOR( bool, DeviceLost );
    GET_SET_ACCESSOR( bool, NotifyOnMouseMove );
    GET_SET_ACCESSOR( bool, Automation );
    GET_SET_ACCESSOR( bool, InSizeMove );
    GET_SET_ACCESSOR( UINT, TimerLastID );

    GET_SET_ACCESSOR( int, OverrideForceAPI );
    GET_SET_ACCESSOR( int, OverrideAdapterOrdinal );
    GET_SET_ACCESSOR( bool, OverrideWindowed );
    GET_SET_ACCESSOR( int, OverrideOutput );
    GET_SET_ACCESSOR( bool, OverrideFullScreen );
    GET_SET_ACCESSOR( int, OverrideStartX );
    GET_SET_ACCESSOR( int, OverrideStartY );
    GET_SET_ACCESSOR( int, OverrideWidth );
    GET_SET_ACCESSOR( int, OverrideHeight );
    GET_SET_ACCESSOR( bool, OverrideForceHAL );
    GET_SET_ACCESSOR( bool, OverrideForceREF );
    GET_SET_ACCESSOR( bool, OverrideForcePureHWVP );
    GET_SET_ACCESSOR( bool, OverrideForceHWVP );
    GET_SET_ACCESSOR( bool, OverrideForceSWVP );
    GET_SET_ACCESSOR( bool, OverrideConstantFrameTime );
    GET_SET_ACCESSOR( float, OverrideConstantTimePerFrame );
    GET_SET_ACCESSOR( int, OverrideQuitAfterFrame );
    GET_SET_ACCESSOR( int, OverrideForceVsync );
    GET_SET_ACCESSOR( bool, OverrideRelaunchMCE );
    GET_SET_ACCESSOR( bool, ReleasingSwapChain );

    GET_SET_ACCESSOR( LPDXUTCALLBACKMODIFYDEVICESETTINGS, ModifyDeviceSettingsFunc );
    GET_SET_ACCESSOR( LPDXUTCALLBACKDEVICEREMOVED, DeviceRemovedFunc );
    GET_SET_ACCESSOR( LPDXUTCALLBACKFRAMEMOVE, FrameMoveFunc );
    GET_SET_ACCESSOR( LPDXUTCALLBACKKEYBOARD, KeyboardFunc );
    GET_SET_ACCESSOR( LPDXUTCALLBACKMOUSE, MouseFunc );
    GET_SET_ACCESSOR( LPDXUTCALLBACKMSGPROC, WindowMsgFunc );

    GET_SET_ACCESSOR( LPDXUTCALLBACKISD3D9DEVICEACCEPTABLE, IsD3D9DeviceAcceptableFunc );
    GET_SET_ACCESSOR( LPDXUTCALLBACKD3D9DEVICECREATED, D3D9DeviceCreatedFunc );
    GET_SET_ACCESSOR( LPDXUTCALLBACKD3D9DEVICERESET, D3D9DeviceResetFunc );
    GET_SET_ACCESSOR( LPDXUTCALLBACKD3D9DEVICELOST, D3D9DeviceLostFunc );
    GET_SET_ACCESSOR( LPDXUTCALLBACKD3D9DEVICEDESTROYED, D3D9DeviceDestroyedFunc );
    GET_SET_ACCESSOR( LPDXUTCALLBACKD3D9FRAMERENDER, D3D9FrameRenderFunc );

    GET_SET_ACCESSOR( void*, ModifyDeviceSettingsFuncUserContext );
    GET_SET_ACCESSOR( void*, DeviceRemovedFuncUserContext );
    GET_SET_ACCESSOR( void*, FrameMoveFuncUserContext );
    GET_SET_ACCESSOR( void*, KeyboardFuncUserContext );
    GET_SET_ACCESSOR( void*, MouseFuncUserContext );
    GET_SET_ACCESSOR( void*, WindowMsgFuncUserContext );

    GET_SET_ACCESSOR( void*, IsD3D9DeviceAcceptableFuncUserContext );
    GET_SET_ACCESSOR( void*, D3D9DeviceCreatedFuncUserContext );
    GET_SET_ACCESSOR( void*, D3D9DeviceResetFuncUserContext );
    GET_SET_ACCESSOR( void*, D3D9DeviceLostFuncUserContext );
    GET_SET_ACCESSOR( void*, D3D9DeviceDestroyedFuncUserContext );
    GET_SET_ACCESSOR( void*, D3D9FrameRenderFuncUserContext );

    GET_SET_ACCESSOR( CGrowableArray<DXUT_TIMER>*, TimerList );
    GET_ACCESSOR( bool*, Keys );
    GET_ACCESSOR( bool*, MouseButtons );
    GET_ACCESSOR( WCHAR*, StaticFrameStats );
    GET_ACCESSOR( WCHAR*, FPSStats );
    GET_ACCESSOR( WCHAR*, FrameStats );
    GET_ACCESSOR( WCHAR*, DeviceStats );    
    GET_ACCESSOR( WCHAR*, WindowTitle );
};


//--------------------------------------------------------------------------------------
// Global state 
//--------------------------------------------------------------------------------------
DXUTState* g_pDXUTState = NULL;

HRESULT WINAPI DXUTCreateState()
{
    if( g_pDXUTState == NULL )
    {
        g_pDXUTState = new DXUTState;
        if( NULL == g_pDXUTState ) 
            return E_OUTOFMEMORY;
    }
    return S_OK; 
}

void WINAPI DXUTDestroyState()
{
    SAFE_DELETE( g_pDXUTState );
}

class DXUTMemoryHelper
{
public:
    DXUTMemoryHelper()  { DXUTCreateState(); }
    ~DXUTMemoryHelper() { DXUTDestroyState(); }
};


DXUTState& GetDXUTState()
{
    // This class will auto create the memory when its first accessed and delete it after the program exits WinMain.
    // However the application can also call DXUTCreateState() & DXUTDestroyState() independantly if its wants 
    static DXUTMemoryHelper memory;  

    return *g_pDXUTState;
}

HRESULT DXUTChangeDevice( DXUTDeviceSettings* pNewDeviceSettings, IDirect3DDevice9* pd3d9DeviceFromApp, bool bForceRecreate, bool bClipWindowToSingleAdapter );
void    DXUTCheckForWindowSizeChange();
void    DXUTCheckForWindowChangingMonitors() {};

UINT DXUTGetBackBufferWidthFromDS( DXUTDeviceSettings* pNewDeviceSettings )     { return pNewDeviceSettings->d3d9.pp.BackBufferWidth; }
UINT DXUTGetBackBufferHeightFromDS( DXUTDeviceSettings* pNewDeviceSettings )    { return pNewDeviceSettings->d3d9.pp.BackBufferHeight; }

bool DXUTGetIsWindowedFromDS( DXUTDeviceSettings &rkNewDeviceSettings )          { return rkNewDeviceSettings.d3d9.pp.Windowed == 1; }

//--------------------------------------------------------------------------------------
// External state access functions
//--------------------------------------------------------------------------------------
//const D3DSURFACE_DESC* WINAPI DXUTGetD3D8BackBufferSurfaceDesc() { return GetDXUTState().GetBackBufferSurfaceDesc8(); }
void WINAPI DXUTSetD3D9Device(IDirect3DDevice9 *pkD3dDevice) { GetDXUTState().SetD3D9Device(pkD3dDevice); } 
IDirect3DDevice9* WINAPI DXUTGetD3D9Device()               { return GetDXUTState().GetD3D9Device(); }
HWND WINAPI DXUTGetHWND()                                  { return DXUTIsWindowed() ? GetDXUTState().GetHWNDDeviceWindowed() : GetDXUTState().GetHWNDDeviceFullScreen(); }
double WINAPI DXUTGetTime()                                { return GetDXUTState().GetTime(); }
bool WINAPI DXUTGetAutomation()                            { return GetDXUTState().GetAutomation(); }
bool WINAPI DXUTIsWindowed()                               { return DXUTGetIsWindowedFromDS( GetDXUTState().GetCurrentDeviceSettings() ); }
bool WINAPI DXUTIsActive()                                 { return GetDXUTState().GetActive(); }

void WINAPI DXUTSetCallbackD3D9DeviceCreated( LPDXUTCALLBACKD3D9DEVICECREATED pCallback, void* pUserContext )                  { GetDXUTState().SetD3D9DeviceCreatedFunc( pCallback ); GetDXUTState().SetD3D9DeviceCreatedFuncUserContext( pUserContext ); }
void WINAPI DXUTSetCallbackD3D9DeviceReset( LPDXUTCALLBACKD3D9DEVICERESET pCallback, void* pUserContext )                      { GetDXUTState().SetD3D9DeviceResetFunc( pCallback );  GetDXUTState().SetD3D9DeviceResetFuncUserContext( pUserContext ); }
void WINAPI DXUTSetCallbackD3D9DeviceLost( LPDXUTCALLBACKD3D9DEVICELOST pCallback, void* pUserContext )                        { GetDXUTState().SetD3D9DeviceLostFunc( pCallback );  GetDXUTState().SetD3D9DeviceLostFuncUserContext( pUserContext ); }
void WINAPI DXUTSetCallbackD3D9DeviceDestroyed( LPDXUTCALLBACKD3D9DEVICEDESTROYED pCallback, void* pUserContext )              { GetDXUTState().SetD3D9DeviceDestroyedFunc( pCallback );  GetDXUTState().SetD3D9DeviceDestroyedFuncUserContext( pUserContext ); }

void WINAPI DXUTGetCallbackD3D9DeviceCreated( LPDXUTCALLBACKD3D9DEVICECREATED *ppCallback, void** ppUserContext )              { *ppCallback = GetDXUTState().GetD3D9DeviceCreatedFunc(); *ppUserContext = GetDXUTState().GetD3D9DeviceCreatedFuncUserContext(); }
void WINAPI DXUTGetCallbackD3D9DeviceReset( LPDXUTCALLBACKD3D9DEVICERESET *ppCallback, void** ppUserContext )                  { *ppCallback = GetDXUTState().GetD3D9DeviceResetFunc();  *ppUserContext = GetDXUTState().GetD3D9DeviceResetFuncUserContext(); }
void WINAPI DXUTGetCallbackD3D9DeviceLost( LPDXUTCALLBACKD3D9DEVICELOST *ppCallback, void** ppUserContext )                    { *ppCallback = GetDXUTState().GetD3D9DeviceLostFunc();  *ppUserContext = GetDXUTState().GetD3D9DeviceLostFuncUserContext(); }
void WINAPI DXUTGetCallbackD3D9DeviceDestroyed( LPDXUTCALLBACKD3D9DEVICEDESTROYED *ppCallback, void** ppUserContext )          { *ppCallback = GetDXUTState().GetD3D9DeviceDestroyedFunc();  *ppUserContext = GetDXUTState().GetD3D9DeviceDestroyedFuncUserContext(); }

DXUTDeviceSettings WINAPI DXUTGetDeviceSettings()
{ 
	// Return a copy of device settings of the current device.  If no device exists yet, then
	// return a blank device settings struct
	return GetDXUTState().GetCurrentDeviceSettings();

	//DXUTDeviceSettings* pDS = GetDXUTState().GetCurrentDeviceSettings();
	//if( pDS )
	//{
	//	return *pDS;
	//}
	//else
	//{
	//	DXUTDeviceSettings ds;
	//	ZeroMemory( &ds, sizeof(DXUTDeviceSettings) );
	//	return ds;
	//}
}

//--------------------------------------------------------------------------------------
// Optionally parses the command line and sets if default hotkeys are handled
//
//       Possible command line parameters are:
//          -forceapi:#             forces app to use specified Direct3D API version (fails if the application doesn't support this API or if no device is found)
//          -adapter:#              forces app to use this adapter # (fails if the adapter doesn't exist)
//          -output:#               [D3D10 only] forces app to use a particular output on the adapter (fails if the output doesn't exist) 
//          -windowed               forces app to start windowed
//          -fullscreen             forces app to start full screen
//          -forcehal               forces app to use HAL (fails if HAL doesn't exist)
//          -forceref               forces app to use REF (fails if REF doesn't exist)
//          -forcepurehwvp          [D3D9 only] forces app to use pure HWVP (fails if device doesn't support it)
//          -forcehwvp              [D3D9 only] forces app to use HWVP (fails if device doesn't support it)
//          -forceswvp              [D3D9 only] forces app to use SWVP 
//          -forcevsync:#           if # is 0, then vsync is disabled 
//          -width:#                forces app to use # for width. for full screen, it will pick the closest possible supported mode
//          -height:#               forces app to use # for height. for full screen, it will pick the closest possible supported mode
//          -startx:#               forces app to use # for the x coord of the window position for windowed mode
//          -starty:#               forces app to use # for the y coord of the window position for windowed mode
//          -constantframetime:#    forces app to use constant frame time, where # is the time/frame in seconds
//          -quitafterframe:x       forces app to quit after # frames
//          -noerrormsgboxes        prevents the display of message boxes generated by the framework so the application can be run without user interaction
//          -nostats                prevents the display of the stats
//          -relaunchmce            re-launches the MCE UI after the app exits
//          -automation             a hint to other components that automation is active 
//--------------------------------------------------------------------------------------
HRESULT WINAPI DXUTInit( bool bParseCommandLine, bool bShowMsgBoxOnError, WCHAR* strExtraCommandLineParams, bool bThreadSafeDXUT )
{
    g_bThreadSafe = bThreadSafeDXUT;

    GetDXUTState().SetDXUTInitCalled( true );

    // Not always needed, but lets the app create GDI dialogs
    InitCommonControls();

    // Save the current sticky/toggle/filter key settings so DXUT can restore them later
    STICKYKEYS sk = {sizeof(STICKYKEYS), 0};
    SystemParametersInfo(SPI_GETSTICKYKEYS, sizeof(STICKYKEYS), &sk, 0);
    GetDXUTState().SetStartupStickyKeys( sk );

    TOGGLEKEYS tk = {sizeof(TOGGLEKEYS), 0};
    SystemParametersInfo(SPI_GETTOGGLEKEYS, sizeof(TOGGLEKEYS), &tk, 0);
    GetDXUTState().SetStartupToggleKeys( tk );

    FILTERKEYS fk = {sizeof(FILTERKEYS), 0};
    SystemParametersInfo(SPI_GETFILTERKEYS, sizeof(FILTERKEYS), &fk, 0);
    GetDXUTState().SetStartupFilterKeys( fk );

    GetDXUTState().SetShowMsgBoxOnError( bShowMsgBoxOnError );

    //if( bParseCommandLine )
    //    DXUTParseCommandLine( GetCommandLine() );
    //if( strExtraCommandLineParams )
    //    DXUTParseCommandLine( strExtraCommandLineParams );

    // Declare this process to be high DPI aware, and prevent automatic scaling 
    HINSTANCE hUser32 = LoadLibrary( L"user32.dll" );
    if( hUser32 )
    {
        typedef BOOL (WINAPI * LPSetProcessDPIAware)(void);
        LPSetProcessDPIAware pSetProcessDPIAware = (LPSetProcessDPIAware)GetProcAddress( hUser32, "SetProcessDPIAware" );
        if( pSetProcessDPIAware )
        {
            pSetProcessDPIAware();
        }
        FreeLibrary( hUser32 );
    }

    // Reset the timer
    DXUTGetGlobalTimer()->Reset();

    GetDXUTState().SetDXUTInited( true );

    return S_OK;
}

//--------------------------------------------------------------------------------------
// Handles window messages 
//--------------------------------------------------------------------------------------
LRESULT CALLBACK DXUTStaticWndProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	switch( uMsg )
	{
	case WM_SIZE:
		if( SIZE_MINIMIZED == wParam )
		{
			//DXUTPause( true, true ); // Pause while we're minimized

			GetDXUTState().SetMinimized( true );
			GetDXUTState().SetMaximized( false );
		}
		else
		{
			RECT rcCurrentClient;
			GetClientRect( DXUTGetHWND(), &rcCurrentClient );
			if( rcCurrentClient.top == 0 && rcCurrentClient.bottom == 0 )
			{
				// Rapidly clicking the task bar to minimize and restore a window
				// can cause a WM_SIZE message with SIZE_RESTORED when 
				// the window has actually become minimized due to rapid change
				// so just ignore this message
			}
			else if( SIZE_MAXIMIZED == wParam )
			{
				//if( GetDXUTState().GetMinimized() )
				//	DXUTPause( false, false ); // Unpause since we're no longer minimized
				GetDXUTState().SetMinimized( false );
				GetDXUTState().SetMaximized( true );
				DXUTCheckForWindowSizeChange();
				DXUTCheckForWindowChangingMonitors();
			}
			else if( SIZE_RESTORED == wParam )
			{      
				//DXUTCheckForDXGIFullScreenSwitch();
				if( GetDXUTState().GetMaximized() )
				{
					GetDXUTState().SetMaximized( false );
					DXUTCheckForWindowSizeChange();
					DXUTCheckForWindowChangingMonitors();
				}
				else if( GetDXUTState().GetMinimized() )
				{
					//DXUTPause( false, false ); // Unpause since we're no longer minimized
					GetDXUTState().SetMinimized( false );
					DXUTCheckForWindowSizeChange();
					DXUTCheckForWindowChangingMonitors();
				}
				else if( GetDXUTState().GetInSizeMove() )
				{
					// If we're neither maximized nor minimized, the window size 
					// is changing by the user dragging the window edges.  In this 
					// case, we don't reset the device yet -- we wait until the 
					// user stops dragging, and a WM_EXITSIZEMOVE message comes.
				}
				else
				{
					// This WM_SIZE come from resizing the window via an API like SetWindowPos() so 
					// resize and reset the device now.
					DXUTCheckForWindowSizeChange();
					DXUTCheckForWindowChangingMonitors();
				}
			}
		}
		break;

	case WM_GETMINMAXINFO:
		((MINMAXINFO*)lParam)->ptMinTrackSize.x = DXUT_MIN_WINDOW_SIZE_X;
		((MINMAXINFO*)lParam)->ptMinTrackSize.y = DXUT_MIN_WINDOW_SIZE_Y;
		break;

	case WM_ENTERSIZEMOVE:
		// Halt frame movement while the app is sizing or moving
		//DXUTPause( true, true );
		GetDXUTState().SetInSizeMove( true );
		break;

	case WM_EXITSIZEMOVE:
		//DXUTPause( false, false );
		DXUTCheckForWindowSizeChange();
		DXUTCheckForWindowChangingMonitors();
		GetDXUTState().SetInSizeMove( false );
		break;

	case WM_MOUSEMOVE:
		if( DXUTIsActive() && !DXUTIsWindowed() )
		{
			IDirect3DDevice9* pd3dDevice = DXUTGetD3D9Device();
			if( pd3dDevice )
			{
				POINT ptCursor;
				GetCursorPos( &ptCursor );
				pd3dDevice->SetCursorPosition( ptCursor.x, ptCursor.y, 0 );
			}
		}
		break;

	case WM_SETCURSOR:
		if( DXUTIsActive() && !DXUTIsWindowed() )
		{
			IDirect3DDevice9* pd3dDevice = DXUTGetD3D9Device();
			if( pd3dDevice && GetDXUTState().GetShowCursorWhenFullScreen() )
				pd3dDevice->ShowCursor( true );
		}
		break;

	case WM_ACTIVATEAPP:
		if( wParam == TRUE && !DXUTIsActive() ) // Handle only if previously not active 
		{
			GetDXUTState().SetActive( true );

			// The GetMinimizedWhileFullscreen() varible is used instead of !DXUTIsWindowed()
			// to handle the rare case toggling to windowed mode while the fullscreen application 
			// is minimized and thus making the pause count wrong
			if( GetDXUTState().GetMinimizedWhileFullscreen() ) 
			{
				GetDXUTState().SetMinimizedWhileFullscreen( false );
			}
		}
		else if( wParam == FALSE && DXUTIsActive() ) // Handle only if previously active 
		{               
			GetDXUTState().SetActive( false );

			if( !DXUTIsWindowed() )
			{
				GetDXUTState().SetMinimizedWhileFullscreen( true ); 
			}
		}
		break;
	}

	return 0;
}

void DXUTCheckForWindowSizeChange()
{
	// Skip the check for various reasons
	//if( GetDXUTState().GetIgnoreSizeChange() || !GetDXUTState().GetDeviceCreated() || ( DXUTIsCurrentDeviceD3D9() && !DXUTIsWindowed()) )
	//	return;

	DXUTDeviceSettings deviceSettings = DXUTGetDeviceSettings();
	//if( DXUTIsD3D9( &deviceSettings ) )
	//{
		RECT rcCurrentClient;
		GetClientRect( DXUTGetHWND(), &rcCurrentClient );

		if( (UINT)rcCurrentClient.right != DXUTGetBackBufferWidthFromDS( &deviceSettings ) ||
			(UINT)rcCurrentClient.bottom != DXUTGetBackBufferHeightFromDS( &deviceSettings ) )
		{
			// A new window size will require a new backbuffer size size
			// Tell DXUTChangeDevice and D3D to size according to the HWND's client rect
			deviceSettings.d3d9.pp.BackBufferWidth = 0;
			deviceSettings.d3d9.pp.BackBufferHeight = 0;

			DXUTChangeDevice( &deviceSettings, NULL, NULL, false );
		}
	//}
}

//--------------------------------------------------------------------------------------
// All device changes are sent to this function.  It looks at the current 
// device (if any) and the new device and determines the best course of action.  It 
// also remembers and restores the window state if toggling between windowed and fullscreen
// as well as sets the proper window and system state for switching to the new device.
//--------------------------------------------------------------------------------------
HRESULT DXUTChangeDevice( DXUTDeviceSettings* pNewDeviceSettings, 
						 IDirect3DDevice9* pd3d9DeviceFromApp, bool bForceRecreate, bool bClipWindowToSingleAdapter )
{
	HRESULT hr;

	if( !pNewDeviceSettings )
		return S_FALSE;

	GetDXUTState().SetCurrentDeviceSettings( *pNewDeviceSettings );

	IDirect3DDevice9* pd3dDevice = DXUTGetD3D9Device();
	assert( pd3dDevice != NULL );

	// Call the app's device lost callback
	LPDXUTCALLBACKD3D9DEVICELOST pCallbackDeviceLost = GetDXUTState().GetD3D9DeviceLostFunc();
	if( pCallbackDeviceLost != NULL )
		pCallbackDeviceLost( GetDXUTState().GetD3D9DeviceLostFuncUserContext() );

	// Reset the device
	DXUTDeviceSettings kDeviceSettings = GetDXUTState().GetCurrentDeviceSettings();
	hr = pd3dDevice->Reset( &(kDeviceSettings.d3d9.pp) );
	if( FAILED(hr) )  
	{
		return hr;
		//if( hr == D3DERR_DEVICELOST )
		//	return D3DERR_DEVICELOST; // Reset could legitimately fail if the device is lost
		//else
		//	return DXUT_ERR( L"Reset", DXUTERR_RESETTINGDEVICE );
	}

	//// Setup cursor based on current settings (window/fullscreen mode, show cursor state, clip cursor state)
	//DXUTSetupCursor();

	D3DSURFACE_DESC kBBufferSurfaceDesc;
	IDirect3DSurface9* pBackBuffer;
	hr = GetDXUTState().GetD3D9Device()->GetBackBuffer( 0, 0, D3DBACKBUFFER_TYPE_MONO, &pBackBuffer );
	ZeroMemory( &kBBufferSurfaceDesc, sizeof(D3DSURFACE_DESC) );
	if( SUCCEEDED(hr) )
	{
		pBackBuffer->GetDesc( &kBBufferSurfaceDesc );
		SAFE_RELEASE( pBackBuffer );
	}

	// Call the app's OnDeviceReset callback
	LPDXUTCALLBACKD3D9DEVICERESET pCallbackDeviceReset = GetDXUTState().GetD3D9DeviceResetFunc();
	if( pCallbackDeviceReset != NULL )
		hr = pCallbackDeviceReset( pd3dDevice, &kBBufferSurfaceDesc, GetDXUTState().GetD3D9DeviceResetFuncUserContext() );

	return hr;
}

void WINAPI DXUTSetHotkeyHandling( bool bAltEnterToToggleFullscreen, bool bEscapeToQuit, bool bPauseToToggleTimePause )
{
	GetDXUTState().SetHandleEscape( bEscapeToQuit );
	GetDXUTState().SetHandleAltEnter( bAltEnterToToggleFullscreen );
	GetDXUTState().SetHandlePause( bPauseToToggleTimePause );   
}

void WINAPI DXUTSetDeviceSettings(DXUTDeviceSettings kDeviceSettings)
{
	GetDXUTState().SetCurrentDeviceSettings(kDeviceSettings);
}

void WINAPI DXUTNotifyWinCreated(HINSTANCE hInstance, HWND hWnd)
{
	HINSTANCE hInst = hInstance;
	if(hInst == NULL)
		hInst = GetModuleHandle(NULL);

	GetDXUTState().SetHInstance( hInst );
	GetDXUTState().SetWindowCreated( true );
	GetDXUTState().SetHWNDFocus( hWnd );
	GetDXUTState().SetHWNDDeviceFullScreen( hWnd );
	GetDXUTState().SetHWNDDeviceWindowed( hWnd );
}

void WINAPI DXUTNotifyWinClosed()
{
	GetDXUTState().SetHWNDFocus( NULL );
	GetDXUTState().SetHWNDDeviceFullScreen( NULL );
	GetDXUTState().SetHWNDDeviceWindowed( NULL );
}

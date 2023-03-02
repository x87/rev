#include "StdInc.h"

#include <Windows.h>
#include <Dbt.h>

#include "win.h"
#include "dshow.h"

#include "VideoPlayer.h"
#include "Gamma.h"

// #include "InputEvents.h"
#include "platform.h"

#include "PostEffects.h"
#include "Clouds.h"
#include "Skidmarks.h"
#include "LoadingScreen.h"
#include "VideoMode.h"
#include "ControllerConfigManager.h"
#include "Input.h"
#include "MenuManager.h"
#include <AEAudioHardware.h>

extern void WinPsInjectHooks();

static LPSTR AppClassName = LPSTR(APP_CLASS);

constexpr auto NO_FOREGROUND_PAUSE = true;

// forward declarations
bool IsAlreadyRunning();
char** CommandLineToArgv(char* cmdLine, int* argCount);
LRESULT CALLBACK __MainWndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
INT WINAPI __WinMain(HINSTANCE instance, HINSTANCE hPrevInstance, LPSTR cmdLine, INT nCmdShow);

// @notsa
// @brief Resets the screen gamma if ever changed.
void ResetGammaWhenExiting() {
    if (gbGammaChanged) {
        if (auto d3dDevice = (IDirect3DDevice9*)RwD3D9GetCurrentD3DDevice()) {
            d3dDevice->SetGammaRamp(0u, D3DSGR_CALIBRATE, &savedGamma);
        }
        gbGammaChanged = false;
    }
}

// 0x746870
void MessageLoop() {
    tagMSG msg;
    while (PeekMessageA(&msg, nullptr, 0, 0, PM_REMOVE | PM_NOYIELD)) {
        if (msg.message == WM_QUIT) {
            RsGlobal.quit = true;
        } else {
            TranslateMessage(&msg);
            DispatchMessageA(&msg);
        }
    }
}

// 0x7486A0
bool InitApplication(HINSTANCE hInstance) {
    WNDCLASS windowClass      = { 0 };
    windowClass.style         = CS_BYTEALIGNWINDOW;
    windowClass.lpfnWndProc   = __MainWndProc;
    windowClass.hInstance     = hInstance;
    windowClass.hIcon         = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MAIN_ICON));
    windowClass.hCursor       = LoadCursor(nullptr, IDC_ARROW);
    windowClass.lpszClassName = AppClassName;
    return RegisterClass(&windowClass);
}

// 0x745560
HWND InitInstance(HINSTANCE hInstance) {
    RECT rect = { 0, 0, RsGlobal.maximumWidth, RsGlobal.maximumHeight };
    AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, false);

    return CreateWindowEx(
        0,
        APP_CLASS,
        RsGlobal.appName,
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        rect.right - rect.left,
        rect.bottom - rect.top,
        nullptr,
        nullptr,
        hInstance,
        nullptr
    );
}

// 0x7468E0
bool IsAlreadyRunning() {
    CreateEventA(nullptr, false, true, AppClassName);
    if (GetLastError() != ERROR_ALREADY_EXISTS) {
        return false;
    }

    HWND window = FindWindowA(AppClassName, RsGlobal.appName);
    if (window)
        SetForegroundWindow(window);
    else
        SetForegroundWindow(PSGLOBAL(window));

    return true;
};

// 0x746060
bool IsForegroundApp() {
    return ForegroundApp;
}

// 0x746480
char** CommandLineToArgv(char* cmdLine, int* argCount) {
    return plugin::CallAndReturn<char**, 0x746480, char*, int*>(cmdLine, argCount);
}

// 0x747820
BOOL GTATranslateKey(RsKeyCodes* ck, LPARAM lParam, UINT vk) {
    *ck = [&] {
        // Handle extended keys
        const auto Ext = [kf = HIWORD(lParam)](RsKeyCodes extended, RsKeyCodes unextended) {
            return (kf & KF_EXTENDED) ? extended : unextended;
        };

        switch (vk) {
        case VK_RETURN:     return Ext(rsPADENTER, rsENTER);
        case VK_CONTROL:    return Ext(rsRCTRL, rsLCTRL);
        case VK_MENU:       return Ext(rsRALT, rsLALT);
        case VK_PRIOR:      return Ext(rsPGUP, rsPADPGUP);
        case VK_NEXT:       return Ext(rsPGDN, rsPADPGDN);
        case VK_END:        return Ext(rsEND, rsPADEND);
        case VK_HOME:       return Ext(rsHOME, rsPADHOME);
        case VK_LEFT:       return Ext(rsLEFT, rsPADLEFT);
        case VK_UP:         return Ext(rsUP, rsPADUP);
        case VK_RIGHT:      return Ext(rsRIGHT, rsPADRIGHT);
        case VK_DOWN:       return Ext(rsDOWN, rsPADDOWN);
        case VK_INSERT:     return Ext(rsINS, rsPADINS);
        case VK_DELETE:     return Ext(rsDEL, rsPADDEL);
        case VK_BACK:       return rsBACKSP;
        case VK_TAB:        return rsTAB;
        case VK_PAUSE:      return rsPAUSE;
        case VK_CAPITAL:    return rsCAPSLK;
        case VK_ESCAPE:     return rsESC;
        case VK_LWIN:       return rsLWIN;
        case VK_RWIN:       return rsRWIN;
        case VK_APPS:       return rsAPPS;
        case VK_NUMPAD0:    return rsPADINS;
        case VK_NUMPAD1:    return rsPADEND;
        case VK_NUMPAD2:    return rsPADDOWN;
        case VK_NUMPAD3:    return rsPADPGDN;
        case VK_NUMPAD4:    return rsPADLEFT;
        case VK_NUMPAD5:    return rsPAD5;
        case VK_NUMPAD6:    return rsPADRIGHT;
        case VK_NUMPAD7:    return rsPADHOME;
        case VK_NUMPAD8:    return rsPADUP;
        case VK_NUMPAD9:    return rsPADPGUP;
        case VK_MULTIPLY:   return rsTIMES;
        case VK_ADD:        return rsPLUS;
        case VK_SUBTRACT:   return rsMINUS;
        case VK_DECIMAL:    return rsPADDEL;
        case VK_DIVIDE:     return rsDIVIDE;
        case VK_F1:         return rsF1;
        case VK_F2:         return rsF2;
        case VK_F3:         return rsF3;
        case VK_F4:         return rsF4;
        case VK_F5:         return rsF5;
        case VK_F6:         return rsF6;
        case VK_F7:         return rsF7;
        case VK_F8:         return rsF8;
        case VK_F9:         return rsF9;
        case VK_F10:        return rsF10;
        case VK_F11:        return rsF11;
        case VK_F12:        return rsF12;
        case VK_NUMLOCK:    return rsNUMLOCK;
        case VK_SCROLL:     return rsSCROLL;
        case VK_SHIFT: {
            return s_OSStatus.OSVer == WinVer::WIN_98 // Will be handled later
                ? rsSHIFT
                : rsNULL;
        }
        default: { // Try mapping to regular ASCII char
            const auto chr = MapVirtualKey(vk, MAPVK_VK_TO_CHAR);
            if (chr <= 0xFF) {
                return (RsKeyCodes)(chr);
            }
            break;
        }
        }
        return rsNULL;
    }();
    return *ck != rsNULL;
}

/*!
* Process shift keys.
* Unless Win98, in which case `GTATranslateKey` should handle it.
* @addr 0x747CD0
*/
BOOL GTATranslateShiftKey(RsKeyCodes*) { // The in keycode is ignored, so we won't bother
    if (s_OSStatus.OSVer == WinVer::WIN_98) {
        return false; // Already handled by `GTATranslateKey`
    }

    constexpr struct { RsKeyCodes ck; INT vk; } Keys[]{
        {rsLSHIFT, VK_LSHIFT},
        {rsRSHIFT, VK_RSHIFT},
    };

    for (auto shouldBeDown : { false, true }) {
        for (auto [ck, vk] : Keys) {
            // GetKeyState reads from the message queue,
            // so we must call it like the og code
            const auto isDown = (HIWORD(GetKeyState(vk)) & 0x80) == 1; // Check is key pressed
            if (isDown == shouldBeDown) {
                RsEventHandler(
                    isDown ? rsKEYDOWN : rsKEYUP,
                    &ck
                );
            }
        }
    }

    return true;
}

// 0x747EB0
LRESULT CALLBACK __MainWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
    case WM_SETCURSOR: {
        ShowCursor(false);
        SetCursor(NULL);
        break;
    }
    case WM_KEYDOWN:
    case WM_SYSKEYDOWN: 
    case WM_KEYUP:
    case WM_SYSKEYUP: { //< 0x74823B - wParam is a `VK_` (virtual key), lParam are the flags (See https://learn.microsoft.com/en-us/windows/win32/inputdev/wm-keyup)
        if (RsKeyCodes ck; GTATranslateKey(&ck, lParam, wParam)) {
            RsKeyboardEventHandler(
                (uMsg == WM_KEYDOWN || uMsg == WM_SYSKEYDOWN)
                    ? rsKEYDOWN
                    : rsKEYUP,
                &ck
            );
        }
        if (wParam == VK_SHIFT) {
            RsKeyCodes ck;
            GTATranslateShiftKey(&ck); // Original code uses this variable for storage, so we can't pass in nullptr - TODO: Remove parameter
        }
        return 0;
    }
    case WM_ACTIVATEAPP: { // 0x748087
        const auto wndBeingActivated = !!wParam;

        //> 0x748087 - Set gamma (If changed)
        if (gbGammaChanged) {
            if (const auto dev = RwD3D9GetCurrentD3DDevice()) {
                dev->SetGammaRamp(9, 0, wndBeingActivated ? &gammaTable : &savedGamma);
            }
        }

        //
        switch (gGameState) {
        case GAME_STATE_PLAYING_LOGO:
        case GAME_STATE_PLAYING_INTRO: {
            const auto mc = VideoPlayer::GetMediaControl();

            OAFilterState state;
            VERIFY(SUCCEEDED(mc->GetState(10, &state)));
            if (wndBeingActivated) {
                VideoPlayer::UpdateWindow();
                if (state == State_Running) {
                    break;
                }
                VERIFY(SUCCEEDED(mc->Run()));
                VideoPlayer::WaitState(State_Running);
                SetFocus(PSGLOBAL(window));
            } else if (state == State_Running) { // Window is being deactivatd, pause media
                VERIFY(SUCCEEDED(mc->Pause()));
            }

            break;
        }
        case GAME_STATE_INITIAL: {
            if (!wndBeingActivated) { // 0x748171
                if (PSGLOBAL(fullScreen)) {
                    Windowed = true; // ???
                }
            }
            break;
        }
        }

        //> 0x748183 Clear pads
        for (auto& pad : CPad::Pads) {
            pad.Clear(false, true);
        }
        
        return 0;
    }
    case WA_CLICKACTIVE:
    case WM_CLOSE: { // 0x747EF3
        VERIFY(ClipCursor(nullptr));
        VERIFY(SUCCEEDED(WinInput::Shutdown()));
        PostQuitMessage(0);
        return 0;
    }
    case WM_SIZE: { // 0x747F04
        // Figure out new size
        const auto width = LOWORD(lParam), height = HIWORD(lParam);
        RwRect wndsz{
            .x = 0,     .y = 0,
            .w = width, .h = height, 
        };

        // Nothing to do?
        if (!RwInitialized || !wndsz.h || !wndsz.w) {
            return 0;
        }

        // Try resizing the camera
        RsEventHandler(rsCAMERASIZE, &wndsz);

        // Check if it has failed
        if (wndsz.w != width && wndsz.h != height) { // TODO/BUG: Shouldnt `&&` instead be `||`?
            VERIFY(ReleaseCapture());

            WINDOWPLACEMENT wndpl;
            VERIFY(GetWindowPlacement(PSGLOBAL(window), &wndpl));

            if (wndpl.showCmd == SW_SHOWMAXIMIZED) {
                SendMessage(PSGLOBAL(window), WM_WINDOWPOSCHANGED, 0, 0);
            }
        }

        return 0;
    }
    case WM_ACTIVATE: { // 0x747FA3
        const auto wndBeingActivated = !!wParam;

        if (wndBeingActivated) {
            CAudioEngine::ResumeAllSounds();
            ShowCursor(FALSE);
        } else {
            CAudioEngine::PauseAllSounds();
            VERIFY(SUCCEEDED(SetTimer(PSGLOBAL(window), 1, (UINT)(1000.f / RsGlobal.frameLimit), nullptr)));
        }

        ForegroundApp = wndBeingActivated;
        RsEventHandler(rsACTIVATE, (void*)wndBeingActivated);

        //> 0x74800C Clear pads
        for (auto& pad : CPad::Pads) {
            pad.Clear(false, true);
        }

#ifdef FIX_BUGS
        break;
#else
        return 0;
#endif
    }
    case WM_SETFOCUS: { // 0x748063
        ForegroundApp = true;
        if (!FrontEndMenuManager.m_bMainMenuSwitch && !FrontEndMenuManager.m_bMenuActive) { // OnSetFocus
            FrontEndMenuManager.m_bActivateMenuNextFrame = true;
        }
#ifdef FIX_BUGS
        break;
#else
        return 0;
#endif
    }
    case WM_KILLFOCUS: { // 0x748054
        ForegroundApp = false;
#ifdef FIX_BUGS
        break;
#else
        return 0;
#endif
    }
    case WM_GRAPHNOTIFY: { //< 0x74842A - Dispatched from VideoPlayer::Play
        switch (gGameState) {
        case GAME_STATE_PLAYING_INTRO:
        case GAME_STATE_PLAYING_LOGO: {
            VideoPlayer::OnGraphNotify();
            break;
        }
        }
#ifdef FIX_BUGS
        break;
#else
        return 0;
#endif
    }
    case WM_DEVICECHANGE: { //> 0x748282 - Handle AudioHardware DVD removal
        if (wParam != DBT_DEVICEREMOVECOMPLETE) {
            break;
        }
        const auto eparams = reinterpret_cast<PDEV_BROADCAST_HDR>(lParam);
        if (eparams->dbch_devicetype != DBT_DEVTYP_VOLUME) {
            break;
        }
        const auto idev = reinterpret_cast<PDEV_BROADCAST_VOLUME>(lParam);
        if ((idev->dbcv_flags & DBTF_MEDIA) == 0) { // Not a media drive?
            break;
        }
        if (!AEAudioHardware.m_bInitialised || !AEAudioHardware.IsStreamingFromDVD()) {
            break;
        }
        const auto dvletter = AEAudioHardware.GetDVDDriveLetter();
        if (dvletter < 'A' || (idev->dbcv_unitmask & (1 << dvletter)) == 0) {
            break;
        }
        DEV_LOG("About to check CD drive");
        CTimer::SetCodePause(true);
        if (CCutsceneMgr::IsRunning()) {
            CCutsceneMgr::SkipCutscene();
        }
        while (!AEAudioHardware.CheckDVD()) {
            FrontEndMenuManager.NoDiskInDriveMessage();
            if (FrontEndMenuManager.m_bQuitGameNoDVD) {
                DEV_LOG("Exiting game as Audio CD was not inserted");
                break;
            }
        }
        DEV_LOG("GTA Audio DVD has been inserted");
        CTimer::SetCodePause(false);
        break;
    }
    case WM_MOUSEFIRST: { //< 0x748323
        FrontEndMenuManager.m_nMousePosWinX = GET_X_LPARAM(lParam);
        FrontEndMenuManager.m_nMousePosWinY = GET_Y_LPARAM(lParam);
        return 0;
    }
    case WM_LBUTTONDOWN:
    case WM_RBUTTONDOWN:
    case WM_MBUTTONDOWN: {
        SetCapture(hWnd);
        return 0;
    }
    case WM_LBUTTONUP:
    case WM_RBUTTONUP:
    case WM_MBUTTONUP: {
        ReleaseCapture();
        return 0;
    }
    case WM_MOUSEWHEEL: {
        return 0;
    }
    case WM_SIZING: { // 0x74829E
        if (RwInitialized) {
            if (gGameState == GAME_STATE_IDLE) {
                RsEventHandler(rsIDLE, (void*)TRUE);
            }
        }

        const auto wndrect = reinterpret_cast<PRECT>(lParam);
        VERIFY(SetWindowPos(
            hWnd,
            HWND_TOP,
            0,                              0,
            wndrect->right - wndrect->left, wndrect->bottom - wndrect->top,
            SWP_NOSIZE | SWP_NOMOVE
        ));

        CPostEffects::SetupBackBufferVertex();

        return 0;
    }
    }
    return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

// Code from winmain, 0x748DCF
bool ProcessGameLogic(INT nCmdShow, MSG& Msg) {
    if (RsGlobal.quit || FrontEndMenuManager.m_bStartGameLoading) {
        return false;
    }

    // Process Window messages
    if (PeekMessage(&Msg, nullptr, 0u, 0u, PM_REMOVE | PM_NOYIELD)) {
        if (Msg.message == WM_QUIT) {
            return false;
        }
        TranslateMessage(&Msg);
        DispatchMessage(&Msg);
        return true;
    }

    // Game is in background
    if (!NO_FOREGROUND_PAUSE && !ForegroundApp) {
        if (isForeground) {
            isForeground = false;
        }
        Sleep(100);
        return true;
    }

    // TODO: Move this out from here (It's not platform specific at all)
    switch (gGameState) {
    case GAME_STATE_INITIAL: {
        const auto ProcessSplash = [](bool isNVidia) {
            CLoadingScreen::LoadSplashes(true, isNVidia);
            CLoadingScreen::Init(true, true);
            CLoadingScreen::DoPCTitleFadeOut();
            CLoadingScreen::DoPCTitleFadeIn();
            CLoadingScreen::Shutdown();
        };
        if (!FastLoadSettings.NoEAX) {
            ProcessSplash(false);
        }
        if (!FastLoadSettings.NoNVidia) {
            ProcessSplash(true);
        }
        ChangeGameStateTo(GAME_STATE_LOGO);
        break;
    }
    case GAME_STATE_LOGO: {
        if (!FastLoadSettings.NoLogo) {
            if (!Windowed) {
                VideoPlayer::Play(nCmdShow, "movies\\Logo.mpg");
            }
        }
        ChangeGameStateTo(FastLoadSettings.NoLogo ? GAME_STATE_TITLE : GAME_STATE_PLAYING_LOGO);
        break;
    }
    case GAME_STATE_PLAYING_LOGO:
    case GAME_STATE_PLAYING_INTRO: { // 0x748B17
        CPad::UpdatePads();
        auto* pad = CPad::GetPad();
        if (   Windowed
            || ControlsManager.GetJoyButtonJustDown()
            || pad->NewState.CheckForInput()
            || CPad::IsMouseLButtonPressed()
            || CPad::IsEnterJustPressed()
            || pad->IsStandardKeyJustPressed(VK_SPACE)
            || CPad::IsMenuKeyJustPressed()
            || CPad::IsTabJustPressed()
        ) {
            ChangeGameStateTo([] {
                switch (gGameState) {
                case GAME_STATE_PLAYING_LOGO:  return GAME_STATE_TITLE;
                case GAME_STATE_PLAYING_INTRO: return GAME_STATE_FRONTEND_LOADING;
                default:                       NOTSA_UNREACHABLE();
                }
            }());
        }
        break;
    }
    case GAME_STATE_TITLE: {
        if (!FastLoadSettings.NoTitleOrIntro) {
            VideoPlayer::Shutdown();
            VideoPlayer::Play(nCmdShow, FrontEndMenuManager.GetMovieFileName());
        }
        ChangeGameStateTo(FastLoadSettings.NoTitleOrIntro ? GAME_STATE_FRONTEND_LOADING : GAME_STATE_PLAYING_INTRO);
        break;
    }
    case GAME_STATE_FRONTEND_LOADING: {
        VideoPlayer::Shutdown();
        CLoadingScreen::Init(true, false);
        if (!FastLoadSettings.NoCopyright) {
            CLoadingScreen::DoPCTitleFadeOut();
        }
        if (!CGame::InitialiseEssentialsAfterRW()) {
            RsGlobal.quit = true;
        }
        CGame::InitialiseCoreDataAfterRW();
        ChangeGameStateTo(GAME_STATE_FRONTEND_LOADED);
        anisotropySupportedByGFX = (RwD3D9GetCaps()->RasterCaps & D3DPRASTERCAPS_ANISOTROPY) != 0; // todo: func
        break;
    }
    case GAME_STATE_FRONTEND_LOADED: {
        FrontEndMenuManager.m_bActivateMenuNextFrame = true;
        FrontEndMenuManager.m_bMainMenuSwitch = true;
        if (IsVMNotSelected) {
            FrontEndMenuManager.m_nPrefsVideoMode = FrontEndMenuManager.m_nDisplayVideoMode = gCurrentVideoMode;
            IsVMNotSelected = false;
        }
        ChangeGameStateTo(GAME_STATE_FRONTEND_IDLE);
        if (FastLoadSettings.NoCopyright) {
            CLoadingScreen::SkipCopyrightSplash();
        } else {
            CLoadingScreen::DoPCTitleFadeIn();
        }
        break;
    }
    case GAME_STATE_FRONTEND_IDLE: { // 0x748CB2
        WINDOWPLACEMENT wndpl{ .length = sizeof(WINDOWPLACEMENT) };
        VERIFY(GetWindowPlacement(PSGLOBAL(window), &wndpl));
        if (FastLoadSettings.ShouldLoadSaveGame()) {
            RsEventHandler(rsFRONTENDIDLE, nullptr); // We need to still run the frontend processing once because it has some important stuff
            if ((GetAsyncKeyState(FastLoadSettings.SkipSaveGameLoadKey) & 0xF000) == 0) {
                FastLoadSettings.StartGame(FastLoadSettings.SaveGameToLoad); // Load game
            }
            FastLoadSettings.TriedLoadingSaveGame = true;
        } else if (FastLoadSettings.RenderAtAllTimes || wndpl.showCmd != SW_SHOWMINIMIZED) {
            RsEventHandler(rsFRONTENDIDLE, nullptr);
        }
        if (FrontEndMenuManager.m_bMenuActive && !FrontEndMenuManager.m_bLoadingData) {
            break;
        }
        ChangeGameStateTo(GAME_STATE_LOADING_STARTED);
        if (!FrontEndMenuManager.m_bLoadingData) {
            break;
        }
        NOTSA_SWCFALLTHRU; // Fall down and start loading
    }
    case GAME_STATE_LOADING_STARTED: {
        if (!FastLoadSettings.NoLoadingTune) {
            AudioEngine.StartLoadingTune();
        }

        InitialiseGame();
        ChangeGameStateTo(GAME_STATE_IDLE);
        FrontEndMenuManager.m_bMainMenuSwitch = false;

        AudioEngine.InitialisePostLoading();
        break;
    }
    case GAME_STATE_IDLE: {
        if (!RwInitialized)
            break;

        auto v9_1 = 1000.0f / (float)RsGlobal.frameLimit;
        auto v9_2 = (float)CTimer::GetCurrentTimeInCycles() / (float)CTimer::GetCyclesPerMillisecond();
        if (!FrontEndMenuManager.m_bPrefsFrameLimiter && CReplay::Mode != eReplayMode::MODE_PLAYBACK && !AudioEngine.IsBeatInfoPresent() || v9_1 < v9_2) {
            RsEventHandler(rsIDLE, (void*)true);
        }
        break;
    }
    }

    if (!isForeground) {
        isForeground = true;
    }

    return true;
}

// Code from winmain, 0x7489FB
void MainLoop(INT nCmdShow, MSG& Msg) {
    bool bNewGameFirstTime = false;
    while (true) {
        RwInitialized = true;

        RwV2d pos{ SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f };
        RsMouseSetPos(&pos);

        gamma.Init();

        // Game logic main loop
        while (ProcessGameLogic(nCmdShow, Msg));

        // 0x748DDA
        RwInitialized = false;
        FrontEndMenuManager.UnloadTextures();
        if (!FrontEndMenuManager.m_bStartGameLoading) {
            break;
        }

        // load game
        CCheat::ResetCheats();
        CTimer::Stop();

        if (FrontEndMenuManager.m_bLoadingData) {
            CGame::ShutDownForRestart();
            CGame::InitialiseWhenRestarting();
            FrontEndMenuManager.m_bLoadingData = false;
        } else if (bNewGameFirstTime) {
            CTimer::Stop();
            ChangeGameStateTo(
                FrontEndMenuManager.m_nGameState != 1
                    ? GAME_STATE_LOADING_STARTED
                    : GAME_STATE_FRONTEND_LOADED
            );
        } else {
            CCheat::ResetCheats();
            CGame::ShutDownForRestart();
            CTimer::Stop();
            CGame::InitialiseWhenRestarting();
        }

        bNewGameFirstTime = false;
        FrontEndMenuManager.m_nGameState = 0;
        FrontEndMenuManager.m_bStartGameLoading = false;
    }

}

// 0x748710
INT WINAPI __WinMain(HINSTANCE instance, HINSTANCE hPrevInstance, LPSTR cmdLine, INT nCmdShow) {
    SystemParametersInfo(SPI_SETFOREGROUNDLOCKTIMEOUT, 0u, nullptr, 2);
    if (IsAlreadyRunning()) {
        return false;
    }

    auto initializeEvent = RsEventHandler(rsINITIALIZE, nullptr);
    if (rsEVENTERROR == initializeEvent) {
        return false;
    }

    if (!InitApplication(instance)) {
        return false;
    }

    cmdLine = GetCommandLine();
    int argc;
    char** argv = CommandLineToArgv(cmdLine, &argc);
    for (int i = 0; i < argc; i++) {
        RsEventHandler(rsPREINITCOMMANDLINE, argv[i]);
    }

    PSGLOBAL(window) = InitInstance(instance);
    if (!PSGLOBAL(window)) {
        return false;
    }
    PSGLOBAL(instance) = instance;

    // 0x7487CF
    VERIFY(WinInput::Initialise());

    ControlsManager.InitDefaultControlConfigMouse(WinInput::GetMouseState(), !FrontEndMenuManager.m_nController);

    // 0x748847
    if (RsEventHandler(rsRWINITIALIZE, PSGLOBAL(window)) == rsEVENTERROR) {
        DestroyWindow(PSGLOBAL(window));
        RsEventHandler(rsTERMINATE, nullptr);
        return false;
    }

    // 0x7488EE
    for (auto i = 0; i < argc; i++) {
        RsEventHandler(rsCOMMANDLINE, argv[i]);
    }

    if (MultipleSubSystems || PSGLOBAL(fullScreen)) {
        SetWindowLongPtr(PSGLOBAL(window), GWL_STYLE, (LONG_PTR)WS_POPUP);
        SetWindowPos(PSGLOBAL(window), nullptr, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE | SWP_NOZORDER | SWP_FRAMECHANGED);
    }

    RwRect rect{ 0, 0, RsGlobal.maximumWidth, RsGlobal.maximumHeight };
    RsEventHandler(rsCAMERASIZE, &rect);

    SystemParametersInfo(SPI_SETSCREENSAVEACTIVE, 0u, nullptr, 2u);
    SystemParametersInfo(SPI_SETPOWEROFFACTIVE, 0u, nullptr, 2u);
    SystemParametersInfo(SPI_SETLOWPOWERACTIVE, 0u, nullptr, 2u);
    STICKYKEYS pvParam { .cbSize = sizeof(STICKYKEYS) };
    SystemParametersInfo(SPI_GETSTICKYKEYS, sizeof(STICKYKEYS), &pvParam, 2u);
    STICKYKEYS pvParam1 = { .cbSize = sizeof(STICKYKEYS), .dwFlags = SKF_TWOKEYSOFF };
    SystemParametersInfo(SPI_SETSTICKYKEYS, sizeof(STICKYKEYS), &pvParam1, 2u);

    ShowWindow(PSGLOBAL(window), nCmdShow);
    UpdateWindow(PSGLOBAL(window));

    // 0x748995
    CFileMgr::SetDirMyDocuments();
    if (auto* file = CFileMgr::OpenFile("gta_sa.set", "rb")) {
        if (!ControlsManager.LoadSettings(file)) {
            ControlsManager.ReinitControls();
        }
        CFileMgr::CloseFile(file);
    }
    CFileMgr::SetDir("");

    SetErrorMode(SEM_FAILCRITICALERRORS);

    // 0x7489FB
    MSG Msg;
    MainLoop(nCmdShow, Msg);

    // if game is loaded, shut it down
    if (gGameState == GAME_STATE_IDLE) {
        CGame::Shutdown();
    }

    // now quit 0x748E75
    AudioEngine.Shutdown();
    FreeVideoModeList();
    RsEventHandler(rsRWTERMINATE, nullptr);
    DestroyWindow(PSGLOBAL(window));
    RsEventHandler(rsTERMINATE, nullptr);
    free(argv);
    ShowCursor(true);

    SystemParametersInfo(SPI_SETSTICKYKEYS, sizeof(STICKYKEYS), &pvParam, 2u);
    SystemParametersInfo(SPI_SETPOWEROFFACTIVE, 1u, nullptr, 2u); // TODO: GUID_VIDEO_POWERDOWN_TIMEOUT
    SystemParametersInfo(SPI_SETLOWPOWERACTIVE, 1u, nullptr, 2u);
    SystemParametersInfo(SPI_SETSCREENSAVEACTIVE, 1u, nullptr, 2u);
    // nullsub_0x72F3C0()
    SetErrorMode(0);

    return Msg.wParam;
}

void Win32InjectHooks() {
    RH_ScopedCategory("Win");
    RH_ScopedNamespaceName("Win");

    RH_ScopedGlobalInstall(IsForegroundApp, 0x746060);
    RH_ScopedGlobalInstall(IsAlreadyRunning, 0x7468E0);
    RH_ScopedGlobalInstall(CommandLineToArgv, 0x746480, {.reversed = false});
    
    RH_ScopedGlobalInstall(GTATranslateShiftKey, 0x747CD0);
    RH_ScopedGlobalInstall(GTATranslateKey, 0x747820);
    RH_ScopedGlobalInstall(__MainWndProc, 0x747EB0);

    // Unhooking these 2 after the game has started will do nothing
    RH_ScopedGlobalInstall(__WinMain, 0x748710);
    RH_ScopedGlobalInstall(InitInstance, 0x745560);
    
    WinPsInjectHooks();
    WinInput::InjectHooks();
}

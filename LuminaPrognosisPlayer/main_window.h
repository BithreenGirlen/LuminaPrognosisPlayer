#ifndef MAIN_WINDOW_H_
#define MAIN_WINDOW_H_

#include <Windows.h>

#include <string>
#include <vector>
#include <unordered_map>

#include "d2_image_drawer.h"
#include "d2_text_writer.h"
#include "mf_media_player.h"
#include "view_manager.h"
#include "adv.h"
#include "lumina_image_transferor.h"

class CMainWindow
{
public:
	CMainWindow();
	~CMainWindow();
	bool Create(HINSTANCE hInstance);
	int MessageLoop();
	HWND GetHwnd()const { return m_hWnd;}
private:
	const wchar_t* m_swzClassName = L"Lumina player window";
	std::wstring m_wstrWindowName = L"Lumina player";
	HINSTANCE m_hInstance = nullptr;
	HWND m_hWnd = nullptr;

	static LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	LRESULT HandleMessage(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	LRESULT OnCreate(HWND hWnd);
	LRESULT OnDestroy();
	LRESULT OnClose();
	LRESULT OnPaint();
	LRESULT OnSize();
	LRESULT OnKeyUp(WPARAM wParam, LPARAM lParam);
	LRESULT OnCommand(WPARAM wParam, LPARAM lParam);
	LRESULT OnTimer(WPARAM wParam);
	LRESULT OnMouseWheel(WPARAM wParam, LPARAM lParam);
	LRESULT OnLButtonDown(WPARAM wParam, LPARAM lParam);
	LRESULT OnLButtonUp(WPARAM wParam, LPARAM lParam);
	LRESULT OnMButtonUp(WPARAM wParam, LPARAM lParam);

	enum Menu
	{
		kOpenFile = 1, 
		kAudioLoop, kAudioSetting,
		kPauseImage
	};
	enum MenuBar
	{
		kFolder, kAudio, kImage
	};
	enum EventMessage
	{
		kAudioPlayer = WM_USER + 1,
	};
	enum Timer
	{
		kText = 1,
	};

	POINT m_CursorPos{};
	bool m_bLeftDowned = false;
	bool m_bLeftCombinated = false;

	HMENU m_hMenuBar = nullptr;
	bool m_bBarHidden = false;
	bool m_bPlayReady = false;
	bool m_bTextHidden = false;

	std::vector<std::wstring> m_scriptFilePaths;
	size_t m_nScriptFilePathIndex = 0;

	void InitialiseMenuBar();

	void MenuOnOpenFile();
	void MenuOnNextFile();
	void MenuOnForeFile();

	void MenuOnAudioLoop();
	void MenuOnAudioSetting();

	void MenuOnPauseImage();

	void ChangeWindowTitle(const wchar_t* pzTitle);
	void SwitchWindowMode();

	bool SetupScenario(const wchar_t* pwzFolderPath);
	void ClearScenarioInfo();

	void UpdateScreen();

	CD2ImageDrawer* m_pD2ImageDrawer = nullptr;
	CD2TextWriter* m_pD2TextWriter = nullptr;
	CMfMediaPlayer* m_pAudioPlayer = nullptr;
	CViewManager* m_pViewManager = nullptr;
	CLuminaImageTransferor* m_pLuminaImageTransferor = nullptr;

	std::vector<adv::TextDatum> m_textData;
	size_t m_nTextIndex = 0;

	void ShiftPaintData(bool bForward);
	void UpdatePaintData();
	void ShiftText(bool bForward);
	void UpdateText();
	void AutoTexting();
	std::wstring FormatCurrentText();

	void OnAudioPlayerEvent(unsigned long ulEvent);
};

#endif //MAIN_WINDOW_H_
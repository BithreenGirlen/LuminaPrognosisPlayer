#ifndef LUMINA_IMAGE_TRANSFEROR_H_
#define LUMINA_IMAGE_TRANSFEROR_H_

#include <Windows.h>
#include <d2d1.h>
#include <d2d1_1.h>
#include <dxgi1_2.h>
#include <atlbase.h>

#include <vector>

#include "adv.h"
#include "win_timer.h"

class CLuminaImageTransferor
{
public:
	CLuminaImageTransferor(ID2D1DeviceContext* pD2d1DeviceContext, HWND hWnd);
	~CLuminaImageTransferor();

	bool SetImages(std::vector<adv::ImageFileDatum>& imageFileData);
	void GetImageSize(unsigned int* uiWidth, unsigned int* uiHeight);

	void ShiftImage();
	ID2D1Bitmap* GetCurrentImage();

	bool SwitchPause();
	void RescaleTimer(bool bFaster);
	void ResetSpeed();
private:
	ID2D1DeviceContext* m_pStoredD2d1DeviceContext = nullptr;
	HWND m_hRenderWindow = nullptr;

	std::vector<std::vector<CComPtr<ID2D1Bitmap>>> m_images;
	size_t m_nImageIndex = 0;
	size_t m_nAnimationIndex = 0;

	bool m_bPaused = false;

	void ClearImages();

	void ShiftAnimation();

	CWinTimer m_winTimer;

	static void TimerCallback(void *pData);
};

#endif // !LUMINA_IMAGE_TRANSFEROR_H_

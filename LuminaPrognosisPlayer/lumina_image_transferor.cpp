

#include "lumina_image_transferor.h"

#include "win_filesystem.h"
#include "win_image.h"

CLuminaImageTransferor::CLuminaImageTransferor(ID2D1DeviceContext* pD2d1DeviceContext, HWND hWnd)
	: m_pStoredD2d1DeviceContext(pD2d1DeviceContext), m_hRnderWindow(hWnd)
{
	m_winTimer.SetCallback(&CLuminaImageTransferor::TimerCallback, this);
}

CLuminaImageTransferor::~CLuminaImageTransferor()
{
	m_winTimer.End();
}
/*画像流し込み*/
bool CLuminaImageTransferor::SetImages(std::vector<adv::ImageFileDatum>& imageFileData)
{
	if (m_pStoredD2d1DeviceContext == nullptr)return false;

	ClearImages();

	/*静止画のみの場合はタイマを作成しない*/
	const auto HasAnimation = [&imageFileData]()
		-> bool
		{
			for (const auto& imageFileDatum : imageFileData)
			{
				if (imageFileDatum.bAnimation)return true;
			}
			return false;
		};

	bool bAnimationContained = HasAnimation();

	/*横幅1280のものと1920のものが混在しているので、1920の場合は予め縮小しておく。*/
	const auto GetStillScale = [&imageFileData]()
		-> float
		{
			if (imageFileData.empty())return 1.f;

			unsigned int uiWidth = 0;
			unsigned int uiHeight = 0;
			bool bRet = win_image::SkimImageSize(imageFileData[0].wstrFilePath.c_str(), &uiWidth, &uiHeight);

			return uiWidth == 1920 ? 1/1.5f : 1.f;
		};

	float fStillScale = GetStillScale();

	for (const auto& imageFileDatum : imageFileData)
	{
		std::vector<CComPtr<ID2D1Bitmap>> bitmaps;

		const auto ImportImage = [this, &bitmaps](const SImageFrame& s)
			-> void
			{
				CComPtr<ID2D1Bitmap> pD2d1Bitmap;

				HRESULT hr = m_pStoredD2d1DeviceContext->CreateBitmap(D2D1::SizeU(s.uiWidth, s.uiHeight),
					D2D1::BitmapProperties(D2D1::PixelFormat(DXGI_FORMAT_B8G8R8A8_UNORM, D2D1_ALPHA_MODE_IGNORE)),
					&pD2d1Bitmap);

				D2D1_RECT_U rc = { 0, 0, s.uiWidth, s.uiHeight };
				hr = pD2d1Bitmap->CopyFromMemory(&rc, s.pixels.data(), s.iStride);
				if (SUCCEEDED(hr))
				{
					bitmaps.push_back(std::move(pD2d1Bitmap));
				}
			};

		if (imageFileDatum.bAnimation)
		{
			std::vector<std::wstring> wstrFilePaths;
			win_filesystem::CreateFilePathList(imageFileDatum.wstrFilePath.c_str(), L".jpg", wstrFilePaths);

			for (const auto& filePath : wstrFilePaths)
			{
				SImageFrame s{};
				bool bRet = win_image::LoadImageToMemory(filePath.c_str(), &s);
				if (bRet)
				{
					ImportImage(s);
				}
			}
		}
		else
		{
			SImageFrame s{};
			bool bRet = win_image::LoadImageToMemory(imageFileDatum.wstrFilePath.c_str(), &s, fStillScale);
			if (bRet)
			{
				ImportImage(s);
			}
		}

		if (!bitmaps.empty())
		{
			m_images.push_back(std::move(bitmaps));
		}
	}

	if (bAnimationContained && !m_images.empty())
	{
		m_winTimer.Start();
	}
	else
	{
		m_winTimer.End();
	}

	return !m_images.empty();
}
/*画像寸法取得*/
void CLuminaImageTransferor::GetImageSize(unsigned int* uiWidth, unsigned int* uiHeight)
{
	if (m_nImageIndex < m_images.size() || m_nAnimationIndex < m_images[m_nImageIndex].size())
	{
		D2D1_SIZE_U s = m_images[m_nImageIndex][m_nAnimationIndex]->GetPixelSize();
		*uiWidth = s.width;
		*uiHeight = s.height;
	}
}
/*画像移行*/
void CLuminaImageTransferor::ShiftImage()
{
	if (m_nImageIndex >= m_images.size() || m_nAnimationIndex >= m_images[m_nImageIndex].size())
	{
		return;
	}

	if (m_bPaused)
	{
		ShiftAnimation();
	}
	else
	{
		m_nAnimationIndex = 0;
		if (++m_nImageIndex >= m_images.size())m_nImageIndex = 0;
	}
}
/*現在の画像受け渡し*/
ID2D1Bitmap* CLuminaImageTransferor::GetCurrentImage()
{
	if (m_nImageIndex >= m_images.size() || m_nAnimationIndex >= m_images[m_nImageIndex].size())
	{
		return nullptr;
	}

	ID2D1Bitmap* p = m_images[m_nImageIndex][m_nAnimationIndex];
	if (!m_bPaused)
	{
		ShiftAnimation();
	}

	return p;
}
/*停止切り替え*/
bool CLuminaImageTransferor::SwitchPause()
{
	m_bPaused ^= true;
	return m_bPaused;
}
/*加速・減速*/
void CLuminaImageTransferor::RescaleTimer(bool bFaster)
{
	long long llInterval = m_winTimer.GetInterval();
	if (bFaster)
	{
		if (--llInterval <= 1)llInterval = 1;
	}
	else
	{
		++llInterval;
	}
	m_winTimer.SetInterval(llInterval);
}
/*速度初期化*/
void CLuminaImageTransferor::ResetSpeed()
{
	m_winTimer.ResetInterval();
}
/*消去*/
void CLuminaImageTransferor::ClearImages()
{
	m_images.clear();
	m_nImageIndex = 0;
	m_nAnimationIndex = 0;

	ResetSpeed();
}
/*コマ送り*/
void CLuminaImageTransferor::ShiftAnimation()
{
	if (m_nImageIndex >= m_images.size() || m_nAnimationIndex >= m_images[m_nImageIndex].size())
	{
		return;
	}

	if (++m_nAnimationIndex >= m_images[m_nImageIndex].size())
	{
		m_nAnimationIndex = 0;
	}
}
/*再描画要求*/
void CLuminaImageTransferor::TimerCallback(void* pData)
{
	auto pThis = static_cast<CLuminaImageTransferor*>(pData);
	if (pThis != nullptr)
	{
		HWND hWnd = pThis->m_hRnderWindow;
		if (hWnd != nullptr && ::IsWindow(hWnd))
		{
			::InvalidateRect(hWnd, nullptr, FALSE);
		}
	}
}

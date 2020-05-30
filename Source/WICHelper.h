/*
* (C) 2020 see Authors.txt
*
* This file is part of MPC-BE.
*
* MPC-BE is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation; either version 3 of the License, or
* (at your option) any later version.
*
* MPC-BE is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program.  If not, see <http://www.gnu.org/licenses/>.
*
*/

#pragma once

enum ColorSystem_t {
	CS_YUV,
	CS_RGB,
	CS_GRAY,
	CS_IDX
};

static const wchar_t* ContainerFormat2Str(const GUID guid)
{
	const wchar_t* pStr;

	if (guid == GUID_ContainerFormatBmp)       { pStr = L"BMP"; }
	else if (guid == GUID_ContainerFormatPng)  { pStr = L"PNG"; }
	else if (guid == GUID_ContainerFormatIco)  { pStr = L"ICO"; }
	else if (guid == GUID_ContainerFormatJpeg) { pStr = L"JPEG"; }
	else if (guid == GUID_ContainerFormatTiff) { pStr = L"TIFF"; }
	else if (guid == GUID_ContainerFormatGif)  { pStr = L"GIF"; }
	else if (guid == GUID_ContainerFormatWmp)  { pStr = L"HD Photo/JPEG XR"; }
	else if (guid == GUID_ContainerFormatDds)  { pStr = L"DDS"; }
//	else if (guid == GUID_ContainerFormatAdng) { pStr = L"DNG"; }
//	else if (guid == GUID_ContainerFormatHeif) { pStr = L"HEIF"; }
//	else if (guid == GUID_ContainerFormatWebp) { pStr = L"WebP"; }
//	else if (guid == GUID_ContainerFormatRaw)  { pStr = L"RAW"; }
	else { pStr = L"Unknown"; }

	return pStr;
}

struct PixelFormatDesc {
	WICPixelFormatGUID wicpfguid = GUID_NULL;
	const wchar_t*     str       = nullptr;
	int                cdepth    = 8;
	int                depth     = 32;
	ColorSystem_t      cstype    = CS_RGB;
	bool               alpha     = true;
};

static const PixelFormatDesc s_UnknownPixelFormatDesc = {};

static const PixelFormatDesc s_PixelFormatDescs[] = {
	{ GUID_WICPixelFormat1bppIndexed, L"1bppIndexed",  8,  1, CS_IDX,  true  },
	{ GUID_WICPixelFormat2bppIndexed, L"2bppIndexed",  8,  2, CS_IDX,  true  },
	{ GUID_WICPixelFormat4bppIndexed, L"4bppIndexed",  8,  4, CS_IDX,  true  },
	{ GUID_WICPixelFormat8bppIndexed, L"8bppIndexed",  8,  8, CS_IDX,  true  },
	{ GUID_WICPixelFormatBlackWhite , L"BlackWhite",   1,  1, CS_GRAY, false },
	{ GUID_WICPixelFormat2bppGray,    L"2bppGray",     2,  2, CS_GRAY, false },
	{ GUID_WICPixelFormat4bppGray,    L"4bppGray",     4,  4, CS_GRAY, false },
	{ GUID_WICPixelFormat8bppGray,    L"8bppGray",     8,  8, CS_GRAY, false },
	{ GUID_WICPixelFormat16bppBGR555, L"16bppBGR555",  5, 16, CS_RGB,  false },
	{ GUID_WICPixelFormat16bppBGR565, L"16bppBGR565",  6, 16, CS_RGB,  false },
	{ GUID_WICPixelFormat16bppGray,   L"16bppGray",   16, 16, CS_GRAY, false },
	{ GUID_WICPixelFormat24bppBGR,    L"24bppBGR",     8, 24, CS_RGB,  false },
	{ GUID_WICPixelFormat24bppRGB,    L"24bppRGB",     8, 24, CS_RGB,  false },
	{ GUID_WICPixelFormat32bppBGR,    L"32bppBGR",     8, 32, CS_RGB,  false },
	{ GUID_WICPixelFormat32bppBGRA,   L"32bppBGRA",    8, 32, CS_RGB,  true  },
	{ GUID_WICPixelFormat32bppPBGRA,  L"32bppPBGRA",   8, 32, CS_RGB,  true  },
	{ GUID_WICPixelFormat32bppRGB,    L"32bppRGB",     8, 32, CS_RGB,  false },
	{ GUID_WICPixelFormat32bppRGBA,   L"32bppRGBA",    8, 32, CS_RGB,  true  },
	{ GUID_WICPixelFormat32bppPRGBA,  L"32bppPRGBA",   8, 32, CS_RGB,  true  },
	{ GUID_WICPixelFormat48bppRGB,    L"48bppRGB",    16, 48, CS_RGB,  false },
	{ GUID_WICPixelFormat48bppBGR,    L"48bppBGR",    16, 48, CS_RGB,  false },
	{ GUID_WICPixelFormat64bppRGB,    L"64bppRGB",    16, 64, CS_RGB,  false },
	{ GUID_WICPixelFormat64bppRGBA,   L"64bppRGBA",   16, 64, CS_RGB,  true  },
	{ GUID_WICPixelFormat64bppBGRA,   L"64bppBGRA",   16, 64, CS_RGB,  true  },
	{ GUID_WICPixelFormat64bppPRGBA,  L"64bppPRGBA",  16, 64, CS_RGB,  true  },
	{ GUID_WICPixelFormat64bppPBGRA,  L"64bppPBGRA",  16, 64, CS_RGB,  true  },
};

static const PixelFormatDesc* GetPixelFormatDesc(const WICPixelFormatGUID guid)
{
	for (const auto& pfd : s_PixelFormatDescs) {
		if (pfd.wicpfguid == guid) {
			return &pfd;
		}
	}
	return &s_UnknownPixelFormatDesc;
}

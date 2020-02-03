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

#include "stdafx.h"
#include <memory>
#include "../Include/Version.h"
#include "Helper.h"

// missing GUIDs for Win8.1 SDK
//DEFINE_GUID(GUID_ContainerFormatAdng, 0xf3ff6d0d, 0x38c0, 0x41c4, 0xb1, 0xfe, 0x1f, 0x38, 0x24, 0xf1, 0x7b, 0x84);
//DEFINE_GUID(GUID_ContainerFormatHeif, 0xe1e62521, 0x6787, 0x405b, 0xa3, 0x39, 0x50, 0x07, 0x15, 0xb5, 0x76, 0x3f);
//DEFINE_GUID(GUID_ContainerFormatWebp, 0xe094b0e2, 0x67f2, 0x45b3, 0xb0, 0xea, 0x11, 0x53, 0x37, 0xca, 0x7c, 0xf3);
//DEFINE_GUID(GUID_ContainerFormatRaw, 0xfe99ce60, 0xf19c, 0x433c, 0xa3, 0xae, 0x00, 0xac, 0xef, 0xa9, 0xca, 0x21);

#ifndef _WIN32_WINNT_WINTHRESHOLD
#define _WIN32_WINNT_WINTHRESHOLD 0x0A00
VERSIONHELPERAPI IsWindows10OrGreater()
{
	return IsWindowsVersionOrGreater(HIBYTE(_WIN32_WINNT_WINTHRESHOLD), LOBYTE(_WIN32_WINNT_WINTHRESHOLD), 0);
}
#endif

LPCWSTR GetWindowsVersion()
{
	if (IsWindows10OrGreater()) {
		return L"10";
	}
	else if (IsWindows8Point1OrGreater()) {
		return L"8.1";
	}
	else if (IsWindows8OrGreater()) {
		return L"8";
	}
	else if (IsWindows7SP1OrGreater()) {
		return L"7 SP1";
	}
	else if (IsWindows7OrGreater()) {
		return L"7";
	}
	return L"Vista or older";
}

CStringW GetVersionStr()
{
	CStringW version;
#if MPCIS_RELEASE
	version.Format(L"v%S", MPCIS_VERSION_STR);
#else
	version.Format(L"v%S (git-%s-%s)",
		MPCIS_VERSION_STR,
		_CRT_WIDE(_CRT_STRINGIZE(MPCIS_REV_DATE)),
		_CRT_WIDE(_CRT_STRINGIZE(MPCIS_REV_HASH))
	);
#endif
#ifdef _WIN64
	version.Append(L" x64");
#endif
#ifdef _DEBUG
	version.Append(L" DEBUG");
#endif
	return version;
}

LPCWSTR GetNameAndVersion()
{
	static CStringW version = L"MPC Image Source " + GetVersionStr();

	return (LPCWSTR)version;
}

CStringW HR2Str(const HRESULT hr)
{
	CStringW str;
#define UNPACK_VALUE(VALUE) case VALUE: str = L#VALUE; break;
#define UNPACK_HR_WIN32(VALUE) case (((VALUE) & 0x0000FFFF) | (FACILITY_WIN32 << 16) | 0x80000000): str = L#VALUE; break;
	switch (hr) {
		// common HRESULT values https://docs.microsoft.com/en-us/windows/desktop/seccrypto/common-hresult-values
		UNPACK_VALUE(S_OK);
		UNPACK_VALUE(S_FALSE);
		UNPACK_VALUE(E_NOTIMPL);
		UNPACK_VALUE(E_NOINTERFACE);
		UNPACK_VALUE(E_POINTER);
		UNPACK_VALUE(E_ABORT);
		UNPACK_VALUE(E_FAIL);
		UNPACK_VALUE(E_UNEXPECTED);
		UNPACK_VALUE(E_ACCESSDENIED);
		UNPACK_VALUE(E_HANDLE);
		UNPACK_VALUE(E_OUTOFMEMORY);
		UNPACK_VALUE(E_INVALIDARG);
		// some System Error Codes
		UNPACK_HR_WIN32(ERROR_INVALID_WINDOW_HANDLE);
	default:
		str.Format(L"0x%08x", hr);
	};
#undef UNPACK_VALUE
#undef UNPACK_HR_WIN32
	return str;
}

const char* ContainerFormat2Str(const GUID guid)
{
	const char* pStr;

	if (guid == GUID_ContainerFormatBmp)       { pStr = "BMP"; }
	else if (guid == GUID_ContainerFormatPng)  { pStr = "PNG"; }
	else if (guid == GUID_ContainerFormatIco)  { pStr = "ICO"; }
	else if (guid == GUID_ContainerFormatJpeg) { pStr = "JPEG"; }
	else if (guid == GUID_ContainerFormatTiff) { pStr = "TIFF"; }
	else if (guid == GUID_ContainerFormatGif)  { pStr = "GIF"; }
	else if (guid == GUID_ContainerFormatWmp)  { pStr = "HD Photo/JPEG XR"; }
	else if (guid == GUID_ContainerFormatDds)  { pStr = "DDS"; }
//	else if (guid == GUID_ContainerFormatAdng) { pStr = "DNG"; }
//	else if (guid == GUID_ContainerFormatHeif) { pStr = "HEIF"; }
//	else if (guid == GUID_ContainerFormatWebp) { pStr = "WebP"; }
//	else if (guid == GUID_ContainerFormatRaw)  { pStr = "RAW"; }
	else { pStr = "Unknown"; }

	return pStr;
}

static const PixelFormatDesc s_UnknownPixelFormatDesc = { GUID_NULL, "Unknown", 0, CS_RGB, false };

static const PixelFormatDesc s_PixelFormatDescs[] = {
	{ GUID_WICPixelFormat1bppIndexed, "1bppIndexed",  8, CS_IDX,  false },
	{ GUID_WICPixelFormat2bppIndexed, "2bppIndexed",  8, CS_IDX,  false },
	{ GUID_WICPixelFormat4bppIndexed, "4bppIndexed",  8, CS_IDX,  false },
	{ GUID_WICPixelFormat8bppIndexed, "8bppIndexed",  8, CS_IDX,  false },
	{ GUID_WICPixelFormatBlackWhite , "BlackWhite",   1, CS_GRAY, false },
	{ GUID_WICPixelFormat2bppGray,    "2bppGray",     2, CS_GRAY, false },
	{ GUID_WICPixelFormat4bppGray,    "4bppGray",     4, CS_GRAY, false },
	{ GUID_WICPixelFormat8bppGray,    "8bppGray",     8, CS_GRAY, false },
	{ GUID_WICPixelFormat16bppBGR555, "16bppBGR555",  5, CS_RGB,  false },
	{ GUID_WICPixelFormat16bppBGR565, "16bppBGR565",  6, CS_RGB,  false },
	{ GUID_WICPixelFormat16bppGray,   "16bppGray",   16, CS_GRAY, false },
	{ GUID_WICPixelFormat24bppBGR,    "24bppBGR",     8, CS_RGB,  false },
	{ GUID_WICPixelFormat24bppRGB,    "24bppRGB",     8, CS_RGB,  false },
	{ GUID_WICPixelFormat32bppBGR,    "32bppBGR",     8, CS_RGB,  false },
	{ GUID_WICPixelFormat32bppBGRA,   "32bppBGRA",    8, CS_RGB,  true  },
	{ GUID_WICPixelFormat32bppPBGRA,  "32bppPBGRA",   8, CS_RGB,  true  },
	{ GUID_WICPixelFormat32bppRGB,    "32bppRGB",     8, CS_RGB,  false },
	{ GUID_WICPixelFormat32bppRGBA,   "32bppRGBA",    8, CS_RGB,  true  },
	{ GUID_WICPixelFormat32bppPRGBA,  "32bppPRGBA",   8, CS_RGB,  true  },
	{ GUID_WICPixelFormat48bppRGB,    "48bppRGB",    16, CS_RGB,  false },
	{ GUID_WICPixelFormat48bppBGR,    "48bppBGR",    16, CS_RGB,  false },
	{ GUID_WICPixelFormat64bppRGB,    "64bppRGB",    16, CS_RGB,  false },
	{ GUID_WICPixelFormat64bppRGBA,   "64bppRGBA",   16, CS_RGB,  true  },
	{ GUID_WICPixelFormat64bppBGRA,   "64bppBGRA",   16, CS_RGB,  true  },
	{ GUID_WICPixelFormat64bppPRGBA,  "64bppPRGBA",  16, CS_RGB,  true  },
	{ GUID_WICPixelFormat64bppPBGRA,  "64bppPBGRA",  16, CS_RGB,  true  },
};

const PixelFormatDesc* GetPixelFormatDesc(const GUID guid)
{
	for (const auto& pfd : s_PixelFormatDescs) {
		if (pfd.wicguid == guid) {
			return &pfd;
		}
	}
	return &s_UnknownPixelFormatDesc;
}

void CopyFrameAsIs(const UINT height, BYTE* dst, UINT dst_pitch, const BYTE* src, int src_pitch)
{
	if (dst_pitch == src_pitch) {
		memcpy(dst, src, dst_pitch * height);
		return;
	}

	const UINT linesize = std::min((UINT)abs(src_pitch), dst_pitch);

	for (UINT y = 0; y < height; ++y) {
		memcpy(dst, src, linesize);
		src += src_pitch;
		dst += dst_pitch;
	}
}

HRESULT GetDataFromResource(LPVOID& data, DWORD& size, UINT resid)
{
	static const HMODULE hModule = (HMODULE)&__ImageBase;

	HRSRC hrsrc = FindResourceW(hModule, MAKEINTRESOURCEW(resid), L"FILE");
	if (!hrsrc) {
		return E_INVALIDARG;
	}
	HGLOBAL hGlobal = LoadResource(hModule, hrsrc);
	if (!hGlobal) {
		return E_FAIL;
	}
	size = SizeofResource(hModule, hrsrc);
	if (!size) {
		return E_FAIL;
	}
	data = LockResource(hGlobal);
	if (!data) {
		return E_FAIL;
	}

	return S_OK;
}

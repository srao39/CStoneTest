/// Copyright Cornerstone; all rights reserved

#pragma once

// NT
#include <ntstatus.h>

// WinAPI
#define NOMINMAX
#include <Windows.h>
#undef NOMINMAX
#pragma comment( lib, "Crypt32.lib" )

// CNG
#include <Bcrypt.h>
#pragma comment( lib, "Bcrypt.lib" )

// Tool Help Library
#include <TlHelp32.h>

// COM
#include <comdef.h>

// DTC WMI
#include <WbemCli.h>
#pragma comment( lib, "wbemuuid.lib" )

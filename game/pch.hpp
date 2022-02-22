/// Copyright Cornerstone; all rights reserved

#pragma once

#define _HAS_EXCEPTIONS 0
#define _STATIC_CPPLIB

#define include_stl true
#define extend_stl true
#define include_intrin true
#define include_win true
#define include_xor true
#define include_vmp false
#define include_dx false
#define include_protection false // todo use std::shuffle instead of on-the-fly RNG when this is false

#if include_stl
// Utilities library
#include <cstdlib>
#include <csignal>
#include <csetjmp>
#include <cstdarg>
#include <typeinfo>
#include <bitset>
#include <functional>
#include <utility>
#include <ctime>
#include <cstddef>
#include <typeindex>
#include <type_traits>
#include <chrono>
#include <initializer_list>
#include <tuple>
#include <any>
#include <optional>
#include <variant>

// Dynamic memory management
#include <new>
#include <memory>
#include <scoped_allocator>
#include <memory_resource>

// Numeric limits
#include <climits>
#include <cfloat>
#include <limits>
#include <cstdint>
#include <cinttypes>

// Error handling
#include <exception>
#include <stdexcept>
#include <cassert>
#include <cerrno>
#include <system_error>

// Strings library
#include <cctype>
#include <cwctype>
#include <cstring>
#include <cwchar>
#include <string>
#include <cuchar>
#include <string_view>
#include <charconv>

// Containers library
#include <vector>
#include <deque>
#include <list>
#include <set>
#include <map>
#include <stack>
#include <queue>
#include <array>
#include <forward_list>
#include <unordered_set>
#include <unordered_map>

// Iterators library
#include <iterator>

// Algorithms library
#include <algorithm>
#include <execution>

// Numerics library
#include <cmath>
#include <complex>
#include <valarray>
#include <numeric>
#include <random>
#include <ratio>
#include <cfenv>

// Localization library
#include <locale>
#include <clocale>

// Input/output library
#include <iosfwd>
#include <ios>
#include <istream>
#include <ostream>
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <streambuf>
#include <cstdio>

// Filesystem library
#include <filesystem>

// Regular Expressions library
#include <regex>

// Atomic Operations library
#include <atomic>

// Thread support library
#include <thread>
#include <mutex>
#include <future>
#include <condition_variable>
#include <shared_mutex>
#endif

#if include_intrin
#include <immintrin.h>
#endif

#if extend_stl
template< typename elem_t,
		  typename traits_t,
		  typename alloc_t >
auto operator *( std::basic_string< elem_t,
									traits_t,
									alloc_t > const & s,
				 typename std::basic_string< elem_t,
											 traits_t,
											 alloc_t >::size_type const n )
{
	using s_t = std::basic_string< elem_t,
								   traits_t,
								   alloc_t >;

	s_t tmp;
	tmp.reserve( n * s.size( ) );

	for( typename s_t::size_type i = 0;
		 i < n;
		 ++i )
		tmp += s;

	return tmp;
}

template< typename elem_t,
		  typename traits_t,
		  typename alloc_t >
auto operator *( typename std::basic_string< elem_t,
											 traits_t,
											 alloc_t >::size_type n,
				 std::basic_string< elem_t,
									traits_t,
									alloc_t > const & s )
{
	return s * n;
}
#endif

#if include_win
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
#endif

#if include_xor
#include "xorstr.hpp"
#endif

#if include_vmp
// VMProtect
#include <VMProtectSDK.h>

namespace vmp
{
	[[ nodiscard ]] inline char const * s( char const * const value ) noexcept
	{
		return VMProtectDecryptStringA( value );
	}

	[[ nodiscard ]] inline VMP_WCHAR const * s( VMP_WCHAR const * const value ) noexcept
	{
		return VMProtectDecryptStringW( value );
	}

	inline namespace literal
	{
		[[ nodiscard ]] inline char const * operator ""_s( char const * const value,
														   std::size_t const ) noexcept
		{
			return s( value );
		}

		[[ nodiscard ]] inline VMP_WCHAR const * operator ""_s( VMP_WCHAR const * const value,
																std::size_t const ) noexcept
		{
			return s( value );
		}
	}

	[[ nodiscard ]] inline std::string hwid( ) noexcept
	{
		std::string hwid;
		hwid.resize( VMProtectGetCurrentHWID( nullptr,
											  0 ) );

		VMProtectGetCurrentHWID( &hwid[ 0 ],
								 static_cast< int >( hwid.size( ) ) );
		return hwid;
	}
}
#endif

#if include_dx
// Direct Graphics
#include <DXGI.h>
#pragma comment( lib, "dxgi.lib" )
#include <D2D1.h>
#pragma comment( lib, "d2d1.lib" )
#include <d3d9.h>
#pragma comment( lib, "d3d9.lib" )
#include <d3dx9.h>
#pragma comment( lib, "d3dx9" dlib )
#include <D3D11.h>
#pragma comment( lib, "d3d11.lib" )
#include <D3DX11.h>
#pragma comment( lib, "d3dx11" dlib )
#endif

#undef include_stl
#undef include_win
#undef include_vmp
#undef include_dx

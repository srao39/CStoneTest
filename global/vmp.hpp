/// Copyright Cornerstone; all rights reserved

#pragma once

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

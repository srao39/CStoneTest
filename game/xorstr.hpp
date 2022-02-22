/// Copyright https://github.com/JustasMasiulis/xorstr under Apache License 2.0
/// File significantly changed
/// Copyright Cornerstone; all rights reserved

#pragma once

namespace dtl
{
	template< std::uint32_t seed >
	[[ nodiscard ]] __forceinline auto constexpr key4( ) noexcept
	{
		auto val = seed;
		for( char const c: __TIME__ )
			val = static_cast< std::uint32_t >( ( val ^ c ) * 16777619ull );
		return val;
	}

	template< auto size >
	[[ nodiscard ]] __forceinline auto constexpr key8( )
	{
		auto constexpr high = key4< 2166136261 + size >( );
		auto constexpr low = key4< high >( );
		return static_cast< std::uint64_t >( high ) << 32 | low;
	}

	template< typename char_t,
			  auto size,
			  class keys_t,
			  class idcs_t >
	class xor_str_t;
	
	template< class char_t,
			  auto size,
			  auto... keys,
			  auto... idcs >
	class xor_str_t< char_t,
					 size,
					 std::integer_sequence< std::uint64_t,
											keys... >,
					 std::index_sequence< idcs... > >
	{
	public:
		template< class lambda_t >
		__forceinline xor_str_t( lambda_t l,
								 std::integral_constant< std::size_t,
														 size > /*unused*/,
								 std::index_sequence< idcs... > /*unused*/ ) noexcept:
			storage{ std::integral_constant< std::uint64_t,
											 load_xored_str8( keys,
															  idcs,
															  l( ) ) >::value... }
		{ }
	
		[[ nodiscard ]] __forceinline char_t * crypt_get( ) noexcept
		{
			alignas( alignment ) std::uint64_t key[ ]{ keys... };
	
			( ( idcs >= sizeof storage / 32
					? static_cast< void >( 0 )
					: _mm256_store_si256( reinterpret_cast< __m256i * >( storage ) + idcs,
										  _mm256_xor_si256( _mm256_load_si256( reinterpret_cast< __m256i const * >( storage ) + idcs ),
															_mm256_load_si256( reinterpret_cast< __m256i const * >( key ) + idcs ) ) ) ),
				... );
	
			if constexpr( sizeof storage % 32 != 0 )
				_mm_store_si128( reinterpret_cast< __m128i * >( storage + sizeof...( keys ) - 2 ),
								 _mm_xor_si128( _mm_load_si128( reinterpret_cast< __m128i const * >( storage + sizeof...( keys ) - 2 ) ),
												_mm_load_si128( reinterpret_cast< __m128i const * >( key + sizeof...( keys ) - 2 ) ) ) );
	
			return reinterpret_cast< char_t * >( storage );
		}
	
	private:
		static std::uint64_t constexpr alignment = size > 16
													   ? 32
													   : 16;
		alignas( alignment ) std::uint64_t storage[ sizeof...( keys ) ];
	
		[[ nodiscard ]] __forceinline static auto constexpr load_xored_str8( std::uint64_t val,
																			 std::size_t const idx,
																			 char_t const * const str ) noexcept
		{
			using std::uint64_t;
			using uchar_t = std::make_unsigned_t< char_t >;
			auto constexpr value_size = sizeof( char_t );
			auto constexpr idx_off = 8 / value_size;
	
			for( std::size_t i = 0;
				 i + idx * idx_off < size
				 && i < idx_off;
				 ++i )
				val ^= static_cast< uint64_t >( static_cast< uchar_t >( str[ i + idx * idx_off ] ) )
					   << 8 * value_size * ( i % idx_off );
	
			return val;
		}
	};
	
	template< class lambda_t,
			  std::size_t size,
			  std::size_t... idcs >
	xor_str_t( lambda_t l,
			   std::integral_constant< std::size_t,
									   size >,
			   std::index_sequence< idcs... > ) -> xor_str_t< std::decay_t< decltype( l( )[ 0 ] ) >,
															  size,
															  std::integer_sequence< std::uint64_t,
																					 key8< idcs >( )... >,
															  std::index_sequence< idcs... > >;
}

#define xs_val( str ) \
	( ::dtl::xor_str_t( [ ]\
						{ \
							return str;\
						}, \
						std::integral_constant< std::size_t, \
												sizeof( str ) / sizeof( *str ) >{ }, \
												std::make_index_sequence< 2 * ( sizeof( str ) / 16 + ( 0 != sizeof( str ) % 16 ) ) >{ } ) )

#if include_protection
#define xs( str_named ) \
	( xs_val( str_named ).crypt_get( ) )
#else
#define xs( str_named ) \
	( str_named )
#endif

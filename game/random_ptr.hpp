/// Copyright Cornerstone; all rights reserved

#pragma once

#include "globals.hpp"

template< class object_t >
class random_ptr_t
{
public:
	// i don't want to deal with uniqueness/shared counting
	random_ptr_t( random_ptr_t const & ) = delete;
	random_ptr_t( random_ptr_t && ) = delete;
	auto & operator =( random_ptr_t const & ) = delete;
	auto & operator =( random_ptr_t && ) = delete;

	random_ptr_t( ):
		p{ new object_t{ } },
		levels_of_indirection( static_cast< std::uint8_t >( generator( random_engine ) ) )
	{
		for( std::uint8_t level = 1;
			 level < levels_of_indirection;
			 ++level )
			p = reinterpret_cast< object_t * >( new object_t *{ p } );
	}

	~random_ptr_t( )
	{
		delete &**this;

		for( std::uint8_t target_level = levels_of_indirection - 1;
			 target_level > 0;
			 --target_level )
		{
			object_t * p_target = p;

			for( std::uint8_t level = 1;
				 level < target_level;
				 ++level )
				p_target = *reinterpret_cast< object_t * * >( p_target );

			delete reinterpret_cast< object_t * * >( p_target );
		}
	}

	object_t & operator *( )
	{
		object_t * p_target = p;
		for( std::uint8_t level = 1;
			 level < levels_of_indirection;
			 ++level )
			p_target = *reinterpret_cast< object_t * * >( p_target );
		return *p_target;
	}

private:
	static inline std::uniform_int_distribution< std::uint16_t > generator{ 1, 255 };
	object_t * p = nullptr;
	std::uint8_t levels_of_indirection;
};

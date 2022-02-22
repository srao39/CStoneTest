/// Copyright Cornerstone; all rights reserved

#pragma once

inline std::random_device random_device;
inline std::default_random_engine random_engine( random_device( ) );

#include "random_ptr.hpp"


#if include_protection
inline random_ptr_t< double > money_ptr;
#define money ( *money_ptr )
#else
inline double money = 0;
#endif

std::string money_str( double d = money );

std::string center( std::string const & s,
					std::size_t width = 11 );

double spend( std::string const & msg );

bool yesno( std::string const & msg,
			std::string const & yes = xs( "yes" ),
			std::string const & no = xs( "no" ) );

size_t select( std::vector< std::string > const & info );

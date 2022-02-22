/// Copyright Cornerstone; all rights reserved

#include "pch.hpp"

#include "globals.hpp"

std::string money_str( double const d /*= money*/ )
{
	std::ostringstream o;
	o.precision( 2 );
	o << std::fixed << d;
	return o.str( );
}

std::string center( std::string const & s,
					std::size_t const width /*= 11*/ )
{
	if( width <= s.size( ) )
		return s;

	auto const whitespace = width - s.size( );
	auto const lpad = whitespace / 2;
	auto const rpad = whitespace - lpad;
	return std::string( lpad,
						' ' ) + s + std::string( rpad,
												 ' ' );
}

void clean_cin( )
{
	std::cin.clear( );
	std::cin.ignore( std::numeric_limits< std::streamsize >::max( ),
					 '\n' );
}

double spend( std::string const & msg )
{
	double cost;
	std::cin >> cost;

	if( !std::cin.fail( )
		&& 0 <= cost
		&& cost <= money )
	{
		cost = std::round( 100 * cost ) / 100;
		money -= cost;
		return cost;
	}

	clean_cin( );
	std::cout << xs( "You must enter a value less than or equal to the money you have!\n"
					"You have $" ) << money_str( ) << xs( ".\n" );
	return spend( msg );
}

bool yesno( std::string const & msg,
			std::string const & yes /*= xs( "yes" )*/,
			std::string const & no /*= xs( "no" )*/ )
{
	std::cout << msg << ' ';

	std::string i{ };
	std::cin >> i;

	if( yes == i )
		return true;
	if( no == i )
		return false;

	clean_cin( );
	std::cout << xs( "I didn't understand your response! Reply \"" ) << yes << xs( "\" or \"" ) << no << xs( "\".\n" );
	return yesno( msg,
				  yes,
				  no );
}

size_t select( std::vector< std::string > const & info )
{
	std::cout << info[ 0 ];

	for( std::size_t i = 1;
		 i < info.size( );
		 ++i )
		std::cout << xs( "\n\t#" ) << i << xs( ": " ) << info[ i ];

	std::cout << xs( "\nWhich would you like? " );

	std::size_t i;
	std::cin >> i;

	if( !std::cin.fail( )
		&& i != 0
		&& i < info.size( ) )
		return i;

	clean_cin( );
	std::cout << xs( "You must enter a number between 1 and " ) << info.size( ) - 1 << xs( "!\n" );
	return select( info );
}

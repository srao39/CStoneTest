/// Copyright Cornerstone; all rights reserved

#include "pch.hpp"

#include "scratch_off/scratch_off_game.hpp"
#include "blackjack/blackjack_game.hpp"

#include "xorstr.hpp"

void play( )
{
	while( money > 0 )
	{
		system( xs( "cls" ) );
		std::cout << xs( "Thank you for gambling!\n"
						"You have $" ) + money_str( ) + xs( ".\n"
														   "There are 2 games to play:\n"
														   "\t#1: Scratch offs\n"
														   "\t#2: Blackjack\n" );
	retry:
		std::cout << xs( "Which would you like to play? " );

		std::size_t i;
		std::cin >> i;

		if( std::cin.fail( )
			|| 1 > i
			|| 2 < i )
		{
			std::cin.clear( );
			std::cin.ignore( std::numeric_limits< std::streamsize >::max( ),
							 '\n' );
			std::cout << xs( "You must enter a number between 1 and 2!\n" );
			goto retry;
		}

		switch( i )
		{
			case 1:
				scratch_off_game( );
				break;
			case 2:
				blackjack_game( );
				break;
		}
	}
}

// todo reformat to "input, update, render" because the current format is spaghetti code.
// todo other games like coin flip
// todo add music
// todo get console click position
// todo change drawing from system("cls") to a set-cursor kind of deal
// todo gui https://github.com/FlatGlobus/WTLBuilder
int main( )
{
	// faster printing by disabling syncing
	std::ios_base::sync_with_stdio( false );
	
	money = 50;
	
	play( );

	std::cout << xs( "Say anything to exit. " ) << std::flush;
	std::cin.get( );

	return 0;
}

#if 0
int main( )
{
	std::cout << "hello\nhello!\nhello\n";
	SetConsoleCursorPosition( GetStdHandle( STD_OUTPUT_HANDLE ),
							  { 0, 1 } );
	std::cout << "world\n";
	std::cin.get( );
}
#endif

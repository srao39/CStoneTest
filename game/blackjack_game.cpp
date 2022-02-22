/// Copyright Cornerstone; all rights reserved

#include "pch.hpp"

#include "blackjack_game.hpp"

using std::cout;
using std::cin;
using namespace std::string_literals;

auto & hit = blackjack_table_t::hit;
auto & cards = blackjack_table_t::cards;
auto & value = blackjack_table_t::value;
auto & rng_card = blackjack_table_t::rng_card;
using hand_t = blackjack_table_t::hand_t;
using match_t = blackjack_table_t::match_t;

std::array< std::string, 4 > const names{ xs( "SPADE" ), xs( "HEART" ), xs( "DIAMOND" ), xs( "CLOVER" ) };
std::array< std::string, 4 > const short_names{ xs( "S" ), xs( "H" ), xs( "D" ), xs( "C" ) };
std::array< std::string, 13 > const values{ xs( "ACE" ),
											xs( "TWO" ),
											xs( "THREE" ),
											xs( "FOUR" ),
											xs( "FIVE" ),
											xs( "SIX" ),
											xs( "SEVEN" ),
											xs( "EIGHT" ),
											xs( "NINE" ),
											xs( "TEN" ),
											xs( "JACK" ),
											xs( "QUEEN" ),
											xs( "KING" ) };
std::array< std::string, 13 > const short_values{ xs( "A" ),
												  xs( "2" ),
												  xs( "3" ),
												  xs( "4" ),
												  xs( "5" ),
												  xs( "6" ),
												  xs( "7" ),
												  xs( "8" ),
												  xs( "9" ),
												  xs( "10" ),
												  xs( "J" ),
												  xs( "Q" ),
												  xs( "K" ) };

void print_hand( hand_t const & h,
				 std::string const & name )
{
	auto color = [ ]( auto kind )
	{
		static auto const con_h = GetStdHandle( STD_OUTPUT_HANDLE );

		WORD clr = FOREGROUND_INTENSITY;
		switch( kind )
		{
			case 0: // spade
				clr |= FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE;
				break;
			case 2: // diamond
				clr |= FOREGROUND_BLUE;
				break;
			case 1: // heart
				clr |= FOREGROUND_RED;
				break;
			case 3: // clover
				clr |= FOREGROUND_GREEN;
				break;
		}

		SetConsoleTextAttribute( con_h,
								 clr );
	};

	auto const c = std::max( static_cast< blackjack_table_t::count_t >( 2 ),
							 cards( h ) );

	// TITLE BAR
	cout << xs( " ------------" );
	for( std::size_t i = 1;
		 i < c;
		 ++i )
		cout << xs( "-------------" );

	cout << xs( "\n|" ) << center( name,
								   12 * c + c - 1 ) << xs( "|\n" );

#define card_for \
	for( std::size_t i = 0; \
		 i < c; \
		 ++i )

	auto title_bar = [ & ]
	{
		card_for
			cout << xs( " ------------" );
		cout << '\n';
	};

	auto corner_row = [ & ]
	{
		card_for
		{
			cout << xs( "| " );
			color( h[ i ].first );
			auto const v = h[ i ].second - 1;
			if( -1 != v )
				cout << short_values[ v ] << std::string( xs( "          " ) ).substr( short_values[ v ].size( ) * 2 ) << short_values[ v ] << xs( " " );
			else
				cout << '?' << std::string( xs( "          " ) ).substr( 2 ) << '?' << xs( " " );
			color( 0 );
		}
		cout << xs( "|\n" );
	};

	auto print_art = [ & ]( auto k,
							auto const & art )
	{
		cout << '|';
		color( k );
		cout << center( art,
						12 );
		color( 0 );
	};

	auto edge_row = [ & ]
	{
		card_for
		{
			auto [ k, v ] = h[ i ];
			auto symbol = short_names[ k ];
			std::string art{ };

			if( v == 0 )
				symbol = '?';

			if( 3 == v
				|| 9 == v
				|| 10 == v )
				art = symbol;
			else
				art = ' ';

			if( 4 < v
				&& v < 11 )
				art = symbol + ' ' + art + ' ' + symbol;

			print_art( k,
					   art );
		}
		cout << xs( "|\n" );
	};

	auto sandwich_row = [ & ]
	{
		card_for
		{
			auto [ k, v ] = h[ i ];
			auto symbol = short_names[ k ];
			std::string art{ };

			if( v == 0 )
				symbol = '?';

			if( 2 == v
				|| 8 == v
				|| 10 == v )
				art = symbol;
			else if( 4 == v )
				art = symbol + xs( "   " ) + symbol;

			print_art( k,
					   art );
		}
		cout << xs( "|\n" );
	};

	auto center_row = [ & ]
	{
		card_for
		{
			auto [ k, v ] = h[ i ];
			auto symbol = short_names[ k ];
			std::string art{ };

			if( v == 0 )
				symbol = '?';

			if( v % 2
				|| v > 10 )
				art = symbol;
			else
				art = ' ';

			if( 5 < v
				&& v <= 10 )
				art = symbol + ' ' + art + ' ' + symbol;

			print_art( k,
					   art );
		}
		cout << xs( "|\n" );
	};

	title_bar( );

	corner_row( );

	edge_row( );

	sandwich_row( );

	center_row( );

	sandwich_row( );

	edge_row( );

	corner_row( );

	title_bar( );
}

double get_bet( )
{
	cout << xs( "How much would you like to wager? " );

	double bet;
	std::cin >> bet;
	if( !std::cin.fail( ) )
	{
		if( bet > money )
		{
			cout << xs( "You must bet less than you have!\n"
					   "You have $" ) << money_str( ) << xs( ".\n" );
			return get_bet( );
		}

		bet = std::round( 100 * bet ) / 100;
		money -= bet;
		return bet;
	}

	std::cin.clear( );
	std::cin.ignore( std::numeric_limits< std::streamsize >::max( ),
					 '\n' );
	cout << xs( "I didn't understand your response! Reply an amount you'd like to wager.\n" );
	return get_bet( );
}

bool surrender( )
{
	std::cout << xs( "Would you like to surrender and reclaim half of your wager? " );

	std::string i;
	std::cin >> i;

	if( xs( "yes" ) == i )
		return true;
	if( xs( "no" ) == i )
		return false;

	cout << xs( "I didn't understand your response! Reply \"yes\" or \"no\" to reclaim surrender.\n" );
	return surrender( );
}

double get_sidebet( )
{
	std::cout << xs( "The dealer has a visible ace! Would you like to make a side-wager that the dealer has blackjack? " );

	std::string i;
	std::cin >> i;

	if( xs( "yes" ) == i )
		return get_bet( );
	if( xs( "no" ) == i )
		return 0;

	cout << xs( "I didn't understand your response! Reply \"yes\" or \"no\" to make a side-wager.\n" );
	return get_sidebet( );
}

bool get_splitbet( std::vector< match_t > & h )
{
	std::cout << xs( "You have two of the same card! Would you like to split your hand into two hands? " );

	std::string i;
	std::cin >> i;

	if( xs( "yes" ) == i )
	{
		std::cout << xs( "You have $" ) << money_str( ) << xs( ".\n" );
		auto const bet = get_bet( );
		auto const card = h.back( ).second[ 1 ];
		h.back( ).second[ 1 ] = rng_card( );
		h.push_back( { bet, { card, rng_card( ) } } );
		return true;
	}
	if( xs( "no" ) == i )
		return false;

	cout << xs( "I didn't understand your response! Reply \"yes\" or \"no\" to split your hand.\n" );
	return get_splitbet( h );
}

bool ask_for_hit( blackjack_table_t & t,
				  hand_t & h )
{
	std::cout << xs( "Would you like to hit or stand? " );

	std::string i;
	std::cin >> i;

	if( xs( "hit" ) == i )
	{
		t.hit( h );
		return true;
	}
	if( xs( "stand" ) == i )
		return false;

	cout << xs( "I didn't understand your response! Reply \"hit\" or \"stand\".\n" );
	return ask_for_hit( t,
						h );
}

extern bool play_again( );

bool double_bet( )
{
	std::cout << xs( "Would you like to double your bet? " );

	std::string i;
	std::cin >> i;

	if( xs( "yes" ) == i )
		return true;
	if( xs( "no" ) == i )
		return false;

	cout << xs( "I didn't understand your response! Reply \"yes\" or \"no\" to double your bet.\n" );
	return double_bet( );
}

void blackjack_game( )
{
	for( bool c = money >= 0.01;
		 c;
		 c = money > 0.01
			 && play_again( ) )
	{
		system( xs( "cls" ) );
		cout << xs( "Thank you for playing Blackjack!\n" );
		cout << xs( "You have $" ) << money_str( ) << xs( "!\n" );

		double const bet = get_bet( );

		blackjack_table_t t;

		auto print_table = [ & ]
		{
			system( xs( "cls" ) );

			cout << xs( "Thank you for playing Blackjack!\n" );

			print_hand( t.dealer_hand,
						xs( "Dealer (" ) + std::to_string( value( t.dealer_hand ) ) + xs( ")" ) );

			for( auto && [ bet, hand ]: t.player_matches )
				print_hand( hand,
							'$' + money_str( bet ) + xs( " (" ) + std::to_string( value( hand ) ) + xs( ")" ) );
		};

		t.player_matches[ 0 ].first = bet;

		print_table( );

		if( bet <= money
			&& double_bet( ) )
		{
			money -= bet;
			t.player_matches[ 0 ].first = bet * 2;
		}

		t.player_matches[ 0 ].second[ cards( t.player_matches[ 0 ].second ) ] = rng_card( );

		print_table( );

		double sidebet = 0;

		// dealer has blackjack sidebet
		if( money >= 0.01 )
			if( 1 == t.dealer_hand[ 0 ].second )
				sidebet = get_sidebet( );

		if( 21 != value( t.player_matches[ 0 ].second ) )
		{
			// surrender and get 1/2 money back
			if( surrender( ) )
			{
				money -= t.player_matches[ 0 ].first / 2;
				if( !play_again( ) )
					return;
				continue;
			}

			// dupe cards split bet
			for( auto && [ b, hand ]: t.player_matches )
				if( hand[ 0 ].second == hand[ 1 ].second )
					if( money >= 0.01 )
						if( get_splitbet( t.player_matches ) )
							print_table( );

			// play hands
			for( auto && [ b, hand ]: t.player_matches )
				while( value( hand ) < 21
					   && ask_for_hit( t,
									   hand ) )
					print_table( );
		}

		t.hit( t.dealer_hand );
		print_table( );
		while( value( t.dealer_hand ) < 17 )
		{
			std::this_thread::sleep_for( std::chrono::seconds( 1 ) );
			t.hit( t.dealer_hand );
			print_table( );
		}

		for( ;
			value( t.dealer_hand ) < 17;
			t.hit( t.dealer_hand ),
			print_table( ),
			std::this_thread::sleep_for( std::chrono::seconds( 1 ) ) )
			print_table( );

		auto w = t.winnings( sidebet );
		money += w;
		if( w < 0 )
			std::cout << xs( "You lost $" ) << money_str( -w ) << xs( ".\n" );
		else
			std::cout << xs( "You won $" ) << money_str( w ) << xs( "!\n" );
	}

	cout << xs( "You're out of money! Thanks for playing Blackjack!" ) << std::endl;
}

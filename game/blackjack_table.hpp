/// Copyright Cornerstone; all rights reserved

#pragma once

class blackjack_table_t
{
public:
	using count_t = std::uint8_t;
	using idx_t = count_t;
	using kind_t = count_t; // 0 is spade, 1 is heart, 2 is diamond, 3 is clover
	using value_t = count_t; // 0 is none, 1 is a, 2-10, 11 is j, 12 is q, 13 is k
#if 0 // only one 52-card deck at play
	static auto constexpr max_cards = 11; // = 4 + 4 + 3 = ( a a a a = 4 ) + ( 2 2 2 2 = 8 ) + ( 3 3 3 = 9 )
	using hand_t = std::array< kind_t,
							   max_cards >;
#else // we can have multiple decks, this prevents card-counting
	static auto constexpr max_cards = 21;
	using card_t = std::pair< kind_t,
							  value_t >;
	using hand_t = std::array< card_t,
							   max_cards >;
	using match_t = std::pair< double,
							   hand_t >;
#endif
	[[ nodiscard ]] static card_t rng_card( )
	{
		static std::uniform_int_distribution value( 1,
													13 );
		static std::uniform_int_distribution kind( 0,
												   3 );
		card_t result;
		result.first = static_cast< kind_t>( kind( random_engine ) );
		result.second = static_cast< kind_t>( value( random_engine ) );
		return result;
	}

	static count_t cards( hand_t const & hand )
	{
		for( std::size_t i = 0;
			 i < hand.size( );
			 ++i )
			if( 0 == hand[ i ].second )
				return static_cast< count_t >( i );

		return static_cast< count_t >( hand.size( ) );
	}

	static void hit( hand_t & hand )
	{
		hand[ cards( hand ) ] = rng_card( );
	}

	[[ nodiscard ]] static value_t value( hand_t const & hand )
	{
		value_t total = 0;
		count_t aces = 0;

		for( auto [ kind, value ]: hand )
		{
			if( 0 == value )
				break;
			if( 1 == value )
				++aces;
			total += std::min( 10ui8,
							   value );
		}

		for( count_t i = 0;
			 i < aces
			 && 21 >= total + 10;
			 ++i )
			total += 10;

		return total;
	}

	[[ nodiscard ]] double winnings( double const sidebet ) const
	{
		double total = 0;
		auto const target = value( dealer_hand );
		if( 21 == target
			&& 2 == cards( dealer_hand ) )
			total += sidebet;

		for( auto && [ bet, hand ]: player_matches )
		{
			auto const base = value( hand );

			if( base == target )
				total += bet;
			else if( base <= 21 )
				if( target > 21
					|| target < base )
					total += 2.5 * bet;
		}

		return total;
	}

	std::vector< match_t > player_matches{ { 0, { rng_card( ) } } };
	hand_t dealer_hand{ rng_card( ) };
};

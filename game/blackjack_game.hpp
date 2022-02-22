/// Copyright Cornerstone; all rights reserved

#pragma once

#include "../globals.hpp"
#include "blackjack_table.hpp"

void print_hand( blackjack_table_t::hand_t const & h,
				 std::string const & name );
void blackjack_game( );

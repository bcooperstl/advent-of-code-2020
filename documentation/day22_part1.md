# Day 22, Part 1 #

## Requirements ##

There is a two player game, where each player starts with a deck of cards.

Simulate the game war, where
1) Each player plays their top cards
2) The player with the higher card wins both players cards
3) The winning player places the two cards at the back of the winner's deck, with the winner's card ahead of the loser's card
4) Repeat until one player is out of cards.

When a player has all the cards, calculate the score by multiplying the value of the card by it's position in the deck.
The top card is multiplied by *n*, the next card by *n-1*, so on until the last card is multiplied by 1, where *n* is the number of 

### Input Format ###

A label "Player 1:" followed by a list of card that player 1 has, one per line.
Then a blank line.
Then A label "Player 2:" followed by a list of card that player 2 has, one per line.

### Output Format ###

Integer - the final score for the winning player.

## Test Cases ##

### Given Test Cases ###

There is [one test case](../data/test_cases/day22_part1.txt) given. It gives a game where player 2 wins.

### Custom Test Cases ###

Just for completion sake, I will create an [inverted test case](../data/test_cases/day22_part2.txt) where player 1 wins.

## Approach ##

### Data structures ###

I'll create a deck data structure that contains:
* the player's list of cards.
* method to remove the top card
* method to add a card to the bottom of the deck
* method to calculate the score for the deck.
* method to return the number of cards in the deck

### Main Program ###

* Parse the input and create the two player's decks from it.
* While both players have cards in the deck
    * Pop the top card off of each player's deck
    * If player 1's card is larger:
        * Add player 1's card to the back of player 1's deck
        * Add player 2's card to the back of player 1's deck
    * else
        * Add player 2's card to the back of player 1's deck
        * Add player 1's card to the back of player 1's deck
* If there are still cards in player 1's deck:
    * Output player 1's score
* else 
    * Output player 2's score

## Things I learned ##

Pretty simple to develop the algorithm to play this game. I expect part 2 to be much harder.

Using the c++ deque STL class to handle all of the queue operations. I was going to use the simple queue class, but I need to iterate over it.


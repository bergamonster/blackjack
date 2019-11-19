/*
  int vector of size 312 (52*6)
  %52 + 1 to get value of card
  /52 to get suit
    [0,1) Clubs
    [1,2) Diamonds
    [2,3) Hearts
    [3,4) Spades
    
    Randomize array
    If conditions for 10-14
    Ace = 1 (or 11, add 10)
    
    Jack = 11, Queen = 12, King = 13 -> if > 12 then = 10
    
    Suit variable
*/

#include <iostream>
#include <vector>
#include <algorithm>
#include <cstdlib>
#include <ctime>

using namespace std;

// Random generator function from c++ documentation:
int myrandom(int i){ 
  return std::rand()%i;
}

// Randomly shuffles order of deck
void shuffle(vector<int> &cards){
  random_shuffle(cards.begin(), cards.end(), myrandom);
}

int getValue(int card){
  int counter = card/52; // Figure out card in deck
  int deck = card - counter*52; 
  counter = deck/13; // Figure out value of card
  return deck - counter*13 + 1;
}

/*
int getSuit(int card){
  //suit map
}
*/

int main(){
  srand(time(NULL)); // Set seed
  int numDecks = 6;
  int wallet;
  vector<int> cards;
  vector<int> playerHand;
  vector<int> dealerHand;
  int currentCard = 0;
  
  for (int i = 0; i < numDecks * 52; ++i){ // Creates deck in order
    cards.push_back(i);
  }

  shuffle(cards);
  
  for (const auto &card : cards){
    //cout << card << " ";
  }
  
  // Deal
  playerHand.push_back(cards[currentCard++]);
  dealerHand.push_back(cards[currentCard++]);
  playerHand.push_back(cards[currentCard++]);
  dealerHand.push_back(cards[currentCard++]);
  
  for (const auto &card : playerHand){
    cout << getValue(card) << " ";
  }
  cout << endl;
  for (const auto &card : dealerHand){
    cout << getValue(card) << " ";
  }
  cout << endl;
  return 0;
}

/*
   * Old syntax.
   *
  for (int i = 0; i < words.size(); ++i){
    cout << words[i] << endl;
  }
  
  // Ranged-for syntax
  for (const string &temp : words){
    cout << temp << endl;
  }
  */
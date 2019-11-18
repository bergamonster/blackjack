#include <iostream>
#include <vector>
#include <algorithm>
#include <cstdlib>
#include <ctime>

using namespace std;

// random generator function from c++ documentation:
int myrandom (int i) { return std::rand()%i;}

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
    Ace = 11 (or 1, subtract 10)
    
    Jack = 12, Queen = 13, King = 14 -> if > 11 then = 10
    
    Suit variable
*/

int main() {
  srand(time(NULL));
  int numDecks = 6;
  vector<int> cards;
  
  for (int i = 0; i < numDecks * 52; ++i){ // Creates deck in order
    cards.push_back(i);
  }
  
  for (const auto &card : cards){
    cout << card << " ";
  }
  
  cout << endl << endl;
  
  random_shuffle(cards.begin(), cards.end(), myrandom); // Shuffles deck
  
  for (const auto &card : cards){
    cout << card << " ";
  }

  return 0;
}


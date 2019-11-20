#include <iostream>
#include <vector>
#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <cstring>

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
  int val =  deck - counter*13 + 1;
  return val;
}

int getSum(vector<int> hand){
  //Factor in 1
  int sum = 0;
  for (const auto &card : hand){
    sum += card;
  }
  return sum;
}

char* getSuit(int card){
  int counter = card/52; // Figure out card in deck
  card = card - counter*52;
  if (card <= 13){
    return (char*)"Heart";
  }
  else if (card <=26){
    return (char*)"Diamond";
  }
  else if (card <=39) {
    return (char*)"Club";
  }
  else if (card <= 52){
    return (char*)"Spade";
  }
  else{
    return (char*)"Error";
  }
}

void printCard(int card){
  int val = getValue(card);
  if (val == 1){
    cout << "Ace";
  }
  else if (val == 11){
    cout << "Jack";
  }
  else if (val == 12){
    cout << "Queen";
  }
  else if (val == 13){
    cout << "King";
  }
  else{
    cout << val;
  }
  cout << " of " << getSuit(card) << "s";
}

int main(){
  srand(time(NULL)); // Set seed
  int numDecks = 6;
  int wallet;
  vector<int> cards; // Total deck
  vector<int> playerHand; // Exact cards in player hand
  vector<int> dealerHand;  // Exact cards in dealer hand
  
  // Deals with ace value
  vector<int> playerValues;  // Values of cards in player hand
  vector<int> dealerValues;  // Values of cards in dealer hand
  
  int playerSum;  // Total sum of player hand
  int dealerSum;  // Total sum of dealer hand
  vector<string> playerSuit;  // Suit of cards in player hand
  vector<string> dealerSuit;  // Suit of cards in dealer hand
  int currentCard = 0;


  for (int i = 0; i < numDecks * 52; ++i){ // Creates deck in order
    cards.push_back(i);
  }

  shuffle(cards);
/*
// Disp Deck
  for (const auto &card : cards){
   cout << card << " ";
  }
        cout << endl << endl;
*/
  // Deal
  playerHand.push_back(cards[currentCard++]);
  dealerHand.push_back(cards[currentCard++]);
  playerHand.push_back(cards[currentCard++]);
  dealerHand.push_back(cards[currentCard++]);
/*
  for (const auto &card : playerHand){
    cout << getValue(card) << " ";
  }
  cout << endl;

  for (const auto &card : dealerHand){
    cout << getValue(card) << " ";
  }
  cout << endl;
*/
  
  //Determine Suits
  for (const auto &card : playerHand){
    playerSuit.push_back(getSuit(card));
  }
  for (const auto &card : dealerHand){
    dealerSuit.push_back(getSuit(card));
  }

  //Determine Initial Card Values
  for (const auto &card : playerHand){
    if (getValue(card) == 1){
      playerValues.push_back(11); // Set Aces initially to 11
    }
    else if (getValue(card) > 10){
      playerValues.push_back(10);
    }
    else{
      playerValues.push_back(getValue(card));
    }
  }
  for (const auto &card : dealerHand){
    if (getValue(card) == 1){
      dealerValues.push_back(11); // Set Aces initially to 11
    }
    else if (getValue(card) > 10){
      dealerValues.push_back(10);
    }
    else{
      dealerValues.push_back(getValue(card));
    }
  }

  //Determine Sums
  playerSum = getSum(playerValues);
  dealerSum = getSum(dealerValues);

  // Begin Game
  
  //Disp Dealer Card
  cout << "The Dealer Has Been Dealt: " << endl;
  //cout << getValue(dealerHand[1]) << endl;
  printCard(dealerHand[1]);
  cout << endl << endl;
  
  //Disp Player Cards
  cout << "Your Cards are:" << endl;
  //cout << getValue(playerHand[0]) << " " << getValue(playerHand[1]) << endl;
  printCard(playerHand[0]); 
  cout << endl;
  printCard(playerHand[1]);
  cout << endl;
  cout << playerSum << endl;
  
  cout << endl;
  for (const auto &val : playerValues){
    cout << val << endl;
  }
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
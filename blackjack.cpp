#include <iostream>
#include <vector>
#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <cstring>
#include <sstream>

using namespace std;

// Random generator function from c++ documentation:
int myrandom(int i){
  return std::rand()%i;
}

// Randomly shuffles order of deck
void shuffle(vector<int> &cards){
  random_shuffle(cards.begin(), cards.end(), myrandom);
}

int checkAce(vector<int> hand){
  for (int i = 0; i < hand.size(); ++i){
    if (hand[i] ==  11){
      return i;
    }
  }
  return -1;
}

int getValue(int card, bool real){
  int counter = card/52; // Figure out card in deck
  int deck = card - counter*52;
  counter = deck/13; // Figure out value of card
  int val =  deck - counter*13 + 1;
  if (real && val > 10){
    val = 10;
  }
  if (real && val == 1){
    val = 11;
  }
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
  int val = getValue(card, false);
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

//Will need to change this to a function for multiple games
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
    if (getValue(card, false) == 1){
      playerValues.push_back(11); // Set Aces initially to 11
    }
    else if (getValue(card, false) > 10){
      playerValues.push_back(10);
    }
    else{
      playerValues.push_back(getValue(card, false));
    }
  }
  for (const auto &card : dealerHand){
    if (getValue(card, false) == 1){
      dealerValues.push_back(11); // Set Aces initially to 11
    }
    else if (getValue(card, false) > 10){
      dealerValues.push_back(10);
    }
    else{
      dealerValues.push_back(getValue(card, false));
    }
  }

  //Determine Sums
  playerSum = getSum(playerValues);
  dealerSum = getSum(dealerValues);

  // Begin Game
  
  //Disp Dealer Card
  cout << endl << "The Dealer Has Been Dealt: " << endl;
  //cout << getValue(dealerHand[1]) << endl;
  printCard(dealerHand[0]);
  cout << endl << endl;
  
  //Disp Player Cards
  cout << "Your Cards are:" << endl;
  //cout << getValue(playerHand[0]) << " " << getValue(playerHand[1]) << endl;
  printCard(playerHand[0]); 
  cout << endl;
  printCard(playerHand[1]);
  cout << endl << "Hand Total: " << playerSum;
  cout << endl << endl;
  
  bool dealerBust = false;
  bool playerBust = false;
  
  //Player's Turn
  char message[10];
  while(playerSum != 21 && cin.getline(message, 5)){
    if (!strcmp(message, "Quit")){
      return 0;
    }
    else if (!strcmp(message, "Hit")){
      cout << endl;
      int newCard = cards[currentCard++];
      playerHand.push_back(newCard);
      playerValues.push_back(getValue(newCard, true));
      playerSum = getSum(playerValues);
      printCard(playerHand[playerHand.size()-1]);
      cout << endl;
      
      while (playerSum > 21 && checkAce(playerValues) != -1){
        int i = checkAce(playerValues);
        playerValues[i] = 1;
        playerSum = getSum(playerValues); 
        }
      if(playerSum > 21){
        cout << "Hand Total: " << playerSum << endl << endl;
        cout << "Bust" << endl << endl;
        playerBust = true;
        break;
      }
      cout << "Hand Total: " << playerSum << endl << endl;
    }
    else if (!strcmp(message, "Stand")){
      break;
    }
    else{
      cout << "Accepted commands are \'Quit\', \'Hit\', and \'Stand\'" << endl;
    }
    /*
    if (dealerBust || playerBust){
      break;
    }*/
  }
  
  // Dealer's Turn
  
  cout << endl << "Dealer\'s Covered Card: " << endl;
  printCard(dealerHand[1]);
  cout << endl << "Hand Total: " << dealerSum << endl<< endl;
  
  
  while (dealerSum < 17 && !playerBust){
    cout << endl << "Dealer Hits" << endl;
    int newCard = cards[currentCard++];
    dealerHand.push_back(newCard);
    dealerValues.push_back(getValue(newCard, true));
    dealerSum = getSum(dealerValues);
    printCard(dealerHand[dealerHand.size()-1]);
    cout << endl;
        
    // Work in dealer having two aces
    while (dealerSum > 21 && checkAce(playerValues) != -1){
      int i = checkAce(dealerValues);
      dealerValues[i] = 1;
      dealerSum = getSum(dealerValues); 
    }
        
    if(dealerSum > 21){
      dealerBust = true;
      cout << "Hand Total: " << dealerSum << endl << endl;
      cout << "Dealer Busts" << endl << endl;
      break;
      }
    cout << "Hand Total: " << dealerSum << endl << endl;
  } 
  
  if (!playerBust && ! dealerBust){
  cout << "Dealer Stands" << endl << endl;
  }
  
  // Print results
  cout << "***************************************" << endl << endl;
  if (playerBust){
    cout << "Player Busted, Dealer Wins" << endl;
  }
  else if (dealerBust){
    cout << "Dealer Busted, Player Wins" << endl;
  }
  else if (dealerSum > playerSum){
    cout << "Dealer Wins: " << dealerSum << " > " << playerSum << endl;
  }
  else if (playerSum > dealerSum){
    cout << "Player Wins: " << playerSum << " > " << dealerSum << endl;
  }
  else { //tie
    if (playerHand.size() > dealerHand.size()){
      cout << "Dealer Wins: " << "(Less Cards)" << endl;
    }
    else if (dealerHand.size() > playerHand.size()){
      cout << "Player Wins: " << "(Less Cards)" << endl;
    }
    else{ // Same cards
      cout << "It's a tie!" << endl;
    }
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
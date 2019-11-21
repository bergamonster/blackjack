/*
If you lose, the dealer gets your $100. If you win, you get your original $100 bet back, 
plus the dealer gives you $100. If you draw (or push) you keep your bet money. And if you 
get blackjack, you get your original $100 bet back, plus $150 from the dealer because for 
blackjack, you get 1.5 times your bet.
*/

#include <iostream>
#include <vector>
#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <cstring>
#include <string>
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

// Checks hand for an ace that could be changed in value to 1
int checkAce(vector<int> hand){
  for (int i = 0; i < hand.size(); ++i){
    if (hand[i] ==  11){
      return i;
    }
  }
  return -1;
}

// Gets value of card, A-10, J (11), Q (12), K (13)
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

// Returns sum of cards in hand
int getSum(vector<int> hand){
  //Factor in 1
  int sum = 0;
  for (const auto &card : hand){
    sum += card;
  }
  return sum;
}

// Returns suit of card
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

// Print function for cards
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

// Single round of blackjack
int game(double& wallet){
  srand(time(NULL)); // Set seed
  int numDecks = 6;
  vector<int> cards; // Total deck
  vector<int> playerHand; // Exact cards in player hand
  vector<int> dealerHand;  // Exact cards in dealer hand
  double bet;
  bool blackJack = false;
  
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

  // Bet
  cout << "Enter Bet: $";
  cin >> bet;
  cin.clear();
  wallet -= bet; 
  
  // Display Wallet
  cout << "Wallet after bet: $" << wallet << endl << endl;
  
  // Deal
  playerHand.push_back(cards[currentCard++]);
  dealerHand.push_back(cards[currentCard++]);
  playerHand.push_back(cards[currentCard++]);
  dealerHand.push_back(cards[currentCard++]);
  
  // Determine Suits
  for (const auto &card : playerHand){
    playerSuit.push_back(getSuit(card));
  }
  for (const auto &card : dealerHand){
    dealerSuit.push_back(getSuit(card));
  }

  // Determine Initial Card Values
  for (const auto &card : playerHand){
    if (getValue(card, false) == 1){
      playerValues.push_back(11); // Set Aces initially to 11
    }
    else if (getValue(card, false) > 10){
      playerValues.push_back(10); // Set face card value to 10
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
      dealerValues.push_back(10); // Set face card value to 10
    }
    else{
      dealerValues.push_back(getValue(card, false));
    }
  }

  // Determine Sums
  playerSum = getSum(playerValues);
  dealerSum = getSum(dealerValues);

  // *******************
  // Begin Game
  // *******************
  
  // Display Dealer Card
  cout << endl << "The Dealer Has Been Dealt: " << endl;
  printCard(dealerHand[0]);
  cout << endl << endl;
  
  // Display Player Cards
  cout << "Your Cards are:" << endl;
  printCard(playerHand[0]); 
  cout << endl;
  printCard(playerHand[1]);
  cout << endl << "Hand Total: " << playerSum;
  cout << endl << endl;
  
  bool dealerBust = false;
  bool playerBust = false;
  
  if(playerSum == 21){
    blackJack = true;
  }
  
  // Player's Turn
  string message;
  if (!blackJack){ // Auto pass to Dealer turn if blackjack
    cin >> ws;
    while(cin >> message){
      cin.clear();
      if (message == "Quit"){ // Quit from game
        return 0;
      	}
      else if (message == "Hit"){
        cout << endl;
        int newCard = cards[currentCard++];
        playerHand.push_back(newCard);
        playerValues.push_back(getValue(newCard, true));
        playerSum = getSum(playerValues);
        printCard(playerHand[playerHand.size()-1]);
        cout << endl;
      
        while (playerSum > 21 && checkAce(playerValues) != -1){ // Makes sure no Ace values could change
          int i = checkAce(playerValues);
          playerValues[i] = 1;
          playerSum = getSum(playerValues); 
          }
        if(playerSum > 21){ // Player Busts
          cout << "Hand Total: " << playerSum << endl << endl;
          cout << "Bust" << endl << endl;
          playerBust = true;
          break;
        }
        cout << "Hand Total: " << playerSum << endl << endl;
      }
      else if (message == "Stand"){
        break;
      }
      else{
        cout << "Accepted commands are \'Quit\', \'Hit\', and \'Stand\'" << endl;
      }
    }
  }
  
  // Dealer's Turn
  cout << endl << "Dealer\'s Covered Card: " << endl;
  printCard(dealerHand[1]); // Reveal dealer's card
  cout << endl << "Hand Total: " << dealerSum << endl << endl;
  
  
  while (dealerSum < 17 && !playerBust){ // Dealer only hits up to 17 and if the player hasn't busted
    cout << "Dealer Hits" << endl;
    int newCard = cards[currentCard++];
    dealerHand.push_back(newCard);
    dealerValues.push_back(getValue(newCard, true));
    dealerSum = getSum(dealerValues);
    printCard(dealerHand[dealerHand.size()-1]);
    cout << endl;
        
    while (dealerSum > 21 && checkAce(dealerValues) != -1){ // Checks Aces
      int i = checkAce(dealerValues);
      dealerValues[i] = 1;
      dealerSum = getSum(dealerValues); 
    }
        
    if(dealerSum > 21){ // Dealer Busts
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
    cout << "Player Busted, Dealer Wins" << endl; // Bet already removed
  }
  else if (dealerBust){
    cout << "Dealer Busted, Player Wins" << endl;
    if (blackJack){
      wallet += bet + 1.5 * bet; // Payout is 1.5x
    }
    else{
      wallet += 2 * bet; // Return bet
    }
    
  }
  else if (dealerSum > playerSum){
    cout << "Dealer Wins: " << dealerSum << " > " << playerSum << endl; // Bet already removed
  }
  else if (playerSum > dealerSum){
    cout << "Player Wins: " << playerSum << " > " << dealerSum << endl;
    if (blackJack){
      wallet += bet + 1.5 * bet; // Payout is 1.5x
    }
    else{
      wallet += 2*bet; // Return bet
    }
    
  }
  else { // Tie
      cout << "It's a tie!" << endl;
      wallet += bet; // Push
  }
  cout << endl << "Current Amount in Wallet: $" << wallet << endl << endl;
  
  string play;
  cout << "Play Again? (Y/N): "; // Read in to continue
  cin >> play;
  cout << endl;
  return (play == "Y");
}

int main(){
  double wallet;
  cout << "Enter Starting Money: $"; 
  cin >> wallet;
  bool newGame;
  newGame = game(wallet);
  while (newGame && wallet > 0){ // Wallet must contain money
    newGame = game(wallet);
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
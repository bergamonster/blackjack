#include <iostream>
#include <vector>
#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <cstring>
#include <string>
#include <sstream>
#include <thread>
#include <chrono>

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
string getSuit(int card){
  int counter = card/52; // Figure out card in deck
  card = card - counter*52;
  if (card < 13){
    return "Heart";
  }
  else if (card < 26){
    return "Diamond";
  }
  else if (card < 39) {
    return "Club";
  }
  else if (card < 52){
    return "Spade";
  }
  else{
    return "Error";
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

vector<string> displayCard(int card, bool cover){
  int val = getValue(card, false);
  stringstream ss;
  string suit = getSuit(card);
  string name;
  string line1,line2,line3,line4,line5,line6,line7;
  if (val > 1 && val < 11){
    ss << val; // Converting int to string
    ss >> name;
  }
  else if (val == 1){
    name =  'A';
  }
  else if (val == 11){
    name = 'J';
  }
  else if (val == 12){
    name = 'Q';
  }
  else if (val == 13){
    name = 'K';
  }
  else {
    name = '!';
  }
  
  line1 = " _____ ";
  
  //ASCII Art inspiration from ejm98
  if (cover){
    line2 = "|X    |";
    line3 = "|XXXXX|";
    line4 = "|XXXXX|";
    line5 = "|XXXXX|";
    line6 = "|XXXXX|";
    line7 = "|____X|";
    
  }
  else {
    line2 = "|" + name;
    if (val != 10){
      line2 += " ";
    }
    line2 += "   |";
  
    if (suit == "Heart"){
      line3 = "| _ _ |";
      line4 = "|( v )|";
      line5 = "| \\ / |";
      line6 = "|  .  |";  
    }
    else if (suit == "Diamond"){
      line3 = "|  ^  |";
      line4 = "| / \\ |";
      line5 = "| \\ / |";
      line6 = "|  .  |";
    }
    else if (suit == "Club"){
      line3 = "|  _  |";
      line4 = "| ( ) |";
      line5 = "|(_\'_)|";
      line6 = "|  |  |";
    }
    else if (suit == "Spade"){
      line3 = "|  .  |";
      line4 = "| /.\\ |";
      line5 = "|(_._)|";
      line6 = "|  |  |";
    }
    else{
      cout << "Error" << endl;
    }
    line7 =  "|___";
    if (val != 10){
      line7 += "_";
    }
    line7 += name + "|";
  }
  vector<string> lines;
  lines.push_back(line1);
  lines.push_back(line2);
  lines.push_back(line3);
  lines.push_back(line4);
  lines.push_back(line5);
  lines.push_back(line6);
  lines.push_back(line7);
  return lines;
}

void displayHand(vector<int> hand, bool cover){
  string line1,line2,line3,line4,line5,line6,line7;
  vector<string> lines;
  for (int i = 0; i < hand.size(); ++i){
    if (cover && i == 1){
      lines = displayCard(hand[i], true);
    }
    else {
      lines = displayCard(hand[i], false);
    }
    line1 += lines[0] + "  ";
    line2 += lines[1] + "  ";
    line3 += lines[2] + "  ";
    line4 += lines[3] + "  ";
    line5 += lines[4] + "  ";
    line6 += lines[5] + "  ";
    line7 += lines[6] + "  ";
  }
  cout << line1 << endl << line2 << endl << line3 << endl << line4 << endl;
  cout << line5 << endl << line6 << endl << line7 << endl;
}

// Single round of blackjack
int game(double& wallet){
  srand(time(NULL)); // Set seed
  int numDecks = 6;
  vector<int> cards; // Total deck
  vector<int> playerHand; // Exact cards in player hand
  vector<int> dealerHand;  // Exact cards in dealer hand
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
  
  double bet;
  string input;
  cin.clear();
  cout << "Enter Bet: $"; 
  getline(cin, input);
  stringstream ss(input); // Used to make sure input is completely numeric
  ss >> bet;
  while (bet <= 0 || !ss.eof() || bet > wallet){
    ss.str("");  // Clear input
    ss.clear(); // Reset flags
    cin.clear();  // Reset flags
    cout << "Bet Must Be Positive and No More Than Wallet" << endl << "Enter Bet: $";
    getline(cin, input);
    ss << input;
    ss >> bet;
  }
  ss.str("");  // Clear input
  ss.clear();  // Reset flags
  cin.clear();  // Reset flags
  
  wallet -= bet;  // Remove bet from wallet
  
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
  cout << endl << "Dealer's Hand: " << endl;
  //printCard(dealerHand[0]);
  cout << endl;
  displayHand(dealerHand, true);
  cout << endl << endl;
  
  // Display Player Cards
  cout << "Your Hand: " << endl;
  /*
  printCard(playerHand[0]); 
  cout << ", ";
  printCard(playerHand[1]);
  cout << endl;
  */
  displayHand(playerHand, false);
  cout << endl << "Hand Total: " << playerSum;
  cout << endl << endl;
  
  bool dealerBust = false;
  bool playerBust = false;
  
  if(playerSum == 21){
    blackJack = true;
  }
  
  cout << "Accepted Commands are Quit, Hit, and Stand" << endl;
  
  // Player's Turn
  string message;
  if (!blackJack){ // Auto pass to Dealer turn if blackjack
    cin >> ws;
    while(cin >> message){
      transform (message.begin(), message.end(), message.begin(), ::tolower);
      cin.clear();
      if (message == "quit"){ // Quit from game
        return 0;
      	}
      else if (message == "hit"){
        cout << "\n\n\n\n\n\n\n\n\n";
        cout << endl;
        int newCard = cards[currentCard++];
        playerHand.push_back(newCard);
        playerValues.push_back(getValue(newCard, true));
        playerSum = getSum(playerValues);
        
        cout << "Dealer\'s Hand: " << endl;
        displayHand(dealerHand, true);
        /*
        cout << endl << endl << "You Were Dealt: " << endl;
        printCard(playerHand[playerHand.size()-1]);
        cout << endl;
        */
        cout << endl << endl;
        cout << "Your Hand: " << endl;
        displayHand(playerHand, false);
        
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
      else if (message == "stand"){
        cout << "\n\n\n\n\n\n\n\n\n";
        break;
      }
      else{
        cout << "Accepted Commands are Quit, Hit, and Stand" << endl;
      }
    }
  }
  
  // Dealer's Turn
  //cout << endl << "Dealer\'s Covered Card: " << endl;
  //printCard(dealerHand[1]); // Reveal dealer's card
  
  cout << "Dealer\'s Hand: " << endl;
  displayHand(dealerHand, false);
  cout << endl << "Hand Total: " << dealerSum << endl << endl;

  cout << "Your Hand: " << endl;
  displayHand(playerHand, false);
  cout << endl << "Hand Total: " << playerSum << endl << endl;
  cout << endl;
  
  this_thread::sleep_for (chrono::seconds(2)); // Pause before Dealer's turn
  
  while (dealerSum < 17 && !playerBust){ // Dealer only hits up to 17 and if the player hasn't busted
    cout << "\n\n\n\n\n\n\n\n\n";
    cout << "Dealer Hits" << endl;
    int newCard = cards[currentCard++];
    dealerHand.push_back(newCard);
    dealerValues.push_back(getValue(newCard, true));
    dealerSum = getSum(dealerValues);
    //printCard(dealerHand[dealerHand.size()-1]);
    
    cout << endl;
        
    while (dealerSum > 21 && checkAce(dealerValues) != -1){ // Checks Aces
      int i = checkAce(dealerValues);
      dealerValues[i] = 1;
      dealerSum = getSum(dealerValues); 
    }
        
    if(dealerSum > 21){ // Dealer Busts
      dealerBust = true;
      cout << "Dealer\'s Hand: " << endl;
      displayHand(dealerHand, false);
      cout << endl << "Hand Total: " << dealerSum << endl << endl;
    
      cout << "Your Hand: " << endl;
      displayHand(playerHand, false);
      cout << endl << "Hand Total: " << playerSum << endl << endl;
      break;
      }
      
    cout << "Dealer\'s Hand: " << endl;
    displayHand(dealerHand, false);
    cout << endl << "Hand Total: " << dealerSum << endl << endl;

    cout << "Your Hand: " << endl;
    displayHand(playerHand, false);
    cout << endl << "Hand Total: " << playerSum << endl << endl;
    
    this_thread::sleep_for (chrono::seconds(2)); // Pause after each of Dealer's turns
  } 
  
  if (!playerBust && ! dealerBust){
    cout << "\n\n\n\n\n\n\n";
    cout << "Dealer Stands" << endl << endl;
    cout << "Dealer\'s Hand: " << endl;
    displayHand(dealerHand, false);
    cout << endl << "Hand Total: " << dealerSum << endl << endl;
    
    cout << "Your Hand: " << endl;
    displayHand(playerHand, false);
    cout << endl << "Hand Total: " << playerSum << endl << endl;
  }
  
  this_thread::sleep_for (chrono::seconds(1)); // Pause before results
  
  // Print results
  cout << "\n\n\n\n\n\n\n\n\n\n\n\n\n\n";
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
  cout << "\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n";
  
   
  string play;
  string trash;
  cout << "";
  getline(cin, play); // Get rid of extra input
  cout << "Play Again? (Y/N): "; // Read in to continue
  getline(cin, play);
  transform (play.begin(), play.end(), play.begin(), ::tolower);
  while (play != "y" && play != "n"){
    cin.clear();
    cout << "Please Enter Y or N: ";
    getline(cin, play);
  }
  cin.clear();
  cout << endl;
  return (play == "y");
  
}

int main(){
  double wallet;
  string input;
  cout << "Enter Starting Money: $"; 
  getline(cin, input);
  stringstream ss(input); // Used to make sure input is completely numeric
  ss >> wallet;
  while (wallet <= 0 || !ss.eof()){
    ss.str("");  // Clear input
    ss.clear(); // Reset flags
    cin.clear();  // Reset flags
    cout << "Starting Money Must Be Positive" << endl << "Enter Starting Money: $";
    getline(cin, input);
    ss << input;
    ss >> wallet;
  }
  ss.str("");  // Clear input
  ss.clear();  // Reset flags
  cin.clear();  // Reset flags
  
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
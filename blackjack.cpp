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

//Guide to set screen size for optimal graphics
void setScreen(){
  string trash;
  cout << "----------------------------------------------------------------------------------" << endl;
  for (int i = 0; i < 23; ++i){
    if (i == 10){
      cout << "Please set screen size so you can see the above and below lines, then press Enter" << endl;
    }
    else{
      cout << "|" << endl;
    }
  }
  cout << "----------------------------------------------------------------------------------" << endl;
  getline(cin, trash);
  cout << string(24, '\n'); // Clear screen
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

// Gets value of card, J (11), Q (12), K (13)
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

// Returns suit of car
string getSuit(int card){
  int counter = card/52; // Figure out card in deck
  card = card - counter*52;
  if (card < 13){
    return "Heart";
  }
  else if (card <26){
    return "Diamond";
  }
  else if (card <39) {
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
  vector<int> playerHandSplit; // Exact cards in PLAYER hand if they split
  bool blackJack = false;
  bool blackJackSplit = false;
  
  // Deals with ace value
  vector<int> playerValues;  // Values of cards in player hand
  vector<int> dealerValues;  // Values of cards in dealer hand
  vector<int> playerValuesSplit; //Values of cards in case of split
  
  
  int playerSum;  // Total sum of player hand
  int dealerSum;  // Total sum of dealer hand
  int playerSumSplit; // Total Sum in case of a split
  vector<string> playerSuit;  // Suit of cards in player hand
  vector<string> dealerSuit;  // Suit of cards in dealer hand
  vector<string> playerSuitSplit; // Suit of cards in split hand
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
  
  cout << string(24, '\n'); // Clear screen
  
  // Display Wallet
  cout << "Wallet after bet: $" << wallet << endl;
  
  // Deal
  playerHand.push_back(cards[currentCard++]);
  dealerHand.push_back(cards[currentCard++]);
  playerHand.push_back(cards[currentCard++]);
  dealerHand.push_back(cards[currentCard++]);
  /*
  playerHand.push_back(0);
  playerHand.push_back(0);
  */
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
  bool playerBustSplit = false;
  
  if(playerSum == 21){
    blackJack = true;
  }
  
  playerSumSplit=0;
  // Following if statement Deals with splits
  string message0;
   if(getValue(playerHand[1],false) == getValue(playerHand[0],false) && wallet >= bet){
  	cout << "Would you like to split?" << endl;
  	cout << "Y/N" << endl;
  	while(cin >> message0){
  	    cout << "\n\n";
  	    transform (message0.begin(), message0.end(), message0.begin(), ::tolower);
        if (message0 == "y"){ //Split
        playerHandSplit.push_back(playerHand[1]);
        playerHand.pop_back();
        playerHand.push_back(cards[currentCard++]);
        playerHandSplit.push_back(cards[currentCard++]);
        playerValues.clear();
        dealerValues.clear();
  		// Determine Suits
  		for (const auto &card : playerHand){
    		playerSuit.push_back(getSuit(card));
  		}
  		for (const auto &card : dealerHand){
    		dealerSuit.push_back(getSuit(card));
  		}
  		for (const auto &card : playerHandSplit) {
  			playerSuitSplit.push_back(getSuit(card));
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
  		for (const auto &card : playerHandSplit){
    		if (getValue(card, false) == 1){
      			playerValuesSplit.push_back(11); // Set Aces initially to 11
   			 }
    		else if (getValue(card, false) > 10){
      			playerValuesSplit.push_back(10); // Set face card value to 10
    		}
   			else{
      			playerValuesSplit.push_back(getValue(card, false));
    		}
  		}
  	// Determine Sums
  	playerSum = 0;
  	dealerSum = 0;
  	playerSumSplit = 0;
  	playerSum = getSum(playerValues);
  	dealerSum = getSum(dealerValues);
  	playerSumSplit = getSum(playerValuesSplit);
  		//Print Results
        cout << "First Hand: " << endl;
      displayHand(playerHand, false);
      cout << endl << "Hand Total: " << playerSum << endl << endl;
    
      cout << "Second Hand: " << endl;
      displayHand(playerHandSplit, false);
      cout << endl << "Hand Total: " << playerSumSplit << endl << endl;
      cout << "You Will Now Play Your First Hand" << endl;
      break;
    	}  	
        else if (message0 == "n"){
        cout << string(7, '\n');
        cout << endl;
        break;
        }
        else{
        cout << "Please enter either \'Y\' or \'N\' " << endl;
        }
}
}
    	cout << "(Accepted Commands are Quit, Hit, and Stand)" << endl;
  
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
        if (playerSum == 21) {
        cout << "You Got Blackjack" << endl;
        break;
		}
        cout << "Hand Total: " << playerSum << endl << endl;
      }
      else if (message == "stand"){
      	cout << "\n\n\n\n\n\n\n\n\n";
		break;
		}
		else if (message == "quit"){
        break;
    	}

      else{
        cout << "Accepted Commands are Quit, Hit, and Stand" << endl;
      }
    }
  }
  
    
  // Possible Turn for Split
  if (playerSumSplit >= 2) {
  	cout << "You Will Now Play Your Second Hand: " << endl;
  	cout << "Your Second Hand is:" << endl;
       // Display Player Cards
  //cout << "Your Hand: " << endl;
  /*
  printCard(playerHand[0]); 
  cout << ", ";
  printCard(playerHand[1]);
  cout << endl;
  */
  displayHand(playerHandSplit, false);
  cout << endl << "Hand Total: " << playerSumSplit;
  cout << string(12, '\n');
  cout << endl << endl;
  	
   string message;
  if (!blackJackSplit){ // Auto pass to Dealer turn if blackjack
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
        playerHandSplit.push_back(newCard);
        playerValuesSplit.push_back(getValue(newCard, true));
        playerSumSplit = getSum(playerValuesSplit);
        
        cout << "Dealer\'s Hand: " << endl;
        displayHand(dealerHand, true);
        /*
        cout << endl << endl << "You Were Dealt: " << endl;
        printCard(playerHand[playerHand.size()-1]);
        cout << endl;
        */
        cout << endl << endl;
        cout << "Your Hand: " << endl;
        displayHand(playerHandSplit, false);
        
        cout << endl;
      
        while (playerSumSplit > 21 && checkAce(playerValuesSplit) != -1){ // Makes sure no Ace values could change
          int i = checkAce(playerValues);
          playerValuesSplit[i] = 1;
          playerSumSplit = getSum(playerValues); 
          }
        if(playerSumSplit > 21){ // Player Busts
          cout << "Hand Total: " << playerSumSplit << endl << endl;
          cout << "Bust" << endl << endl;
          playerBustSplit = true;
          break;
        }
        if (playerSumSplit == 21){
        	cout << "You Got Blackjack" << endl;
        	break;
		}
        cout << "Hand Total: " << playerSumSplit << endl << endl;
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
  
  this_thread::sleep_for (chrono::seconds(2)); // Pause before results
  
  if (playerSumSplit < 2){
   // Print results
   cout << "\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n";
  //cout << "***************************************" << endl << endl;
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
  cout << "\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n";
}



else if (playerSumSplit >= 2){
	//Print Results for split
	cout << "\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n";
if (playerBust){
    cout << "Player Busted Hand 1, Dealer Wins Half of Bet" << endl; // Bet already removed
  }
if (playerBustSplit){
cout << "Player Split Busted, Dealer Wins Half of Bet" << endl; // Bet already removed
}
if (dealerBust){
	if (!playerBust && !playerBustSplit){
	cout << "Dealer Busted, Player Wins" << endl;
		if (blackJack){
			wallet += bet + .75 * bet; // Payout is 1.5x
		}
		if (blackJackSplit){
			wallet += bet + 0.75*bet;
		}
		if (!blackJack && !blackJackSplit){
			wallet += 2 * bet; // Return bet
		}
	}
	else if (playerBust && playerBustSplit){
		cout << "Everyone Busted" << endl;}
	else if (playerBust || playerBustSplit){
		cout << "Dealer Busted, ";
		if (playerBust){
			cout << "Player Split wins";
		}
		if (playerBustSplit){
			cout << "Player Hand 1 wins";
		}
		
	}
}
if (dealerSum > playerSum && !dealerBust){
    cout << "Dealer Wins against Hand 1: " << dealerSum << " > " << playerSum << endl; // Bet already removed
  }
else if (playerSum > dealerSum && !playerBust){
cout << "Player Wins with Hand 1: " << playerSum << " > " << dealerSum << endl;
    if (blackJack){
      wallet += bet + .75 * bet; // Payout is 1.5x
    }
    else{
      wallet += bet; // Return bet
}
}

if (dealerSum > playerSumSplit && !dealerBust){
    cout << "Dealer Wins against Hand 2: " << dealerSum << " > " << playerSumSplit << endl; // Bet already removed
  }
else if (playerSumSplit > dealerSum && !playerBustSplit){
cout << "Player Wins with Hand 2: " << playerSumSplit << " > " << dealerSum << endl;
    if (blackJackSplit){
      wallet += bet + .75 * bet; // Payout is 1.5x
    }
    else{
      wallet += bet; // Return bet
}
}
  if (playerSum == dealerSum && !dealerBust) { // Tie
      cout << "It's a tie! (Hand 1)" << endl;
      wallet += bet; // Push
  }
  if (playerSumSplit == dealerSum && !dealerBust){ // Tie
      cout << "It's a tie! (Hand 2)" << endl;
      wallet += bet; // Push
  }
  cout << endl << "Current Amount in Wallet: $" << wallet << endl << endl;
  cout << "\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n";
  
}	
	

  string play;
  string trash;
  cout << "";
  getline(cin, play); // Get rid of extra input
    if (wallet > 0){
  cout << "Play Again? (Y/N): "; // Read in to continue
  }
  else {
  cout << "End of Game, Wallet Empty" << endl;
  return 0;
  }
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
  setScreen();
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
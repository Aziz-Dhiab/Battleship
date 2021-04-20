#include<iostream>
#include<iomanip>
#include<windows.h>
#include<conio.h>
#include<string>
#include<ctime>
#include<cstdlib>
#include <list>
#include <tuple>
#include <iterator>
using namespace std;
const int title_row = 33;
const int title_col = 40;
const int options_row = 4;
const int options_col = 16;




class Boat{
    friend class Board;
    friend class Player;
    private:
    int length;
    int width;
    int coordinates[20][3]; //represents a boat coordinates when on the board (x,y,hit or not)
    int rotation;
    string state;
    int HitCells;
    int placed; //to know if the boat is placed yet or not
    public:
    //Boat constructor, by default the width is 1
    Boat(int l, int w = 1){
        length = l;
        width = w;
        state = "unhit";
        rotation = 0; //0 means the rotation is horizontal
        placed = 0;
        HitCells = 0;
    }
    //update the boat after a hit in (x,y) coordinates and return the result of the hit if it landed on the boat
    string Update(int x, int y, string s){
        if (s == ""){
            for (int i = 0; i < length * width; i++){
                if (coordinates[i][0] == x && coordinates[i][1] == y){
                    coordinates[i][2] = 1;
                    state = "hit";
                    s = "Hit";
                    HitCells++;
                    if (HitCells == length * width){
                        state = "dead";
                        s = "Sunk";
                        return s; // to break out of the method
                    }
                }
            }
        }
        return s;
    }
    //display the boat either before or after it's placed on the board
    void display(){
        string s = "";
        if (!placed){
            if(rotation == 0){
                for (int i = 0; i < width; i++){
                    for (int j = 0; j < length; j++){
                        cout << "O"//"O" if on windows shell  //windows shell doesn't display UTF-8/unicode characters
                            << " ";
                    }
                    cout << endl;
                }
            }else{
                for (int j = 0; j < length; j++){
                    for (int i = 0; i < width; i++){
                        cout <<setw(88)<< "O"//"O" if on windows shell  //windows shell doesn't display UTF-8/unicode characters
                            << " ";
                    }
                    cout << endl;
                }
            }
        }else{ //(coordinates[0][0],coordinates[0][1]) are the top left coordinates of a placed boat
            int x = coordinates[0][0];
            int y = coordinates[0][1];
            if (rotation == 0){
                for (int i = x; i < x + width; i++){
                    for (int j = y; j < y + length; j++){
                        string s = "O";//"O" if on windows shell
                        for (int k = 0; k < width * length; k++){
                            if (coordinates[k][0] == i && coordinates[k][1] == j && coordinates[k][2] == 1){
                                s = (char)232;//"Ø" if on windows shell
                            }
                        }
                        cout << s << " ";
                    }
                    cout << endl;
                }
                cout <<setw(94)<< state << endl;
            }else{ //to test if the boat is vertical
                for (int j = y; j < y + width; j++){      //or swap the 2 loops if you want to display the boats as vertical
                    for (int i = x; i < x + length; i++){ //
                        string s = "O";//"O" if on windows shell
                        for (int k = 0; k < width * length; k++){
                            if (coordinates[k][0] == i && coordinates[k][1] == j && coordinates[k][2] == 1){
                                s = (char)232;//"Ø" if on windows shell
                            }
                        }
                        cout << s << " ";
                    }
                    cout << endl;
                }
                cout <<setw(94)<< state << endl;
            }
        }
    }
};
// represents a board with the boats in it (can represents both a player's or an opponent's board)
class Board{
    friend class Player;
    private:
    int MaxLength;
    int MaxWidth;
    int NbrBoats;
    list<Boat> BoatList;
    int grid[20][20][2]; // grid the represents the board (x,y,(includeboat,hit)) default grid is 8x8
    public:
    // Constructor with the default MaxLength and MaxWidth equals 8
    Board(int l = 8, int w = 8){
        MaxLength = l;
        MaxWidth = w;
        for (int i = 0; i < MaxLength; i++){
            for (int j = 0; j < MaxWidth; j++){
                grid[i][j][0] = 0;
                grid[i][j][1] = 0;
            }
        }
    }
    //assumes (x,y) are valid coordinates to strike, update the board after a strike in the (x,y) coordinates
    string Strike(int x, int y){
        string s = "";
        grid[x][y][1] = 1;
        for (auto iter = BoatList.begin(); iter != BoatList.end(); iter++){ //iterate over the BoatList and update the boats one by one
            s = iter->Update(x, y, s);
        }
        if (s == ""){
            s = "Miss";
        }
        return s;
    }
    //verify you can place the boat in the (x,y) coordinates where (x,y) are the top left coordinates
    int VerifyBoatPlacement(int x, int y, Boat b){
        if (b.rotation == 0){ // the boat is horizontal in its rotation
            if (x + b.width > MaxLength){
                return 0;
            }
            if (y + b.length > MaxWidth){
                return 0;
            }
            for (int i = x; i < x + b.width; i++){ //verify the boat is not colliding with any other boat
                for (int j = y; j < y + b.length; j++){
                    if (grid[i][j][0] == 1){
                        return 0;
                    }
                }
            }
        }
        if (b.rotation == 1){ // the boat is vertical in its rotation
            if (x + b.length > MaxWidth){
                return 0;
            }
            if (y + b.width > MaxLength){
                return 0;
            }
            for (int i = x; i < x + b.length; i++){ //verify the boat is not colliding with any other boat
                for (int j = y; j < y + b.width; j++){
                    if (grid[i][j][0] == 1){
                        return 0;
                    }
                }
            }
        }
        return 1;
    }
    //takes a boat and places it where (x,y) are the top left coordinates and add it to the list of placed boats
    void PlaceBoat(int x, int y, Boat b){
        b.placed = 1;
        if (b.rotation == 0){ //the boat is horizontal
            int k = 0;
            for (int i = x; i < x + b.width; i++){
                for (int j = y; j < y + b.length; j++){
                    grid[i][j][0] = 1;
                    b.coordinates[k][0] = i; // add (i,j) coordinates to the boat and initialize it to not hit
                    b.coordinates[k][1] = j; //
                    b.coordinates[k][2] = 0; //
                    k++;
                }
            }
        }
        else{ //the boat is vertical
            int k = 0;
            for (int i = x; i < x + b.length; i++){
                for (int j = y; j < y + b.width; j++){
                    grid[i][j][0] = 1;
                    b.coordinates[k][0] = i; // add (i,j) coordinates to the boat and initialize it to not hit
                    b.coordinates[k][1] = j; //
                    b.coordinates[k][2] = 0; //
                    k++;
                }
            }
        }
        BoatList.push_back(b);
    }
    //display a player's board with the boats in it and the hits
    void display(){
        char letter = 'A';

        cout <<setw(89); //first line that contains numbers
        for (int k = 0; k < MaxWidth; k++){
            cout << k + 1 << "   ";
        }
        cout << endl;

        cout <<setw(87)<<(char)201; //top part of the grid
        for (int k = 0; k < MaxWidth - 1; k++){
            cout << (char)205<< (char)205<< (char)205<< (char)203;
        }
        cout << (char)205<< (char)205<< (char)205<< (char)187 << endl;

        for (int i = 0; i < MaxLength; i++){
            cout <<setw(85)<< char(letter) << " ";
            letter = char(letter + 1);

            for (int j = 0; j < MaxWidth; j++){
                string s = " ";                                   //represents unhit cell that doesn't contain a bot
                if (grid[i][j][0] == 1){
                    s = "O";          //"O" if on windows shell  //represnets unhit cell that contains a boat
                    if (grid[i][j][1] == 1){
                        s = (char)232;   //"Ø" if on windows shell          //represnets hit cell that contains a boat"ø×Ø•θΘΦΟOₒ"
                    }
                }else{
                    if (grid[i][j][1] == 1){
                        s = (char)88;                                  //represnets hit cell that doesn't contain a bot
                    }
                }
                cout << (char)186<<" " << s << " ";
            }
            cout << (char)186<<" " << endl;
            if (i < MaxLength - 1){   //condition so you don't draw an extra line
                cout <<setw(87)<< (char)204;
                for (int l = 0; l < MaxWidth - 1; l++){
                    cout << (char)205<< (char)205<< (char)205<< (char)206;
                }
                cout << (char)205<< (char)205<< (char)205<< (char)185 << endl;
            }
        }

        cout <<setw(87)<<(char)200; //bottom part of the grid
        for (int k = 0; k < MaxWidth - 1; k++){
            cout << (char)205<< (char)205<< (char)205<< (char)202;
        }
        cout<< (char)205<< (char)205<< (char)205 << (char)188 << endl;
    }
    //display an opponent's board with only the hits and the damaged cells
    void OpponentDisplay(){
        cout <<setw(89); //first line that contains numbers
        char letter = 'A';
        for (int k = 0; k < MaxWidth; k++){
            cout << k + 1 << "   ";
        }
        cout << endl;

        cout <<setw(87)<< (char)201; //top part of the grid
        for (int k = 0; k < MaxWidth - 1; k++){
            cout << (char)205<< (char)205<< (char)205<< (char)203;
        }
        cout << (char)205<< (char)205<< (char)205<< (char)187 << endl;

        for (int i = 0; i < MaxLength; i++){
            cout <<setw(85)<< char(letter) << " ";
            letter = char(letter + 1);
            for (int j = 0; j < MaxWidth; j++){
                string s = " ";              //empty cell
                if (grid[i][j][1] == 1){     //if cell is hit
                    if (grid[i][j][0] == 0){ //if the cell doesn't contain a bot
                        s = (char)88;
                    }else{                   //if the cell contains a boat
                        s = (char)232;//"Ø" if on windows shell
                    }
                }
                cout << (char)186 <<" "<< s << " ";
            }
            cout << (char)186 <<" "<< endl;
            if (i < MaxLength - 1){ //condition so you don't draw an extra line
                cout <<setw(87)<<(char)204;
                for (int l = 0; l < MaxWidth - 1; l++){
                    cout << (char)205<< (char)205<< (char)205<< (char)206;
                }
                cout<< (char)205<< (char)205<< (char)205 << (char)185 << endl;
            }
        }

        cout <<setw(87)<< (char)200; //bottom part of the grid
        for (int k = 0; k < MaxWidth - 1; k++){
            cout << (char)205<< (char)205<< (char)205<< (char)202;
        }
        cout << (char)205<< (char)205<< (char)205<< (char)188 << endl;
    }
};
// represents one of two players in a given game
class Player{
    private:
    int Turn;
    int MaxLength;
    int MaxWidth;
    int NbrOfBoatsInInventory;
    int BoatsAlive;
    list<Boat> inventory; //represents the list of boat before they are placed on the board
    Board YourBoard;
    Board OpponentBoard;
    public:
    void SetMaxLength(int i){
        MaxWidth = i;
    }
    void SetMaxWidth(int i){
        MaxWidth = i;
    }
    //special setter for the turn attribute that can only set it to 1
    void BeginTurn(){
        Turn = 1;
    }
    int GetBoatsAlive() const {
        return BoatsAlive;
    }
    //default constructor
    Player(){
        Turn = 0;
        NbrOfBoatsInInventory = 0;
        BoatsAlive = 0;
    }
    //parametrised constructor that adds the default boats to the player (both classic and belgium)
    Player(string s){
        Turn = 0;
        MaxLength = 8;
        MaxWidth = 8;
        NbrOfBoatsInInventory = 0;
        BoatsAlive = 0;
        if (s == "classic"){ //one length 5 bot, one length 4 boat, two length 3 boats, one length 2 boat
            Boat b(5);
            AddBoat(b);
            b = Boat(4);
            AddBoat(b);
            b = Boat(3);
            AddBoat(b);
            AddBoat(b);
            b = Boat(2);
            AddBoat(b);
        }else if (s == "belgium"){ //one length 4 boat, two length 3 boats, three length 2 boats, four length 1 boats
            Boat b(4);
            AddBoat(b);
            b = Boat(3);
            AddBoat(b);
            AddBoat(b);
            b = Boat(2);
            AddBoat(b);
            AddBoat(b);
            AddBoat(b);
            b = Boat(1);
            AddBoat(b);
            AddBoat(b);
            AddBoat(b);
            AddBoat(b);
        }else if (s == "test"){
            Boat b(2);
            AddBoat (b);
        }
    }
    void AddBoat(Boat b){
        inventory.push_back(b);
        NbrOfBoatsInInventory ++;
    }
    int InventoryIsEmpty(){
        return inventory.empty();
    }
    void ShowInventory(){
        cout<<endl<<endl<<setw(122)<< "------ The Boats In Your Inventory ------" << endl<<endl;
        int index = 1;

        for (auto iter = inventory.begin(); iter != inventory.end(); iter++)
        { //iterate over the boats in the inventory

            cout<<setw(89);
            iter->display();
            cout<<setw(100) << "Boat number " << index <<":"<< endl;
            index++;

        }
    }
    //takes the chosen boat and put it in the front of the list in the inventory (and rotate it if needed)
    void SelectBoat(int index, int rot = 0){
        auto iter = inventory.begin();
        for (int i = 0; i < index - 1; i++){
            iter++;
        }
        inventory.erase(iter);
        if (rot == 1){
            iter->rotation = 1;
        }
        inventory.push_front(*iter);
    }
    void ShowSelectedBoat(){
        inventory.front().display();
    }
    // verify the player's input is in the right form i.e E5 , B7
    int VerifyInput(string s){
        int a = int('a');
        int A = int('A');
        char c = s[0];
        int letter = int(c);
        if (s.length() != (MaxWidth / 10) + 2){ //verify the length of the input (its determined by the width of the board)
            return 0;
        }
        string s1 = s.substr(1);
        if (!(s1.find_first_not_of("0123456789") == -1)){ //verify the rest of the string is numbers // s.find_first_not_of(s1) returns -1 if s only contain chars from s1
            return 0;
        }
        int numbers = stoi(s1);
        if (!(numbers > 0 && numbers <= MaxWidth)){ //verify the numbers are within the appropriate range
            return 0;
        }
        if (!((letter >= a && letter < a + MaxLength) || (letter >= A && letter < A + MaxLength))){ // verify the letter is within the range of letters i.e [a..g]
            return 0;
        }
        return 1;
    }
    // transform the input into the grid coordinates i.e "B4" -> (1,3)
    tuple<int, int> TransformInput(string s){ //
        int A = int('A');
        int c = toupper(s[0]) + 1; //make the letter uppercase and get it's ascii code
        c -= A;
        int numbers = stoi(s.substr(1));
        return make_tuple(c - 1, numbers - 1);
    }
    //verify you can place the boat in the (x,y) coordinates where (x,y) are the top left coordinates
    int VerifySelectedBoatPlacement(int x, int y){
        Boat b = inventory.front();
        return YourBoard.VerifyBoatPlacement(x,y,b);
    }
    void PlaceBoatFromInventory(int x, int y){
        Boat b = inventory.front();
        inventory.pop_front();
        YourBoard.PlaceBoat(x, y, b);
        NbrOfBoatsInInventory --;
        BoatsAlive ++;
    }
    //allows a player to place his boats from his inventory to his board one by one using controlled keyboard inputs
    void PlaceTheBoats(){

        string s;
        int i;
        tuple<int, int> t;
        while (!InventoryIsEmpty()){
            DisplayBoard();
            ShowInventory();
            cout << endl
                 <<setw(122)<< "Enter the number of the boat to place : ";
            while (1){
                cin >> s;
                if ((s.find_first_not_of("0123456789") == -1)){ //the input is a number
                    i = stoi(s);
                    if (i >= 1 && i <= NbrOfBoatsInInventory){
                        break;
                    }
                }
                cout<<endl<<setw(122) << "Not a valid number, please try again : ";
            }
            cout <<endl<<setw(135) << "Do you want to rotate the boat ? enter 1 for yes and 0 for no : ";
            do{
                cin >> s;
                if (s != "0" && s != "1"){
                    cout<<endl<<setw(122) << "Not a valid input, please try again : ";
                }
            }while (s != "0" && s != "1");
            SelectBoat(i, stoi(s));
            printf("\033c"); //clears the console (linux) system("CLS"); for windows
            DisplayBoard();
            cout <<endl<<endl<<setw(115)<< "This is your selected Boat !" << endl<<endl;
            cout<< setw(88);
            ShowSelectedBoat();
            cout << endl<<endl
                 <<setw(140)<< "Please enter the coordinates of the top-left corner of the boat : ";
            while (1){
                cin >> s;
                if (VerifyInput(s)){ //the input is in the right form
                    t = TransformInput(s);
                    if (VerifySelectedBoatPlacement(get<0>(t), get<1>(t))){
                        break;
                    }
                }
                cout <<setw(122)<< "Invalid coordinates, please try again : ";
            }
            PlaceBoatFromInventory(get<0>(t), get<1>(t));
            printf("\033c");
        }
        DisplayBoard();
    }
    //allow a player(current object) to strike another player (object in parameters) and return the result of his strike
    string Strike(int x, int y, Player &p){
        string s;
        s = OpponentBoard.Strike(x,y);
        p.YourBoard.Strike(x,y);
        if (s == "Miss"){
            Turn = 0;
        }else if (s == "Sunk"){
            p.BoatsAlive --;
        }
        return s;
    }
    //verify the cell wasn't stuck before
    int VerifyStrike(int x, int y){
        return OpponentBoard.grid[x][y][1] == 0;
    }
    //initialize the opponent board for both players after boat placements
    void Initialize(Player &p){
        OpponentBoard = p.YourBoard;
        p.OpponentBoard = YourBoard;
    }
    //allows a player to keep striking his opponent and show the result of previous strikes until he misses or the game ends using controlled keyboard inputs
    string PlayTurn(string Hits,Player &p){
        string s;
        tuple<int, int> t;
        while (Turn == 1){
            printf("\033c");
            display();
            cout<< Hits;
            cout<< "Please enter the coordinates to strike : ";
            while (1){
                cin >> s;
                s[0] = toupper(s[0]);
                if (VerifyInput(s)){ //the input is in the right form
                    t = TransformInput(s);
                    if (VerifyStrike(get<0>(t), get<1>(t))){
                        break;
                    }
                }
                cout << "Invalid coordinates, please try again : ";
            }
            Hits = Hits + Strike(get<0>(t), get<1>(t), p) + " at " + s + "\n";
            if (p.BoatsAlive == 0){break;}
            printf("\033c");
            display();
            cout<< Hits;
        }
        return Hits;
    }
    //show a player's placed boats and their state and the player's and opponent's boards
    void display(){
        system("Color F1");
        cout <<endl<<setw(122)<< "-------------- Your Boats ---------------" << endl<<endl;
        for (auto iter = YourBoard.BoatList.begin(); iter != YourBoard.BoatList.end(); iter++){ //iterate over the boats in player's board list
            cout<<setw(90);
            iter->display();
        }
        cout <<setw(122)<< "--------------- Your Board ---------------" << endl<<endl;
        YourBoard.display();
        cout <<setw(122)<< "---------- Your Opponent's Board ---------" << endl<<endl;
        OpponentBoard.OpponentDisplay();
    }
    void DisplayBoard(){
        system("Color F1");
        cout <<endl<<endl<<setw(122)<< "--------------- Your Board ---------------" << endl;
        YourBoard.display();
    }
};

class BattleShip {
private:

    char options_move;
    int arrow_row = 0, arrow_col = 0;
    char upordown;

	char game_title[title_row][title_col] =
	{
	  {'#','#','#','#','#','#','#','#','#','#','#','#','#','#','#',' ',' ','$',' ',' ',' ',' ',' ',' ',' ',' ','$','$',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',},
	  {'#','#','#','#','#','#','#','#','#','#','#','#','#','#','#',' ',' ','$','$',' ',' ',' ',' ',' ',' ',' ','$','$',' ',' ',' ',' ',' ','$',' ','$',' ','$',' ','$',},
	  {'#','#',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','#','#',' ',' ','$','$',' ','!','!','!','!','!',' ','$','$',' ',' ',' ',' ',' ',' ','$',' ','$',' ',' ','$',},
	  {'#','#','$',' ',' ',' ',' ',' ','$','$',' ',' ',' ','#','#',' ',' ',' ','$',' ',' ',' ','#',' ',' ',' ','$',' ',' ',' ','#',' ','#','!','!','!','!',' ',' ',' ',},
	  {'#','#',' ',' ',' ','$',' ',' ',' ',' ','$',' ','$','#','#','$','#','#','#','#','#',' ','#',' ',' ',' ',' ',' ','$',' ','#',' ','#',' ',' ',' ',' ',' ',' ',' ',},
	  {'#','#',' ',' ','$',' ','$',' ',' ',' ',' ',' ','$','#','#',' ','#',' ',' ',' ','#',' ','#',' ','!','!','!','!','!',' ','#',' ','#',' ','$',' ',' ',' ',' ',' ',},
	  {'#','#',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','#','#','$','#','$',' ',' ','#',' ','#',' ',' ',' ','#',' ',' ',' ','#',' ','#',' ','$',' ','$',' ','$',' ',},
	  {'#','#','#','#','#','#','#','#','#','#','#','#','#','#','#',' ','#',' ','$',' ','#',' ','#',' ',' ',' ','#',' ',' ',' ','#',' ','#',' ',' ','$',' ',' ','$',' ',},
	  {'#','#','!','!','!','!','!','!','!','!','!','!','!','!','!',' ','#',' ','$',' ','#',' ','#',' ',' ',' ','#',' ','$',' ','#',' ','#','!','!','!',' ',' ',' ',' ',},
	  {'#','#','#','#','#','#','#','#','#','#','#','#','#','#','#',' ','#',' ',' ',' ','#',' ','#',' ','$',' ','#',' ',' ','$','#',' ','#','!','!','!',' ',' ',' ',' ',},
	  {'#','#',' ',' ',' ',' ',' ',' ',' ',' ','$',' ',' ','#','#',' ','#','!','!','!','#',' ','#',' ','$',' ','#',' ',' ',' ','#',' ','#',' ',' ',' ',' ',' ',' ',' ',},
	  {'#','#',' ',' ',' ',' ',' ',' ',' ',' ',' ','$',' ','#','#','$','#',' ',' ',' ','#',' ','#',' ',' ',' ','#',' ',' ',' ','#',' ','#','$',' ',' ',' ',' ','$',' ',},
	  {'#','#','$',' ','$','$',' ',' ',' ',' ',' ',' ',' ','#','#','$','#',' ','$',' ','#',' ','#',' ',' ',' ','#',' ',' ','$','#',' ','#',' ','$',' ','$',' ',' ',' ',},
	  {'#','#',' ',' ',' ',' ',' ',' ','$',' ',' ','$',' ','#','#',' ','#',' ',' ',' ','#',' ','#',' ','$',' ','#',' ',' ',' ','#',' ','#',' ',' ',' ',' ',' ',' ',' ',},
	  {'#','#',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','#','#',' ','#',' ',' ',' ','#',' ','#',' ',' ',' ','#',' ','$','$','#',' ','#','!','!','!','!','$',' ',' ',},
	  {'#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','$','#','$',' ',' ','#',' ','#',' ','$',' ','#',' ',' ',' ','#',' ',' ',' ',' ',' ',' ',' ',' ',' ',},
	  {'#','#','#','#','#','#','#','#','#','#','#','#','#','#','#',' ','#',' ',' ','$','#',' ','#',' ',' ',' ','#',' ',' ',' ','#','!','!','!','!','!',' ','$',' ',' ',},
	  {'$',' ',' ','$',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','$',' ',' ',' ',' ',' ',' ','$',' ',' ',' ','$',' ',' ',' ',' ',' ',' ',' ',' ','$',' ','$',},
	  {' ',' ',' ',' ',' ','#','#','!','!','!','!','!','!','!','!','$',' ',' ',' ',' ',' ',' ',' ','!','!','!','!','!','!','!',' ','#','#','#','#','#','#','#',' ',' ',},
	  {' ','$',' ',' ',' ','#','#','!','!','!','!','!','!','!','!',' ','#',' ',' ','$',' ','#',' ',' ',' ',' ','#',' ',' ',' ',' ','#',' ',' ',' ',' ',' ','#',' ',' ',},
	  {' ',' ',' ',' ',' ','#','#',' ',' ',' ',' ',' ','$',' ',' ',' ','#',' ',' ',' ',' ','#',' ',' ',' ',' ','#',' ','$',' ',' ','#','$','$',' ',' ','$','#',' ',' ',},
	  {' ',' ',' ',' ',' ','#','#',' ',' ',' ',' ','$',' ',' ','$','$','#',' ',' ','$',' ','#',' ','$',' ',' ','#',' ',' ',' ',' ','#',' ',' ',' ',' ',' ','#',' ',' ',},
	  {' ','$',' ','$',' ','#','#',' ',' ','$','$',' ',' ',' ',' ',' ','#',' ',' ',' ',' ','#',' ',' ',' ','$','#',' ',' ',' ','$','#','$',' ','$','$',' ','#',' ',' ',},
	  {' ',' ',' ',' ',' ','#','#',' ',' ',' ',' ',' ',' ',' ',' ',' ','#',' ',' ',' ',' ','#',' ','$',' ',' ','#',' ','$',' ',' ','#',' ',' ',' ',' ','$','#',' ',' ',},
	  {' ',' ',' ',' ',' ','#','#','#','#','#','#','#','#','#','#',' ','#',' ',' ','$',' ','#',' ','$',' ',' ','#',' ',' ',' ','$','#','!','!','!','!','!','#',' ','$',},
	  {' ','$',' ','$',' ','#','#','#','#','#','#','#','#','#','#',' ','#','!','!','!','!','#',' ',' ',' ','$','#',' ',' ',' ',' ','#',' ',' ',' ',' ',' ',' ',' ',' ',},
	  {' ',' ','$',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','#','#',' ','#','!','!','!','!','#',' ','$',' ',' ','#',' ','$',' ',' ','#',' ',' ','$',' ','$',' ','$',' ',},
	  {' ','$',' ',' ',' ',' ','$',' ',' ',' ',' ',' ',' ','#','#',' ','#',' ','$',' ',' ','#',' ',' ',' ','$','#',' ','$',' ',' ','#',' ',' ',' ','$',' ',' ',' ',' ',},
	  {' ',' ',' ','$',' ',' ',' ',' ',' ',' ',' ',' ',' ','#','#',' ','#',' ',' ',' ',' ','#',' ','!','!','!','!','!','!','!',' ','#',' ',' ','$',' ',' ',' ',' ',' ',},
	  {' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','#','#',' ','#',' ',' ','$',' ','#',' ',' ',' ',' ',' ',' ',' ',' ',' ','#',' ',' ',' ',' ',' ',' ',' ',' ',},
	  {' ','$',' ',' ','!','!','!','!','!','!','!','!','!','#','#',' ','#',' ',' ','$',' ','#',' ','$',' ',' ',' ',' ',' ',' ','$','#',' ',' ','$','$',' ',' ','$',' ',},
	  {' ',' ',' ',' ','!','!','!','!','!','!','!','!','!','#','#',' ','#',' ','$','$',' ','#',' ','$','$','$',' ',' ',' ',' ',' ','#',' ',' ',' ',' ',' ',' ','$',' ',},
	  {' ',' ','$',' ',' ',' ',' ',' ',' ',' ',' ','$','$',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','$',' ','$','$',' ',' ','$','$',' ',' ',' ',' ',' ',},
	};

	char options[options_row][options_col] =
	{
	  {' ',' ',' ',' ','N','E','W',' ','G','A','M','E',' ',' ',' ',' '},
	  {' ',' ',' ',' ',' ',' ','E','X','I','T',' ',' ',' ',' ',' ',' '},
	};





public:
	void print_game_title() {
		system("Color 3A");
		printf("\n\n\n\n");
		for (int i = 0; i < title_row; i++) {
			cout << setw(83);
			for (int j = 0; j < title_col; j++) {
				if (game_title[i][j] == '#') {
					cout << (char)219;
				}
				else if (game_title[i][j] == '!') {
					cout << (char)178;
				}
				else if (game_title[i][j] == '+') {
					cout << (char)43;
				}
				else if (game_title[i][j] == '@') {
					cout << (char)233;
				}
				else if (game_title[i][j] == '$') {
					cout << (char)173;
				}
				else {
					cout << (char)32;
				}
			}cout << endl;
		}
	}
	void print_options() {
	    cout<<endl<<endl;
		for (int i = 0; i < options_row; i++) {
			cout << setw(90);
			for (int j = 0; j < options_col; j++) {
				if (i == arrow_row && j == arrow_col) {
					cout << (char)175;
				}
				cout << options[i][j];
			}cout << endl;
		}

	}
	void player_chooses_options() {
		while (1) {
			if (kbhit()) {
				options_move = getch();
				if (arrow_row == 0 && arrow_col == 0 && options_move == 13) {
					system("CLS");
					string s;
                    int i;
                    tuple<int, int> t;
                    //setting up and choosing mode
                    Player p1;
                    Player p2;
                    //Sleep(300);
                    printf("\033c");
                    system("CLS");
                    system("Color 0A");
                    for (int i = 0; i < 17; i++) {
                        cout << endl;
                    }
                    cout << setw(123) << "--------- Welcome To Battleship ----------" << endl<<endl;
                    //selecting the play mode
                    cout<< setw(120) << "To play classic mode please enter 1" << endl;
                    cout<< setw(120) << "To play Belgium mode please enter 2" << endl<<endl;
                    cout<< setw(106) << "PICK : ";
                    do {
                        cin >> s;

                        if (s != "1" && s != "2"){
                            cout<< setw(120) << "Not a valid input, please try again" << endl<<endl;
                            cout<< setw(110) << "PICK AGAIN : ";
                        }
                    }while (s != "1" && s != "2");
                    if (s == "1"){
                        p1 = Player("classic");
                        p2 = Player("classic");
                    }else {
                        p1 = Player("belgium");
                        p2 = Player("belgium");
                    }
                    printf("\033c");
                    system("CLS");
                    system("Color 0A");
                    for (int i = 0; i < 17; i++) {
                        cout << endl;
                    }
                    cout<< setw(115) << "It\'s the first player turn" << endl;
                    cout<< setw(114) << "Press any key to continue ";
                    cin >> s;
                    printf("\033c");

                    loadingBar();
                    printf("\033c");

                    p1.PlaceTheBoats();


                    cout <<  endl<<endl<<endl<< setw(117) << "First player\'s turn is finished" << endl;
                    cout << setw(116)<< "Press any key to continue ";
                    cin >> s;
                    printf("\033c");
                    system("Color F1");
                    for (int i = 0; i < 17; i++) {
                        cout << endl;
                    }
                    cout<< setw(115) << "It\'s the second player turn" << endl;
                    cout<< setw(114) << "Press any key to continue ";
                    cin >> s;
                    printf("\033c");


                    p2.PlaceTheBoats();

                    p1.Initialize(p2);

                    printf("\033c");
                    system("Color F1");
                    for (int i = 0; i < 17; i++) {
                        cout << endl;
                    }
                    cout<< setw(115) << "It\'s the first player turn" << endl;
                    cout<< setw(114) << "Press any key to continue ";
                    cin >> s;
                    printf("\033c");

                    string Hits ="";
                    while (1){
                        p1.BeginTurn();
                        if (Hits != ""){
                            printf("\033c");
                            p1.display();
                            cout << setw(123)<<  "-------- Your Opponent's Strikes --------" << endl;
                            cout << setw(123)<< Hits;
                            cout << setw(123)<< "Press any key to continue ";
                            cin >> s;
                            Hits = "";
                        }

                        Hits = p1.PlayTurn(Hits,p2);
                        if (p2.GetBoatsAlive() == 0){break;}

                        cout << endl<< setw(117)  << "First player\'s turn is finished" << endl;
                        cout << setw(114)<< "Press any key to continue ";
                        cin >> s;
                        printf("\033c");
                        system("Color F1");
                        for (int i = 0; i < 17; i++) {
                        cout << endl;
                        }
                        cout<< setw(115) << "It\'s the second player turn" << endl;
                        cout<< setw(114) << "Press any key to continue ";
                        cin >> s;
                        printf("\033c");

                        p2.BeginTurn();
                        if (Hits != ""){
                            printf("\033c");
                            p2.display();
                            cout << setw(123)<<  "-------- Your Opponent's Strikes --------" << endl;
                            cout << setw(123)<< Hits;
                            cout << setw(123)<< "Press any key to continue ";
                            cin >> s;
                            Hits = "";
                        }
                        Hits = p2.PlayTurn(Hits,p1);
                        if (p1.GetBoatsAlive() == 0){break;}

                        cout <<  endl<<endl<<endl<< setw(117)<< "Second player\'s turn is finished" << endl;
                        cout << setw(116)<< "Press any key to continue ";
                        cin >> s;
                        printf("\033c");
                        system("Color F1");
                        for (int i = 0; i < 17; i++) {
                        cout << endl;
                        }
                        cout<< setw(115) << "It\'s the second player turn" << endl;
                        cout<< setw(114) << "Press any key to continue ";
                        cin >> s;
                        printf("\033c");
                    }
                    printf("\033c");

                    if (p2.GetBoatsAlive() == 0){
                        p1.display();
                        printf("\033c");
                        system("Color 0A");
                        printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
                        cout<< setw(114)<< "The first player wins" << endl;
                    }else{
                        p2.display();
                        printf("\033c");
                        system("Color 0A");
                        printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
                        cout <<setw(114)<< "The second player wins" << endl;
                    }
				}


				else if (arrow_row == 1 && arrow_col == 0 && options_move == 13) {
					system("CLS");
					system("Color 3A");
					for (int i = 0; i < 18; i++) {
						cout << endl;
					}cout << setw(100);
					for (int i = 1; i <= 6; i++) {
						cout << (char)i << " ";
					}
					for (int i = 0; i < 18; i++) {
						cout << endl;
					}
					Sleep(5000);
					exit(0);
				}
				else if (options_move == 27) {
					system("CLS");
					print_game_title();
					print_options();
				}
				else if (options_move == -32) {
					upordown = getch();
					switch (upordown) {
					case 72:
						if (arrow_row > 0) {
							arrow_row--;
						}
						break;
					case 80:
						if (arrow_row < options_row - 1) {
							arrow_row++;
						}
						break;
					}
					system("CLS");
					print_game_title();
					print_options();
				}
			}
		}
	}
	void loadingBar()
    {

        // 0 - black background,
        // A - Green Foreground
        system("color 0A");

        // Initialize char for printing
        // loading bar
        char a = 177, b = 219;

        printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
        cout<< setw(114)<<"L O A D I N G . . .";
        printf("\n\n\t\t\t\t\t\t\t\t\t\t\t   ");


        // Print initial loading bar
        for (int i = 0; i < 26; i++)
            printf("%c", a);

        // Set the cursor again starting
        // point of loading bar
        printf("\r");
        printf("\t\t\t\t\t\t\t\t\t\t\t   ");

        // Print loading bar progress
        for (int i = 0; i < 26; i++) {
            printf("%c", b);

            // Sleep for 1 second
            Sleep(1000);
        }
    }

};
int main()
{
    BattleShip screen;
    screen.print_game_title();
    screen.print_options();
    screen.player_chooses_options();
    return 0;
}

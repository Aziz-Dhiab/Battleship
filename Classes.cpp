#include <iostream>
#include <string>
#include <list>
#include <tuple>
#include <iterator>
#include <windows.h>
using namespace std;
string s = "▢⛝◼⛞⛋⊡⬜⧉⬚⟎◳◻⛶□⧠⧈⧇⦰⦱⦲⦳⦴⦵⦶⦷⦸⦹⦺⦻⦼⦽⦾⦿⧀⧁⧂⧃";
// represents a boat and its size and its state if it's placed
class Boat{
    public:
    int length;
    int width;
    int coordinates[20][3]; //represents a boat coordinates when on the board (x,y,hit or not)
    int rotation;
    string state;
    int HitCells;
    int placed; //to know if the boat is placed yet or not
    public:
    Boat(int l, int w = 1){
        length = l;
        width = w;
        state = "unhit";
        rotation = 0; //0 means the rotation is horizontal
        placed = 0;
        HitCells = 0;
    }
    //update the boat after a hit in (x,y) coordinates
    void Update(int x, int y){
        for (int i = 0; i < length * width; i++){
            if (coordinates[i][0] == x && coordinates[i][1] == y){
                coordinates[i][2] = 1;
                state = "hit";
                HitCells++;
                if (HitCells == length * width){
                    state = "dead";
                    return; // to break out of the function
                }
            }
        }
    }
    void display(){
        string s = "";
        if (!placed){
            if(rotation == 0){
                for (int i = 0; i < width; i++){
                    for (int j = 0; j < length; j++){
                        cout << "◯"//"O" if on windows shell  //windows shell doesn't display UTF-8/unicode characters
                            << " ";
                    }
                    cout << endl;
                }
            }else{
                for (int j = 0; j < length; j++){
                    for (int i = 0; i < width; i++){
                        cout << "◯"//"O" if on windows shell  //windows shell doesn't display UTF-8/unicode characters
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
                        string s = "◯";//"O" if on windows shell
                        for (int k = 0; k < width * length; k++){
                            if (coordinates[k][0] == i && coordinates[k][1] == j && coordinates[k][2] == 1){
                                s = "⭙";//"Ø" if on windows shell
                            }
                        }
                        cout << s << " ";
                    }
                    cout << endl;
                }
                cout << state << endl;
            }else{ //to test if the boat is vertical
                for (int j = y; j < y + width; j++){      //or swap the 2 loops if you want to display the boats as vertical
                    for (int i = x; i < x + length; i++){ //
                        string s = "◯";//"O" if on windows shell
                        for (int k = 0; k < width * length; k++){
                            if (coordinates[k][0] == i && coordinates[k][1] == j && coordinates[k][2] == 1){
                                s = "⭙";//"Ø" if on windows shell
                            }
                        }
                        cout << s << " ";
                    }
                    cout << endl;
                }
                cout << state << endl;
            }
        }
    }
};
// represents a board with the boats in it (can represents both a player's or an opponent's board)
class Board{
    public:
    int MaxLength;
    int MaxWidth;
    int NbrBoats;
    list<Boat> BoatList;
    int grid[20][20][2]; // grid the represents the board (x,y,(includeboat,hit)) default grid is 8x8
    public:
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
    //assumes (x,y) are valid coordinates to strike
    void Strike(int x, int y){
        grid[x][y][1] = 1;
        for (auto iter = BoatList.begin(); iter != BoatList.end(); iter++){ //iterate over the BoatList and update the boats one by one
            iter->Update(x, y);
        }
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
    void display(){
        char letter = 'A';

        cout << "    "; //first line that contains numbers
        for (int k = 0; k < MaxWidth; k++){
            cout << k + 1 << "   ";
        }
        cout << endl;

        cout << "  ╔"; //top part of the grid
        for (int k = 0; k < MaxWidth - 1; k++){
            cout << "═══╦";
        }
        cout << "═══╗" << endl;

        for (int i = 0; i < MaxLength; i++){
            cout << char(letter) << " ";
            letter = char(letter + 1);
            for (int j = 0; j < MaxWidth; j++){
                string s = " ";                                   //represents unhit cell that doesn't contain a bot
                if (grid[i][j][0] == 1){
                    s = "◯";          //"O" if on windows shell  //represnets unhit cell that contains a boat
                    if (grid[i][j][1] == 1){
                        s = "⭙";   //"Ø" if on windows shell          //represnets hit cell that contains a boat"ø×Ø•θΘΦΟOₒ"
                    }
                }else{
                    if (grid[i][j][1] == 1){
                        s = "Χ";                                  //represnets hit cell that doesn't contain a bot
                    }
                }
                cout << "║ " << s << " ";
            }
            cout << "║ " << endl;
            if (i < MaxLength - 1){   //condition so you don't draw an extra line
                cout << "  ╠";
                for (int l = 0; l < MaxWidth - 1; l++){
                    cout << "═══╬";
                }
                cout << "═══╣" << endl;
            }
        }

        cout << "  ╚"; //bottom part of the grid
        for (int k = 0; k < MaxWidth - 1; k++){
            cout << "═══╩";
        }
        cout << "═══╝" << endl;
    }
    void OpponentDisplay(){
        cout << "    "; //first line that contains numbers
        char letter = 'A';
        for (int k = 0; k < MaxWidth; k++){
            cout << k + 1 << "   ";
        }
        cout << endl;

        cout << "  ╔"; //top part of the grid
        for (int k = 0; k < MaxWidth - 1; k++){
            cout << "═══╦";
        }
        cout << "═══╗" << endl;

        for (int i = 0; i < MaxLength; i++){
            cout << char(letter) << " ";
            letter = char(letter + 1);
            for (int j = 0; j < MaxWidth; j++){
                string s = " ";              //empty cell
                if (grid[i][j][1] == 1){     //if cell is hit
                    if (grid[i][j][0] == 0){ //if the cell doesn't contain a bot
                        s = "Χ";
                    }else{                   //if the cell contains a boat
                        s = "⭙";//"Ø" if on windows shell
                    }
                }
                cout << "║ " << s << " ";
            }
            cout << "║ " << endl;
            if (i < MaxLength - 1){ //condition so you don't draw an extra line
                cout << "  ╠";
                for (int l = 0; l < MaxWidth - 1; l++){
                    cout << "═══╬";
                }
                cout << "═══╣" << endl;
            }
        }

        cout << "  ╚"; //bottom part of the grid
        for (int k = 0; k < MaxWidth - 1; k++){
            cout << "═══╩";
        }
        cout << "═══╝" << endl;
    }
};
// represents one of two players in a given game
class Player{
    public:
    int turn;
    int defeat;
    int MaxLength;
    int MaxWidth;
    int NbrOfBoatsInInventory;
    list<Boat> inventory;
    Board YourBoard;
    Board OpponentBoard;

    public:
    void SetMaxLength(int i){
        MaxWidth = i;
    }
    void SetMaxWidth(int i){
        MaxWidth = i;
    }
    Player(){
        turn = 0;
        defeat = 0;
        NbrOfBoatsInInventory = 0;
    }
    //parametrised constructor that adds the default boats to the player (both classic and belgium)
    Player(string s){
        turn = 0;
        defeat = 0;
        MaxLength = 8;
        MaxWidth = 8;
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
        cout << "------ The Boats In Your Inventory ------" << endl;
        int index = 1;
        for (auto iter = inventory.begin(); iter != inventory.end(); iter++)
        { //iterate over the boats in the inventory
            iter->display();
            cout << "Boat number " << index << endl;
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
        int c = toupper(s[0]) + 1; //make the letter uppercase ang get it's ascii code
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
    }
    void PlaceTheBoats(){
        string s;
        int i;
        tuple<int, int> t;
        while (!InventoryIsEmpty()){
        DisplayBoard();
        ShowInventory();
        cout << endl << "Enter the number of the boat to place" << endl;
        while (1){
            cin >> s;
            if ((s.find_first_not_of("0123456789") == -1)){ //the input is a number
                i = stoi(s);
                if (i>=1 && i<=NbrOfBoatsInInventory){
                    break;
                }
            }
            cout << "Not a valid number, please try again" << endl;
        }
        cout << "Do you want to rotate the boat ? enter 1 for yes and 0 for no" << endl;
        do {
            cin >> s;
            if (s != "0" && s != "1"){
                cout << "Not a valid input, please try again" << endl;
            }
        }while (s != "0" && s != "1");
        SelectBoat(i,stoi(s));
        printf("\033c");
        DisplayBoard();
        cout << "This is your selected Boat" << endl << endl;
        ShowSelectedBoat();
        cout << endl << "Please enter the coordinates of the top-left corner of the boat" << endl;
        while (1){
            cin >> s;
            if (VerifyInput(s)){ //the input is in the right form
                t = TransformInput(s);
                if (VerifySelectedBoatPlacement(get<0>(t),get<1>(t))){
                    break;
                }
            }
            cout << "Invalid coordinates, please try again" << endl;
        }
        PlaceBoatFromInventory(get<0>(t),get<1>(t));
        printf("\033c");
    }
    DisplayBoard();
    }
    //check if all the boats a player have on board are dead
    void loser(){
        int DeadBoats = 0;
        for (auto iter = YourBoard.BoatList.begin(); iter != YourBoard.BoatList.end(); iter++){ //iterate over the boats in the inventory
            if (iter->state == "dead"){
                DeadBoats++;
            }
            if (DeadBoats == YourBoard.BoatList.size()){
                defeat = 1;
            }
        }
    }
    //show a player's placed boats and their state and the player's and opponent's boards
    void display(){
        cout << "-------------- Your Boats ---------------" << endl;
        for (auto iter = YourBoard.BoatList.begin(); iter != YourBoard.BoatList.end(); iter++){ //iterate over the boats in player's board list
            iter->display();
        }
        cout << "--------------- Your Board ---------------" << endl;
        YourBoard.display();
        cout << "---------- Your Opponent's Board ---------" << endl;
        OpponentBoard.OpponentDisplay();
    }
    void DisplayBoard(){
        cout << "--------------- Your Board ---------------" << endl;
        YourBoard.display();
    }
};

int main(){
    //global variables used for input control
    string s;
    int i;
    tuple<int, int> t;
    //setting up and choosing mode
    Player p1;
    Player p2;
    //Sleep(300);
    cout << "--------- Welcome To Battleship ----------" << endl;
    //Sleep(500); //selecting the play mode
    cout << "To play classic mode please enter 1" << endl;
    //Sleep(500);
    cout << "To play Belgium mode please enter 2" << endl;
    //Sleep(500);
    do {
        cin >> s;
        if (s != "1" && s != "2"){
            cout << "Not a valid input, please try again" << endl;
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
    //Sleep(500);
    cout << "It\'s the first player turn" << endl;
    //Sleep(500);
    cout << "Press any key to continue" << endl;
    cin >> s;
    //Sleep(500);
    printf("\033c");
    //Sleep(500);

    p1.PlaceTheBoats();

    cout << endl << endl << "First player\'s turn is finished" << endl;
    cout << "Press any key to continue" << endl;
    cin >> s;
    printf("\033c");
    cout << "It\'s the secound player turn" << endl;
    cout << "Press any key to continue" << endl;
    cin >> s;
    printf("\033c");

    p2.PlaceTheBoats();
    


    system("pause");
    return 0;
}
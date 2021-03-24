#include <iostream>
#include <string>
#include <list>
#include <tuple>
#include <iterator>
using namespace std;
string s = "▢⛝◼⛞⛋⊡⬜⧉⬚⟎◳◻⛶□⧠⧈⧇⦰⦱⦲⦳⦴⦵⦶⦷⦸⦹⦺⦻⦼⦽⦾⦿⧀⧁⧂⧃";
class Boat {
    public:
    int length;
    int width;
    int coordinates [20][3]; //represents a boat coordinates when on the board (x,y,hit or not)
    int rotation;
    string state;
    int HitCells;
    int placed; //to know if the boat is placed yet or not
    public:
    Boat(int l,int w=1){
        length = l;
        width = w;
        state = "unhit";
        rotation = 0; //0 means the rotation is horizontal
        placed = 0;
        HitCells = 0;
    }
    void Update(int x,int y){ //update the boat after a hit in (x,y) coordinates
        for (int i=0 ; i<length*width ; i++){
            if (coordinates[i][0] == x && coordinates[i][1] == y){
                coordinates[i][2] = 1;
                state = "hit";
                HitCells++;
                if (HitCells == length*width){
                    state = "dead";
                    return; // to break out of the function
                }
            }
        }
    }
    void display(){
        string s ="";
        if (!placed){
            for (int i=0 ; i<width ; i++){
                for (int j=0 ; j<length ; j++){
                    cout << "⦿" << " ";
                }
                cout << endl;
            }
        }
        else
        { //(coordinates[0][0],coordinates[0][1]) are the top left coordinates of a placed boat
            int x = coordinates[0][0];
            int y = coordinates[0][1];
            if (rotation == 0){
                for (int i = x; i<x+width; i++){
                    for (int j = y; j<y+length; j++){
                        string s = "⦿";
                        for (int k = 0; k < width*length; k++){
                            if (coordinates[k][0] == i && coordinates[k][1] == j && coordinates[k][2] == 1){
                                s = "⦻";
                            }
                        }
                        cout << s << " ";
                    }
                    cout << endl;
                }
                cout << state << endl;
            }else{  //to test if the boat is vertical
                for (int j = y; j<y+width; j++){      //or swap the 2 loops if you want to display the boats as vertical
                    for (int i = x; i<x+length; i++){ //
                        string s = "⦿";
                        for (int k = 0; k < width * length; k++){
                            if (coordinates[k][0] == i && coordinates[k][1] == j && coordinates[k][2] == 1){
                                s = "⦻";
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
class Board {
    public:
    int MaxLength;
    int MaxWidth;
    int NbrBoats;
    list<Boat> BoatList;
    int grid [20][20][2]; // grid the represents the board (x,y,(includeboat,hit)) default grid is 8x8
    public:
    Board(int l=8,int w=8){
        MaxLength = l;
        MaxWidth = w;
        for (int i=0 ; i<MaxLength ; i++){
            for (int j=0 ; j<MaxWidth ; j++){
                grid[i][j][0] = 0;
                grid[i][j][1] = 0;
            }
        }   
    }
    void Strike (int x, int y){ //assumes (x,y) are valid coordinates to strike
        grid[x][y][1] = 1;
        for (auto iter = BoatList.begin() ; iter != BoatList.end() ; iter++){ //iterate over the BoatList and update the boats one by one
            iter->Update(x,y);
        }
    }
    int VerifyBoatPlacement(int x, int y, Boat b){ //verify you can place the boat in the (x,y) coordinates where (x,y) are the top left coordinates
        if (b.rotation == 0){ // the boat is horizontal in its rotation
            if (x+b.width>MaxLength){ 
                return 0;
            }
            if (y+b.length>MaxWidth){
                return 0;
            }
            for (int i=x ; i<x+b.width ; i++){ //verify the boat is not colliding with any other boat
                for (int j=y ; j<y+b.length ; j++){
                    if (grid[i][j][0] == 1){
                        return 0;
                    }
                }
            }
        }
        if (b.rotation == 1){ // the boat is vertical in its rotation
            if (x+b.length>MaxWidth){
                return 0;
            }
            if (y+b.width>MaxLength){
                return 0;
            }
            for (int i=x ; i<x+b.length ; i++){ //verify the boat is not colliding with any other boat
                for (int j=y ; j<y+b.width ; j++){
                    if (grid[i][j][0] == 1){
                        return 0;
                    }
                }
            }
        }
        return 1;
    }
    void PlaceBoat(int x, int y, Boat b){ //takes a boat and places it where (x,y) are the top left coordinates and add it to the list of placed boats
        b.placed = 1;
        if (b.rotation == 0 ){ //the boat is horizontal
            int k = 0;
            for (int i=x ; i<x+b.width ; i++){
                for (int j=y ; j<y+b.length ; j++){
                    grid[i][j][0] = 1;
                    b.coordinates[k][0] = i ; // add (i,j) coordinates to the boat and initialize it to not hit
                    b.coordinates[k][1] = j ; //
                    b.coordinates[k][2] = 0 ; //
                    k++;
                }
            }
        }else{ //the boat is vertical
            int k = 0;
            for (int i=x ; i<x+b.length ; i++){
                for (int j=y ; j<y+b.width ; j++){
                    grid[i][j][0] = 1;
                    b.coordinates[k][0] = i ; // add (i,j) coordinates to the boat and initialize it to not hit
                    b.coordinates[k][1] = j ; //
                    b.coordinates[k][2] = 0 ; //
                    k++;
                }
            }
        }
        BoatList.push_back(b);
    }
    void display(){
        cout << "  ";
        char letter = 'A';
        for (int k=0 ; k<MaxWidth ; k++){
            cout << k+1 << "  ";
        }
        cout << endl;
        for (int i=0 ; i<MaxLength ; i++){
            cout << char(letter) << " ";
            letter = char(letter+1);
                for (int j=0 ; j<MaxWidth ; j++){
                    string s = "〇";            //represents unhit cell that doesn't contain a bot
                    if (grid[i][j][0]==1){
                        s = "⦿";               //represnets unhit cell that contains a boat
                        if (grid[i][j][1]==1){
                            s="⦻";             //represnets hit cell that contains a boat
                        }
                    }
                    else{
                        if (grid[i][j][1]==1){
                            s= "⦰";            //represnets hit cell that doesn't contain a bot
                        }
                    }
                    cout << s << " ";
                }
                cout << endl;
            }
    }
    void OpponentDisplay(){
        cout << "  ";
        char letter = 'A';
        for (int k=0 ; k<MaxWidth ; k++){
            cout << k+1 << "  ";
        }
        cout << endl;
        for (int i=0 ; i<MaxLength ; i++){
            cout << char(letter) << " ";
            letter = char(letter+1);
                for (int j=0 ; j<MaxLength ; j++){
                    string s = "〇";   //cell is empty by default
                    if (grid[i][j][1]==1){ //cell is hit
                        if (grid[i][j][0]==0){ //if the cell doesn't contain a bot
                            s = "⦰";
                        }else { // if the cell contains a boat
                            s = "⦻";
                        }
                    }
                    cout << s << " ";
                }
                cout << endl;
            }
    }
};
class Player{
    public:
    int turn;
    int defeat;
    list<Boat> inventory;
    Board YourBoard;
    Board OpponentBoard;
    public:
    Player(){
        turn = 0;
        defeat = 0;
    }
    Player(string s){ //parametrised constructor that adds the default boats to the player (both classic and belgium)
        turn = 0;
        defeat = 0;
        if (s == "classic"){ //one length 5 bot, one length 4 boat, two length 3 boats, one length 2 boat
            Boat b(5);
            this->AddBoat(b);
            b = Boat(4);
            this->AddBoat(b);
            b = Boat(3);
            this->AddBoat(b);
            this->AddBoat(b);
            b = Boat(2);
            this->AddBoat(b);
        }else if (s == "belgium"){ //one length 4 boat, two length 3 boats, three length 2 boats, four length 1 boats
            Boat b(4);
            this->AddBoat(b);
            b = Boat(3);
            this->AddBoat(b);
            this->AddBoat(b);
            b = Boat(2);
            this->AddBoat(b);
            this->AddBoat(b);
            this->AddBoat(b);
            b = Boat(1);
            this->AddBoat(b);
            this->AddBoat(b);
            this->AddBoat(b);
            this->AddBoat(b);
        }

    }
    void AddBoat(Boat b){
        inventory.push_back(b);
    }
    void ShowInventory(){
        int index = 1;
        for (auto iter = inventory.begin() ; iter != inventory.end() ; iter++){ //iterate over the boats in the inventory
            iter->display();
            cout << "Boat number " << index << endl;
            index ++;
        }
    }
    void SelectBoat(int index, int rot=0){ //takes the chosen boat and put it in the front of the list in the inventory (and rotate it if needed)
        auto iter = inventory.begin();
        for (int i = 0 ; i< index-1 ; i++){
        iter++;
        }
        Boat b = *iter;
        inventory.erase(iter);
        if (rot == 1){
            b.rotation = 1;
        }
        inventory.push_front(b);
        } 
    void PlaceBoatFromInventory(int x, int y){
        Boat b = inventory.front();
        inventory.pop_front();
        YourBoard.PlaceBoat(x,y,b);
    }
    void loser(){ //check if all the boats a player have on board are dead
        int DeadBoats = 0;
        for (auto iter = YourBoard.BoatList.begin() ; iter != YourBoard.BoatList.end() ; iter++){ //iterate over the boats in the inventory
            if (iter->state == "dead"){
                DeadBoats++;
            }
            if (DeadBoats == YourBoard.BoatList.size()){
                defeat = 1;
            }
        }
    }
    void display(){ //show a player's placed boats and their state and the player's and opponent's boards
        for (auto iter = YourBoard.BoatList.begin() ; iter != YourBoard.BoatList.end() ; iter++){ //iterate over the boats in player's board list
            iter->display();
        }
        OpponentBoard.OpponentDisplay();
        YourBoard.display();
    }

};
int VerifyInput(string s,int length,int width){ //verify the player's input is in the right form i.e E5 , B7
    int a = int('a');
    int A = int('A');
    char c = s[0];
    int letter = int(c);
    string s1 = s.substr(1);
    int numbers = stoi( s1 );
    if (s.length() != (width / 10) +2){ //verify the length of the input
        return 0;
    }
    if (!(s1.find_first_not_of( "0123456789" ) == string::npos)){ //verify the rest of the string is numbers //or you can use /*if (!(s1.find_first_not_of( "0123456789" ) == -1))*/ because s.find_first_not_of(s1) returns -1 if s only contain chars from s1
        return 0;
    }
    if (!(numbers>0 && numbers<=width)){ //verify the numbers are within the appropriate range
        return 0;
    }
    if (!((letter >= a && letter < a+width) || (letter >= A && letter < A+width))){ // verify the letter is within the range of letters i.e [a..g]
        return 0;
    }
    return 1;
}
tuple <int,int> TransformInput(string s){ // transform the input into the grid coordinates i.e "B4" -> (1,3)
    int A = int('A');
    int c = toupper(s[0]) +1 ; //make the letter uppercase ang get it's ascii code
    c -= A;
    int numbers = stoi(s.substr(1));
    return make_tuple(c-1,numbers-1);
}
int main(){
    //debugging start


    Player p1("classic");
    p1.PlaceBoatFromInventory(0,0);
    p1.PlaceBoatFromInventory(2,0);
    p1.PlaceBoatFromInventory(4,0);
    p1.PlaceBoatFromInventory(4,4);
    p1.SelectBoat(1,1); // rotate boat
    p1.PlaceBoatFromInventory(6,0);
    p1.YourBoard.Strike(0,0);
    p1.YourBoard.Strike(1,0);
    p1.YourBoard.Strike(6,0);
    p1.YourBoard.Strike(7,0);
    p1.ShowInventory(); // it's empty
    p1.display(); //showing your boats then opponent board then your board
    

    //debugging end
    return 0;
}
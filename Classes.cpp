#include <iostream>
#include <string>
#include <list>
using namespace std;
string s = "▢⛝◼⛞⛋⬜⧉⬚⟎◳◻⛶□⧠⧈⧇";
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
        rotation = 0;
        placed = 0;
    }
    void RotateBoat(){
        rotation = !rotation;
    }
    void Update(int x,int y){ //update the boat after a hit in (x,y) coordinates
        for (int i=0 ; i<length*width ; i++){
            if (coordinates[i][0] == x && coordinates[i][1] == y){
                coordinates[i][2] = 1;
                state = "hit";
                HitCells++;
                if (HitCells == length*width){
                    state = "dead";
                }
            }
        }
    }
    void display(){
        string s ="";
        if (!placed){
            for (int i=0 ; i<width ; i++){
                for (int j=0 ; j<length ; j++){
                    cout << "⛋" << " ";
                }
                cout << endl;
            }
        }
        else {
            for (int i=0 ; i<width ; i++){
                for (int j=0 ; j<length ; j++){
                    string s = "⛋";
                    for (int k=0 ; k<width*length ; k++){
                        if (coordinates[k][0] == i && coordinates[k][1] == j){
                            s = "⛝";
                        }
                    }
                    cout << s << " ";
                }
                cout << endl;
            }
            cout << state << endl;
        }
    }
};
class Board {
    public:
    int NbrBoats;
    list<Boat> BoatList;
    int grid [8][8][2]; // grid the represents the board (x,y,(includeboat,hit))
    public:
    Board(){
        for (int i=0 ; i<8 ; i++){
            for (int j=0 ; j<8 ; j++){
                grid[i][j][0] = 0;
                grid[i][j][1] = 0;
            }
        }   
    }
    void Strike (int x,int y){ //assumes (x,y) are valid coordinates to strike
        grid[x][y][2] = 1;
        for (auto iter = BoatList.begin() ; iter != BoatList.end() ; iter++){ //iterate over the BoatList and update the boats one by one
            iter->Update(x,y);
        }
    }
    void display(){
        for (int i=0 ; i<8 ; i++){
                for (int j=0 ; j<8 ; j++){
                    string s = "⧉";
                    if (grid[i][j][0]==1){
                        s = "⛋";
                        if (grid[i][j][1]==1){
                            s="⛝";
                        }
                    }
                    else{
                        if (grid[i][j][1]==1){
                            s= "⛞";
                        }
                    }
                    cout << s << " ";
                }
                cout << endl;
            }
    }
    void OpponentDisplay(){
        for (int i=0 ; i<8 ; i++){
                for (int j=0 ; j<8 ; j++){
                    string s = "⧉";   //cell is empty by default
                    if (grid[i][j][1]==1){ //cell is hit
                        if (grid[i][j][0]==0){ //if the cell doesn't contain a bot
                            s = "⛞";
                        }else { // if the cell contains a boat
                            s = "⛝";
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
    void AddBoat(Boat b){
        inventory.push_back(b);
    }
    void ShowInventory(){
        for (auto iter = inventory.begin() ; iter != inventory.end() ; iter++){ //iterate over the boats in the inventory
            iter->display();
        }
    }
    void loser(){
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
        for (auto iter = YourBoard.BoatList.begin() ; iter != YourBoard.BoatList.end() ; iter++){ //iterate over the boats in the inventory
            iter->display();
        }
        OpponentBoard.OpponentDisplay();
        YourBoard.display();
    }

};
int main(){
    Boat b(5,2); //debugging start
    b.display();

    b.coordinates[0][0]=0;
    b.coordinates[0][1]=0;
    b.coordinates[0][2]=1;
    b.placed=1;
    b.display();
    cout << endl;


    Board bo;
    bo.grid[0][0][0]=0;
    bo.grid[0][0][1]=0;

    bo.grid[0][1][0]=1;
    bo.grid[0][1][1]=0;

    bo.grid[1][0][0]=0;
    bo.grid[1][0][1]=1;

    bo.grid[1][1][0]=1;
    bo.grid[1][1][1]=1;

    bo.display(); //debugging end
    return 0;
}
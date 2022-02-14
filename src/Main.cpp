#include <iostream>
#include <vector>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <string>
using namespace std;

vector<string> readFile(string fileName){   // Reading file function for PART 1
    fstream new_file;
    vector<string> str;
    new_file.open(fileName, ios::in);
    string line;
    if(!new_file) {
        cout<<"No such file";
    }
    else {
        while (getline(new_file, line)) {
            str.push_back(line);
        }
        new_file.close();
    }
    return str;
}
vector<string> readFile2(string fileName){ // Reading file function for PART 2
    fstream new_file;
    vector<string> str;
    new_file.open(fileName, ios::in);
    string line;
    if(!new_file) {
        cout<<"No such file";
    }
    else {
        while (getline(new_file, line)) {
            str.push_back(line);
        }
        new_file.close();
    }
    return str;

}
int strToint(string int_x){ // Converting string to int function
    stringstream value(int_x);
    int number;
    value >> number;
    return number;
}
int* split(string str){  // Splitting string  by given parameter
    int* commandArray=new int[3];
    string strToken;
    int index=0;

    stringstream X(str);
    while(getline(X,strToken,' ')){
        commandArray[index]= strToint(strToken);
        index++;
    }
    return commandArray;
}
vector<vector<int>> create2DArray(int size){  // Creating 2d Array for grid
    vector<vector<int>> grid;
    vector<int> arr;
    for(int i=0;i<size;i++){
        arr.push_back(0);
    }
    for(int i=0;i<size;i++){
        grid.push_back(arr);
    }

    return grid;
}
void check(int row,int col,vector<vector<int>> &grid,vector<vector<int>> &indexList) {  // Checking the squares around the index
    int size=grid.size();
    int curr = grid[row][col];
    vector<int> arr = {row,col};
    if (find(indexList.begin(), indexList.end(), arr) == indexList.end()){  // Checking whether index is in indexList or not.
        indexList.push_back(arr);
    }

    if (col != 0) {
        if (grid[row][col - 1] == curr) {  // Checking left square of current index
            vector<int> arr = {row,col-1};
            if (find(indexList.begin(), indexList.end(), arr) == indexList.end()) {
                indexList.push_back(arr);
                check(row, col - 1, grid,indexList);
            }
        }
    }
    if (col != size - 1) {
        if (grid[row][col + 1] == curr) {  // Checking right square of current index
            vector<int> arr = {row,col+1};
            if (find(indexList.begin(), indexList.end(), arr) == indexList.end()) {
                indexList.push_back(arr);
                check(row, col + 1, grid,indexList);
            }
        }
    }

    if (row != 0) {
        if (grid[row - 1][col] == curr) {  // Checking up square of current index
            vector<int> arr = {row-1,col};
            if (find(indexList.begin(), indexList.end(), arr) == indexList.end()) {
                indexList.push_back(arr);
                check(row - 1, col, grid,indexList);
            }
        }
    }

    if (row != size - 1) {
        if (grid[row + 1][col] == curr) {  // Checking down square of current index
            vector<int> arr = {row+1,col};

            if (find(indexList.begin(), indexList.end(), arr) == indexList.end()) {
                indexList.push_back(arr);
                check(row + 1, col, grid,indexList);
            }
        }
    }
}
void balloon(int row,int col,int &bal,vector<vector<int>> &grid){  // Adding balloon to the squares if they are matching or not.
    vector<vector<int>> index2;
    grid[row][col] = bal;
    check(row,col,grid,index2);
    if(index2.size()>=3){
        for (int i=0;i<index2.size();i++) {
            if(!(index2[i][0]==row && index2[i][1]==col)){
                grid[index2[i][0]][index2[i][1]]=0;
            }
        }
        bal++;
        balloon(row,col,bal,grid);
    }
}


int main(int argc, char *argv[]) {
    fstream outputFile;
    outputFile.open(argv[3], ios::out);

    //PART 1 //
    vector<string> commands = readFile(argv[1]);
    int gridSize = strToint(commands[0]);
    vector<vector<int>> grid = create2DArray(gridSize);

    int bal, row, col;
    for (int i = 1; i < commands.size(); i++) {
        int *array = split(commands[i]);
        bal = array[0];
        row = array[1];
        col = array[2];

        int key=0;
        while(key<commands.size()){
            balloon(row,col,bal,grid);
            key++;}
    }
    outputFile<<"PART 1:"<<endl;
    for (int i = 0; i < gridSize; i++) {
        for (int j = 0; j < gridSize; j++) {
            outputFile <<grid[i][j]<<" ";
        }
        outputFile << endl;
    }


    // PART2 //

    vector<string> commands2= readFile2(argv[2]);
    int gridSize2 = strToint(commands2[0]);
    vector<vector<int>> grid2 = create2DArray(gridSize2);
    for (int i = 1; i < gridSize2+1; i++) {
        int *array = split(commands2[i]);
        for (int j = 0; j < gridSize2; j++){
            grid2[i-1][j]=array[j];
        }
    }

    vector<int*> bombList;
    int point=0;
    int number=0;
    for (int i =gridSize2+1; i < commands2.size(); i++) {
        int *array = split(commands2[i]);
        int* bombArray =new int[2] {array[0],array[1]};  // Array that contains indexs of bombs
        bombList.push_back(bombArray);}

    for (int i = 0; i < bombList.size(); i++) {  // Creating grid
        int x= bombList[i][0];
        int y= bombList[i][1];
        number= grid2[x][y];


        for(int key=0;key<gridSize2;key++){ // Checking horizontally
            if(grid2[x][key]==number){
                point+=number;
                grid2[x][key]=0;
            }
        }
        for(int key=0;key<gridSize2;key++){ // Checking vertically
            if(grid2[key][y]==number){
                point+=number;
                grid2[key][y]=0;
            }
        }

        for(int i= y-1, j=x+1;i>-1 && j<gridSize2 ;i--,j++){   // Checking diagonally
            if(grid2[j][i]==number){
                point+=number;
                grid2[j][i]=0;
            }
        }

        for(int i= y+1, j=x+1;i<gridSize2 && j<gridSize2 ;i++,j++){ // Checking diagonally
            if(grid2[j][i]==number){
                point+=number;
                grid2[j][i]=0;
            }

        }

        for(int i= y-1, j=x-1;i>-1 && j>-1 ;i--,j--){ // Checking diagonally
            if(grid2[j][i]==number){
                point+=number;
                grid2[j][i]=0;
            }

        }

        for(int i= y+1, j=x-1;i<gridSize2 && j>-1 ;i++,j--){ // Checking diagonally
            if(grid2[j][i]==number){
                point+=number;
                grid2[j][i]=0;
            }
        }

    }
    outputFile << "\nPART 2:" << endl;
    for (int i = 0; i < gridSize2; i++) {
        for (int j = 0; j < gridSize2; j++) {
            outputFile << grid2[i][j] << " ";
        }
        outputFile << endl;
    }
    

    outputFile<<"\nFinal Point: "<<point<<"p";
    outputFile.close();

    return 0;
}
j
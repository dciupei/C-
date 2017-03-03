//David Ciupei
//Assignment #2

#include <string>
#include <iostream>
#include <algorithm>
#include "SudokuGrid.h"

using namespace std;

bool conflictingNumber(SudokuGrid& grid, int row, int col, int n){
    for (int c = 0; c < 9; c++) {           //checks the row
        if(grid.number(row,c) == n){
            return true;
        }
    }
    for (int r = 0; r < 9; r++) {           //checks the column
        if(grid.number(r,col) == n){
            return true;
        }
    }
    
    for (int j = 0; j < 3; j++) {           //checks the 3x3 block
        for (int i = 0; i < 3; i++) {
            int r = ((row/3)*3) + j;
            int c =  ((col/3)*3) + i;
            if (grid.number(r,c) == n){
                return true;
            }
        }
    }
    
    
    return false;
}

int numPencilsInRow(SudokuGrid& grid,int row,int n){
    int count = 0;
    for (int c = 0; c < 9; c++) {           //checks if pencil value is in row, if it is increments counter
        if(grid.isPencilSet(row,c, n)){
            count++;
        }
        
    }
    
    return count;
}

int numPencilsInColumn(SudokuGrid& grid,int col,int n){
    int count = 0;
    for (int r = 0; r < 9; r++) {           //checks if pencil value is in column, if it is increments counter
        if(grid.isPencilSet(r,col, n)){
            count++;
        }
        
    }
    
    return count;
}

int numPencilsInBlock(SudokuGrid& grid,int row,int col,int n){
    int count = 0;
    
    for (int j = 0; j < 3; j++) {           // checks if pencil value is in block, if it is increments counter
        for (int i = 0; i < 3; i++) {
            int r = ((row/3)*3) + j;
            int c = ((col/3)*3) + i;
            if(grid.isPencilSet(r,c, n)){
                count++;
            }
            
        }
    }
    
    return count;
}

bool findUnassignedLocation(SudokuGrid& grid,int &row,int &col){
    for(row = 0; row < 9; row++){
        for(col = 0; col < 9; col++){
            if(grid.number(row,col) == 0){
                return true;
            }
        }
    }
    return false;
}

void autoPencil(SudokuGrid& grid) {
    for (int r = 0; r < 9; r++)
        for (int c = 0; c < 9; c++)
            if (grid.number(r,c) == 0) {
                grid.setAllPencils(r,c);
                for (int n = 1; n <= 9; n++)
                    if (conflictingNumber(grid,r,c,n))
                        grid.clearPencil(r,c,n);
            }
}


void deduce(SudokuGrid& grid) {
    bool changed;
    do { // repeat until no changes made
        autoPencil(grid);
        changed = false;
        for (int row = 0; row < 9; row++)
            for (int col = 0; col < 9; col++)
                for (int n = 1; n <= 9; n++)
                    if (grid.isPencilSet(row, col, n) &&
                        (numPencilsInRow(grid, row, n) == 1 ||
                         numPencilsInColumn(grid, col, n) == 1 ||
                         numPencilsInBlock(grid, row, col, n) == 1)) {
                            grid.clearAllPencils(row, col);
                            grid.setNumber(row,col,n);
                            grid.setSolved(row,col);
                            autoPencil(grid);
                            changed = true;
                            break;
                        }
    } while(changed);
}

bool solveSudoku(SudokuGrid& grid) {
    int row, col;
    if (!findUnassignedLocation(grid, row, col))
        return true; // puzzle filled, solution found!
    for (int num = 1; num <= 9; num++)
        if (!conflictingNumber(grid, row, col, num)) {
            grid.setNumber(row, col, num); // try next number
            if (solveSudoku(grid))
                return true; // solved!
            grid.setNumber(row, col, 0); // not solved, clear number
        }
    return false; // not solved, back track
}

void printGrid(SudokuGrid& grid){
    cout << "\n";
    
    for(int r = 0; r < 9; r++){
        for(int c = 0; c < 9; c++){
            
            if (c == 3 || c==6){
                cout << "|";
                if (grid.number(r,c) == 0) {
                    cout << " " << "." << " ";
                    continue;
                }
                cout << " " << grid.number(r,c) << " ";
                continue;
                
            }
            if (grid.number(r,c) == 0) {
                cout << "." << " ";
                continue;
            }
            
            
            cout << grid.number(r,c) << " ";
        }
        cout << '\n';
        if(r == 2 || r == 5){
            cout << "------+-------+------" << "\n";
        }
        
    }
    
}

int main(int argc, char *argv[]) {
    string puzzle;
    cin >> puzzle;
    if (puzzle.length() != 9*9 ||
        !all_of(puzzle.begin(), puzzle.end(),
                [](char ch) {
                    return ch == '.' || ('1' <= ch && ch <= '9');
                })) {
                    cerr << "bogus puzzle!" << endl;
                    exit(1);
                }
    SudokuGrid grid(puzzle);
    deduce(grid);
    printGrid(grid);
    solveSudoku(grid);
    printGrid(grid);
    
    
    return 0;
}
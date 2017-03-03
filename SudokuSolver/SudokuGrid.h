//David Ciupei
//Assignment #2

#ifndef SUDOKUGRID_H
#define SUDOKUGRID_H

#include <array>
#include <bitset>
#include <string>
using namespace std;


class SudokuGrid {
private:
    // private class instance vars go here
    struct Cell{
        int value;
        bitset<9> pencils;
        bool isSolved;
        bool isFixed;
        Cell():value(0),pencils(),isSolved(), isFixed(){}
        Cell(int val, bitset<9> p, bool s, bool f):value(val),pencils(p.reset()),isSolved(s), isFixed(f){}
        
    };
    
    std::array<std::array<Cell,9>,9> grid;
    
public:
    // implement the following
    SudokuGrid(std::string s) {
        string::iterator puzzle = s.begin();    //iterate through string
        for(int r = 0; r < 9; r++){
            for(int c = 0; c < 9; c++){
                int value;
                bitset<9> p;
                bool isFixed;
                if(*puzzle == '.'){             // if empty print
                    value = 0;
                    isFixed = false;
                }
                else{
                    value = *puzzle - '0';      //if not gets actual value
                    isFixed = true;
                }
                grid[r][c] = *new Cell(value, p, false, isFixed );    //insert the value in cell
                
                puzzle++;               //increment to next input
            }
        }
        
        for(int r = 0; r < 9; r++){
            for(int c = 0; c < 9; c++){
                
                if (c == 3 || c==6){
                    cout << "|";
                    if (grid[r][c].value == 0) {
                        cout << " " << "." << " ";
                        continue;
                    }
                    cout << " " << grid[r][c].value << " ";
                    continue;
                    
                }
                if (grid[r][c].value == 0) {
                    cout << "." << " ";
                    continue;
                }
                
                
                cout << grid[r][c].value << " ";
            }
            cout << '\n';
            if(r == 2 || r == 5){
                cout << "------+-------+------" << "\n";
            }
            
        }
        
    } // constructor
    int number(int row, int col) const{
        if (grid[row][col].value == 0){             //returns number at location
            return 0;
        }
        return grid[row][col].value;
    }
    
    void setNumber(int row, int col, int number) {     //assigns number at location
        grid[row][col].value = number;
        number = 0;
    }
    
    bool isFixed(int row, int col) const{              //if the number is original fixed value
        if(grid[row][col].isFixed == true){
            return true;
        }
        return false;
    }
    
    bool isSolved(int row, int col) const{              //checks if cell is solved
        if(grid[row][col].isSolved == true){
            return true;
        }
        return false;
    }
    
    void setSolved(int row, int col) {                  //sets cell to solved
        grid[row][col].isSolved = true;
    }
    
    bool isPencilSet(int row, int col, int n) const{    //checks if pencil is true
        if(grid[row][col].pencils.test(n-1)){
            return true;
        }
        return false;
    }
    
    bool anyPencilsSet(int row, int col) const{        //checks if any pencil values are true
        for(int i = 0; i < 9; i++){
            if(grid[row][col].pencils.test(i)){
                return true;
            }
        }
        return false;
    }
    
    void setPencil(int row, int col, int n) {           //sets specific pencil to true
        grid[row][col].pencils.set(n-1);
        
    }
    
    void setAllPencils(int row, int col) {              //sets all pencils to true
        grid[row][col].pencils.set();
        
    }
    
    void clearPencil(int row, int col, int n) {         //sets specific pencil to false
        grid[row][col].pencils.reset(n-1);
        
    }
    
    void clearAllPencils(int row, int col) {            //sets all pencils to false
        grid[row][col].pencils.reset();
        
    }
    
    
    
};

#endif // SUDOKUGRID_H


//David Ciupei
//CS355 Project #1

#include <iostream>
#include <string>
#include <vector>

int main() {
  std::vector<std::string> lines;
  
  std::string line;
  while (std::getline(std::cin, line)) {
    lines.push_back(line);
    
  }
  

  int row = lines.size(); //checks to see how many words are being passed through
  int MAXXX = 0;          //will store the max length string that is found
  for(int k = 0; k< (int)lines.size(); k++){   // will go through each string and check for the longest
    int maxL = lines[k].size();
    if(maxL > MAXXX){
      MAXXX = maxL;
    }                                           
  }
  //std::cout << "longest number is: "<< MAXXX << '\n';
  
  for(int i = 0; i< (int)MAXXX; i++){
    for(int j= row-1;j>=0;j--){
      if((int)lines[j].length() < MAXXX){   // if the length of the string is less then the max length
	int spaces = MAXXX - lines[j].length();  // gets the difference from max and current string
	int word = MAXXX - spaces;    // subtracts the difference from the max string
	if(i < word){    
	  std::cout << lines[j][i];  // prints the letters 
	}else{
	  std::cout << ' ';      // when no more letters prints blanks
	}
      }else{
	std::cout << lines[j][i]; // if the longest string will just print the letters
      }
    }     
    std::cout << '\n';       
  }
  
  return 0;
}

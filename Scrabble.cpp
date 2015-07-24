#include<iostream>
#include<fstream>
#include<vector>
#include<algorithm>
#include<map>
using namespace std;

class ScrabbleWordGenerator{

private:
  
  vector<string> allWords;
  string file;
  std::map<int,string> scoreWordMap;
  int *charMultiplier;
  int *scoreOfLetter;
  char wildChar;
  
public:
  
  ScrabbleWordGenerator(string inputFile)
  {
    file = inputFile;
    charMultiplier= new int[7]{1, 2, 1, 1, 2, 1, 1};
    
    // Mapping:                 a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q,  r, s, t, u, v, w, x, y, z    
    scoreOfLetter = new int[26]{1, 3, 3, 2, 1, 4, 2, 4, 1, 8, 5, 1, 3, 1, 1, 3, 10 ,1 ,1 ,1 ,1 ,4 ,4 ,8 ,4 ,10};
    wildChar = '_';
  }

  void readDictionary()
  {
    ifstream wordsFile(file);	
    string currentWord;
    
    if(wordsFile.is_open())
      {
	while(getline(wordsFile, currentWord))
	  {
	    allWords.push_back(currentWord);
	  }
	wordsFile.close();
      }

  }
  
  int generateScore(string word)
  {
    int score = 0;    
    
    for(int i = 0; i < word.size(); i++)
      score += scoreOfLetter[word[i] - 'a'];

    return score;

  }
  
  void addToMap(int key, string value)
  {

    if(scoreWordMap.find(key) == scoreWordMap.end())
      scoreWordMap[key] = value;
    else
      scoreWordMap[key] = scoreWordMap[key] + " " + value;
	
  }

  int getWildCharCount(string character){
    int count = 0;
    
    for(int i = 0; i < character.size(); i++){
      if(character[i] == wildChar)
	count++;
    } 
    
    return count;
  }

  int getCountofDistinctChars(string one, string two)
  {
    int count = 0;
    int hash[26] = { 0 };
    
    for(int i = 0; i < one.size(); i++)
      hash[one[i] - 'a']++;

    for(int i = 0; i < two.size(); i++)
      hash[two[i] - 'a']--;

    for(int i = 0; i < 26; i++) {
      if(hash[i] < 0)
	count += -hash[i];
    }

    return count;
  }

  string getCommonCharacters(string one, string two)
  {
    
    string commonChars = "";
    int hash1[26] = { 0 };
    int hash2[26] = { 0 };
    
    for(int i = 0; i < one.size(); i++)
      hash1[one[i] - 'a']++;

    for(int i = 0; i < two.size(); i++)
      hash2[two[i] - 'a']++;

    for(int i = 0; i < 26; i++){
      
      if(hash1[i] != 0 && hash2[i] != 0) {
	
	int commonCount = min(hash1[i], hash2[i]);
	while(commonCount-- > 0){
	  commonChars = commonChars + (char)('a' + i);
	}
	  
      }      
    }

    return commonChars;
  }
  
  void getAllPossibleWords(string characters){
   
    int wildCharCount = getWildCharCount(characters);
    
    characters.erase(std::remove(characters.begin(), characters.end(), wildChar), characters.end());    
    
    for(std::vector<string>::iterator i = allWords.begin(); i != allWords.end(); ++i)
      {
	string commonChars = getCommonCharacters(*i, characters);
	
	if(commonChars.size() == (*i).size()) {	  
	  addToMap(generateScore(*i), *i);	  
	} else if(getCountofDistinctChars(characters, *i) == wildCharCount) {	    
	    addToMap(generateScore(commonChars), *i);	    
	}
	
      } 
  }

  void printMap(){

    cout << "Possible Words:" << endl;
    for(std::map<int, string>::iterator i = scoreWordMap.begin(); i != scoreWordMap.end(); ++i) {
      cout << endl;
      cout << "Score: " << (*i).first << " Word: " << (*i).second  << "\n" << endl;
    }
    
  }
};

int main(){
  
  string filePath = "/tmp/wordList.txt";
  string characters;
  ScrabbleWordGenerator scrabble(filePath);
  
  scrabble.readDictionary();
  cin >> characters;
  scrabble.getAllPossibleWords(characters);
  scrabble.printMap();
  
}

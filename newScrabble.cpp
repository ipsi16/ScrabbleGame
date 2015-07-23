#include<iostream>
#include<fstream>
#include<vector>
#include<algorithm>
#include<string>
#include<map>
using namespace std;

int scoreOfLetter[26] = {1, 3, 3, 2, 1, 4, 2, 4, 1, 8, 5, 1, 3, 1, 1, 3, 10 ,1 ,1 ,1 ,1 ,4 ,4 ,8 ,4 ,10};

class Scrabble{

private:
  vector<string> allWords;
  string file;
  std::map<int,string> scoreWordMap;
  
public:
  Scrabble(string inputFile)
  {
    file = inputFile;
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

  int getWildCardCount(string character){
    int count = 0;
    
    for(int i = 0; i < character.size(); i++){
      if(character[i] == '_')
	count++;
    } 
    
    return count;
  }

  string removeWildCardChars(string characters){
    
    string finalString = "";
    
    for(int i = 0; i < characters.size(); i++){
      
      if(characters[i] >= 'a' && characters[i] <= 'z'){
	finalString =  finalString + characters[i];
      }
      
    }
    
    return finalString;
    
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
    
    int wildCardCount = getWildCardCount(characters);

    if(wildCardCount){
      characters = removeWildCardChars(characters);
    }     
    
    for(std::vector<string>::iterator i = allWords.begin(); i != allWords.end(); ++i)
      {
	string commonChars = getCommonCharacters(*i, characters);
	
	if(commonChars.size() == (*i).size()) {	  
	  addToMap(generateScore(*i), *i);	  
	}else if(getCountofDistinctChars(characters, *i) == wildCardCount) {	    
	    addToMap(generateScore(getCommonCharacters(*i, characters)), *i);	    
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
  Scrabble scrabble(filePath);
  
  scrabble.readDictionary();
  cin >> characters;
  scrabble.getAllPossibleWords(characters);
  scrabble.printMap();
  
}

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
using namespace std;
//This function checks if one token is compelted 
bool isBreak(char ch){
    char arr[] = {' ', ':', '+', '-' , '*', '/', '=', '<', '>' , '!', '\"', '(', ')', '{', '}', '[', ']', ',', ';', '\'' };
    for(char c: arr){
        if(ch == c) return true; 
    }
    return false;  
}
bool isDatatype(string s){
    if(s.compare("char") == 0 || s.compare("int") == 0) return true;
    return false;
}
bool isKeyword(string s){
    string arr[] = {"if", "else", "while", "return", "read", "write"};
    for(string keyword: arr){
        if(s.compare(keyword) == 0) return true; //checks both string same or not
    }
    return false;
}
bool isNumericConst(string s ){
    for(int i=0; i< s.length(); i++){
        if(!(s[i]>='0' && s[i]<='9')) return false; 
    }
    return true;

}
bool isIdentifier(string s){
    if(s[0]>='a' && s[0]<='z' || s[0]>='A' && s[0] <='Z')
        return true;
    return false;
}
bool isArithmeticOp(string s){
    string arr[] = {"+", "-", "*", "/"};
    for(string op: arr){
        if(s.compare(op) == 0) return true;
    }
    return false;
}
bool isRelationalOp(string s){
    string arr[] = {">", ">=", "<", "<=", "==", "!="};
    for(string op: arr){
        if(s.compare(op) == 0) return true;
    }
    return false;
}
int main(){
    //Reading code from file
    string filePath;
    cout << "Enter file path of your code: ";
    getline(cin, filePath);
    ifstream in(filePath);
    if(in.fail()){
        cout << "File not found check your path and try again\n";
        return 0;
    }
    vector<string> fileData;
    while(!in.eof()){
        string line;
        getline(in, line);
        fileData.push_back(line);
    }
    in.close();
    vector<string> tokenLexeme;
    vector<string> identifiers;
    int id = 1;
    bool isComment = false;
    for(string line : fileData){
        string token = "";
        for(int i=0; i < line.length(); i++){
            if(!isComment){
                if(!isBreak(line[i])) token += line[i];
                else{
                    if(token.length()!=0){
                        if(isDatatype(token))
                            tokenLexeme.push_back("<Datatype " + token + ">");
                        else if(isKeyword(token))
                            tokenLexeme.push_back("<Keyword " + token + ">");
                        else if(token.length() == 1 && line[i] == '\'' && line[i-2] == '\''){
                            tokenLexeme.push_back("<Literal_Const " + token + ">");
                        }
                        else if(isNumericConst(token))
                            tokenLexeme.push_back("<Numeric_Const " + token + ">");
                        else if(isIdentifier(token)){
                            tokenLexeme.push_back("<ID " + to_string(id) + ">");
                            string temp = to_string(id) + ", " + token;
                            identifiers.push_back(temp);
                            id++;
                        }
                        else{
                            cout << token << " Not Defined" << endl;
                        }
                        token = "";
                    }
                    if(line[i] != ' '){
                        token = "";
                        token += line[i];
                        if(line[i] == '<' || line[i] == '>' || line[i] == '!' || line[i] == '='){
                            if(i+1 < line.length() && line[i+1] == '='){ 
                                token = token + line[i+1];
                                i++;
                            }        
                        }
                        if(i+1 < line.length() && line[i] == '>' && line[i+1] == '>'){
                            tokenLexeme.push_back("<Input_Opt>");
                            i++;
                        }
                        else if(isRelationalOp(token))
                            tokenLexeme.push_back("<Relational_Opt " + token + ">");
                        else if(i+1 < line.length() && line[i] == '/' && line[i+1] == '*'){
                            i++;
                            isComment = true;
                        }
                        else if(isArithmeticOp(token))
                            tokenLexeme.push_back("<ArithmaticOpt " + token + ">");
                        else if(line[i] == ':')
                            tokenLexeme.push_back("<Variable_Dec_Opt>");
                        else if(line[i] == '(' || line[i] == ')')
                            tokenLexeme.push_back("<Paraenthesis>");
                        else if(line[i] == '{' || line[i] == '}')
                            tokenLexeme.push_back("<Bracess>");
                        else if(line[i] == '[' || line[i] == ']')
                            tokenLexeme.push_back("<Square Brackets>");
                        else if(line[i] == '=')
                            tokenLexeme.push_back("<Assignment_Opt>");
                        else if(line[i] == ';')
                            tokenLexeme.push_back("<Semicolon>");
                        else if(line[i] == ',')
                            tokenLexeme.push_back("<Comma>");
                        else{
                            if(line[i] == '"'){
                                i++;
                                for(; line[i] != '"' && i<line.length(); i++)
                                    token+= line[i];
                                token += '"';
                                tokenLexeme.push_back("<String " + token + ">");
                            }
                        }
                    }
                    token = "";
                }
            }
            else{
                if(i+1 < line.length() && line[i] == '*' && line[i+1] == '/'){
                    isComment = false;
                    i++;
                }
            }
        }
    }
    //Writing data on files
    ofstream out("words.txt");
    for(string s: tokenLexeme)
        out << s << '\n';
    out.close();
    ofstream out2("symboltable.txt");
    out2 << "ID  Identifier\n";
    for(string s: identifiers)
        out2 << s << '\n';
    out2.close();
    tokenLexeme.clear();
    identifiers.clear();
    cout << "Compilation Completed.\nCheck words.txt & systemtable.txt for output\n";
    return 0;
}
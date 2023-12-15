#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>


bool in_array(const std::string &value, const std::vector<std::string> &array)
{
    return std::find(array.begin(), array.end(), value) != array.end();
}

int check_char(char c, int line, std::string alphabet){
    if(alphabet.find(c)==std::string::npos){
        std::cerr << "Erro: " << c << "does not belong to the language alphabet" << std::endl;
        return 0;
    }else{
        return 1;
    }
}

int main(){
    std::ifstream program_template;
    program_template.open("program.txt", std::ifstream::in);

    if(program_template.is_open()){
        std::cout << "Arquivo aberto com sucesso" << std::endl;
    }
    
    char c;
    unsigned int line = 1;
    std::cout << line;

    unsigned short int comment[2] = {0, 0};

    unsigned short int erro_count = 0;

    unsigned short int states[16] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};

    unsigned short int accept_states[12] = {4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};

    unsigned short int current_state = 0;

    std::vector<std::string>misc_chars = {" ", "\t" "\n"};

    std::vector<std::string>key_words = {"program", "var", "integer", "real", "boolean", "procedure",
    "begin", "end", "if", "then", "else", "while", "do", "not"};

    std::vector<std::string>delimiters = {";", ".", "(", ")", ","};

    std::string alphabet = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789 \t\n{}();:.(),=<>+-*/";

    std::string atribution = ":=";

    std::vector<std::string>relacional_operators= {"=", "<", ">"};

    std::vector<std::string>relacional_operators2= {"<=", ">=", "<>"};

    std::vector<std::string>additive_operators = {"+", "-", "or"};

    std::vector<std::string>multiplicative_operators = {"*", "/", "and"}; 

    std::string word = "\0";

    std::string numbers = "0123456789";

    std::string letters = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";

    std::string numbersletters = letters + numbers;
    
    while(program_template.get(c)){
        std::cout << c;

        if(current_state == 0){
            word = "\0";
        }

        if(c == '\n'){
            ++line;
            std::cout << line;
        }

        if(check_char(c, line, alphabet)){
            word = word+c;
        }else {
            ++erro_count;
            continue;
        }

        switch (current_state) { 
            case 0:
                if (in_array(word, misc_chars)) current_state = 0;

                else if (word.find('{')!=std::string::npos){ 
                    current_state = 1;
                    ++comment[0];
                }

                else if (letters.find(word)!=std::string::npos) current_state = 2;
                
                else if (numbers.find(word)!=std::string::npos) current_state = 3;

                else if (in_array(word, delimiters)) current_state = 4;

                else if (in_array(word, relacional_operators)) current_state = 5;

                else if (in_array(word, additive_operators)) current_state = 6;
                 
                else if (in_array(word, multiplicative_operators)) current_state = 7;

                else if (word.find(':')) current_state = 8;

            case 1:
                if(word.back() == '}') {
                    current_state = 0;
                    ++comment[1];
                }
            
            case 2:
                if(numbersletters.find(word.back())!=std::string::npos) current_state = 2;
                else current_state = 0;
        }
    

    }

    if(comment[0]!=comment[1]){
        std::cerr << "Erro: comment unclosed" << std::endl;
    }

    return 0;
}
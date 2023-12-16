
#include <iostream>
#include <fstream>
#include <ostream>
#include <string>
#include <vector>
#include <algorithm>


bool in_array(const std::string &value, const std::vector<std::string> &array)
{
    return std::find(array.begin(), array.end(), value) != array.end();
}

int check_char(char c, unsigned int line, std::string alphabet){
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
        std::cout << "File opened successfully" << std::endl;
    }

    std::ofstream table;
    table.open("table.txt", std::ofstream::out);

    if(table.is_open()){
        std::cout << "File created successfully" << std::endl;
    }
    
    char c;
    unsigned int line = 1;
    std::cout << line;

    unsigned short int comment[2] = {0, 0};

    unsigned short int erro_count = 0;

    unsigned short int current_state = 0;

    std::vector<std::string>misc_chars = {" ", "\t" "\n"};

    std::vector<std::string>key_words = {"program", "var", "integer", "real", "boolean", "procedure",
    "begin", "end", "if", "then", "else", "while", "do", "not"};

    std::vector<std::string>delimiters = {";", ".", "(", ")", ","};

    std::string alphabet = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789 \t\n{}();:.(),=<>+-*/";

    std::string atribution = ":=";

    std::vector<std::string>relacional_operators= {"=", "<", ">"};

    std::vector<std::string>relacional_operators2= {"<=", ">=", "<>"};

    std::vector<std::string>additive_operators = {"+", "-"};

    std::vector<std::string>multiplicative_operators = {"*", "/"}; 

    std::string word = "\0";

    std::string numbers = "0123456789";

    std::string letters = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";

    std::string numbersletters = letters + numbers;
    
    while(program_template.get(c)){

        if(current_state == 0){
            word = "\0";
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

                else if (word.find(':')!=std::string::npos) current_state = 8;
                
                break;

            case 1:
                if(word.back() == '}') {
                    current_state = 0;
                    ++comment[1];
                }
                break;
            
            case 2:
                if(numbersletters.find(word.back())!=std::string::npos) current_state = 2;

                else{ 
                    current_state = 0;

                    word.pop_back();

                    if(in_array(word, key_words)){
                        table << line << ' ' << word << " key word \n";

                    }else if (word.find("and")!=std::string::npos) {
                        table << line << ' ' << word << " multiplicative operator \n";

                    }else if (word.find("or")!=std::string::npos) {
                        table << line << ' ' << word << " additive operator \n";

                    }else{
                        table << line << ' ' << word << " indentifier \n";
                    }

                    program_template.unget();
                }
                break;

            case 3:
                if (numbers.find(word.back())!=std::string::npos) current_state = 3;

                else if (word.find('.')!=std::string::npos) current_state = 9;

                else{
                    current_state = 0;
                    word.pop_back();
                    table << line << ' ' << word << " integer\n";
                    program_template.unget();
                }
                break;
            
            case 4:
                current_state = 0;
                word.pop_back();
                program_template.unget();
                table << line << ' ' << word << " delimiter\n";
                break;
            
            case 5:
                current_state = 0;
                program_template.get(c);
                word += c;
                if(in_array(word, relacional_operators2)){
                    table << line << ' ' << word << " relational operator\n";
                }else{
                    word.pop_back();
                    program_template.unget();
                    table << line << ' ' << word << " relational operator\n";
                }
                break;

            case 6:
                current_state = 0;
                word.pop_back();
                program_template.unget();
                table << line << ' ' << word << " additive operator\n";
                break;
            
            case 7:
                current_state = 0;
                word.pop_back();
                program_template.unget();
                table << line << ' ' << word << " multiplicative operator\n";
                break;
            
            case 8:
                current_state = 0;
                program_template.get(c);
                word = word + c;
                if(word.find(":=")!=std::string::npos){
                    word.pop_back();
                    program_template.unget();
                    table << line << ' ' << word << " atribution\n"; 
                }else {
                    word.pop_back();
                    program_template.unget();
                    table << line << ' ' << word << " delimiter\n";
                }
                break;

            case 9:
                if(numbers.find(word.back())!=std::string::npos){
                    current_state = 9;
                }else {
                    current_state = 0;
                    word.pop_back();
                    program_template.unget();
                    table << line << ' ' << word << " real\n";
                }
                break;
        }

        if(word.back() == '\n' && current_state==0){
            ++line;
            std::cout << line;
        }
    }

    if(comment[0]!=comment[1]){
        std::cerr << "Erro: comment unclosed" << std::endl;
    }

    return 0;
}
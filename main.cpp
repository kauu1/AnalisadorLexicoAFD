#include <iostream>
#include <fstream>

int main(){
    std::ifstream program_template;
    program_template.open("program.txt", std::ifstream::in);

    if(program_template.is_open()){
        std::cout << "Arquivo aberto com sucesso" << std::endl;
    }

    char c;
    unsigned int line = 1;
    std::cout << line;

    while(program_template.get(c)){

        

        std::cout << c;

        if(c == '\n'){
            ++line;
            std::cout << line;
            
        }
    }
    return 0;
}
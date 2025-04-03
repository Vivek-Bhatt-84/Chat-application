#include <iostream>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>

using namespace std;

#define PORT 8080

int main() {
    int client_socket;
    struct sockaddr_in server_addr;
    char buffer[1024];

    // Create socket
    client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket == -1) {
        cerr << "❌ Socket creation failed!\n";
        return -1;
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    // Connect to server
    if (connect(client_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        cerr << "❌ Connection failed!\n";
        return -1;
    }

    cout << "✅ Connected to chat server!\n";

    // Authentication
    memset(buffer, 0, sizeof(buffer));
    recv(client_socket, buffer, sizeof(buffer), 0);
    cout << buffer; // Prompt for username
    string username;
    getline(cin, username);
    send(client_socket, username.c_str(), username.length(), 0);

    memset(buffer, 0, sizeof(buffer));
    recv(client_socket, buffer, sizeof(buffer), 0);
    cout << buffer; // Prompt for password
    string password;
    getline(cin, password);
    send(client_socket, password.c_str(), password.length(), 0);

    memset(buffer, 0, sizeof(buffer));
    recv(client_socket, buffer, sizeof(buffer), 0);
    cout << buffer << endl; // Authentication result

    // **STEP 1: Receive and display offline messages**
    memset(buffer, 0, sizeof(buffer));
    int bytesReceived = recv(client_socket, buffer, sizeof(buffer), 0);
    if (bytesReceived > 0) {
        cout << "\n📩 Offline Messages:\n" << buffer << endl;
    }

    // **STEP 2: Chat Loop**
    while (true) {
        cout << "You: ";
        string message;
        getline(cin, message);

        if (message == "/exit") {
            cout << "👋 Disconnecting from chat...\n";
            break;
        }

        send(client_socket, message.c_str(), message.length(), 0);

        memset(buffer, 0, sizeof(buffer));
        int bytesReceived = recv(client_socket, buffer, sizeof(buffer), 0);
        if (bytesReceived <= 0) {
            cout << "⚠️ Server disconnected! Exiting...\n";
            break;
        }

        cout << "📩 New message: " << buffer << endl;
    }

    close(client_socket);
    return 0;
}

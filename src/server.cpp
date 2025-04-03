#include <iostream>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>
#include <map>
#include "../include/userDatabase.h"

using namespace std;

#define PORT 8080
#define MAX_CLIENTS 10

map<int, string> clients;
map<string, int> onlineUsers;

void *handleClient(void *socket_desc) {
    int client_socket = *(int *)socket_desc;
    char buffer[1024];

    UserDatabase db;
    string username, password;

    send(client_socket, "Enter username: ", 16, 0);
    recv(client_socket, buffer, sizeof(buffer), 0);
    username = string(buffer);

    send(client_socket, "Enter password: ", 16, 0);
    recv(client_socket, buffer, sizeof(buffer), 0);
    password = string(buffer);

    if (!db.loginUser(username, password)) {
        send(client_socket, "❌ Authentication failed! Disconnecting...\n", 42, 0);
        close(client_socket);
        return nullptr;
    }

    send(client_socket, "✅ Login successful!\n", 22, 0);
    cout << "🔐 User " << username << " logged in!\n";

    vector<string> messages = db.getOfflineMessages(username);
    if (!messages.empty()) {
        send(client_socket, "📩 You have new messages:\n", 30, 0);
        for (const string &msg : messages) {
            send(client_socket, (msg + "\n").c_str(), msg.length() + 1, 0);
        }
    }

    clients[client_socket] = username;
    onlineUsers[username] = client_socket;

    while (true) {
        memset(buffer, 0, sizeof(buffer));
        int bytesReceived = recv(client_socket, buffer, sizeof(buffer), 0);

        if (bytesReceived <= 0) {
            cout << "❌ Client " << username << " disconnected\n";
            onlineUsers.erase(username);
            clients.erase(client_socket);
            close(client_socket);
            return nullptr;
        }

        string message(buffer);
        cout << "📩 " << username << " sent: " << message << endl;

        if (message[0] == '@') {
            size_t spacePos = message.find(' ');
            if (spacePos != string::npos) {
                string recipient = message.substr(1, spacePos - 1);
                string privateMsg = message.substr(spacePos + 1);

                if (onlineUsers.find(recipient) != onlineUsers.end()) {
                    int recipientSocket = onlineUsers[recipient];
                    send(recipientSocket, privateMsg.c_str(), privateMsg.length(), 0);
                } else {
                    db.storeMessage(recipient, username, privateMsg);
                }
            }
        }
    }
    return nullptr;
}

void startServer() {
    int server_socket, new_socket;
    struct sockaddr_in server_addr, client_addr;
    socklen_t addr_size = sizeof(client_addr);

    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == -1) {
        cerr << "❌ Socket creation failed!\n";
        return;
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    if (bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        cerr << "❌ Bind failed!\n";
        return;
    }

    if (listen(server_socket, MAX_CLIENTS) < 0) {
        cerr << "❌ Listen failed!\n";
        return;
    }

    cout << "🚀 Chat Server running on port " << PORT << "\n";

    while (true) {
        new_socket = accept(server_socket, (struct sockaddr *)&client_addr, &addr_size);
        if (new_socket < 0) {
            cerr << "❌ Accept failed!\n";
            continue;
        }

        pthread_t thread;
        pthread_create(&thread, nullptr, handleClient, (void *)&new_socket);
    }

    close(server_socket);
}


int main() {
    startServer();
    return 0;
}

#ifndef NAME_SERVER_H
#define NAME_SERVER_H

#include "tree.h"


#define SERVER_BACKLOG (10) // the number of clients that can be queued
#define SERVER_DEF_PORT "6958" // the default port the server will listen on

#define CLIENT_PEER_DEF_PORT "4758" // default client port

#define SERVER_MAX_MESSAGE (512) // the maximum message size in bytes

#define PEER_POOL_SIZE (5) // the default peer pool size to send to the client


struct _client {
	int socket_fd; // the socket descriptor for this client
	struct sockaddr_storage client_addr; // struct storing the client address
	char address[NI_MAXHOST]; // the string containing the address of the client
	char port[NI_MAXSERV]; // the string containing the port of the client
	pthread_t* client_thread; // pointer to the thread that this client is executing on

};

typedef struct _client client;

/** Prints the usage of this program
*/
void print_usage();

/** Thread function for handling a new client that has connected to the server
	@param arg Pointer to a client struct, represnting the client that has connected
*/

void* client_handle(void* arg);

/** Creates a set of peers for the specified client, and sends them to the client
	@param client_o A pointer to a client structure in which to send the peers to
*/

void client_send_peers(client* client_o, node* graph);

/** Crafts the peer message to send to the client when the number o peers is above the peer threshold
	@param client_o The client who is requesting the peers
	@param max_msg_size  The maximum size of the message
	@return A pointer to a cstring containing the list of peers, seperated by a space,
		should be freed after use
*/

char* client_peers_rand(client* client_o, int max_msg_size, node* graph);

/** Returns the peer message to send to the client when the number of peers
	is below the peer threshold
	@param client_o The client who is requesting the peers
	@param max_msg_size  The maximum size of the message
	@return A pointer to a cstring containing the list of peers, seperated by a space,
		should be freed after use
*/

char* client_peers_static(client* client_o, int max_msg_size);

/** Sends the message in msg to the given socket
	@param socket_fd the Socket descriptor of the client to send the message to
	@param Pointer to the message to send
	@param size The size of the message being sent
	@return 0 if sucess, error code other wise */

int server_send_message(int socket_fd, void* msg, int size);

/** Determines if the string a starts with the string b,
	@param a cstring a
	@param b cstring b
	@return 1 if a starts with b, 0 otherwise
*/

int str_starts_with(const char* a, const char* b); 

/** Sets up the socket the server will use to listen on for new clients
	@param port cstring that contains the port to start the server on
	@return The descriptor of the socket that was created, or -1 if it failed
*/

int init_server(char* port);

/** Listens for clients 
	@param socket_fd The descriptor of the socket to listen on	
*/

void listen_for_clients(int socket_fd);

/** Assign all nodes in list to an array in random order, 
	determine peers, and then send those peers to the nodes
	
*/

void manage_graph();


#endif

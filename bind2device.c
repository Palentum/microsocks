#define _GNU_SOURCE
#define _DARWIN_C_SOURCE
#include <errno.h>
#include <netinet/in.h>
#include <net/if.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>

#if defined(IP_BOUND_IF) /* macOS/BSD */
int bind2device(int sockfd, int socket_family, const char* device) {
	unsigned int ifindex = if_nametoindex(device);
	if(ifindex == 0) return -1;
	if(socket_family == AF_INET6)
		return setsockopt(sockfd, IPPROTO_IPV6, IPV6_BOUND_IF, &ifindex, sizeof(ifindex));
	return setsockopt(sockfd, IPPROTO_IP, IP_BOUND_IF, &ifindex, sizeof(ifindex));
}
#elif defined(SO_BINDTODEVICE) /* Linux */
int bind2device(int sockfd, int socket_family, const char* device) {
	(void)socket_family;
	return setsockopt(sockfd, SOL_SOCKET, SO_BINDTODEVICE, device, strlen(device));
}
#else
#pragma message("warning: bind2device not supported on this platform")
int bind2device(int sockfd, int socket_family, const char* device) {
	(void)sockfd; (void)socket_family; (void)device;
	errno = ENOSYS;
	return -1;
}
#endif

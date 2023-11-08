/* Written from scratch, but kernel-to-user space API usage
* dissected from lolpcap:
*  Copyright 2011, Chetan Loke <loke.chetan@gmail.com>
*  License: GPL, version 2.0
*/
// http://www.microhowto.info/howto/capture_ethernet_frames_using_an_af_packet_ring_buffer_in_c.html
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <assert.h>
#include <net/if.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <poll.h>
#include <unistd.h>
#include <signal.h>
#include <inttypes.h>
#include <sys/socket.h>
#include <sys/mman.h>
#include <linux/if_packet.h>
#include <linux/if_ether.h>
#include <linux/ip.h>

#include <iostream>

int main() {

    int fd = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ALL));
    if (fd == -1) {
        perror("socket");
        exit(1);
    }

    struct tpacket_req req = {0};
    req.tp_frame_size =  1 << 22; //TPACKET_ALIGN(TPACKET_HDRLEN + ETH_HLEN);// + TPACKET_ALIGN(SNAPLEN);
    req.tp_block_size = sysconf(_SC_PAGESIZE);
    while (req.tp_block_size < req.tp_frame_size) {
        req.tp_block_size <<= 1;
    }
    req.tp_block_nr = sysconf(_SC_PHYS_PAGES) * sysconf(_SC_PAGESIZE) / (2 * req.tp_block_size);
    size_t frames_per_buffer = req.tp_block_size / req.tp_frame_size;
    req.tp_frame_nr = req.tp_block_nr * frames_per_buffer;
    if (setsockopt(fd, SOL_PACKET, PACKET_RX_RING, &req, sizeof(req))==-1) {
        perror("setsockopt");
        exit(1);
    }

    size_t rx_ring_size = req.tp_block_nr * req.tp_block_size;
    char* rx_ring = (char*)mmap(0, rx_ring_size, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);

    struct pollfd fds[1] = {0};
    fds[0].fd = fd;
    fds[0].events = POLLIN;
    size_t frame_idx = 0;
    char* frame_ptr = rx_ring;

    while (true) {
        struct tpacket_hdr* tphdr = (struct tpacket_hdr*)frame_ptr;
        while (!(tphdr->tp_status & TP_STATUS_USER)) {
            if (poll(fds, 1, -1) == -1) {
                perror("poll");
                exit(1);
            }
        }

        struct sockaddr_ll* addr = (struct sockaddr_ll*)(frame_ptr + TPACKET_HDRLEN - sizeof(struct sockaddr_ll));
        char* l2content = frame_ptr + tphdr->tp_mac;
        char* l3content = frame_ptr + tphdr->tp_net;
        // handle_frame(tphdr, addr, l2content, l3content);

        std::cout << l2content << ' ' << l3content << '\n';

        frame_idx = (frame_idx + 1) % req.tp_frame_nr;
        int buffer_idx = frame_idx / frames_per_buffer;
        char* buffer_ptr = rx_ring + buffer_idx * req.tp_block_size;
        int frame_idx_diff = frame_idx % frames_per_buffer;
        frame_ptr = buffer_ptr + frame_idx_diff * req.tp_frame_size;
    }



    return 0;
}
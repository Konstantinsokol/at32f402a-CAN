#include <iostream>
#include <cstring>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <poll.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <sys/select.h>
#include <sys/time.h>
#include <net/if.h>
#include <linux/can.h>
#include <linux/can/raw.h>

//char* decodeMessage(can_frame frame) {
//
//		char* command = (char*)calloc(8, 1);
//		for(int i = 0; i < frame.can_dlc; i++)
//			command[i] = (char)frame.data[i];
//
//         return command;
//}

bool pollIn(int fd)
{
    struct pollfd fds[1];
    fds[0].fd = fd;
    fds[0].events = POLLIN;
    int pollReturn = poll(fds, 1, 500);;

    if (pollReturn > 0)
    {
        if (fds[0].revents & POLLIN)
        {
            fds[0].revents = 0;
            return true;
        }
    }
    return false;
}


void Send_CAN_Float(u_int32_t ID, u_int8_t DLC, float &curr_l, float &curr_r, int s) {
    can_frame frame;
    frame.can_id = ID;
    frame.can_dlc = DLC;
    memcpy(frame.data, &curr_l, sizeof(float));
    memcpy(frame.data + sizeof(float), &curr_r, sizeof(float));
    //memcpy(frame.data, Data, DLC);
    write(s, &frame, sizeof(struct can_frame));
}

int main() {
    int s;
    struct sockaddr_can addr;
    struct can_frame frame;
    struct ifreq ifr;

    const char *ifname = "vcan0";
    std::system("sudo ip link add dev vcan0 type vcan && sudo ip link set up vcan0");
     //std::system("sudo ip link set can0 type can bitrate 1000000 && sudo ip link set up can0");
    if ((s = socket(PF_CAN, SOCK_RAW, CAN_RAW)) < 0) {
        perror("Error while opening socket");
        return -1;
    }

    strcpy(ifr.ifr_name, ifname);
    ioctl(s, SIOCGIFINDEX, &ifr);

    addr.can_family = AF_CAN;
    addr.can_ifindex = ifr.ifr_ifindex;

    if (bind(s, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
        perror("Error in socket bind");
        return -2;
    }

     // Установка неблокирующего режима чтения и записи
    int flags = fcntl(s, F_GETFL, 0);
    fcntl(s, F_SETFL, flags | O_NONBLOCK);
    
    float x = 3.44;
    float y = 5.22;

    u_int32_t id = 0x111;
    int nbytes;
    bool flag1 = true;

    while (1) {
        

        usleep(5000000);

        Send_CAN(id, 8, x, y, s);
       
        usleep(5000000);
       
    }

    close(s);
    return 0;
}

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

void Send_CAN_Float(u_int32_t ID, u_int8_t DLC, float &curr_l, float &curr_r, int socket) {
    can_frame frame;
    frame.can_id = ID;
    frame.can_dlc = DLC;
    memcpy(frame.data, &curr_l, sizeof(float));
    memcpy(frame.data + sizeof(float), &curr_r, sizeof(float));
    //memcpy(frame.data, Data, DLC);
    write(socket, &frame, sizeof(struct can_frame));
}


void Send_CAN(u_int32_t ID, u_int8_t DLC, u_int8_t Data, int socket) {
    can_frame frame;
    frame.can_id = ID;
    frame.can_dlc = DLC;
    frame.data[0] = Data;
    //memcpy(frame.data, Data, DLC);
    write(socket, &frame, sizeof(struct can_frame));
}

int main() {
    int s;
    struct sockaddr_can addr;
    struct can_frame frame;
    struct ifreq ifr;

    const char *ifname = "can0";//при необходимости менять на vcan0, если нужны тесты на виртуальном
    //std::system("sudo ip link add dev vcan0 type vcan && sudo ip link set up vcan0"); для тестов на виртуальном
    std::system("sudo ip link set can0 type can bitrate 1000000 && sudo ip link set up can0");
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
    
    u_int8_t data1 = 1;
    u_int32_t id = 0x111;
    u_int8_t data2 = 2;
    int nbytes;
    bool flag1 = true;

    while (1) {
        
        if(pollIn(s))
         {
        u_int8_t command;
        u_int8_t package1[4];
        u_int8_t package2[4];


        nbytes = read(s, &frame, sizeof(struct can_frame));
        if (nbytes < 0) {
            perror("Error while reading from socket");
            return -3;
        }


        else 
        {
            if(frame.can_dlc == 1)
                command = frame.data[0];

            else
            {
                memcpy(package1, frame.data, 4);
                memcpy(package2, frame.data + 4, 4);
            }

        float& current_l = (float&)(package1);
        float& current_r = (float&)(package2);
        
        printf("%f\n", current_l);
        printf("%f\n", current_r);
        printf("\n");
        }
        }
         
        //Send_CAN(id, 1, data1, s); отправить команду data1

        float x = 3.44;
        float y = 5.22;        

        //Send_CAN_Float(id, 8, x, y, s); если отправляем float
       
        usleep(5000000);
       
    }

    close(s);
    return 0;
}

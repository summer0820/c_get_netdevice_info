#include <stdio.h>
#include <string.h>
#include <net/if.h>
#include <sys/ioctl.h>
#include <arpa/inet.h>
#include <errno.h>
#include <sys/socket.h>

#define netcard_num 16  //网卡数量

int get_netdevice_info()
{
    int fd;
    int interface_num = 0;
    struct ifreq ifr_buf[netcard_num];  //网卡信息
    struct ifconf ifc;  //用于取回网卡接口信息
    char mac_addr[16];  //mac地址
    char ip_addr[32];   //ip地址
    char broad_addr[32];    //广播地址
    char mask_addr[32];     //子网掩码

    memset(mac_addr,0,16);  //置0
    memset(ip_addr,0,32);
    memset(broad_addr,0,32);
    memset(mask_addr,0,32);

    if((fd = socket(AF_INET,SOCK_DGRAM,0)<0)
       {
            perror("open socket failed");
            close(fd);
            return -1;
       }

    ifc.ifc_len = sizeof(ifr_buf);
    ifc.ifcu_buf = (__caddr_t)ifr_buf;
    if ((ioctl(fd, SIOCGIFCONF, (char *)&ifc))<0)
    {
        perror("ioctl failed");
    }


}

int main(void)
{
    get_netdevice_info();
    return 0;
}


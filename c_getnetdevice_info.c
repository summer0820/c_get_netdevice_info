#include <stdio.h>
#include <string.h>
#include <net/if.h>
#include <sys/ioctl.h>
#include <arpa/inet.h>
#include <errno.h>
#include <sys/socket.h>
#include <unistd.h>
#define netcard_num 16  //网卡数量

int get_netdevice_info()
{
    int fd;
    int interface_num = 0;              //网卡设备个数，eg:eth0 lo
    struct ifreq ifr_buf[netcard_num];  //单个网卡接口信息
    struct ifreq ifr_buf_copy;           //用于保存存活网卡接口信息
    struct ifconf ifc;                  //用于取回网卡所有信息
    char mac_addr[32];                  //mac地址
    char ip_addr[32];                   //ip地址
    char broad_addr[32];                //广播地址
    char mask_addr[32];                 //子网掩码

    memset(mac_addr,0,32);
    memset(ip_addr,0,32);
    memset(broad_addr,0,32);
    memset(mask_addr,0,32);

    if((fd = socket(AF_INET,SOCK_DGRAM,0))<0)   //
       {
            perror("open socket failed");
            close(fd);
            return -1;
       }

    ifc.ifc_len = sizeof(ifr_buf);
    ifc.ifc_buf = (__caddr_t)ifr_buf;
    if ((ioctl(fd, SIOCGIFCONF, (char *)(&ifc)))<0)
    {
        perror("ioctl failed");
    }
    else
    {
        interface_num = ifc.ifc_len/sizeof(struct ifreq);
        printf("netcard interface num is %d\n",interface_num);

        while(interface_num--)
        {
            if(strcmp(ifr_buf[interface_num].ifr_name,"lo") == 0)
            {
                continue;
            }
            printf("\ndevice name: %s\n",ifr_buf[interface_num].ifr_name);

            ifr_buf_copy = ifr_buf[interface_num];      //利用ifr_buf_copy保留存活的接口
            if(ioctl(fd,SIOCGIFFLAGS,(char *)(&ifr_buf_copy))<0)
            {
                printf("ioctl: %s [%s:%d]\n",strerror(errno),__FILE__, __LINE__);
                close(fd);
                return -1;
            }

            if(ioctl(fd,SIOCGIFHWADDR,(char *)(&ifr_buf[interface_num]))<0)     //
            {
                printf("ioctl: %s [%s:%d]\n",strerror(errno),__FILE__, __LINE__);
                close(fd);
                return -1;
            }
            else
            {
                memset(mac_addr,0,sizeof(mac_addr));
                snprintf(mac_addr, sizeof(mac_addr), "%02X:%02X:%02X:%02X:%02X:%02X",
                            (unsigned char)ifr_buf[interface_num].ifr_hwaddr.sa_data[0],
                            (unsigned char)ifr_buf[interface_num].ifr_hwaddr.sa_data[1],
                            (unsigned char)ifr_buf[interface_num].ifr_hwaddr.sa_data[2],
                            (unsigned char)ifr_buf[interface_num].ifr_hwaddr.sa_data[3],
                            (unsigned char)ifr_buf[interface_num].ifr_hwaddr.sa_data[4],
                            (unsigned char)ifr_buf[interface_num].ifr_hwaddr.sa_data[5]);
                printf("device %s mac address is %s\n",ifr_buf[interface_num].ifr_name,mac_addr);

            }

            if(ioctl(fd,SIOCGIFADDR,(char *)(&ifr_buf[interface_num]))<0)
            {
                printf("ioctl: %s [%s:%d]\n",strerror(errno),__FILE__, __LINE__);
                close(fd);
                return -1;
            }
            else
            {
                snprintf(ip_addr,sizeof(ip_addr),"%s",(char *)(inet_ntoa(((struct sockaddr_in *)&(ifr_buf[interface_num].ifr_addr))->sin_addr)));
                printf("device %s ip address is %s\n",ifr_buf[interface_num].ifr_name,ip_addr);
            }
        }



    }

    return 0;
}

int main(void)
{
    get_netdevice_info();
    return 0;
}


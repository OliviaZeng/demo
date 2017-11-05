#include <iostream>
#include <string>
#include "IM_TEST.pb.h" 
#include <errno.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

using namespace std;

#define SOCKET_INVALID -1

string g_ip = "192.168.202.26";
short g_port = 5555;
char out_buf[10*1024*1024] = {'\0'};
int out_len = 0;

static int create_socket(const char *address, short port, int *netfd)
{
	*netfd = SOCKET_INVALID;    
	if ((*netfd = socket(AF_INET, SOCK_STREAM, 0)) == -1)    
	{        
		return -1;    
	}	

	sockaddr_in mysock;
	memset((char*)&mysock, sizeof(mysock), 0);	
	mysock.sin_family      = AF_INET;	
	mysock.sin_port        = htons(port);	
	mysock.sin_addr.s_addr = inet_addr(address); 	
	socklen_t addrlen = sizeof(struct sockaddr_in);   

	printf("connect, fd:%d adddress:%s port:%d\n", *netfd, address, port);
	
	int ret = ::connect(*netfd, (struct sockaddr *)&mysock, addrlen);	
	if (ret < 0)
	{
		printf("connect fail, reason:%s\n", strerror(errno));
		*netfd = SOCKET_INVALID;  
		return -1;
	}
	
	printf("connect suc\n");
	
	return 0;
}

static void destory_socket(int *netfd)
{
	printf("close socket, fd:%d\n", *netfd);
	::close(*netfd);
	*netfd = SOCKET_INVALID;
}

static int send_msg(int *netfd, char *data, int len)
{
	if (len <= 0)
	{
		printf("len <= 0\n");
		return -1;
	}
	
	if (data == NULL)
	{
		printf("data is null\n");
		return -1;
	}
	
	printf("send msg, fd:%d len:%d\n", *netfd, len);

	char *send_buf = data;
	int send_len = len;
	while (send_len > 0)
	{
		int cur_send_len = ::send(*netfd, send_buf, send_len, 0);
		if (cur_send_len == -1)
		{
			if ((errno == EINTR) || (errno == EAGAIN))
			{
				continue;
			}
			else
			{
				printf("send error, reasion:%s\n", strerror(errno));
				destory_socket(netfd);
				return -1;
			}
		} 
		
		send_len -= cur_send_len;
		send_buf += cur_send_len;
	}
	
	printf("send suc\n");
	
	return 0;
}

#pragma pack(1)
struct PBHeader
{
 int len;
 int cmd;
 char type;
};
#pragma pack()

int encode(char *buf, int len)
{
	printf("head len=%ld, body len=%d\n", sizeof(struct PBHeader), len);
	out_len = len + sizeof(struct PBHeader);

	int send_len = htonl(out_len);
	int cmd = htonl(0x8000);
	char type = 0;
	memcpy(out_buf, &send_len, sizeof(send_len));
	memcpy(out_buf+offsetof(struct PBHeader, cmd), &cmd, sizeof(cmd));
	memcpy(out_buf+offsetof(struct PBHeader, type), &type, sizeof(type));
	memcpy(out_buf+sizeof(struct PBHeader), buf, len);

	return 0;
}

int get_head_len()
{
	return sizeof(struct PBHeader);
}

int get_packet_len(const char *buf)
{
	struct PBHeader *head = (struct PBHeader *)buf;
	int cmd = ntohl(head->cmd);	
	int len = ntohl(head->len);
	int type = head->type;
	printf("cmd:0x%02x len:%d type:%d\n", cmd, len, type);

	return len;
}

int recv_msg(int *netfd, char *data, int *len)
{
	int sum_recv_len = 0;
	char recv_buf[1024*1024] = {'\0'};
	while (true)
	{
		int cur_recv_len = ::recv(*netfd, &recv_buf[sum_recv_len], sizeof(recv_buf), 0);
		if (cur_recv_len == -1)
		{
			if ((errno == EINTR) || (errno == EAGAIN))
			{
				continue;
			}
			else
			{
				printf("recv error, reasion:%s\n", strerror(errno));
                destory_socket(netfd);
                return -1; 	
		    }	
		}
		sum_recv_len += cur_recv_len;
		int msg_head_len = get_head_len();
		if (sum_recv_len < msg_head_len)
		{
			continue;	
		}
		
		int msg_len = get_packet_len(recv_buf);
		printf("msg_head_len:%d msg_len:%d cur_recv_len:%d\n", msg_head_len, msg_len, sum_recv_len);
		if (sum_recv_len < msg_len)
		{
			continue;
		}
	
		*len = msg_len - msg_head_len;	
		memcpy(data, &recv_buf[msg_head_len], *len);
		break;
	}

	printf("recv suc\n");

	return 0;
}

int global_init(int argc, char *argv[])
{
	if (argc < 3)
	{
		printf("Usage: ./test ip port\n");
		return -1;
	}

	g_ip = argv[1];
    g_port = atoi(argv[2]);	

	return 0;
}

int main(int argc, char *argv[])
{
	int ret = 0;
    if (-1 == (ret = global_init(argc, argv)))
        return -1;

	//GetUsersInfoRequest req;
	//char cid[] = { "2033499" };
	////! test
	//for(int i = 40; i < 100; ++i)
	//{
	//	snprintf(cid, sizeof(cid), "20438%d", i);
	//	req.add_cid_list(atoi(cid));
	//}

	//char buf[1024*1024] = { '\0' };
	//int len = req.ByteSize(); 
    //if (req.SerializeToArray(buf, len) < 0)
    //{   
    //    printf("encode error\n");
    //    return -1;
    //}

	//RPCIMMessage im;
	//im.set_message_name("GetUsersInfoRequest");
	//im.set_flow(111);
	//im.set_extend("extend");
	//im.set_data(buf, len);
	//
	//len = im.ByteSize();
	//if (im.SerializeToArray(buf, len) < 0)
    //{   
    //    printf("encode im msg error\n");
    //    return -1; 
    //}   	

	//printf("im | size:%d info:%s\n", im.ByteSize(), im.ShortDebugString().c_str());

	char buf[1024*1024] = { '\0' };


	printf("Option C++ mar\n");
    OptionalTest t;
    t.set_k1(11111.1111);
    t.set_k2(22.22);
    t.set_k3(3333);
    t.set_k4(444444);
    t.set_k5(5555);
    t.set_k6(666666);
    t.set_k7(-7777);
    t.set_k8(-88888);
    t.set_k9(9999);
    t.set_k10(101010);
    t.set_k11(1111);
    t.set_k12(1212);
    t.set_k13(false);
    t.set_k14("abcdefghijklmn");
    t.set_k15("12345678901234567890");
    t.set_k16(Y);
	

    //t.set_k1(11111.1111);
    //t.set_k2(22.22);
    //t.set_k3(3333);
    //t.set_k4(444444);
    //t.set_k5(5555);
    //t.set_k6(666666);
    //t.set_k7(-7777);
    //t.set_k8(-88888);
    //t.set_k9(9999);
    //t.set_k10(101010);
    //t.set_k11(1111);
    //t.set_k12(1212);
    //t.set_k13(false);
    //t.set_k14("abcdefghijklmn");
    ////t.set_k15("12345678901234567890");


    int len = t.ByteSize();
    if (t.SerializeToArray(buf, len) < 0) {
        printf("serializeToArray err\n");
        return -1;
    }


	encode(buf, len);
	
	int fd = SOCKET_INVALID;
	ret = create_socket(g_ip.c_str(), g_port, &fd);
	if (ret == -1)
	{
		return -1;
	}
	
	ret = send_msg(&fd, out_buf, out_len); 
	if (ret == -1)
	{
		return -1;
	}

	recv_msg(&fd, buf, &len);

	if (t.ParseFromArray(buf, len) < 0)
	{
		printf("decode im msg error\n");
	}
	else
	{
		printf("Option C++ unmar\n");
		printf("k1=%f\n",    t.k1());
		printf("k2=%f\n",    t.k2());
		printf("k3=%d\n",    t.k3());
		printf("k4=%ld\n",   t.k4());
		printf("k5=%u\n",    t.k5());
		printf("k6=%lu\n",   t.k6());
		printf("k7=%d\n",    t.k7());
		printf("k8=%ld\n",   t.k8());
		printf("k9=%u\n",    t.k9());
		printf("k10=%lu\n",  t.k10());
		printf("k11=%d\n",   t.k11());
		printf("k12=%ld\n",  t.k12());
		printf("k13=%d\n",   t.k13());
		printf("k14=%s\n",   t.k14().c_str());
		printf("k15=%s\n",   t.k15().c_str());
		printf("k16=%d\n",   t.k16());

		printf("k17=%f\n",    t.k17());
		printf("k18=%f\n",    t.k18());
		printf("k19=%d\n",    t.k19());
		printf("k20=%ld\n",   t.k20());
		printf("k21=%u\n",    t.k21());
		printf("k22=%lu\n",   t.k22());
		printf("k23=%d\n",    t.k23());
		printf("k24=%ld\n",   t.k24());
		printf("k25=%u\n",    t.k25());
		printf("k26=%lu\n",   t.k26());
		printf("k27=%d\n",    t.k27());
		printf("k28=%ld\n",   t.k28());
		printf("k29=%d\n",    t.k29());
		printf("k30=%s\n",    t.k30().c_str());
		printf("k31=%s\n",    t.k31().c_str());
		printf("k32=%d\n",    t.k32());
	}		
	
	destory_socket(&fd);
	
	return 0;	
}


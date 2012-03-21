#ifndef SSH_TUNNELD_OPTIONS_H
#define SSH_TUNNELD_OPTIONS_H

void print_usage(const char* program_name);

void process_options(int argc, char** argv, int* nofork, char** log_filename,
                     char** remote_host, char** remote_port,
                     char** proxy_port, char** tun_port,
                     int* accept_remote);

#endif
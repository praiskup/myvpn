#include <sys/types.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <cstdlib>
#include <libconfig.h>
#include <iostream>
#include <map>
#include <boost/filesystem.hpp>

#include "config.h"

typedef std::map <std::string, std::string> TOptions;
typedef std::vector <const char *> TArgVector;

namespace fs = boost::filesystem;

void read_option(TOptions &opts, config_t *config, const char *option_name,
                 bool required=1)
{
    const char *value;
    if (!config_lookup_string(config, option_name, &value)) {
        if (required) {
            std::cerr << "mising " << option_name << " option" << std::endl;
            exit (1);
        }
        else
            return;
    }

    opts[option_name] = value;
}

int main(int argc, char **argv)
{
    TOptions options;

    config_t *cf;
    if (setuid(0) || setgid(0)) {
        std::cerr << "can't swtich to root\n";
        return 1;
    }


    cf = (config_t *)malloc(sizeof(config_t));
    config_init(cf);

    const char *sysconfdir = getenv("SYSCONFDIR");
    if (!sysconfdir)
        sysconfdir = "/etc";

    fs::path config_file (sysconfdir);
    config_file = config_file / "myvpn.cfg";

    if (!config_read_file(cf, config_file.c_str())) {
        std::cerr << config_error_file(cf) << ':'
                  << config_error_line(cf) << ": "
                  << config_error_text(cf) << std::endl;
        config_destroy(cf);
        return 1;
    }

    read_option (options, cf, "network_name");
    read_option (options, cf, "ip");
    read_option (options, cf, "server");
    read_option (options, cf, "server_port");
    read_option (options, cf, "password");
    read_option (options, cf, "mac", 0);
    read_option (options, cf, "port", 0);

    char prog[] = "/usr/sbin/edge";

    TArgVector args;
    args.push_back((char *)prog);
    args.push_back("-c");
    args.push_back(strdup(options["network_name"].c_str()));

    args.push_back("-d");
    args.push_back(strdup(options["network_name"].c_str()));

    args.push_back("-k");
    args.push_back(strdup(options["password"].c_str()));

    args.push_back("-l");
    args.push_back(strdup(
            (options["server"] + ":" + options["server_port"]).c_str()));

    args.push_back("-a");
    args.push_back(strdup(options["ip"].c_str()));

    if (options.find("mac") != options.end()) {
        args.push_back("-m");
        args.push_back(strdup(options["mac"].c_str()));
    }

    if (options.find("port") != options.end()) {
        args.push_back("-p");
        args.push_back(strdup(options["port"].c_str()));
    }

    args.push_back(NULL);

    if (!fork()) {
        const char* const a[] = {
            "killall", "edge", NULL,
        };

        execv("/usr/bin/killall", (char* const*)a);
        printf("can't killall");
        return 1;
    }

    int status;
    if (-1 == waitpid(-1, &status, 0)) {
        printf("can't wait!\n");
        return 1;
    }

    execv(prog, (char* const*)&args[0]);
    fprintf(stderr, "can't execute %s\n", (const char *)prog);
    return 1;
}

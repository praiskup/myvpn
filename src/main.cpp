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

namespace fs = boost::filesystem;

void read_option(TOptions &opts, config_t *config, const char *option_name)
{
    const char *value;
    if (!config_lookup_string(config, option_name, &value)) {
        std::cerr << "mising " << option_name << " option" << std::endl;
        exit (1);
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

    char prog[] = "/usr/sbin/edge";
    const char* const args[] = {
        (char *)prog,
        "-c", strdup(options["network_name"].c_str()),
        "-d", strdup(options["network_name"].c_str()),
        "-k", "secret",
        "-l", strdup((options["server"] + ":" + options["server_port"]).c_str()),
        "-a", strdup(options["ip"].c_str()),
        NULL
    };

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

    execv(prog, (char* const*)args);
    fprintf(stderr, "can't execute %s\n", (const char *)prog);
    return 1;
}

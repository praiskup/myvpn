bin_PROGRAMS = src/myvpn_client

src_myvpn_client_SOURCES = src/main.cpp
src_myvpn_client_LDFLAGS = -lboost_filesystem -lboost_system -lconfig

sysconf_DATA = myvpn.cfg

EXTRA_DIST = myvpn.cfg

myvpn_client_suid: src/myvpn_client
	sudo cp "$^" $@
	sudo chown root:root $@
	sudo chmod +s $@
	sudo chmod a+x $@

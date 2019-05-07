Name:           myvpn
Version:        1.1
Release:        5%{?dist}
Summary:        Connect to vpn through n2n

License:        GPLv3+
URL:            https://github.com/praiskup/myvpn
Source0:        %{name}-%{version}.tar.gz

BuildRequires:  boost-devel
BuildRequires:  gcc-c++
BuildRequires:  libconfig-devel
Requires:       n2n

%description
Using the 'n2n' toolset, connect to vpn.


%prep
%setup -q


%build
%configure
make %{?_smp_mflags}


%install
%make_install


%files
%license COPYING
%doc README
%attr(6755, root, root) %_bindir/myvpn_client
%config %_sysconfdir/myvpn.cfg


%changelog
* Tue May 07 2019 Pavel Raiskup <praiskup@redhat.com> - 1.1-5
- rebuild for new boost in f30

* Sun Dec 30 2018 Pavel Raiskup <praiskup@redhat.com> - 1.1-4
- add "password" option

* Wed Oct 31 2018 Pavel Raiskup <praiskup@redhat.com> - 1.1-3
- rebuild for new libconfig in Fedora
- BR g++

* Mon May 07 2018 Pavel Raiskup <praiskup@redhat.com> - 1.1-2
- bump for boost on f28

* Tue Sep 12 2017 Pavel Raiskup <praiskup@redhat.com> - 1.1-1
- strdup() the string.c_str() before calling execve()

* Sun Sep 03 2017 Pavel Raiskup <praiskup@redhat.com> - 1.0-2
- config file installed %%config

* Sun Sep 03 2017 Pavel Raiskup <praiskup@redhat.com> - 1.0-1
- initial packaging

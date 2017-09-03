Name:           myvpn
Version:        1.0
Release:        1%{?dist}
Summary:        Connect to vpn through n2n

License:        GPLv3+
URL:            https://github.com/praiskup/myvpn
Source0:        %{name}-%{version}.tar.gz

BuildRequires:  boost-devel
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
%attr(6755, root, root) %_bindir/myvpn_client
%_sysconfdir/myvpn.cfg



%changelog
* Sun Sep 03 2017 Pavel Raiskup <praiskup@redhat.com> - 1.0-1
- initial packaging

%define _unpackaged_files_terminate_build 1

Name: gpui
Version: 0.1.0
Release: alt1

Summary: Group policy editor
License: GPLv2+
Group: Other
Url: https://github.com/august-alt/gpui

BuildRequires(pre): cmake
BuildRequires(pre): rpm-macros-cmake
BuildRequires(pre): cmake-modules
BuildRequires(pre): gcc-c++
BuildRequires(pre): qt5-base-devel
BuildRequires(pre): qt5-tools-devel

BuildRequires: qt5-base-common
BuildRequires: doxygen

Requires: qt5-base-common
Requires: libqt5-help

Source0: %name-%version.tar

%description
Group policy editor

%prep
%setup -q

%build
%cmake -DCMAKE_INSTALL_LIBDIR=%_libdir
%cmake_build VERBOSE=1

%install
cd BUILD
%makeinstall_std

%files
%doc README.md
%doc INSTALL.md
%_bindir/gpui-main

%_libdir/libgpui-gui.so
%_libdir/libgpui-io.so
%_libdir/libgpui-model.so

%_libdir/gpui/plugins/libadml-plugin.so
%_libdir/gpui/plugins/libadmx-plugin.so
%_libdir/gpui/plugins/libreg-plugin.so
%_libdir/gpui/plugins/libspol-plugin.so

%changelog
* Tue Jun 08 2021 <august@altlinux.org> 0.1.0-alt1
- Initial build

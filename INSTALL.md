# Building and installing on ALT-Linux

# Build dependencies

On ALT-Linux distributions all build dependencies can be installed with following command:

```
apt-get install cmake rpm-macros-cmake cmake-modules gcc-c++ qt5-base-devel qt5-declarative-devel qt5-tools-devel
libsmbclient-devel libsmbclient samba-devel libldap-devel libsasl2-devel libsmbclient-devel libuuid-devel glib2-devel
libpcre-devel libkrb5-devel qt5-base-common doxygen libxerces-c-devel xsd boost-devel-headers desktop-file-utils 
ImageMagick-tools libqt-mvvm-devel xorg-xvfb xvfb-run
```

# Building with CMake and Make

```
mkdir -p build
cmake -B build -DGPUI_BUILD_TESTS=OFF .

cd build && make -j `nproc`
```


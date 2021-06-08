# Container image that runs your code
FROM alt:p9

RUN apt-get update \
&& apt-get install -y cmake rpm-macros-cmake cmake-modules gcc-c++ qt5-base-devel qt5-tools-devel qt5-base-common doxygen libxerces-c-devel rpm-build gear xsd \
&& useradd -ms /bin/bash builder && mkdir /app && chown root:builder /app

# Copies your code file from your action repository to the filesystem path `/` of the container
COPY script/build.sh /build.sh

USER builder
WORKDIR /home/builder

# Code file to execute when the docker container starts up (`build.sh`)
ENTRYPOINT ["/build.sh"]

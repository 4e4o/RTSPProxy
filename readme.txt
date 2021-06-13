git clone https://github.com/4e4o/RTSPProxy.git
cd RTSPProxy
git submodule update --init --recursive
cd ../
mkdir rtsp_server_build
cd rtsp_server_build
cmake ../RTSPProxy -DCMAKE_BUILD_TYPE=MinSizeRel .
cmake --build .

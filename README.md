git clone --recurse-submodules git@github.com:4e4o/RTSPProxy.git  
mkdir build_rtsp_proxy && cd build_rtsp_proxy  
cmake -G Ninja -DCMAKE_BUILD_TYPE=Release ../RTSPProxy/ .  
cmake --build .  
sudo cmake --install .  

sudo systemctl daemon-reload  
sudo systemctl enable rtsp_proxy.service  
sudo systemctl start rtsp_proxy.service  

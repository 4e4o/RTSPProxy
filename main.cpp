#include <set>
#include <string>
#include <signal.h>

#include <Player/PlayerProxy.h>
#include <Rtsp/RtspSession.h>
#include <Network/TcpServer.h>

using namespace std;

namespace mediakit {
namespace Rtmp {
string kPort = "12333";
}
namespace Rtsp {
string kPort = "544";
}
}

int main(/*int count, char **argv*/) {

    Logger::Instance().add(std::make_shared<ConsoleChannel>("ConsoleChannel", LogLevel::LDebug));


    TcpServer::Ptr rtspSrv(new TcpServer());
    rtspSrv->start<RtspSession>(33554, "127.0.0.1");

    //support rtmp and rtsp url
    //just support H264+AAC
        auto urlList = {"rtmp://live.hkstv.hk.lxdns.com/live/hks",
                        "rtsp://184.72.239.149/vod/mp4://BigBuckBunny_175k.mov"};

    set<PlayerProxy::Ptr> proxyMap;
    int i = 0;
    for(auto url : urlList){
        PlayerProxy::Ptr player(new PlayerProxy(DEFAULT_VHOST, "live", to_string(i++).data(),
                                                false));
        player->play(url);
        proxyMap.emplace(player);
    }

    static semaphore sem;
    signal(SIGINT, [](int) {
        InfoL << "SIGINT:exit";
        signal(SIGINT, SIG_IGN);
        sem.post();
    });

    sem.wait();

    return 0;
}

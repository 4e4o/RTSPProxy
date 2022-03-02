#include <set>
#include <string>
#include <signal.h>

#include <Player/PlayerProxy.h>
#include <Rtsp/RtspSession.h>
#include <Network/TcpServer.h>

using namespace std;

int main(int count, char **argv) {
    if (count != 2) {
        std::cout << "<config_path>" << std::endl;
        return 1;
    }

    mINI_basic<string, variant> config;
    config.parseFile(argv[1]);

    const string ip = config["main.ip"];
    const string port_str = config["main.port"];
    const int port = std::stoi(port_str);

    vector<string> sources;

    for (int i = 0 ; ; i++) {
        const string key = "url." + std::to_string(i);

        if (!config.count(key))
            break;

        sources.push_back(config[key]);
    }

#ifndef NDEBUG
    Logger::Instance().add(std::make_shared<ConsoleChannel>("ConsoleChannel", LogLevel::LDebug));
#endif

    TcpServer::Ptr rtspSrv(new TcpServer());
    rtspSrv->start<RtspSession>(port, ip);

    set<PlayerProxy::Ptr> proxyMap;
    int i = 0;
    for(auto& url : sources){
        PlayerProxy::Ptr player(new PlayerProxy(DEFAULT_VHOST, "proxy", to_string(i++).data(),
                                                false));

        if (url.rfind("udp:", 0) == 0) {
            player->setOnPlayerCreatedEvent([] (PlayerBase::Ptr ptr) {
                RtspPlayer::Ptr rtspPlayer = dynamic_pointer_cast<RtspPlayer>(ptr);

                if (rtspPlayer) {
                    rtspPlayer->setRtpType(Rtsp::eRtpType::RTP_UDP);
                }
            });

            url = url.erase(0, 4);
        }

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

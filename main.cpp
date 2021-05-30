#include <set>
#include <string>
#include <signal.h>

#include <Player/PlayerProxy.h>
#include <Rtsp/RtspSession.h>
#include <Network/TcpServer.h>

using namespace std;

int main(int count, char **argv) {
    if (count < 4) {
        std::cout << "ip port sources" << std::endl;
        return 1;
    }

    string ip(argv[1]);
    string port_str(argv[2]);
    vector<string> sources;

    for (int i = 3 ; i < count ; i++)
        sources.emplace_back(argv[i]);

    int port = std::stoi(port_str);

    Logger::Instance().add(std::make_shared<ConsoleChannel>("ConsoleChannel", LogLevel::LDebug));

    TcpServer::Ptr rtspSrv(new TcpServer());
    rtspSrv->start<RtspSession>(port, ip);

    set<PlayerProxy::Ptr> proxyMap;
    int i = 0;
    for(auto url : sources){
        PlayerProxy::Ptr player(new PlayerProxy(DEFAULT_VHOST, "proxy", to_string(i++).data(),
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

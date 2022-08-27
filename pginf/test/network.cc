#include "network.h"
#include "data_event.h"

#include <atomic>
static std::atomic_int s_recv_cnt = 0;

void
Network::Init() {

    event_work_ = std::make_shared<std::thread>([&] {
        while (!thread_exit_) {
            pginf::Pipe::Send(PipeEventType::PIPE_NETWORK_EVENT, std::make_shared<NetworkEvent>());
        }
    });

}

void
Network::RecvMapEvent(_Topic topic, std::shared_ptr<pginf::EventMeta>& event) {
    MapEvent * map_event = static_cast<MapEvent*>(event.get());
    if (!map_event)
        return ;
    
    s_recv_cnt.fetch_add(1);
}

Network::Network() {

    Init();
    // Subscribe func
    pginf::Pipe::Subscribe(PipeEventType::PIPE_MAP_EVENT, this, &Network::RecvMapEvent);
}

Network::~Network() {
    pginf::Pipe::Unsubscribe(PipeEventType::PIPE_MAP_EVENT, this, &Network::RecvMapEvent);
    thread_exit_ = true;
    if (event_work_->joinable())
        event_work_->join();
    printf("[Network] Recv: %d pieces \n", s_recv_cnt.load()); fflush(stdout); 
}

std::string
Network::get_description() const {
    return "Network plugin: handling network events";
}

std::string
Network::get_name() const {
    return "[Network]";
}
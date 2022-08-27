#include "map.h"
#include "data_event.h"

#include <atomic>
static std::atomic_int s_recv_cnt = 0;

void
Map::Init() {

    event_work_ = std::make_shared<std::thread>([&] {
        while (!thread_exit_) {
            pginf::Pipe::Send(PipeEventType::PIPE_MAP_EVENT, new MapEvent);
        }
    });

}

void
Map::RecvNetworkEvent(_Topic topic, std::shared_ptr<pginf::EventMeta>& event) {
    NetworkEvent * network_event = static_cast<NetworkEvent*>(event.get());
    if (!network_event)
        return ;
    
    s_recv_cnt.fetch_add(1); 
}

Map::Map() {

    Init();
    // Subscribe func
    pginf::Pipe::Subscribe(PipeEventType::PIPE_NETWORK_EVENT, this, &Map::RecvNetworkEvent);
}

Map::~Map() {
    // Unsubscribe func
    pginf::Pipe::Unsubscribe(PipeEventType::PIPE_NETWORK_EVENT, this, &Map::RecvNetworkEvent);
    thread_exit_ = true;
    if (event_work_->joinable())
        event_work_->join();
    printf("[Map] Recv: %d pieces \n", s_recv_cnt.load()); fflush(stdout); 
}

std::string
Map::get_description() const {
    return "Map plugin: displays 2D or 3D maps.";
}

std::string
Map::get_name() const {
    return "[Map]";
}
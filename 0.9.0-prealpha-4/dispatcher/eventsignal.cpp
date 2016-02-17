#include "eventsignal.h"


void SignalBase::disconnect(ehid_t h_id)
{
    auto it = conn_map.find(h_id);
    if(it != conn_map.end()) {
        conn_map[h_id].disconnect();
        conn_map.erase(it);
    }
}


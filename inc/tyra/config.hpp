#ifndef TYRA_CONFIG_H
#define TYRA_CONFIG_H

#include <cstdint>

namespace tyra {

    const std::size_t MAX_COMPONENT_TYPES = UINT8_MAX;
    const std::size_t MAX_ENTITIES = UINT16_MAX;
    const std::size_t MAX_EVENT_TYPES = UINT8_MAX;

    typedef std::uint16_t   EntityVersion;  // entity_id >> 16
    typedef std::uint16_t   EntityIndex;    // entity_id & 0xFFFF
    typedef std::uint32_t   EntityId;       // (entity_version << 16) | entity_index
}

#endif

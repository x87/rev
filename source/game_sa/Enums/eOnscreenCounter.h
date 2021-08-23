#pragma once

enum class eOnscreenCounter : uint16_t {
    SIMPLE = 0, // counter (%)
    LINE = 1,
    DOUBLE = 2 // counter/counter (%/%)
};

enum class eTimerDirection : bool {
    DECREASE,
    INCREASE,
};
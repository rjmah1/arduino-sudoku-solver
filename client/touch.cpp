#include "touch.h"

Touch::Touch() {
    
}

button Touch::readButtons() {
    TSPoint touch = ts.getPoint();

    // If the touch is too soft not button registered
    if (touch.z < touchconsts::MINPRESSURE ||
        touch.z > touchconsts::MAXPRESSURE) {
        return button::NONE;
    }

    // Convert x and y to on screen coords
    int16_t x = map(touch.y, touchconsts::TS_MINY,
                       touchconsts::TS_MAXY,
                       displayconsts::tft_width - 1,
                       0);
    int16_t y = map(touch.x, touchconsts::TS_MINX,
                       touchconsts::TS_MAXX,
                       displayconsts::tft_height - 1,
                       0);     

    // if (in the correct horizontal position) {
    //     if (first vert position) {
    //         return button::TOP;
    //     } else if (second vert position) {
    //         return button::MIDDLE;
    //     } else if (third vert position) {
    //         return button::BOTTOM;
    //     }
    // }
    // return buttons::NONE;
    return button::NONE;
}
#pragma once

#include <Arduino.h>

#include <U8g2lib.h>

// 'led_0', 14x14px
const unsigned char bitmap_led_0[] PROGMEM = {
    0x00, 0x00, 0xe0, 0x01, 0x10, 0x02, 0x08, 0x04, 0x08, 0x04, 0x08, 0x04, 0x08, 0x04, 0x10, 0x02,
    0xf0, 0x03, 0x00, 0x00, 0xe0, 0x01, 0x00, 0x00, 0xc0, 0x00, 0x00, 0x00};
// 'led_1', 14x14px
const unsigned char bitmap_led_1[] PROGMEM = {
    0x08, 0x04, 0xe1, 0x21, 0x12, 0x12, 0x08, 0x04, 0x08, 0x04, 0x0b, 0x34, 0x08, 0x04, 0x10, 0x02,
    0xf4, 0x0b, 0x02, 0x10, 0xe1, 0x21, 0x00, 0x00, 0xc0, 0x00, 0x00, 0x00};

// Array of all bitmaps for convenience. (Total bytes used to store images in PROGMEM = 96)
const int bitmap_led_LEN = 2;
const unsigned char *bitmap_array_led[bitmap_led_LEN] = {
    bitmap_led_0,
    bitmap_led_1};

// 'dummy', 16x16px
const unsigned char bitmap_dummy[] PROGMEM = {
    0x01, 0x80, 0xfe, 0x7f, 0x06, 0x60, 0x0a, 0x50, 0x12, 0x48, 0x22, 0x44, 0x42, 0x42, 0x82, 0x41,
    0x82, 0x41, 0x42, 0x42, 0x22, 0x44, 0x12, 0x48, 0x0a, 0x50, 0x06, 0x60, 0xfe, 0x7f, 0x01, 0x80};

// Array of all bitmaps for convenience. (Total bytes used to store images in PROGMEM = 48)
const int bitmap_dummy_LEN = 1;
const unsigned char *bitmap_array_dummy[bitmap_dummy_LEN] = {
    bitmap_dummy};
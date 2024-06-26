#pragma once

#define KC_A 0x41
#define KC_B 0x42
#define KC_C 0x43
#define KC_D 0x44
#define KC_E 0x45
#define KC_F 0x46
#define KC_G 0x47
#define KC_H 0x48
#define KC_I 0x49
#define KC_J 0x4A
#define KC_K 0x4B
#define KC_L 0x4C
#define KC_M 0x4D
#define KC_N 0x4E
#define KC_O 0x4F
#define KC_P 0x50
#define KC_Q 0x51
#define KC_R 0x52
#define KC_S 0x53
#define KC_T 0x54
#define KC_U 0x55
#define KC_V 0x56
#define KC_W 0x57
#define KC_X 0x58
#define KC_Y 0x59
#define KC_Z 0x5A
#define KC_0 0x30
#define KC_1 0x31
#define KC_2 0x32
#define KC_3 0x33
#define KC_4 0x34
#define KC_5 0x35
#define KC_6 0x36
#define KC_7 0x37
#define KC_8 0x38
#define KC_9 0x39

#define KC_NUMLOCK        0x90
#define KC_NUMPAD0        0x60
#define KC_NUMPAD1        0x61
#define KC_NUMPAD2        0x62
#define KC_NUMPAD3        0x63
#define KC_NUMPAD4        0x64
#define KC_NUMPAD5        0x65
#define KC_NUMPAD6        0x66
#define KC_NUMPAD7        0x67
#define KC_NUMPAD8        0x68
#define KC_NUMPAD9        0x69
#define KC_MULTIPLY       0x6A
#define KC_ADD            0x6B
#define KC_SEPARATOR      0x6C
#define KC_SUBTRACT       0x6D
#define KC_DECIMAL        0x6E
#define KC_DIVIDE         0x6F
#define KC_F1             0x70
#define KC_F2             0x71
#define KC_F3             0x72
#define KC_F4             0x73
#define KC_F5             0x74
#define KC_F6             0x75
#define KC_F7             0x76
#define KC_F8             0x77
#define KC_F9             0x78
#define KC_F10            0x79
#define KC_F11            0x7A
#define KC_F12            0x7B
#define KC_F13            0x7C
#define KC_F14            0x7D
#define KC_F15            0x7E
#define KC_F16            0x7F
#define KC_F17            0x80
#define KC_F18            0x81
#define KC_F19            0x82
#define KC_F20            0x83
#define KC_F21            0x84
#define KC_F22            0x85
#define KC_F23            0x86
#define KC_F24            0x87
#define KC_LSHIFT         0xA0
#define KC_RSHIFT         0xA1
#define KC_LCONTROL       0xA2
#define KC_RCONTROL       0xA3
#define KC_LMENU          0xA4
#define KC_RMENU          0xA5
#define KC_LWIN           0x5B
#define KC_RWIN           0x5C
#define KC_ESCAPE         0x1B
#define KC_SPACE          0x20
#define KC_PRIOR          0x21
#define KC_NEXT           0x22
#define KC_END            0x23
#define KC_HOME           0x24
#define KC_LEFT           0x25
#define KC_UP             0x26
#define KC_RIGHT          0x27
#define KC_DOWN           0x28
#define KC_SELECT         0x29
#define KC_PRINT          0x2A
#define KC_EXECUTE        0x2B
#define KC_SNAPSHOT       0x2C
#define KC_INSERT         0x2D
#define KC_DELETE         0x2E
#define KC_HELP           0x2F
#define KC_SHIFT          0x10
#define KC_CONTROL        0x11
#define KC_MENU           0x12
#define KC_PAUSE          0x13
#define KC_CAPITAL        0x14
#define KC_BACK           0x08
#define KC_TAB            0x09
#define KC_CLEAR          0x0C
#define KC_RETURN         0x0D

#define KC_VOLUME_MUTE         0xAD
#define KC_VOLUME_DOWN         0xAE
#define KC_VOLUME_UP           0xAF
#define KC_MEDIA_NEXT_TRACK    0xB0
#define KC_MEDIA_PREV_TRACK    0xB1
#define KC_MEDIA_STOP          0xB2
#define KC_MEDIA_PLAY_PAUSE    0xB3
#define KC_LAUNCH_MAIL         0xB4

#define KC_BROWSER_BACK        0xA6
#define KC_BROWSER_FORWARD     0xA7
#define KC_BROWSER_REFRESH     0xA8
#define KC_BROWSER_STOP        0xA9
#define KC_BROWSER_SEARCH      0xAA
#define KC_BROWSER_FAVORITES   0xAB
#define KC_BROWSER_HOME        0xAC

#define M_LEFT 0
#define M_RIGHT 1

#define EXTENDED_KEYS \
  KC_RCONTROL, KC_RMENU, KC_INSERT, KC_DELETE, KC_HOME, \
  KC_END, KC_PRIOR, KC_NEXT, KC_UP, KC_DOWN, \
  KC_LEFT, KC_RIGHT, KC_NUMLOCK, KC_DIVIDE, KC_SNAPSHOT, \
  KC_LWIN, KC_RWIN, KC_VOLUME_MUTE, \
  KC_VOLUME_DOWN, KC_VOLUME_UP, KC_MEDIA_NEXT_TRACK, \
  KC_MEDIA_PREV_TRACK, KC_MEDIA_STOP, KC_MEDIA_PLAY_PAUSE, KC_LAUNCH_MAIL, \
  KC_BROWSER_BACK, KC_BROWSER_FORWARD, KC_BROWSER_REFRESH, KC_BROWSER_STOP, \
  KC_BROWSER_SEARCH, KC_BROWSER_FAVORITES, KC_BROWSER_HOME

#define EXTENDED_KEYS_SIZE 31

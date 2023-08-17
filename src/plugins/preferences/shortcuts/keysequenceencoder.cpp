/***********************************************************************************************************************
**
** Copyright (C) 2023 BaseALT Ltd. <org@basealt.ru>
**
** This program is free software; you can redistribute it and/or
** modify it under the terms of the GNU General Public License
** as published by the Free Software Foundation; either version 2
** of the License, or (at your option) any later version.
**
** This program is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU General Public License for more details.
**
** You should have received a copy of the GNU General Public License
** along with this program; if not, write to the Free Software
** Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
**
***********************************************************************************************************************/

#include "keysequenceencoder.h"

#include "../../../core/logger/log.h"

const uint32_t VK_CANCEL     = 0x03;
const uint32_t VK_BACK       = 0x08;
const uint32_t VK_TAB        = 0x09;
const uint32_t VK_CLEAR      = 0x0C;
const uint32_t VK_RETURN     = 0x0D;
const uint32_t VK_SHIFT      = 0x10;
const uint32_t VK_CONTROL    = 0x11;
const uint32_t VK_MENU       = 0x12;
const uint32_t VK_PAUSE      = 0x13;
const uint32_t VK_CAPITAL    = 0x14;
const uint32_t VK_ESCAPE     = 0x1B;
const uint32_t VK_MODECHANGE = 0x1F;
const uint32_t VK_SPACE      = 0x20;
const uint32_t VK_PRIOR      = 0x21;
const uint32_t VK_NEXT       = 0x22;
const uint32_t VK_END        = 0x23;
const uint32_t VK_HOME       = 0x24;
const uint32_t VK_LEFT       = 0x25;
const uint32_t VK_UP         = 0x26;
const uint32_t VK_RIGHT      = 0x27;
const uint32_t VK_DOWN       = 0x28;
const uint32_t VK_SELECT     = 0x29;
const uint32_t VK_PRINT      = 0x2A;
const uint32_t VK_EXECUTE    = 0x2B;
const uint32_t VK_SNAPSHOT   = 0x2C;
const uint32_t VK_INSERT     = 0x2D;
const uint32_t VK_DELETE     = 0x2E;
const uint32_t VK_HELP       = 0x2F;
const uint32_t VK_0          = 0x30;
const uint32_t VK_1          = 0x31;
const uint32_t VK_2          = 0x32;
const uint32_t VK_3          = 0x33;
const uint32_t VK_4          = 0x34;
const uint32_t VK_5          = 0x35;
const uint32_t VK_6          = 0x36;
const uint32_t VK_7          = 0x37;
const uint32_t VK_8          = 0x38;
const uint32_t VK_9          = 0x39;
const uint32_t VK_A          = 0x41;
const uint32_t VK_B          = 0x42;
const uint32_t VK_C          = 0x43;
const uint32_t VK_D          = 0x44;
const uint32_t VK_E          = 0x45;
const uint32_t VK_F          = 0x46;
const uint32_t VK_G          = 0x47;
const uint32_t VK_H          = 0x48;
const uint32_t VK_I          = 0x49;
const uint32_t VK_J          = 0x4A;
const uint32_t VK_K          = 0x4B;
const uint32_t VK_L          = 0x4C;
const uint32_t VK_M          = 0x4D;
const uint32_t VK_N          = 0x4E;
const uint32_t VK_O          = 0x4F;
const uint32_t VK_P          = 0x50;
const uint32_t VK_Q          = 0x51;
const uint32_t VK_R          = 0x52;
const uint32_t VK_S          = 0x53;
const uint32_t VK_T          = 0x54;
const uint32_t VK_U          = 0x55;
const uint32_t VK_V          = 0x56;
const uint32_t VK_W          = 0x57;
const uint32_t VK_X          = 0x58;
const uint32_t VK_Y          = 0x59;
const uint32_t VK_Z          = 0x5A;
const uint32_t VK_LWIN       = 0x5B;
const uint32_t VK_RWIN       = 0x5C;
const uint32_t VK_APPS       = 0x5D;
const uint32_t VK_SLEEP      = 0x5F;
const uint32_t VK_NUMPAD0    = 0x60;
const uint32_t VK_NUMPAD1    = 0x61;
const uint32_t VK_NUMPAD2    = 0x62;
const uint32_t VK_NUMPAD3    = 0x63;
const uint32_t VK_NUMPAD4    = 0x64;
const uint32_t VK_NUMPAD5    = 0x65;
const uint32_t VK_NUMPAD6    = 0x66;
const uint32_t VK_NUMPAD7    = 0x67;
const uint32_t VK_NUMPAD8    = 0x68;
const uint32_t VK_NUMPAD9    = 0x69;
const uint32_t VK_MULTIPLY   = 0x6A;
const uint32_t VK_ADD        = 0x6B;
const uint32_t VK_SEPARATOR  = 0x6C;
const uint32_t VK_SUBTRACT   = 0x6D;
const uint32_t VK_DECIMAL    = 0x6E;
const uint32_t VK_DIVIDE     = 0x6F;
const uint32_t VK_F1         = 0x70;
const uint32_t VK_F2         = 0x71;
const uint32_t VK_F3         = 0x72;
const uint32_t VK_F4         = 0x73;
const uint32_t VK_F5         = 0x74;
const uint32_t VK_F6         = 0x75;
const uint32_t VK_F7         = 0x76;
const uint32_t VK_F8         = 0x77;
const uint32_t VK_F9         = 0x78;
const uint32_t VK_F10        = 0x79;
const uint32_t VK_F11        = 0x7A;
const uint32_t VK_F12        = 0x7B;
const uint32_t VK_F13        = 0x7C;
const uint32_t VK_F14        = 0x7D;
const uint32_t VK_F15        = 0x7E;
const uint32_t VK_F16        = 0x7F;
const uint32_t VK_F17        = 0x80;
const uint32_t VK_F18        = 0x81;
const uint32_t VK_F19        = 0x82;
const uint32_t VK_F20        = 0x83;
const uint32_t VK_F21        = 0x84;
const uint32_t VK_F22        = 0x85;
const uint32_t VK_F23        = 0x86;
const uint32_t VK_F24        = 0x87;
const uint32_t VK_NUMLOCK    = 0x90;
const uint32_t VK_SCROLL     = 0x91;
//const uint32_t VK_OEM_FJ_JISHO        = 0x92;
const uint32_t VK_OEM_FJ_MASSHOU = 0x93;
const uint32_t VK_OEM_FJ_TOUROKU = 0x94;
//const uint32_t VK_I_LEFT              = 0x95;
//const uint32_t VK_I_RIGHT             = 0x96;
const uint32_t VK_LSHIFT              = 0xA0;
const uint32_t VK_RSHIFT              = 0xA1;
const uint32_t VK_LCONTROL            = 0xA2;
const uint32_t VK_RCONTROL            = 0xA3;
const uint32_t VK_LMENU               = 0xA4;
const uint32_t VK_RMENU               = 0xA5;
const uint32_t VK_BROWSER_BACK        = 0xA6;
const uint32_t VK_BROWSER_FORWARD     = 0xA7;
const uint32_t VK_BROWSER_REFRESH     = 0xA8;
const uint32_t VK_BROWSER_STOP        = 0xA9;
const uint32_t VK_BROWSER_SEARCH      = 0xAA;
const uint32_t VK_BROWSER_FAVORITES   = 0xAB;
const uint32_t VK_BROWSER_HOME        = 0xAC;
const uint32_t VK_VOLUME_MUTE         = 0xAD;
const uint32_t VK_VOLUME_DOWN         = 0xAE;
const uint32_t VK_VOLUME_UP           = 0xAF;
const uint32_t VK_MEDIA_NEXT_TRACK    = 0xB0;
const uint32_t VK_MEDIA_PREV_TRACK    = 0xB1;
const uint32_t VK_MEDIA_STOP          = 0xB2;
const uint32_t VK_MEDIA_PLAY_PAUSE    = 0xB3;
const uint32_t VK_LAUNCH_MAIL         = 0xB4;
const uint32_t VK_LAUNCH_MEDIA_SELECT = 0xB5;
const uint32_t VK_LAUNCH_APP1         = 0xB6;
const uint32_t VK_LAUNCH_APP2         = 0xB7;
const uint32_t VK_OEM_1               = 0xBA;
const uint32_t VK_OEM_PLUS            = 0xBB;
const uint32_t VK_OEM_COMMA           = 0xBC;
const uint32_t VK_OEM_MINUS           = 0xBD;
const uint32_t VK_OEM_PERIOD          = 0xBE;
const uint32_t VK_OEM_2               = 0xBF;
const uint32_t VK_OEM_3               = 0xC0;
const uint32_t VK_OEM_4               = 0xDB;
const uint32_t VK_OEM_5               = 0xDC;
const uint32_t VK_OEM_6               = 0xDD;
const uint32_t VK_OEM_7               = 0xDE;
const uint32_t VK_OEM_8               = 0xDF;
const uint32_t VK_OEM_9               = 0xE0;
const uint32_t VK_OEM_AX              = 0xE1;
const uint32_t VK_OEM_102             = 0xE2;
const uint32_t VK_ICO_HELP            = 0xE3;
const uint32_t VK_ICO_00              = 0xE4;
const uint32_t VK_PROCESSKEY          = 0xE5;
const uint32_t VK_ICO_CLEAR           = 0xE6;
const uint32_t VK_PACKET              = 0xE7;
const uint32_t VK_OEM_RESET           = 0xE9;
const uint32_t VK_OEM_JUMP            = 0xEA;
const uint32_t VK_OEM_PA1             = 0xEB;
const uint32_t VK_OEM_PA2             = 0xEC;
const uint32_t VK_OEM_PA3             = 0xED;
const uint32_t VK_OEM_WSCTRL          = 0xEE;
const uint32_t VK_OEM_CUSEL           = 0xEF;
const uint32_t VK_OEM_ATTN            = 0xF0;
const uint32_t VK_OEM_FINISH          = 0xF1;
const uint32_t VK_OEM_COPY            = 0xF2;
const uint32_t VK_OEM_AUTO            = 0xF3;
const uint32_t VK_OEM_ENLW            = 0xF4;
const uint32_t VK_OEM_BACKTAB         = 0xF5;
const uint32_t VK_ATTN                = 0xF6;
const uint32_t VK_CRSEL               = 0xF7;
const uint32_t VK_EXSEL               = 0xF8;
const uint32_t VK_EREOF               = 0xF9;
const uint32_t VK_PLAY                = 0xFA;
const uint32_t VK_ZOOM                = 0xFB;
const uint32_t VK_NONAME              = 0xFC;
const uint32_t VK_PA1                 = 0xFD;
const uint32_t VK_OEM_CLEAR           = 0xFE;

const uint32_t MOD_ALT     = 0x0004; // Either ALT key was held down.
const uint32_t MOD_CONTROL = 0x0002; // Either CTRL key was held down.
const uint32_t MOD_SHIFT   = 0x0001; // Either SHIFT key was held down.
const uint32_t MOD_WIN     = 0x0008; // Either WIN key was held down.

uint32_t toNativeModifiers(uint32_t modifiers)
{
    uint32_t native = 0;
    if ((modifiers & Qt::ShiftModifier) == Qt::ShiftModifier)
    {
        native |= MOD_SHIFT;
    }
    if ((modifiers & Qt::ControlModifier) == Qt::ControlModifier)
    {
        native |= MOD_CONTROL;
    }
    if ((modifiers & Qt::AltModifier) == Qt::AltModifier)
    {
        native |= MOD_ALT;
    }
    if ((modifiers & Qt::MetaModifier) == Qt::MetaModifier)
    {
        native |= MOD_WIN;
    }

    return native;
}

uint32_t toNativeKeycode(int key)
{
    switch (key)
    {
    case Qt::Key_Cancel:
        return VK_CANCEL;
    case Qt::Key_Backspace:
        return VK_BACK;
    case Qt::Key_Tab:
        return VK_TAB;
    case Qt::Key_Clear:
        return VK_CLEAR;
    case Qt::Key_Return:
        return VK_RETURN;
    case Qt::Key_Shift:
        return VK_SHIFT;
    case Qt::Key_Control:
        return VK_CONTROL;
    case Qt::Key_Alt:
        return VK_MENU;
    case Qt::Key_Pause:
        return VK_PAUSE;
    case Qt::Key_CapsLock:
        return VK_CAPITAL;
    case Qt::Key_Escape:
        return VK_ESCAPE;
    case Qt::Key_Mode_switch:
        return VK_MODECHANGE;
    case Qt::Key_Space:
        return VK_SPACE;
    case Qt::Key_PageUp:
        return VK_PRIOR;
    case Qt::Key_PageDown:
        return VK_NEXT;
    case Qt::Key_End:
        return VK_END;
    case Qt::Key_Home:
        return VK_HOME;
    case Qt::Key_Left:
        return VK_LEFT;
    case Qt::Key_Up:
        return VK_UP;
    case Qt::Key_Right:
        return VK_RIGHT;
    case Qt::Key_Down:
        return VK_DOWN;
    case Qt::Key_Select:
        return VK_SELECT;
    case Qt::Key_Printer:
        return VK_PRINT;
    case Qt::Key_Execute:
        return VK_EXECUTE;
    case Qt::Key_Print:
        return VK_SNAPSHOT;
    case Qt::Key_Insert:
        return VK_INSERT;
    case Qt::Key_Delete:
        return VK_DELETE;
    case Qt::Key_Help:
        return VK_HELP;
    case Qt::Key_0:
        return VK_0;
    case Qt::Key_1:
        return VK_1;
    case Qt::Key_2:
        return VK_2;
    case Qt::Key_3:
        return VK_3;
    case Qt::Key_4:
        return VK_4;
    case Qt::Key_5:
        return VK_5;
    case Qt::Key_6:
        return VK_6;
    case Qt::Key_7:
        return VK_7;
    case Qt::Key_8:
        return VK_8;
    case Qt::Key_9:
        return VK_9;
    case Qt::Key_A:
        return VK_A;
    case Qt::Key_B:
        return VK_B;
    case Qt::Key_C:
        return VK_C;
    case Qt::Key_D:
        return VK_D;
    case Qt::Key_E:
        return VK_E;
    case Qt::Key_F:
        return VK_F;
    case Qt::Key_G:
        return VK_G;
    case Qt::Key_H:
        return VK_H;
    case Qt::Key_I:
        return VK_I;
    case Qt::Key_J:
        return VK_J;
    case Qt::Key_K:
        return VK_K;
    case Qt::Key_L:
        return VK_L;
    case Qt::Key_M:
        return VK_M;
    case Qt::Key_N:
        return VK_N;
    case Qt::Key_O:
        return VK_O;
    case Qt::Key_P:
        return VK_P;
    case Qt::Key_Q:
        return VK_Q;
    case Qt::Key_R:
        return VK_R;
    case Qt::Key_S:
        return VK_S;
    case Qt::Key_T:
        return VK_T;
    case Qt::Key_U:
        return VK_U;
    case Qt::Key_V:
        return VK_V;
    case Qt::Key_W:
        return VK_W;
    case Qt::Key_X:
        return VK_X;
    case Qt::Key_Y:
        return VK_Y;
    case Qt::Key_Z:
        return VK_Z;
    case Qt::Key_Meta:
        return VK_LWIN;
    case Qt::Key_Menu:
        return VK_APPS;
    case Qt::Key_Sleep:
        return VK_SLEEP;
    case Qt::Key_Asterisk:
        return VK_MULTIPLY;
    case Qt::Key_Plus:
        return VK_ADD;
    case Qt::Key_Comma:
        return VK_SEPARATOR;
    case Qt::Key_Minus:
        return VK_SUBTRACT;
    case Qt::Key_Period:
        return VK_DECIMAL;
    case Qt::Key_Slash:
        return VK_DIVIDE;
    case Qt::Key_F1:
        return VK_F1;
    case Qt::Key_F2:
        return VK_F2;
    case Qt::Key_F3:
        return VK_F3;
    case Qt::Key_F4:
        return VK_F4;
    case Qt::Key_F5:
        return VK_F5;
    case Qt::Key_F6:
        return VK_F6;
    case Qt::Key_F7:
        return VK_F7;
    case Qt::Key_F8:
        return VK_F8;
    case Qt::Key_F9:
        return VK_F9;
    case Qt::Key_F10:
        return VK_F10;
    case Qt::Key_F11:
        return VK_F11;
    case Qt::Key_F12:
        return VK_F12;
    case Qt::Key_F13:
        return VK_F13;
    case Qt::Key_F14:
        return VK_F14;
    case Qt::Key_F15:
        return VK_F15;
    case Qt::Key_F16:
        return VK_F16;
    case Qt::Key_F17:
        return VK_F17;
    case Qt::Key_F18:
        return VK_F18;
    case Qt::Key_F19:
        return VK_F19;
    case Qt::Key_F20:
        return VK_F20;
    case Qt::Key_F21:
        return VK_F21;
    case Qt::Key_F22:
        return VK_F22;
    case Qt::Key_F23:
        return VK_F23;
    case Qt::Key_F24:
        return VK_F24;
    case Qt::Key_NumLock:
        return VK_NUMLOCK;
    case Qt::Key_ScrollLock:
        return VK_SCROLL;
    case Qt::Key_Massyo:
        return VK_OEM_FJ_MASSHOU;
    case Qt::Key_Touroku:
        return VK_OEM_FJ_TOUROKU;
    case Qt::Key_Back:
        return VK_BROWSER_BACK;
    case Qt::Key_Forward:
        return VK_BROWSER_FORWARD;
    case Qt::Key_Refresh:
        return VK_BROWSER_REFRESH;
    case Qt::Key_Stop:
        return VK_BROWSER_STOP;
    case Qt::Key_Search:
        return VK_BROWSER_SEARCH;
    case Qt::Key_Favorites:
        return VK_BROWSER_FAVORITES;
    case Qt::Key_HomePage:
        return VK_BROWSER_HOME;
    case Qt::Key_VolumeMute:
        return VK_VOLUME_MUTE;
    case Qt::Key_VolumeDown:
        return VK_VOLUME_DOWN;
    case Qt::Key_VolumeUp:
        return VK_VOLUME_UP;
    case Qt::Key_MediaNext:
        return VK_MEDIA_NEXT_TRACK;
    case Qt::Key_MediaPrevious:
        return VK_MEDIA_PREV_TRACK;
    case Qt::Key_MediaStop:
        return VK_MEDIA_STOP;
    case Qt::Key_MediaPlay:
        return VK_MEDIA_PLAY_PAUSE;
    case Qt::Key_LaunchMail:
        return VK_LAUNCH_MAIL;
    case Qt::Key_LaunchMedia:
        return VK_LAUNCH_MEDIA_SELECT;
    case Qt::Key_Launch0:
        return VK_LAUNCH_APP1;
    case Qt::Key_Launch1:
        return VK_LAUNCH_APP2;
    case Qt::Key_Play:
        return VK_PLAY;
    case Qt::Key_Zoom:
        return VK_ZOOM;
    case Qt::Key_unknown:
        return VK_NONAME;
    case Qt::Key_Equal:
        return VK_OEM_PLUS;
    default:
        return 0;
    };
}

uint32_t fromNativeModifiers(uint32_t modifiers)
{
    modifiers = (modifiers >> 8);

    GPUI_WARNING_STREAM << "Modifiers: " << modifiers;

    uint32_t qt = 0;
    if ((modifiers & MOD_SHIFT) == MOD_SHIFT)
    {
        qt |= Qt::ShiftModifier;
    }
    if ((modifiers & MOD_CONTROL) == MOD_CONTROL)
    {
        qt |= Qt::ControlModifier;
    }
    if ((modifiers & MOD_ALT) == MOD_ALT)
    {
        qt |= Qt::AltModifier;
    }
    if ((modifiers & MOD_WIN) == MOD_WIN)
    {
        qt |= Qt::MetaModifier;
    }

    return qt;
}

uint32_t fromNativeKeycode(uint32_t code)
{
    switch (code)
    {
    case VK_CANCEL:
        return Qt::Key_Cancel;
    case VK_BACK:
        return Qt::Key_Back;
    case VK_TAB:
        return Qt::Key_Tab;
    case VK_CLEAR:
        return Qt::Key_Clear;
    case VK_RETURN:
        return Qt::Key_Return;
    case VK_SHIFT:
        return Qt::Key_Shift;
    case VK_CONTROL:
        return Qt::Key_Control;
    case VK_MENU:
        return Qt::Key_Menu;
    case VK_PAUSE:
        return Qt::Key_Pause;
    case VK_CAPITAL:
        return Qt::Key_CapsLock;
    case VK_ESCAPE:
        return Qt::Key_Escape;
    case VK_MODECHANGE:
        return Qt::Key_Mode_switch;
    case VK_SPACE:
        return Qt::Key_Space;
    case VK_PRIOR:
        return Qt::Key_PageUp;
    case VK_NEXT:
        return Qt::Key_PageDown;
    case VK_END:
        return Qt::Key_End;
    case VK_HOME:
        return Qt::Key_Home;
    case VK_LEFT:
        return Qt::Key_Left;
    case VK_UP:
        return Qt::Key_Up;
    case VK_RIGHT:
        return Qt::Key_Right;
    case VK_DOWN:
        return Qt::Key_Down;
    case VK_SELECT:
        return Qt::Key_Select;
    case VK_PRINT:
        return Qt::Key_Print;
    case VK_EXECUTE:
        return Qt::Key_Execute;
    case VK_SNAPSHOT:
        return Qt::Key_Print;
    case VK_INSERT:
        return Qt::Key_Insert;
    case VK_DELETE:
        return Qt::Key_Delete;
    case VK_HELP:
        return Qt::Key_Help;
    case VK_0:
        return Qt::Key_0;
    case VK_1:
        return Qt::Key_1;
    case VK_2:
        return Qt::Key_2;
    case VK_3:
        return Qt::Key_3;
    case VK_4:
        return Qt::Key_4;
    case VK_5:
        return Qt::Key_5;
    case VK_6:
        return Qt::Key_6;
    case VK_7:
        return Qt::Key_7;
    case VK_8:
        return Qt::Key_8;
    case VK_9:
        return Qt::Key_9;
    case VK_A:
        return Qt::Key_A;
    case VK_B:
        return Qt::Key_B;
    case VK_C:
        return Qt::Key_C;
    case VK_D:
        return Qt::Key_D;
    case VK_E:
        return Qt::Key_E;
    case VK_F:
        return Qt::Key_F;
    case VK_G:
        return Qt::Key_G;
    case VK_H:
        return Qt::Key_H;
    case VK_I:
        return Qt::Key_I;
    case VK_J:
        return Qt::Key_J;
    case VK_K:
        return Qt::Key_K;
    case VK_L:
        return Qt::Key_L;
    case VK_M:
        return Qt::Key_M;
    case VK_N:
        return Qt::Key_N;
    case VK_O:
        return Qt::Key_O;
    case VK_P:
        return Qt::Key_P;
    case VK_Q:
        return Qt::Key_Q;
    case VK_R:
        return Qt::Key_R;
    case VK_S:
        return Qt::Key_S;
    case VK_T:
        return Qt::Key_T;
    case VK_U:
        return Qt::Key_U;
    case VK_V:
        return Qt::Key_V;
    case VK_W:
        return Qt::Key_W;
    case VK_X:
        return Qt::Key_X;
    case VK_Y:
        return Qt::Key_Y;
    case VK_Z:
        return Qt::Key_Z;
    case VK_LWIN:
        return Qt::Key_Meta;
    case VK_RWIN:
        return Qt::Key_Meta;
    case VK_APPS:
        return Qt::Key_Menu;
    case VK_SLEEP:
        return Qt::Key_Sleep;
    case VK_NUMPAD0:
        return Qt::Key_0;
    case VK_NUMPAD1:
        return Qt::Key_1;
    case VK_NUMPAD2:
        return Qt::Key_2;
    case VK_NUMPAD3:
        return Qt::Key_3;
    case VK_NUMPAD4:
        return Qt::Key_4;
    case VK_NUMPAD5:
        return Qt::Key_5;
    case VK_NUMPAD6:
        return Qt::Key_6;
    case VK_NUMPAD7:
        return Qt::Key_7;
    case VK_NUMPAD8:
        return Qt::Key_8;
    case VK_NUMPAD9:
        return Qt::Key_9;
    case VK_MULTIPLY:
        return Qt::Key_multiply;
    case VK_ADD:
        return Qt::Key_Plus;
    case VK_SEPARATOR:
        return Qt::Key_Period;
    case VK_SUBTRACT:
        return Qt::Key_Minus;
    case VK_DECIMAL:
        return Qt::Key_Slash;
    case VK_DIVIDE:
        return Qt::Key_division;
    case VK_F1:
        return Qt::Key_F1;
    case VK_F2:
        return Qt::Key_F2;
    case VK_F3:
        return Qt::Key_F3;
    case VK_F4:
        return Qt::Key_F4;
    case VK_F5:
        return Qt::Key_F5;
    case VK_F6:
        return Qt::Key_F6;
    case VK_F7:
        return Qt::Key_F7;
    case VK_F8:
        return Qt::Key_F8;
    case VK_F9:
        return Qt::Key_F9;
    case VK_F10:
        return Qt::Key_F10;
    case VK_F11:
        return Qt::Key_F11;
    case VK_F12:
        return Qt::Key_F12;
    case VK_F13:
        return Qt::Key_F13;
    case VK_F14:
        return Qt::Key_F14;
    case VK_F15:
        return Qt::Key_F15;
    case VK_F16:
        return Qt::Key_F16;
    case VK_F17:
        return Qt::Key_F17;
    case VK_F18:
        return Qt::Key_F18;
    case VK_F19:
        return Qt::Key_F19;
    case VK_F20:
        return Qt::Key_F20;
    case VK_F21:
        return Qt::Key_F21;
    case VK_F22:
        return Qt::Key_F22;
    case VK_F23:
        return Qt::Key_F23;
    case VK_F24:
        return Qt::Key_F24;
    case VK_NUMLOCK:
        return Qt::Key_NumLock;
    case VK_SCROLL:
        return Qt::Key_ScrollLock;
    //case VK_OEM_FJ_JISHO        : return Qt::Key_Jisho;
    case VK_OEM_FJ_MASSHOU:
        return Qt::Key_Massyo;
    case VK_OEM_FJ_TOUROKU:
        return Qt::Key_Touroku;
    //case VK_I_LEFT              : return Qt::Key_Oyayubi_Left;
    //case VK_I_RIGHT             : return Qt::Key_Oyayubi_Right;
    case VK_LSHIFT:
        return Qt::Key_Shift;
    case VK_RSHIFT:
        return Qt::Key_Shift;
    case VK_LCONTROL:
        return Qt::Key_Control;
    case VK_RCONTROL:
        return Qt::Key_Control;
    case VK_LMENU:
        return Qt::Key_Menu;
    case VK_RMENU:
        return Qt::Key_Menu;
    case VK_BROWSER_BACK:
        return Qt::Key_Back;
    case VK_BROWSER_FORWARD:
        return Qt::Key_Forward;
    case VK_BROWSER_REFRESH:
        return Qt::Key_Refresh;
    case VK_BROWSER_STOP:
        return Qt::Key_Stop;
    case VK_BROWSER_SEARCH:
        return Qt::Key_Search;
    case VK_BROWSER_FAVORITES:
        return Qt::Key_Favorites;
    case VK_BROWSER_HOME:
        return Qt::Key_HomePage;
    case VK_VOLUME_MUTE:
        return Qt::Key_VolumeMute;
    case VK_VOLUME_DOWN:
        return Qt::Key_VolumeDown;
    case VK_VOLUME_UP:
        return Qt::Key_VolumeUp;
    case VK_MEDIA_NEXT_TRACK:
        return Qt::Key_MediaNext;
    case VK_MEDIA_PREV_TRACK:
        return Qt::Key_MediaPrevious;
    case VK_MEDIA_STOP:
        return Qt::Key_MediaStop;
    case VK_MEDIA_PLAY_PAUSE:
        return Qt::Key_MediaPause;
    case VK_LAUNCH_MAIL:
        return Qt::Key_LaunchMail;
    case VK_LAUNCH_MEDIA_SELECT:
        return Qt::Key_LaunchMedia;
    case VK_LAUNCH_APP1:
        return Qt::Key_Launch0;
    case VK_LAUNCH_APP2:
        return Qt::Key_Launch1;
    case VK_OEM_1:
        return Qt::Key_unknown;
    case VK_OEM_PLUS:
        return Qt::Key_Equal;
    case VK_OEM_COMMA:
        return Qt::Key_Comma;
    case VK_OEM_MINUS:
        return Qt::Key_Minus;
    case VK_OEM_PERIOD:
        return Qt::Key_Period;
    case VK_OEM_2:
        return Qt::Key_unknown;
    case VK_OEM_3:
        return Qt::Key_unknown;
    case VK_OEM_4:
        return Qt::Key_unknown;
    case VK_OEM_5:
        return Qt::Key_unknown;
    case VK_OEM_6:
        return Qt::Key_unknown;
    case VK_OEM_7:
        return Qt::Key_unknown;
    case VK_OEM_8:
        return Qt::Key_unknown;
    case VK_OEM_9:
        return Qt::Key_unknown;
    case VK_OEM_AX:
        return Qt::Key_unknown;
    case VK_OEM_102:
        return Qt::Key_unknown;
    case VK_ICO_HELP:
        return Qt::Key_unknown;
    case VK_ICO_00:
        return Qt::Key_unknown;
    case VK_PROCESSKEY:
        return Qt::Key_unknown;
    case VK_ICO_CLEAR:
        return Qt::Key_unknown;
    case VK_PACKET:
        return Qt::Key_unknown;
    case VK_OEM_RESET:
        return Qt::Key_unknown;
    case VK_OEM_JUMP:
        return Qt::Key_unknown;
    case VK_OEM_PA1:
        return Qt::Key_unknown;
    case VK_OEM_PA2:
        return Qt::Key_unknown;
    case VK_OEM_PA3:
        return Qt::Key_unknown;
    case VK_OEM_WSCTRL:
        return Qt::Key_unknown;
    case VK_OEM_CUSEL:
        return Qt::Key_unknown;
    case VK_OEM_ATTN:
        return Qt::Key_unknown;
    case VK_OEM_FINISH:
        return Qt::Key_unknown;
    case VK_OEM_COPY:
        return Qt::Key_unknown;
    case VK_OEM_AUTO:
        return Qt::Key_unknown;
    case VK_OEM_ENLW:
        return Qt::Key_unknown;
    case VK_OEM_BACKTAB:
        return Qt::Key_unknown;
    case VK_ATTN:
        return Qt::Key_unknown;
    case VK_CRSEL:
        return Qt::Key_unknown;
    case VK_EXSEL:
        return Qt::Key_unknown;
    case VK_EREOF:
        return Qt::Key_unknown;
    case VK_PLAY:
        return Qt::Key_Play;
    case VK_ZOOM:
        return Qt::Key_Zoom;
    case VK_NONAME:
        return Qt::Key_unknown;
    case VK_PA1:
        return Qt::Key_unknown;
    case VK_OEM_CLEAR:
        return Qt::Key_Clear;
    default:
        return 0;
    }
}

namespace preferences
{
KeySequenceEncoder::KeySequenceEncoder() {}

uint32_t KeySequenceEncoder::encode(const QKeySequence &sequence)
{
    GPUI_WARNING_STREAM << "Debug: Encoding shortcut key!";

    GPUI_WARNING_STREAM << "Encoding sequence: " << sequence << " 0: " << sequence[0] << " 1: " << sequence[1]
               << " 2: " << sequence[2] << " 3: " << sequence[3];

    uint32_t modifiers = 0xFF00;
    modifiers &= sequence[0]; //mask out modifiers
    uint32_t qKey = 0x00FF;
    qKey &= sequence[0];

    GPUI_WARNING_STREAM << "Debug: modifiers - " << modifiers << " key: " << qKey;

    uint32_t nativeCode = toNativeKeycode(qKey);
    uint32_t nativeMods = toNativeModifiers(sequence[0]);

    uint32_t result = ((nativeMods << 8) ^ nativeCode);

    GPUI_WARNING_STREAM << "Debug: native modifiers - " << nativeMods << " nativeCode: " << nativeCode << " m^k " << result;

    return result;
}

QKeySequence KeySequenceEncoder::decode(const uint32_t sequence)
{
    GPUI_WARNING_STREAM << "Debug: Decoding shortcut key!";

    uint32_t nativeModifiers = 0xFF00;
    nativeModifiers &= sequence; //mask out modifiers
    uint32_t nativeKeycode = 0x00FF;
    nativeKeycode &= sequence;

    GPUI_WARNING_STREAM << "Debug: modifiers - " << nativeModifiers << " key: " << nativeKeycode;

    uint32_t code      = fromNativeKeycode(nativeKeycode);
    uint32_t modifiers = fromNativeModifiers(nativeModifiers);

    GPUI_WARNING_STREAM << "Debug: native modifiers - " << modifiers << " nativeCode: " << code << " m^k " << (modifiers ^ code);

    QKeySequence result(modifiers ^ code);

    GPUI_WARNING_STREAM << "Debug: encoded sequence - " << result;

    return result;
}

} // namespace preferences

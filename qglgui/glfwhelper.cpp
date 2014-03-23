/*
 * Copyright (c) 2014, Denis Biryukov <denis.birukov@gmail.com>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *     * Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in the
 *     documentation and/or other materials provided with the distribution.
 *     * Neither the name of the <organization> nor the
 *     names of its contributors may be used to endorse or promote products
 *     derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY Denis Biryukov <denis.birukov@gmail.com> ''AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL Denis Biryukov <denis.birukov@gmail.com> BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */

#include "glfwhelper.h"

#include "libcppprofiler/src/cppprofiler.h"

#include "GLFW/glfw3.h"

namespace QGL
{
namespace GlfwHelper
{

Qt::MouseButton MouseButton(int mouseButton, int state)
{
	PROFILE_FUNCTION

	if (state == GLFW_RELEASE) return Qt::NoButton;
	
	switch (mouseButton)
	{
		case GLFW_MOUSE_BUTTON_1:
			return Qt::LeftButton;
		case GLFW_MOUSE_BUTTON_2:
			return Qt::RightButton;
		case GLFW_MOUSE_BUTTON_3:
			return Qt::MidButton;
		case GLFW_MOUSE_BUTTON_4:
			return Qt::ExtraButton1;
		case GLFW_MOUSE_BUTTON_5:
			return Qt::ExtraButton2;
		case GLFW_MOUSE_BUTTON_6:
			return Qt::ExtraButton3;
		case GLFW_MOUSE_BUTTON_7:
			return Qt::ExtraButton4;
		case GLFW_MOUSE_BUTTON_8:
			return Qt::ExtraButton5;
		default:
			return Qt::NoButton;
	}
	return Qt::NoButton;
}

Qt::MouseButtons MouseButtons(int mouseButtons)
{
	Qt::MouseButtons buttons = Qt::NoButton;
	for (int b = GLFW_MOUSE_BUTTON_1; b < GLFW_MOUSE_BUTTON_LAST; ++b)
	{
		if ((mouseButtons & (1 < b)) == 0)continue;

		buttons |= MouseButton(b, GLFW_PRESS);
	}
	return buttons;
}

QEvent::Type KeyboardEvent(int isPressed)
{
	if (isPressed == GLFW_PRESS || isPressed == GLFW_REPEAT) return QEvent::KeyPress;
	else return QEvent::KeyRelease;
}

Qt::Key KeyboardKey(int key)
{
	PROFILE_FUNCTION

	switch (key)
	{
		case GLFW_KEY_SPACE:
			return Qt::Key::Key_Space;
		case GLFW_KEY_APOSTROPHE:
			return Qt::Key::Key_Apostrophe;
		case GLFW_KEY_COMMA:
			return Qt::Key::Key_Comma;
		case GLFW_KEY_MINUS:
			return Qt::Key::Key_Minus;
		case GLFW_KEY_PERIOD:
			return Qt::Key::Key_Period;
		case GLFW_KEY_SLASH:
			return Qt::Key::Key_Slash;
		case GLFW_KEY_0:
			return Qt::Key::Key_0;
		case GLFW_KEY_1:
			return Qt::Key::Key_1;
		case GLFW_KEY_2:
			return Qt::Key::Key_2;
		case GLFW_KEY_3:
			return Qt::Key::Key_3;
		case GLFW_KEY_4:
			return Qt::Key::Key_4;
		case GLFW_KEY_5:
			return Qt::Key::Key_5;
		case GLFW_KEY_6:
			return Qt::Key::Key_6;
		case GLFW_KEY_7:
			return Qt::Key::Key_7;
		case GLFW_KEY_8:
			return Qt::Key::Key_8;
		case GLFW_KEY_9:
			return Qt::Key::Key_9;
		case GLFW_KEY_SEMICOLON:
			return Qt::Key::Key_Semicolon;
		case GLFW_KEY_EQUAL:
			return Qt::Key::Key_Equal;
		case GLFW_KEY_A:
			return Qt::Key::Key_A;
		case GLFW_KEY_B:
			return Qt::Key::Key_B;
		case GLFW_KEY_C:
			return Qt::Key::Key_C;
		case GLFW_KEY_D:
			return Qt::Key::Key_D;
		case GLFW_KEY_E:
			return Qt::Key::Key_E;
		case GLFW_KEY_F:
			return Qt::Key::Key_F;
		case GLFW_KEY_G:
			return Qt::Key::Key_G;
		case GLFW_KEY_H:
			return Qt::Key::Key_H;
		case GLFW_KEY_I:
			return Qt::Key::Key_I;
		case GLFW_KEY_J:
			return Qt::Key::Key_J;
		case GLFW_KEY_K:
			return Qt::Key::Key_K;
		case GLFW_KEY_L:
			return Qt::Key::Key_L;
		case GLFW_KEY_M:
			return Qt::Key::Key_M;
		case GLFW_KEY_N:
			return Qt::Key::Key_N;
		case GLFW_KEY_O:
			return Qt::Key::Key_O;
		case GLFW_KEY_P:
			return Qt::Key::Key_P;
		case GLFW_KEY_Q:
			return Qt::Key::Key_Q;
		case GLFW_KEY_R:
			return Qt::Key::Key_R;
		case GLFW_KEY_S:
			return Qt::Key::Key_S;
		case GLFW_KEY_T:
			return Qt::Key::Key_T;
		case GLFW_KEY_U:
			return Qt::Key::Key_U;
		case GLFW_KEY_V:
			return Qt::Key::Key_V;
		case GLFW_KEY_W:
			return Qt::Key::Key_W;
		case GLFW_KEY_X:
			return Qt::Key::Key_X;
		case GLFW_KEY_Y:
			return Qt::Key::Key_Y;
		case GLFW_KEY_Z:
			return Qt::Key::Key_Z;
		case GLFW_KEY_LEFT_BRACKET:
			return Qt::Key::Key_BracketLeft;
		case GLFW_KEY_BACKSLASH:
			return Qt::Key::Key_Backslash;
		case GLFW_KEY_RIGHT_BRACKET:
			return Qt::Key::Key_BracketRight;
		case GLFW_KEY_GRAVE_ACCENT:
			return Qt::Key::Key_Apostrophe;
		case GLFW_KEY_ESCAPE:
			return Qt::Key::Key_Escape;
		case GLFW_KEY_ENTER:
			return Qt::Key::Key_Enter;
		case GLFW_KEY_TAB:
			return Qt::Key::Key_Tab;
		case GLFW_KEY_BACKSPACE:
			return Qt::Key::Key_Backspace;
		case GLFW_KEY_INSERT:
			return Qt::Key::Key_Insert;
		case GLFW_KEY_DELETE:
			return Qt::Key::Key_Delete;
		case GLFW_KEY_RIGHT:
			return Qt::Key::Key_Right;
		case GLFW_KEY_LEFT:
			return Qt::Key::Key_Left;
		case GLFW_KEY_DOWN:
			return Qt::Key::Key_Down;
		case GLFW_KEY_UP:
			return Qt::Key::Key_Up;
		case GLFW_KEY_PAGE_UP:
			return Qt::Key::Key_PageUp;
		case GLFW_KEY_PAGE_DOWN:
			return Qt::Key::Key_PageDown;
		case GLFW_KEY_HOME:
			return Qt::Key::Key_Home;
		case GLFW_KEY_END:
			return Qt::Key::Key_End;
		case GLFW_KEY_CAPS_LOCK:
			return Qt::Key::Key_CapsLock;
		case GLFW_KEY_SCROLL_LOCK:
			return Qt::Key::Key_ScrollLock;
		case GLFW_KEY_NUM_LOCK:
			return Qt::Key::Key_NumLock;
		case GLFW_KEY_PRINT_SCREEN:
			return Qt::Key::Key_SysReq;
		case GLFW_KEY_PAUSE:
			return Qt::Key::Key_Pause;
		case GLFW_KEY_F1:
			return Qt::Key::Key_F1;
		case GLFW_KEY_F2:
			return Qt::Key::Key_F2;
		case GLFW_KEY_F3:
			return Qt::Key::Key_F3;
		case GLFW_KEY_F4:
			return Qt::Key::Key_F4;
		case GLFW_KEY_F5:
			return Qt::Key::Key_F5;
		case GLFW_KEY_F6:
			return Qt::Key::Key_F6;
		case GLFW_KEY_F7:
			return Qt::Key::Key_F7;
		case GLFW_KEY_F8:
			return Qt::Key::Key_F8;
		case GLFW_KEY_F9:
			return Qt::Key::Key_F9;
		case GLFW_KEY_F10:
			return Qt::Key::Key_F10;
		case GLFW_KEY_F11:
			return Qt::Key::Key_F11;
		case GLFW_KEY_F12:
			return Qt::Key::Key_F12;
		case GLFW_KEY_F13:
			return Qt::Key::Key_F13;
		case GLFW_KEY_F14:
			return Qt::Key::Key_F14;
		case GLFW_KEY_F15:
			return Qt::Key::Key_F15;
		case GLFW_KEY_F16:
			return Qt::Key::Key_F16;
		case GLFW_KEY_F17:
			return Qt::Key::Key_F17;
		case GLFW_KEY_F18:
			return Qt::Key::Key_F18;
		case GLFW_KEY_F19:
			return Qt::Key::Key_F19;
		case GLFW_KEY_F20:
			return Qt::Key::Key_F20;
		case GLFW_KEY_F21:
			return Qt::Key::Key_F21;
		case GLFW_KEY_F22:
			return Qt::Key::Key_F22;
		case GLFW_KEY_F23:
			return Qt::Key::Key_F23;
		case GLFW_KEY_F24:
			return Qt::Key::Key_F24;
		case GLFW_KEY_F25:
			return Qt::Key::Key_F25;
		case GLFW_KEY_KP_0:
			return Qt::Key::Key_0;
		case GLFW_KEY_KP_1:
			return Qt::Key::Key_1;
		case GLFW_KEY_KP_2:
			return Qt::Key::Key_2;
		case GLFW_KEY_KP_3:
			return Qt::Key::Key_3;
		case GLFW_KEY_KP_4:
			return Qt::Key::Key_4;
		case GLFW_KEY_KP_5:
			return Qt::Key::Key_5;
		case GLFW_KEY_KP_6:
			return Qt::Key::Key_6;
		case GLFW_KEY_KP_7:
			return Qt::Key::Key_7;
		case GLFW_KEY_KP_8:
			return Qt::Key::Key_8;
		case GLFW_KEY_KP_9:
			return Qt::Key::Key_9;
		case GLFW_KEY_KP_DECIMAL:
			return Qt::Key::Key_Period;
		case GLFW_KEY_KP_DIVIDE:
			return Qt::Key::Key_division;
		case GLFW_KEY_KP_MULTIPLY:
			return Qt::Key::Key_multiply;
		case GLFW_KEY_KP_SUBTRACT:
			return Qt::Key::Key_Minus;
		case GLFW_KEY_KP_ADD:
			return Qt::Key::Key_Plus;
		case GLFW_KEY_KP_ENTER:
			return Qt::Key::Key_Enter;
		case GLFW_KEY_KP_EQUAL:
			return Qt::Key::Key_Equal;
		case GLFW_KEY_LEFT_SHIFT:
			return Qt::Key::Key_Shift;
		case GLFW_KEY_LEFT_CONTROL:
			return Qt::Key::Key_Control;
		case GLFW_KEY_LEFT_ALT:
			return Qt::Key::Key_Alt;
		case GLFW_KEY_LEFT_SUPER:
			return Qt::Key::Key_Super_L;
		case GLFW_KEY_RIGHT_SHIFT:
			return Qt::Key::Key_Shift;
		case GLFW_KEY_RIGHT_CONTROL:
			return Qt::Key::Key_Control;
		case GLFW_KEY_RIGHT_ALT:
			return Qt::Key::Key_Alt;
		case GLFW_KEY_RIGHT_SUPER:
			return Qt::Key::Key_Super_R;
		case GLFW_KEY_MENU:
			return Qt::Key::Key_Menu;
		default:
			return Qt::Key::Key_unknown;
	}
	return Qt::Key::Key_unknown;
}

Qt::KeyboardModifiers KeyboardModifiers(int modifiers)
{
	Qt::KeyboardModifiers mods;
	if ((modifiers & GLFW_MOD_ALT) == GLFW_MOD_ALT) mods |= Qt::AltModifier;
	if ((modifiers & GLFW_MOD_SHIFT) == GLFW_MOD_SHIFT) mods |= Qt::ShiftModifier;
	if ((modifiers & GLFW_MOD_CONTROL) == GLFW_MOD_CONTROL) mods |= Qt::ControlModifier;
	if ((modifiers & GLFW_MOD_SUPER) == GLFW_MOD_SUPER) mods |= Qt::MetaModifier;
	return mods;
}

}
}

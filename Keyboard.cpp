#include "Keyboard.h"

/// 仅拿取对应键码的布尔状态
bool Keyboard::KeyIsPressed(unsigned char keycode) const noexcept
{
	return keystates[keycode];
}

Keyboard::Event Keyboard::ReadKey() noexcept
{
	if (keybuffer.size() > 0u) { // 若键盘队列不为空,就拿到第一个Event并弹出
		Keyboard::Event e = keybuffer.front();
		keybuffer.pop();
		return e;
	} else {
		return Keyboard::Event();
	}
}

bool Keyboard::KeyIsEmpty() const noexcept
{
	return keybuffer.empty();
}

char Keyboard::ReadChar() noexcept
{
	if (charbuffer.size() > 0u) {// 字符队列不为空就取出第一个字符并弹出
		unsigned char charcode = charbuffer.front();
		charbuffer.pop();
		return charcode;
	} else {
		return 0;
	}
}

bool Keyboard::CharIsEmpty() const noexcept
{
	return charbuffer.empty();
}

void Keyboard::FlushKey() noexcept
{
	keybuffer = std::queue<Event>();
}

void Keyboard::FlushChar() noexcept
{
	charbuffer = std::queue<char>();
}

void Keyboard::Flush() noexcept
{
	FlushKey();
	FlushChar();
}

void Keyboard::EnableAutorepeat() noexcept
{
	autorepeatEnabled = true;
}

void Keyboard::DisableAutorepeat() noexcept
{
	autorepeatEnabled = false;
}

bool Keyboard::AutorepeatIsEnabled() const noexcept
{
	return autorepeatEnabled;
}

void Keyboard::OnKeyPressed(unsigned char keycode) noexcept
{
	keystates[keycode] = true;// 刷新对应键码的布尔为真
	keybuffer.push(Keyboard::Event(Keyboard::Event::Type::Press, keycode));// 事件队列里构造并注册进1个PRESS事件
	TrimBuffer(keybuffer);// 修剪缓存
}

void Keyboard::OnKeyReleased(unsigned char keycode) noexcept
{
	keystates[keycode] = false;
	keybuffer.push(Keyboard::Event(Keyboard::Event::Type::Release, keycode));
	TrimBuffer(keybuffer);
}

void Keyboard::OnChar(char character) noexcept
{
	charbuffer.push(character);
	TrimBuffer(charbuffer);
}

void Keyboard::ClearState() noexcept
{
	keystates.reset();
}

template<typename T>
void Keyboard::TrimBuffer(std::queue<T>& buffer) noexcept
{
	// 某种缓存队列若超过 缓存阈值,就把缓存队列持续弹出修剪
	while (buffer.size() > bufferSize) {
		buffer.pop();
	}
}
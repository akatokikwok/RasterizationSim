#pragma once
#include <queue>
#include <bitset>

/* 键盘类,注意!!!这里理解为 键盘类只是窗口类的一个扩展*/
class Keyboard
{
	friend class Window;// 窗口是键盘类的友元
public:
	/* 类种类,主要负责检测按下/释放状态*/
	class Event
	{
	public:
		enum class Type
		{
			Press,
			Release,
			Invalid
		};
	private:
		Type type;
		unsigned char code;
	public:
		Event()
			:
			type(Type::Invalid),
			code(0u)
		{}
		Event(Type type, unsigned char code) noexcept
			:
			type(type),
			code(code)
		{}
		bool IsPress() const noexcept
		{
			return type == Type::Press;
		}
		bool IsRelease() const noexcept
		{
			return type == Type::Release;
		}
		bool IsValid() const noexcept
		{
			return type != Type::Invalid;
		}
		unsigned char GetCode() const noexcept
		{
			return code;
		}
	};


public:
	Keyboard() = default;
	Keyboard(const Keyboard&) = delete;
	Keyboard& operator=(const Keyboard&) = delete;
	/* 一些键盘事件函数*/
	bool KeyIsPressed(unsigned char keycode) const noexcept;// 仅拿取对应键码的布尔状态
	Event ReadKey() noexcept;// 此函数将读取键盘queue event中的事件
	bool KeyIsEmpty() const noexcept;// 此函数检查键盘队列事件是否为空
	void FlushKey() noexcept;// 刷新事件队列
	/* 一些字符事件,用于处理文本输入即读取字符流*/
	char ReadChar() noexcept;// 取字符队列里第一个字符
	bool CharIsEmpty() const noexcept;
	void FlushChar() noexcept;// 刷新字符队列
	void Flush() noexcept;// 此函数将同时刷新字符队列 和 事件队列
	/* 一些 自动重复的控制流 事件*/
	void EnableAutorepeat() noexcept;// 开启 自动重复
	void DisableAutorepeat() noexcept;// 关闭 自动重复
	bool AutorepeatIsEnabled() const noexcept;// 拿取 重复状态布尔
private:/// 设置成私有的,又由于窗口是友元,所以可以供窗口使用这些函数
	/* 一些 仅在有特定窗口消息才会触发的函数*/
	void OnKeyPressed(unsigned char keycode) noexcept;
	void OnKeyReleased(unsigned char keycode) noexcept;
	void OnChar(char character) noexcept;
	void ClearState() noexcept;// 此函数负责 刷新字段里的bitset
	template<typename T>
	static void TrimBuffer(std::queue<T>& buffer) noexcept;// 此函数负责 当缓存超过16时候,被触发来暂时移除掉一些东西
private:
	static constexpr unsigned int nKeys = 256u;
	static constexpr unsigned int bufferSize = 16u;// 缓存上限阈值,仅拿来做是否被修剪的判断标准用
	bool autorepeatEnabled = false;// 自动重复开关
	std::bitset<nKeys> keystates;// 储存了256位的bool, 至于bitset 在 bitset 头文件中，它是一种类似数组的结构，它的每一个元素只能是０或１，每个元素仅用１bit空间
	std::queue<Event> keybuffer;// 键盘队列事件
	std::queue<char> charbuffer;// 字符队列
};
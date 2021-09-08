#pragma once
#include "ChiliWin.h"

class Window
{
private:
	/// 类种类, 单例来负责管理 WindowClass的注册与清理
	class WindowClass
	{
	public:
		static const char* GetName() noexcept;  // 静态小接口1: 获取本类名称(获取窗口实例名)
		static HINSTANCE GetInstance() noexcept;// 静态小接口2: 获取本类实例句柄
	private:
		WindowClass() noexcept;// 构造器负责注册窗口
		~WindowClass();// 析构函数负责解注册窗口实例
		WindowClass(const WindowClass&) = delete;
		WindowClass& operator=(const WindowClass&) = delete;
		static constexpr const char* wndClassName = "renbin Direct3D Engine Window";
		static WindowClass wndClass; // 由于仅有1个窗口,所以设计仅1个窗口实例
		HINSTANCE hInst;
	};
public:
	/* 构造器,负责注册并呈现窗口*/
	Window(int width, int height, const char* name) noexcept;
	/* 析构器仅负责 调用DestroyWindow系统API销毁窗口句柄*/
	~Window();
	Window(const Window&) = delete;
	Window& operator=(const Window&) = delete;
private:
	/* 窗口的消息机制处理函数, 设计成静态的原因是 我们不可以通过WinAPI调用类成员函数*/
	static LRESULT CALLBACK HandleMsgSetup(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;
	static LRESULT CALLBACK HandleMsgThunk(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;
	/* 供上面2个静态消息处理函数 调用,使用此函数间接地参与消息处理机制*/
	LRESULT HandleMsg(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;
private:
	int width;
	int height;
	HWND hWnd;// 被创建出的窗口的句柄
};
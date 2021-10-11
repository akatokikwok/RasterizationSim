#pragma once
#include "ChiliWin.h"
#include "ChiliException.h"
#include "Keyboard.h"
#include "Mouse.h"

class Window
{
public:/// 继承自1个异常类,并分析
	class Exception : public ChiliException
	{
	public:
		Exception(int line, const char* file, HRESULT hr) noexcept;

		const char* what() const noexcept override;/// 打印所有的错误信息 (主要是本类4个成员方法的信息)

		virtual const char* GetType() const noexcept;			   // 重写父类的 获取异常种类
		static std::string TranslateErrorCode(HRESULT hr) noexcept;// 负责把句柄类型 的错误信息转化成字符串
		HRESULT GetErrorCode() const noexcept;					   // 仅拿取hr句柄,这个句柄里信息丰富,保存着错误讯息
		std::string GetErrorString() const noexcept;			   // 仅调用 TranslateErrorCode
	private:
		HRESULT hr;// 异常类子类里自带1个句柄
	};


private:
	/// 类中类, 单例来负责管理 WindowClass的注册与清理
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
	Window(int width, int height, const char* name);
	/* 析构器仅负责 调用DestroyWindow系统API销毁窗口句柄*/
	~Window();
	Window(const Window&) = delete;
	Window& operator=(const Window&) = delete;

	void SetTitle(const std::string& title);// 测试用函数
private:
	/* 窗口的消息机制处理函数, 设计成静态的原因是 我们不可以通过WinAPI调用类成员函数*/
	static LRESULT CALLBACK HandleMsgSetup(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;
	static LRESULT CALLBACK HandleMsgThunk(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;
	/* 供上面2个静态消息处理函数 调用,使用此函数间接地参与消息处理机制*/
	LRESULT HandleMsg(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;
public:
	Keyboard kbd;// 持有1个键盘,键盘类本质上是窗口类控制字符的一种延伸思想
	Mouse mouse; // 持有1个鼠标
private:
	int width;
	int height;
	HWND hWnd;// 被创建出的窗口的句柄
};

// error exception helper macro
#define CHWND_EXCEPT( hr ) Window::Exception( __LINE__, __FILE__, hr ) 
#define CHWND_LAST_EXCEPT() Window::Exception( __LINE__, __FILE__, GetLastError() )
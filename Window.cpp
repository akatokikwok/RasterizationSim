#include "Window.h"
#include <sstream>
#include "resource.h"

// Window Class Stuff
Window::WindowClass Window::WindowClass::wndClass;

Window::WindowClass::WindowClass() noexcept
	:
	hInst(GetModuleHandle(nullptr))// GetModuleHandleAPI负责初始化句柄
{
	WNDCLASSEX wc = { 0 };
	wc.cbSize = sizeof(wc);			// 值就是窗口类的大小
	wc.style = CS_OWNDC;			// 使用CS_OWNDC给窗口一个device context,迫使窗口被独立渲染
	wc.lpfnWndProc = HandleMsgSetup;// 窗口的消息处理机制; 此字段负责处理所有有关窗口的消息,通常与外部的一个消息处理函数绑定
	wc.cbClsExtra = 0;				// 可以用它存储数据,但是暂时设为空
	wc.cbWndExtra = 0;				// 用于给每个窗口实例分配字节
	wc.hInstance = GetInstance();	// 即WinMain入口的入参
	wc.hIcon = static_cast<HICON>(LoadImage(
		GetInstance(), MAKEINTRESOURCE(IDI_ICON2),
		IMAGE_ICON, 32, 32, 0
	));								// 用于定义图标
	wc.hCursor = nullptr;			// 用于定义鼠标
	wc.hbrBackground = nullptr;		// GDI据此绘制窗口背景,社为空表示弃用GDI需要人手动绘制背景
	wc.lpszMenuName = nullptr;		// 暂设为空
	wc.lpszClassName = GetName();	// 表示窗口实例依赖于哪个className开始绘制
	wc.hIconSm = static_cast<HICON>(LoadImage(
		GetInstance(), MAKEINTRESOURCE(IDI_ICON1),
		IMAGE_ICON, 16, 16, 0
	));								// 用于给不同的窗口设置不同的图标
	RegisterClassEx(&wc);// 窗口类注册完毕
}

Window::WindowClass::~WindowClass()
{
	UnregisterClass(wndClassName, GetInstance());
}

const char* Window::WindowClass::GetName() noexcept
{
	return wndClassName;
}

HINSTANCE Window::WindowClass::GetInstance() noexcept
{
	return wndClass.hInst;
}


// Window Stuff
Window::Window(int width, int height, const char* name)
{
	// Client区域,不包括标题栏和边框
	RECT wr;
	wr.left = 100;
	wr.right = width + wr.left;
	wr.top = 100;
	wr.bottom = height + wr.top;

	if (AdjustWindowRect(&wr/*裁剪矩形*/, WS_CAPTION | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_OVERLAPPEDWINDOW | WS_SYSMENU,
			FALSE/*是否具备菜单*/) == 0)/*自适应匹配窗口尺寸*/
	{
		throw CHWND_LAST_EXCEPT();
	}

	/// 创建窗口实例句柄
	hWnd = CreateWindow(
		WindowClass::GetName(), // 窗口实例名
		name,// 窗口本身名字
		WS_CAPTION | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_OVERLAPPEDWINDOW | WS_SYSMENU,//窗口样式
		CW_USEDEFAULT, CW_USEDEFAULT, // 窗口位置
		wr.right - wr.left, wr.bottom - wr.top, // 这两个表示窗口尺寸
		nullptr/*父级窗口*/, nullptr, WindowClass::GetInstance(), this/*设置为本窗口实例this*/
	);
	// check for error
	if (hWnd == nullptr) {
		throw CHWND_LAST_EXCEPT();
	}

	// 呈现展示此窗口(使用句柄)
	ShowWindow(hWnd, SW_SHOWDEFAULT);
}

void Window::SetTitle(const std::string& title)
{
	if (SetWindowText(hWnd, title.c_str()) == 0) {
		throw CHWND_LAST_EXCEPT();
	}
}

Window::~Window()
{
	DestroyWindow(hWnd);// 仅负责 调用DestroyWindow系统API销毁窗口句柄
}

LRESULT WINAPI Window::HandleMsgSetup(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
{
	if (msg == WM_NCCREATE) { // 先检测是不是non-client区域创建的消息
		// 先把lp转换成 CREATESTRUCTW型 变量
		const CREATESTRUCTW* const pCreate = reinterpret_cast<CREATESTRUCTW*>(lParam);
		// 再把lp内部的lpCreateParams字段强转为 窗口实例指针
		Window* const pWnd = static_cast<Window*>(pCreate->lpCreateParams);
		// set WinAPI-managed user data to store ptr to window class; 
		// 即从数据类型使用的角度上为用户窗口设计数据; 数据是从WINAPI端的USERDATA里取得,将其映射为窗口指针
		SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pWnd));
		// set message proc to normal (non-setup) handler now that setup is finished
		// 注意,这里的设计意图是让所有的消息均被Window::HandleMsgThunk处理 而非 由Window::HandleMsgSetup处理
		SetWindowLongPtr(hWnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(&Window::HandleMsgThunk));
		// forward message to window class handler
		return pWnd->HandleMsg(hWnd, msg, wParam, lParam);
	}

	// 科普,窗口被创建 早于 WM_CREATE的时间点
	return DefWindowProc(hWnd, msg, wParam, lParam);
}

/// 真正的消息处理接口
LRESULT WINAPI Window::HandleMsgThunk(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
{
	// retrieve ptr to window class
	Window* const pWnd = reinterpret_cast<Window*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
	// forward message to window class handler
	return pWnd->HandleMsg(hWnd, msg, wParam, lParam);
}

/// 供上面2个静态消息处理函数 调用,使用此函数间接地参与消息处理机制
LRESULT Window::HandleMsg(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
{
	switch (msg) {
		case WM_CLOSE:
			PostQuitMessage(0);// 这一步表示当点了关闭窗口,仅会post消息而非销毁窗口,因为销毁工作是在析构器里负责的
			return 0;
			/*********** 当主窗口失去了焦点,焦点落在别的消息窗时候,需要清除那个256位的bitset ***********/
		case WM_KILLFOCUS:
			kbd.ClearState();
			break;

			/*********** KEYBOARD MESSAGES ***********/
		case WM_KEYDOWN:

			// 在这种case里 诸如ALT key (VK_MENU) and F10的系统键位才会被认出来
		case WM_SYSKEYDOWN:
			if (!(lParam & 0x40000000) || kbd.AutorepeatIsEnabled()) // 此判断是为了过滤重复按键,仅需要按键的一次消息就足够了
			{
				kbd.OnKeyPressed(static_cast<unsigned char>(wParam));
			}
			break;
		case WM_KEYUP:

		case WM_SYSKEYUP:
			kbd.OnKeyReleased(static_cast<unsigned char>(wParam));
			break;
		case WM_CHAR:
			kbd.OnChar(static_cast<unsigned char>(wParam));
			break;
			/*********** END KEYBOARD MESSAGES ***********/

			/************* MOUSE MESSAGES ****************/
		case WM_MOUSEMOVE:
		{
			POINTS pt = MAKEPOINTS(lParam);// 科普:一般lParam储存光标的坐标位置
			mouse.OnMouseMove(pt.x, pt.y);
		}
		case WM_LBUTTONDOWN:
		{
			const POINTS pt = MAKEPOINTS(lParam);
			mouse.OnLeftPressed(pt.x, pt.y);
			break;
		}
		case WM_RBUTTONDOWN:
		{
			const POINTS pt = MAKEPOINTS(lParam);
			mouse.OnRightPressed(pt.x, pt.y);
			break;
		}
		case WM_LBUTTONUP:
		{
			const POINTS pt = MAKEPOINTS(lParam);
			mouse.OnLeftReleased(pt.x, pt.y);
			break;
		}
		case WM_RBUTTONUP:
		{
			const POINTS pt = MAKEPOINTS(lParam);
			mouse.OnRightReleased(pt.x, pt.y);
			break;
		}
		case WM_MOUSEWHEEL:
		{
			const POINTS pt = MAKEPOINTS(lParam);
			if (GET_WHEEL_DELTA_WPARAM(wParam) > 0) {// 利用wParam的正负值判定滚轮方向
				mouse.OnWheelUp(pt.x, pt.y);
			}
			else if (GET_WHEEL_DELTA_WPARAM(wParam) < 0) {
				mouse.OnWheelDown(pt.x, pt.y);
			}
			break;
		}
		/************** END MOUSE MESSAGES **************/
	}
	// 返回默认的消息处理lpfn
	return DefWindowProc(hWnd, msg, wParam, lParam);
}


// Window Exception Stuff
Window::Exception::Exception(int line, const char* file, HRESULT hr) noexcept
	:
	ChiliException(line, file),// 调异常基类的构造器
	hr(hr)
{}

/// 打印所有的错误信息 (主要是本类4个成员方法的信息)
const char* Window::Exception::what() const noexcept
{
	std::ostringstream oss;
	oss << GetType() << std::endl
		<< "[Error Code] " << GetErrorCode() << std::endl
		<< "[Description] " << GetErrorString() << std::endl
		<< GetOriginString();

	whatBuffer = oss.str();
	return whatBuffer.c_str();
}

/// 在窗口类中 把异常种类视作 特有的字符串"grb Window Exception"
const char* Window::Exception::GetType() const noexcept
{
	return "grb Window Exception";
}

/// 负责把句柄类型 的错误信息转化成字符串
std::string Window::Exception::TranslateErrorCode(HRESULT hr) noexcept
{
	char* pMsgBuf = nullptr; // 这个指针专门负责 指向 携带错误信息的字符串
	DWORD nMsgLen = FormatMessage( // FormatMessage内建接口负责把 HR转成 字符串型; nMsgLen表示是错误代码的长度
		FORMAT_MESSAGE_ALLOCATE_BUFFER |
		FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
		nullptr, hr, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		reinterpret_cast<LPSTR>(&pMsgBuf), 0, nullptr
	);
	if (nMsgLen == 0) {
		return "Unidentified error code";
	}

	std::string errorString = pMsgBuf;
	LocalFree(pMsgBuf);
	return errorString;
}

/// 仅拿取hr句柄,这个句柄里信息丰富,保存着错误讯息
HRESULT Window::Exception::GetErrorCode() const noexcept
{
	return hr;
}

/// 仅调用 TranslateErrorCode
std::string Window::Exception::GetErrorString() const noexcept
{
	return TranslateErrorCode(hr);
}
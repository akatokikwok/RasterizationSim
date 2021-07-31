#include <Windows.h>

int CALLBACK/*调用约定,即stdcall,参数传栈*/ WinMain(
	HINSTANCE hInstance,/*指向结构指针,例如加载进内存*/
	HINSTANCE hPrevInstance,
	LPSTR     lpCmdLine,/*使用这个命令行来接收别人发来的一些参数,只接受字符串*/
	int       nCmdShow/*关联窗口创建后的展示方式*/
)
{
	const auto pClassName = "Dx113dclassname";// 自定义1个窗口实例名字
	/// 注册窗口类,使用WindowsAPI
	WNDCLASSEX wc = { 0 };
	wc.cbSize = sizeof(wc);			// 值就是窗口类的大小
	wc.style = CS_OWNDC;			// 使用CS_OWNDC给窗口一个device context,迫使窗口被独立渲染
	wc.lpfnWndProc = DefWindowProc; // 此字段负责处理所有有关窗口的消息,通常与外部的一个消息处理函数绑定
	wc.cbClsExtra = 0;				// 可以用它存储数据,但是暂时设为空
	wc.cbWndExtra = 0;				// 用于给每个窗口实例分配字节
	wc.hInstance = hInstance;		// 即WinMain入口的入参
	wc.hIcon = nullptr;				// 用于定义图标
	wc.hCursor = nullptr;			// 用于定义鼠标
	wc.hbrBackground = nullptr;		// GDI据此绘制窗口背景,社为空表示弃用GDI需要人手动绘制背景
	wc.lpszMenuName = nullptr;		// 暂设为空
	wc.lpszClassName = pClassName;	// 表示窗口实例依赖于哪个className开始绘制
	wc.hIconSm = nullptr;			// 用于给不同的窗口设置不同的图标
	RegisterClassEx(&wc);// 窗口类注册完毕
	/// 创建窗口实例句柄
	HWND hWnd = CreateWindowEx(
		0, 
		pClassName/*具体的窗口实例名字*/,
		"Happy Hard Window"/*窗口的名字*/,
		WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU,/*窗口样式*/
		200, 200, 640, 480,
		nullptr/*父级窗口*/, nullptr, hInstance, nullptr
	);
	// 呈现展示此窗口(使用句柄)
	ShowWindow(hWnd, SW_SHOW);
	
	while (true);// 阻止程序立刻退出
	return 0;
}
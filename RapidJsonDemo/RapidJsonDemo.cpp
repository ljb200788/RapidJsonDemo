// RapidJsonDemo.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <rapidjson\rapidjson.h> 
#include <rapidjson\document.h> 
#include <rapidjson\writer.h> 
#include <rapidjson\stringbuffer.h>
#include <iostream>
#include <ostream>
#include <stdio.h>
#include <windows.h>
#include <tlhelp32.h>
#pragma warning（disable：4996）

using namespace rapidjson;
using namespace std;

void testOffline()
{
	//std::string para = "{\"message\":\"调用C++方法\",\"type\":\"offline\"}";

	//std::string para = "{\"message\":\"调用C++方法\"}";

	std::string para = "{\"message\":\"调用C++方法\",\"type\":}";



	rapidjson::Document doc;
	doc.Parse(para.c_str());

	if (!doc.IsNull())
	{
		if (doc.HasMember("type"))
		{
			Value& m = doc["type"];
			if (m.IsString())
			{
				string type = m.GetString();
				if (type.find("offline") != string::npos)
				{
					std::cout << type.c_str() << endl;
				}
			}
		}
	}
}

void testWebsocketMessageBox()
{

	//std::string para = "{\"message\":, \"url\" : \"http://172.18.0.105/index.html#/ada/diagResult?0.20173703965271406\", \"width\" : null, \"height\" : null}";

	std::string para = nullptr;

	//std::string para = "{}";

	rapidjson::Document doc;
	doc.Parse(para.c_str());

	if (!doc.IsNull())
	{
		if (doc.HasMember("url"))
		{
			Value& m = doc["url"];
			if (m.IsString())
			{
				std::string url = m.GetString();

				int width = 0;
				int height = 0;
				if (doc.HasMember("width"))
				{
					Value& m = doc["width"];
					if (m.IsInt())
					{
						width = m.GetInt() + 50;
					}
				}

				if (doc.HasMember("height"))
				{
					Value& m = doc["height"];
					if (m.IsInt())
					{
						height = m.GetInt() + 35;
					}
				}

			}
		}
	}
}
string wstr2str(const wstring &wstr)
{

	string curLocale = setlocale(LC_ALL, NULL); // curLocale = "C";

	setlocale(LC_ALL, "chs");

	const wchar_t* _Source = wstr.c_str();
	size_t _Dsize = 2 * wstr.size() + 1;
	char *_Dest = new char[_Dsize];
	memset(_Dest, 0, _Dsize);
	size_t   i;
	wcstombs_s(&i,_Dest, _Dsize,_Source,_Dsize);
	string result = _Dest;
	delete[]_Dest;

	setlocale(LC_ALL, curLocale.c_str());

	return result;
}
wstring str2wstr(const string &str)
{
	setlocale(LC_ALL, "chs");
	const char* _Source = str.c_str();
	size_t _Dsize = str.size() + 1;
	wchar_t *_Dest = new wchar_t[_Dsize];
	wmemset(_Dest, 0, _Dsize);
	size_t   i;
	mbstowcs_s(&i,_Dest, _Dsize, _Source, _Dsize);
	wstring result = _Dest;
	delete[]_Dest;

	setlocale(LC_ALL, "C");

	return result;
}

void StartAssistant()
{
	TCHAR _szPath[MAX_PATH + 1] = { 0 };
	GetModuleFileName(NULL, _szPath, MAX_PATH);
	(_tcsrchr(_szPath, _T('\\')))[1] = 0;//删除文件名，只获得路径 字串
	string strPath;
	for (int n = 0; _szPath[n]; n++)
	{
		if (_szPath[n] != _T('\\'))
		{
			strPath += _szPath[n];
		}
		else
		{
			strPath += "\\\\";
		}
	}

	string path = strPath + "DiagnosticAssistant.exe";
	cout << path.c_str() << endl;
	ShellExecute(NULL, _T("open"), str2wstr(path).c_str(), NULL, NULL, SW_SHOWNORMAL);
}
void AutoStart()
{
	HKEY hKey;
	std::wstring strRegPath = L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run";
	try
	{
		//1、找到系统的启动项  
		if (RegOpenKeyEx(HKEY_CURRENT_USER, strRegPath.c_str(), 0, KEY_ALL_ACCESS, &hKey) == ERROR_SUCCESS) ///打开启动项       
		{
			//2、得到本程序自身的全路径
			TCHAR strExeFullDir[MAX_PATH];
			GetModuleFileName(NULL, strExeFullDir, MAX_PATH);

			//3、判断注册表项是否已经存在
			TCHAR strDir[MAX_PATH] = {};
			DWORD nLength = MAX_PATH;
			long result = RegGetValue(hKey, nullptr, L"DiagnosticAssistantStart", RRF_RT_REG_SZ, 0, strDir, &nLength);

			StartAssistant();


			//4、不存在
			if (result != ERROR_SUCCESS || _tcscmp(strExeFullDir, strDir) != 0)
			{

				//5、添加一个子Key,并设置值，"GISRestart"是应用程序名字（不加后缀.exe） 
				RegSetValueEx(hKey, L"DiagnosticAssistantStart", 0, REG_SZ, (LPBYTE)strExeFullDir,
					(lstrlen(strExeFullDir) + 1) * sizeof(TCHAR));
				//6、关闭注册表
				RegCloseKey(hKey);
			}
		}
		else
		{
			std::cout << "警告\n系统参数错误,不能随系统启动" << std::endl;
		}
	}
	catch (std::exception e)
	{
	}

}
int main()
{

	//testWebsocketMessageBox();
	AutoStart();
    return 0;
}


/*

void GetWordContentThreadFun()
{

SetCursorPos(500, 500);

mouse_event(MOUSEEVENTF_LEFTDOWN | MOUSEEVENTF_LEFTUP, 500, 500, 0, 0);

POINT rect;
GetCursorPos(&rect);

YLog log(YLog::INFO, "log.txt", YLog::ADD);
log.W(__FILE__, __LINE__, YLog::INFO, "x", rect.x);
log.W(__FILE__, __LINE__, YLog::INFO, "y", rect.y);

//mouse_event(MOUSEEVENTF_RIGHTDOWN | MOUSEEVENTF_RIGHTUP, 500, 500, 0, 0);

keybd_event(VK_CONTROL, 0, 0, 0);
keybd_event('A', 0, 0, 0);
//std::this_thread::sleep_for(std::chrono::milliseconds(100));
keybd_event('A', 0, KEYEVENT_KEYUP, 0);
keybd_event(VK_CONTROL, 0, KEYEVENT_KEYUP, 0);


SetCursorPos(500, 500);
mouse_event(MOUSEEVENTF_RIGHTDOWN | MOUSEEVENTF_RIGHTUP, 500, 500, 0, 0);
std::this_thread::sleep_for(std::chrono::milliseconds(3000));
SetCursorPos(550, 185);
std::this_thread::sleep_for(std::chrono::milliseconds(3000));
mouse_event(MOUSEEVENTF_LEFTDOWN | MOUSEEVENTF_LEFTUP, 550, 185, 0, 0);
std::this_thread::sleep_for(std::chrono::milliseconds(3000));
SetCursorPos(500, 500);
mouse_event(MOUSEEVENTF_LEFTDOWN | MOUSEEVENTF_LEFTUP, 500, 500, 0, 0);
std::this_thread::sleep_for(std::chrono::milliseconds(3000));
mouse_event(MOUSEEVENTF_RIGHTDOWN | MOUSEEVENTF_RIGHTUP, 500, 500, 0, 0);
std::this_thread::sleep_for(std::chrono::milliseconds(3000));
SetCursorPos(550, 235);
std::this_thread::sleep_for(std::chrono::milliseconds(3000));
mouse_event(MOUSEEVENTF_LEFTDOWN | MOUSEEVENTF_LEFTUP, 550, 235, 0, 0);
std::this_thread::sleep_for(std::chrono::milliseconds(3000));

return;
std::this_thread::sleep_for(std::chrono::milliseconds(500));
if (OpenClipboard(NULL))//打开剪贴板  
{
	if (IsClipboardFormatAvailable(CF_TEXT))//判断格式是否是我们所需要  
	{
		HANDLE hClip;
		char* pBuf;

		//读取数据  
		hClip = GetClipboardData(CF_TEXT);
		pBuf = (char*)GlobalLock(hClip);
		GlobalUnlock(hClip);
		MessageBox(NULL, nbase::UTF8ToUTF16(pBuf).c_str(), _T(""), 0);
		CloseClipboard();
	}
}


}

void GetTextContentThreadFun()
{

	for (int i = 0; i < 20; i++)
	{
		SetCursorPos(500, 500);

		mouse_event(MOUSEEVENTF_LEFTDOWN | MOUSEEVENTF_LEFTUP, 500, 500, 0, 0);

		POINT rect;
		GetCursorPos(&rect);

		YLog log(YLog::INFO, "log.txt", YLog::ADD);
		log.W(__FILE__, __LINE__, YLog::INFO, "x", rect.x);
		log.W(__FILE__, __LINE__, YLog::INFO, "y", rect.y);

		mouse_event(MOUSEEVENTF_RIGHTDOWN | MOUSEEVENTF_RIGHTUP, 500, 500, 0, 0);

		std::this_thread::sleep_for(std::chrono::milliseconds(3000));
		SetCursorPos(550, 330);
		std::this_thread::sleep_for(std::chrono::milliseconds(3000));
		mouse_event(MOUSEEVENTF_LEFTDOWN | MOUSEEVENTF_LEFTUP, 550, 330, 0, 0);
		std::this_thread::sleep_for(std::chrono::milliseconds(3000));
		SetCursorPos(500, 500);
		mouse_event(MOUSEEVENTF_RIGHTDOWN | MOUSEEVENTF_RIGHTUP, 500, 500, 0, 0);
		std::this_thread::sleep_for(std::chrono::milliseconds(3000));
		SetCursorPos(550, 260);
		std::this_thread::sleep_for(std::chrono::milliseconds(3000));
		mouse_event(MOUSEEVENTF_LEFTDOWN | MOUSEEVENTF_LEFTUP, 550, 260, 0, 0);
		std::this_thread::sleep_for(std::chrono::milliseconds(3000));
		SetCursorPos(500, 500);
		mouse_event(MOUSEEVENTF_LEFTDOWN | MOUSEEVENTF_LEFTUP, 500, 500, 0, 0);
		std::this_thread::sleep_for(std::chrono::milliseconds(3000));
		mouse_event(MOUSEEVENTF_RIGHTDOWN | MOUSEEVENTF_RIGHTUP, 500, 500, 0, 0);
		std::this_thread::sleep_for(std::chrono::milliseconds(3000));
		SetCursorPos(550, 280);
		std::this_thread::sleep_for(std::chrono::milliseconds(3000));
		mouse_event(MOUSEEVENTF_LEFTDOWN | MOUSEEVENTF_LEFTUP, 550, 280, 0, 0);
		std::this_thread::sleep_for(std::chrono::milliseconds(3000));
	}

	return;
	std::this_thread::sleep_for(std::chrono::milliseconds(500));
	if (OpenClipboard(NULL))//打开剪贴板  
	{
		if (IsClipboardFormatAvailable(CF_TEXT))//判断格式是否是我们所需要  
		{
			HANDLE hClip;
			char* pBuf;

			//读取数据  
			hClip = GetClipboardData(CF_TEXT);
			pBuf = (char*)GlobalLock(hClip);
			GlobalUnlock(hClip);
			MessageBox(NULL, nbase::UTF8ToUTF16(pBuf).c_str(), _T(""), 0);
			CloseClipboard();
		}
	}
}
*/

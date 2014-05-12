#include <Windows.h>
#include <fstream>
#include <string>
#include <vector>
#include <cstdlib>
#include <wchar.h>
#include "BasicWindow.h"

#define IDC_MAIN_EDIT 101
#define IDC_MAIN_BUTTON 102
HWND hEdit;
HWND hButton;
HWND hRead;
bool edit_triggered = false;

wchar_t text[131072];

using namespace std;

//struct that will hold the user properites -- values to be read from a file
struct UserProperties{

	string font_size;
	int chunk_size;
	int wpm;

};

//to be used the first time a user runs the program, creates the properties file
void createFile(){
	
	ofstream ofs("user.properties");
	
	if(ofs.good()){
		
		ofs << "font-size:16\n";
		ofs << "chunk-size:1\n";
		ofs << "wpm:400";

	}

	ofs.close();
}

//reads the user properties file
UserProperties readFile(){

	UserProperties userp;
	string arr[3];

	ifstream ifs("user.properties");
	if(!ifs.good()){
		createFile();
		ifs.open("user.properties");
	}

	string line = "";
	string last = "";
	getline(ifs, line);

	for(int i = 0; line != last; i++){
		int position = line.find(':') + 1;
		arr[i] = line.substr(position, line.length());
		last = line;
		getline(ifs, line);
	}
	
	userp.font_size = arr[0];
	userp.chunk_size = atoi(arr[1].c_str());
	userp.wpm = atoi(arr[2].c_str());

	ifs.close();

	return userp;
}

//two variables for using QueryPerformanceCounter as a timer
__int64 start = 0;
float freq = 0;

//starts the timer
void startCounter(){
	LARGE_INTEGER li;
	QueryPerformanceCounter(&li);

	freq = float(li.QuadPart)/55000.0;
	QueryPerformanceCounter(&li);
	start = li.QuadPart;
}

//returns the current time of the timer
float getCounter(){

	LARGE_INTEGER li;
	QueryPerformanceCounter(&li);
	return float(li.QuadPart - start) / freq;

}

//sepatates words
vector<wstring> separate(){
	
	int count = 0;
	vector<wstring> out;
	wchar_t *str = new wchar_t[sizeof(text)/sizeof(wchar_t)];
	wcscpy(str, text);
	
	wchar_t *space = L" ";

	wchar_t* token = wcstok(str, space);

	while(token != NULL){
		out.push_back(token);
		token = wcstok(NULL, space);
	}

	delete [] str;
	delete [] token;
	return out;
}

LRESULT CALLBACK DlgProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam){

	switch(msg){
	case WM_INITDIALOG:
		hRead = GetDlgItem(hwnd, IDC_READ_TEXT);
		return TRUE;
	case WM_COMMAND:
		switch(wParam){
		case IDC_BUTTON_PLAY:
			break;
		case IDC_BUTTON_RESET:
			break;
		case IDC_SLIDER1:
			break;
		}
		break;
	case WM_CLOSE:
		EndDialog(hwnd, 0);
		break;
	}
	return FALSE;

}

//message loop for the main window
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam){

	//text box width
	int width = 100, height = 100;
	vector<wstring> words;

	switch(msg){
	case WM_CREATE:
		RECT rect;
		if(GetWindowRect(hwnd, &rect)){
			width = rect.right - rect.left - 20;
			height = rect.bottom - rect.top - 60;
		}
		hEdit = CreateWindowEx(WS_EX_CLIENTEDGE, L"EDIT", L"", WS_CHILD|WS_VISIBLE|ES_MULTILINE|WS_VSCROLL, 0, 0, width, 400, hwnd, (HMENU)IDC_MAIN_EDIT, GetModuleHandle(NULL), NULL);
		SendMessage(hEdit, WM_SETTEXT, NULL, (LPARAM)L"Enter text here...");
		hButton = CreateWindowEx(NULL, L"BUTTON", L"Start Reader", WS_TABSTOP|WS_VISIBLE|WS_CHILD|BS_DEFPUSHBUTTON, width - 150, height - 74, 100, 24, hwnd, (HMENU)IDC_MAIN_BUTTON, GetModuleHandle(NULL), NULL);
		break;
	case WM_LBUTTONDOWN:
		break;
	case WM_COMMAND:
		switch(LOWORD(wParam)){
		case ID_FILE_EXIT:
			PostMessage(hwnd, WM_CLOSE, NULL, NULL);
			break;
		case ID_EDIT_PREFERENCES:
			break;
		case ID_VIEW_SCHEME:
			break;
		case IDC_MAIN_BUTTON:
			GetWindowText(hEdit, text, 1000);
			words = separate();
			for(int i = 0; i <  words.size(); i++){
				MessageBox(NULL, words.at(i).c_str(), L"Error", MB_ICONEXCLAMATION | MB_OK);
			}
			DialogBox(NULL, MAKEINTRESOURCE(IDD_READAPP), hwnd, (DLGPROC)DlgProc);
			break;
		case IDC_MAIN_EDIT:
			if(!edit_triggered){
				SendMessage(hEdit, WM_SETTEXT, NULL, (LPARAM)L"");
				edit_triggered = true;
			}
			break;
		default:
			break;
		}
		break;
	case WM_CLOSE:
		DestroyWindow(hwnd);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hwnd, msg, wParam, lParam);
	}
	return 0;	
};

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd){

	UserProperties user_props = readFile();

	BasicWindow window(hInstance, 800, 600, WndProc);
	window.showWindow(nShowCmd);
}
#include <cstdio>
#include <windows.h>
#include <string>
#include <vector>
#include <iostream>
using namespace std;

vector<string> g_s;

void t(const string &path){
    WIN32_FIND_DATA dirFile;
    string searchPath = path + "\\*";
    HANDLE hFind = FindFirstFile(searchPath.c_str(), &dirFile);

    if (hFind == INVALID_HANDLE_VALUE){
        return;
    }
    do{
        string fileName = dirFile.cFileName;
        // skip
        if(fileName == "." || fileName == ".."){
            continue;
        }

        string fullFileName = path + "\\" + fileName;
        // dir
        if(dirFile.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY){
            t(fullFileName);
        }
        // file
        else{
            for(int i = 0; i < g_s.size(); i++){
                int pos = fullFileName.rfind(g_s[i]);
                if(pos != string::npos){
                    remove(fullFileName.c_str());
                }
            }
        }
    // search next file
    } while (FindNextFile(hFind, &dirFile));
    // close
    FindClose(hFind);
}

int main(int argc, char **argv){
    g_s.push_back(".cpp");
    g_s.push_back(".jpg");

    vector<string> args(argv, argv + argc);
    //引数は0は実行ファイルの絶対パス
    //cout << args[0] << endl;
    //if(argc <= 2) return 0;
    string s = "E:\\workspace\\deltest";
    t(s);
    return 0;
}

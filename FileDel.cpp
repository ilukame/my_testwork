// g++ -O3 -o DeleteFiles DeleteFiles_gcc.cpp -static
#include <iostream>
#include <windows.h>
#include <string>
#include <vector>
using namespace std;

// サブフォルダも検索しながらfileExの拡張子が付いたファイルを削除する
bool DelFiles(const string &path, const vector<string> &fileEx) {
	WIN32_FIND_DATA dirFile;
	string searchPath = path + "\\*"; // all file and dir search
	HANDLE hFind = FindFirstFile(searchPath.c_str(), &dirFile);

	// file and dir not find
	if (hFind == INVALID_HANDLE_VALUE) {
		return false;
	}
	do {
		string fileName = dirFile.cFileName;
		// skip
		if (fileName == "." || fileName == "..") {
			continue;
		}

		string fullFileName = path + "\\" + fileName;
		// dir
		if (dirFile.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
			if (DelFiles(fullFileName, fileEx) == false) {
				return false;
			}
		}
		// file
		else {
			for (int i = 0; i < fileEx.size(); i++) {
				// 右から検索
				int pos = fullFileName.rfind(fileEx[i]);
				// 右端ではなかったら削除しない
				if (pos != string::npos && fileEx[i].size() + pos == fullFileName.size()) {
					// 削除
					if (remove(fullFileName.c_str()) != 0) {
						printf("error\n");
						return false;
					}
					break;
				}
			}
		}
		// search next file
	} while (FindNextFile(hFind, &dirFile));
	// close
	FindClose(hFind);
	return true;
}


// 最初がドットではない場合は削除．
void DelNotDot(vector<string> &extensnion_names) {
	// .が付かないものは後ろから消す
	vector<int> indexes;
	for (int i = 0; i < extensnion_names.size(); i++) {
		if (extensnion_names[i][0] != '.') {
			indexes.push_back(i);
		}
	}
	for (int i = indexes.size() - 1; 0 <= i; i--) {
		extensnion_names.erase(extensnion_names.begin() + indexes[i]);
	}
}

int main(int argc, char **argv){
    vector<string> args(argv, argv + argc);
    //cout << args[0] << endl;
    if(argc <= 2){
		printf("argument is short\n");
		printf("ex : ./DeleteFiles dir file_ex1 file_ex2...\n");
		printf("ex : ./DeleteFiles D:\\workspace\\python\\deltest .py .cpp .js");
		return 0;
	}
	string path = args[1];

	// 拡張子だけにする
	args.erase(args.begin());
	args.erase(args.begin());
	DelNotDot(args);
	if (args.size() == 0) return 0;

    if(DelFiles(path, args)){
		printf("Delete Complete!");
	}
	else{
		printf("error");
	}
    return 0;
}

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

// 配列を表示する関数 (dncの「可視化(A)」の代用)
void printArray(const int arr[], int size) {
    for (int i = 0; i < size; i++) {
        std::cout << arr[i] << " ";
    }
    std::cout << std::endl;
}

// DNCファイルの内容を処理する関数
std::string processContent(std::istream& input) {
    std::stringstream processed;
    std::string line;
    
    while (std::getline(input, line)) {
        size_t pos = 0;

        // '整数' を 'int ' に変換
        while ((pos = line.find("整数 ")) != std::string::npos) {
            line.replace(pos, 6, "int ");
        }

        // '←' を '=' に変換
        pos = 0;
        while ((pos = line.find("←")) != std::string::npos) {
            line.replace(pos, 3, " = ");
        }

        // '→' を '.' に変換
        pos = 0;
        while ((pos = line.find("→")) != std::string::npos) {
            line.replace(pos, 3, ".");
        }

        // '可視化(A)' を 'printArray(A, サイズ)'
        pos = 0;
        while ((pos = line.find("可視化(")) != std::string::npos) {
            line.replace(pos, 9, "printArray(");
        }

        processed << line << std::endl;
    }

    return processed.str();
}

void insertContent(const std::string& sourceFile, const std::string& targetFile, const std::string& outputFile, const std::string& marker) {
    std::ifstream src(sourceFile);
    std::ifstream tgt(targetFile);
    std::ofstream out(outputFile);

    if (!src || !tgt || !out) {
        std::cerr << "Error: Failed to open one or more files." << std::endl;
        return;
    }

    // dncファイルの内容を処理
    std::string processedContent = processContent(src);

    std::string line;
    bool inserted = false;

    while (std::getline(tgt, line)) {
        out << line << std::endl;
        if (line.find(marker) != std::string::npos) {
            out << processedContent;  // 変換された内容を挿入
            inserted = true;
        }
    }

    if (!inserted) {
        std::cerr << "Warning: Marker not found in target file." << std::endl;
    } else {
        std::cout << "Content inserted successfully into " << outputFile << std::endl;
    }

    src.close();
    tgt.close();
    out.close();
}

int main() {
    std::string sourceFile = "sample.dnc";   // 入力DNCファイル
    std::string targetFile = "template.cpp"; // テンプレートC++ファイル
    std::string outputFile = "output.cpp";   // 出力C++ファイル
    std::string marker = "// ここにdncファイルの中身をコピーアンドペースト";

    insertContent(sourceFile, targetFile, outputFile, marker);

    return 0;
}

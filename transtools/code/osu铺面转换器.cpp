#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

using namespace std;


int main() {
    cout << "osu谱面转换器" << endl;
    cout << "将需要转换的.osu文件放在本程序同一目录下，输入文件名即可(不需要后缀)" << endl;
    cout << "转换后的文件将输出到本程序同一目录下，输入文件名即可(不需要后缀)" << endl;
    while (1) {
        string namein, nameout;


        cout << "请输入读取文件名" << endl;
        getline(cin, namein);




        std::ifstream file(namein+".osu"); // 请替换成你的文件名
        if (file.is_open()) {

        }
        else {
           std::cout << "无法打开读取文件" << std::endl;
           continue;
        }

        cout << "请输入写入文件名" << endl;
        getline(cin, nameout);
        // 创建文件输出流
        std::ofstream outputFile(nameout+".txt"); // 替换为你想要的文件名

        // 检查文件是否成功打开
        if (outputFile.is_open()) {

        }
        else {
            std::cout << "无法打开写入文件" << std::endl;
            continue;
        }

        std::string line;
        bool hitObjectsSection = false;


        while (getline(file, line)) {
            if (line == "[HitObjects]") {
                hitObjectsSection = true;
                continue;
            }

            if (hitObjectsSection) {
                istringstream iss(line);
                string token;
                vector<std::string> tokens;
                int trackNumber;
                int type;
                float startTime;
                float endTime;
                while (getline(iss, token, ',')) {
                    tokens.push_back(token);
                }

                if (tokens.size() >= 5) {

                    trackNumber = stoi(tokens[0]);
                    if (trackNumber == 64)
                        trackNumber = 1;
                    else if (trackNumber == 192)
                        trackNumber = 2;
                    else if (trackNumber == 320)
                        trackNumber = 3;
                    else if (trackNumber == 448)
                        trackNumber = 4;

                    startTime = stod(tokens[2]) / 1000.0; // 毫秒转换为秒
                    type = (tokens[3] == "128") ? 2 : 1;
                    endTime = (tokens[3] == "128") ? stod(tokens[5]) / 1000.0 : startTime;

                    //cout << type << ' ' << trackNumber << ' ' << startTime << ' ' << endTime << endl;
                    outputFile << type << ' ' << trackNumber << ' ' << 3 << ' ' << startTime << ' ' << endTime << '\n';
                }
            }
        }
        file.close();
        outputFile.close();
        cout<< "转换完成" << endl;
    }
    return 0;
}
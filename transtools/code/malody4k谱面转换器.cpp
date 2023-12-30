#include <iostream>
#include <fstream>
#include <string>
#include "rapidjson/document.h"

using namespace rapidjson;
using namespace std;

int main() {
    cout<< "malody4k谱面转换器" << endl;
    cout<<"将需要转换的.mc文件放在本程序同一目录下，输入文件名即可(不需要后缀)"<<endl;
    cout<<"转换后的文件将输出到本程序同一目录下，输入文件名即可(不需要后缀)"<<endl;

    while (1) {
        string namein, nameout;

        cout << "请输入读取文件名" << endl;
        getline(cin, namein);


        std::ifstream file(namein+".mc"); // 替换为你的.mc文件名
        if (!file.is_open()) {
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
            std::cout << "无法打开文件" << std::endl;
            continue;
        }




        // 读取文件内容到一个字符串中
        std::string jsonString((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());

        Document document;
        document.Parse(jsonString.c_str());

        if (document.HasParseError()) {
            std::cout << "解析失败" << std::endl;
            continue;
        }

        // 提取文件中的信息
        std::string creator = document["meta"]["creator"].GetString();
        std::string version = document["meta"]["version"].GetString();
        double bpm = document["time"][0]["bpm"].GetDouble();

        // 输出提取的信息
        std::cout << "创建者: " << creator << std::endl;
        std::cout << "版本: " << version << std::endl;
        std::cout << "BPM: " << bpm << std::endl;

        // 可以继续提取其他信息，比如音符数据等
    // 提取音符数据
        const Value& notes = document["note"];
        if (notes.IsArray()) {
            for (SizeType i = 0; i < notes.Size(); i++) {
                const Value& note = notes[i];
                if (note.HasMember("beat") && note.HasMember("column")) {
                    const Value& beatArray = note["beat"];
                    int beat[3];

                    // 获取音符的节拍信息
                    for (SizeType j = 0; j < beatArray.Size(); j++) {
                        beat[j] = beatArray[j].GetInt();
                    }

                    int endbeat[3];
                    int type;
                    // 检查是否存在 endbeat 字段
                    if (note.HasMember("endbeat")) {
                        const Value& endbeatArray = note["endbeat"];

                        // 获取 endbeat 的节拍信息
                        for (SizeType j = 0; j < endbeatArray.Size(); j++) {
                            endbeat[j] = endbeatArray[j].GetInt();
                        }
                        type = 2;
                    }
                    else {
                        // 如果没有 endbeat 字段，将 endbeat 设为与 beat 相同的值
                        for (SizeType j = 0; j < beatArray.Size(); j++) {
                            endbeat[j] = beat[j];
                        }
                        type = 1;
                    }

                    int column = note["column"].GetInt() + 1;
                    float starttime = (beat[0] + float(beat[1]) / beat[2]) * 60 / bpm;
                    float endtime = (endbeat[0] + float(endbeat[1]) / endbeat[2]) * 60 / bpm;
                    outputFile << type << ' ' << column << ' ' << 3 << ' ' << starttime << ' ' << endtime << '\n';
                    // 输出音符信息
                    //std::cout << "音符 " << i + 1 << ":" << std::endl;
                    //std::cout << "起始节拍: " << beat[0] << " " << beat[1] << " " << beat[2] << std::endl;
                    //std::cout << "结束节拍: " << endbeat[0] << " " << endbeat[1] << " " << endbeat[2] << std::endl;
                    //std::cout << "列: " << column << std::endl;
                }


            }
        }
        else {
            std::cout << "未找到音符数据" << std::endl;
            continue;
        }


        // 关闭文件流
        outputFile.close();
        

        file.close();
        std::cout << "转换完成" << std::endl;
    }
    return 0;
}
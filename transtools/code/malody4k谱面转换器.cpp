#include <iostream>
#include <fstream>
#include <string>
#include "rapidjson/document.h"

using namespace rapidjson;
using namespace std;

int main() {
    cout<< "malody4k����ת����" << endl;
    cout<<"����Ҫת����.mc�ļ����ڱ�����ͬһĿ¼�£������ļ�������(����Ҫ��׺)"<<endl;
    cout<<"ת������ļ��������������ͬһĿ¼�£������ļ�������(����Ҫ��׺)"<<endl;

    while (1) {
        string namein, nameout;

        cout << "�������ȡ�ļ���" << endl;
        getline(cin, namein);


        std::ifstream file(namein+".mc"); // �滻Ϊ���.mc�ļ���
        if (!file.is_open()) {
            std::cout << "�޷��򿪶�ȡ�ļ�" << std::endl;
            continue;
        }

        cout << "������д���ļ���" << endl;
        getline(cin, nameout);
        // �����ļ������
        std::ofstream outputFile(nameout+".txt"); // �滻Ϊ����Ҫ���ļ���

        // ����ļ��Ƿ�ɹ���
        if (outputFile.is_open()) {

        }
        else {
            std::cout << "�޷����ļ�" << std::endl;
            continue;
        }




        // ��ȡ�ļ����ݵ�һ���ַ�����
        std::string jsonString((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());

        Document document;
        document.Parse(jsonString.c_str());

        if (document.HasParseError()) {
            std::cout << "����ʧ��" << std::endl;
            continue;
        }

        // ��ȡ�ļ��е���Ϣ
        std::string creator = document["meta"]["creator"].GetString();
        std::string version = document["meta"]["version"].GetString();
        double bpm = document["time"][0]["bpm"].GetDouble();

        // �����ȡ����Ϣ
        std::cout << "������: " << creator << std::endl;
        std::cout << "�汾: " << version << std::endl;
        std::cout << "BPM: " << bpm << std::endl;

        // ���Լ�����ȡ������Ϣ�������������ݵ�
    // ��ȡ��������
        const Value& notes = document["note"];
        if (notes.IsArray()) {
            for (SizeType i = 0; i < notes.Size(); i++) {
                const Value& note = notes[i];
                if (note.HasMember("beat") && note.HasMember("column")) {
                    const Value& beatArray = note["beat"];
                    int beat[3];

                    // ��ȡ�����Ľ�����Ϣ
                    for (SizeType j = 0; j < beatArray.Size(); j++) {
                        beat[j] = beatArray[j].GetInt();
                    }

                    int endbeat[3];
                    int type;
                    // ����Ƿ���� endbeat �ֶ�
                    if (note.HasMember("endbeat")) {
                        const Value& endbeatArray = note["endbeat"];

                        // ��ȡ endbeat �Ľ�����Ϣ
                        for (SizeType j = 0; j < endbeatArray.Size(); j++) {
                            endbeat[j] = endbeatArray[j].GetInt();
                        }
                        type = 2;
                    }
                    else {
                        // ���û�� endbeat �ֶΣ��� endbeat ��Ϊ�� beat ��ͬ��ֵ
                        for (SizeType j = 0; j < beatArray.Size(); j++) {
                            endbeat[j] = beat[j];
                        }
                        type = 1;
                    }

                    int column = note["column"].GetInt() + 1;
                    float starttime = (beat[0] + float(beat[1]) / beat[2]) * 60 / bpm;
                    float endtime = (endbeat[0] + float(endbeat[1]) / endbeat[2]) * 60 / bpm;
                    outputFile << type << ' ' << column << ' ' << 3 << ' ' << starttime << ' ' << endtime << '\n';
                    // ���������Ϣ
                    //std::cout << "���� " << i + 1 << ":" << std::endl;
                    //std::cout << "��ʼ����: " << beat[0] << " " << beat[1] << " " << beat[2] << std::endl;
                    //std::cout << "��������: " << endbeat[0] << " " << endbeat[1] << " " << endbeat[2] << std::endl;
                    //std::cout << "��: " << column << std::endl;
                }


            }
        }
        else {
            std::cout << "δ�ҵ���������" << std::endl;
            continue;
        }


        // �ر��ļ���
        outputFile.close();
        

        file.close();
        std::cout << "ת�����" << std::endl;
    }
    return 0;
}
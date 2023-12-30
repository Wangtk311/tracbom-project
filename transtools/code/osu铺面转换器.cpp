#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

using namespace std;


int main() {
    cout << "osu����ת����" << endl;
    cout << "����Ҫת����.osu�ļ����ڱ�����ͬһĿ¼�£������ļ�������(����Ҫ��׺)" << endl;
    cout << "ת������ļ��������������ͬһĿ¼�£������ļ�������(����Ҫ��׺)" << endl;
    while (1) {
        string namein, nameout;


        cout << "�������ȡ�ļ���" << endl;
        getline(cin, namein);




        std::ifstream file(namein+".osu"); // ���滻������ļ���
        if (file.is_open()) {

        }
        else {
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
            std::cout << "�޷���д���ļ�" << std::endl;
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

                    startTime = stod(tokens[2]) / 1000.0; // ����ת��Ϊ��
                    type = (tokens[3] == "128") ? 2 : 1;
                    endTime = (tokens[3] == "128") ? stod(tokens[5]) / 1000.0 : startTime;

                    //cout << type << ' ' << trackNumber << ' ' << startTime << ' ' << endTime << endl;
                    outputFile << type << ' ' << trackNumber << ' ' << 3 << ' ' << startTime << ' ' << endTime << '\n';
                }
            }
        }
        file.close();
        outputFile.close();
        cout<< "ת�����" << endl;
    }
    return 0;
}
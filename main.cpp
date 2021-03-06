#include <iostream>
#include <stdio.h>
#include <string>
#include <dirent.h>
#include <dirent.h>
#include <sstream>
#include <vector>
#include <time.h>
#include <iterator>
#include <algorithm>
#include <fstream>
#include <filesystem>
// if were on windows
// void include_libs(){
#ifdef _WIN32 || _WIN64
#include <filesystem>
#include "process.h"
#include <windows.h>
#include <limits.h>
#include <direct.h>
#elif __linux__
#include <unistd.h>
#endif
using namespace std;

const string MUSIC_DIRECTORY = "./Piano Samples/";

string CWD = "";

string platform()
{
#ifdef _WIN32
    return "Windows";
#elif _WIN64
    return "Windows";
#elif __APPLE__ || __MACH__
    return "Mac";
#elif __linux__
    return "Linux";
#elif __FreeBSD__
    return "FreeBSD";
#elif __unix || __unix__
    return "Unix";
#else
    return "Other";
#endif
}
#ifdef _WIN32 || _WIN64
string getCurrentDir()
{
    char buff[MAX_PATH];
    GetModuleFileName(NULL, buff, MAX_PATH);
    string::size_type position = string(buff).find_last_of("\\/");
    return string(buff).substr(0, position);
}
#endif
vector<string> list_dir(const char *path)
{
    struct dirent *entry;
    DIR *dir = opendir(path);

    vector<string> list_directory;
    vector<string> list_directory2;

    if (dir == NULL)
        return list_directory;
    while ((entry = readdir(dir)) != NULL)
        list_directory.push_back(entry->d_name);
    closedir(dir);

    for (string &file_name : list_directory)
    {
        if (file_name != "." && file_name != "..")
        {
            replace(CWD.begin(), CWD.end(), '\\', '/'); // replace all 'x' to 'y'
            list_directory2.push_back(CWD + "/Piano Samples/" + file_name);
        }
    }
    return list_directory2;
}

void combine_audio_files(vector<string> notes, vector<double> note_types, string output_filename)
{
    system(("mkdir \"" + CWD + "/Process\"").c_str());
    int index = 0;
    ofstream list_file;
    list_file.open("Process/list.txt");
    for (string &file_name : notes)
    {
        string s_index = to_string(index);
        string trim_ammount = to_string(note_types[index]);
        list_file << "file '" << s_index + ".mp3'\n";
        system(("ffmpeg -t " + trim_ammount + " -i \"" + file_name + "\" -acodec copy \"" + CWD + "/Process/" + s_index + ".mp3\"").c_str());
        index++;
    }
    list_file.close();
    string command_string = "ffmpeg -f concat -i \"" + CWD + "/Process/list.txt\" -c copy \"" + CWD + "/Music/" + output_filename + "\"";
    system(command_string.c_str());
    if (platform() == "Windows")
        system(("rd /s /q \"" + CWD + "/Process/\"").c_str());
    else if (platform() == "Linux")
        system(("rm -rf \"" + CWD + "/Process/\"").c_str());
}
string get_audio_file_length(string command)
{
    char buffer[128];
    string result = "";
    // Open pipe to file
    FILE *pipe = popen(command.c_str(), "r");
    if (!pipe)
        return "popen failed!";

    // read till end of process:
    while (!feof(pipe))
    {
        // use buffer to read and add to result
        if (fgets(buffer, 128, pipe) != NULL)
            result += buffer;
    }

    pclose(pipe);
    return result;
}
int main()
{
#if _WIN64 || _WIN32
    CWD = getCurrentDir();
#elif __linux__
    CWD = get_current_dir_name();
#endif

    vector<string> list_directory = list_dir("./Piano Samples/");
    vector<string> files_to_compile = {
        list_directory[1],
        list_directory[15],
        list_directory[22],
        list_directory[3],
        list_directory[10],
        list_directory[40],
        list_directory[32],
        list_directory[1],
        list_directory[15],
        list_directory[1],
        list_directory[15],
        list_directory[22],
        list_directory[3],
        list_directory[10],
        list_directory[40],
        list_directory[32],
        list_directory[1],
        list_directory[15],
        list_directory[1],
        list_directory[15],
        list_directory[22],
        list_directory[3],
        list_directory[10],
        list_directory[40],
        list_directory[32],
        list_directory[1],
        list_directory[15],
        list_directory[1],
        list_directory[15],
        list_directory[22],
        list_directory[3],
        list_directory[10],
        list_directory[40],
        list_directory[32],
        list_directory[1],
        list_directory[15],
        list_directory[1],
        list_directory[15],
        list_directory[22],
        list_directory[3],
        list_directory[10],
        list_directory[40],
        list_directory[32],
        list_directory[1],
        list_directory[15],
        list_directory[1],
        list_directory[15],
        list_directory[22],
        list_directory[3],
        list_directory[10],
        list_directory[40],
        list_directory[32],
        list_directory[1],
        list_directory[15],
    };
    vector<double> note_types_value = {
        8,
        16,
        32,
        4,
        2,
        1,
        4,
        16,
        32,
        8,
        16,
        32,
        4,
        2,
        1,
        4,
        16,
        32,
        8,
        16,
        32,
        4,
        2,
        1,
        4,
        16,
        32,
        8,
        16,
        32,
        4,
        2,
        1,
        4,
        16,
        32,
        8,
        16,
        32,
        4,
        2,
        1,
        4,
        16,
        32,
        8,
        16,
        32,
        4,
        2,
        1,
        4,
        16,
        32,
    };
    vector<double> trim_note_audio_values;
    int index = 0;
    for (string &file_name : files_to_compile)
    {
        string length = get_audio_file_length("ffprobe -v error -show_entries format=duration -of default=noprint_wrappers=1:nokey=1 \"" + file_name + "\"");
        double audio_length = ::atof(length.c_str());
        trim_note_audio_values.push_back(audio_length / note_types_value[index]);
        index++;
    }
    combine_audio_files(files_to_compile, trim_note_audio_values, "output1min.mp3");
    return 0;
}

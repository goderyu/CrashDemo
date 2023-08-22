#include "client/crashpad_client.h"
#include "client/crash_report_database.h"
#include "client/settings.h"

#include <filesystem>
#include <iostream>
#include <thread>
#include <chrono>

using namespace std;

vector<int> numbers;

void thread_read() {
    for (int i = 0; i < 10000000; ++i) {
        if (numbers.size() >= 0) {
            cout << "read and pop: " << numbers[numbers.size() - 1] << endl;
            numbers.pop_back();
        }
    }
}


void thread_write() {
    for (int i = 0; i < 10000000; ++i) {
        numbers.emplace_back(i);
    }
}


int main(int argc, char** argv) {
#if OS_LINUX
    string exeDir = std::filesystem::current_path().u8string();
    base::FilePath handler(exeDir + "/crashpad_handler");
    base::FilePath reportsDir(exeDir + "/Crashes");
    base::FilePath metricsDir(exeDir + "/Crashes");
    string url = "";
    map<string, string> annotations;
    annotations["format"] = "minidump";
    annotations["product"] = "CrashDemo";
    annotations["version"] = "1.0.0";
    vector<string> arguments;
    arguments.emplace_back("--no-rate-limit");
    vector<base::FilePath> attachments;
    base::FilePath attachment(exeDir + "/Crashes/attachment.txt");
    attachments.emplace_back(attachment);
#elif OS_WIN
    wstring exeDir = std::filesystem::current_path().wstring();
    base::FilePath handler(exeDir + L"/crashpad_handler.exe");
    base::FilePath reportsDir(exeDir + L"/Crashes");
    base::FilePath metricsDir(exeDir + L"/Crashes");
    string url = "";
    map<string, string> annotations;
    annotations["format"] = "minidump";
    annotations["product"] = "CrashDemo";
    annotations["version"] = "1.0.0";
    vector<string> arguments;
    arguments.emplace_back("--no-rate-limit");
    vector<base::FilePath> attachments;
    base::FilePath attachment(exeDir + L"/Crashes/attachment.txt");
    attachments.emplace_back(attachment);
#endif

    unique_ptr<crashpad::CrashReportDatabase> database =
        crashpad::CrashReportDatabase::Initialize(reportsDir);
    if (database == nullptr) {
        cout << "database is nullptr" << endl;
        int a = 0;
    }
    crashpad::Settings* settings = database->GetSettings();
    settings->SetUploadsEnabled(false);
    crashpad::CrashpadClient* client = new crashpad::CrashpadClient();
#if OS_LINUX
    client->StartHandlerAtCrash(handler, reportsDir, metricsDir, url, annotations, arguments);
#elif OS_WIN
    client->StartHandler(handler, reportsDir, metricsDir, url, annotations, arguments, false, false, attachments);
#endif
    // // test nullptr assign
    cout << "Wait for crash..." << endl;
    this_thread::sleep_for(chrono::seconds(3));
    int* a = (int*)nullptr;
    *a = 1;

    // test multi-thread read and write without lock
    // thread write_thread(thread_write);
    // thread read_thread(thread_read);
    // write_thread.join();
    // read_thread.join();

    this_thread::sleep_for(chrono::seconds(10));
}
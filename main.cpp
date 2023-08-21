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
    string exeDir = std::filesystem::current_path().u8string();
    base::FilePath handler(exeDir + "/crashpad_handler");
    base::FilePath reportsDir(exeDir + "/Crashes");
    base::FilePath metricsDir(exeDir + "/Crashes");
    map<string, string> annotations;
    string url = "https://133077719_qq_com.bugsplat.com/post/bp/crash/crashpad.php";
    annotations["format"] = "minidump";
    annotations["database"] = "133077719_qq_com";
    annotations["product"] = "OpenZIEditorLauncher";
    annotations["version"] = "1.0.0";
    annotations["key"] = "Sample key";
    annotations["user"] = "goderyu@cengzi.com";
    annotations["list_annotations"] = "Sample comment";

    vector<string> arguments;
    arguments.emplace_back("--no-rate-limit");
    vector<base::FilePath> attachments;
    base::FilePath attachment(exeDir + "/Crashes/attachment.txt");
    attachments.emplace_back(attachment);
    unique_ptr<crashpad::CrashReportDatabase> database =
        crashpad::CrashReportDatabase::Initialize(reportsDir);
    if (database == nullptr) {
        cout << "database is nullptr" << endl;
        int a = 0;
    }
    crashpad::Settings* settings = database->GetSettings();
    settings->SetUploadsEnabled(false);
    crashpad::CrashpadClient* client = new crashpad::CrashpadClient();
    client->StartHandlerAtCrash(handler, reportsDir, metricsDir, url, annotations, arguments);
    // // test nullptr assign
    // int* a = (int*)nullptr;
    // *a = 1;

    // test multi-thread read and write without lock
    thread write_thread(thread_write);
    thread read_thread(thread_read);
    write_thread.join();
    read_thread.join();

    this_thread::sleep_for(chrono::seconds(10));
}

#include <iostream>
#include <filesystem>
#include <vector>
#include <string>
#include <algorithm>
#include <fstream>
#include <limits>

namespace fs = std::filesystem;

const std::string ROOT_DIR = fs::current_path().string() + "/InfoSystem_Root";

// 辅助函数：读取用户输入，如果为空则返回 "Unknown"
std::string readField(const std::string& prompt) {
    std::string value;
    std::cout << prompt;
    std::getline(std::cin, value);
    if (value.empty()) {
        return "Unknown";
    }
    return value;
}

// 扫描并获取所有子目录名称
std::vector<std::string> getSubDirectories() {
    std::vector<std::string> dirs;
    // 如果总目录都不存在，直接返回空列表
    if (!fs::exists(ROOT_DIR)) return dirs;

    // 遍历总目录下的所有条目
    for (const auto& entry : fs::directory_iterator(ROOT_DIR)) {
        // 只保留文件夹（排除文件）
        if (entry.is_directory()) {
            dirs.push_back(entry.path().filename().string());
        }
    }
    return dirs;
}

//创建新目录
std::string createDirectory() {
    std::string newDirName;
    std::cout << "Enter new directory name: ";
    std::getline(std::cin, newDirName);

    if (newDirName.empty()) {
        std::cout << "[Error] Directory name cannot be empty." << std::endl;
        return "";
    }

    std::string newPath = ROOT_DIR + "/" + newDirName;

    //防止重名
    if (fs::exists(newPath)) {
        std::cout << "[Error] Directory '" << newDirName << "' already exists." << std::endl;
        return "";
    }

    //创建失败时反馈错误信息
    try {
        fs::create_directory(newPath);
        std::cout << "[Success] Directory '" << newDirName << "' created." << std::endl;
        return newDirName;
    }
    catch (const std::exception& e) {
        std::cout << "[Error] Failed to create directory: " << e.what() << std::endl;
        return "";
    }
}

//重命名目录
std::string renameDirectory(const std::vector<std::string>& dirs) {
    if (dirs.empty()) {
        std::cout << "[Error] No directories available to rename." << std::endl;
        return "";
    }

    int idx;
    std::cout << "Enter the number of the directory to rename: ";
    std::cin >> idx;

    if (idx < 1 || idx > static_cast<int>(dirs.size())) {
        std::cout << "[Error] Invalid number." << std::endl;
        return "";
    }

    std::string oldName = dirs[idx - 1];
    std::string newName;
    std::cout << "Current name: " << oldName << std::endl;
    std::cout << "Enter new name: ";
    std::cin >> newName;

    if (newName.empty()) {
        std::cout << "[Error] New name cannot be empty." << std::endl;
        return "";
    }

    std::string oldPath = ROOT_DIR + "/" + oldName;
    std::string newPath = ROOT_DIR + "/" + newName;

    if (fs::exists(newPath)) {
        std::cout << "[Error] A directory named '" << newName << "' already exists." << std::endl;
        return "";
    }

    try {
        fs::rename(oldPath, newPath);
        std::cout << "[Success] Directory renamed to '" << newName << "'." << std::endl;
        return newName;
    }
    catch (const std::exception& e) {
        std::cout << "[Error] Rename failed: " << e.what() << std::endl;
        return "";
    }
}

// 获取指定目录下所有成员名称
// 成员以 .txt 文件形式存储，文件名即为成员名
std::vector<std::string> getMembers(const std::string& dirName) {
    std::vector<std::string> members;
    std::string dirPath = ROOT_DIR + "/" + dirName;
    if (!fs::exists(dirPath)) return members;

    for (const auto& entry : fs::directory_iterator(dirPath)) {
        if (entry.is_regular_file() && entry.path().extension() == ".txt") {
            members.push_back(entry.path().stem().string());
        }
    }
    return members;
}

// 添加新成员：输入成员名及详细信息（姓名/年龄/电话/邮箱/地址），在对应目录下创建 .txt 文件
// 如果成员已存在或文件创建失败，给出错误提示
// 输入为空时自动填充 "Unknown"
void addMember(const std::string& dirName) {
    // 清除输入缓冲区，避免上一轮 cin >> 残留的换行符影响 getline
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    std::string memberName;
    std::cout << "Enter member name:" << std::endl;
    std::getline(std::cin, memberName);

    if (memberName.empty()) {
        std::cout << "[Error] Name cannot be empty." << std::endl;
        return;
    }

    std::string filePath = ROOT_DIR + "/" + dirName + "/" + memberName + ".txt";
    if (fs::exists(filePath)) {
        std::cout << "[Error] Member '" << memberName << "' already exists." << std::endl;
        return;
    }

    // 使用 readField 读取各字段，空输入自动填 "Unknown"
    std::string age = readField("Enter age: ");
    std::string phone = readField("Enter phone: ");
    std::string email = readField("Enter email: ");
    std::string address = readField("Enter address: ");
    std::string other = readField("Enter other information: ");

    std::ofstream outFile(filePath);
    if (outFile.is_open()) {
        outFile << "Name: " << memberName << std::endl;
        outFile << "Age: " << age << std::endl;
        outFile << "Phone: " << phone << std::endl;
        outFile << "Email: " << email << std::endl;
        outFile << "Address: " << address << std::endl;
		outFile << "Other: " << other << std::endl;
        outFile.close();
        std::cout << "[Success] Member '" << memberName << "' added." << std::endl;
    }
    else {
        std::cout << "[Error] Failed to create member file." << std::endl;
    }
}

// 查看成员详情：列出所有成员，选择后读取并显示其 .txt 文件内容
void viewMember(const std::string& dirName, const std::vector<std::string>& members) {
    if (members.empty()) {
        std::cout << "[Info] No members found." << std::endl;
        return;
    }

    std::cout << "Members:" << std::endl;
    for (int i = 0; i < members.size(); ++i) {
        std::cout << "  " << (i + 1) << ". " << members[i] << std::endl;
    }

    int idx;
    std::cout << "Enter the number to view details: ";
    std::cin >> idx;

    if (idx < 1 || idx > static_cast<int>(members.size())) {
        std::cout << "[Error] Invalid number." << std::endl;
        return;
    }

    std::string filePath = ROOT_DIR + "/" + dirName + "/" + members[idx - 1] + ".txt";
    std::ifstream inFile(filePath);
    if (inFile.is_open()) {
        std::cout << "\n--- Details ---" << std::endl;
        std::string line;
        while (std::getline(inFile, line)) {
            std::cout << line << std::endl;
        }
        std::cout << "---------------" << std::endl;
        inFile.close();
    }
    else {
        std::cout << "[Error] Failed to read member file." << std::endl;
    }
}

// 修改成员信息：列出所有成员，选择后显示文件各行内容
// 用户选择行号并输入新内容，修改后写回文件
void editMember(const std::string& dirName, const std::vector<std::string>& members) {
    if (members.empty()) {
        std::cout << "[Info] No members found." << std::endl;
        return;
    }

    std::cout << "Members:" << std::endl;
    for (size_t i = 0; i < members.size(); ++i) {
        std::cout << "  " << (i + 1) << ". " << members[i] << std::endl;
    }

    int idx;
    std::cout << "Enter the number to edit: ";
    std::cin >> idx;

    if (idx < 1 || idx > static_cast<int>(members.size())) {
        std::cout << "[Error] Invalid number." << std::endl;
        return;
    }

    std::string memberName = members[idx - 1];
    std::string filePath = ROOT_DIR + "/" + dirName + "/" + memberName + ".txt";

    // 读取文件所有行
    std::vector<std::string> lines;
    std::ifstream inFile(filePath);
    if (inFile.is_open()) {
        std::string line;
        while (std::getline(inFile, line)) {
            lines.push_back(line);
        }
        inFile.close();
    }
    else {
        std::cout << "[Error] Failed to read member file." << std::endl;
        return;
    }

    // 显示当前内容供用户选择要修改的行
    std::cout << "Current content:" << std::endl;
    for (size_t i = 0; i < lines.size(); ++i) {
        std::cout << "  [" << (i + 1) << "] " << lines[i] << std::endl;
    }

    int lineIdx;
    std::cout << "Enter the line number to edit (0 to cancel): ";
    std::cin >> lineIdx;

    if (lineIdx == 0) {
        std::cout << "[Info] Edit cancelled." << std::endl;
        return;
    }

    if (lineIdx < 1 || lineIdx > static_cast<int>(lines.size())) {
        std::cout << "[Error] Invalid line number." << std::endl;
        return;
    }

    std::cin.ignore(); // 清除缓冲区，避免影响后续 getline
    std::string newContent;
    std::cout << "Enter new content for line " << lineIdx << ": ";
    std::getline(std::cin, newContent);

    lines[lineIdx - 1] = newContent;

    // 将修改后的内容写回文件
    std::ofstream outFile(filePath);
    if (outFile.is_open()) {
        for (const auto& l : lines) {
            outFile << l << std::endl;
        }
        outFile.close();
        std::cout << "[Success] Member information updated." << std::endl;
    }
    else {
        std::cout << "[Error] Failed to save changes." << std::endl;
    }
}

// 删除成员：列出所有成员，选择后二次确认，确认后删除对应的 .txt 文件
void deleteMember(const std::string& dirName, const std::vector<std::string>& members) {
    if (members.empty()) {
        std::cout << "[Info] No members found." << std::endl;
        return;
    }
    std::cout << "Members:" << std::endl;
    for (size_t i = 0; i < members.size(); ++i) {
        std::cout << "  " << (i + 1) << ". " << members[i] << std::endl;
    }

    int idx;
    std::cout << "Enter the number to delete" << std::endl;
    std::cin >> idx;

    if (idx < 1 || idx > static_cast<int>(members.size())) {
        std::cout << "[Error] Invalid number." << std::endl;
        return;
    }

    std::string memberName = members[idx - 1];
    std::string filePath = ROOT_DIR + "/" + dirName + "/" + memberName + ".txt";

    std::cout << "Are you sure you want to delete '" << memberName << "'? (y/n): ";
    char confirm;
    std::cin >> confirm;

    if (confirm == 'y' || confirm == 'Y') {
        try {
            fs::remove(filePath);
            std::cout << "[Success] Member '" << memberName << "' deleted." << std::endl;
        }
        catch (const std::exception& e) {
            std::cout << "[Error] Delete failed: " << e.what() << std::endl;
        }
    }
    else {
        std::cout << "[Info] Delete cancelled." << std::endl;
    }
}

// 进入指定目录后的成员管理界面
// 循环显示成员列表和可用命令，直到用户输入 back 返回
void memberManagement(const std::string& dirName) {
    while (true) {
        std::vector<std::string> members = getMembers(dirName);

        std::cout << "\n=== Directory: [" << dirName << "] ===" << std::endl;
        if (members.empty()) {
            std::cout << "[Info] No members in this directory." << std::endl;
        }
        else {
            std::cout << "Members (" << members.size() << "):" << std::endl;
            for (size_t i = 0; i < members.size(); ++i) {
                std::cout << "  " << (i + 1) << ". " << members[i] << std::endl;
            }
        }

        std::cout << "\nCommands:" << std::endl;
        std::cout << "  add    - Add a new member" << std::endl;
        std::cout << "  view   - View member details" << std::endl;
        std::cout << "  edit   - Edit member information" << std::endl;
        std::cout << "  del    - Delete a member" << std::endl;
        std::cout << "  back   - Go back to directory selection" << std::endl;
        std::cout << "Your input: ";

        std::string input;
        std::cin >> input;

        if (input == "add") {
            addMember(dirName);
        }
        else if (input == "view") {
            viewMember(dirName, members);
        }
        else if (input == "edit") {
            editMember(dirName, members);
        }
        else if (input == "del") {
            deleteMember(dirName, members);
        }
        else if (input == "back") {
            break;
        }
        else {
            std::cout << "[Error] Unknown command." << std::endl;
        }
    }
}

int main() {
    if (!fs::exists(ROOT_DIR)) {
        fs::create_directories(ROOT_DIR);
        std::cout << "[System] Root directory not found. Automatically created " << ROOT_DIR << std::endl;
    }
    std::vector<std::string> dirs = getSubDirectories();
    if (dirs.empty()) {
        std::cout << "[Info] No subdirectories found. Please create one first." << std::endl;
    }
    else {
        std::cout << "[Info] Found " << dirs.size() << " existing director" << (dirs.size() == 1 ? "y" : "ies") << ":" << std::endl;
        for (size_t i = 0; i < dirs.size(); ++i) {
            std::cout << "  " << (i + 1) << ". " << dirs[i] << std::endl;
        }
    }

    std::string currentDir = "";

    while (currentDir.empty()) {
        std::vector<std::string> dirs = getSubDirectories();

        std::cout << "\nCommands:" << std::endl;
        std::cout << "  create dir  - Create a new directory" << std::endl;
        std::cout << "  rename dir  - Rename an existing directory" << std::endl;
        std::cout << "  <number>    - Enter a directory by its number" << std::endl;

        std::string input;
        std::getline(std::cin, input);

        if (input == "create dir") {
            currentDir = createDirectory();
        }
        else if (input == "rename dir") {
            currentDir = renameDirectory(dirs);
        }
        else {
            // 尝试解析为数字，进入对应目录
            int idx = 0;
            try {
                idx = std::stoi(input);
            }
            catch (...) {
                std::cout << "[Error] Unknown command. Please try again." << std::endl;
                continue;
            }

            if (idx < 1 || idx > static_cast<int>(dirs.size())) {
                std::cout << "[Error] Invalid number." << std::endl;
                continue;
            }
            currentDir = dirs[idx - 1];
        }
    }

    memberManagement(currentDir);

    std::cout << "\n[Info] Back to directory selection. Exiting." << std::endl;
    return 0;
}


/*项目的挑战性：
1.处理不同的报错信息和异常情况
2.文件创建、打开的函数
*/
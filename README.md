# InfoSystem - 个人信息管理系统

一个基于 C++17 文件系统库的命令行个人信息管理系统。通过目录和文件组织数据，支持目录管理、成员增删改查等功能。

## 功能特性

### 目录管理
- **创建目录** — 输入 `create dir` 创建新目录
- **重命名目录** — 输入 `rename dir` 重命名已有目录
- **进入目录** — 输入目录编号进入对应目录

### 成员管理（进入目录后）
- **添加成员** — 输入 `add`，采集姓名、年龄、电话、邮箱、地址五个字段（空输入自动填 Unknown）
- **查看成员** — 输入 `view` 查看成员详细信息
- **修改成员** — 输入 `edit` 按行编辑成员信息
- **删除成员** — 输入 `del` 删除成员（含二次确认）
- **返回上级** — 输入 `back` 返回目录选择界面

### 数据存储
每个成员对应一个 `.txt` 文件，文件名即成员名，文件内容格式如下：

```
Name: Alice
Age: 20
Phone: 1234567890
Email: alice@example.com
Address: Hangzhou
```

## 项目结构

```
InfoSystem_Root/          # 程序运行时自动创建
├── ClassA/               # 用户创建的目录
│   ├── Alice.txt         # 成员文件
│   └── Bob.txt
└── ClassB/
    └── Charlie.txt
```

## 编译与运行

### Linux / macOS (GCC / Clang)

```bash
g++ -std=c++17 -o InfoSystem InfoSystem_Complete.cpp
./InfoSystem
```

### Windows (MSVC)

```powershell
cl /std:c++17 InfoSystem_Complete.cpp
InfoSystem.exe
```

## 使用说明

1. 编译并运行程序
2. 程序会自动在当前目录创建 `InfoSystem_Root` 文件夹
3. 输入 `create dir` 创建第一个目录
4. 输入目录编号进入目录
5. 在目录内使用 `add / view / edit / del / back` 管理成员

## 注意事项

- 需要 C++17 或更高标准编译（GCC 7+ / Clang 7+ / MSVC 2017 15.7+）
- 程序运行目录下会自动生成 `InfoSystem_Root` 文件夹，请勿手动删除
- 同一目录下成员名不能重复
- 目录名不能重复

## 技术栈

| 项目 | 说明 |
|------|------|
| 语言 | C++17 |
| 核心库 | std::filesystem（C++17 标准库） |
| 数据存储 | 纯文本文件（.txt） |

## 项目亮点

- 使用 C++17 `std::filesystem` 进行跨平台文件系统操作
- 完整的异常处理与用户友好的错误提示
- 模块化设计，目录管理与成员管理职责分离
- 空输入自动填充 "Unknown" 的友好交互设计

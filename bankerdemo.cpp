/**
    课题三：编程模拟银行家算法
    设计目的：通过设计和调试银行家算法通用程序，加深对死锁概念和死锁避免方法的了解。 
    设计内容：编制银行家算法程序，并检测所给状态的系统安全性。
*/

#include <bits/stdc++.h>
using namespace std;
#define endl "\n"

// ==============变量声明=================

// 最大资源数
int max_size = 6;
// 进程数量
int process_num;
// 各类资源数
int resourse_num;

// 进程所需要的资源总量
vector<vector<int>> Claim;
// 已经分配给进程的资源数
vector<vector<int>> Allocation;
// 进程还需要的资源
vector<vector<int>> Need;
// 系统剩余的可用资源
vector<int> Available;
// 安全序列，存储进程 id
vector<int> securityList;

// ==============函数声明=================

// 显示菜单
void menu();
// 初始化
bool init();
// 检查系统安全性
bool checkSecurity();
// 进程请求资源
void request();
// 打印
void printTable();
// 重载运算符 ==
bool operator==(vector<int> &Need, int n);
// 重载运算符 <=
bool operator<=(vector<int> &Request, vector<int> &Need);
// 显示安全序列
void printSecurityList();
// 输出一维数组
ostream &operator<<(ostream &out, vector<int> &A);
// 输出二维数组
ostream &operator<<(ostream &out, vector<vector<int>> &A);

// ===============主函数=================

int main() {
    int choose;
    // 初始化系统资源,检查系统安全性
    if (init() && checkSecurity()) {
        cout << "系统初始化成功！" << endl;
        cout << "系统处于安全状态！" << endl << "当前安全序列为: ";
        printSecurityList();
    }
    while (true) {
        cout << endl
             << "================================银行家算法===================="
                "==========="
             << endl
             << endl;
        // 显示菜单
        menu();
        cin >> choose;
        switch (choose) {
        case -1:
            return 0;
        case 0:
            main();
            continue;
        case 1:
            request();
            continue;
        case 2:
            printSecurityList();
            continue;
        case 3:
            printTable();
            continue;
        default:
            cout << "输入有误！请重新输入: ";
            cin >> choose;
            continue;
        }
    }
    return 0;
}

// ==============函数实现=================

// 显示菜单
void menu() {
    cout << "===================================菜单======================="
            "==========="
         << endl;
    cout << "==  1 - 进程发出请求" << endl;
    cout << "==  2 - 显示安全序列" << endl;
    cout << "==  3 - 显示当前资源分配" << endl;
    cout << "==  0 - 结束本轮模拟" << endl;
    cout << "== -1 - 退出程序" << endl << endl;
    cout << "输入你的选择： ";
}

// 初始化，返回初始化详情，true为成功
bool init() {
    // 先将数组清空,主要是防止一次运行的过程中，二次输入数据时出现的数组错乱。
    // clear()清除数据，swap()释放空间
    Claim.clear();
    vector<vector<int>>().swap(Claim);
    Allocation.clear();
    vector<vector<int>>().swap(Allocation);
    Need.clear();
    vector<vector<int>>().swap(Need);
    Available.clear();
    vector<int>().swap(Available);
    securityList.clear();
    vector<int>().swap(securityList);

    // 初始化时输入进程数量个各类资源数
    cout << "请输入进程的数量(1~9): ";
    cin >> process_num;
    while (process_num < 1 || process_num > 9) {
        cout << "请重新输入进程数(1~9): ";
        cin >> process_num;
    }
    cout << "请输入各类资源的数量(1~6): ";
    cin >> resourse_num;
    while (process_num < 1 || process_num > 9) {
        cout << "请重新输入各类资源数(1~9): ";
        cin >> resourse_num;
    }

    int temp;
    // 输入系统剩余的资源数量
    cout << "输入系统剩余的资源数量(Available): " << endl;
    for (int i = 0; i < resourse_num; i++) {
        cin >> temp;
        // 将数据放入数组末尾
        Available.push_back(temp);
    }
    cout << endl;

    // 输入已经分配的资源矩阵
    cout << "输入已经分配的资源矩阵(Allocation): " << endl;
    for (int i = 0; i < process_num; i++) {
        // 插入空行
        Allocation.push_back(vector<int>());

        // 往每一行中插入数据
        for (int j = 0; j < resourse_num; j++) {
            cin >> temp;
            Allocation[i].push_back(temp);
        }
    }
    cout << endl;

    // 输入进程还需要的资源矩阵
    cout << "输入进程还需要的资源矩阵(Need): " << endl;
    for (int i = 0; i < process_num; i++) {
        // 插入空行
        Need.push_back(vector<int>());

        // 往每一行中插入数据
        for (int j = 0; j < resourse_num; j++) {
            cin >> temp;
            Need[i].push_back(temp);
        }
    }
    cout << endl;

    // 计算出 Claim 矩阵
    for (int i = 0; i < process_num; i++) {
        // 插入空行
        Claim.push_back(vector<int>());

        // 往每一行中写入数据
        for (int j = 0; j < resourse_num; j++) {
            temp = Allocation[i][j] + Need[i][j];
            Claim[i].push_back(temp);
        }
    }

    return true;
}

// 检查系统安全性（安全性算法），返回系统安全性，true 为安全
bool checkSecurity() {
    // 保存安全性, true为安全
    bool status = false;

    // 保存向量，在检查安全性时使用
    vector<int> Check(Available);
    // 记录该进程是否分配
    vector<bool> isDistribute(process_num, false);

    // 标记不满足安全状态的进程, 每找到一个安全进程，flag--
    int flag, temp_flag;

    while (true) {
        // 统计isDistribute中所有false的个数
        flag = count(isDistribute.begin(), isDistribute.end(), false);
        // 将flag暂存temp_flag
        temp_flag = flag;

        for (int i = 0; i < process_num; i++) {
            // 可分配，但未分配
            if (Need[i] <= Check && isDistribute[i] == false) {
                for (int j = 0; j < Check.size(); j++) {
                    // 分配过后回收资源，将资源加入Check
                    Check[j] += Allocation[i][j];
                }
                isDistribute[i] = true;
                // 找到一个，flag--
                flag--;
                // 将进程 id 放入安全序列
                securityList.push_back(i);
            }
        }
        // 判断一下flag，以确定系统是否安全
        // 当所有的进程状态均为安全，则系统处于安全状态
        if (flag == 0) {
            status = true;
            break;
        }
        // 反之不安全
        else if (temp_flag == flag) {
            status = false;
            break;
        }
    }
    // 返回安全状态
    return status;
}

// 银行家算法，返回一个状态字符串
string banker(int process_id, vector<int> &req) {
    // 资源分配状态
    string distribute_status;
    // 比较请求大小与所需大小
    // 这里直接比较可能会出现错误了，重载运算符 <=，见末尾功能性函数段
    if (!(req <= Available)) {
        distribute_status = "请求资源大于系统拥有的资源！";
        return distribute_status;
    }
    if (!(req <= Need[process_id])) {
        distribute_status = "请求资源大于所需要的资源！";
        return distribute_status;
    } else if (
        (req <= Available) &&
        checkSecurity()) { //资源请求符合要求，且系统处于安全状态才可以进行资源申请
        // 分配资源
        cout << "=================================分配过程================="
                "===="
                "==========="
             << endl;
        // 直接打印的话，会出错，重载运算符 <<
        cout << "分配前的 Available: " << Available << endl;
        cout << "分配前的 Allocation: " << Allocation;
        cout << "分配前的 Need: " << Need << endl;
        cout << "为进程 P" << process_id << " 分配资源..." << endl << endl;
        for (int i = 0; i < resourse_num; i++) {
            // 将系统可用资源减去请求资源
            // 打印过程
            Available[i] -= req[i];
            cout << "第" << i + 1 << "次分配后 Available: " << Available
                 << endl;
            // 将已经分配资源矩阵加上请求资源

            Allocation[process_id][i] += req[i];
            cout << "第" << i + 1 << "次分配后的 Allocation: " << Allocation;

            // 将还需要资源的矩阵减去请求矩阵
            Need[process_id][i] -= req[i];
            cout << "第" << i + 1 << "次分配后的 Need: " << Need;
            // 资源分配完成后，完成预分配
            cout << endl;
        }
        cout << "暂定分配资源矩阵: " << endl;
        // 打印
        printTable();

        // 预分配完资源过后，检查当前系统的安全性
        if (checkSecurity() && req == Need[process_id]) {
            distribute_status = "安全";
        } else if (req <= Need[process_id]) {
            distribute_status = "进程还需要继续请求资源！";
            return distribute_status;
        } else {
            distribute_status = "不安全";
        }

        // 若安全
        if (distribute_status == "安全") {
            // 如果进程所需资源已经已经完全得到，就回收资源
            // 这里直接比较会出错，重载运算符 ==，见末尾功能性函数段
            cout << endl << "当前系统处于安全状态... " << endl;
            if (Need[process_id] == 0) {
                for (int i = 0; i < process_num; i++) {
                    // 将已分配矩阵的资源还给可用资源矩阵
                    Available[i] += Allocation[process_id][i];
                    // 该进程的已分配资源清零
                    Allocation[process_id][i] -= Allocation[process_id][i];
                }
            }
            // 进程请求未完成，不回收
            // 分配成功！
            distribute_status = "分配成功！";
        }
        // 若不安全，不分配，回退，将进程挂起
        else if (distribute_status == "不安全") {
            for (int i = 0; i < process_num; i++) {
                // 将暂定分配资源归还，做预分配的逆操作
                Available[i] += req[i];
                Allocation[process_id][i] -= req[i];
                Need[process_id][i] += req[i];
            }
            distribute_status =
                "分配失败！此次分配会导致系统处于不安全状态，已挂起！";
        }
    }
    return distribute_status;
}

// 进程请求资源
void request() {
    int process_id, temp;
    vector<int> req;

    cout << "输入请求资源的进程 id: ";
    cin >> process_id;

    cout << "输入请求资源的大小: " << endl;
    for (int i = 0; i < resourse_num; i++) {
        cin >> temp;
        // 将请求量放入
        req.push_back(temp);
    }
    // 银行家算法,返回的是一个表示分配状态的字符串
    cout << banker(process_id, req) << endl;
    // 打印当前资源分配表
    cout << "当前资源分配为：" << endl;
    printTable();
    cout << endl;
}

// ========功能性函数========
// 在写代码过程中发现的问题，需要函数解决的，均放在此处
// 比较是否Request都 >= Need
bool operator<=(vector<int> &req, vector<int> &need) {
    for (int i = 0; i < req.size(); i++) {
        if (req[i] > need[i]) {
            return false;
        }
    }
    return true;
}
// Need的每一项都 = 0,
bool operator==(vector<int> &need, int n) {
    for (auto &e : need) {
        if (e != n) {
            return false;
        }
    }
    return true;
}
// 输出一维数组，返回一个输出流
ostream &operator<<(ostream &out, vector<int> &A) {
    out << "[";
    for (int i = 0; i < A.size(); i++) {
        out << A[i] << ",";
    }
    out << "\b"
        << "]";
    return out;
}
// 输出二维数组，返回一个输出流
ostream &operator<<(ostream &out, vector<vector<int>> &A) {
    out << endl;
    for (int i = 0; i < A.size(); i++) {
        out << "[";
        for (int j = 0; j < A[i].size(); j++) {
            out << A[i][j] << ",";
        }
        out << "\b"
            << "]" << endl; //输出完一行过后换行
    }
    return out;
}
// 显示安全序列
void printSecurityList() {
    cout << "[";
    for (int i = 0; i < process_num; i++) {
        cout << "P" << securityList[i] << ",";
    }
    cout << "\b"
         << "]" << endl
         << endl;
}
// 打印当前表
void printTable() {
    cout << "==========================资源表格=================="
            "========"
         << endl;
    for (int i = 0; i < process_num; i++) {
        cout << "P" << i;
        cout << "  Claim:[";
        for (int j = 0; j < resourse_num; j++) {
            cout << setw(2) << Claim[i][j] << ",";
        }
        cout << "\b"
             << "]";
        cout << "  Allocation:[";
        for (int j = 0; j < resourse_num; j++) {
            cout << setw(2) << Allocation[i][j] << ",";
        }
        cout << "\b"
             << "]";
        cout << "  Need:[";
        for (int j = 0; j < resourse_num; j++) {
            cout << setw(2) << Need[i][j] << ",";
        }
        cout << "\b"
             << "]" << endl;
    }
    cout << "\n当前系统可用资源 Available: " << Available << endl;
}

/*

参考数据

5
3
3 3 2
0 1 0
2 0 0
3 0 2
2 1 1
0 0 2
6 4 3
1 2 0
6 0 0
0 1 1
4 3 1

分配成功
进程 P1 请求
1 2 0

超出系统资源
进程 P1 请求
3 2 1

超出所需资源
进程 P1 请求
3 2 1

分配失败
进程 P0 请求
3 3 2

************

4
4
1 5 2 0
0 0 1 4
1 4 3 2
1 3 5 4
1 0 0 0
0 6 4 2
0 5 1 0
0 0 0 2
0 7 5 0

**************

5
4
3 3 2 4
0 1 0 1
2 0 0 2
3 0 2 3
2 1 1 1
0 0 2 2
7 4 3 1
1 2 2 0
6 0 0 2
0 1 1 3
4 3 1 1

*/
#include "tutor.h" // 引入教程公共接口和标准库

typedef struct Student { // 用 typedef 给结构体起一个可直接使用的类型名
    int id; // 学号
    char name[32]; // 姓名，固定长度字符数组
    double score; // 综合成绩
} Student; // Student 就是 struct Student 的别名

typedef enum Season { // 枚举把一组有名字的整数常量组织在一起
    SPRING = 1, // 春季，显式从 1 开始
    SUMMER, // 夏季，自动变成 2
    AUTUMN, // 秋季，自动变成 3
    WINTER // 冬季，自动变成 4
} Season; // Season 是 enum Season 的别名

typedef enum ValueKind { // 标记联合当前保存的是哪种类型
    VALUE_INT, // 标记为整数
    VALUE_DOUBLE // 标记为浮点数
} ValueKind; // ValueKind 是 enum ValueKind 的别名

typedef union Number { // 联合的所有成员共享同一块存储区
    int integer; // 按 int 解释这块内存
    double real; // 按 double 解释同一块内存
    char bytes[8]; // 按 8 个字节查看同一块内存
} Number; // Number 是 union Number 的别名

typedef struct TaggedValue { // 带标签的联合，避免误读当前未激活的成员
    ValueKind kind; // 标签说明当前有效成员
    union { // 匿名联合直接嵌套在结构体里
        int integer; // 整数分支
        double real; // 浮点数分支
    } value; // 联合成员名为 value
} TaggedValue; // TaggedValue 是结构体类型别名

typedef struct LayoutA { // 第一种成员顺序，用来观察内存对齐填充
    char tag; // 1 字节字符
    int number; // 4 字节整数
    char flag; // 1 字节字符
} LayoutA; // 第一种布局类型结束

typedef struct LayoutB { // 第二种成员顺序，把两个 char 放在一起
    char tag; // 1 字节字符
    char flag; // 再放一个 1 字节字符
    int number; // 最后放 4 字节整数
} LayoutB; // 第二种布局类型结束

typedef struct Permissions { // 用位域把一个整数拆成多个标记位
    unsigned int read : 1; // 只占 1 位的读权限
    unsigned int write : 1; // 只占 1 位的写权限
    unsigned int execute : 1; // 只占 1 位的执行权限
} Permissions; // 权限结构体类型别名

static const char *season_name(Season season); // 函数声明：把枚举值转换为文字
static void print_student(const Student *student); // 函数声明：只读打印一个学生
static void sort_students_by_score(Student students[], int count); // 函数声明：按成绩降序排序
static void print_value(const TaggedValue *value); // 函数声明：按标签安全打印联合内容

static const char *season_name(Season season) { // 把枚举映射为可读文本
    switch (season) { // 根据枚举值分支
        case SPRING: // 春季
            return "春季"; // 返回对应文字
        case SUMMER: // 夏季
            return "夏季"; // 返回对应文字
        case AUTUMN: // 秋季
            return "秋季"; // 返回对应文字
        case WINTER: // 冬季
            return "冬季"; // 返回对应文字
        default: // 非法枚举值
            return "未知季节"; // 返回安全文本
    } // switch 结束
} // season_name 函数结束

static void print_student(const Student *student) { // 通过结构体指针读取字段
    if (student == NULL) { // 防御性检查空指针
        return; // 空指针不打印
    } // 空指针检查结束
    printf("  学号：%d  姓名：%s  成绩：%.1f\n", student->id, student->name, student->score); // 用 -> 访问指针指向结构体的成员
} // print_student 函数结束

static void sort_students_by_score(Student students[], int count) { // 按成绩从高到低排序
    int i = 0; // 外层轮次
    int j = 0; // 内层比较
    Student temp; // 临时保存整个结构体，便于交换
    for (i = 0; i < count - 1; i++) { // 冒泡排序的轮次
        for (j = 0; j < count - 1 - i; j++) { // 比较相邻两个学生
            if (students[j].score < students[j + 1].score) { // 前一个成绩更低就交换
                temp = students[j]; // 复制整个结构体
                students[j] = students[j + 1]; // 把后一个结构体赋给前一个位置
                students[j + 1] = temp; // 把暂存结构体放到后一个位置
            } // 比较结束
        } // 内层循环结束
    } // 外层循环结束
} // sort_students_by_score 函数结束

static void print_value(const TaggedValue *value) { // 根据标签决定读取哪个联合成员
    if (value == NULL) { // 检查空指针
        return; // 空指针不处理
    } // 空指针检查结束
    if (value->kind == VALUE_INT) { // 标签指出当前是整数
        printf("  整数分支：%d\n", value->value.integer); // 安全读取整数成员
    } else if (value->kind == VALUE_DOUBLE) { // 标签指出当前是浮点数
        printf("  浮点分支：%.2f\n", value->value.real); // 安全读取浮点成员
    } else { // 未知标签
        puts("  未知的标签，拒绝读取联合成员。"); // 不读取数据，避免误解
    } // 标签判断结束
} // print_value 函数结束

static void demo_student(void) { // 案例一：结构体数组、排序与指针访问
    Student students[] = { // 初始化结构体数组
        {1001, "林小满", 88.5}, // 第一个学生
        {1002, "陈一鸣", 94.0}, // 第二个学生
        {1003, "周可欣", 76.5}, // 第三个学生
        {1004, "赵子航", 91.0} // 第四个学生
    }; // 学生数组结束
    int count = (int)(sizeof(students) / sizeof(students[0])); // 计算学生人数
    int i = 0; // 循环下标
    double total = 0.0; // 成绩总和
    ui_example("结构体：把学号、姓名、成绩打包"); // 显示案例标题
    puts("\n  排序前："); // 输出提示
    ui_rule('-', 52); // 输出分隔线
    for (i = 0; i < count; i++) { // 遍历所有学生
        print_student(&students[i]); // 传结构体地址，避免复制整个结构体
        total += students[i].score; // 累加成绩
    } // 遍历结束
    sort_students_by_score(students, count); // 按成绩降序排序
    puts("\n  按成绩降序排序后："); // 输出提示
    ui_rule('-', 52); // 输出分隔线
    for (i = 0; i < count; i++) { // 遍历排序结果
        print_student(&students[i]); // 打印每个学生
    } // 遍历结束
    printf("\n  班级平均分：%.2f\n", total / count); // 输出平均分
    printf("  sizeof(Student) = %lu 字节，id 偏移 %lu，name 偏移 %lu，score 偏移 %lu。\n", (unsigned long)sizeof(Student), (unsigned long)offsetof(Student, id), (unsigned long)offsetof(Student, name), (unsigned long)offsetof(Student, score)); // 显示结构体布局信息
    puts("  结构体赋值会复制全部字段；函数传结构体指针则只复制一个地址。"); // 说明值传递与指针传递
} // demo_student 函数结束

static void demo_enum_union(void) { // 案例二：枚举、联合与带标签的变体
    const char *options[] = {"春季", "夏季", "秋季", "冬季"}; // 季节菜单
    int choice = -1; // 菜单下标
    Season season = SPRING; // 保存枚举值
    Number number; // 联合变量，同一时刻只应读最近写入的成员
    TaggedValue int_value = {VALUE_INT, {.integer = 42}}; // 带标签的整数
    TaggedValue double_value = {VALUE_DOUBLE, {.real = 3.14159}}; // 带标签的浮点数
    ui_example("枚举与联合：一个名字固定，一个内存共享"); // 显示案例标题
    choice = ui_choose("请选择一个季节：", options, 4); // 读取季节选择
    if (choice < 0) { // 用户取消
        puts("  已取消输入。"); // 提示取消
        return; // 结束案例
    } // 选择检查结束
    season = (Season)(choice + 1); // 把下标转换为从 1 开始的枚举值
    printf("\n  枚举编号：%d，对应名称：%s\n", (int)season, season_name(season)); // 输出枚举值与文字
    number.integer = 65; // 先写入整数成员
    printf("  联合大小：%lu 字节，当前 integer = %d\n", (unsigned long)sizeof(Number), number.integer); // 输出联合大小和当前成员
    number.real = 2.5; // 再写入浮点成员，会覆盖同一块存储区
    printf("  改写 real 后，real = %.2f；不要期待 integer 还保留旧值。\n", number.real); // 说明联合成员互相覆盖
    puts("\n  带标签联合可以安全区分当前类型："); // 输出提示
    print_value(&int_value); // 打印整数变体
    print_value(&double_value); // 打印浮点变体
    printf("  sizeof(Number) = %lu，sizeof(TaggedValue) = %lu\n", (unsigned long)sizeof(Number), (unsigned long)sizeof(TaggedValue)); // 对比联合与带标签结构体大小
    puts("  规则：联合同一时刻只有一个成员有效；读取前必须知道最近写的是哪个。"); // 总结联合使用规则
} // demo_enum_union 函数结束

static void demo_layout(void) { // 案例三：结构体填充、位域与可移植性
    LayoutA a = {'A', 7, 'x'}; // 初始化第一种布局
    LayoutB b = {'B', 'y', 9}; // 初始化第二种布局
    Permissions permissions = {0}; // 所有权限位先清零
    permissions.read = 1; // 打开读权限
    permissions.write = 1; // 打开写权限
    permissions.execute = 0; // 保持执行权限关闭
    ui_example("内存布局：填充字节与位域"); // 显示案例标题
    printf("\n  sizeof(LayoutA) = %lu 字节（char, int, char）\n", (unsigned long)sizeof(LayoutA)); // 输出第一种布局大小
    printf("  sizeof(LayoutB) = %lu 字节（char, char, int）\n", (unsigned long)sizeof(LayoutB)); // 输出第二种布局大小
    printf("  LayoutA 偏移：tag=%lu，number=%lu，flag=%lu\n", (unsigned long)offsetof(LayoutA, tag), (unsigned long)offsetof(LayoutA, number), (unsigned long)offsetof(LayoutA, flag)); // 显示各字段偏移
    printf("  LayoutB 偏移：tag=%lu，flag=%lu，number=%lu\n", (unsigned long)offsetof(LayoutB, tag), (unsigned long)offsetof(LayoutB, flag), (unsigned long)offsetof(LayoutB, number)); // 显示优化后的偏移
    puts("  编译器会在成员之间插入填充字节，以满足对齐要求；调整顺序可能减少空间。"); // 解释结构体填充
    printf("\n  LayoutA 数据：tag=%c，number=%d，flag=%c\n", a.tag, a.number, a.flag); // 输出实际数据
    printf("  LayoutB 数据：tag=%c，flag=%c，number=%d\n", b.tag, b.flag, b.number); // 输出实际数据
    printf("  Permissions 大小：%lu 字节，read=%u，write=%u，execute=%u\n", (unsigned long)sizeof(Permissions), (unsigned int)permissions.read, (unsigned int)permissions.write, (unsigned int)permissions.execute); // 输出位域组合
    puts("  位域能节省空间，但具体位布局由实现决定，不适合直接映射外部二进制协议。"); // 强调可移植性
    puts("  跨平台序列化时应按字节协议手动打包，而不是直接 fwrite 结构体。"); // 给出工程建议
} // demo_layout 函数结束

void lesson_09_structs(void) { // 第 9 章教学入口
    int correct = 0; // 记录小测答对题数
    const char *homework[] = { // 课后作业文本
        "定义 Date 和 Student 结构体，实现按成绩排序、按学号查找、打印成绩单。", // 作业一
        "用枚举实现星期/月份转换，并用 switch 输出对应中文名称。", // 作业二
        "实现带标签的 Value 联合，支持 int、double 和字符串三种类型。", // 作业三
        "用 offsetof 打印不同成员顺序的结构体布局，重构以减少填充字节。", // 作业四
        "用位域实现权限集合，增加切换、检查和打印权限的函数。" // 作业五
    }; // 作业数组结束
    ui_heading("第 9 章  结构体、联合、枚举与 typedef"); // 显示章节标题
    ui_section("本章目标"); // 显示目标小节
    ui_note("掌握结构体的定义、初始化、成员访问、数组和指针。"); // 列出目标一
    ui_note("理解值传递与地址传递在结构体场景下的成本差异。"); // 列出目标二
    ui_note("掌握枚举、联合、带标签联合和 typedef 的使用。"); // 列出目标三
    ui_note("理解对齐、填充、位域及其可移植性限制。"); // 列出目标四
    ui_section("知识讲解"); // 开始知识讲解
    puts("  1. 结构体：把相关数据打包"); // 讲解第一点标题
    puts("     struct 定义一种复合类型；成员通过 . 访问，结构体指针通过 -> 访问。"); // 说明访问语法
    puts("     结构体可以整体赋值、作为函数参数、作为返回值，也可以组成数组。"); // 说明结构体能力
    puts("     传结构体值会复制所有字段；大结构体优先传 const 指针，既快又只读。"); // 说明传参选择
    puts("  2. typedef 与命名"); // 讲解第二点标题
    puts("     typedef 为已有类型创建别名，不创建新类型；能减少 struct 关键字的重复。"); // 解释 typedef
    puts("     命名建议：类型名用 Student，变量名用 student，避免同名混淆。"); // 给出命名约定
    puts("  3. 枚举与联合"); // 讲解第三点标题
    puts("     enum 用有意义的名字代替魔法整数，底层通常仍是整数类型。"); // 说明枚举
    puts("     union 的所有成员共享一块内存，大小至少能容纳最大成员。"); // 说明联合
    puts("     联合必须配合标签使用，否则无法判断当前哪个成员有效，容易误读。"); // 说明带标签联合
    puts("  4. 对齐、填充与位域"); // 讲解第四点标题
    puts("     编译器按成员对齐要求插入填充；结构体大小可能大于成员大小之和。"); // 说明填充
    puts("     位域用 unsigned x : n 指定 n 个二进制位，但布局依赖实现，不适合协议解析。"); // 说明位域
    puts("     需要跨平台传输时，应按明确的字节序和字段宽度手动序列化。"); // 给出工程建议
    ui_pause(); // 等待阅读
    ui_section("运行案例"); // 开始案例
    demo_student(); // 运行学生结构体案例
    ui_pause(); // 暂停阅读
    demo_enum_union(); // 运行枚举联合案例
    ui_pause(); // 暂停阅读
    demo_layout(); // 运行内存布局案例
    ui_section("本章小测"); // 开始小测
    if (ui_quiz("已有 Student *p，访问其 score 成员应使用？", "p.score", "p->score", "*p.score", "p[score]", 'B', "p 是指针，-> 等价于 (*p).score；. 用于结构体对象本身。")) { // 第一题
        correct++; // 答对计数加一
    } // 第一题结束
    if (ui_quiz("union 的所有成员有什么特点？", "各自独立存储", "共享同一块存储区", "必须是同一种类型", "不能包含数组", 'B', "联合的所有成员起始地址相同，写入一个成员会覆盖其他成员的存储。")) { // 第二题
        correct++; // 答对计数加一
    } // 第二题结束
    if (ui_quiz("为什么不能直接把结构体的二进制内容发到另一台机器？", "结构体不能写入文件", "可能有填充字节、字节序和宽度差异", "结构体只能存整数", "fwrite 只能写文本", 'B', "结构体布局依赖实现，跨平台交换数据应定义明确的字节级协议。")) { // 第三题
        correct++; // 答对计数加一
    } // 第三题结束
    ui_homework(homework, (int)(sizeof(homework) / sizeof(homework[0]))); // 显示作业
    ui_complete(9, correct, 3); // 显示完成信息和小测成绩
} // lesson_09_structs 函数结束

// ============================================================================
// 第 9 章课后作业（先设计类型，再设计函数，最后写输入输出）
// 1. 成绩单：定义 Date{year,month,day} 和 Student{id,name,birthday,score}。
//    实现按成绩降序排序、按学号查找、输出格式化成绩单，全部使用 const 指针读取。
// 2. 日历小工具：用 enum 表示星期和月份，输入数字输出中文名称；输入月份输出天数。
//    要求：用 switch 处理，闰年 2 月 29 天；非法数字要给提示。
// 3. 通用值：实现 TaggedValue 支持 int、double、字符串三种类型，按标签打印。
//    进阶：写 value_add(a, b)，只有两个都是数值时才相加，否则返回错误码。
// 4. 内存布局：定义至少 3 种成员顺序的结构体，打印 sizeof 和每个 offsetof。
//    尝试调整成员顺序使某一个结构体最小，并解释填充字节从何而来。
// 5. 权限位域：实现 read/write/execute 位域，提供 grant、revoke、check、print 函数。
//    进阶：改用 unsigned int 掩码实现同一功能，比较两种方案的大小与可移植性。
//
// 提交标准：
// - 用 typedef 命名类型，不使用难以阅读的匿名嵌套结构。
// - 结构体作为函数参数时，明确选择值传递还是 const 指针，并说明理由。
// - 能解释一次结构体赋值复制了哪些字节，联合为什么不能同时保存两个有效成员。
// ============================================================================

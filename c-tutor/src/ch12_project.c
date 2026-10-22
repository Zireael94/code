#include "tutor.h" // 引入教程公共接口和标准库

#define DATA_FILE "students.csv" // 学生数据保存在当前目录的 CSV 文件
#define SUBJECT_COUNT 3 // 每个学生有语文、数学、英语三门课

typedef struct Student { // 学生记录：身份信息 + 三门成绩 + 派生统计
    int id; // 学号，作为唯一标识
    char name[32]; // 姓名，最多保存 31 个字节
    double scores[SUBJECT_COUNT]; // 三门课成绩，按固定顺序保存
    double total; // 总分，由成绩计算得到
    double average; // 平均分，由总分计算得到
} Student; // Student 类型别名

typedef struct StudentDatabase { // 固定容量数据库，后续作业可改成动态数组
    Student students[MAX_STUDENTS]; // 学生数组，容量由头文件常量控制
    int count; // 当前有效学生数量
    bool dirty; // 是否存在尚未写入文件的修改
} StudentDatabase; // StudentDatabase 类型别名

static void update_totals(Student *student); // 函数声明：重算总分与平均分
static int find_student_index(const StudentDatabase *database, int id); // 函数声明：按学号查找下标
static void print_table_header(void); // 函数声明：打印成绩表表头
static void print_student_row(const Student *student); // 函数声明：打印一行学生数据
static bool read_student_name(char *buffer, size_t size); // 函数声明：读取合法姓名
static bool input_scores(Student *student); // 函数声明：录入三门成绩
static bool add_student(StudentDatabase *database); // 函数声明：添加学生
static bool edit_student(StudentDatabase *database); // 函数声明：修改学生
static bool delete_student(StudentDatabase *database); // 函数声明：删除学生
static void search_student(const StudentDatabase *database); // 函数声明：按学号查找
static void list_students(const StudentDatabase *database); // 函数声明：列出全部学生
static int compare_by_total_desc(const void *left, const void *right); // 函数声明：qsort 比较函数
static void sort_students(StudentDatabase *database); // 函数声明：按总分降序排序
static void show_statistics(const StudentDatabase *database); // 函数声明：输出统计报告
static bool load_database(StudentDatabase *database); // 函数声明：从 CSV 加载
static bool save_database(StudentDatabase *database); // 函数声明：保存到 CSV
static void run_management_menu(StudentDatabase *database); // 函数声明：运行管理菜单

static void update_totals(Student *student) { // 集中计算派生字段，避免各处重复公式
    int i = 0; // 科目下标
    student->total = 0.0; // 总分从 0 开始
    for (i = 0; i < SUBJECT_COUNT; i++) { // 累加所有科目
        student->total += student->scores[i]; // 把当前科目加入总分
    } // 累加结束
    student->average = student->total / SUBJECT_COUNT; // 计算平均分
} // update_totals 函数结束

static int find_student_index(const StudentDatabase *database, int id) { // 线性查找学号
    int i = 0; // 数组下标
    for (i = 0; i < database->count; i++) { // 遍历有效学生
        if (database->students[i].id == id) { // 找到相同学号
            return i; // 返回下标
        } // 比较结束
    } // 遍历结束
    return -1; // 未找到
} // find_student_index 函数结束

static void print_table_header(void) { // 输出成绩明细区域标题
    puts("  ── 学生成绩明细 ──"); // 输出提示标题
    ui_rule('-', 68); // 输出分隔线
} // print_table_header 函数结束

static void print_student_row(const Student *student) { // 输出一个学生的完整记录
    printf("  学号：%d  姓名：%s  语文：%.1f  数学：%.1f  英语：%.1f  总分：%.1f  平均：%.2f\n", student->id, student->name, student->scores[0], student->scores[1], student->scores[2], student->total, student->average); // 用带字段名的单行格式输出，避免中文宽度导致表格错位
} // print_student_row 函数结束

static bool read_student_name(char *buffer, size_t size) { // 读取姓名并检查文件格式限制
    if (!ui_read_text("请输入姓名（不能包含英文逗号）：", buffer, size)) { // 读取一行非空文本
        puts("  已取消输入。"); // 提示取消
        return false; // 返回失败
    } // 读取结束
    if (strchr(buffer, ',') != NULL) { // CSV 用逗号分隔字段
        puts("  姓名不能包含英文逗号，否则会破坏 CSV 结构。"); // 说明限制
        return false; // 返回失败
    } // 逗号检查结束
    return true; // 姓名合法
} // read_student_name 函数结束

static bool input_scores(Student *student) { // 录入并校验三门成绩
    double chinese = 0.0; // 语文成绩
    double math = 0.0; // 数学成绩
    double english = 0.0; // 英语成绩
    chinese = ui_read_double("请输入语文成绩（0-100）：", 0.0, 100.0); // 读取语文成绩
    if (isnan(chinese)) { // 输入结束
        return false; // 返回失败
    } // 语文检查结束
    math = ui_read_double("请输入数学成绩（0-100）：", 0.0, 100.0); // 读取数学成绩
    if (isnan(math)) { // 输入结束
        return false; // 返回失败
    } // 数学检查结束
    english = ui_read_double("请输入英语成绩（0-100）：", 0.0, 100.0); // 读取英语成绩
    if (isnan(english)) { // 输入结束
        return false; // 返回失败
    } // 英语检查结束
    student->scores[0] = chinese; // 写入语文
    student->scores[1] = math; // 写入数学
    student->scores[2] = english; // 写入英语
    return true; // 三门成绩都有效
} // input_scores 函数结束

static bool add_student(StudentDatabase *database) { // 添加一条新记录
    Student student = {0}; // 用零初始化所有字段
    int id = 0; // 学号
    if (database->count >= MAX_STUDENTS) { // 检查容量
        printf("  数据库已满（最多 %d 人），请先删除记录或扩展容量。\n", MAX_STUDENTS); // 说明容量限制
        return false; // 添加失败
    } // 容量检查结束
    id = ui_read_int("请输入学号（1-999999）：", 1, 999999); // 读取学号
    if (id < 1) { // 输入结束
        puts("  已取消添加。"); // 提示取消
        return false; // 添加失败
    } // 学号检查结束
    if (find_student_index(database, id) >= 0) { // 学号必须唯一
        puts("  该学号已存在，添加失败。"); // 提示重复
        return false; // 添加失败
    } // 重复检查结束
    if (!read_student_name(student.name, sizeof(student.name))) { // 读取姓名
        return false; // 姓名失败则取消
    } // 姓名读取结束
    if (!input_scores(&student)) { // 录入成绩
        puts("  成绩录入未完成，已取消添加。"); // 说明取消
        return false; // 添加失败
    } // 成绩录入结束
    student.id = id; // 保存学号
    update_totals(&student); // 计算总分和平均分
    database->students[database->count] = student; // 放到数组尾部
    database->count++; // 有效数量加一
    database->dirty = true; // 标记有未保存修改
    printf("  已添加：%s（学号 %d），总分 %.1f。\n", student.name, student.id, student.total); // 反馈添加结果
    return true; // 添加成功
} // add_student 函数结束

static bool edit_student(StudentDatabase *database) { // 修改已有学生，先改副本再提交
    int id = 0; // 目标学号
    int index = -1; // 目标下标
    Student copy; // 修改副本
    char new_name[32] = {0}; // 新姓名缓冲区
    if (database->count == 0) { // 空数据库无法修改
        puts("  当前没有学生记录，请先添加。"); // 提示
        return false; // 返回失败
    } // 空库检查结束
    id = ui_read_int("请输入要修改的学号：", 1, 999999); // 读取学号
    if (id < 1) { // 输入结束
        return false; // 取消
    } // 学号检查结束
    index = find_student_index(database, id); // 查找学生
    if (index < 0) { // 不存在
        printf("  未找到学号 %d。\n", id); // 提示
        return false; // 返回失败
    } // 查找结束
    copy = database->students[index]; // 先复制一份，输入失败时不破坏原记录
    puts("\n  当前记录："); // 输出提示
    print_table_header(); // 输出表头
    print_student_row(&copy); // 输出原记录
    if (ui_confirm("是否修改姓名？")) { // 询问是否改姓名
        if (!read_student_name(new_name, sizeof(new_name))) { // 读取新姓名
            return false; // 取消修改
        } // 姓名读取结束
        snprintf(copy.name, sizeof(copy.name), "%s", new_name); // 安全复制新姓名
    } // 姓名修改结束
    if (!input_scores(&copy)) { // 录入新成绩
        puts("  成绩录入未完成，原记录保持不变。"); // 说明原子性
        return false; // 取消修改
    } // 成绩录入结束
    update_totals(&copy); // 重算派生字段
    database->students[index] = copy; // 用完整副本覆盖原记录
    database->dirty = true; // 标记未保存
    printf("  学号 %d 的成绩已更新。\n", id); // 反馈成功
    return true; // 修改成功
} // edit_student 函数结束

static bool delete_student(StudentDatabase *database) { // 按学号删除学生
    int id = 0; // 目标学号
    int index = -1; // 目标下标
    int i = 0; // 移动元素的下标
    id = ui_read_int("请输入要删除的学号：", 1, 999999); // 读取学号
    if (id < 1) { // 输入结束
        return false; // 取消
    } // 学号检查结束
    index = find_student_index(database, id); // 查找学生
    if (index < 0) { // 未找到
        printf("  未找到学号 %d，未删除。\n", id); // 提示
        return false; // 返回失败
    } // 查找结束
    printf("  将删除：%s（学号 %d），请确认。\n", database->students[index].name, id); // 显示待删除记录
    if (!ui_confirm("确定删除吗？此操作不可撤销。")) { // 删除需要确认
        puts("  已取消删除。"); // 提示取消
        return false; // 返回失败
    } // 确认结束
    for (i = index; i < database->count - 1; i++) { // 从删除位置开始把后面元素前移
        database->students[i] = database->students[i + 1]; // 用后一个覆盖前一个
    } // 移动循环结束
    database->count--; // 有效数量减一
    memset(&database->students[database->count], 0, sizeof(Student)); // 清除原先最后一个位置的残留数据
    database->dirty = true; // 标记未保存
    printf("  已删除学号 %d。\n", id); // 反馈删除结果
    return true; // 删除成功
} // delete_student 函数结束

static void search_student(const StudentDatabase *database) { // 按学号查找并输出
    int id = 0; // 目标学号
    int index = -1; // 查找结果
    id = ui_read_int("请输入要查找的学号：", 1, 999999); // 读取学号
    if (id < 1) { // 输入结束
        return; // 取消
    } // 学号检查结束
    index = find_student_index(database, id); // 执行查找
    if (index < 0) { // 未找到
        printf("  未找到学号 %d。\n", id); // 输出未找到
        return; // 结束
    } // 查找结束
    puts("\n  查找结果："); // 输出提示
    print_table_header(); // 表头
    print_student_row(&database->students[index]); // 输出目标学生
} // search_student 函数结束

static void list_students(const StudentDatabase *database) { // 按当前顺序列出全部学生
    int i = 0; // 数组下标
    if (database->count == 0) { // 空数据库
        puts("  当前没有学生记录。"); // 提示
        return; // 结束
    } // 空库检查结束
    print_table_header(); // 输出表头
    for (i = 0; i < database->count; i++) { // 遍历所有学生
        print_student_row(&database->students[i]); // 输出当前学生
    } // 遍历结束
    printf("\n  共 %d 名学生。\n", database->count); // 输出总人数
} // list_students 函数结束

static int compare_by_total_desc(const void *left, const void *right) { // qsort 使用的降序比较函数
    const Student *left_student = (const Student *)left; // 把通用指针转换为学生指针
    const Student *right_student = (const Student *)right; // 转换第二个学生指针
    if (left_student->total < right_student->total) { // 左边总分更小
        return 1; // 返回正数表示左边应排在后面
    } // 较小判断结束
    if (left_student->total > right_student->total) { // 左边总分更大
        return -1; // 返回负数表示左边应排在前面
    } // 较大判断结束
    return 0; // 总分相同，保持等价
} // compare_by_total_desc 函数结束

static void sort_students(StudentDatabase *database) { // 按总分从高到低排序
    if (database->count < 2) { // 少于两个元素无需排序
        puts("  记录不足两条，无需排序。"); // 提示
        return; // 结束
    } // 数量检查结束
    qsort(database->students, (size_t)database->count, sizeof(Student), compare_by_total_desc); // 调用标准快速排序
    database->dirty = true; // 顺序改变，标记需要保存
    puts("  已按总分降序排序。"); // 反馈结果
    list_students(database); // 立即显示排序结果
} // sort_students 函数结束

static void show_statistics(const StudentDatabase *database) { // 输出班级统计报告
    int i = 0; // 遍历下标
    int best_index = 0; // 最高分学生下标
    int worst_index = 0; // 最低分学生下标
    int pass_count = 0; // 及格人数
    int excellent_count = 0; // 优秀人数（平均分 >= 90）
    double class_total = 0.0; // 班级平均分累加器
    if (database->count == 0) { // 空数据库
        puts("  当前没有学生记录，无法统计。"); // 提示
        return; // 结束
    } // 空库检查结束
    for (i = 0; i < database->count; i++) { // 遍历所有学生
        class_total += database->students[i].average; // 累加个人平均分
        if (database->students[i].average >= 60.0) { // 判断及格
            pass_count++; // 及格人数加一
        } // 及格判断结束
        if (database->students[i].average >= 90.0) { // 判断优秀
            excellent_count++; // 优秀人数加一
        } // 优秀判断结束
        if (database->students[i].total > database->students[best_index].total) { // 发现更高总分
            best_index = i; // 更新最高分下标
        } // 最高分判断结束
        if (database->students[i].total < database->students[worst_index].total) { // 发现更低总分
            worst_index = i; // 更新最低分下标
        } // 最低分判断结束
    } // 统计循环结束
    puts("\n  班级统计报告"); // 输出标题
    ui_rule('=', 52); // 输出分隔线
    printf("  学生人数：%d\n", database->count); // 输出人数
    printf("  班级平均分：%.2f\n", class_total / database->count); // 输出班级平均
    printf("  最高总分：%s（%.1f）\n", database->students[best_index].name, database->students[best_index].total); // 输出最高总分
    printf("  最低总分：%s（%.1f）\n", database->students[worst_index].name, database->students[worst_index].total); // 输出最低总分
    printf("  及格人数：%d，及格率：%.1f%%\n", pass_count, 100.0 * pass_count / database->count); // 输出及格统计
    printf("  优秀人数：%d，优秀率：%.1f%%\n", excellent_count, 100.0 * excellent_count / database->count); // 输出优秀统计
    ui_rule('=', 52); // 输出底部分隔线
} // show_statistics 函数结束

static bool load_database(StudentDatabase *database) { // 从 CSV 文件加载全部记录
    FILE *file = NULL; // 文件指针
    char line[256] = {0}; // 保存一行文本
    int loaded = 0; // 成功加载的记录数
    int skipped = 0; // 跳过的无效行数
    StudentDatabase temp; // 先加载到临时数据库，失败时不破坏当前数据
    memset(&temp, 0, sizeof(temp)); // 清空临时数据库
    file = fopen(DATA_FILE, "r"); // 以只读模式打开 CSV
    if (file == NULL) { // 文件可能不存在
        if (errno == ENOENT) { // 首次运行没有数据文件是正常情况
            puts("  没有找到 students.csv，当前使用空数据库。"); // 友好提示
        } else { // 其他打开错误
            printf("  无法打开 %s：%s\n", DATA_FILE, strerror(errno)); // 输出原因
        } // 错误类型判断结束
        return false; // 加载失败
    } // 打开检查结束
    while (fgets(line, sizeof(line), file) != NULL) { // 逐行读取
        int id = 0; // 临时学号
        char name[32] = {0}; // 临时姓名
        double chinese = 0.0; // 临时语文成绩
        double math = 0.0; // 临时数学成绩
        double english = 0.0; // 临时英语成绩
        Student student = {0}; // 临时学生记录
        if (line[0] == '#' || line[0] == '\n' || line[0] == '\r') { // 跳过表头注释和空行
            continue; // 读取下一行
        } // 跳过判断结束
        if (sscanf(line, "%d,%31[^,],%lf,%lf,%lf", &id, name, &chinese, &math, &english) != 5) { // 解析五个字段
            skipped++; // 无效行计数加一
            continue; // 继续下一行
        } // 解析检查结束
        if (id <= 0 || chinese < 0.0 || chinese > 100.0 || math < 0.0 || math > 100.0 || english < 0.0 || english > 100.0) { // 校验字段范围
            skipped++; // 无效数据计数加一
            continue; // 跳过这一行
        } // 范围检查结束
        if (temp.count >= MAX_STUDENTS) { // 超过容量
            skipped++; // 把超出的记录计入跳过
            continue; // 不再加载
        } // 容量检查结束
        if (find_student_index(&temp, id) >= 0) { // 重复学号
            skipped++; // 计为无效行
            continue; // 跳过重复记录
        } // 重复检查结束
        student.id = id; // 写入学号
        snprintf(student.name, sizeof(student.name), "%s", name); // 安全复制姓名
        student.scores[0] = chinese; // 写入语文
        student.scores[1] = math; // 写入数学
        student.scores[2] = english; // 写入英语
        update_totals(&student); // 重算总分和平均
        temp.students[temp.count] = student; // 放入临时数据库
        temp.count++; // 计数加一
        loaded++; // 成功加载数加一
    } // 读取循环结束
    if (ferror(file)) { // 检查读取错误
        perror("  读取 students.csv 时出错"); // 输出错误
        fclose(file); // 关闭文件
        return false; // 加载失败
    } // 读取错误检查结束
    fclose(file); // 关闭文件
    temp.dirty = false; // 刚从文件加载，没有未保存修改
    *database = temp; // 用临时数据库覆盖当前数据
    printf("  已从 %s 加载 %d 条记录", DATA_FILE, loaded); // 输出加载结果
    if (skipped > 0) { // 有无效行时补充说明
        printf("，跳过 %d 条无效或重复记录", skipped); // 输出跳过数量
    } // 跳过说明结束
    putchar('\n'); // 换行
    return true; // 加载成功
} // load_database 函数结束

static bool save_database(StudentDatabase *database) { // 把当前数据写入 CSV
    FILE *file = NULL; // 文件指针
    int i = 0; // 学生下标
    file = fopen(DATA_FILE, "w"); // 以覆盖写模式保存
    if (file == NULL) { // 无法创建文件
        printf("  保存失败：%s\n", strerror(errno)); // 输出错误原因
        return false; // 返回失败
    } // 打开检查结束
    fprintf(file, "# id,name,chinese,math,english\n"); // 写入可读表头
    for (i = 0; i < database->count; i++) { // 遍历所有学生
        if (fprintf(file, "%d,%s,%.1f,%.1f,%.1f\n", database->students[i].id, database->students[i].name, database->students[i].scores[0], database->students[i].scores[1], database->students[i].scores[2]) < 0) { // 写入一条记录并检查
            perror("  写入记录失败"); // 输出错误
            fclose(file); // 关闭文件
            return false; // 保存失败
        } // 写入检查结束
    } // 遍历结束
    if (fclose(file) != 0) { // 关闭时刷新缓冲区，必须检查
        printf("  关闭文件失败：%s\n", strerror(errno)); // 输出错误
        return false; // 保存失败
    } // 关闭检查结束
    database->dirty = false; // 保存成功，清除未保存标记
    printf("  已保存 %d 条记录到 %s。\n", database->count, DATA_FILE); // 反馈结果
    return true; // 保存成功
} // save_database 函数结束

static void run_management_menu(StudentDatabase *database) { // 运行增删改查主循环
    const char *options[] = { // 主菜单选项
        "查看全部学生", // 选项一
        "添加学生", // 选项二
        "按学号查找", // 选项三
        "修改学生成绩", // 选项四
        "删除学生", // 选项五
        "按总分降序排序", // 选项六
        "查看班级统计", // 选项七
        "保存到 students.csv", // 选项八
        "从 students.csv 重新加载" // 选项九
    }; // 菜单数组结束
    int choice = -1; // 菜单选择
    while (true) { // 循环直到用户返回
        choice = ui_choose("\n学生成绩管理系统（固定容量版）", options, 9); // 显示菜单
        if (choice < 0) { // 选择 0 或输入结束
            break; // 退出管理菜单
        } // 退出判断结束
        switch (choice) { // 分发菜单操作
            case 0: // 查看全部
                list_students(database); // 调用列表函数
                break; // 结束分支
            case 1: // 添加学生
                add_student(database); // 调用添加函数
                break; // 结束分支
            case 2: // 查找学生
                search_student(database); // 调用查找函数
                break; // 结束分支
            case 3: // 修改学生
                edit_student(database); // 调用修改函数
                break; // 结束分支
            case 4: // 删除学生
                delete_student(database); // 调用删除函数
                break; // 结束分支
            case 5: // 排序
                sort_students(database); // 调用排序函数
                break; // 结束分支
            case 6: // 统计
                show_statistics(database); // 调用统计函数
                break; // 结束分支
            case 7: // 保存
                save_database(database); // 调用保存函数
                break; // 结束分支
            case 8: // 重新加载
                if (database->dirty && !ui_confirm("重新加载会丢失未保存修改，继续吗？")) { // 先确认
                    puts("  已取消重新加载。"); // 提示取消
                } else { // 用户确认
                    load_database(database); // 从文件重新加载
                } // 加载分支结束
                break; // 结束分支
            default: // 理论上不可达
                puts("  未知操作。"); // 防御性提示
                break; // 结束分支
        } // switch 结束
    } // 菜单循环结束
    if (database->dirty) { // 退出时处理未保存修改
        puts("\n  注意：数据库还有未保存的修改。"); // 提醒
        if (ui_confirm("退出前保存到 students.csv 吗？")) { // 询问保存
            save_database(database); // 执行保存
        } else { // 用户不保存
            puts("  修改未保存，程序返回后这些修改将丢失。"); // 说明后果
        } // 保存确认结束
    } // 未保存处理结束
} // run_management_menu 函数结束

void lesson_12_project(void) { // 第 12 章教学入口
    int correct = 0; // 记录项目设计题答对数
    StudentDatabase database; // 学生数据库对象
    const char *homework[] = { // 项目扩展作业
        "把固定数组改成动态数组：容量满时 realloc 翻倍，并保持所有功能通过测试。", // 作业一
        "增加按姓名模糊查找、按科目排序、修改单科成绩，并补充边界测试。", // 作业二
        "增加导入/导出功能：支持 CSV 与二进制两种格式，并处理格式错误。", // 作业三
        "为项目补一份测试清单，至少覆盖 20 个正常、边界和错误场景。", // 作业四
        "拆分模块：student.h/student.c/database.h/database.c/main.c，并写 Makefile。" // 作业五
    }; // 作业数组结束
    memset(&database, 0, sizeof(database)); // 初始化数据库为空
    ui_heading("第 12 章  综合实战：学生成绩管理系统"); // 显示章节标题
    ui_section("项目需求"); // 显示需求小节
    ui_note("支持添加、查看、查找、修改、删除、排序和统计学生成绩。"); // 需求一
    ui_note("学号必须唯一；成绩限制在 0-100；输入错误不能破坏已有数据。"); // 需求二
    ui_note("数据保存到 CSV，下次启动可以重新加载；退出前提醒保存。"); // 需求三
    ui_note("所有操作都有反馈，删除等危险操作必须二次确认。"); // 需求四
    ui_section("设计思路"); // 显示设计小节
    puts("  1. 数据模型"); // 讲解第一点
    puts("     Student 保存身份、三科成绩和派生统计；StudentDatabase 保存数组、数量和脏标记。"); // 说明结构
    puts("     total 与 average 不独立存储，而是由 update_totals 统一重算，避免不一致。"); // 说明派生字段
    puts("  2. 分层职责"); // 讲解第二点
    puts("     输入输出、数据操作、文件持久化分开写；菜单只负责分发，不写业务细节。"); // 说明分层
    puts("     每个函数尽量只做一件事，失败时返回 bool 或 -1，并让调用者决定下一步。"); // 说明错误传递
    puts("  3. 稳健性"); // 讲解第三点
    puts("     先校验再写入；修改时先改副本；删除时移动数组并减少 count；退出时统一释放资源。"); // 说明安全流程
    puts("     加载失败不能清空当前数据；保存失败不能清除 dirty 标记。"); // 说明事务意识
    puts("  4. 测试路径"); // 讲解第四点
    puts("     至少测试：空库、重复学号、非法成绩、删除头/尾/中间、文件不存在、文件损坏。"); // 列出测试场景
    puts("     真实项目还应拆分模块、编写自动化测试，并用调试器检查失败路径。"); // 说明工程升级方向
    ui_pause(); // 请用户阅读完设计思路后再进入小测
    ui_section("项目设计小测"); // 开始小测
    if (ui_quiz("修改学生成绩时，为什么建议先修改一份副本？", "提高排序速度", "输入中途失败时原记录不会被破坏", "减少文件数量", "让代码更长", 'B', "先把所有新值收齐再一次性提交，可以避免一半字段已改、一半字段没改的中间状态。")) { // 第一题
        correct++; // 答对计数加一
    } // 第一题结束
    if (ui_quiz("删除数组中间的学生后，必须做哪一步？", "只把 count 减一", "把后面的元素整体前移，再减少 count", "清空整个数组", "重新启动程序", 'B', "数组必须保持前 count 个元素连续有效；只减 count 会留下空洞，导致数据顺序错误。")) { // 第二题
        correct++; // 答对计数加一
    } // 第二题结束
    ui_section("开始操作"); // 开始项目操作
    puts("  提示：第一次使用可以先添加 2-3 名学生，再依次体验查找、修改、排序、统计和保存。"); // 给出体验建议
    puts("  文件 students.csv 会保存在程序当前工作目录，可用记事本打开检查。"); // 说明数据位置
    load_database(&database); // 启动时尝试加载已有数据
    ui_pause(); // 让用户准备后进入菜单
    run_management_menu(&database); // 运行完整管理系统
    ui_homework(homework, (int)(sizeof(homework) / sizeof(homework[0]))); // 显示项目扩展作业
    ui_complete(12, correct, 2); // 显示项目完成信息
} // lesson_12_project 函数结束

// ============================================================================
// 第 12 章项目作业（先让当前版本稳定，再逐步增加复杂度）
// 1. 动态容量：把 Student students[MAX_STUDENTS] 改为 Student *students，维护 size/capacity。
//    要求：添加时容量满则翻倍；加载、释放、错误路径都不泄漏；所有旧功能保持可用。
// 2. 查询与排序：增加按姓名子串查找、按单科排序、修改单科成绩、显示排名。
//    要求：输入非法科目或空关键字时有明确提示；排序稳定性不影响学号唯一性。
// 3. 导入导出：在 CSV 基础上增加二进制格式；导入前校验文件头、记录数和每条数据。
//    进阶：实现“先写临时文件，成功后再替换原文件”，避免保存中途损坏数据。
// 4. 测试清单：至少 20 条，覆盖空库、满库、重复学号、0/100 分、删除头尾中间、坏文件。
//    每条包含输入、预期输出、实际结果和是否通过；修复所有失败项。
// 5. 模块化重构：拆成 student.h/student.c/database.h/database.c/main.c，写 Makefile。
//    要求：头文件有包含守卫；内部函数用 static；编译命令开启 -Wall -Wextra -Wpedantic。
//
// 提交标准：
// - 项目能连续完成“添加 -> 保存 -> 退出 -> 重新运行 -> 加载 -> 修改 -> 再保存”。
// - 每个失败路径都有错误信息，且不破坏已有数据。
// - 代码仍遵守“每行 C 代码都有注释”的约定，并附一份不超过 10 行的架构说明。
// ============================================================================

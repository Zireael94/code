param( # 冒烟测试参数
    [string]$Root = (Split-Path -Parent $PSScriptRoot) # 默认项目根目录为 tests 的上一级
) # 参数声明结束

$ErrorActionPreference = 'Stop' # 任何未捕获错误都让测试失败
$gcc = Get-Command gcc -ErrorAction SilentlyContinue # 查找 C 编译器
if ($null -eq $gcc) { # 没有编译器就无法继续
    Write-Host '[FAIL] 未找到 gcc，请先安装 MinGW-w64 并加入 PATH。' -ForegroundColor Red # 输出失败原因
    exit 1 # 失败退出
} # 编译器检查结束

$tempDir = Join-Path ([System.IO.Path]::GetTempPath()) ('c_tutor_smoke_' + [guid]::NewGuid().ToString('N')) # 创建隔离的临时工作目录
$testExe = Join-Path $tempDir 'c_tutor_smoke.exe' # 测试用可执行文件路径
$pushed = $false # 记录是否已经进入临时目录
$passed = 0 # 通过项计数

function Assert-True { # 断言辅助函数：条件为真才继续
    param([bool]$Condition, [string]$Message) # 条件与失败说明
    if (-not $Condition) { # 条件不成立
        throw $Message # 抛出异常，由外层统一报告
    } # 条件判断结束
    $script:passed++ # 通过项加一
    Write-Host "[PASS] $Message" -ForegroundColor Green # 输出通过信息
} # Assert-True 函数结束

try { # 无论成功失败都清理临时文件
    New-Item -ItemType Directory -Force -Path $tempDir | Out-Null # 创建临时目录
    $sources = @(Get-ChildItem -LiteralPath (Join-Path $Root 'src') -Filter '*.c' -File | Sort-Object Name | ForEach-Object { $_.FullName }) # 收集全部源文件
    $includeDir = Join-Path $Root 'include' # 头文件目录
    $buildArgs = @('-std=c11', '-Wall', '-Wextra', '-Wpedantic', '-Werror', '-g', '-finput-charset=UTF-8', '-fexec-charset=UTF-8', '-I', $includeDir) + $sources + @('-o', $testExe, '-lm') # 使用零警告标准编译
    $buildOutput = & $gcc.Source @buildArgs 2>&1 # 执行编译并收集诊断
    if ($LASTEXITCODE -ne 0) { # 编译失败
        throw "编译失败：`n$($buildOutput | Out-String)" # 带上编译器输出
    } # 编译检查结束
    Assert-True (Test-Path -LiteralPath $testExe) '严格警告模式下编译成功' # 确认产物存在

    Push-Location $tempDir # 把工作目录切到临时目录，避免污染项目数据
    $pushed = $true # 记录目录已切换

    $versionOutput = & $testExe --version 2>&1 | Out-String # 测试版本参数
    Assert-True ($LASTEXITCODE -eq 0 -and $versionOutput -match 'C 语言阶梯教程') '--version 正常输出并返回 0' # 检查版本命令

    $listOutput = & $testExe --list 2>&1 | Out-String # 测试章节列表
    Assert-True ($LASTEXITCODE -eq 0 -and $listOutput -match '12\.') '--list 列出完整 12 章' # 检查列表包含第 12 章

    $chapter1Input = @('', '', 'Smoke', '20', 'TestCity', '', '3.5', '4', 'B', 'C', '') -join "`n" # 构造第 1 章脚本输入
    $chapter1Output = ($chapter1Input + "`n" | & $testExe --chapter 1 2>&1 | Out-String) # 运行第 1 章并捕获输出
    Assert-True ($LASTEXITCODE -eq 0 -and $chapter1Output -match '2 / 2' -and $chapter1Output -match 'Hello, C!') '第 1 章案例、小测和作业流程可完成' # 检查关键输出

    $projectInput = @('', 'B', 'B', '', '2', '9001', 'Smoke', '80', '90', '70', '8', '0', '') -join "`n" # 构造综合项目添加并保存的输入
    $projectOutput = ($projectInput + "`n" | & $testExe --chapter 12 2>&1 | Out-String) # 运行综合项目
    Assert-True ($LASTEXITCODE -eq 0 -and $projectOutput -match '已添加') '综合项目可以添加学生并正常返回' # 检查执行结果
    $dataPath = Join-Path $tempDir 'students.csv' # 综合项目数据文件
    Assert-True (Test-Path -LiteralPath $dataPath) '综合项目生成了 students.csv' # 检查文件持久化
    $dataText = Get-Content -LiteralPath $dataPath -Raw -Encoding UTF8 # 读取保存内容
    Assert-True ($dataText -match '9001,Smoke,80\.0,90\.0,70\.0') 'CSV 内容包含新增学生的完整成绩' # 检查保存字段

    $loadInput = @('', 'B', 'B', '', '1', '0', '') -join "`n" # 构造重新加载并查看的输入
    $loadOutput = ($loadInput + "`n" | & $testExe --chapter 12 2>&1 | Out-String) # 再次启动综合项目
    Assert-True ($LASTEXITCODE -eq 0 -and $loadOutput -match '已从 students.csv 加载 1 条记录' -and $loadOutput -match '9001') '重新启动后能加载并显示持久化数据' # 检查加载结果

    Write-Host "[PASS] 冒烟测试全部通过：$passed 项。" -ForegroundColor Green # 输出总结果
    exit 0 # 成功退出
} catch { # 捕获任何断言或运行错误
    Write-Host "[FAIL] $($_.Exception.Message)" -ForegroundColor Red # 输出失败原因
    exit 1 # 失败退出
} finally { # 清理阶段
    if ($pushed) { # 只有切换过目录才需要返回
        Pop-Location # 返回原目录
    } # 目录恢复结束
    if (Test-Path -LiteralPath $tempDir) { # 临时目录存在时清理
        Remove-Item -LiteralPath $tempDir -Recurse -Force -ErrorAction SilentlyContinue # 删除测试产物，避免留下垃圾
    } # 清理判断结束
} # 测试脚本结束

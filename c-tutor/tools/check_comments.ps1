param( # 脚本参数：允许从任意目录检查指定项目根目录
    [string]$Root = (Split-Path -Parent $PSScriptRoot) # 默认使用 tools 目录的上一级作为项目根目录
) # 参数声明结束

$ErrorActionPreference = 'Stop' # 遇到意外错误立即停止，避免误报“全部通过”
$sourceDir = Join-Path $Root 'src' # 检查所有章节和公共实现
$includeDir = Join-Path $Root 'include' # 检查公共头文件
$files = @() # 收集待检查的 .c 和 .h 文件
$files += Get-ChildItem -LiteralPath $sourceDir -Filter '*.c' -File # 加入所有 C 源文件
$files += Get-ChildItem -LiteralPath $sourceDir -Filter '*.h' -File # 加入 src 下的头文件（如果有）
$files += Get-ChildItem -LiteralPath $includeDir -Filter '*.h' -File # 加入 include 下的头文件
$problems = @() # 保存缺少注释的行
$checkedLines = 0 # 统计实际检查的非空行数

foreach ($file in $files | Sort-Object FullName) { # 按路径顺序检查，输出稳定
    $lineNumber = 0 # 行号从 1 开始
    foreach ($line in Get-Content -LiteralPath $file.FullName -Encoding UTF8) { # 逐行读取 UTF-8 文件
        $lineNumber++ # 更新行号
        if ($line.Trim().Length -eq 0) { # 空行不算代码行，也不要求注释
            continue # 跳过空行
        } # 空行判断结束
        $checkedLines++ # 统计一个需要注释的非空行
        $inString = $false # 标记是否在双引号字符串内
        $inChar = $false # 标记是否在单引号字符常量内
        $escaped = $false # 标记下一个字符是否被反斜杠转义
        $foundComment = $false # 标记是否找到真正的行注释
        for ($i = 0; $i -lt $line.Length - 1; $i++) { # 逐字符扫描，寻找字符串之外的 //
            $current = $line[$i] # 当前字符
            if ($escaped) { # 如果上一个字符是反斜杠
                $escaped = $false # 当前字符已被转义，不参与状态切换
                continue # 继续扫描下一个字符
            } # 转义状态处理结束
            if (($inString -or $inChar) -and $current -eq '\') { # 字符串或字符常量中的反斜杠
                $escaped = $true # 标记下一个字符被转义
                continue # 继续扫描
            } # 反斜杠处理结束
            if (-not $inChar -and $current -eq '"') { # 双引号切换字符串状态
                $inString = -not $inString # 进入或离开字符串
                continue # 继续扫描
            } # 双引号处理结束
            if (-not $inString -and $current -eq "'") { # 单引号切换字符常量状态
                $inChar = -not $inChar # 进入或离开字符常量
                continue # 继续扫描
            } # 单引号处理结束
            if (-not $inString -and -not $inChar -and $current -eq '/' -and $line[$i + 1] -eq '/') { # 发现字符串之外的 //
                $foundComment = $true # 标记这一行有真正的注释
                break # 不必继续扫描
            } # 注释判断结束
        } # 字符扫描结束
        if (-not $foundComment) { # 如果没有找到字符串外的行注释
            $problems += [PSCustomObject]@{ # 记录问题文件、行号和内容
                File = $file.FullName # 完整路径，便于编辑器跳转
                Line = $lineNumber # 行号
                Text = $line.Trim() # 精简后的源码内容
            } # 问题对象结束
        } # 缺失注释判断结束
    } # 文件行遍历结束
} # 文件遍历结束

if ($problems.Count -eq 0) { # 所有非空行都带注释
    Write-Host "[PASS] 已检查 $($files.Count) 个 C/H 文件、$checkedLines 个非空行，每行都包含 // 注释。" -ForegroundColor Green # 输出成功摘要
    exit 0 # 成功退出
} # 成功分支结束

Write-Host "[FAIL] 发现 $($problems.Count) 行缺少字符串之外的行注释：" -ForegroundColor Red # 输出失败摘要
$problems | ForEach-Object { # 逐条打印问题
    Write-Host ("  {0}:{1}: {2}" -f $_.File, $_.Line, $_.Text) -ForegroundColor Yellow # 输出可定位的文件、行号和代码
} # 问题打印结束
exit 1 # 失败退出，方便 CI 和 make check 使用

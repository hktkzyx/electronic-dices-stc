# Electronic dices STC 电子骰子

电子骰子固件项目([Official Website](https://hktkzyx.github.io/electronic-dices-stc/))。

对应的电子骰子硬件项目@hktkzyx/electronic-dices-stc-hardware
和[文档](https://hktkzyx.github.io/electronic-dices-stc-hardware/)。

## 简介

该固件检测按键`Set`和按键`Roll`，实现投掷1–4个骰子并自动休眠的功能。

## 安装

本项目依赖[platformio](https://platformio.org/)平台。
应先安装[platformio IDE](https://platformio.org/platformio-ide)或[platformio core](https://platformio.org/install/cli)。

安装platformio平台之后，克隆或下载本固件代码库并进入项目文件夹

```bash
git clone https://github.com/hktkzyx/electronic-dices-stc.git
cd electronic-dices-stc
```

如使用platformio IDE，则用VSCode打开项目文件夹，编译(build)并上传(upload)该固件即可。
如使用platformio core，则运行下列命令编译并上传

```bash
pio run --target upload
```

## 贡献

本人非电子专业，能力有限，非常欢迎对本项目的批评指正。
在修改本代码前，请先熟悉对应的[硬件项目](https://github.com/hktkzyx/electronic-dices-stc-hardware)。

为方便开发，保持代码的一致性，开发前需要

1. [commitizen](https://github.com/commitizen-tools/commitizen) and [pre-commit](https://github.com/commitizen-tools/commitizen)
   
    安装`commitizen`和`pre-commit`,并用`commitizen`提交commit。

2. [clang-format](https://clang.llvm.org/docs/ClangFormat.html)

    使用`clang-format`和项目里的配置文件[.clang-format](https://github.com/hktkzyx/electronic-dices-stc/blob/main/.clang-format)格式化代码。

3. [git-flow](https://github.com/petervanderdoes/gitflow-avh)

    遵循gitflow工作模式

贡献的代码合并到`develop`分支，再提交 PR 。

*[PR]: Pull Request

## License

Copyright (c) 2022 hktkzyx.

Electronic-dices-stc-firmware is licensed under Mulan PSL v2.

You can use this software according to the terms and conditions of the Mulan PSL v2.
You may obtain a copy of Mulan PSL v2 at: http://license.coscl.org.cn/MulanPSL2.

THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY 
KIND, EITHER EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO 
NON-INFRINGEMENT,
MERCHANTABILITY OR FIT FOR A PARTICULAR PURPOSE.

See the Mulan PSL v2 for more details.

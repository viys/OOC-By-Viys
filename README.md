# 用 C 语言实现面向对象编程（OOP）教程  
## ——以“动物说话系统”为例

---

## 目录

1. [面向对象简介](#面向对象简介)  
2. [项目结构总览](#项目结构总览)  
3. [四大特性详解](#四大特性详解)  
   - [封装（Encapsulation）](#封装encapsulation)  
   - [继承（Inheritance）](#继承inheritance)  
   - [多态（Polymorphism）](#多态polymorphism)  
   - [抽象（Abstraction）](#抽象abstraction)  
4. [编译与运行](#编译与运行)  
5. [运行结果与解释](#运行结果与解释)  
6. [总结](#总结)

---

## 面向对象简介

虽然 C 是过程式语言，但我们可以通过 `struct` 和函数指针模拟出“类、对象、方法”等机制，实现如下特性：

- **封装**：隐藏内部数据，通过接口访问  
- **继承**：子类复用父类属性和方法  
- **多态**：相同接口，不同实现  
- **抽象**：定义通用接口，屏蔽细节差异  

本项目演示了如何在 C 中构造一个支持 OOP 的框架。

---

## 项目结构总览

```text
oopc/
├─ build
│  └─ my_config.h	// 配置项（通过 Kconfig 生成）
└─ project
   ├─ animal
   │  ├─ animal.c	// 类实现：Animal、Cat、Dog
   │  └─ animal.h	// 类接口声明
   └─ app
      └─ main.c		// 程序入口，创建和使用对象
```

## 四大特性详解

### 封装（Encapsulation）

> 数据私有，通过接口访问

```c
typedef struct {
    ANIMAL_CLASS_IMPLEMENTS api;
    Animal_Attr attr;
} ANIMAL_CLASS;
```

外部代码通过 `get_name()` 访问 `attr.name`，而非直接操作：

```c
int animal_get_name(void* t, char* name);
```

在 `main.c` 中使用方式：

```c
anmial_cat->get_name(anmial_cat, name);
```

------

### 继承（Inheritance）

> 子类构造函数基于父类对象构造，只重写行为

构造函数示例（dog）：

```c
DOG_CLASS* DOG_CLASS_CTOR(ANIMAL_CLASS_IMPLEMENTS* t);
```

重写 `speak` 行为：

```c
this->api.speak = dog_speak;
```

形成“继承自 Animal，仅重写发声”的类结构。

------

### 多态（Polymorphism）

> 相同接口，不同实现

统一接口：

```c
static int animal_sound(void* t) {
    ANIMAL_CLASS_IMPLEMENTS* this = (ANIMAL_CLASS_IMPLEMENTS*)t;
    this->speak(this);
    return 0;
}
```

无论传入 `cat` 还是 `dog`，都能调用正确的 `speak` 方法。

------

### 抽象（Abstraction）

> 先定义通用接口，各类实现

```c
typedef struct {
    int (*init)(void* t, Animal_Attr attr);
    int (*get_name)(void* t, char* name);
    int (*speak)(void* t);
} ANIMAL_CLASS_IMPLEMENTS;
```

所有类都必须实现该接口，这构成了最基本的行为契约。

------

## 编译与运行

本项目使用 **CMake 构建系统**，并支持通过 `menuconfig` 进行配置。

### 步骤 1：创建构建目录

```bash
mkdir build
cd build
```

### 步骤 2：生成构建系统

#### Windows（使用 MinGW）：

```bash
cmake -G "MinGW Makefiles" ..
```

#### Linux：

```bash
cmake ..
```

> 💡 请确保已安装 CMake、GCC（或 MinGW）、以及支持 `menuconfig` 的工具（如 kconfig-frontends）。

------

### 步骤 3：菜单配置（可选）

> `kconfig` 使用教程详见 https://github.com/viys/kconfig.git，若不想配置则删除 `#include "my_config.h"` 并对 `CONFIG_ANIMAL_NAME_1` 和 `CONFIG_ANIMAL_NAME_2` 作出定义。也可在 `build` 目录下手动创建如下的 `my_config.h` 文件。

```bash
make menuconfig
```

该步骤用于生成配置文件 `my_config.h`，例如设置：

```c
#define CONFIG_ANIMAL_NAME_1 "cat"
#define CONFIG_ANIMAL_NAME_2 "dog"
```

------

### 步骤 4：编译项目

```bash
make
```

------

### 步骤 5：运行程序

#### Windows：

```bash
oopc.exe
```

#### Linux / macOS：

```bash
./oopc
```

------

## 运行结果与解释

```bash
>
animal name is cat
animal name is dog
animal cat say: Meow!
animal dog say: Woof!
animal cat say: Meow!
animal dog say: Woof!
<
```

含义解析：

- `animal name is ...`：测试封装（通过接口获取名称）
- `say:`：继承后重写 `speak`
- 再次调用 `animal_sound`：验证多态接口调用行为

------

## 总结

| 特性 | 在代码中的体现                                       |
| ---- | ---------------------------------------------------- |
| 封装 | `get_name()` 方法隐藏结构细节                        |
| 继承 | Dog/Cat 构造函数使用 Animal 对象并重写部分方法       |
| 多态 | `animal_sound()` 使用统一接口，实际调用因对象而异    |
| 抽象 | `ANIMAL_CLASS_IMPLEMENTS` 为统一接口，各类实现其行为 |




------


> ✅ 本项目演示了如何用纯 C 模拟 C++/Java 中的类继承结构，是构建模块化、可扩展嵌入式系统的有效实践方式。

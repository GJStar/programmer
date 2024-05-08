# 动态库加载方式

这里说的so就是动态库 ，加载方式分为两种

* explicit: dlopen显式拉起

* implicit: 在链接的时候指定so的依赖

# 1. 显示加载：

## 1.1 dlopen

dlopen之后会获得一个句柄，就是这个so在内存中的加载地址

* 同一个进程dlopen同一个so，内存中不会增加副本，只会增加dlopen的指针计数（配合dlclose使用）

* 可以用于热加载，用于实现c语言的多态

* dlopen同一个so，不会增加句柄计数，所以一个进程里面忘记dlclose一次就会增加一个句柄泄漏

* dlopen之后如果立即dlclose，那么下一次dlopen获得的内存地址是一样的（参考实验）

## 1.2 dlsym

dlsym两种使用方法：

1. 使用dlopen获得的handle，在指定的so里面寻找符号

2. 不指定dlopen的句柄，在依赖的so里面全局搜索
   
   1. RTLD_NEXT： 按照链接顺序，找到符号表第一次出现的地方，可以用于实现**preload机制**
   
   2. RTLD_DEFAULT：直接全局搜索符号表第一次出现的地方

## 1.3 dlclose

* dlclose用于unload。每次dlclose都会导致指针技术-1， 计数为0的时候，就会unload 当前动态库
  
  * 隐式load的动态库，也会增加计数

# 2. 隐示加载

一般使用ld或者cmakelist里面指定的方式，隐式制定依赖关系，在加载一个so的时候，会同时把所有的依赖so都加载到内存里面

* 优点：编码方便，不容易导致句柄泄漏，大部分情况用隐示加载

* 缺点：不够灵活，耦合严重，实现松耦合的目标，不可以使用隐式加载

# 3. 实验步骤

```shell
cd 
make all
./demo
```



```c
#include <stdio.h>
#include <dlfcn.h>

typedef void (* func_t)();
const char* g_relative_path = "./libsym.so";

/* lab3：dlopen/dlclose之后，下一次打开so还是在同一个内存地址 */
void test_open_close()
{

	void *handle = dlopen(g_relative_path, RTLD_LAZY);
	if (!handle) {
		printf("%s\n", dlerror());
		return;
	}

	printf("handle fisrt addr:0x%llx\n", (unsigned long long)handle);
	func_t func = dlsym(handle, "sym");
	dlclose(handle);

	/* 第二次打开so */
	handle = dlopen(g_relative_path, RTLD_LAZY);
	if (!handle) {
		printf("%s\n", dlerror());
		return;
	}

	printf("handle second addr:0x%llx\n", (unsigned long long)handle);
	func(); // 这种是违规操作，这里是为了验证两次内存空间一模一样
	dlclose(handle);

	handle = dlopen("./libadd.so", RTLD_LAZY);
	if (!handle) {
		printf("%s\n", dlerror());
		return;
	}

	printf("handle dlopen libadd.so addr:0x%llx\n", (unsigned long long)handle);


	void *handle2 = dlopen(g_relative_path, RTLD_LAZY);
	if (!handle2) {
		printf("%s\n", dlerror());
		return;
	}

	printf("handle third addr:0x%llx\n", (unsigned long long)handle2);
	dlclose(handle);
	dlclose(handle2);
	return;
}

int main()
{
	// 测试不同情况查下dlopen之后的效果
	test_open_close();
	return 0;
}
```

#include <stdio.h>
#include <dlfcn.h>

typedef void (* func_t)();
const char* g_relative_path = "./libsym.so";

/* lab1：dlopen/dlclose之后，下一次打开so还是在同一个内存地址 */
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

	// 第三次打开so,起始地址同第一次第二次
	handle = dlopen("./libadd.so", RTLD_LAZY);
	if (!handle) {
		printf("%s\n", dlerror());
		return;
	}

	printf("handle dlopen libadd.so addr:0x%llx\n", (unsigned long long)handle);

	// 第四次打开so，起始地址和前三次不一样
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

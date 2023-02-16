# module_test

STM32f103 系列单片机的代码库总结



| `Project.jscope` | jscope测试文件          |
| ---------------- | ----------------------- |
| `gitignore`      | github的配置文件        |
| `.clang-format`  | clang的代码格式配置文件 |
| `.git`           | github存储文件          |



​            

#### USB CDC 虚拟串口速度测试

USB CDC传输应该采用的是类似中断的算法（USB传输具体原理还没学会-_-||），直接记录`CDC_Transmit_FS()`  前后的时间是测不出来的，无论传输的数据大小是多少，时间差都是很小的不足`1ms`，所以需要用循环检测USB状态，确保USB不繁忙，才使用该函数。这样才能测出真实的传输速度。其次为了检测数据传输的完整性也为了保证程序不死在循环里（未连接时USB寄存器无法发出数据，状态为繁忙），我在测试的时候采用串口调试助手接收数据，检查数据的完整性。由于每次重新烧写代码后，需要插拔USB（[博客](https://blog.csdn.net/zhanglifu3601881/article/details/90645559) 里提出了一种不需要插拔的方法，后续可以测试一下），重连串口助手才能传输数据，从烧写入代码到能够接收数据需要一段时间，只发一次数据的话，难以接收到，所以需要循环发送数据。实际测试时，发现循环不能太快，不然串口接收助手直接就卡死了，发送速度太快了。一个下午的折腾后，最终采用循环发送，间隔1s的程序来进行测试。

```c
unsigned char buff[10] = {"mmcdablcdd"};
unsigned char buff[100] = {"mmcdablcddmmcdablcddmmcdablcddmmcdablcddmmcdablcddmmcdablcddmmcdablcddmmcdablcddmmcdablcddmmcdablcdd"};
unsigned char buff[1000] = {"mmabcdabcbcdabcdabcdbcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcddmabcdabcbcdabcdabcdbcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcddmabcdabcbcdabcdabcdbcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcddmabcdabcbcdabcdabcdbcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcddmabcdabcbcdabcdabcdbcdabcdabcdabcddabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdd"};
uint8_t result = USBD_BUSY;
uint32_t a,b;
TEST 	test_USB_CDC_speed(TEST)
{
	MX_USB_DEVICE_Init();
	
	while(1){
		a = HAL_GetTick();
		for(int i = 0;i<1000;i++){
//			printf("send package %d\r\n",i);
			while(CDC_Transmit_FS(buff, sizeof(buff)) != USBD_OK);
		}
		b = HAL_GetTick();
		TEST_MSG("%d-%d, usb speed = %.2f k/s \r\n", a,b,10.0*1000.0/(b-a));
		HAL_Delay(1000);
	}
}
```

USB虚拟串口传输测试修改条件有每次传输的数据大小，总传输的数据大小。

每次传输的数据有1000，100和10字节，每次传输的总数据控制在10 k，测试结果为：

```
// buff 1000字节时，每次发送10k，传输速度约 833 kb/s 
// buff 100字节时，每次发送10k，555.56kb/s
// buff 10字节时，每次发送10k，147k-153k/s 
```

每次传输的数据有100字节，每次传输的总数据控制在1k，10k，100k，测试结果为：

```
// buff 100字节时
500-1000 kb/s  每次发送1k数据
555.56kb/s 每次发送10k
2.5k/s 每次发送100k

// buff 10字节时
140-160 kb/s 每次发送1k数据
147k-153k/s  每次发送10k
```





<img src="README.assets/image-20220929200727556.png" style="zoom:80%;" /> 

 

 


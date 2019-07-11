### 多终端切换操作
ctrl+alt+t是打开一个terminal
ctrl+shift+t是在terminal中打开多个标签

在多个标签中切换的方法：
方法1
alt+1 alt+2 alt+3…….

方法二
ctrl + pageUp
ctrl + pageDown.

ctrl+ d：关闭一个terminal快捷键
### 网络基本概念
#### OSI模型
OSI模型(Open System Interconnection model, 开放系统互联模型）是一个由国际标准化组织提出概念模型，试图
提供一个使各种不同的计算机和网络在世界范围内实现互联的标准框架。他讲计算机网络体系结构划分为七层，每层都可以提供抽象良好的接口，七层分别为：应用层-->表示层-->会话层-->传输层-->网络层-->数据链路层-->物理层。
OSI模型是一个理想化的模型，实际上的协议比如TCP/IP并不是严格按照此模型来做的。
#### 物理层
物理层负责最后将信息编码成电流脉冲或其他信号用于网上传输。比如RS-232规范、RJ-45都属于物理层。所有比物理层高的层都通过事先定义好的接口与它通信。
#### 数据链路层
数据链路层通过物理网络链路提供数据传输。不同的数据链路层定义了不同的网络和协议特征，其中包括物理编址、网络拓扑结构、错误校验、数据帧序列以及流控。数据链路层实际上由两个独立的部分组成，介质存取控制(Media Access Control, MAC)和逻辑链路控制层(Logical Link Control, LLC)。
#### 网络层
网络层负责在源和终点之间建立连接， 它一般包括网络寻径，还可能包括流量控制、错误检查等。相同 MAC 标准的不同网段之间的数据传输一般只涉及到数据链路层，而不同的MAC标准之间的数据传输都涉及到网络层。网络层使不同类型的数据网络能够实现互联。
#### 传输层
传输层向高层提供可靠的端到端的网络数据流服务。传输层的功能一般包括流控、多路传输、虚电路管理及差错校验和恢复。
#### 会话层
#### 表示层
表示层提供多种功能用于应用层数据编码和转化，以确保以一个系统应用层发送的信息可以被另一个系统应用层识别。
#### 应用层
应用层是最接近终端用户的OSI层，这就意味着OSI应用层与用户之间是通过应用软件直接相互作用的。
### TCP/IP协议基本概念
OSI 模型所分的七层，在实际应用中，往往有一些层被整合，或者功能分散到其他层去。TCP/IP协议是现在互联网络事实上的协议标准，但是 TCP/IP 并没有照搬 OSI 模型，也没有一个公认的TCP/IP层级模型，所以很多技术文件按照 TCP/IP 的实际情况，划分为三层到五层模型来描述TCP/IP协议。

IP 定义了TCP/IP的地址，寻址方法，以及路由规则。现在广泛使用的IP协议有IPv4和IPv6两种：IPv4使用**32**位二进制整数做地址，一般使用点分十进制方式表示，比如192.168.0.1。**IP地址由两部分组成，即网络号和主机号**。故一个完整的IPv4地址往往表示为192.168.0.1/24或192.168.0.1/255.255.255.0这种形式。

IPv6是为了解决IPv4地址耗尽和其它一些问题而研发的最新版本的IP。使用**128**位整数表示地址，通常使用冒号分隔的十六进制来表示，并且可以省略其中一串连续的0，比如：fe80::200:1ff:fe00:1。IPv6提供了一些IPv4没有的新特性，并且有几乎用不完的地址，但目前还在部署过程中，尤其是国内除高校和科研机构外并不常用。

TCP(Transmission Control Protocol，传输控制协议)是一种面向连接的，可靠的，基于字节流传输的通信协议。TCP具有端口号的概念，用来标识同一个地址上的不同应用。描述TCP的标准文档是RFC793。

UDP（User Datagram Protocol，用户数据报协议）是一个面向数据报的传输层协议。UDP的传输是不可靠的，简单的说就是发了不管，发送者不会知道目标地址的数据通路是否发生拥塞，也不知道数据是否到达，是否完整以及是否还是原来的次序。它同TCP一样有用来标识本地应用的端口号。所以应用UDP的应用，都能够容忍一定数量的错误和丢包，但是对传输性能敏感的，比如流媒体、DNS等。描述UDP的标准文档是RFC768。

ECHO（Echo Protocol，回声协议）是一个简单的调试和检测工具。服务器器会原样回发它收到的任何数据，既可以使用TCP传输，也可以使用UDP传输。使用端口号7，描述它的标准文档是RFC862。

FTP（File Transfer Protocol，文件传输协议）是用来进行文件传输的标准协议。FTP基于TCP使用端口号20来传输数据，21来传输控制信息。现在对其的描述文档是RFC959。

TFTP（Trivial File Transfer Protocol，简单文件传输协议）是一个简化的文件传输协议，其设计非常简单，通过少量存储器就能轻松实现，所以**一般被用来通过网络引导计算机过程中传输引导文件等小文件**。早期甚至有相当糟糕的协议缺陷，**在传输大量文件时建议不要使用TFTP**。相关的文档有RFC1350以及RFC2347等一系列文档。

SSH（Secure Shell，安全 Shell），因为传统的网络服务程序比如TELNET本质上都极不安全，明文传说数据和用户信息包括密码，SSH被开发出来避免这些问题，它其实是一个协议框架，有大量的扩展冗余能力，并且提供了加密压缩的通道可以为其他协议使用。

HTTP（HyperText Transfer Protocol，超文本传输协议）是现在广为流行的WEB网络的基础，HTTPS是HTTP的加密安全版本。协议通过TCP传输，HTTP默认使用端口80，HTTPS使用443。描述它的文档有很多，最广泛使用的HTTP 1.1描述在RFC2616中。

### 编程接口 BSD Socket
#### Socket简介
用Socket能够实现网络上的不同主机之间或同一主机的不同对象之间的数据通信。所以，现在Socket已经是一类通用通信接口的集合。
#### 基础数据结构和函数
IP协议使用的地址描述数据结构，使用需要包括头文件**<netinet/in.h>**。Linux下该结构的典型原型声明如下：
```c++
236 /* Structure describing an Internet socket address. */
237 struct sockaddr_in
238 {
239 	__SOCKADDR_COMMON (sin_);
	// 端口号
240 	in_port_t sin_port; /* Port number. */
	// 一个 32 位二进制整数代表的IP地址
241 	struct in_addr sin_addr; /* Internet address. */
242
243 	/* Pad to size of `struct sockaddr'. */
244 	unsigned char sin_zero[sizeof (struct sockaddr) -
245 	__SOCKADDR_COMMON_SIZE -
246 	sizeof (in_port_t)
247 	sizeof (struct in_addr)];
248 };
```
#### BSD Socket常用操作
1. 创建Socket
```c++
// 在进行Socket通信之前，一般调用 socket(2)函数来创建一个Socket通信端点。socket(2)函数原型如下：
// domain代表这个Socket所使用的地址类型，即IP协议簇，IPv4为AF_INET
// type代表了这个Socket的类型，我们讨论范围是有面向流的(TCP)和面向数据报的(UDP)Socket.分别取值
// SOCK_STREAM 和 SOCK_DGRAM。protocol 是协议类型，对于我们的应用场景，都取0即可。
// 成功返回一个有效的文件描述符，出错时返回-1，此时需要处理的错误码见表 17.1
int socket(int domain, int type, int protocol);
// 创建 TCP Socket：
sock_fd = socket(AF_INET, SOCK_STREAM, 0);
// 创建 UDP Socket：
sock_fd = socket(AF_INET, SOCK_DGRAM, 0);
```
2. 绑定地址和端口
```c++
// 可以调用 bind(2)函数来将这个 Socket 绑定到特定的地址和端口上来进行通信
// address参数就是一个指向struct sockaddr结构的指针，根据不同的协议，可以有不同的具体结构，
// 对于IP地址，就是struct sockaddr_in. 但是在调用函数的时候需要强制转换一下这个指针避免警告。
// address_len，因为前面的地址可能有各种不同的地址结构，所以此处应该指明使用的地址数据结构的长度。
// 编程时直接取 sizeof(struct sockaddr_in)即可. 检查的 errno 值见表 17.2
int bind(int socket, const struct sockaddr *address, socklentaddress_len);
```
3. 连接服务器
```c++
/*
   对于客户机， 使用 TCP 协议时，在通讯前必须调用 connect(2)连接到需要通信的服务器的特定通信端点后
   才能正确进行通信。对于使用UDP协议的客户机，这个步骤是可选项。如果使用了connect(2)，在此之后可以
   不需要指定数据报的目的地址而直接发送，否则每次发送数据均需要指定数据报的目的地址和端口。
   检查的 errno 见表 17.3
*/
int connect(int socket, const struct sockaddr *address, socklentaddress_len);
```
4. 设置Socket为监听模式
```c++
/*
   基于 TCP 协议的服务器，需调用 listen(2)函数将其 Socket 设置成被动模式，等待客户机的连接
   检测处理的errno见表 17.4
*/
int listen(int socket, int backlog)
```
5. 接受连接
```c++
/*
   TCP 服务器还需要调用 accept(2)用来处理到来的客户机连接请求
   socket 和前面的函数都一样， address 也是一样的结构， 但是在accept(2)这里是用来返回值的，
   在成功返回的时候，如果这个指针非空，这里将存储请求连接的客户端的地址和端口。
   须处理的 errno 如表 17.5 所示
*/
int accept(int socket, structsockaddr *restrict address, socklen_t*restrict address_len);
```
6. 数据读写函数
```
read(2)函数一般用于流式 Socket 简单读写数据，也就是对应 TCP 协议。
```

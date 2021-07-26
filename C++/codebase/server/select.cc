// summarize the select model
// 1. 单个进程可管理的最大文件描述符数有限制（1024），可以通过修改内核文件（FD_SETSIZE）来改变该限制，
//    但是这样就需要遍历一个更大的文件描述符集，来确定到底哪个文件描述符上有事件发生
// 2. select 需要在内核空间和用户空间拷贝大量的句柄数据结构，这会带来不小的负载
// 3. select 的触发模式只有水平触发

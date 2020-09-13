# l476mem_freertos
semaphore and memory 草稿
本试验目的加强freertos应用。试验中运用到：信号量、任务挂起与恢复，内存的申请与取消。
任务1：灯闪烁，一定时间后，释放信号量，试验同步目的；
任务2：等待信号量，收到信号量后，才开始申请内存，将内存首地址打印出来，将字符串信息存入
申请到的内存中，挂起任务1。
任务3：压下按键，释放内存，再将内存指针首地址打印出来观察，将任务1恢复。

freertos的调试工具是tracealyze。在多任务中，用打断点和跟踪寄存器方式不合适了。这些方式无法查任务之
间的联络关系，堆栈有没有溢出、任务运行状态等。tracealyze可用于多种软件调试，但是要付费的，很恼人。
以上是大致内容，时间紧，粗糙未修改。

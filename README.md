# fast-external-sort
海量浮点数快速外部排序算法
* 限制512MB内存，2.5亿浮点数排序
* 输入：IEEE754双精度浮点数标准，存在非法条目，`\n`分割
* 输出：10位有效数字，规整化`/^[-]?[\d][.][\d]{9}[E][+|-]?[\d]{3}$/`

---
## 技术介绍
* 文件大块二进制读写
* 浮点数-字符串快速转换
* 非法数据快速检查
* 浮点数基数内排
* 败者树加速k路归并
* 一些打表技巧

---
## 使用说明
* 测试数据生成：直接运行test/quick_tester2.exe，输入测试数据量
* 运行：配置exe/Sort.param，运行exe/Sort.exe

---
## 测评
* Intel(R) Core(TM) i7-8850H CPU @ 2.60GHz

| # Items | File Size | Rep. 1/ms | Rep. 2/ms | Rep. 3/ms | Avg. (sec.) |
| - | - | - | - | - | - |
| 30,000,000 | 517.1 MB | 7,881 | 7,455 | 7,517 | 7.6 |
| 250,000,000 | 4.3 GB | 69,095 | 68,834 | 69,465 | 69.1 |
| 1,000,000,000 | 17.2 GB | 296,758 | 304166 | 300701 | 300.5 |


---
## 致谢
* 本项目吸收了[01joy](https://github.com/01joy), georgeljc, [ctarn](https://github.com/ctarn)等人的部分技术技巧
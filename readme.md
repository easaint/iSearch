## 1.项目说明
本项目为打造PC端全文检索引擎，兼容XP系统，使用vs2013进行编译

### 1.1项目模块说明
 > TLSearchEngine ：全文检索引擎库项目

 > TLSeachEngineTest ：全文检索引擎库测试项目，使用MFC编写

 > TLCommon ：公共库项目，其他库可以引用

## 2.编译
### 2.1安装zlib库
因为项目使用了zlib库压缩，所以需要安装zlib库，下载地址：
[zlib库.zip](https://doc.tianlent.com:8443/media/attachment/2023/08/01%E5%8E%9F%E5%A7%8B%E5%BC%80%E5%8F%91%E5%8C%85%E4%BB%A5%E5%8F%8Avs%E7%BC%96%E8%AF%91%E8%AF%B4%E6%98%8E%E6%95%99%E7%A8%8B.zip)

### 2.2编译代码
首先克隆项目:

`git clone https://gitea.tianlent.com:8443/cpp_project/TLDataSearch.git`

然后使用vs2013即可编译成功

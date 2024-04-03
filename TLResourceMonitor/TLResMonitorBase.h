/***
 * 该文件定义一些基本公共的属性
 */
#ifndef _TL_RES_MONITOR_BASE_H
#define _TL_RES_MONITOR_BASE_H


//定义常见的文档格式
#define     WORD_97         _T(".doc")
#define     WORD            _T(".docx")
#define     EXCEL_97        _T(".xls")
#define     EXCEL           _T(".xlsx")
#define     EXCEL_M         _T(".xlsm")
#define     PPT_97          _T(".ppt")
#define     PPT             _T(".pptx")
#define     PDF             _T(".pdf")
#define     TEXT_TEXT       _T(".txt")
#define     TEXT_LOG        _T(".log")
#define     TEXT_CSV        _T(".csv")
#define     TEXT_SQL        _T(".sql")
#define     TEXT_ODF        _T(".odf")

//定义软件名称
#define SOFTWARE_NAME                       "iSearch"

//定义索引元数据表名称
#define T_INDEX_META_DATA                   "t_index_meta_data" //索引元数据表
#define T_INDEX_META_DATA_CREATE_SQL        "CREATE TABLE t_index_meta_data(id CHAR(32) PRIMARY KEY,file_hash CHAR(64) not null,file_name VARCHAR(300) not null,file_path VARCHAR(1024) not null,file_suffix VARCHAR(20), create_time VARCHAR(20));" //创建元数据表的sql语句

#define T_MONITOR_RECORD				    "t_monitor_record"  //监控记录表
#define T_MONITOR_RECORD_CREATE_SQL         "CREATE TABLE t_monitor_record(key VARCHAR(100),val VARCHAR(4000));" //创建监控记录表sql
#define T_MONITOR_RECORD_FULL_SCAN_KEY_SQL  "insert into t_monitor_record(key,val) values('full_disk_scan_last_path','');" //全盘扫描上一次扫描的路径

#endif
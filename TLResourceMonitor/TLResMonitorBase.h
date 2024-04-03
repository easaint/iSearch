/***
 * ���ļ�����һЩ��������������
 */
#ifndef _TL_RES_MONITOR_BASE_H
#define _TL_RES_MONITOR_BASE_H


//���峣�����ĵ���ʽ
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

//�����������
#define SOFTWARE_NAME                       "iSearch"

//��������Ԫ���ݱ�����
#define T_INDEX_META_DATA                   "t_index_meta_data" //����Ԫ���ݱ�
#define T_INDEX_META_DATA_CREATE_SQL        "CREATE TABLE t_index_meta_data(id CHAR(32) PRIMARY KEY,file_hash CHAR(64) not null,file_name VARCHAR(300) not null,file_path VARCHAR(1024) not null,file_suffix VARCHAR(20), create_time VARCHAR(20));" //����Ԫ���ݱ��sql���

#define T_MONITOR_RECORD				    "t_monitor_record"  //��ؼ�¼��
#define T_MONITOR_RECORD_CREATE_SQL         "CREATE TABLE t_monitor_record(key VARCHAR(100),val VARCHAR(4000));" //������ؼ�¼��sql
#define T_MONITOR_RECORD_FULL_SCAN_KEY_SQL  "insert into t_monitor_record(key,val) values('full_disk_scan_last_path','');" //ȫ��ɨ����һ��ɨ���·��

#endif
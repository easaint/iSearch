#ifndef _TL_SYNC_MAP_H
#define _TL_SYNC_MAP_H

#include <map>
#include <mutex>
#include <list>

#include "TLCommonBase.h"

/***
 * @��˵����
 *    ʵ���̰߳�ȫ��map����װstd::map
 */
template <typename K, typename V>
class CTLSyncMap{

private:
	//map
	std::map<K,V> m_map;
	//�߳���
	std::recursive_mutex m_mtx;

public:
	CTLSyncMap();
	~CTLSyncMap();

public:
	CTLSyncMap& operator=(const CTLSyncMap& sm)
	{
		if (&sm != this) {
			this->m_map = sm.map_;
		}
		return *this;
	}
	
	V& operator[](const K& key)
	{
		return this->m_map[key];
	}

	/***
	 * @����˵����
	 *    ��Ԫ�ط���map��
	 * @����˵����
	 *    key��map��key
	 *    value��ֵ����
	 */
	void put(const K& key, const V& value);

	/***
	 * @����˵����
	 *    ��ȡmap�Ĵ�С��Ԫ�ظ���
	 * @����ֵ��
	 *    ����Ԫ�صĸ���
	 */
	int size();

	/***
	 * @����˵����
	 *    ͨ��key��ȡvalue
	 * @������
	 *    key����Ҫ��ȡvalue��key
	 *    value��ֵ������
	 * @����ֵ��
	 *    ����ҵ�����TRUE��û���ҵ�����FALSE
	 */
	bool get(const K& key, V& value);

	/***
	 * @����˵����
	 *    ͨ��keyɾ��Ԫ��
	 * @����˵����
	 *    key����Ҫɾ����key
	 */
	void remove(const K& key);

	/***
	 * @����˵����
	 *    ��ȡ���е�key list
	 * @����ֵ
	 *    �������е�key
	 */
	std::list<K> keyList();

};

template <typename K, typename V>
CTLSyncMap<K, V>::CTLSyncMap()
{
}

template <typename K, typename V>
CTLSyncMap<K, V>::~CTLSyncMap()
{

}

template <typename K, typename V>
void CTLSyncMap<K, V>::put(const K& key, const V& value)
{
	std::lock_guard<std::recursive_mutex> lock(this->m_mtx);
	this->m_map.insert(std::pair<K, V>(key, value));
}

template <typename K, typename V>
int CTLSyncMap<K, V>::size()
{
	std::lock_guard<std::recursive_mutex> lock(this->m_mtx);
	return this->m_map.size();
}


template <typename K, typename V>
bool CTLSyncMap<K, V>::get(const K& key, V& value)
{
	bool ret = false;
	std::lock_guard<std::recursive_mutex> lock(this->m_mtx);
	auto iter = this->m_map.find(key);
	if (iter != this->m_map.end()) {
		value = iter->second;
		ret = true;
	}
	return ret;
}

template <typename K, typename V>
void CTLSyncMap<K, V>::remove(const K& key)
{
	std::lock_guard<std::recursive_mutex> lock(this->m_mtx);
	this->m_map.erase(key);
}

template <typename K, typename V>
std::list<K> CTLSyncMap<K, V>::keyList()
{
	std::list<K> list;
	std::lock_guard<std::recursive_mutex> lock(this->m_mtx);
	for (std::map<K, V>::iterator it = this->m_map.begin(); it != this->m_map.end(); ++it)
	{
		list.push_back(it->first);
	}
	return list;
}
#endif
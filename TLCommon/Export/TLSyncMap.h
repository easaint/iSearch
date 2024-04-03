#ifndef _TL_SYNC_MAP_H
#define _TL_SYNC_MAP_H

#include <map>
#include <mutex>
#include <list>

#include "TLCommonBase.h"

/***
 * @类说明：
 *    实现线程安全的map，包装std::map
 */
template <typename K, typename V>
class CTLSyncMap{

private:
	//map
	std::map<K,V> m_map;
	//线程锁
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
	 * @函数说明：
	 *    将元素放入map中
	 * @参数说明：
	 *    key：map的key
	 *    value：值对象
	 */
	void put(const K& key, const V& value);

	/***
	 * @函数说明：
	 *    获取map的大小，元素个数
	 * @返回值：
	 *    返回元素的个数
	 */
	int size();

	/***
	 * @函数说明：
	 *    通过key获取value
	 * @参数：
	 *    key：你要获取value的key
	 *    value：值得引用
	 * @返回值：
	 *    如果找到返回TRUE，没有找到返回FALSE
	 */
	bool get(const K& key, V& value);

	/***
	 * @函数说明：
	 *    通过key删除元素
	 * @参数说明：
	 *    key：你要删除的key
	 */
	void remove(const K& key);

	/***
	 * @函数说明：
	 *    获取所有的key list
	 * @返回值
	 *    返回所有的key
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
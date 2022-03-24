package tmp

import "container/list"

const base = 769

type entry struct {
	Key, Value int
}

type MyHashMap struct {
	Data []list.List
}

func (this *MyHashMap) hash(x int) int {
	return x % base
}

/** Initialize your data structure here. */
func Constructor() MyHashMap {
	return MyHashMap{Data: make([]list.List, base)}
}

/** value will always be non-negative. */
func (this *MyHashMap) Put(key int, value int) {
	idx := this.hash(key)
	for e := this.Data[idx].Front(); e != nil; e = e.Next() {
		if et := e.Value.(entry); et.Key == key {
			e.Value = entry{key, value}
			return
		}
	}
	this.Data[idx].PushBack(entry{key, value})
}

/** Returns the value to which the specified key is mapped, or -1 if this map contains no mapping for the key */
func (this *MyHashMap) Get(key int) int {
	idx := this.hash(key)
	for e := this.Data[idx].Front(); e != nil; e = e.Next() {
		if et := e.Value.(entry); et.Key == key {
			return et.Value
		}
	}
	return -1
}

/** Removes the mapping of the specified value key if this map contains a mapping for the key */
func (this *MyHashMap) Remove(key int) {
	idx := this.hash(key)
	for e := this.Data[idx].Front(); e != nil; e = e.Next() {
		if et := e.Value.(entry); et.Key == key {
			this.Data[idx].Remove(e)
			return
		}
	}
}

/**
 * Your MyHashMap object will be instantiated and called as such:
 * obj := Constructor();
 * obj.Put(key,value);
 * param_2 := obj.Get(key);
 * obj.Remove(key);
 */

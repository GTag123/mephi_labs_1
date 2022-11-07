#include "iostream"
#include "unordered_map"
#include "vector"
#include "list"

using namespace std;

template <typename K, typename V>
class LruCache {
    unordered_map<K, typename list<pair<V, K>>::iterator> pointers; // опять всё поломалось
    list<pair<V, K>> data;
    size_t maxsize;
    size_t size;
public:
    LruCache(size_t max_size):maxsize(max_size), size(0){
    };

    void Put(const K& key, const V& value){
        if (pointers.contains(key)) {
            size--;
            data.erase(pointers[key]);
        } else if (size == maxsize){
            size--;
            pointers.erase((*data.begin()).second); // не работает переполнение
            data.pop_front();
        }
        data.push_back({value, key});
        pointers[key] = --data.end();
        size++;
    };

    bool Get(const K& key, V* value){
        if (!pointers.contains(key)) return false; // тут пизда
        *value = (*pointers[key]).first;
        data.erase(pointers[key]);
        data.push_back({*value, key});
        pointers[key] = --data.end();
        return true;
    };
};

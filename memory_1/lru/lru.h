template <typename K, typename V>
class LruCache {
public:
    LruCache(size_t max_size);

    void Put(const K& key, const V& value);

    bool Get(const K& key, V* value);
};

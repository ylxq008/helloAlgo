/**
 * File: hash_map_open_addressing.c
 * Created Time: 2023-10-6
 * Author: lclc6 (w1929522410@163.com)
 */

#include "../utils/common.h"

/* 使用开发地址法来解决哈希冲突*/

/* 开放寻址哈希表 */
struct Pair{
    int key;
    char *val;
};
typedef struct Pair Pair;

struct HashMapOpenAddressing{
    int size;           // 键值对数量   
    int capacity;       // 哈希表容量
    double loadThres;   // 触发扩容的负载因子阈值
    int extendRatio;    // 扩容倍数
    Pair **buckets;     // 桶数组
    Pair *TOMBSTONE;    // 删除标记
};

typedef struct HashMapOpenAddressing HashMapOpenAddressing;

/* 构造方法 */
HashMapOpenAddressing *newHashMapOpenAddressing() {
    HashMapOpenAddressing *hashMap = (HashMapOpenAddressing *)malloc(sizeof(HashMapOpenAddressing));
    hashMap->size = 0;
    hashMap->capacity = 4;
    hashMap->loadThres = 2.0 / 3.0;
    hashMap->extendRatio = 2;
    hashMap->buckets = (Pair **)malloc(sizeof(Pair *) * hashMap->capacity);
    hashMap->TOMBSTONE = (Pair *)malloc(sizeof(Pair));
    hashMap->TOMBSTONE->key = -1;
    hashMap->TOMBSTONE->val = "-1";

    return hashMap;
}

/* 析构方法 */
void delHashMapOpenAddressing(HashMapOpenAddressing *hashMap) {
    for(int i = 0;i < hashMap->capacity;i++) {
        Pair *pair = hashMap->buckets[i];
        if(pair == NULL || pair->key == -1) continue;
        free(pair->val);
    }
    free(hashMap->TOMBSTONE);
    free(hashMap);

    return ;
}

/* 哈希函数 */
int hashFunc(HashMapOpenAddressing *hashMap,int key) {
    return key % hashMap->capacity;
}

/* 负载因子 */
double loadFactor(HashMapOpenAddressing *hashMap) {
    return (double)hashMap->size / hashMap->capacity;
}

/* 搜索 key 对应的桶索引 */
int findBucket(HashMapOpenAddressing *hashMap,int key) {
    int index = hashFunc(hashMap,key);
    int firstTombstone = -1;
    // 线性探测，当遇到空桶时跳出
    while(hashMap->buckets[index] != NULL) {
        // 若遇到 key ，返回对应桶索引
        if(hashMap->buckets[index]->key == key) {
            // 若之前遇到了删除标记，则将键值对移动至该索引
            if(firstTombstone != -1) {
                hashMap->buckets[firstTombstone] = hashMap->buckets[index];
                hashMap->buckets[index] = hashMap->TOMBSTONE;
                return firstTombstone; // 返回移动后的桶索引
            }
            return index; // 返回桶索引
        }
        // 记录遇到的首个删除标记
        if(firstTombstone == -1 && hashMap->buckets[index] == hashMap->TOMBSTONE) {
            firstTombstone = index;
        }
        // 计算桶索引，越过尾部返回o头部
        index = (index + 1) % hashMap->capacity;
    }
    // 若 key 不存在，则返回添加点的索引
    return firstTombstone == -1 ? index : firstTombstone;
}

/* 查询操作 */
char *get(HashMapOpenAddressing *hashMap,int key) {
    // 搜索 key 对应的桶索引
    int index = findBucket(hashMap,key);
    // 若找到键值对，则返回对应 val
    if(hashMap->buckets[index] != NULL && hashMap->buckets[index] != hashMap->TOMBSTONE) {
        return hashMap->buckets[index]->val;
    }
    // 若a键值对不存在，则返回空字符串
    return "";
}

/* 添加操作 */
void put(HashMapOpenAddressing *hashMap,int key,char *val) {
    // 当负载因子超过阈值时，执行扩容
    if(loadFactor(hashMap) > hashMap->loadThres) {
        extend(hashMap);
    }
    // 搜索 key 对应的桶索引
    int index = findBucket(hashMap,key);
    // 若找到键值对，则覆盖 val 并返回
    if(hashMap->buckets[index] != NULL && hashMap->buckets[index] != hashMap->TOMBSTONE) {
        free(hashMap->buckets[index]->val);
        hashMap->buckets[index]->val = (char *)malloc(sizeof(strlen(val + 1)));
        strcpy(hashMap->buckets[index]->val,val);
        hashMap->buckets[index]->val[strlen(val)] = '\0';
        return ;
    }
    // 若键值对不存在，则添加该键值对
    Pair *pair = (Pair *)malloc(sizeof(Pair));
    pair->key = key;
    pair->val = (char *)malloc(sizeof(strlen(val + 1)));
    strcpy(pair->val,val);
    pair->val[strlen(val)] = '\0';

    hashMap->buckets[index] = pair;
    hashMap->size++;
}

/* 删除操作 */
void removeItem(HashMapOpenAddressing *hashMap, int key) {
    // 搜索 key 对应的桶索引
    int index = findBucket(hashMap,key);
    // 若找到键值对，则用删除标记覆盖它
    if(hashMap->buckets[index] != NULL && hashMap->buckets[index] != hashMap->TOMBSTONE) {
        Pair *pair = hashMap->buckets[index];
        free(pair->val);
        free(pair);
        hashMap->buckets[index] = hashMap->TOMBSTONE;
        hashMap->size--;
    }
    return ;
}

/* 扩容哈希表 */
void extend(HashMapOpenAddressing *hashMap) {
    // 暂存原哈希表
    Pair **bucketsTmp = hashMap->buckets;
    int oldCapacity = hashMap->capacity;
    // 初始化扩容后的新哈希表
    hashMap->capacity *= hashMap->extendRatio;
    hashMap->buckets = (Pair **)malloc(sizeof(Pair *) * hashMap->capacity);
    hashMap->size = 0;
    // 将键值对从原哈希表搬运至新哈希表
    for(int i = 0;i < oldCapacity;i++) {
        Pair *pair = bucketsTmp[i];
        if(pair != NULL && pair != hashMap->TOMBSTONE) {
            put(hashMap,pair->key,pair->val);
            free(pair->val);
            free(pair);
        }
    }
    free(bucketsTmp);
    return ;
}

/* 打印哈希表 */
void print(HashMapOpenAddressing *hashMap) {
    for(int i = 0;i < hashMap->capacity;i++) {
        Pair *pair = hashMap->buckets[i];
        if(pair == NULL) {
            printf("NULL\n");
        }else if(pair == hashMap->TOMBSTONE) {
            printf("TOMBSTONE\n");
        }else {
            printf("%d -> %s\n",pair->key,pair->val);
        }
    }
}

int main(int argc, char const *argv[])
{
    HashMapOpenAddressing *map = newHashMapOpenAddressing();
    put(map,0,"0000");
    put(map,5,"hh");
    put(map,1,"lc");
    put(map,9,"hello");
    removeItem(map,5);
    print(map);
    
    delHashMapOpenAddressing(map);
    return 0;
}

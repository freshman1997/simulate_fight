#ifndef __OBJECT_MANANGE_H__
#define __OBJECT_MANANGE_H__
#include <unordered_map>
#include <cassert>
#include "game/object.h"

#define MAX_OBJECT_TYPE     (10000)
#define MAX_OBJECT_PER_TAG  (10000)

// Note. 要求必须要有首个
class ObjectManager
{
public:
    static ObjectManager & get_instance();

public:
    Object * clone_one_object(const std::string &tag)
    {
        return clone_object(tag, false);
    }

    Object * clone_one_clean_object(const std::string &tag)
    {
        return clone_object(tag, true);
    }

    template<class T>
    inline bool register_type(const char *tag)
    {
        static_assert(std::is_base_of<Object, T>::value, "T must base from Object");

        auto it = this->idxs.find(tag);
        if (it != this->idxs.end()) {
            assert(0);
            return false;
        }

        if (this->idxs.size() >= MAX_OBJECT_TYPE) {
            assert(0);
            return false;
        }

        short id = this->idxs[tag];
        this->idle_objects[tag][id] = new T;
        ++this->idxs[tag];
        return true;
    }

    bool release_object(Object *);

    size_t get_using_size() const;
    size_t get_idle_size() const;

    size_t get_tag_idle_size(std::string &tag) const;
    size_t get_tag_using_size(std::string &tag) const;

    void print_info();

private:
    ObjectManager();

    // 尝试 clone 一个对象, 必须要有原型才能 clone
    Object * clone_object(const std::string &tag, bool clean);

private:
    std::unordered_map<std::string, std::unordered_map<int, Object *>> using_objects;
    std::unordered_map<std::string, std::unordered_map<int, Object *>> idle_objects;
    std::unordered_map<std::string, short> idxs;
};

extern bool val;

#define REGISTER_TYPE(_name, type) \
    namespace { \
        bool val = ObjectManager::get_instance().register_type<type>(_name); \
    }

#endif

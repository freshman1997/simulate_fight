#include <iostream>
#include "manager/object_manager.hpp"

ObjectManager::ObjectManager()
{}

ObjectManager & ObjectManager::get_instance()
{
    thread_local static ObjectManager instance;
    return instance;
}

bool ObjectManager::release_object(Object *obj)
{
    if (!obj) {
        return false;
    }

    auto it = this->using_objects.find(obj->tag);
    if (it == this->using_objects.end()) {
        return false;
    }

    auto oit = it->second.find(obj->idx);
    if (oit == it->second.end()) {
        return false;
    }

    it->second.erase(obj->idx);
    this->idle_objects[obj->tag][obj->idx] = obj;

    return true;
}

size_t ObjectManager::get_using_size() const
{
    return this->using_objects.size();
}

size_t ObjectManager::get_idle_size() const
{
    return this->idle_objects.size();
}

size_t ObjectManager::get_tag_idle_size(std::string &tag) const
{
    auto it = this->idle_objects.find(tag);
    if (it == this->idle_objects.end()) {
        return 0;
    }

    return it->second.size();
}

size_t ObjectManager::get_tag_using_size(std::string &tag) const
{
    auto it = this->using_objects.find(tag);
    if (it == this->using_objects.end()) {
        return 0;
    }

    return it->second.size();
}

Object * ObjectManager::clone_object(const std::string &tag, bool clean)
{
    if (this->idle_objects[tag].empty()) {
        if (this->idxs[tag] >= MAX_OBJECT_PER_TAG) {
            // TODO 满了
            return nullptr; 
        }

        if (this->using_objects[tag].empty()) {
            // TODO error log
            return nullptr;
        }

        auto it = this->using_objects.find(tag);
        short id = this->idxs[tag];
        Object *obj = clean ? it->second.begin()->second->clone_and_clean() : it->second.begin()->second->clone();
        if (!obj) {
            return nullptr;
        }

        ++this->idxs[tag];
        it->second[id] = obj;
        
        return obj;
    } else {
        auto it = this->idle_objects.find(tag);
        Object *obj = it->second.begin()->second;
        it->second.erase(it->second.begin());
        this->using_objects[tag][obj->idx] = obj;

        return obj;
    }
}

void ObjectManager::print_info()
{
    using namespace std;
    for (auto &it : using_objects) {
        cout << it.first << " ===> " << it.second.size() << endl;
    }

    for (auto &it : idle_objects) {
        cout << it.first << " ===> " << it.second.size() << endl;
    }
}

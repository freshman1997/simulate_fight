#include "manager/object_manager.hpp"

ObjectManager::ObjectManager()
{}

ObjectManager & ObjectManager::get_instance()
{
    static ObjectManager instance;
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
